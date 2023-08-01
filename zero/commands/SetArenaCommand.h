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
    std::string c_arg = RemoveWhitespace(arg);

    if (c_arg.empty()) {
      msg = sender + " has sent me to pub.";
      chat.SendMessage(ChatType::Public, msg.c_str());
      // 0xFFFF is what the game probably sends when a player types ?go
      // it's not a garantee to be send to pub 0 because pub 0 could hit the player cap
      // and push players into pub 1
      // this is left up to the zone to handle
      bot.game->connection.SendArenaLogin(player->ship, 0, 1920, 1080, 0xFFFF, "");
    } else if (IsDigit(c_arg)) {
      int num = std::atoi(c_arg.c_str());
      // send bot to a pub arena (pub 0, pub 1, pub 2, etc)
      // for arenas like eg and tw, this just sends you to pub, they don't really allow it
      // for ASSS zones, numbers over 2 byte signed int max get ignored and won't send you anywhere
      if (num >= 0 && num <= 32767) {
        msg = sender + " has sent me to ?go " + c_arg;
        chat.SendMessage(ChatType::Public, msg.c_str());
        bot.game->connection.SendArenaLogin(player->ship, 0, 1920, 1080, num, "");
      } else {
        chat.SendPrivateMessage("That arena is not allowed.", player->id);
      }
    } else {
      msg = sender + " has sent me to ?go " + c_arg;
      chat.SendMessage(ChatType::Public, msg.c_str());
      // 0xFFFD is the flag that allows joining arenas by name, which is entered in the last argument
      bot.game->connection.SendArenaLogin(player->ship, 0, 1920, 1080, 0xFFFD, c_arg.c_str());
    }
  }

  CommandAccessFlags GetAccess() { return CommandAccess_All; }
  void SetAccess(CommandAccessFlags flags) { return; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"go"}; }
  std::string GetDescription(){return "Sends the bot to an arena. Leave argument empty to return to pub."; }
  int GetSecurityLevel() { return 5; }
};

}  // namespace marv.