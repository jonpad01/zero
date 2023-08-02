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

//constexpr int kShip = 0;



BotController::BotController(Game& game, Zone zone) {
  
  uint8_t ship = game.player_manager.GetSelf()->ship;
  this->zone = zone;

  this->behavior_tree = GetBehaviorTree(game);
  this->input = nullptr;
}

void BotController::Update(float dt, Game& game, InputState& input, behavior::ExecuteContext& execute_ctx) {
  this->input = &input;

  uint8_t ship = game.player_manager.GetSelf()->ship;
  float radius = game.connection.settings.ShipSettings[ship].GetRadius();

  // TOOD: Rebuild on ship change and use ship radius.
  if (this->radius != radius) {
      this->radius = radius;
    
    auto processor = std::make_unique<path::NodeProcessor>(game);

    region_registry = std::make_unique<RegionRegistry>();
    region_registry->CreateAll(game.GetMap(), radius);

    pathfinder = std::make_unique<path::Pathfinder>(std::move(processor), *region_registry);
    pathfinder->CreateMapWeights(game.GetMap(), radius);

    //execute_ctx.blackboard.Set("request_ship", kShip);
    execute_ctx.blackboard.Set("leash_distance", 15.0f);

    std::vector<Vector2f> waypoints{
        Vector2f(440, 460),
        Vector2f(565, 465),
        Vector2f(570, 565),
        Vector2f(415, 590),
    };

    execute_ctx.blackboard.Set("waypoints", waypoints);

    std::vector<Vector2f> center_warpers{
        Vector2f(500, 500),
        Vector2f(523, 500),
        Vector2f(523, 523),
        Vector2f(500, 523),
    };

    execute_ctx.blackboard.Set("center_warpers", center_warpers);

    std::vector<Vector2f> levi_camp_points{
        Vector2f(630, 475),
        Vector2f(600, 600),
        Vector2f(420, 605),
        Vector2f(410, 450),
    };

    execute_ctx.blackboard.Set("levi_camp_points", levi_camp_points);

    std::vector<Vector2f> levi_aim_points{
        Vector2f(512, 480),  // North
        Vector2f(543, 511),  // East
        Vector2f(512, 543),  // South
        Vector2f(480, 511),  // West
    };

    execute_ctx.blackboard.Set("levi_aim_points", levi_aim_points);

    behavior_tree = GetBehaviorTree(game);
  }

  steering.Reset();

  if (behavior_tree) {
    behavior_tree->Execute(execute_ctx);
  }

  actuator.Update(game, input, steering.force, steering.rotation);
}

std::unique_ptr<behavior::BehaviorNode> BotController::GetBehaviorTree(Game& game) {
       
    std::unique_ptr<behavior::BehaviorNode> result = nullptr;

  switch (zone) {
    case Zone::Hyperspace: {
       result = GetHyperSpaceShipBuilder(game);
       break;
    }
  }

  return result;
}

}  // namespace zero
