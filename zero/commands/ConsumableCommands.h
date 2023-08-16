#pragma once

#include <algorithm>
#include <string>
#include <zero/commands/CommandSystem.h>
#include <zero/game/game.h>
#include <zero/ZeroBot.h>

namespace zero {

class RepelCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    auto& bb = bot.execute_ctx.blackboard;
    auto& chat = bot.game->chat;
    //Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

    chat.SendPrivateMessage("Turning repels on.", sender);
    
    bb.Set<bool>("AllowRepel", true);
  }

  CommandAccessFlags GetAccess() { return CommandAccess_All; }
  void SetAccess(CommandAccessFlags flags) { return; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"repel"}; }
  std::string GetDescription() { return "Sets repels on"; }
  int GetSecurityLevel() { return 0; }
};

class RepelOffCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    auto& bb = bot.execute_ctx.blackboard;
    auto& chat = bot.game->chat;
    //Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

    chat.SendPrivateMessage("Turning repels off.", sender);
    
    bb.Set<bool>("AllowRepel", false);
  }

  CommandAccessFlags GetAccess() { return CommandAccess_All; }
  void SetAccess(CommandAccessFlags flags) { return; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"repeloff"}; }
  std::string GetDescription() { return "Sets repels off"; }
  int GetSecurityLevel() { return 0; }
};

class BurstCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    auto& bb = bot.execute_ctx.blackboard;
    auto& chat = bot.game->chat;
    //Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

    chat.SendPrivateMessage("Turning bursts on.", sender);

    bb.Set<bool>("AllowBurst", true);
  }

  CommandAccessFlags GetAccess() { return CommandAccess_All; }
  void SetAccess(CommandAccessFlags flags) { return; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"burst"}; }
  std::string GetDescription() { return "Sets burst on"; }
  int GetSecurityLevel() { return 0; }
};

class BurstOffCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    auto& bb = bot.execute_ctx.blackboard;
    auto& chat = bot.game->chat;
    //Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

    chat.SendPrivateMessage("Turning bursts off.", sender);

    bb.Set<bool>("AllowBurst", false);
  }

  CommandAccessFlags GetAccess() { return CommandAccess_All; }
  void SetAccess(CommandAccessFlags flags) { return; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"burstoff"}; }
  std::string GetDescription() { return "Sets burst off"; }
  int GetSecurityLevel() { return 0; }
};

class DecoyCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    auto& bb = bot.execute_ctx.blackboard;
    auto& chat = bot.game->chat;
    //Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

    chat.SendPrivateMessage("Turning decoys on.", sender);

    bb.Set<bool>("AllowDecoy", true);
  }

  CommandAccessFlags GetAccess() { return CommandAccess_All; }
  void SetAccess(CommandAccessFlags flags) { return; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"decoy"}; }
  std::string GetDescription() { return "Sets decoys on"; }
  int GetSecurityLevel() { return 0; }
};

class DecoyOffCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    auto& bb = bot.execute_ctx.blackboard;
    auto& chat = bot.game->chat;
    //Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

    chat.SendPrivateMessage("Turning decoys off.", sender);

    bb.Set<bool>("AllowDecoy", false);
  }

  CommandAccessFlags GetAccess() { return CommandAccess_All; }
  void SetAccess(CommandAccessFlags flags) { return; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"decoyoff"}; }
  std::string GetDescription() { return "Sets decoys off"; }
  int GetSecurityLevel() { return 0; }
};

class RocketCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    auto& bb = bot.execute_ctx.blackboard;
    auto& chat = bot.game->chat;
    //Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

    chat.SendPrivateMessage("Turning rockets on.", sender);

    bb.Set<bool>("AllowRocket", true);
  }

  CommandAccessFlags GetAccess() { return CommandAccess_All; }
  void SetAccess(CommandAccessFlags flags) { return; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"rocket"}; }
  std::string GetDescription() { return "Sets rockets on"; }
  int GetSecurityLevel() { return 0; }
};

class RocketOffCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    auto& bb = bot.execute_ctx.blackboard;
    auto& chat = bot.game->chat;
    //Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

    chat.SendPrivateMessage("Turning rockets off.", sender);

    bb.Set<bool>("AllowRocket", false);
  }

  CommandAccessFlags GetAccess() { return CommandAccess_All; }
  void SetAccess(CommandAccessFlags flags) { return; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"rocketoff"}; }
  std::string GetDescription() { return "Sets rockets off"; }
  int GetSecurityLevel() { return 0; }
};

class BrickCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    auto& bb = bot.execute_ctx.blackboard;
    auto& chat = bot.game->chat;
    //Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

    chat.SendPrivateMessage("Turning bricks on.", sender);

    bb.Set<bool>("AllowBrick", true);
  }

  CommandAccessFlags GetAccess() { return CommandAccess_All; }
  void SetAccess(CommandAccessFlags flags) { return; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"brick"}; }
  std::string GetDescription() { return "Sets bricks on"; }
  int GetSecurityLevel() { return 0; }
};

class BrickOffCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    auto& bb = bot.execute_ctx.blackboard;
    auto& chat = bot.game->chat;
    //Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

    chat.SendPrivateMessage("Turning bricks off.", sender);

    bb.Set<bool>("AllowBrick", false);
  }

  CommandAccessFlags GetAccess() { return CommandAccess_All; }
  void SetAccess(CommandAccessFlags flags) { return; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"brickoff"}; }
  std::string GetDescription() { return "Sets bricks off"; }
  int GetSecurityLevel() { return 0; }
};

class PortalCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    auto& bb = bot.execute_ctx.blackboard;
    auto& chat = bot.game->chat;
    //Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

    chat.SendPrivateMessage("Turning portals on.", sender);

    bb.Set<bool>("AllowPortal", true);
  }

  CommandAccessFlags GetAccess() { return CommandAccess_All; }
  void SetAccess(CommandAccessFlags flags) { return; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"portal"}; }
  std::string GetDescription() { return "Sets portals on"; }
  int GetSecurityLevel() { return 0; }
};

class PortalOffCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    auto& bb = bot.execute_ctx.blackboard;
    auto& chat = bot.game->chat;
    //Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

    chat.SendPrivateMessage("Turning portals off.", sender);

    bb.Set<bool>("AllowPortal", false);
  }

  CommandAccessFlags GetAccess() { return CommandAccess_All; }
  void SetAccess(CommandAccessFlags flags) { return; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"portaloff"}; }
  std::string GetDescription() { return "Sets portals off"; }
  int GetSecurityLevel() { return 0; }
};

class ThorCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    auto& bb = bot.execute_ctx.blackboard;
    auto& chat = bot.game->chat;
    //Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

    chat.SendPrivateMessage("Turning thors on.", sender);

    bb.Set<bool>("AllowThor", true);
  }

  CommandAccessFlags GetAccess() { return CommandAccess_All; }
  void SetAccess(CommandAccessFlags flags) { return; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"thor"}; }
  std::string GetDescription() { return "Sets thors on"; }
  int GetSecurityLevel() { return 0; }
};

class ThorOffCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    auto& bb = bot.execute_ctx.blackboard;
    auto& chat = bot.game->chat;
    //Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

    chat.SendPrivateMessage("Turning thors off.", sender);

    bb.Set<bool>("AllowThor", false);
  }

  CommandAccessFlags GetAccess() { return CommandAccess_All; }
  void SetAccess(CommandAccessFlags flags) { return; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"thoroff"}; }
  std::string GetDescription() { return "Sets thors off"; }
  int GetSecurityLevel() { return 0; }
};

}  // namespace marvin
