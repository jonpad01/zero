#pragma once

#include <zero/ZeroBot.h>
#include <zero/behavior/BehaviorTree.h>
#include <zero/game/Game.h>

namespace zero {
namespace behavior {

struct BuyShipsNode : public BehaviorNode {
  BuyShipsNode() {}

  ExecuteResult Execute(ExecuteContext& ctx) override {
    auto& bot = ctx.bot;
    auto& bb = ctx.blackboard;
    auto& game = ctx.bot->game;
    auto self = ctx.bot->game->player_manager.GetSelf();

    bool on_safe = game->GetMap().GetTileId(self->position) == kTileSafeId;

    uint64_t last_run = bb.ValueOr<uint64_t>("buy_ships_node", 0);

    if (bot->time.GetTime() < last_run + 5000 || !on_safe) {
      return ExecuteResult::Failure;
    }
    bb.Set<uint64_t>("buy_ships_node", bot->time.GetTime());

    std::vector<std::string> buy_list;
    const std::vector<std::string> ships = {"warbird", "javelin", "spider",    "leviathan",
                                            "terrier", "weasel",  "lancaster", "shark"};

    for (const ChatEntry& entry : game->chat.GetRecentChat()) {

      if (entry.type != ChatType::Arena) continue;
      std::string_view msg(entry.message);

      if (msg.size() < 2 || msg[0] != '|' || msg[1] != ' ') continue;

      // on the correct message, process it
      std::size_t check_pos = 3;

      for (std::size_t i = 0; i < ships.size(); i++) {
        if (msg[check_pos] == ' ') {
          buy_list.emplace_back(ships[i]);
        }
        check_pos += ships[i].size() + 3;
      }
      break;
    }


    #if 0
    // get last 10 messages
    std::size_t count = 10;
    std::size_t start = game->chat.entry_index;
    std::size_t end = game->chat.entry_index - count;
    // the index was less than the array size, so just set the end to 0
    if (start < count) end = 0;

    
    // message format for every ship owned:
    // | Warbird | Javelin | Spider | Leviathan | Terrier | Weasel | Lancaster | Shark |
    // when player doesnt own a spider:
    // | Warbird | Javelin |        | Leviathan | Terrier | Weasel | Lancaster | Shark |

    // loop through recent chat
    for (size_t i = start; i > end; --i) {
      size_t index = (i - 1) % ZERO_ARRAY_SIZE(game->chat.entries);
      ChatEntry* entry = game->chat.entries + index;

      if (entry->type != ChatType::Arena) continue;
      std::string msg(entry->message);

      if (msg.size() < 2 || msg[0] != '|' || msg[1] != ' ') continue;

      // on the correct message, process it
      std::size_t check_pos = 3;

      for (std::size_t i = 0; i < ships.size(); i++) {
        if (msg[check_pos] == ' ') {
          buy_list.emplace_back(ships[i]);
        }
        check_pos += ships[i].size() + 3;
      }
    }
    #endif
    if (buy_list.empty()) return ExecuteResult::Failure;

    // send a buy command
    std::string buy_msg = "?";

    for (std::string ship : buy_list) {
      buy_msg += "|buy " + ship;
    }

    game->chat.SendMessage(ChatType::Public, buy_msg.c_str());

    return ExecuteResult::Failure;
  }
};








}
}  // namespace zero
