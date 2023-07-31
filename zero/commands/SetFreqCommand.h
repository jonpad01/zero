#pragma once

#include <algorithm>
#include <string>
#include <zero/commands/CommandSystem.h>
#include <zero/game/game.h>
#include <zero/ZeroBot.h>
#include <zero/Common.h>

namespace zero {

class SetFreqCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    auto& bb = bot.execute_ctx.blackboard;
    auto& chat = bot.game->chat;
    Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

    if (arg.empty() || !IsDigit(arg)) {
      SendUsage(chat, player->id);
      return;
    }

    uint16_t freq = atoi(arg.c_str());

    if (freq >= 0 && freq < 100) {
      std::string msg = "=" + arg;
      chat.SendMessage(ChatType::Public, msg.c_str());
      chat.SendPrivateMessage("If you don't see me change freq, either the balancer stopped me, or I jumped back automatically.", player->id);
    } else {
      SendUsage(chat, player->id);
    }
  }

  void SendUsage(ChatController& chat, uint16_t sender) {
    chat.SendPrivateMessage("Invalid selection. !setfreq [freq]", sender);
  }

  CommandAccessFlags GetAccess() { return CommandAccess_All; }
  void SetAccess(CommandAccessFlags flags) { return; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"setfreq", "sf"}; }
  std::string GetDescription() { return "Sets the bot to a public frequency"; }
  int GetSecurityLevel() { return 0; }
};

}  // namespace marvin
