#ifndef ZERO_CHATCONTROLLER_H_
#define ZERO_CHATCONTROLLER_H_

#include <deque>
#include <zero/Types.h>
#include <zero/game/InputState.h>
#include <zero/Time.h>

namespace zero {

struct Connection;
struct PacketDispatcher;
struct Player;
struct PlayerManager;

enum class ChatType {
  Arena,
  PublicMacro,
  Public,
  Team,
  OtherTeam,
  Private,
  RedWarning,
  RemotePrivate,
  RedError,
  Channel,
  Fuchsia = 79
};

struct ChatEntry {
  ChatType type;
  u8 sound;
  char sender[20];
  char message[520];
};

struct RecentSenderNode {
  char name[24];

  struct RecentSenderNode* next;
};

struct PrivateHistory {
  RecentSenderNode* recent = nullptr;

  RecentSenderNode nodes[5];

  void InsertRecent(char* name);
  char* GetPrevious(char* current);
  void RemoveNode(RecentSenderNode* node);
};

struct ChatController {
  Connection& connection;
  PlayerManager& player_manager;

  size_t entry_index = 0;
  ChatEntry entries[64] = {};
  

  PrivateHistory history;

  ChatController(PacketDispatcher& dispatcher, Connection& connection, PlayerManager& player_manager);

  void Update(float dt);
  ChatEntry* PushEntry(const char* mesg, size_t size, ChatType type);
  void AddMessage(ChatType type, const char* fmt, ...);

  void SendMessage(ChatType type, const char* mesg);
  void SendPrivateMessage(const char* mesg, u16 pid);
  void SendMessage(ChatType type, const std::string& mesg);
  void SendPrivateMessage(const std::string& mesg, u16 pid);
  
  const std::deque<ChatEntry>& GetRecentChat();
  void ClearRecentChat();

  void OnChatPacket(u8* packet, size_t size);

  Player* GetBestPlayerNameMatch(char* name, size_t length);

  private:

   void SendQueuedMessage();

  struct OutBoundEntry {
    ChatType type;
    u8 sound;
    uint16_t sender;
    char message[520];
  };

  Time time;
  std::deque<OutBoundEntry> outbound_msgs;
  uint64_t outbound_timestamp = 0;

  std::deque<ChatEntry> recent_chat;
  std::size_t entries_read = 0;
};

}  // namespace zero

#endif
