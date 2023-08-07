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


// ctx.bot can't be called here because it hasnt been assigned yet
BotController::BotController(Game& game, behavior::ExecuteContext& ctx, Zone zone) {
  
  this->zone = zone;
  input = nullptr;
}

// TODO: find method to update zone
void BotController::Update(float dt, Game& game, InputState& input, behavior::ExecuteContext& ctx) {
  this->input = &input;

  // initial run
  if (this->radius == 0.0f) {
    ctx.blackboard.Set<int>("request_ship", (rand() % 8));
    SetZoneBuilder();
    if (zoneBuilder) behavior_tree = zoneBuilder(ctx);
  }

  uint8_t ship = game.player_manager.GetSelf()->ship;
  float radius = game.connection.settings.ShipSettings[ship].GetRadius();

  // float comparisons are ok here because they are always the same values
  if (this->radius != radius || this->radius == 0.0f) {
      this->radius = radius;
    
    auto processor = std::make_unique<path::NodeProcessor>(game);

    region_registry = std::make_unique<RegionRegistry>();
    region_registry->CreateAll(game.GetMap(), radius);

    pathfinder = std::make_unique<path::Pathfinder>(std::move(processor), *region_registry);
    pathfinder->CreateMapWeights(game.GetMap(), radius);

    //execute_ctx.blackboard.Set("request_ship", kShip);
    ctx.blackboard.Set("leash_distance", 15.0f);

    std::vector<Vector2f> waypoints{
        Vector2f(440, 460),
        Vector2f(565, 465),
        Vector2f(570, 565),
        Vector2f(415, 590),
    };

    ctx.blackboard.Set("waypoints", waypoints);

    std::vector<Vector2f> center_warpers{
        Vector2f(500, 500),
        Vector2f(523, 500),
        Vector2f(523, 523),
        Vector2f(500, 523),
    };

    ctx.blackboard.Set("center_warpers", center_warpers);

    std::vector<Vector2f> levi_camp_points{
        Vector2f(630, 475),
        Vector2f(600, 600),
        Vector2f(420, 605),
        Vector2f(410, 450),
    };

    ctx.blackboard.Set("levi_camp_points", levi_camp_points);

    std::vector<Vector2f> levi_aim_points{
        Vector2f(512, 480),  // North
        Vector2f(543, 511),  // East
        Vector2f(512, 543),  // South
        Vector2f(480, 511),  // West
    };

    ctx.blackboard.Set("levi_aim_points", levi_aim_points);
  }

  SendMessages(game);
  
  // call the zone builder every update so it can switch trees
  if (zoneBuilder) behavior_tree = zoneBuilder(ctx);
  steering.Reset();

  if (behavior_tree) {
    behavior_tree->Execute(ctx);
  }

  actuator.Update(game, input, steering.force, steering.rotation);
}

void BotController::SetZoneBuilder() {
  
  switch (zone) {
    case Zone::Hyperspace: {
      zoneBuilder = GetHyperSpaceBehaviorTree;
       break;
    }
    default: {
       zoneBuilder = nullptr;
    }
  }
}

void BotController::SendMessages(Game& game) {

  if (time.GetTime() < message_sender_timestamp + 10000) return;
  message_sender_timestamp = time.GetTime();

  switch (zone) {
    case Zone::Hyperspace: {
       bool flagging = game.player_manager.GetSelf()->frequency == 90 || game.player_manager.GetSelf()->frequency == 91;
      // game.chat.SendMessage(ChatType::Public, "?ships");
       if (flagging) game.chat.SendMessage(ChatType::Public, "?lancs");
       break;
    }
    default: {
    }
  }
}

}  // namespace zero
