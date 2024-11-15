#pragma once

#include <filesystem>
#include <exception>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include "settings.h"
#include "event.h"

#define MVP_LOG_EXEPTION(e) logger::ErrorExeption(e, __func__, __LINE__);

namespace logger
{
    void InitLogging();

    void ServerStart();
    void ClientConnect(const std::string&);
    void AuthenticateAttempt(const std::string&);
    void ClientDisconnect(const std::string&);

    void ErrorFatal();
    void ErrorExeption(const std::exception&, const std::string&, int);
    void ErrorEmptyQueue();
    void ErrorHandlerNotFound(EventType);
    void ErrorSerrialisation(EventType);
    void ErrorDynamicPointerCast(EventType);
    void ErrorSqlExecuteStep(EventType);
    
    static const char* kInfoServerStart = "Сервер запущен и ожидает подключения...";
    static const char* kInfoClientConnect = "Клиент подключен: ";
    static const char* kInfoAuthenticateAttempt = "Попытка аутентификации пользователя: ";

    

    /*GameHandler, NetworkHandler uses:*/
    static const char* kErrExeptEmptyVariable = "не получены данные";

    /*Logger uses:*/
    static const char* kErrorFatal = "Неизвестная ошибка";
    static const char* kErrorExeption = "Исключение в ";
    static const char* kErrorEmptyQueue = "Попытка извлечь событие из пустой очереди";
    static const char* kErrorHandlerNotFound = "Не найден обработчик события # ";
    static const char* kErrorSerrialisation = "Не удалось сериализовать событие # ";
    static const char* kErrorDynamicPointerCast = "Ошибка преобразования shared_ptr";
    static const char* kErrorSqlExecuteStep = "Ошибка при чтении данных из таблицы по событию ";

    static const char* kClientDisconnect = "Клиент отключен: ";

}//logger