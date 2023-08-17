#ifndef ZONEBUILDER_H
#define ZONEBUILDER_H

#include <memory>
#include <zero/behavior/BehaviorBuilder.h>

namespace zero {
namespace behavior {

class ZoneBuilder {
 public:
  virtual std::unique_ptr<behavior::BehaviorNode> GetNewZoneTree(behavior::ExecuteContext& ctx) = 0;
  virtual bool ShouldRebuildTree(behavior::ExecuteContext& ctx) = 0;
};

}  // namespace behavior
}  // namespace zero

#endif