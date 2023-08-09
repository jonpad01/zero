#pragma once

#include <zero/ZeroBot.h>
#include <zero/behavior/BehaviorTree.h>
#include <zero/game/Game.h>

namespace zero {
namespace behavior {

const std::string kBuySuccess = "You purchased ";
const std::string kSellSuccess = "You sold ";
const std::string kSlotsFull = "You do not have enough free ";
const std::string kTooMany = "You may only have ";
const std::string kDoNotHave = "You do not have any of item ";
const std::string kNotAllowed = "is not allowed on a ";
const std::string kNoItem = "No item ";
const std::string kSpectator = "You cannot buy or sell items ";
const std::string kNotAvalibe = " are not avalible for sale in this arena.";
const std::string kAlreadyOwn = "You already own a ";
const std::string kDoNotOwn = "You do not own a ";
const std::string kUsingShip = "You cannot sell the ship you are using. ";
const std::string kPleaseWait = "Please wait a few moments between ship buy and sell requests";
const std::string kGoToCenter = "Go to Center Safe to ";
const std::string kGoToDepotBuy = "Go to Ammo Depots to buy it!";
const std::string kGoToDepotSell = "Go to Ammo Depots to sell it!";

const std::string kLine = "+-";
const std::string kListing = "| ";

const std::vector<std::string> match_list{
    kBuySuccess, kSellSuccess, kSlotsFull, kTooMany,   kDoNotHave,  kNotAllowed, kNoItem,       kSpectator,
    kNotAvalibe, kAlreadyOwn,  kDoNotOwn,  kUsingShip, kPleaseWait, kGoToCenter, kGoToDepotBuy, kGoToDepotSell};

struct BuySellListenNode : public BehaviorNode {
  BuySellListenNode() {}

  ExecuteResult Execute(ExecuteContext& ctx) override {
    auto& bot = ctx.bot;
    auto& bb = ctx.blackboard;
    auto& game = ctx.bot->game;
    auto self = ctx.bot->game->player_manager.GetSelf();

    int count = bb.ValueOr<int>("transaction_count", 0);
    uint16_t sender = bb.ValueOr<uint16_t>("transaction_sender_id", 0);

    for (const ChatEntry& entry : game->chat.GetRecentChat()) {
      if (entry.type != ChatType::Arena) continue;
      std::string msg(entry.message);

      for (std::size_t i = 0; i < match_list.size(); i++) {
        std::size_t found = msg.find(match_list[i]);

        if (found != std::string::npos) {
          if (match_list[i] == kGoToDepotBuy) {
            std::size_t offset = 20;
            std::size_t next = msg.find(" here.", offset);
            std::string item = msg.substr(offset, next - offset);
            bb.Set<std::vector<std::string>>("buy_list", std::vector<std::string>({item}));
            game->chat.SendPrivateMessage("Moving to ammo depot to buy " + item + ".", sender);
            bb.Set<ItemTransaction>("transaction_type", ItemTransaction::DepotBuy);
          } else if (match_list[i] == kGoToDepotSell) {
            std::size_t offset = 21;
            std::size_t next = msg.find(" here.", offset);
            std::string item = msg.substr(offset, next - offset);
            bb.Set<std::vector<std::string>>("buy_list", std::vector<std::string>({item}));
            game->chat.SendPrivateMessage("Moving to ammo depot to sell " + item + ".", sender);
            bb.Set<ItemTransaction>("transaction_type", ItemTransaction::DepotSell);
          } else if (match_list[i] == kGoToCenter) {
            game->chat.SendPrivateMessage(
                "I was supposed to be on safe before buying/selling, but something went wrong.  Please try again.",
                sender);
            count = 0;
          } else {
            game->chat.SendPrivateMessage(msg, sender);
            count--;
          }
          break;
        }
      }
    }

    if (count == 0) {
      bb.Set<ItemTransaction>("transaction_type", ItemTransaction::None);
    } else {
      bb.Set<int>("transaction_count", count);
    }

    return ExecuteResult::Success;
  }

};

struct ShipStatusListenNode : public BehaviorNode {
  ShipStatusListenNode() {}

