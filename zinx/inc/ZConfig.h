#if !defined(ZINX_CONFIG_H)
#define ZINX_CONFIG_H

#include <string>

namespace zinx {
namespace base {

class GlobalConfig {
    GlobalConfig() = delete;
    GlobalConfig(const GlobalConfig&) = delete;
public:
    /// proj logo
    static const std::string logo;

    /// default value: zinx-muduo
    static std::string server_name;

    /// default value: V0.1
    static std::string version;

    /// default value: 127.0.0.1
    static std::string host;

    /// default value: 0
    static uint16_t port;

    /**
     * default value: 3
     * brief:
     *      = 0: single-Reactor mode, Acceptor and IO-handler run in same thread
     *      > 0: mutiple-Reactor mode
     */
    static size_t io_thread_num;

    /// default value: 5
    static size_t worker_thread_num;

    /**
     * default value: 30
     * brief:
     *      = 0: unlimited
     *      > 0: queue size
     */
    static size_t max_task_queue_size;

    static void InitConfig();
};

} // namespace base 
} // namespace zinx 

#endif // ZINX_CONFIG_H
