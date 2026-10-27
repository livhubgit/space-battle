#pragma once

#include <exception>
#include <unordered_map>
#include <typeindex>
#include <functional>
#include "commands/Command.h"

using Handler = std::function<
    CommandPtr(CommandPtr, const std::exception&)
>;


class ExceptionHandlerStore {
private:
inline static std::unordered_map <
    std::type_index,   
    std::unordered_map< 
            std::type_index,
            Handler  
        >
    >store;
//таблица: пара «тип упавшей команды + тип исключения»  -> обработчик (Handler)
//Handler - функция: получает упавшую команду и исключение, возвращает новую команду

public:    
    static CommandPtr handle(CommandPtr cmd, const std::exception &e);
    static void registerHandler(std::type_index ct, std::type_index et, Handler h);
    static void clear();



};