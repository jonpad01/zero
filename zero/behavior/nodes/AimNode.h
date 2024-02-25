#pragma once

#include <zero/ZeroBot.h>
#include <zero/behavior/BehaviorTree.h>
#include <zero/game/Game.h>

namespace zero {
namespace behavior {


struct ShotResult {
  ShotResult() : hit(false) {}
  bool hit;
  Vector2f solution;
};

inline ShotResult CalculateShot(Vector2f pShooter, Vector2f pTarget, Vector2f vShooter, Vector2f vTarget,
                                  float sProjectile) {
  ShotResult result;

  Vector2f totarget = pTarget - pShooter;  // directional vector pointing to target from shooter
  Vector2f v = vTarget - vShooter;         // relative velocity

  // dot product
  double a = double(v.Dot(v)) - double(sProjectile * sProjectile);
  double b = 2.0 * double(v.Dot(totarget));
  double c = double(totarget.Dot(totarget));

  double disc = (b * b) - 4.0 * a * c;  // quadratic formula for the discriminant
  double t = 0.0;                       // time when the bullet intercepts the target

  // discriminant can't be negative, the target is probably moving away from the shooter at a
  // faster velocity than the bullet
  if (disc <= 0.0 || a == 0.0) {
    return result;
  }

  double t1 = (-b - std::sqrt(disc)) / (2.0 * a);
  double t2 = (-b + std::sqrt(disc)) / (2.0 * a);

  if (t1 >= 0.0 && t2 >= 0.0) {
    t = std::min(t1, t2);
  } else if (t1 >= 0.0) {
    t = t1;
  } else if (t2 >= 0.0) {
    t = t2;
  } else {  // time can't be negative
    return result;
  }

  Vector2f solution = pTarget + (v * float(t));

  result.solution = solution;
  result.hit = true;

  return result;
}

struct ShotVelocityQueryNode : public BehaviorNode {
  ShotVelocityQueryNode(WeaponType weapon, const char* velocity_key)
      : weapon_type(weapon), player_key(nullptr), velocity_key(velocity_key) {}
  ShotVelocityQueryNode(const char* player_key, WeaponType weapon, const char* velocity_key)
      : weapon_type(weapon), player_key(player_key), velocity_key(velocity_key) {}

  ExecuteResult Execute(ExecuteContext& ctx) override {
    Player* player = nullptr;

    if (player_key) {
      auto opt_player = ctx.blackboard.Value<Player*>(player_key);
      if (!opt_player.has_value()) return ExecuteResult::Failure;

      player = opt_player.value();
    } else {
      player = ctx.bot->game->player_manager.GetSelf();
    }

    if (!player) return ExecuteResult::Failure;
    if (player->ship >= 8) return ExecuteResult::Failure;

    Vector2f velocity;
    float weapon_speed = 0.0f;

    switch (weapon_type) {
      case WeaponType::Bomb:
      case WeaponType::ProximityBomb:
      case WeaponType::Thor: {
        weapon_speed = ctx.bot->game->connection.settings.ShipSettings[player->ship].BombSpeed / 16.0f / 10.0f;
      } break;
      case WeaponType::Bullet:
      case WeaponType::BouncingBullet: {
        weapon_speed = ctx.bot->game->connection.settings.ShipSettings[player->ship].BulletSpeed / 16.0f / 10.0f;
      } break;
      case WeaponType::Decoy: {
        // Do nothing since it just matches the player's velocity.
      } break;
      default: {
        return ExecuteResult::Failure;
      } break;
    }

    velocity = player->velocity + player->GetHeading() * weapon_speed;

    ctx.blackboard.Set(velocity_key, velocity);

    return ExecuteResult::Success;
  }

  const char* player_key;
  const char* velocity_key;
  WeaponType weapon_type;
};

struct AimAtPlayerNode : public BehaviorNode {
  AimAtPlayerNode(const char* target_player_key, const char* position_key)
      : target_player_key(target_player_key), position_key(position_key) {}

  ExecuteResult Execute(ExecuteContext& ctx) override {
    auto self = ctx.bot->game->player_manager.GetSelf();
    if (!self) return ExecuteResult::Failure;

    auto opt_target = ctx.blackboard.Value<Player*>(target_player_key);
    if (!opt_target.has_value()) return ExecuteResult::Failure;

    float weapon_speed = ctx.bot->game->connection.settings.ShipSettings[self->ship].BulletSpeed / 16.0f / 10.0f;

    Player* target = opt_target.value();
    ShotResult aimResult =
        CalculateShot(self->position, target->position, self->velocity, target->velocity, weapon_speed);

    if (!aimResult.hit) {
      aimResult.solution = target->position;
    }
    ctx.blackboard.Set(position_key, aimResult.solution);
    return ExecuteResult::Success;
  }

  const char* target_player_key;
  const char* position_key;
};

struct AimAtPositionNode : public BehaviorNode {
  AimAtPositionNode(const char* target_position_key, const char* position_key)
      : target_position_key(target_position_key), position_key(position_key) {}

  ExecuteResult Execute(ExecuteContext& ctx) override {
    auto self = ctx.bot->game->player_manager.GetSelf();
    if (!self) return ExecuteResult::Failure;

    auto opt_target = ctx.blackboard.Value<Vector2f>(target_position_key);
    if (!opt_target.has_value()) return ExecuteResult::Failure;

    float weapon_speed = ctx.bot->game->connection.settings.ShipSettings[self->ship].BulletSpeed / 16.0f / 10.0f;

    Vector2f target = opt_target.value();
    ShotResult aimResult = CalculateShot(self->position, target, self->velocity, Vector2f(0.0f, 0.0f), weapon_speed);

    if (!aimResult.hit) {
      aimResult.solution = target;
    }

    ctx.blackboard.Set(position_key, aimResult.solution);

    return ExecuteResult::Success;
  }

  const char* target_position_key;
  const char* position_key;
};

struct CalculateShotSuccessQueryNode : public BehaviorNode {
  CalculateShotSuccessQueryNode(const char* target_player_key) : target_player_key(target_player_key) {}

  ExecuteResult Execute(ExecuteContext& ctx) override {
    auto self = ctx.bot->game->player_manager.GetSelf();
    if (!self) return ExecuteResult::Failure;

    auto opt_target = ctx.blackboard.Value<Player*>(target_player_key);
    if (!opt_target.has_value()) return ExecuteResult::Failure;

    float weapon_speed = ctx.bot->game->connection.settings.ShipSettings[self->ship].BulletSpeed / 16.0f / 10.0f;

    Player* target = opt_target.value();
    ShotResult aimResult =
        CalculateShot(self->position, target->position, self->velocity, target->velocity, weapon_speed);

    if (!aimResult.hit) {
      return ExecuteResult::Failure;
    }

    return ExecuteResult::Success;
  }
  
  
  const char* target_player_key;
};

}  // namespace behavior
}  // namespace zero
