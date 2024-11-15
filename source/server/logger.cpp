#include "logger.h"


void logger::InitLogging() 
{
    std::filesystem::create_directories(settings::kLoggsDir);
    const int rotation_size = 10 * 1024 * 1024;

    const auto time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0);
    const auto text_format = "[%TimeStamp%] [%Severity%] %Message%";

    // Логи для уровня info и выше
    boost::log::add_file_log(
        boost::log::keywords::file_name = settings::kLoggsInfoFile,
        boost::log::keywords::rotation_size = rotation_size,
        boost::log::keywords::time_based_rotation = time_based_rotation,
        boost::log::keywords::format = text_format,
        boost::log::keywords::open_mode = std::ios::trunc,
        boost::log::keywords::filter = boost::log::trivial::severity <= boost::log::trivial::info
    );

    // Логи для уровня warning и выше
    boost::log::add_file_log(
        boost::log::keywords::file_name = settings::kLoggsWarningPlusFile,
        boost::log::keywords::rotation_size = rotation_size,
        boost::log::keywords::time_based_rotation = time_based_rotation,
        boost::log::keywords::format = text_format,
        boost::log::keywords::open_mode = std::ios::trunc,
        boost::log::keywords::filter = boost::log::trivial::severity >= boost::log::trivial::warning
    );

    boost::log::add_common_attributes();
}

void logger::ServerStart()
{
    BOOST_LOG_TRIVIAL(info) << kInfoServerStart << std::endl;
    boost::log::core::get()->flush();
}

void logger::ClientConnect(const std::string& id)
{
    BOOST_LOG_TRIVIAL(info) << logger::kInfoClientConnect << id;
    boost::log::core::get()->flush();
}

void logger::ClientDisconnect(const std::string& id)
{
    BOOST_LOG_TRIVIAL(info) << logger::kClientDisconnect << id;
    boost::log::core::get()->flush();
}

void logger::ErrorFatal()
{
    BOOST_LOG_TRIVIAL(fatal) << logger::kErrorFatal << std::endl;
    boost::log::core::get()->flush();
}

void logger::ErrorExeption(const std::exception& e, const std::string& s, int l)
{
    BOOST_LOG_TRIVIAL(error) << logger::kErrorExeption << s << "::" << l << " " << e.what() << std::endl;
    boost::log::core::get()->flush();
}

void logger::ErrorEmptyQueue()
{
    BOOST_LOG_TRIVIAL(error) << logger::kErrorEmptyQueue << std::endl;
    boost::log::core::get()->flush();
}

void logger::ErrorHandlerNotFound(EventType t)
{
    BOOST_LOG_TRIVIAL(error) << logger::kErrorHandlerNotFound << static_cast<unsigned short >(t) <<std::endl;
    boost::log::core::get()->flush();
}

void logger::ErrorSerrialisation(EventType t)
{
    BOOST_LOG_TRIVIAL(error) << logger::kErrorSerrialisation << static_cast<unsigned short>(t) << std::endl;
    boost::log::core::get()->flush();
}

void logger::AuthenticateAttempt(const std::string& login)
{
    BOOST_LOG_TRIVIAL(info) << logger::kInfoAuthenticateAttempt << login;
    boost::log::core::get()->flush();
}

void logger::ErrorDynamicPointerCast(EventType t)
{
    BOOST_LOG_TRIVIAL(error) << logger::kErrorDynamicPointerCast << static_cast<unsigned short>(t) << std::endl;
    boost::log::core::get()->flush();
}

void logger::ErrorSqlExecuteStep(EventType t)
{
    BOOST_LOG_TRIVIAL(error) << logger::kErrorSqlExecuteStep << static_cast<unsigned short>(t) << std::endl;
    boost::log::core::get()->flush();
}