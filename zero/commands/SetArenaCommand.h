#pragma once

#include <algorithm>
#include <string>
#include <zero/commands/CommandSystem.h>
#include <zero/game/game.h>
#include <zero/ZeroBot.h>
#include <zero/Common.h>

namespace zero {

class SetArenaCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    auto& bb = bot.execute_ctx.blackboard;
    auto& chat = bot.game->chat;
    Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());
    std::string msg;

    if (arg.empty()) {
      msg = sender + " has sent me to pub.";
      chat.SendMessage(ChatType::Public, msg.c_str());
      //chat.SendMessage(ChatType::Public, "?go");
      return;
    } else {
      msg = sender + " has sent me to ?go " + arg;
      chat.SendMessage(ChatType::Public, msg.c_str());
      // TODO: cant get arena request with chat message
      msg = "?go " + arg;
     //chat.SendMessage(ChatType::Public, msg.c_str());
      //bot.game->connection.
    }
  }

  CommandAccessFlags GetAccess() { return CommandAccess_All; }
  void SetAccess(CommandAccessFlags flags) { return; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"setarena"}; }
  std::string GetDescription(){return "Sends the bot to an arena. Leave argument empty to return to pub."; }
  int GetSecurityLevel() { return 5; }
};

}  // namespace marv.