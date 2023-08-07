#pragma once

#include <zero/ZeroBot.h>
#include <zero/behavior/BehaviorTree.h>
#include <zero/game/Game.h>

namespace zero {
namespace behavior {

struct BuySellNode : public BehaviorNode {
  BuySellNode() {}

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
    bb.Set<ItemTransaction>("item_transaction", ItemTransaction::None);

    return ExecuteResult::Failure;
  }
};

struct GetToSafeNode : public BehaviorNode {
  GetToSafeNode() {}

    ExecuteResult Execute(ExecuteContext& ctx) override {
    auto& bot = ctx.bot;
    auto& bb = ctx.blackboard;
    auto& game = ctx.bot->game;
    auto self = ctx.bot->game->player_manager.GetSelf();

    bool on_safe = game->GetMap().GetTileId(self->position) == kTileSafeId;
  
    if (!on_safe && self->ship != 9) {
     // game->player_manager.Get
      return ExecuteResult::Failure;
    }


    }

};








}
}  // namespace zero
