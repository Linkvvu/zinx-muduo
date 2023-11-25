#include <muduo/base/Logging.h>
#include <zinx/inc/ZConfig.h>
#include <zinx/inc/ZRouter.h>
// #include <zinx/inc/ZPacket.h>
#include <zinx/inc/ZPacket_LTD.h>

using namespace zinx;

void ZinxRouter::RouteAndHandle(RequestContext& req) const {
    assert(dynamic_cast<const ZinxPacket_LTD*>(req.GetPacket()) != nullptr);

    /// FIXME: use dynamic_cast for the safe down-cast
    const ZinxPacket_LTD* z_packet = static_cast<const ZinxPacket_LTD*>(req.GetPacket());
    auto it = mapper_.find(z_packet->GetId());
    if (it == mapper_.end()) {
        LOG_WARN << "Doesn`t find a available handler for request from [" << req.GetConnection()->GetRemoteAddr().GetIpPort()
            << "], detail: " << "zinx packet id=" << z_packet->GetId();
        return;
    }

    zinx::Handler* handler = it->second.get();

    /* handle by work-threads*/
    workerThreads_.Run(
        [req, handler]() mutable {  // catch by value to ensure adequate lifecycle 
            handler->PreHandle(req);
            handler->Handle(req);
            handler->PostHandle(req);
        }
    );
}

bool ZinxRouter::AddHandler(uint32_t id, std::unique_ptr<Handler> && handler) {
    const auto it = mapper_.find(id);
    if (it == mapper_.end()) {
        mapper_[id] = std::move(handler);  // should update handler ?
        return false;
    } 
    mapper_[id] = std::move(handler);
    return true;
}

void ZinxRouter::InitWorkerConfig() {
    /// TODO:
    /// set MaxQueueSize and workers number in ThreadPool from config file

    // workerThreads_.SetMaxQueueSize();

    // workerThreads_.SetThreadInitCallback();
    workerThreads_.Start(static_cast<int>(GlobalConfig::worker_thread_num));
    workerThreads_.SetMaxQueueSize(GlobalConfig::max_task_queue_size);
}