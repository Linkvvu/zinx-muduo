#include <zinx/inc/ZServer.h>
#include <zinx/inc/ZRouter.h>
#include <zinx/inc/ZPacket_LTD.h>
#include <zinx/inc/ZDecoder_LTD.h>

using namespace zinx;

ZinxServer::ZinxServer(const muduo::InetAddr& addr, const std::string& name) 
    : AbstractServer(addr, name)
    , workerPool_(std::make_unique<muduo::ThreadPool>("worker-pool of" + name))
{
    // sets the config of worker-pool
    workerPool_->SetMaxQueueSize(base::GlobalConfig::max_task_queue_size);
    workerPool_->Start(static_cast<int>(base::GlobalConfig::worker_thread_num));
    // registers ZinxRouter and ZinxDecoder
    AbstractServer::SetRouter(std::make_unique<ZinxRouter>());
    AbstractServer::SetDecoder(std::make_unique<ZinxDecoder_LTD>());
}
