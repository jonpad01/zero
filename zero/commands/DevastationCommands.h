#pragma once
#include <algorithm>
#include <string>
#include <zero/commands/CommandSystem.h>
#include <zero/game/game.h>
#include <zero/ZeroBot.h>

namespace zero {

class BDPublicCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    auto& bb = bot.execute_ctx.blackboard;
    auto& chat = bot.game->chat;
    Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

    const std::vector<std::string> commands = {"bdstart, bdstop, bdhold, bdresume"};

    for (std::string command : commands) {
      auto iter = cmd.GetCommands().find(command);
      if (iter != cmd.GetCommands().end()) {
        iter->second->SetAccess(CommandAccess_Public);
      }
    }

    std::string msg = "This bot now only listens to public baseduel commands. (Command sent by: " + sender + ")";
    chat.SendMessage(ChatType::Public, msg.c_str());
  }

  CommandAccessFlags GetAccess() { return CommandAccess_Private; }
  void SetAccess(CommandAccessFlags flags) { return; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"bdpublic"}; }
  std::string GetDescription() { return "Allow the bot to accept public baseduel commands."; }
  int GetSecurityLevel() { return 1; }
};

class BDPrivateCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    auto& bb = bot.execute_ctx.blackboard;
    auto& chat = bot.game->chat;
    Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

    const std::vector<std::string> commands = {"bdstart, bdstop, bdhold, bdresume"};

    for (std::string command : commands) {
      auto iter = cmd.GetCommands().find(command);
      if (iter != cmd.GetCommands().end()) {
        iter->second->SetAccess(CommandAccess_Private);
      }
    }

    std::string msg = "This bot now only listens to private baseduel commands. (Command sent by: " + sender + ")";
    chat.SendMessage(ChatType::Public, msg.c_str());
  }

  CommandAccessFlags GetAccess() { return CommandAccess_Private; }
  void SetAccess(CommandAccessFlags flags) { return; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"bdprivate"}; }
  std::string GetDescription() { return "Set the bot to only allow private baseduel commands."; }
  int GetSecurityLevel() { return 1; }
};


class StartBDCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    auto& bb = bot.execute_ctx.blackboard;
    auto& chat = bot.game->chat;
    Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

    BaseDuelState state = bb.ValueOr<BaseDuelState>("BaseDuelState", BaseDuelState::NotRunning);
    
      if (state == BaseDuelState::Running) {
      chat.SendPrivateMessage("I am currently running a baseduel game.", player->id);
      return;
      } else if (state == BaseDuelState::Paused) {
      chat.SendPrivateMessage("I am currently paused in the middle of a game.", player->id);
      return;
      } else if (state == BaseDuelState::Ending) {
      chat.SendPrivateMessage("I am currently ending a game, please try the command again.", player->id);
      return;
      }

      bb.Set<BaseDuelState>("BaseDuelState", BaseDuelState::Running);
      bb.Set<Area>("WarpToArea", Area::Base);

      std::string msg = "Starting a base duel game. (Command sent by: " + sender + ")";
      chat.SendMessage(ChatType::Public, msg.c_str());
  }

  StartBDCommand() : access(CommandAccess_Private) {}
  CommandAccessFlags GetAccess() { return access; }
  void SetAccess(CommandAccessFlags flags) { access = flags; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"bdstart"}; }
  std::string GetDescription() { return "Start a base duel game."; }
  int GetSecurityLevel() { return 0; }

  private:
  CommandAccessFlags access;
};

class StopBDCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
      auto& bb = bot.execute_ctx.blackboard;
      auto& chat = bot.game->chat;
      Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

      BaseDuelState state = bb.ValueOr<BaseDuelState>("BaseDuelState", BaseDuelState::NotRunning);

    if (state == BaseDuelState::NotRunning || state == BaseDuelState::Ending) {
      chat.SendPrivateMessage("I am not running a baseduel game.", player->id);
    } else if (state != BaseDuelState::Paused) {
      chat.SendPrivateMessage("You must hold the game first before using this command.", player->id);
    } else {
      bb.Set<BaseDuelState>("BaseDuelState", BaseDuelState::Ending);
      std::string msg = "The baseduel game has been stopped. (Command sent by: " + sender + ")";
      chat.SendMessage(ChatType::Public, msg.c_str());
    }
  }

  StopBDCommand() : access(CommandAccess_Private) {}
  CommandAccessFlags GetAccess() { return access; }
  void SetAccess(CommandAccessFlags flags) { access = flags; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"bdstop"}; }
  std::string GetDescription() { return "Stop a base duel game (resets score)."; }
  int GetSecurityLevel() { return 1; }

 private:
  CommandAccessFlags access;
};

class HoldBDCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    auto& bb = bot.execute_ctx.blackboard;
    auto& chat = bot.game->chat;
    Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

    BaseDuelState state = bb.ValueOr<BaseDuelState>("BaseDuelState", BaseDuelState::NotRunning);

      if (state == BaseDuelState::Paused) {
        chat.SendPrivateMessage("I am already holding a baseduel game.", player->id);
        return;
      } else if (state == BaseDuelState::NotRunning || state == BaseDuelState::Ending) {
        chat.SendPrivateMessage("I am not running a baseduel game.", player->id);
        return;
      }

      bb.Set<BaseDuelState>("BaseDuelState", BaseDuelState::Paused);
      std::string msg = "Holding game. (Command sent by: " + sender + ")";
      chat.SendMessage(ChatType::Public, msg.c_str());
  }

  HoldBDCommand() : access(CommandAccess_Private) {}
  CommandAccessFlags GetAccess() { return access; }
  void SetAccess(CommandAccessFlags flags) { access = flags; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"bdhold"}; }
  std::string GetDescription() { return "Hold a base duel game (score is saved)"; }
  int GetSecurityLevel() { return 1; }

  private:
  CommandAccessFlags access;
};

class ResumeBDCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    auto& bb = bot.execute_ctx.blackboard;
    auto& chat = bot.game->chat;
    Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

    BaseDuelState state = bb.ValueOr<BaseDuelState>("BaseDuelState", BaseDuelState::NotRunning);
      
      if (state == BaseDuelState::NotRunning || state == BaseDuelState::Ending) {
        chat.SendPrivateMessage("I am not running a baseduel game.", player->id);
        return;
      }

        else if (state != BaseDuelState::Paused) {
        chat.SendPrivateMessage("I am not holding a baseduel game. (it's currently running)", player->id);
        return;
      }

      bb.Set<BaseDuelState>("BaseDuelState", BaseDuelState::Running);
      std::string msg = "Resuming game. (Command sent by: " + sender + ")";
      chat.SendMessage(ChatType::Public, msg.c_str());
  }

  ResumeBDCommand() : access(CommandAccess_Private) {}
  CommandAccessFlags GetAccess() { return access; }
  void SetAccess(CommandAccessFlags flags) { access = flags; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"bdresume"}; }
  std::string GetDescription() { return "Resume a base duel game"; }
  int GetSecurityLevel() { return 1; }

  private:
  CommandAccessFlags access;
};

}  // namespace marvin