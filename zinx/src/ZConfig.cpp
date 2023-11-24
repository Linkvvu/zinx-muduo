#ifdef ZINX_CLEAR_ERRMSG
    # define JSON_DIAGNOSTICS 1 // for clear exception message of json lib
#endif
#include <nlohmann/json.hpp>
#include <muduo/base/Logging.h>
#include <zinx/inc/ZConfig.h>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

namespace zinx {
const std::string GlobalConfig::logo = R"(
 _____    _                      __  ___            __            
/__  /   (_)____   _  __        /  |/  /__  __ ____/ /__  __ ____ 
  / /   / // __ \ | |/_/______ / /|_/ // / / // __  // / / // __ \
 / /__ / // / / /_>  < /_____// /  / // /_/ // /_/ // /_/ // /_/ /
/____//_//_/ /_//_/|_|       /_/  /_/ \__,_/ \__,_/ \__,_/ \____/ 
                                                                                 
)"; 

std::string GlobalConfig::server_name = "zinx-muduo";
std::string GlobalConfig::version = "V0.1";
std::string GlobalConfig::host = "127.0.0.1";
uint16_t GlobalConfig::port = 0; // select by OS
size_t GlobalConfig::io_thread_num = 3;
size_t GlobalConfig::worker_thread_num = 5;
size_t GlobalConfig::max_task_queue_size = 30;

void GlobalConfig::InitConfig() {
    std::ifstream ifs("zinx_config.json");
    if (!ifs.is_open()) {
        LOG_WARN << "does`t find config file<zinx_config.json>, use default config!";
        return ;
    }

    json configs;

    try {
        configs = json::parse(ifs);
    } catch (const std::exception& e) {
        LOG_ERROR << "Failed to parse config, will use default config, detail: " << e.what();
        ifs.close();
        return;
    }

    try {
        if (configs.find("server_name") != configs.end())
            server_name = configs["server_name"];
    } catch (const json::exception& e) {
        LOG_ERROR << "Failed to parse config, will use default config, detail: " << e.what();
    }

    try {
        if (configs.find("version") != configs.end())
            version = configs["version"];
    } catch (const json::exception& e) {
        LOG_ERROR << "Failed to parse config, will use default config, detail: " << e.what();
    }

    try {
        if (configs.find("host") != configs.end())
            host = configs["host"];
    } catch (const json::exception& e) {
        LOG_ERROR << "Failed to parse config, will use default config, detail: " << e.what();
    }
    
    try {
        if (configs.find("port") != configs.end())
            port = configs["port"];
    } catch (const json::exception& e) {
        LOG_ERROR << "Failed to parse config, will use default config, detail: " << e.what();
    }

    try {
        if (configs.find("io_thread_num") != configs.end())
            io_thread_num = configs["io_thread_num"];
    } catch (const json::exception& e) {
        LOG_ERROR << "Failed to parse config, will use default config, detail: " << e.what();
    }

    try {
        if (configs.find("worker_thread_num") != configs.end())
            worker_thread_num = configs["worker_thread_num"];
    } catch (const json::exception& e) {
        LOG_ERROR << "Failed to parse config, will use default config, detail: " << e.what();
    }

    try {
        if (configs.find("max_task_queue_size") != configs.end())
            max_task_queue_size = configs["max_task_queue_size"];
    } catch (const json::exception& e) {
        LOG_ERROR << "Failed to parse config, will use default config, detail: " << e.what();
    }
        
    ifs.close();
}

} // namespace zinx