  ExecuteResult Execute(ExecuteContext& ctx) override {
    auto& bot = ctx.bot;
    auto& bb = ctx.blackboard;
    auto& game = ctx.bot->game;
    auto self = ctx.bot->game->player_manager.GetSelf();

    int line_count = bb.ValueOr<int>("transaction_count", 0);
    uint16_t sender = bb.ValueOr<uint16_t>("transaction_sender_id", 0);

    for (const ChatEntry& entry : game->chat.GetRecentChat()) {
      if (entry.type != ChatType::Arena) continue;
      std::string msg(entry.message);

      std::size_t found = msg.find(kLine);
      if (found != std::string::npos) {
        game->chat.SendPrivateMessage(msg, sender);
        line_count++;
        continue;
      }
      found = msg.find(kListing);
      if (found != std::string::npos) {
        game->chat.SendPrivateMessage(msg, sender);
        continue;
      }
    }

    if (line_count >= 4) {
      bb.Set<ItemTransaction>("transaction_type", ItemTransaction::None);
    } else {
      bb.Set<int>("transaction_count", line_count);
    }
  }
};

struct ShipStatusNode : public BehaviorNode {
  ShipStatusNode(const char* key) : key(key) {}

  ExecuteResult Execute(ExecuteContext& ctx) override {
    auto& bot = ctx.bot;
    auto& bb = ctx.blackboard;
    auto& game = ctx.bot->game;
    auto self = ctx.bot->game->player_manager.GetSelf();

    int ship = bb.ValueOr<int>(key, self->ship);

    game->chat.ClearRecentChat();
    game->chat.SendMessage(ChatType::Public, "?shipstatus " + std::to_string(ship));

    bb.Set<ItemTransaction>("transaction_type", ItemTransaction::ShipStatusListen);
    bb.Set<int>("transaction_count", 0);

    return ExecuteResult::Success;
  }

  const char* key;
};

struct BuyNode : public BehaviorNode {
  BuyNode() {}

  ExecuteResult Execute(ExecuteContext& ctx) override {
    auto& bot = ctx.bot;
    auto& bb = ctx.blackboard;
    auto& game = ctx.bot->game;
    auto self = ctx.bot->game->player_manager.GetSelf();

    std::vector<std::string> buy_list = bb.ValueOr<std::vector<std::string>>("transaction_buy_list", std::vector<std::string>());

    if (buy_list.empty()) return ExecuteResult::Failure;

    // send a buy command
    std::string buy_msg = "?";

    for (std::string item : buy_list) {
      buy_msg += "|buy " + item;
    }

    game->chat.ClearRecentChat();
    game->chat.SendMessage(ChatType::Public, buy_msg.c_str());
    bb.Set<ItemTransaction>("transaction_type", ItemTransaction::BuySellListen);
    bb.Set<int>("transaction_count", (int)buy_list.size());

    return ExecuteResult::Success;
  }
};

struct SellNode : public BehaviorNode {
  SellNode() {}

  ExecuteResult Execute(ExecuteContext& ctx) override {
    auto& bot = ctx.bot;
    auto& bb = ctx.blackboard;
    auto& game = ctx.bot->game;
    auto self = ctx.bot->game->player_manager.GetSelf();

    std::vector<std::string> sell_list = bb.ValueOr<std::vector<std::string>>("transaction_sell_list", std::vector<std::string>());

    if (sell_list.empty()) return ExecuteResult::Failure;

    // send a buy command
    std::string buy_msg = "?";

    for (std::string item : sell_list) {
      buy_msg += "|sell " + item;
    }

    game->chat.ClearRecentChat();
    game->chat.SendMessage(ChatType::Public, buy_msg.c_str());
    bb.Set<ItemTransaction>("transaction_type", ItemTransaction::BuySellListen);
    bb.Set<int>("transaction_count", (int)sell_list.size());

    return ExecuteResult::Failure;
  }
};

struct ItemTransactionQuery : public BehaviorNode {
  ItemTransactionQuery(ItemTransaction type, const char* key) : type(type), key(key) {}

  ExecuteResult Execute(ExecuteContext& ctx) override {
    auto& bb = ctx.blackboard;

    ItemTransaction transaction = bb.ValueOr<ItemTransaction>(key, ItemTransaction::None);

    if (transaction == type) {
      return ExecuteResult::Success;
    }

    return ExecuteResult::Failure;
    }

  private: 
   ItemTransaction type;
   const char* key;
};


}
}  // namespace zero
