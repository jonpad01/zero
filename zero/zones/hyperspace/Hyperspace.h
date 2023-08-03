#pragma once

#include <memory>
#include <zero/game/Game.h>

namespace zero {
namespace behavior {
class BehaviorNode;
}  // namespace behavior

std::unique_ptr<behavior::BehaviorNode> GetHyperSpaceBehaviorTree(Game& game);

//std::unique_ptr<behavior::BehaviorNode> BuildHyperspaceSpectator()
//    std::unique_ptr<behavior::BehaviorNode> BuildHyperspaceWarbirdCenter();
//std::unique_ptr<behavior::BehaviorNode> BuildHyperspaceLeviCenter();

}