#pragma once

#include <algorithm>
#include <string>
#include <zero/commands/CommandSystem.h>
#include <zero/game/game.h>
#include <zero/ZeroBot.h>

namespace zero {

class CommandsCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    if (sender.empty()) return;
    //Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());
    //if (!player) return;

    int requester_level = cmd.GetSecurityLevel(sender);
    Commands& commands = cmd.GetCommands();
    // Store a list of executors so multiple triggers linking to the same executor aren't displayed on different lines.
    std::vector<CommandExecutor*> executors;

    // Store the triggers so they can be sorted before output
    struct Trigger {
      CommandExecutor* executor;
      std::string triggers;

      Trigger(CommandExecutor* executor, const std::string& triggers) : executor(executor), triggers(triggers) {}
    };

    std::vector<Trigger> triggers;

    // Loop through every registered command and store the combined triggers
    for (auto& entry : commands) {
      CommandExecutor* executor = entry.second.get();

      if (std::find(executors.begin(), executors.end(), executor) == executors.end()) {
        std::string output;

        if (requester_level >= executor->GetSecurityLevel()) {
          std::vector<std::string> aliases = executor->GetAliases();

          // Combine all the aliases
          for (std::size_t i = 0; i < aliases.size(); ++i) {
            if (i != 0) {
              output += "/";
            }

            output += "-- !" + aliases[i];
          }

          triggers.emplace_back(executor, output);
        }

        executors.push_back(executor);
      }
    }

    // Sort triggers alphabetically
    std::sort(triggers.begin(), triggers.end(),
              [](const Trigger& l, const Trigger& r) { return l.triggers.compare(r.triggers) < 0; });

    // Display the stored triggers
    for (Trigger& trigger : triggers) {
      std::string desc = trigger.executor->GetDescription();
      int security = trigger.executor->GetSecurityLevel();

      std::string output = trigger.triggers + " - " + desc + " [" + std::to_string(security) + "]";

      bot.game->chat.SendPrivateMessage(output.c_str(), sender);
    }
  }

  CommandAccessFlags GetAccess() { return CommandAccess_Private; }
  void SetAccess(CommandAccessFlags flags) { return; }
  std::vector<std::string> GetAliases() { return {"commands", "c"}; }
  std::string GetDescription() { return "Shows available commands"; }
  int GetSecurityLevel() { return 0; }
};

class HelpCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    if (sender.empty()) return;

    Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());
    if (!player) return;

    bot.game->chat.SendPrivateMessage("-- !commands {.c} -- see command list (pm)", sender);
  }

  CommandAccessFlags GetAccess() { return CommandAccess_Private; }
  void SetAccess(CommandAccessFlags flags) { return; }
  std::vector<std::string> GetAliases() { return {"help", "h"}; }
  std::string GetDescription() { return "Helps"; }
  int GetSecurityLevel() { return 0; }
};

class DelimiterCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    if (sender.empty()) return;

    //Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

    bot.game->chat.SendPrivateMessage(
        "The ';' character can be used to send more than one command in a single message.", sender);
    bot.game->chat.SendPrivateMessage("", sender);
    bot.game->chat.SendPrivateMessage("Example:  .setship 2;anchor;setfreq 1", sender);
    bot.game->chat.SendPrivateMessage("", sender);
    bot.game->chat.SendPrivateMessage("The '|' character can be used to buy multiple items in hyperspace.", sender);
    bot.game->chat.SendPrivateMessage("Example: .buy 7|radiating coils|close combat", sender);
  }

  CommandAccessFlags GetAccess() { return CommandAccess_Private; }
  void SetAccess(CommandAccessFlags flags) { return; }
  std::vector<std::string> GetAliases() { return {"delimiter"}; }
  std::string GetDescription() { return "Delimiter usage"; }
  int GetSecurityLevel() { return 0; }
};

class ModListCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    if (sender.empty()) return;

    std::string output;

    for (const auto& entry : cmd.GetOperators()) {
      std::string op = entry.first;
      int level = entry.second;

      output += op + " [" + std::to_string(level) + "] ";
    }

    //Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());
    bot.game->chat.SendPrivateMessage(output.c_str(), sender);
  }

  CommandAccessFlags GetAccess() { return CommandAccess_Private; }
  void SetAccess(CommandAccessFlags flags) { return; }
  std::vector<std::string> GetAliases() { return {"modlist", "ml"}; }
  std::string GetDescription() { return "See who can lock marv"; }
  int GetSecurityLevel() { return 0; }
};

}  // namespace zero