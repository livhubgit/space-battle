#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <any>
#include <atomic>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "Ioc/Ioc.h"
#include "scope/InitCommand.h"
#include "commands/Command.h"

using ::testing::_;
using ::testing::Invoke;
using ::testing::MockFunction;

namespace
{
    using Args = std::vector<std::any>;

    class CommandMockForIoc : public Command
    {
    public:
        MOCK_METHOD(void, execute, (), (override));
    };

    // Регистрация зависимости: тоже через Resolve (ключ "IoC.Register")
    void Register(const std::string &key, DependencyStratgey f)
    {
        Ioc::Resolve<CommandPtr>("IoC.Register", Args{key, f})->execute();
    }

    void SetCurrent(std::shared_ptr<Scope> s)
    {
        Ioc::Resolve<CommandPtr>("IoC.Scope.Current.Set", Args{s})->execute();
    }

    void ClearCurrent()
    {
        Ioc::Resolve<CommandPtr>("IoC.Scope.Current.Clear", Args{})->execute();
    }

    std::shared_ptr<Scope> NewScope(std::shared_ptr<Scope> parent = nullptr)
    {
        return parent
                   ? Ioc::Resolve<std::shared_ptr<Scope>>("IoC.Scope.Create", Args{parent})
                   : Ioc::Resolve<std::shared_ptr<Scope>>("IoC.Scope.Create", Args{});
    }
}

// У каждого теста свой скоуп: корень не засоряется, состояние не течёт между тестами
class IocTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        InitCommand().execute();
        testScope = NewScope();
        SetCurrent(testScope);
    }
    void TearDown() override
    {
        ClearCurrent();
        std::lock_guard<std::mutex> l(InitCommand::registryMutex);
        InitCommand::scopeRegistry.clear();
    }

    std::shared_ptr<Scope> testScope;
};

TEST_F(IocTest, ResolveCallsRegisteredFactoryOnceWithArguments)
{
    MockFunction<std::any(Args)> factory; // мокированная функция
    Args received;                        // вектор аргументов
    EXPECT_CALL(factory, Call(_)).WillOnce(Invoke([&](Args a)
                                                  {
        received = a;
        return std::any(42); })); // ожидаем что factory вызовется один раз , когда вызовется - выполнится лямбда

    Register("t.answer", factory.AsStdFunction()); // регистрируем зависимость - если кто товызовет t.answer  - вызови factory(как std функцию)

    int v = Ioc::Resolve<int>("t.answer", Args{7, std::string("x")}); // запрос зависимостиб действие

    EXPECT_EQ(v, 42);                                        // ожидаем что v= 42
    ASSERT_EQ(received.size(), 2u);                          // проверяем что аргументов ровно 2
    EXPECT_EQ(std::any_cast<int>(received[0]), 7);           // приводим первый аргумент  к инт и проверяем что он 7
    EXPECT_EQ(std::any_cast<std::string>(received[1]), "x"); // приводим второй  аргумент  к string и проверяем что он "x"
}

TEST_F(IocTest, RegistrationTakesEffectOnlyAfterCommandExecute)
{

    MockFunction<std::any(Args)> factory;
    EXPECT_CALL(factory, Call(_)).Times(0); // при регистрации фабрика не вызывается

    CommandPtr cmd = Ioc::Resolve<CommandPtr>(
        "IoC.Register", Args{std::string("t.lazy"), DependencyStratgey(factory.AsStdFunction())}); // регистрируем t.lazy c стратегией зависимости factory

    EXPECT_ANY_THROW(Ioc::Resolve<int>("t.lazy", Args{})); // команда ещё не выполнена, при попытке ресолва t.lazy - ожидаемо исключение
    cmd->execute();                                        // теперь запись есть
    // (вызывать t.lazy не нужно: Times(0) подтверждает, что фабрика не вызывалась, но запись зарегистрирована , потом её можно будет вызвать)
}

TEST_F(IocTest, UnknownDependencyThrows)
{
    // EXPECT_THROW(Ioc::Resolve<int>("t.no.such.key", Args{}), std::logic_erro);
    EXPECT_ANY_THROW(Ioc::Resolve<int>("t.no_such_key", Args{})); // ключа нет ни в одном скоупе — ожидаем исключение при попытке разрешения
}

TEST_F(IocTest, WrongRequestedTypeThrowsBadAnyCast)
{
    Register("t.int", [](Args) -> std::any
             { return 5; }); // регистрируем зависимость  фабрика которой возвращает 5 типа int

    EXPECT_THROW(Ioc::Resolve<std::string>("t.int", Args{}), std::bad_any_cast); // если  попытаемся не верно указать запрашиваемый тип при ресолве - ожидаем исключение
}

TEST_F(IocTest, NewKindsOfDependenciesNeedNoChangesInIoc)
{
    // Устойчивость интерфейса: новые виды зависимостей без правки Ioc

    auto mock = std::make_shared<CommandMockForIoc>();
    EXPECT_CALL(*mock, execute()).Times(1);

    // разные зависимости одним методом
    Register("t.number", [](Args) -> std::any
             { return 3.5; });
    Register("t.text", [](Args a) -> std::any
             { return std::any_cast<std::string>(a[0]) + "!"; });
    Register("t.command", [mock](Args) -> std::any
             {
        CommandPtr c = mock;                    // точный тип в any: CommandPtr
        return c; });

    EXPECT_DOUBLE_EQ(Ioc::Resolve<double>("t.number", Args{}), 3.5);
    EXPECT_EQ(Ioc::Resolve<std::string>("t.text", Args{std::string("hi")}), "hi!");
    Ioc::Resolve<CommandPtr>("t.command", Args{})->execute(); // и проверится что Times(1) отработало
}

