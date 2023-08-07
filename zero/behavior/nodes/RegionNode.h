#pragma once

#include <zero/BotController.h>
#include <zero/ZeroBot.h>
#include <zero/behavior/BehaviorTree.h>
#include <zero/game/Game.h>

namespace zero {
namespace behavior {

struct RegionContainQueryNode : public BehaviorNode {
  RegionContainQueryNode(Vector2f position) : coord(position) {}

  ExecuteResult Execute(ExecuteContext& ctx) override {
    auto self = ctx.bot->game->player_manager.GetSelf();
    auto& registry = ctx.bot->bot_controller->region_registry;

    if (!registry) return ExecuteResult::Failure;

    MapCoord self_coord(self->position);

    bool connected = registry->IsConnected(self_coord, coord);

    return connected ? ExecuteResult::Success : ExecuteResult::Failure;
  }

  MapCoord coord;
};

struct AreaContainQueryNode : public BehaviorNode {
  AreaContainQueryNode(Vector2f position, float distance) : coord(position), distance(distance) {}

  ExecuteResult Execute(ExecuteContext& ctx) override {
    auto self = ctx.bot->game->player_manager.GetSelf();
    auto& registry = ctx.bot->bot_controller->region_registry;

    if (!registry) return ExecuteResult::Failure;

    MapCoord self_coord(self->position);

    bool connected = registry->IsConnected(self_coord, coord);

    if (!connected) return ExecuteResult::Failure;

    if (self->position.Distance(coord) > distance) return ExecuteResult::Failure;

    return ExecuteResult::Success;
  }

  Vector2f coord;
  float distance;
};

struct SafeTileQueryNode : public BehaviorNode {
  SafeTileQueryNode(Vector2f position) : coord(position) {}
  SafeTileQueryNode() : default_constructor(true) {}

  ExecuteResult Execute(ExecuteContext& ctx) override {
    auto self = ctx.bot->game->player_manager.GetSelf();

    if (default_constructor) {
      coord = ctx.bot->game->player_manager.GetSelf()->position;
    }
   
    bool on_safe = ctx.bot->game->GetMap().GetTileId(coord) == kTileSafeId;

    if (!on_safe) return ExecuteResult::Failure;

    return ExecuteResult::Success;
  }

  Vector2f coord;
  bool default_constructor = false;
};

}  // namespace behavior
}  // namespace zero
