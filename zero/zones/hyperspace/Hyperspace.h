#pragma once

#include <memory>
#include <zero/game/Game.h>
#include <zero/behavior/ZoneBuilder.h>
#include <zero/enums.h>


namespace zero {
namespace behavior {

class BehaviorNode;
struct ExecuteContext;

class HyperspaceBuilder : public ZoneBuilder {
 public:

  HyperspaceBuilder(behavior::ExecuteContext& ctx);

  void Initialize(behavior::ExecuteContext& ctx) override;
  std::unique_ptr<behavior::BehaviorNode> GetTree(behavior::ExecuteContext& ctx) override;
  bool ShouldRebuildTree(behavior::ExecuteContext& ctx) override;
 private:

  ItemTransaction transaction = ItemTransaction::None;
  int ship = 0;
};

//std::unique_ptr<behavior::BehaviorNode> GetHyperSpaceBehaviorTree(behavior::ExecuteContext& ctx);

// std::unique_ptr<behavior::BehaviorNode> BuildHyperspaceSpectator()
//     std::unique_ptr<behavior::BehaviorNode> BuildHyperspaceWarbirdCenter();
// std::unique_ptr<behavior::BehaviorNode> BuildHyperspaceLeviCenter();

}  // namespace behavior
}  // namespace zero