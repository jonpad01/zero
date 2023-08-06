#pragma once

#include <algorithm>
#include <string>
#include <zero/commands/CommandSystem.h>
#include <zero/game/game.h>
#include <zero/ZeroBot.h>
#include <zero/Common.h>

namespace zero {

class SetShipCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    auto& bb = bot.execute_ctx.blackboard;
    auto& chat = bot.game->chat;
    Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

    if (arg.empty() || arg < "1" || arg < "9" || arg.size() > 1) {
      SendUsage(chat, player->id);
      return;
    }

    int ship = atoi(arg.c_str()); 

    if (ship == 9) {
      bb.Clear();

      chat.SendPrivateMessage("My behaviors are also reset when sent to spec", player->id);
    } else {
      chat.SendPrivateMessage("Ship selection recieved.", player->id);
    }

    // bot.game->connection.SendShipRequest(ship - 1);
    bb.Set<int>("request_ship", ship - 1);
  }

  void SendUsage(ChatController& chat, uint16_t sender) {
    chat.SendPrivateMessage("Invalid selection. !setship [shipNumber]", sender);
  }

  CommandAccessFlags GetAccess() { return CommandAccess_All; }
  void SetAccess(CommandAccessFlags flags) { return; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"setship", "ss"}; }
  std::string GetDescription() { return "Sets the ship (9 = spec)"; }
  int GetSecurityLevel() { return 0; }
};

}  // namespace marvin
