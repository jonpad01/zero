#pragma once

#include <algorithm>
#include <string>
#include <zero/commands/CommandSystem.h>
#include <zero/game/game.h>
#include <zero/ZeroBot.h>
#include <zero/Common.h>

namespace zero {

class LockFreqCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    auto& bb = bot.execute_ctx.blackboard;
    auto& chat = bot.game->chat;
    Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

    chat.SendPrivateMessage("My frequency is now locked.", player->id);
    
    bb.Set<bool>("FrequencyLock", true);
  }

  CommandAccessFlags GetAccess() { return CommandAccess_All; }
  void SetAccess(CommandAccessFlags flags) { return; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"lockfreq", "lf"}; }
  std::string GetDescription() { return "Locks frequency"; }
  int GetSecurityLevel() { return 1; }
};

class UnlockFreqCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    auto& bb = bot.execute_ctx.blackboard;
    auto& chat = bot.game->chat;
    Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

    chat.SendPrivateMessage("My frequency is now unlocked.", player->id);
    
    bb.Set<bool>("FrequencyLock", false);
  }

  CommandAccessFlags GetAccess() { return CommandAccess_All; }
  void SetAccess(CommandAccessFlags flags) { return; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"unlockfreq", "uf"}; }
  std::string GetDescription() { return "Unlocks frequency"; }
  int GetSecurityLevel() { return 1; }
};

}  // namespace marvin
