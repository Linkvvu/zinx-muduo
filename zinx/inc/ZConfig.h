#if !defined(ZINX_CONFIG_H)
#define ZINX_CONFIG_H

#include <string>

namespace zinx {
    
class GlobalConfig {
    GlobalConfig() = delete;
    GlobalConfig(const GlobalConfig&) = delete;
public:
    static const std::string logo;
    static std::string server_name;
    static std::string version;
    static std::string host;
    static uint16_t port;

    /*
        = 0: single-Reactor mode, Acceptor and IO-handler run in same thread
        > 0: mulple-Reactor mode
    */
    static size_t io_thread_num;

    
    static size_t worker_thread_num;

    /*
        = 0: unlimited
        > 0: queue size
    */
    static size_t max_task_queue_size;

    static void InitConfig();
};

} // namespace zinx 

#endif // ZINX_CONFIG_H
