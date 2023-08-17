#pragma once

#include <zero/ZeroBot.h>
#include <zero/behavior/BehaviorTree.h>
#include <zero/game/Game.h>

namespace zero {
namespace behavior {

  static Vector2f GetBallPosition(PlayerManager& player_manager, Powerball& ball, u64 microtick) {
  if (ball.state == BallState::Carried) {
    Player* carrier = player_manager.GetPlayerById(ball.carrier_id);

    if (carrier && carrier->ship != 8) {
      Vector2f heading = OrientationToHeading((u8)(carrier->orientation * 40.0f));
      float radius = player_manager.connection.settings.ShipSettings[carrier->ship].GetRadius();

      float extension = radius - 0.25f;

      if (extension < 0) {
        extension = 0.0f;
      }

      return carrier->position.PixelRounded() + heading * extension;
    }
  }

  Vector2f current_position(ball.x / 16000.0f, ball.y / 16000.0f);
  Vector2f next_position(ball.next_x / 16000.0f, ball.next_y / 16000.0f);
  float t = (microtick - ball.last_micro_tick) / (float)kTickDurationMicro;

  return current_position * (1 - t) + (t * next_position);
}

struct HoldingPowerBallQueryNode : public BehaviorNode {
  ExecuteResult Execute(ExecuteContext& ctx) override {
    auto self = ctx.bot->game->player_manager.GetSelf();

    bool holding = ctx.bot->game->soccer.IsCarryingBall();

    return holding ? ExecuteResult::Success : ExecuteResult::Failure;
  }

};

struct SetClosestPowerBallNode : public BehaviorNode {
  SetClosestPowerBallNode(const char* powerball_key) : powerball_key(powerball_key) {}

  ExecuteResult Execute(ExecuteContext& ctx) override {
    auto self = ctx.bot->game->player_manager.GetSelf();
    auto& bb = ctx.blackboard;

    float closest_distance = 9999.9f;
    Vector2f closest_ball;

    for (std::size_t i = 0; i < ZERO_ARRAY_SIZE(ctx.bot->game->soccer.balls);  i ++) {
      Powerball ball = ctx.bot->game->soccer.balls[i];
      //Vector2f pos(ball.x / 16000.0F, ball.y / 16000.0F);
      Vector2f pos = GetBallPosition(ctx.bot->game->player_manager, ball, GetMicrosecondTick());
      float distance = pos.Distance(self->position);

      if (distance < closest_distance) {
        closest_distance = distance;
        closest_ball = pos;
      }
    }

    bb.Set(powerball_key, closest_ball);

    return ExecuteResult::Success;
  }

  const char* powerball_key;
};


}
}  // namespace zero
