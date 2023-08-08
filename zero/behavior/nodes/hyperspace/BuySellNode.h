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

const std::vector<std::string> match_list{
    kBuySuccess, kSellSuccess, kSlotsFull, kTooMany,   kDoNotHave,  kNotAllowed, kNoItem,       kSpectator,
    kNotAvalibe, kAlreadyOwn,  kDoNotOwn,  kUsingShip, kPleaseWait, kGoToCenter, kGoToDepotBuy, kGoToDepotSell};

struct ListenNode : public BehaviorNode {
  ListenNode() {}

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
            bb.Set<ItemTransaction>("item_transaction", ItemTransaction::DepotBuy);
          } else if (match_list[i] == kGoToDepotSell) {
            std::size_t offset = 21;
            std::size_t next = msg.find(" here.", offset);
            std::string item = msg.substr(offset, next - offset);
            bb.Set<std::vector<std::string>>("buy_list", std::vector<std::string>({item}));
            game->chat.SendPrivateMessage("Moving to ammo depot to sell " + item + ".", sender);
            bb.Set<ItemTransaction>("item_transaction", ItemTransaction::DepotSell);
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
      bb.Set<ItemTransaction>("item_transaction", ItemTransaction::None);
    } else {
      bb.Set<int>("transaction_count", count);
    }

    return ExecuteResult::Success;
  }

};

struct ShipStatusNode : public BehaviorNode {
  ShipStatusNode() {}

  ExecuteResult Execute(ExecuteContext& ctx) override {
    auto& bot = ctx.bot;
    auto& bb = ctx.blackboard;
    auto& game = ctx.bot->game;
    auto self = ctx.bot->game->player_manager.GetSelf();

    //bb.Set<ItemTransaction>("item_transaction", ItemTransaction::None);

    return ExecuteResult::Failure;
  }
};

struct BuyNode : public BehaviorNode {
  BuyNode() {}

  ExecuteResult Execute(ExecuteContext& ctx) override {
    auto& bot = ctx.bot;
    auto& bb = ctx.blackboard;
    auto& game = ctx.bot->game;
    auto self = ctx.bot->game->player_manager.GetSelf();

    std::vector<std::string> buy_list = bb.ValueOr<std::vector<std::string>>("buy_list", std::vector<std::string>());

    if (buy_list.empty()) return ExecuteResult::Failure;

    // send a buy command
    std::string buy_msg = "?";

    for (std::string ship : buy_list) {
      buy_msg += "|buy " + ship;
    }

    game->chat.SendMessage(ChatType::Public, buy_msg.c_str());
    bb.Set<ItemTransaction>("item_transaction", ItemTransaction::Listen);
    //bb.Set<std::vector<std::string>>("buy_list", std::vector<std::string>());
    bb.Set<int>("transaction_count", (int)buy_list.size());

    return ExecuteResult::Failure;
  }
};

struct SellNode : public BehaviorNode {
  SellNode() {}

  ExecuteResult Execute(ExecuteContext& ctx) override {
    auto& bot = ctx.bot;
    auto& bb = ctx.blackboard;
    auto& game = ctx.bot->game;
    auto self = ctx.bot->game->player_manager.GetSelf();

    std::vector<std::string> sell_list = bb.ValueOr<std::vector<std::string>>("sell_list", std::vector<std::string>());

    if (sell_list.empty()) return ExecuteResult::Failure;

    // send a buy command
    std::string buy_msg = "?";

    for (std::string ship : sell_list) {
      buy_msg += "|sell " + ship;
    }

    game->chat.SendMessage(ChatType::Public, buy_msg.c_str());
    bb.Set<ItemTransaction>("item_transaction", ItemTransaction::Listen);
    //bb.Set<std::vector<std::string>>("sell_list", std::vector<std::string>());
    bb.Set<int>("transaction_count", (int)sell_list.size());

    return ExecuteResult::Failure;
  }
};

struct BuyItemQuery : public BehaviorNode {
  BuyItemQuery() {}

  ExecuteResult Execute(ExecuteContext& ctx) override {
    auto& bb = ctx.blackboard;

    ItemTransaction transaction = bb.ValueOr<ItemTransaction>("item_transaction", ItemTransaction::None);

    if (transaction == ItemTransaction::Buy) {
      return ExecuteResult::Success;
    }

    return ExecuteResult::Failure;
  }
};

struct SellItemQuery : public BehaviorNode {
  SellItemQuery() {}

  ExecuteResult Execute(ExecuteContext& ctx) override {
    auto& bb = ctx.blackboard;

    ItemTransaction transaction = bb.ValueOr<ItemTransaction>("item_transaction", ItemTransaction::None);

    if (transaction == ItemTransaction::Sell) {
      return ExecuteResult::Success;
    }

    return ExecuteResult::Failure;
  }
};

struct BuyShipQuery : public BehaviorNode {
  BuyShipQuery() {}

  ExecuteResult Execute(ExecuteContext& ctx) override {
    auto& bb = ctx.blackboard;

    ItemTransaction transaction = bb.ValueOr<ItemTransaction>("item_transaction", ItemTransaction::None);

    if (transaction == ItemTransaction::BuyShip) {
      return ExecuteResult::Success;
    }

    return ExecuteResult::Failure;
  }
};

struct SellShipQuery : public BehaviorNode {
  SellShipQuery() {}

  ExecuteResult Execute(ExecuteContext& ctx) override {
    auto& bb = ctx.blackboard;

    ItemTransaction transaction = bb.ValueOr<ItemTransaction>("item_transaction", ItemTransaction::None);

    if (transaction == ItemTransaction::SellShip) {
      return ExecuteResult::Success;
    }

    return ExecuteResult::Failure;
  }
};

struct DepotBuyQuery : public BehaviorNode {
  DepotBuyQuery() {}

  ExecuteResult Execute(ExecuteContext& ctx) override {
    auto& bb = ctx.blackboard;

    ItemTransaction transaction = bb.ValueOr<ItemTransaction>("item_transaction", ItemTransaction::None);

    if (transaction == ItemTransaction::DepotBuy) {
      return ExecuteResult::Success;
    }

    return ExecuteResult::Failure;
  }
};

struct DepotSellQuery : public BehaviorNode {
  DepotSellQuery() {}

  ExecuteResult Execute(ExecuteContext& ctx) override {
    auto& bb = ctx.blackboard;

    ItemTransaction transaction = bb.ValueOr<ItemTransaction>("item_transaction", ItemTransaction::None);

    if (transaction == ItemTransaction::DepotSell) {
      return ExecuteResult::Success;
    }

    return ExecuteResult::Failure;
  }
};

struct ListItemsQuery : public BehaviorNode {
  ListItemsQuery() {}

  ExecuteResult Execute(ExecuteContext& ctx) override {
    auto& bb = ctx.blackboard;

    ItemTransaction transaction = bb.ValueOr<ItemTransaction>("item_transaction", ItemTransaction::None);

    if (transaction == ItemTransaction::ListItems) {
      return ExecuteResult::Success;
    }

    return ExecuteResult::Failure;
  }
};

struct ListenQuery : public BehaviorNode {
  ListenQuery() {}

  ExecuteResult Execute(ExecuteContext& ctx) override {
    auto& bb = ctx.blackboard;

    ItemTransaction transaction = bb.ValueOr<ItemTransaction>("item_transaction", ItemTransaction::None);

    if (transaction == ItemTransaction::Listen) {
      return ExecuteResult::Success;
    }

    return ExecuteResult::Failure;
  }
};







}
}  // namespace zero
