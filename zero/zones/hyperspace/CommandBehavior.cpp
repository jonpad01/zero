#include "CommandBehavior.h"

#include <zero/behavior/BehaviorBuilder.h>
#include <zero/behavior/BehaviorTree.h>
#include <zero/behavior/nodes/BehaviorNode.h>
#include <zero/behavior/nodes/BlackboardNode.h>
#include <zero/behavior/nodes/InputActionNode.h>
#include <zero/behavior/nodes/MathNode.h>
#include <zero/behavior/nodes/MoveNode.h>
#include <zero/behavior/nodes/PlayerNode.h>
#include <zero/behavior/nodes/RegionNode.h>

namespace zero {
namespace hyperspace {

template <typename TransactionNodeType>
std::unique_ptr<behavior::BehaviorNode> CreateTransactionTree(behavior::ExecuteContext& ctx) {
  using namespace behavior;

  BehaviorBuilder builder;

  const Vector2f center(512, 512);
  const Vector2f depot_position(435, 345);

  // This rectangle is smaller than the actual safe zone so we're sure to be inside of it when executing commands.
  Rectangle center_safe_rect(Vector2f(503, 503), Vector2f(520, 520));
  Rectangle depot_safe_rect(Vector2f(433, 345), Vector2f(437, 349));

  // clang-format off
  builder
    .Selector()
        .Sequence()
            .Child<ValueCompareQuery<Store>>(TransactionNodeType::State::StoreKey(), Store::Center)
            .Child<PlayerPositionQueryNode>("self_position")
            .Sequence(CompositeDecorator::Success)
                .Child<DistanceNode>(center, "self_position", "to_center_dist", true)
                .Child<ScalarThresholdNode<float>>("to_center_dist", 32.0f * 32.0f)
                .Child<WarpNode>() // Keep trying to warp to center if not near center
                .End()
            .Sequence(CompositeDecorator::Success) // Try to move toward center in case it can reach it before energy is full.
                .Child<RegionContainQueryNode>(center)
                .Child<GoToNode>(center)
                .End()
            .Child<RectangleContainsNode>(center_safe_rect, "self_position") // Check if we are in the center safe
            .Child<InputActionNode>(InputAction::Bullet) // Stop moving in the safe
            .Child<TransactionNodeType>() // Execute the buy/sell node
            .End()
        .Sequence()
            .Child<ValueCompareQuery<Store>>(TransactionNodeType::State::StoreKey(), Store::Depot)
            .Child<PlayerPositionQueryNode>("self_position")
            .Sequence(CompositeDecorator::Success)
                .Selector()
                    .Sequence()
                        .InvertChild<RegionContainQueryNode>(center)
                        .Child<WarpNode>() // Warp to center if not in center
                        .End()
                    .Sequence()
                        .Child<GoToNode>(depot_position)
                        .Child<RectangleContainsNode>(depot_safe_rect, "self_position") // Check if we are in the depot safe
                        .Child<InputActionNode>(InputAction::Bullet) // Stop moving in the safe
                        .Child<TransactionNodeType>() // Execute the buy/sell node
                        .End()
                    .End()
                .End()
            .End()
        .End()
    .End();
  // clang-format on

  return builder.Build();
}

std::unique_ptr<behavior::BehaviorNode> CommandBehavior::CreateTree(behavior::ExecuteContext& ctx) {
  using namespace behavior;

  BehaviorBuilder builder;

  // clang-format off
  builder
    .Selector()
        .Sequence()
            .Child<CommandTypeQuery>(CommandType::ShipItems)
            .Child<ShipItemsParseNode>()
            .End()
        .Sequence()
            .Child<CommandTypeQuery>(CommandType::Buy)
            .Composite(CreateTransactionTree<BuyNode>(ctx), CompositeDecorator::Success)
            .End()
        .Sequence()
            .Child<CommandTypeQuery>(CommandType::Sell)
            .Composite(CreateTransactionTree<SellNode>(ctx), CompositeDecorator::Success)
            .End()
        .Sequence() // If nothing is being executed, restore the old tree.
            .Child<BehaviorSetFromKeyNode>(CommandExecuteState::TreeKey())
            .End()
        .End();
  // clang-format on

  return builder.Build();
}

}  // namespace hyperspace
}  // namespace zero
