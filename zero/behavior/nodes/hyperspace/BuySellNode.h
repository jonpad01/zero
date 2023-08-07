#pragma once

#include <zero/ZeroBot.h>
#include <zero/behavior/BehaviorTree.h>
#include <zero/game/Game.h>

namespace zero {
namespace behavior {

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
    bb.Set<std::vector<std::string>>("buy_list", std::vector<std::string>());

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
    bb.Set<std::vector<std::string>>("sell_list", std::vector<std::string>());

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
