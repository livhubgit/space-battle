#include "ExceptionHandlerStore.h"

CommandPtr ExceptionHandlerStore::handle(CommandPtr cmd, const std::exception &e) 
{
    std::type_index ct = typeid(*cmd);
    std::type_index et = typeid(e); 
    if (store.find(ct) != store.end() && store.at(ct).find(et) != store.at(ct).end()) {
        return store.at(ct).at(et)(cmd, e);
    }
    return nullptr;
}



void ExceptionHandlerStore::registerHandler(std::type_index ct, std::type_index et, Handler h)
{
    store[ct][et] = h;
}


void ExceptionHandlerStore::clear()
{
    store.clear();
}



