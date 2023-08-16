#include "Hyperspace.h"

#include <zero/behavior/BehaviorBuilder.h>
#include <zero/behavior/BehaviorTree.h>
#include <zero/Math.h>

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
#include <zero/behavior/nodes/hyperspace/BuySellNode.h>
#include <zero/behavior/nodes/SetEnergyNode.h>

namespace zero {

// reference coord for region registry
const MapCoord kHyperTunnelCoord = MapCoord(16, 16);
// gates from center to tunnel
const std::vector<MapCoord> kCenterGates = {MapCoord(388, 395), MapCoord(572, 677)};
// gates to go back to center
const std::vector<MapCoord> kTunnelToCenterGates = {MapCoord(62, 351), MapCoord(961, 350)};
// gates to 7 bases plus top area in order
const std::vector<MapCoord> kBaseTunnelGates = {MapCoord(961, 63),  MapCoord(960, 673), MapCoord(960, 960),
                                                MapCoord(512, 959), MapCoord(64, 960),  MapCoord(64, 672),
                                                MapCoord(65, 65),   MapCoord(512, 64)};

const Vector2f kAmmoDepot = Vector2f(590, 349);

std::unique_ptr<behavior::BehaviorNode> BuildHyperspaceBuySell() {
  using namespace behavior;
  BehaviorBuilder builder;

  // clang-format off
  builder
    .Selector() // buy, sell, list, or listen
        .Sequence() // buy a ship
            .Child<ItemTransactionQuery>(ItemTransaction::BuyShip, "transaction_type") // success = move to selector
                .Selector() // spectating or safe
                    .Child<ShipQueryNode>(8) // success = move to buy
                    .Selector()        
                        .Child<TileQueryNode>(kTileSafeId) // success = on safe tile
                        .FailChild<SetEnergyNode>(100)
                        .FailChild<WarpNode>() // warp to center
                    .End()
                .End()
            .SuccessChild<BuyNode>()
        .End()
        .Sequence() // sell a ship
            .Child<ItemTransactionQuery>(ItemTransaction::SellShip, "transaction_type") // success = move to selector
                .Selector() // spectating or safe
                    .Child<ShipQueryNode>(8) // success = move to buy
                    .Selector()        
                        .Child<TileQueryNode>(kTileSafeId) // success = on safe tile
                        .FailChild<SetEnergyNode>(100)
                        .FailChild<WarpNode>() // warp to center
                    .End()
                .End()
            .SuccessChild<SellNode>()
        .End()
        .Sequence() // buy items
            .Child<ItemTransactionQuery>(ItemTransaction::Buy, "transaction_type")
                .Sequence()
                    .Selector()
                        .Child<ShipQueryNode>("transaction_ship")
                        .FailChild<SetEnergyNode>(100)
                        .FailChild<ShipRequestNode>("transaction_ship")
                    .End()
                    .Selector()
                        .Child<TileQueryNode>(kTileSafeId) // success = on safe tile
                        .FailChild<SetEnergyNode>(100)
                        .FailChild<WarpNode>() // warp to center                    
                    .End()
                .End()
            .SuccessChild<BuyNode>()
        .End()
        .Sequence()
            .Child<ItemTransactionQuery>(ItemTransaction::Sell, "transaction_type")
                 .Sequence()
                    .Selector()
                        .Child<ShipQueryNode>("transaction_ship")
                        .FailChild<SetEnergyNode>(100)
                        .FailChild<ShipRequestNode>("transaction_ship")
                    .End()
                    .Selector()
                        .Child<TileQueryNode>(kTileSafeId) // success = on safe tile
                        .FailChild<SetEnergyNode>(100)
                        .FailChild<WarpNode>() // warp to center                    
                    .End()
                .End()
            .SuccessChild<SellNode>()
         .End()
         .Sequence()
            .Child<ItemTransactionQuery>(ItemTransaction::DepotBuy, "transaction_type")
            .Child<GoToNode>(kAmmoDepot)
            .Child<AreaContainQueryNode>(kAmmoDepot, 1.0f)
            .Child<TileQueryNode>(kTileSafeId)
            .Child<InputActionNode>(InputAction::Bullet)
            .Child<SpeedQueryNode>(0.0f)
            .SuccessChild<BuyNode>()
         .End()
         .Sequence()
            .Child<ItemTransactionQuery>(ItemTransaction::DepotSell, "transaction_type")
            .Child<GoToNode>(kAmmoDepot)
            .Child<AreaContainQueryNode>(kAmmoDepot, 1.0f)
            .Child<TileQueryNode>(kTileSafeId)
            .Child<InputActionNode>(InputAction::Bullet)
            .Child<SpeedQueryNode>(0.0f)
            .SuccessChild<SellNode>()
         .End()
         .Sequence()
            .Child<ItemTransactionQuery>(ItemTransaction::ListItems, "transaction_type")
            .Child<ShipStatusNode>("transaction_ship")
         .End()
         .Sequence()
            .Child<ItemTransactionQuery>(ItemTransaction::BuySellListen, "transaction_type")
            .SuccessChild<BuySellListenNode>()
         .End()
        .Sequence()
            .Child<ItemTransactionQuery>(ItemTransaction::ShipStatusListen, "transaction_type")
            .SuccessChild<ShipStatusListenNode>()
         .End()
     .End();
  // clang-format on

  return builder.Build();
}

std::unique_ptr<behavior::BehaviorNode> BuildHyperspaceSpectator() {
  using namespace behavior;
  BehaviorBuilder builder;

  // clang-format off
  builder
    .Selector()
      .Sequence()  // Enter the specified ship if not already in it.
        .InvertChild<ShipQueryNode>("request_ship")
        .Child<ShipRequestNode>("request_ship")
        .End()
    .End();
  // clang-format on

  return builder.Build();
}

std::unique_ptr<behavior::BehaviorNode> BuildHyperspaceWarbirdCenter() {
  using namespace behavior;

  BehaviorBuilder builder;

  const Vector2f center(512, 512);

  // clang-format off
  builder
    .Selector()
        .Sequence() // Enter the specified ship if not already in it.
            .InvertChild<ShipQueryNode>("request_ship")
            .Child<ShipRequestNode>("request_ship")
            .End()
        .Sequence() // Warp back to center.
            .InvertChild<RegionContainQueryNode>(center)
            .Child<WarpNode>()
            .End()
        .Selector() // Choose to fight the player or follow waypoints.
            .Sequence() // Find nearest target and either path to them or seek them directly.
                .Sequence()
                    .Child<NearestTargetNode>("nearest_target")
                    .Child<PlayerPositionQueryNode>("nearest_target", "nearest_target_position")
                    .End()
                .Selector()
                    .Sequence() // Path to target if they aren't immediately visible.
                        .InvertChild<VisibilityQueryNode>("nearest_target_position")
                        .Child<GoToNode>("nearest_target_position")
                        .End()
                    .Sequence() // Aim at target and shoot while seeking them.
                        .Child<AimNode>("nearest_target", "aimshot")
                        .Parallel()
                            .Child<FaceNode>("aimshot")
                            .Child<SeekNode>("aimshot", "leash_distance")
                            .Sequence()
                                .InvertChild<TileQueryNode>(kTileSafeId)
                                .Child<InputActionNode>(InputAction::Bullet)
                                .End()
                            .End()
                        .End()
                    .End()
                .End()
            .Sequence() // Follow set waypoints.
                .Child<RandomWaypointNode>("waypoint_position", 15.0f)
                .Selector()
                    .Sequence()
                        .InvertChild<VisibilityQueryNode>("waypoint_position")
                        .Child<GoToNode>("waypoint_position")
                        .End()
                    .Parallel()
                        .Child<FaceNode>("waypoint_position")
                        .Child<ArriveNode>("waypoint_position", 1.25f)
                        .End()
                    .End()
                .End()
            .End()
        .End();
  // clang-format on

  return builder.Build();
}

std::unique_ptr<behavior::BehaviorNode> BuildHyperspaceLeviCenter() {
  using namespace behavior;

  BehaviorBuilder builder;

  const Vector2f center(512, 512);

  // clang-format off
  builder
    .Selector()
        .Sequence() // Enter the specified ship if not already in it.
            .InvertChild<ShipQueryNode>("request_ship")
            .Child<ShipRequestNode>("request_ship")
            .End()
        .Sequence() // Switch to own frequency when possible.
            .Child<PlayerFrequencyCountQueryNode>("self_freq_count")
            .Child<ScalarThresholdNode<size_t>>("self_freq_count", 2)
            .Child<PlayerEnergyPercentThresholdNode>(1.0f)
            .Child<TimerExpiredNode>("next_freq_change_tick")
            .Child<TimerSetNode>("next_freq_change_tick", 300)
            .Child<RandomIntNode<u16>>(5, 89, "random_freq")
            .Child<PlayerChangeFrequencyNode>("random_freq")
            .End()
        .Sequence() // Warp back to center.
            .InvertChild<RegionContainQueryNode>(center)
            .Child<WarpNode>()
            .End()
        .Sequence() // If in the center safe, move towards the warp tiles.
            .Child<TileQueryNode>(kTileSafeId)
            .Child<ClosestTileQueryNode>("center_warpers", "closest_warper")
            .InvertChild<DistanceThresholdNode>("closest_warper", 25.0f)
            .Child<SeekNode>("closest_warper")
            .End()
        .Selector() // Choose to fight the player or follow waypoints.
            .Sequence()
                .Sequence()
                    .Child<PlayerPositionQueryNode>("self_position")
                    .Child<NearestTargetNode>("nearest_target")
                    .Child<PlayerPositionQueryNode>("nearest_target", "nearest_target_position")
                    .End()
                .Selector()
                    .Sequence() // If the nearest target is close, then activate stealth and cloak.
                        .InvertChild<DistanceThresholdNode>("nearest_target_position", 50.0f)
                        .Parallel()
                            .Sequence()
                                .InvertChild<PlayerStatusQueryNode>(Status_Stealth)
                                .Child<InputActionNode>(InputAction::Stealth)
                                .End()
                            .Sequence()
                                .InvertChild<PlayerStatusQueryNode>(Status_Cloak)
                                .Child<InputActionNode>(InputAction::Cloak)
                                .End()
                            .End()
                        .End()
                    .Sequence() // Path to the nearest camp point if far away from it.
                        .Child<ClosestTileQueryNode>("levi_camp_points", "closest_levi_camp_point")
                        .Child<DistanceThresholdNode>("closest_levi_camp_point", 25.0f)
                        .Child<GoToNode>("closest_levi_camp_point")
                        .End()
                    .Parallel()
                        .Sequence() // Bomb firing sequence
                            .Child<PlayerEnergyPercentThresholdNode>(0.6f) // Only fire bomb if healthy
                            .Selector()
                                .Sequence() // Attempt to fire at closest enemy.
                                    .Child<DistanceThresholdNode>("nearest_target_position", 15.0f)
                                    .Child<VisibilityQueryNode>("nearest_target_position")
                                    .Child<PlayerBoundingBoxQueryNode>("nearest_target", "target_bounds", 12.0f)
                                    .Child<AimNode>("nearest_target", "aim_position")
                                    .Child<MoveRectangleNode>("target_bounds", "aim_position", "target_bounds")
                                    .End()
                                .Sequence() // Attempt to fire at center if enemy is not visible.
                                    .Child<ClosestTileQueryNode>("levi_aim_points", "closest_levi_aim_point")
                                    .Child<VisibilityQueryNode>("closest_levi_aim_point")
                                    .Child<RectangleNode>("closest_levi_aim_point", Vector2f(12, 12), "target_bounds")
                                    .Child<VectorNode>("closest_levi_aim_point", "aim_position")
                                    .End()
                                .End()
                            .Sequence() // After choosing the aim target, attempt to fire the bomb.
                                .Child<FaceNode>("aim_position")
                                .Child<ShotVelocityQueryNode>(WeaponType::Bomb, "bomb_fire_velocity")
                                .Child<RayNode>("self_position", "bomb_fire_velocity", "bomb_fire_ray")
                                .Child<RayRectangleInterceptNode>("bomb_fire_ray", "target_bounds")
                                .InvertChild<TileQueryNode>(kTileSafeId)
                                .Child<InputActionNode>(InputAction::Bomb)
                                .End()
                            .End()
                        .Sequence() // Disable stealth and cloak if nearest enemy is far away
                            .Child<DistanceThresholdNode>("nearest_target_position", 55.0f)
                            .Parallel()
                                .Sequence()
                                    .Child<PlayerStatusQueryNode>(Status_Stealth)
                                    .Child<InputActionNode>(InputAction::Stealth)
                                    .End()
                                .Sequence()
                                    .Child<PlayerStatusQueryNode>(Status_Cloak)
                                    .Child<InputActionNode>(InputAction::Cloak)
                                    .End()
                                .End()
                            .End()
                        .Child<SeekZeroNode>()
                        .End()
                    .Sequence() // Path to the nearest camp point if nothing else to do.
                        .Child<ClosestTileQueryNode>("levi_camp_points", "closest_levi_camp_point")
                        .Selector()
                            .Sequence() // Attempt to seek directly to camp point if visible
                                .Child<VisibilityQueryNode>("closest_levi_camp_point")
                                .Child<SeekNode>("closest_levi_camp_point")
                                .SuccessChild<FaceNode>("aim_position")
                                .End()
                            .Sequence() // Fall back to pathing to camp node
                                .Child<GoToNode>("closest_levi_camp_point")
                                .End()
                            .End()
                        .End()
                    .End()
                .End()
            .Sequence()
                .Child<RandomWaypointNode>("waypoint_position", 15.0f)
                .Selector()
                    .Sequence()
                        .InvertChild<VisibilityQueryNode>("waypoint_position")
                        .Child<GoToNode>("waypoint_position")
                        .End()
                    .Parallel()
                        .Child<FaceNode>("waypoint_position")
                        .Child<ArriveNode>("waypoint_position", 1.25f)
                        .End()
                    .End()
                .End()
            .End()
        .End();
  // clang-format on

  return builder.Build();
}

std::unique_ptr<behavior::BehaviorNode> GetHyperSpaceBehaviorTree(behavior::ExecuteContext& ctx) {
  typedef std::unique_ptr<behavior::BehaviorNode> (*ShipBuilder)();

  auto& bb = ctx.blackboard;

  ItemTransaction transaction = bb.ValueOr<ItemTransaction>("transaction_type", ItemTransaction::None); 

  if (transaction != ItemTransaction::None) {
    return BuildHyperspaceBuySell();
  }

  uint8_t ship = ctx.bot->game->player_manager.GetSelf()->ship;

  // clang-format off
  ShipBuilder shipBuilders[] = {
    BuildHyperspaceWarbirdCenter,
    BuildHyperspaceWarbirdCenter,
    BuildHyperspaceWarbirdCenter,
    BuildHyperspaceLeviCenter,
    BuildHyperspaceWarbirdCenter,
    BuildHyperspaceWarbirdCenter,
    BuildHyperspaceWarbirdCenter,
    BuildHyperspaceWarbirdCenter,
    BuildHyperspaceSpectator,
  };

  return shipBuilders[ship]();
}

}  // namespace zero