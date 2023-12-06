#if !defined(MMO_API_MSG_ID_H)
#define MMO_API_MSG_ID_H

#define SYNC_PID_PACK_ID            1   // 用于同步在线的玩家
#define BROADCAST_PACK_ID           200 // 广播聊天信息、初始坐标、移动坐标
#define SYNC_LEAVE_PACK_ID          201 // 用于同步离线(消失)的玩家
#define SYNC_PLAYERS_PACK_ID        202 // 上线时将周围玩家信息同步给自己

#define HANDLER_WORLD_CHAT_PACK_ID  2
#define HANDLER_MOVE_PACK_ID        3

#define BC_TALK_FIELD               1
#define BC_POS_FIELD                2   // 代表通知初始的坐标
#define BC_MOVE_FIELD               4   // 代表通知移动后的坐标 

#endif // MMO_API_MSG_ID_H