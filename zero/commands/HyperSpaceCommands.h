#pragma once

#include <algorithm>
#include <string>
#include <zero/commands/CommandSystem.h>
#include <zero/game/game.h>
#include <zero/ZeroBot.h>
#include <zero/Common.h>

namespace zero {

class HSShipStatusCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    auto& bb = bot.execute_ctx.blackboard;
    auto& chat = bot.game->chat;
    Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

    Ship ship = bot.game->ship_controller.ship;

    if (!arg.empty() && isdigit(arg[0])) {
      int num = arg[0] - '0';
      if (num >= 1 && num <= 8) {
        ship = Ship(num - 1);
      } else {
        SendUsage(chat, player->id);
      }
    }

    bb.Set<ItemTransaction>("ItemTransaction", ItemTransaction::ListItems);
    bb.Set<Ship>("ItemTransactionShip", ship);
    bb.Set<bool>("ItemTransactionMessageSent", false);
    // TODO: move timing into the behavior node after it sends a buy message
    bb.Set<uint64_t>("ItemTransactionAllowedTime", 5000);
    bb.Set<uint16_t>("ItemTransactionSenderID", player->id);
    bb.Set<uint64_t>("ItemTransactionTimeStamp", bot.time.GetTime());
  }

  void SendUsage(ChatController& chat, uint16_t sender) {
    chat.SendPrivateMessage("Private command only (.shipstatus) (.shipstatus 1-8)", sender);
  }

  CommandAccessFlags GetAccess() { return CommandAccess_Private; }
  void SetAccess(CommandAccessFlags flags) { return; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"shipstatus"}; }
  std::string GetDescription() { return "Prints a list of items the bot currently owns (.items 1 lists items for warbird)."; }
  int GetSecurityLevel() { return 0; }
};

class HSBuyCommand : public CommandExecutor {
  public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    auto& bb = bot.execute_ctx.blackboard;
    auto& chat = bot.game->chat;
    Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());
     
    std::vector<std::string> args = SplitString(arg, "|");
    // adding to the item list in case buy was used in a delimiter

    Ship ship = bot.game->ship_controller.ship;
    std::vector<std::string> list;

    if (!args.empty() && isdigit(args[0][0])) {
      int num = args[0][0] - '0';
      if (num >= 1 && num <= 8) {
        ship = Ship(num - 1);
        args.erase(args.begin());
      } else {
        SendUsage(chat, player->id);
      }
    }

     for (const std::string& item : args) {
      list.emplace_back(item);
     }

     bb.Set<std::vector<std::string>>("ItemTransactionList", list);
     bb.Set<std::size_t>("ItemTransactionCount", list.size());
     bb.Set<uint64_t>("ItemTransactionAllowedTime", 5000);
     bb.Set<uint16_t>("ItemTransactionSenderID", player->id);
     bb.Set<uint64_t>("ItemTransactionTimeStamp", bot.time.GetTime());
     bb.Set<ItemTransaction>("ItemTransaction", ItemTransaction::Buy);
     bb.Set<bool>("ItemTransactionMessageSent", false);
     bb.Set<bool>("ItemTransactionSetShipSent", false);
     bb.Set<Ship>("ItemTransactionShip", ship);

  }

   void SendUsage(ChatController& chat, uint16_t sender) {
     chat.SendPrivateMessage("Example format: .buy 2|close combat|radiating coils", sender);
  }

  CommandAccessFlags GetAccess() { return CommandAccess_All; }
  void SetAccess(CommandAccessFlags flags) { return; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"buy"}; }
  std::string GetDescription() { return "Tell the bot to buy items (use  a '|' delimiter to buy multiple items)."; }
  int GetSecurityLevel() { return 0; }
};

class HSSellCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
     auto& bb = bot.execute_ctx.blackboard;
     auto& chat = bot.game->chat;
     Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

     std::vector<std::string> args = SplitString(arg, "|");
    
     Ship ship = bot.game->ship_controller.ship;
     std::vector<std::string> list;

     if (!args.empty() && isdigit(args[0][0])) {
      int num = args[0][0] - '0';
      if (num >= 1 && num <= 8) {
        ship = Ship(num - 1);
        args.erase(args.begin());
      } else {
        SendUsage(chat, player->id);
      }
     }

     for (const std::string& item : args) {
      list.emplace_back(item);
     }
  
     bb.Set<std::vector<std::string>>("ItemTransactionList", list);
     bb.Set<std::size_t>("ItemTransactionCount", list.size());
     bb.Set<uint64_t>("ItemTransactionAllowedTime", 5000);
     bb.Set<uint16_t>("ItemTransactionSenderID", player->id);
     bb.Set<uint64_t>("ItemTransactionTimeStamp", bot.time.GetTime());
     bb.Set<ItemTransaction>("ItemTransaction", ItemTransaction::Sell);
     bb.Set<bool>("ItemTransactionMessageSent", false);
     bb.Set<bool>("ItemTransactionSetShipSent", false);
     bb.Set<Ship>("ItemTransactionShip", ship);

  }

  void SendUsage(ChatController& chat, uint16_t sender) {
     chat.SendPrivateMessage("Example format: .sell 2|close combat|radiating coils", sender);
  }

  CommandAccessFlags GetAccess() { return CommandAccess_All; }
  void SetAccess(CommandAccessFlags flags) { return; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"sell"}; }
  std::string GetDescription() { return "Example format: /.sell 2|close combat|radiating coils"; }
  int GetSecurityLevel() { return 0; }
};

class HSFlagCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
     auto& bb = bot.execute_ctx.blackboard;
     auto& chat = bot.game->chat;
     Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

   
    chat.SendPrivateMessage("Switching to flagging.", player->id);
    
    bb.Set<bool>("AllowFlagging", true);
  }

  CommandAccessFlags GetAccess() { return CommandAccess_All; }
  void SetAccess(CommandAccessFlags flags) { return; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"flag"}; }
  std::string GetDescription() { return "Allows bot to jump on to a flag team"; }
  int GetSecurityLevel() { return 0; }
};

class HSFlagOffCommand : public CommandExecutor {
 public:
  void Execute(CommandSystem& cmd, ZeroBot& bot, const std::string& sender, const std::string& arg) override {
    auto& bb = bot.execute_ctx.blackboard;
    auto& chat = bot.game->chat;
    Player* player = bot.game->player_manager.GetPlayerByName(sender.c_str());

    chat.SendPrivateMessage("I will stop flagging.", player->id);
    
    bb.Set<bool>("AllowFlagging", false);
  }

  CommandAccessFlags GetAccess() { return CommandAccess_All; }
  void SetAccess(CommandAccessFlags flags) { return; }
  CommandFlags GetFlags() { return CommandFlag_Lockable; }
  std::vector<std::string> GetAliases() { return {"flagoff"}; }
  std::string GetDescription() { return "Disallows bot to jump on to a flag team"; }
  int GetSecurityLevel() { return 0; }
};

}  // namespace marvin