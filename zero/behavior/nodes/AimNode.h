#pragma once

#include <zero/ZeroBot.h>
#include <zero/behavior/BehaviorTree.h>
#include <zero/game/Game.h>

#include <optional>

#define RENDER_AIM 0

namespace zero {
namespace behavior {

inline std::optional<Vector2f> CalculateShot(const Vector2f& pShooter, const Vector2f& pTarget,
                                             const Vector2f& vShooter, const Vector2f& vTarget, float sProjectile) {
  Vector2f totarget = pTarget - pShooter;
  Vector2f v = vTarget - vShooter;

  float a = v.Dot(v) - (sProjectile * sProjectile);
  float b = 2.0f * v.Dot(totarget);
  float c = totarget.Dot(totarget);

  Vector2f solution;

  float disc = (b * b) - 4.0f * a * c;
  float t = 0.0f;

  if (disc <= 0.0 || a == 0.0f) return std::nullopt;

  float t1 = (-b + sqrtf(disc)) / (2.0f * a);
  float t2 = (-b - sqrtf(disc)) / (2.0f * a);

  t = std::min(t1, t2);
  if (t < 0.0f) t = std::max(t1, t2);   
  if (t < 0.0f) return std::nullopt;

  solution = pTarget + (v * t);

  return std::optional(solution);
}

inline float GetWeaponSpeed(Game& game, Player& player, WeaponType type) {
  float weapon_speed = 0.0f;

  switch (type) {
    case WeaponType::Bomb:
    case WeaponType::ProximityBomb:
    case WeaponType::Thor: {
      weapon_speed = game.connection.settings.ShipSettings[player.ship].BombSpeed / 16.0f / 10.0f;
    } break;
    case WeaponType::Bullet:
    case WeaponType::BouncingBullet: {
      weapon_speed = game.connection.settings.ShipSettings[player.ship].BulletSpeed / 16.0f / 10.0f;
    } break;
    case WeaponType::Decoy: {
      // Do nothing since it just matches the player's velocity.
    } break;
    default: {
    } break;
  }

  return weapon_speed;
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

    float weapon_speed = GetWeaponSpeed(*ctx.bot->game, *player, weapon_type);
    Vector2f velocity = player->velocity + player->GetHeading() * weapon_speed;

    ctx.blackboard.Set(velocity_key, velocity);

    return ExecuteResult::Success;
  }

  const char* player_key;
  const char* velocity_key;
  WeaponType weapon_type;
};

struct AimNode : public BehaviorNode {
  AimNode(WeaponType weapon_type, const char* target_player_key, const char* position_key)
      : weapon_type(weapon_type), target_player_key(target_player_key), position_key(position_key) {}

  ExecuteResult Execute(ExecuteContext& ctx) override {
    auto self = ctx.bot->game->player_manager.GetSelf();
    if (!self) return ExecuteResult::Failure;

    auto opt_target = ctx.blackboard.Value<Player*>(target_player_key);
    if (!opt_target.has_value()) return ExecuteResult::Failure;

    float weapon_speed = GetWeaponSpeed(*ctx.bot->game, *self, weapon_type);

    Player* target = opt_target.value();

    Vector2f direction = Normalize(target->position - self->position);
    float amount = target->velocity.Dot(direction);
    // Remove the "away" velocity from the target so it's only moving side to side.
    Vector2f horizontal_vel = target->velocity - direction * amount;

    std::optional<Vector2f> calculated_shot =
      //  CalculateShot(self->position, target->position, self->velocity, horizontal_vel, weapon_speed);
    CalculateShot(self->position, target->position, self->velocity, target->velocity, weapon_speed);

    // Default to target's position if the calculated shot fails.
    Vector2f aimshot = target->position;

    if (calculated_shot.has_value()) {
      aimshot = *calculated_shot;

      // Set the aimshot directly to the player position if it is too far away.
      if (aimshot.DistanceSq(target->position) > 20.0f * 20.0f) {
       // aimshot = target->position;
      }
    }

#if RENDER_AIM
    float radius = ctx.bot->game->connection.settings.ShipSettings[target->ship].GetRadius();

    Vector2f half_extents(radius, radius);
    ctx.bot->game->line_renderer.PushRect(aimshot - half_extents, aimshot + half_extents,
                                 Vector3f(0.0f, 1.0f, 0.0f));
    ctx.bot->game->line_renderer.Render(ctx.bot->game->camera);
#endif  // RENDER_AIM

    ctx.blackboard.Set(position_key, aimshot);

    return ExecuteResult::Success;
  }

  WeaponType weapon_type;
  const char* target_player_key;
  const char* position_key;
};

}  // namespace behavior
}  // namespace zero