// Скоупы тесты

TEST_F(IocTest, ChildScopeSeesParentDependency)
{

    // до начала  отработал SetUp фикстура и testScope создан и установлен как currentScope
    Register("t.base", [](Args) -> std::any
             { return 10; });

    SetCurrent(NewScope()); // родитель = текущий (testScope)

    EXPECT_EQ(Ioc::Resolve<int>("t.base", Args{}), 10);

    // просим  дать  "t.base".
    // Контейнер смотрит в currentScope. Там ничего нет.
    // DependencyResolver спрашивает: «Кто твой родитель?»
    // childScope отвечает: «Мой родитель — testScope».
    // Резолвер идет в testScope, находит там "t.base", вызывает лямбду, получает 10 и возвращает.
}

TEST_F(IocTest, ChildOverridesParentWithoutTouchHim)
{
    MockFunction<std::any(Args)> parentFactory, childFactory;
    EXPECT_CALL(parentFactory, Call(_)).WillOnce(Invoke([](Args)
                                                        { return std::any(1); }));
    EXPECT_CALL(childFactory, Call(_)).WillOnce(Invoke([](Args)
                                                       { return std::any(2); }));

    Register("t.value", parentFactory.AsStdFunction());
    auto child = NewScope();

    // Создаем новый скоуп (его родитель — testScope) и делаем его текущим.
    SetCurrent(child);
    Register("t.value", childFactory.AsStdFunction()); // свой "t.value" только в child

    EXPECT_EQ(Ioc::Resolve<int>("t.value", Args{}), 2); // отвечает child
    SetCurrent(testScope);                              // переключили scope
    EXPECT_EQ(Ioc::Resolve<int>("t.value", Args{}), 1); // родитель не изменился
}

TEST_F(IocTest, ChildScopesWithSameParentAreIsolated)
{
    auto a = NewScope(testScope);
    auto b = NewScope(testScope);

    SetCurrent(a);
    Register("t.only.a", [](Args) -> std::any
             { return 1; });
    SetCurrent(b);

    EXPECT_ANY_THROW(Ioc::Resolve<int>("t.only.a", Args{}));
    SetCurrent(a);
    EXPECT_EQ(Ioc::Resolve<int>("t.only.a", Args{}), 1);

    // скоупы c одним родителем но разные -  не влияют др на др
}

// Многопоточные тесты

TEST_F(IocTest, ThreadsWorkInTheirOwnScopes)
{
    constexpr int kThreads = 8, kIterations = 2000;
    std::atomic<bool> go{false};
    std::vector<int> wrong(kThreads, 0);
    std::vector<std::thread> threads;

    for (int t = 0; t < kThreads; ++t)
    {
        threads.emplace_back([&, t]
                             {
            while (!go) std::this_thread::yield();          // стартуем одновременно
            SetCurrent(NewScope(testScope));                // у потока свой скоуп
            Register("t.id", [t](Args) -> std::any { return t; }); //регистрируем зависимость в каждом потоке свою
            for (int i = 0; i < kIterations; ++i)
                if (Ioc::Resolve<int>("t.id", Args{}) != t) ++wrong[t]; // если возвращает не свой id - считаем ошибку для данного потока
            ClearCurrent(); });
    }
    go = true;
    for (auto &th : threads)
        th.join(); // ждем чтобы все потоки завершились

    for (int t = 0; t < kThreads; ++t)
        EXPECT_EQ(wrong[t], 0) << "thread " << t << " saw a foreign value"; // если ошибки были - выведется в каком потоке
}

TEST_F(IocTest, CurrentScopeIsPerThread)
{
    // thread_local  - поэтому у каждого потока свой scoup
    Register("t.who", [](Args) -> std::any
             { return std::string("main"); }); // зависимость попадает в testScope

    bool keyVisibleInOtherThread = true;
    std::thread th([&]
                   {
                       try
                       {
                           Ioc::Resolve<std::string>("t.who", Args{});
                       }
                       catch (const std::exception &)
                       {
                           keyVisibleInOtherThread = false;
                       } // новый поток не унаследовал currentScope -> ищем в rootScope, там t.who нет поэтому исключение
                   });
    th.join();

    EXPECT_FALSE(keyVisibleInOtherThread);
    EXPECT_EQ(Ioc::Resolve<std::string>("t.who", Args{}), "main");
}

TEST_F(IocTest, NamedScopesAreIsolated)
{
    Ioc::Resolve<CommandPtr>("Scopes.New", Args{std::string("game1")})->execute();
    Ioc::Resolve<CommandPtr>("Scopes.New", Args{std::string("game2")})->execute();

    Ioc::Resolve<CommandPtr>("Scopes.Current", Args{std::string("game1")})->execute();
    Register("t.value", [](Args) -> std::any
             { return std::any(1); });
    EXPECT_EQ(Ioc::Resolve<int>("t.value", {}), 1);

    Ioc::Resolve<CommandPtr>("Scopes.Current", Args{std::string("game2")})->execute();
    Register("t.value", [](Args) -> std::any
             { return std::any(2); });
    EXPECT_EQ(Ioc::Resolve<int>("t.value", {}), 2);

    Ioc::Resolve<CommandPtr>("Scopes.Current", Args{std::string("game1")})->execute();
    EXPECT_EQ(Ioc::Resolve<int>("t.value", {}), 1);
}
