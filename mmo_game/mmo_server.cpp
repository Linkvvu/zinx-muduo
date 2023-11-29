#include <zinx/inc/ZServer.h>
#include <mmo_game/handler/WorldChatHandler.h>
#include <mmo_game/core/WorldManager.h>
#include <mmo_game/core/Player.h>
#include <random>

#define HANDLER_WORLD_CHAT_ID 2

mmo::Position getRandomPoistion() {
    std::default_random_engine rand_engine;
    std::uniform_real_distribution<float> random(0.0, 9.0);
    rand_engine.seed(time(nullptr));

    return mmo::Position {
        160+random(rand_engine),    // length
        0,                          // height
        240+random(rand_engine),    // width
        0,                          // angle
    };
}

void initPlayer(const zinx::ZinxConnectionPtr& conn) {
    /* Create a Player instance */
    std::shared_ptr<mmo::Player> p = mmo::CreateNewPlayer(conn, getRandomPoistion());
    
    /* set current connection`s properties */
    conn->SetContext(static_cast<int32_t>(p->GetPid()));
    
    /* Add new player instance to player queue */
    mmo::GlobalWorldManager->AddPlayer(p);

    /* notify PID to current client */
    p->SyncPid();

    p->SyncWithSurrounding(*mmo::GlobalWorldManager.get());
}

void destroyPlayer(const zinx::ZinxConnectionPtr& conn) {
    assert(conn->GetContext().has_value());

    /* get player`s id from context  */
    const auto& ctx  = conn->GetContext();
    int32_t pid;

    try {
        pid = std::any_cast<int32_t>(ctx);
    } catch (const std::bad_any_cast& e) {
        /* Failed to fetch context */
        LOG_ERROR << e.what();
        /// FIXME: @c abort() instead of return
        return;
    }

    mmo::Player* cur_player = mmo::GlobalWorldManager->GetPlayerByPid(pid);
    cur_player->Offline(*mmo::GlobalWorldManager.get());
    /* remove disconnected player from player queue */
    mmo::GlobalWorldManager->RemovePlayerByPid(pid);
}

int main() {
    std::unique_ptr<zinx::ZinxServer> server = zinx::NewZinxServer();
    server->SetOnConnStart(&initPlayer);
    server->SetOnConnClose(&destroyPlayer);
    server->AddHandler(HANDLER_WORLD_CHAT_ID, std::make_unique<mmo::WorldChatHandler>());
    server->ListenAndServe();
}
