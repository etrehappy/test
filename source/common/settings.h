#pragma once

#include <filesystem>

namespace settings
{

    const static unsigned short kBufferSize = 1024;      

    const static std::filesystem::path kLoggsDir{"./logs"};
    const static std::filesystem::path kLoggsInfoFile{kLoggsDir / "./info.log"};
    const static std::filesystem::path kLoggsWarningPlusFile{kLoggsDir /
        "./warnings_and_errors.log"};
    const static std::filesystem::path kFilesDir{"./files"};


////////////////////////////////////////////////////////////////////////////////
namespace client
{
    const static char* kTestHost{"127.0.0.1"};
    const static char* kPortStr{"65500"};

    const static char* kDatabaseFile{"./files/client_database.db3"};

} /* namespace settings::client */

////////////////////////////////////////////////////////////////////////////////
namespace server
{
    const static unsigned short kPort = 65500;

    /*!
    * @brief There is one DatabaseHandler for all clients in the MVP version so the
    *		EventDispatcher uses the kDatabaseClientId to find the handler.
    */
    const static std::string kDatabaseEventRoutingKey = "db_rounting_key";

    const static char* kDatabaseFile{"./files/database.db3"};

} /* namespace settings::server */
        


} /* namespace settings */