#pragma once

#include <zero/Actuator.h>
#include <zero/Steering.h>
#include <zero/game/Game.h>
#include <zero/path/Pathfinder.h>
#include <zero/Enums.h>
#include <zero/behavior/ZoneBuilder.h>

#include <memory>

namespace zero {

namespace behavior {

class BehaviorNode;
struct ExecuteContext;

}  // namespace behavior

struct BotController {

  std::unique_ptr<path::Pathfinder> pathfinder;
  std::unique_ptr<RegionRegistry> region_registry;
  InputState* input;

  Steering steering;
  Actuator actuator;
  path::Path current_path;
  float radius = 0.0f;
  bool first_run = true;
  Zone zone;
  Time time;

  BotController(Zone zone);

  void Update(float dt, Game& game, InputState& input, behavior::ExecuteContext& ctx);
  void SetZoneBuilder(behavior::ExecuteContext& ctx);

  void SetTeams(behavior::ExecuteContext& ctx);
  void SortPlayers(behavior::ExecuteContext& ctx);

 private:
  //typedef std::unique_ptr<behavior::BehaviorNode> (*ZoneBuilder)(behavior::ExecuteContext& ctx);

  std::unique_ptr<behavior::ZoneBuilder> zoneBuilder = nullptr;
  std::unique_ptr<behavior::BehaviorNode> behavior_tree = nullptr;

};

}  // namespace zero
