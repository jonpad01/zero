#pragma once

#include <zero/Common.h>
#include <zero/ZeroBot.h>
#include <zero/behavior/BehaviorTree.h>
#include <zero/game/Game.h>
#include <zero/game/Logger.h>

namespace zero {
namespace behavior {

struct SetEnergyNode : public BehaviorNode {
  SetEnergyNode(int percent) : percent(percent) {}
  
  ExecuteResult Execute(ExecuteContext& ctx) override { 
	  auto self = ctx.bot->game->player_manager.GetSelf();

	  if (percent < 0) percent = 0;
      if (percent > 100) percent = 100;
	  
	  uint32_t max_energy = ctx.bot->game->ship_controller.ship.energy;

	  self->energy = int(max_energy) * (percent / 100);

	  return ExecuteResult::Success;
  }

  private:
  int percent;
};


}
}
