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
        LOG_WARN << "does't find config file<zinx_config.json>, use default config!";
        return;
    }

    json configs;

    try {
        configs = json::parse(ifs);
    } catch (const std::exception& e) {
        LOG_ERROR << "Failed to parse config, will use default config, detail: " << e.what();
        ifs.close();
        return;
    }

    auto parseConfigValue = [&](const std::string& key, auto& configVariable) {
        try {
            if (configs.find(key) != configs.end())
                configVariable = configs[key];
        } catch (const json::exception& e) {
            LOG_ERROR << "Failed to parse config for " << key << ", will use default config for this field, detail: " << e.what();
        }
    };

    parseConfigValue("server_name", server_name);
    parseConfigValue("version", version);
    parseConfigValue("host", host);
    parseConfigValue("port", port);
    parseConfigValue("io_thread_num", io_thread_num);
    parseConfigValue("worker_thread_num", worker_thread_num);
    parseConfigValue("max_task_queue_size", max_task_queue_size);

    ifs.close();
}

} // namespace zinx