#pragma once

#include <functional>
#include <vector>
#include <any>
#include <string>
#include <stdexcept>
#include <memory>



class Ioc
{
public:
    static std::function<std::any(std::string, std::vector<std::any>)> _strategy;

    template <typename T>
    static T Resolve(std::string dependency, std::vector<std::any> args)
    {
        return std::any_cast<T>(_strategy(dependency, args));
    }
};

/// <summary>
/// Разрешение зависимости.
/// </summary>
/// <typeparam name="T">Ожидаемый тип объекта, получаемого в результате разрешения зависимости.
/// Если полученный объект невозможно привести в запрашиваемому типу, то выбрасывается исключение
/// <see cref="System.InvalidCastException"/>
/// </typeparam>
/// <param name="dependency">Строковое имя разрешаемой зависимости. В реализации контейнера
/// по умолчанию определенп только одна зависимость "Update Ioc Resolve Dependency Strategy",
/// которая позволяет переопределить стратегию разрешения зависимостей по-умолчанию.</param>
/// <param name="args">Произвольное количество аргументов, которые получает на вход стратегия
/// разрешения зависимостей. Для переопределения стратегии разрешения зависимостей по-умолчанию
/// на вход подается лямбда функция типа Func<Func<string, object[], object>, Func<string, object[], object> >,
/// которая на вход принимает текущую стратегию разрешения зависмисостей типа Func<string, object[], object>,
/// на выходе возвращает новую стратегию типа Func<string, object[], object>.
/// </param>
/// <returns>Объект, полученный в результате разрешения зависимости.
/// Если указана несуществующая зависимость, то выбрасывается исключение
/// <see cref="System.ArgumentException"/>
/// </returns>