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

    bb.Set<ItemTransaction>("item_transaction", ItemTransaction::ListItems);
    bb.Set<Ship>("transaction_ship", ship);
    //bb.Set<bool>("transaction_message_sent", false);
    // TODO: move timing into the behavior node after it sends a buy message
    //bb.Set<uint64_t>("ItemTransactionAllowedTime", 5000);
    bb.Set<uint16_t>("transaction_sender_id", player->id);
    //bb.Set<uint64_t>("ItemTransactionTimeStamp", bot.time.GetTime());
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

    int ship = bot.game->player_manager.GetSelf()->ship;
    std::vector<std::string> list;

    if (!args.empty() && isdigit(args[0][0])) {
      int num = args[0][0] - '0';
      if (num >= 1 && num <= 8) {
        ship = num - 1;
        args.erase(args.begin());
      } else {
        SendUsage(chat, player->id);
      }
    }

     for (const std::string& item : args) {
      list.emplace_back(item);
     }

     bb.Set<std::vector<std::string>>("buy_list", list);
     //bb.Set<uint64_t>("ItemTransactionAllowedTime", 5000);
     bb.Set<uint16_t>("transaction_sender_id", player->id);
     //bb.Set<uint64_t>("ItemTransactionTimeStamp", bot.time.GetTime());
     bb.Set<ItemTransaction>("item_transaction", ItemTransaction::Buy);
     //bb.Set<bool>("ItemTransactionMessageSent", false);
     //bb.Set<bool>("ItemTransactionSetShipSent", false);
     bb.Set<int>("transaction_ship", ship);

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
    
     int ship = bot.game->player_manager.GetSelf()->ship;
     std::vector<std::string> list;

     if (!args.empty() && isdigit(args[0][0])) {
      int num = args[0][0] - '0';
      if (num >= 1 && num <= 8) {
        ship = num - 1;
        args.erase(args.begin());
      } else {
        SendUsage(chat, player->id);
      }
     }

     for (const std::string& item : args) {
      list.emplace_back(item);
     }
  
     bb.Set<std::vector<std::string>>("sell_list", list);
     //bb.Set<uint64_t>("ItemTransactionAllowedTime", 5000);
     bb.Set<uint16_t>("transaction_sender_id", player->id);
     //bb.Set<uint64_t>("ItemTransactionTimeStamp", bot.time.GetTime());
     bb.Set<ItemTransaction>("item_transaction", ItemTransaction::Sell);
    // bb.Set<bool>("ItemTransactionMessageSent", false);
    // bb.Set<bool>("ItemTransactionSetShipSent", false);
     bb.Set<int>("transaction_ship", ship);

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