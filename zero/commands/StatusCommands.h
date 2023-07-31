
#pragma once

#include <algorithm>
#include <string>
#include <zero/commands/CommandSystem.h>
#include <zero/game/game.h>
#include <zero/ZeroBot.h>
#include <zero/Common.h>

namespace zero {

class MultiCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    auto& bb = bot.execute_ctx.blackboard;
    auto& chat = bot.game->chat;
    Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

    chat.SendPrivateMessage("Turning on multifire.", player->id);
    
    bb.Set<bool>("AllowMultiFire", true);
  }

  CommandAccessFlags GetAccess() { return CommandAccess_Private; }
  void SetAccess(CommandAccessFlags flags) { return; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"multi"}; }
  std::string GetDescription() { return "Sets multifire on"; }
  int GetSecurityLevel() { return 0; }
};

class MultiOffCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    auto& bb = bot.execute_ctx.blackboard;
    auto& chat = bot.game->chat;
    Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

    chat.SendPrivateMessage("Turning off multifire.", player->id);
    
    bb.Set<bool>("AllowMultiFire", false);
  }

  CommandAccessFlags GetAccess() { return CommandAccess_Private; }
  void SetAccess(CommandAccessFlags flags) { return; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"multioff"}; }
  std::string GetDescription() { return "Sets multifire off"; }
  int GetSecurityLevel() { return 0; }
};

class CloakCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    auto& bb = bot.execute_ctx.blackboard;
    auto& chat = bot.game->chat;
    Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

    chat.SendPrivateMessage("Now allowing cloaking.", player->id);
    
    bb.Set<bool>("AllowCloak", true);
  }

  CommandAccessFlags GetAccess() { return CommandAccess_Private; }
  void SetAccess(CommandAccessFlags flags) { return; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"cloak"}; }
  std::string GetDescription() { return "Sets cloak on"; }
  int GetSecurityLevel() { return 0; }
};

class CloakOffCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    auto& bb = bot.execute_ctx.blackboard;
    auto& chat = bot.game->chat;
    Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

    chat.SendPrivateMessage("Turning off cloaking.", player->id);
    
    bb.Set<bool>("AllowCloak", false);
  }

  CommandAccessFlags GetAccess() { return CommandAccess_Private; }
  void SetAccess(CommandAccessFlags flags) { return; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"cloakoff"}; }
  std::string GetDescription() { return "Sets cloak off"; }
  int GetSecurityLevel() { return 0; }
};

class StealthCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    auto& bb = bot.execute_ctx.blackboard;
    auto& chat = bot.game->chat;
    Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

    chat.SendPrivateMessage("Now allowing stealth.", player->id);
    
    bb.Set<bool>("AllowStealth", true);
  }

  CommandAccessFlags GetAccess() { return CommandAccess_Private; }
  void SetAccess(CommandAccessFlags flags) { return; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"stealth"}; }
  std::string GetDescription() { return "Sets stealth on"; }
  int GetSecurityLevel() { return 0; }
};

class StealthOffCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    auto& bb = bot.execute_ctx.blackboard;
    auto& chat = bot.game->chat;
    Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

    chat.SendPrivateMessage("Turning off stealth.", player->id);
    
    bb.Set<bool>("AllowStealth", false);
  }

  CommandAccessFlags GetAccess() { return CommandAccess_Private; }
  void SetAccess(CommandAccessFlags flags) { return; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"stealthoff"}; }
  std::string GetDescription() { return "Sets stealth off"; }
  int GetSecurityLevel() { return 0; }
};

class XRadarCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    auto& bb = bot.execute_ctx.blackboard;
    auto& chat = bot.game->chat;
    Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

    chat.SendPrivateMessage("Now allowing XRadar.", player->id);
    
    bb.Set<bool>("AllowXRadar", true);
  }

  CommandAccessFlags GetAccess() { return CommandAccess_Private; }
  void SetAccess(CommandAccessFlags flags) { return; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"xradar"}; }
  std::string GetDescription() { return "Sets XRadar on"; }
  int GetSecurityLevel() { return 0; }
};

class XRadarOffCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    auto& bb = bot.execute_ctx.blackboard;
    auto& chat = bot.game->chat;
    Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

    chat.SendPrivateMessage("Turning off XRadar.", player->id);
    
    bb.Set<bool>("AllowXRadar", false);
  }

  CommandAccessFlags GetAccess() { return CommandAccess_Private; }
  void SetAccess(CommandAccessFlags flags) { return; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"xradaroff"}; }
  std::string GetDescription() { return "Sets XRadar off"; }
  int GetSecurityLevel() { return 0; }
};

class AntiWarpCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    auto& bb = bot.execute_ctx.blackboard;
    auto& chat = bot.game->chat;
    Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

    chat.SendPrivateMessage("Now allowing AntiWarp.", player->id);
    
    bb.Set<bool>("AllowAntiWarp", true);
  }

  CommandAccessFlags GetAccess() { return CommandAccess_Private; }
  void SetAccess(CommandAccessFlags flags) { return; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"anti"}; }
  std::string GetDescription() { return "Sets AntiWarp on"; }
  int GetSecurityLevel() { return 0; }
};

class AntiWarpOffCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    auto& bb = bot.execute_ctx.blackboard;
    auto& chat = bot.game->chat;
    Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

    chat.SendPrivateMessage("Turning off AntiWarp.", player->id);
    
    bb.Set<bool>("AllowAntiWarp", false);
  }

  CommandAccessFlags GetAccess() { return CommandAccess_Private; }
  void SetAccess(CommandAccessFlags flags) { return; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"antioff"}; }
  std::string GetDescription() { return "Sets AntiWarp off"; }
  int GetSecurityLevel() { return 0; }
};

}  // namespace marvin