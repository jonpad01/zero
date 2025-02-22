#include "ChatController.h"

#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <zero/game/Camera.h>
#include <zero/game/Clock.h>
#include <zero/game/Logger.h>
#include <zero/game/Platform.h>
#include <zero/game/PlayerManager.h>
#include <zero/game/ShipController.h>
#include <zero/game/net/Connection.h>
#include <zero/game/net/PacketDispatcher.h>

namespace zero {

static void OnChatPacketRaw(void* user, u8* packet, size_t size) {
  ChatController* controller = (ChatController*)user;

  controller->OnChatPacket(packet, size);
}

ChatController::ChatController(PacketDispatcher& dispatcher, Connection& connection, PlayerManager& player_manager)
    : connection(connection), player_manager(player_manager) {
  dispatcher.Register(ProtocolS2C::Chat, OnChatPacketRaw, this);
}

void ChatController::SendMessage(ChatType type, const char* mesg) {
  ChatEntry entry;

  strcpy(entry.message, mesg);
  entry.sound = 0x00;
  entry.type = type;

  outbound_msgs.emplace_back(entry);
}

void ChatController::SendPrivateMessage(const char* mesg, const char* sender) {
  ChatEntry entry;

  strcpy(entry.message, mesg);
  strcpy(entry.sender, sender);
  entry.sound = 0x00;
  entry.type = ChatType::Private;

  outbound_msgs.emplace_back(entry);
}

void ChatController::SendMessage(ChatType type, const std::string& mesg) {
  ChatEntry entry;

  strcpy(entry.message, mesg.c_str());
  entry.sound = 0x00;
  entry.type = type;

  outbound_msgs.emplace_back(entry);
}

void ChatController::SendPrivateMessage(const std::string& mesg, const std::string& sender) {
  ChatEntry entry;

  strcpy(entry.message, mesg.c_str());
  strcpy(entry.sender, sender.c_str());
  entry.sound = 0x00;
  entry.type = ChatType::Private;

  outbound_msgs.emplace_back(entry);
}

void ChatController::SendPrivateMessage(const std::string& mesg, uint16_t sender) {
  ChatEntry entry;

  const Player* player = player_manager.GetPlayerById(sender);

  strcpy(entry.message, mesg.c_str());
  if (player) strcpy(entry.sender, player->name);
  entry.sound = 0x00;
  entry.type = ChatType::Private;

  outbound_msgs.emplace_back(entry);
 }

void ChatController::SendPrivateMessage(const char* mesg, uint16_t sender) {
  ChatEntry entry;

  const Player* player = player_manager.GetPlayerById(sender);

  strcpy(entry.message, mesg);
  if (player) strcpy(entry.sender, player->name);
  entry.sound = 0x00;
  entry.type = ChatType::Private;

  outbound_msgs.emplace_back(entry);
 }

void ChatController::SendQueuedMessage() {
  // TODO: Check if this works on subgame, if not then just fall back to old method for those servers.
  //
  // This should be set to whatever the zone config is set to.
  // TODO: Set different value based on joined server.
  constexpr u32 kFloodLimit = 4;

  while (!outbound_msgs.empty()) {
    Tick current_tick = GetCurrentTick();

    s32 d = TICK_DIFF(current_tick, last_check_tick) / 100;

    if (d > 0) {
      // TODO: This could be more accurate if synced with the server by listening to reliable message packets of the
      // chat messages that were sent. It's technically possible to flood if theres any ping spikes if using local count
      // instead of server.
      sent_message_count >>= d;
      last_check_tick = current_tick;
    }

    // Keep the sent message count right below flood limit
    if (sent_message_count < kFloodLimit - 1) {
      ChatEntry msg = outbound_msgs[0];
      outbound_msgs.pop_front();

      // TODO: Grab pid from player name instead of stored pid
      uint16_t pid = 0;
      if (msg.type == ChatType::Private) {
        const Player* player = player_manager.GetPlayerByName(msg.sender);
        // scrap this message, the player probably left the arena
        if (!player) continue;
        pid = player->id;
      }

      u8 data[kMaxPacketSize];
      NetworkBuffer buffer(data, kMaxPacketSize);
      size_t size = strlen(msg.message) + 1;

      buffer.WriteU8(0x06);
      buffer.WriteU8((u8)msg.type);
      buffer.WriteU8(0x00);  // Sound
      buffer.WriteU16(pid);
      buffer.WriteString(msg.message, size);

      connection.packet_sequencer.SendReliableMessage(connection, buffer.data, buffer.GetSize());
      ++sent_message_count;
    } else {
      break;
    }
  }
}

Player* ChatController::GetBestPlayerNameMatch(char* name, size_t length) {
  Player* best_match = nullptr;

  // Loop through each player looking at the first 'length' characters of their name.
  // If they match up to the length then add them as a candidate.
  // If they match up to the length and the name is exactly the same length as the check name then return that one.
  for (size_t i = 0; i < player_manager.player_count; ++i) {
    Player* p = player_manager.players + i;

    bool is_match = true;

    for (size_t j = 0; j < ZERO_ARRAY_SIZE(p->name) && j < length; ++j) {
      char p_curr = tolower(p->name[j]);
      char n_curr = tolower(name[j]);

      if (p_curr != n_curr) {
        is_match = false;
        break;
      }
    }

    if (is_match) {
      best_match = p;

      // If they match up until the length of the check name and they are the same length then it must be exact
      if (strlen(p->name) == length) {
        return p;
      }
    }
  }

  return best_match;
}

inline int GetShipStatusPercent(u32 upgrade, u32 maximum, u32 current) {
  if (upgrade == 0) return 100;

  u32 maximum_upgrades = maximum / upgrade;
  u32 current_upgrades = current / upgrade;

  return (current_upgrades * 100) / maximum_upgrades;
}

void ChatController::Update(float dt) {
  
 // uint64_t now = time.GetTime();
 // uint64_t diff = now - last_update_timestamp;

 // if (diff > 1) {
 //  decay -= diff;
 //  last_update_timestamp = now;
 // }
  
 // if (decay < 0) decay = 0;

  SendQueuedMessage();

  // using a count instead of clear, because new messages can
  // be push into the list before this update runs
  if (entries_read > 0) {
    for (std::size_t i = 0; i < entries_read; i++) {
      recent_chat.pop_back();
    }
    entries_read = 0;
  }
}

char GetChatTypePrefix(ChatType type) {
  static const char kPrefixes[] = {'A', ' ', ' ', 'T', 'O', 'P', 'W', 'R', 'E', 'C'};

  u8 index = (u8)type;
  if (index >= 0 && index <= (u8)ChatType::Channel) {
    return kPrefixes[index];
  }

  if (type == ChatType::Fuchsia) {
    return 'F';
  }

  return ' ';
}

void ChatController::OnChatPacket(u8* packet, size_t size) {
  ChatType type = (ChatType) * (packet + 1);
  u8 sound = *(packet + 2);
  u16 sender_id = *(u16*)(packet + 3);

  ChatEntry* entry = PushEntry((char*)packet + 5, size - 5, type);

  ChatEntry otherEntry;
  strcpy(otherEntry.message, (char*)packet + 5);
  otherEntry.type = type;
  otherEntry.sound = sound;

  Player* player = player_manager.GetPlayerById(sender_id);
  if (player) {
    memcpy(entry->sender, player->name, 20);
    strcpy(otherEntry.sender, player->name);

    char prefix = GetChatTypePrefix(type);

    if (entry->type == ChatType::Private && player->id != player_manager.player_id) {
      history.InsertRecent(player->name);
    }

    if (type == ChatType::RemotePrivate || type == ChatType::Arena || type == ChatType::RedWarning ||
        type == ChatType::RedError) {
      Log(LogLevel::Info, "%c %s", prefix, entry->message);
    } else {
      Log(LogLevel::Info, "%c %s> %s", prefix, entry->sender, entry->message);
    }
  }

  if (entry->type == ChatType::RemotePrivate) {
    if (entry->message[0] == '(') {
      char* sender = entry->message + 1;
      char* current = entry->message;

      while (*current++) {
        if (*current == ')') {
          char name[20];

          sprintf(name, "%.*s", (u32)(current - sender), sender);

          history.InsertRecent(name);
          break;
        }
      }
    }
  }

  entry->sound = sound;

  recent_chat.push_back(otherEntry);
  if (recent_chat.size() > 64) recent_chat.pop_front();
}

ChatEntry* ChatController::PushEntry(const char* mesg, size_t size, ChatType type) {
  ChatEntry* entry = entries + (entry_index++ % ZERO_ARRAY_SIZE(entries));

  memcpy(entry->message, mesg, size);
  entry->sender[0] = 0;
  entry->type = type;
  entry->sound = 0;

  return entry;
}

void ChatController::AddMessage(ChatType type, const char* fmt, ...) {
  ChatEntry* entry = PushEntry("", 0, type);

  va_list args;
  va_start(args, fmt);

  vsprintf(entry->message, fmt, args);

  va_end(args);
}

void PrivateHistory::InsertRecent(char* name) {
  RecentSenderNode* node = recent;
  RecentSenderNode* alloc_node = nullptr;

  size_t count = 0;

  while (node) {
    ++count;

    if (strcmp(node->name, name) == 0) {
      // Name is already in the list so set this one to the allocation node
      alloc_node = node;

      // Set the count high so it doesn't try to allocate
      count = ZERO_ARRAY_SIZE(nodes);
      break;
    }

    alloc_node = node;
    node = node->next;
  }

  if (count < ZERO_ARRAY_SIZE(nodes)) {
    // Allocate off the nodes until the recent list is fully populated
    node = nodes + count;
  } else {
    // Pop the last node off or the node that was a match for existing name
    RemoveNode(alloc_node);

    node = alloc_node;
  }

  strcpy(node->name, name);
  node->next = recent;
  recent = node;
}

void PrivateHistory::RemoveNode(RecentSenderNode* node) {
  RecentSenderNode* current = recent;

  while (current) {
    if (current->next == node) {
      current->next = node->next;
      break;
    }

    current = current->next;
  }

  if (node != nullptr && node == recent) {
    recent = node->next;
  }
}

char* PrivateHistory::GetPrevious(char* current) {
  RecentSenderNode* node = recent;

  while (current && node) {
    if (strcmp(node->name, current) == 0) {
      RecentSenderNode* next = node->next;

      // If this is the last node in the list then return the first one
      if (!next) {
        next = recent;
      }

      return next->name;
    }

    node = node->next;
  }

  return recent ? recent->name : nullptr;
}

// return a reference to recent, then set the number of entries that was read
const std::deque<ChatEntry>& ChatController::GetRecentChat() {
  entries_read = recent_chat.size();
  return recent_chat;
}

void ChatController::ClearRecentChat() {
  recent_chat.clear();
}

}  // namespace zero
