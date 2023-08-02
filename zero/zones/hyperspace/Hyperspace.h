#pragma once

#include <memory>
#include <zero/game/Game.h>

namespace zero {
namespace behavior {
class BehaviorNode;
}  // namespace behavior

std::unique_ptr<behavior::BehaviorNode> GetHyperSpaceShipBuilder(Game& game);

//std::unique_ptr<behavior::BehaviorNode> BuildHyperspaceSpectator()
//    std::unique_ptr<behavior::BehaviorNode> BuildHyperspaceWarbirdCenter();
//std::unique_ptr<behavior::BehaviorNode> BuildHyperspaceLeviCenter();

}