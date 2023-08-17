#include "BotController.h"

#include <zero/behavior/BehaviorBuilder.h>
#include <zero/behavior/BehaviorTree.h>
#include <zero/behavior/nodes/AimNode.h>
#include <zero/behavior/nodes/BlackboardNode.h>
#include <zero/behavior/nodes/InputActionNode.h>
#include <zero/behavior/nodes/MapNode.h>
#include <zero/behavior/nodes/MathNode.h>
#include <zero/behavior/nodes/MoveNode.h>
#include <zero/behavior/nodes/PlayerNode.h>
#include <zero/behavior/nodes/RegionNode.h>
#include <zero/behavior/nodes/ShipNode.h>
#include <zero/behavior/nodes/TargetNode.h>
#include <zero/behavior/nodes/TimerNode.h>
#include <zero/behavior/nodes/WaypointNode.h>

#include <zero/zones/hyperspace/Hyperspace.h>

namespace zero {


// ctx.bot can't be called here because it hasnt been assigned yet
BotController::BotController(Zone zone) { 
  this->zone = zone;
  input = nullptr;
}

// TODO: find method to update zone
void BotController::Update(float dt, Game& game, InputState& input, behavior::ExecuteContext& ctx) {
  this->input = &input;

  uint8_t ship = game.player_manager.GetSelf()->ship;
  float radius = game.connection.settings.ShipSettings[ship].GetRadius();

  // float comparisons are ok here because they are always the same values
  if (this->radius != radius || first_run) {
      this->radius = radius;
    
    auto processor = std::make_unique<path::NodeProcessor>(game);

    region_registry = std::make_unique<RegionRegistry>();
    region_registry->CreateAll(game.GetMap(), radius);

    pathfinder = std::make_unique<path::Pathfinder>(std::move(processor), *region_registry);
    pathfinder->CreateMapWeights(game.GetMap(), radius);
  }

   // initial run
  if (first_run) {
    ctx.blackboard.Set<int>("request_ship", (rand() % 8));
    SetZoneBuilder(ctx);
    // if (zoneBuilder) behavior_tree = zoneBuilder(ctx);
    if (zone_builder) {
      root = zone_builder->GetRoot(ctx);
      tree = zone_builder->GetTree(ctx);
    }
    first_run = false;
  }
  
  // this call allows bot to switch trees
  if (zone_builder->ShouldRebuildTree(ctx)) tree = zone_builder->GetTree(ctx);

  steering.Reset();

   if (root) {
    root->Execute(ctx);
   }

  if (tree) {
    tree->Execute(ctx);
  }

  actuator.Update(game, input, steering.force, steering.rotation);
}

void BotController::SetZoneBuilder(behavior::ExecuteContext& ctx) {
  
  switch (zone) {
    case Zone::Hyperspace: {
      //zoneBuilder = GetHyperSpaceBehaviorTree;
      zone_builder = std::make_unique<behavior::HyperspaceBuilder>(ctx);
       break;
    }
    default: {
       //zoneBuilder = nullptr;
    }
  }
}


}  // namespace zero
