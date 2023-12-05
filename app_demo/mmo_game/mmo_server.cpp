#include <zinx/inc/ZServer.h>
#include <mmo_game/util/util.h>
#include <mmo_game/api/msg_id.h>
#include <mmo_game/core/Player.h>
#include <mmo_game/core/WorldManager.h>
#include <mmo_game/handler/ChatHandler.h>
#include <mmo_game/handler/MoveHandler.h>

void initPlayer(const zinx::ZinxConnectionPtr& conn) {
    // Create a Player instance
    std::shared_ptr<mmo::Player> p = mmo::CreateNewPlayer(conn, mmo::util::getRandomPoistion());
    
    // set current connection`s properties
    conn->SetContext(static_cast<int32_t>(p->GetPid()));
    
    // Add new player instance to player queue
    mmo::GlobalWorldManager->AddPlayer(p);

    // notify PID to current client
    p->SyncPid();

    // sync self client and surrounding players 
    p->SyncWithSurrounding(*mmo::GlobalWorldManager);
}

void destroyPlayer(const zinx::ZinxConnectionPtr& conn) {
    assert(conn->GetContext().has_value());

    int32_t pid = mmo::util::getPidFromZConnection(conn);
    
    mmo::Player* cur_player = mmo::GlobalWorldManager->GetPlayerByPid(pid);
    // sync other surrounding players 
    cur_player->Disappear(*mmo::GlobalWorldManager);
    // remove disconnected player from player queue
    mmo::GlobalWorldManager->RemovePlayerByPid(pid);
}

int main() {
    std::unique_ptr<zinx::ZinxServer> server = zinx::NewZinxServer();
    server->SetOnConnStart(&initPlayer);
    server->SetOnConnClose(&destroyPlayer);
    server->AddHandler(HANDLER_WORLD_CHAT_PACK_ID, std::make_unique<mmo::ChatHandler>());
    server->AddHandler(HANDLER_MOVE_PACK_ID, std::make_unique<mmo::MoveHandler>());

    mmo::initGlobalWorldManager();
    assert(mmo::GlobalWorldManager.operator bool());
    
    server->ListenAndServe();
}
