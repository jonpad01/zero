
#pragma once

#include <algorithm>
#include <string>
#include <zero/commands/CommandSystem.h>
#include <zero/game/game.h>
#include <zero/ZeroBot.h>
#include <zero/Common.h>


namespace zero {

class SayCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    const char* kFilters[] = {"?password", "?passwd", "?squad"};

    for (size_t i = 0; i < ZERO_ARRAY_SIZE(kFilters); ++i) {
      if (arg.find(kFilters[i]) != std::string::npos) {
        Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

        if (player) {
          bot.game->chat.SendPrivateMessage("Message filtered.", player->id);
        }

        return;
      }
    }

    bot.game->chat.SendMessage(ChatType::Public, arg.c_str());
  }

  CommandAccessFlags GetAccess() { return CommandAccess_Private; }
  void SetAccess(CommandAccessFlags flags) { return; }
  std::vector<std::string> GetAliases() { return {"say"}; }
  std::string GetDescription() { return "Repeats a message publicly"; }
  int GetSecurityLevel() { return 10; }
};

}  // namespace zero