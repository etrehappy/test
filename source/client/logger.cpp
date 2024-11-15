#include "logger.h"
#include "settings.h"

void logger::InitLogging()
{
    namespace bl = boost::log;
    namespace blk = boost::log::keywords;

    std::filesystem::create_directories(settings::kLoggsDir);
    const int rotation_size = 10 * 1024 * 1024;

    const auto time_based_rotation = bl::sinks::file::rotation_at_time_point(0, 0, 0);
    const auto text_format = "[%TimeStamp%] [%Severity%] %Message%";    

    /* INFO */
    boost::log::add_file_log(
        blk::file_name = settings::kLoggsInfoFile
        , blk::rotation_size = rotation_size
        , blk::time_based_rotation = time_based_rotation
        , blk::format = text_format
        , blk::open_mode = std::ios::trunc
        , blk::filter = bl::trivial::severity <= bl::trivial::info
    );

    /* WARNING+ */
    boost::log::add_file_log(
        blk::file_name = settings::kLoggsWarningPlusFile
        , blk::rotation_size = rotation_size
        , blk::time_based_rotation = time_based_rotation
        , blk::format = text_format
        , blk::open_mode = std::ios::trunc
        , blk::filter = bl::trivial::severity >= bl::trivial::warning
    );

    bl::add_common_attributes();
}

void logger::Connect()
{
    BOOST_LOG_TRIVIAL(info) << logger::kInfoConnect << std::endl;
    boost::log::core::get()->flush();
}

void logger::ErrorExeption(const std::exception& e, const std::string& s, int l)
{
    BOOST_LOG_TRIVIAL(error) << logger::kErrorExeption << s << "::" << l << " " << e.what() << std::endl;
    boost::log::core::get()->flush();
}

void logger::ErrorReadSome(const std::string& error)
{
    BOOST_LOG_TRIVIAL(error) << logger::kErrorReadSome << error << std::endl;
    boost::log::core::get()->flush();
}

void logger::ErrorMessageCallbackNotSet(const std::string& function)
{
    BOOST_LOG_TRIVIAL(error) << logger::kErrorMessageCallbackNotSet << function << std::endl;
    boost::log::core::get()->flush();
}

void logger::ErrorParsingMessage()
{
    BOOST_LOG_TRIVIAL(error) << logger::kErrorParsingMessage << std::endl;
    boost::log::core::get()->flush();
}

void logger::ErrorSerializeToString()
{
    BOOST_LOG_TRIVIAL(error) << logger::kErrorSerializeToString << std::endl;
    boost::log::core::get()->flush();
}

void logger::ErrorSend(const std::string& error_text)
{
    BOOST_LOG_TRIVIAL(error) << logger::kErrorSend << error_text << std::endl;
    boost::log::core::get()->flush();
}

void logger::ErrorLocatinReadDb()
{
    BOOST_LOG_TRIVIAL(error) << logger::kErrorLocatinReadDb << std::endl;
    boost::log::core::get()->flush();
}
