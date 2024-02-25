#include "Graphics.h"

#include <zero/game/Logger.h>
#include <zero/game/render/Colors.h>
#include <zero/game/render/SpriteRenderer.h>

#include <cstdio>

// TODO: This entire thing needs to be redone as a database. It should also probably defer loading and creation graphics
// until LVZ is loaded so they can override it first.

namespace zero {

constexpr float kBombAnimDuration = 1.0f;
constexpr float kMineAnimDuration = 1.0f;
constexpr float kShrapnelAnimDuration = 0.6f;

Colors Graphics::colors;
SpriteRenderable* Graphics::text_sprites = nullptr;
SpriteRenderable* Graphics::textf_sprites = nullptr;
SpriteRenderable* Graphics::energyfont_sprites = nullptr;

SpriteRenderable* Graphics::healthbar_sprites = nullptr;

SpriteRenderable* Graphics::ship_sprites = nullptr;
SpriteRenderable* Graphics::turret_sprites = nullptr;
SpriteRenderable* Graphics::spectate_sprites = nullptr;

SpriteRenderable* Graphics::warp_sprites = nullptr;

SpriteRenderable* Graphics::explode0_sprites = nullptr;
SpriteRenderable* Graphics::explode1_sprites = nullptr;
SpriteRenderable* Graphics::explode2_sprites = nullptr;
SpriteRenderable* Graphics::emp_burst_sprites = nullptr;

SpriteRenderable* Graphics::bomb_sprites = nullptr;
SpriteRenderable* Graphics::bomb_trail_sprites = nullptr;
SpriteRenderable* Graphics::mine_sprites = nullptr;
SpriteRenderable* Graphics::shrapnel_sprites = nullptr;
SpriteRenderable* Graphics::bombflash_sprites = nullptr;
SpriteRenderable* Graphics::emp_spark_sprites = nullptr;

SpriteRenderable* Graphics::bullet_sprites = nullptr;
SpriteRenderable* Graphics::bullet_trail_sprites = nullptr;

SpriteRenderable* Graphics::repel_sprites = nullptr;
SpriteRenderable* Graphics::brick_sprites = nullptr;
SpriteRenderable* Graphics::portal_sprites = nullptr;
SpriteRenderable* Graphics::super_sprites = nullptr;
SpriteRenderable* Graphics::shield_sprites = nullptr;
SpriteRenderable* Graphics::flag_indicator_sprites = nullptr;

SpriteRenderable* Graphics::icon_sprites = nullptr;
SpriteRenderable Graphics::empty_icon_sprites[2];
SpriteRenderable* Graphics::icon_count_sprites = nullptr;

SpriteRenderable* Graphics::exhaust_sprites = nullptr;
SpriteRenderable* Graphics::rocket_sprites = nullptr;

SpriteRenderable* Graphics::prize_sprites = nullptr;
SpriteRenderable* Graphics::flag_sprites = nullptr;
SpriteRenderable* Graphics::goal_sprites = nullptr;
SpriteRenderable* Graphics::asteroid_small1_sprites = nullptr;
SpriteRenderable* Graphics::asteroid_small2_sprites = nullptr;
SpriteRenderable* Graphics::asteroid_large_sprites = nullptr;
SpriteRenderable* Graphics::space_station_sprites = nullptr;
SpriteRenderable* Graphics::wormhole_sprites = nullptr;

SpriteRenderable* Graphics::powerball_sprites = nullptr;

AnimatedSprite Graphics::anim_health_high;
AnimatedSprite Graphics::anim_health_medium;
AnimatedSprite Graphics::anim_health_low;

AnimatedSprite Graphics::anim_bombs[4];
AnimatedSprite Graphics::anim_emp_bombs[4];
AnimatedSprite Graphics::anim_bombs_bounceable[4];
AnimatedSprite Graphics::anim_bomb_explode;
AnimatedSprite Graphics::anim_emp_explode;
AnimatedSprite Graphics::anim_thor;
AnimatedSprite Graphics::anim_bomb_trails[4];
AnimatedSprite Graphics::anim_bombflash;
AnimatedSprite Graphics::anim_emp_spark;

AnimatedSprite Graphics::anim_mines[4];
AnimatedSprite Graphics::anim_emp_mines[4];

AnimatedSprite Graphics::anim_shrapnel[3];
AnimatedSprite Graphics::anim_bounce_shrapnel[3];

AnimatedSprite Graphics::anim_bullets[4];
AnimatedSprite Graphics::anim_bullet_explode;
AnimatedSprite Graphics::anim_bullets_bounce[4];
AnimatedSprite Graphics::anim_bullet_trails[4];
AnimatedSprite Graphics::anim_burst_inactive;
AnimatedSprite Graphics::anim_burst_active;

AnimatedSprite Graphics::anim_repel;
AnimatedSprite Graphics::anim_enemy_brick;
AnimatedSprite Graphics::anim_team_brick;
AnimatedSprite Graphics::anim_portal;
AnimatedSprite Graphics::anim_super;
AnimatedSprite Graphics::anim_shield;
AnimatedSprite Graphics::anim_flag_indicator;

AnimatedSprite Graphics::anim_ship_explode;
AnimatedSprite Graphics::anim_ship_warp;
AnimatedSprite Graphics::anim_ship_exhaust;
AnimatedSprite Graphics::anim_ship_rocket;

AnimatedSprite Graphics::anim_prize;
AnimatedSprite Graphics::anim_flag;
AnimatedSprite Graphics::anim_flag_team;
AnimatedSprite Graphics::anim_goal;
AnimatedSprite Graphics::anim_goal_team;
AnimatedSprite Graphics::anim_asteroid_small1;
AnimatedSprite Graphics::anim_asteroid_small2;
AnimatedSprite Graphics::anim_asteroid_large;
AnimatedSprite Graphics::anim_space_station;
AnimatedSprite Graphics::anim_wormhole;

AnimatedSprite Graphics::anim_powerball;
AnimatedSprite Graphics::anim_powerball_phased;
AnimatedSprite Graphics::anim_powerball_trail;

SpriteRenderable* Graphics::character_set[256] = {};

SpriteRenderable* LoadTileSheet(SpriteRenderer& renderer, const char* sheetname, const Vector2f& dimensions,
                                int* count) {
  // Add extension lookup for the non-standard installs.
  const char* kExtensions[] = {"bm2", "png", "gif"};
  char path[256];
  SpriteRenderable* renderables = nullptr;

  for (size_t i = 0; i < sizeof(kExtensions) / sizeof(*kExtensions); ++i) {
    sprintf(path, "graphics/%s.%s", sheetname, kExtensions[i]);

    renderables = renderer.LoadSheet(path, dimensions, count);

    if (renderables != nullptr) {
      return renderables;
    }
  }

  if (renderables == nullptr) {
    Log(LogLevel::Error, "Failed to load %s graphic.", sheetname);
  }

  return renderables;
}

bool Graphics::Initialize(SpriteRenderer& renderer) {
  int count;

  if (!InitializeFont(renderer)) {
    return false;
  }

  if (!InitializeWeapons(renderer)) {
    return false;
  }

  if (!InitializeTiles(renderer)) {
    return false;
  }

  SpriteRenderable* color_sprites =
      LoadTileSheet(renderer, "colors", Vector2f(kColorTextureWidth, kColorTextureHeight), &count);
  if (!color_sprites) return false;

  colors.texture_id = color_sprites->texture;
  colors.texture_dimensions = color_sprites->dimensions;

  icon_sprites = LoadTileSheet(renderer, "Icons", Vector2f(26, 24), &count);
  if (!icon_sprites) return false;

  empty_icon_sprites[0] = empty_icon_sprites[1] = icon_sprites[0];
  empty_icon_sprites[0].dimensions = Vector2f(4, 24);

  float empty_uv_top = 120.0f / 144.0f;
  float empty_left_xstart = 152.0f / 234.0f;
  float empty_left_xend = 156.0f / 234.0f;

  empty_icon_sprites[0].uvs[0] = Vector2f(empty_left_xstart, empty_uv_top);
  empty_icon_sprites[0].uvs[1] = Vector2f(empty_left_xend, empty_uv_top);
  empty_icon_sprites[0].uvs[2] = Vector2f(empty_left_xstart, 1.0f);
  empty_icon_sprites[0].uvs[3] = Vector2f(empty_left_xend, 1.0f);

  float empty_right_xstart = 78.0f / 234.0f;
  float empty_right_xend = 82.0f / 234.0f;

  empty_icon_sprites[1].dimensions = Vector2f(4, 24);
  empty_icon_sprites[1].uvs[0] = Vector2f(empty_right_xstart, empty_uv_top);
  empty_icon_sprites[1].uvs[1] = Vector2f(empty_right_xend, empty_uv_top);
  empty_icon_sprites[1].uvs[2] = Vector2f(empty_right_xstart, 1.0f);
  empty_icon_sprites[1].uvs[3] = Vector2f(empty_right_xend, 1.0f);

  icon_count_sprites = LoadTileSheet(renderer, "icondoor", Vector2f(11, 13), &count);
  if (!icon_count_sprites) return false;

  ship_sprites = LoadTileSheet(renderer, "ships", Vector2f(36, 36), &count);
  if (!ship_sprites) return false;

  turret_sprites = LoadTileSheet(renderer, "turret2", Vector2f(16, 16), &count);
  if (!turret_sprites) return false;

  spectate_sprites = LoadTileSheet(renderer, "spectate", Vector2f(8, 8), &count);
  if (!spectate_sprites) return false;

  warp_sprites = LoadTileSheet(renderer, "warp", Vector2f(48, 48), &count);
  if (!warp_sprites) return false;

  anim_ship_warp.frames = warp_sprites;
  anim_ship_warp.frame_count = count;
  anim_ship_warp.duration = 0.5f;

  explode0_sprites = LoadTileSheet(renderer, "explode0", Vector2f(16, 16), &count);
  if (!explode0_sprites) return false;

  anim_bullet_explode.frames = explode0_sprites;
  anim_bullet_explode.frame_count = count;
  anim_bullet_explode.duration = 0.4f;

  explode1_sprites = LoadTileSheet(renderer, "explode1", Vector2f(48, 48), &count);
  if (!explode1_sprites) return false;

  anim_ship_explode.frames = explode1_sprites;
  anim_ship_explode.frame_count = count;
  anim_ship_explode.duration = 0.8f;

  explode2_sprites = LoadTileSheet(renderer, "explode2", Vector2f(80, 80), &count);
  if (!explode2_sprites) return false;

  CreateBombExplodeAnimations(explode2_sprites, count);

  emp_burst_sprites = LoadTileSheet(renderer, "empburst", Vector2f(80, 80), &count);
  if (!emp_burst_sprites) return false;

  CreateEmpExplodeAnimations(emp_burst_sprites, count);

  exhaust_sprites = LoadTileSheet(renderer, "exhaust", Vector2f(16, 16), &count);
  if (!exhaust_sprites) return false;

  CreateExhaustAnimations(exhaust_sprites, count);

  rocket_sprites = LoadTileSheet(renderer, "rocket", Vector2f(24, 24), &count);
  if (!rocket_sprites) return false;

  CreateRocketAnimations(rocket_sprites, count);

  healthbar_sprites = LoadTileSheet(renderer, "hlthbar", Vector2f(116, 28), &count);
  if (!healthbar_sprites) return false;

  powerball_sprites = LoadTileSheet(renderer, "powerb", Vector2f(16, 16), &count);
  if (!powerball_sprites) return false;

  anim_powerball.duration = 1.0f;
  anim_powerball.frames = powerball_sprites;
  anim_powerball.frame_count = count / 3;

  anim_powerball_phased.duration = 1.0f;
  anim_powerball_phased.frames = powerball_sprites + anim_powerball.frame_count;
  anim_powerball_phased.frame_count = count / 3;

  anim_powerball_trail.duration = 0.3f;
  anim_powerball_trail.frames = powerball_sprites + anim_powerball.frame_count + anim_powerball_phased.frame_count;
  anim_powerball_trail.frame_count = count / 3;

  return true;
}

bool Graphics::InitializeFont(SpriteRenderer& renderer) {
  int count;

  text_sprites = LoadTileSheet(renderer, "tallfont", Vector2f(8, 12), &count);
  if (!text_sprites) return false;

  textf_sprites = LoadTileSheet(renderer, "tallfontf", Vector2f(8, 12), &count);
  if (!textf_sprites) return false;

  energyfont_sprites = LoadTileSheet(renderer, "engyfont", Vector2f(16, 24), &count);
  if (!energyfont_sprites) return false;

  for (size_t i = ' '; i < '~'; ++i) {
    character_set[i] = Graphics::text_sprites + i - ' ';
  }

  for (size_t i = 0xC0; i < 0xD7; ++i) {
    character_set[i] = Graphics::textf_sprites + i - 0xC0 + 5;
  }

  size_t index = 28;
  for (size_t i = 0xD8; i < 0xF7; ++i) {
    character_set[i] = Graphics::textf_sprites + index++;
  }

  index = 59;
  // 0xFF maps to _ on asss due to a bug with asss (should be y with diaeresis)
  for (size_t i = 0xF8; i <= 0xFF; ++i) {
    character_set[i] = Graphics::textf_sprites + index++;
  }

  character_set[0xDF] = Graphics::text_sprites + '~' - ' ' + 1;

  // windows-1252 encoding
  character_set[0x80] = Graphics::textf_sprites + 35;
  character_set[0x8A] = Graphics::textf_sprites;
  character_set[0x8E] = Graphics::textf_sprites + 1;
  character_set[0x9A] = Graphics::textf_sprites + 2;
  character_set[0x9E] = Graphics::textf_sprites + 3;
  character_set[0x9F] = Graphics::textf_sprites + 4;

  return true;
}

bool Graphics::InitializeWeapons(SpriteRenderer& renderer) {
  int count;

  bombflash_sprites = LoadTileSheet(renderer, "bombflsh", Vector2f(8, 8), &count);
  if (!bombflash_sprites) return false;

  anim_bombflash.frames = bombflash_sprites;
  anim_bombflash.frame_count = 6;
  anim_bombflash.duration = 0.12f;

  bomb_sprites = LoadTileSheet(renderer, "bombs", Vector2f(16, 16), &count);
  if (!bomb_sprites) return false;

  CreateBombAnimations(bomb_sprites, count);

  bomb_trail_sprites = LoadTileSheet(renderer, "trail", Vector2f(16, 16), &count);
  if (!bomb_trail_sprites) return false;

  CreateBombTrailAnimations(bomb_trail_sprites, count);

  mine_sprites = LoadTileSheet(renderer, "mines", Vector2f(16, 16), &count);
  if (!mine_sprites) return false;

  CreateMineAnimations(mine_sprites, count);

  shrapnel_sprites = LoadTileSheet(renderer, "shrapnel", Vector2f(8, 8), &count);
  if (!shrapnel_sprites) return false;

  for (size_t i = 0; i < 3; ++i) {
    anim_shrapnel[i].frames = shrapnel_sprites + i * 10;
    anim_shrapnel[i].frame_count = 10;
    anim_shrapnel[i].duration = kShrapnelAnimDuration;
  }

  for (size_t i = 0; i < 3; ++i) {
    anim_bounce_shrapnel[i].frames = shrapnel_sprites + i * 10 + 30;
    anim_bounce_shrapnel[i].frame_count = 10;
    anim_bounce_shrapnel[i].duration = kShrapnelAnimDuration;
  }

  bullet_sprites = LoadTileSheet(renderer, "bullets", Vector2f(5, 5), &count);
  if (!bullet_sprites) return false;

  CreateBulletAnimations(bullet_sprites, count);

  bullet_trail_sprites = LoadTileSheet(renderer, "gradient", Vector2f(1, 1), &count);
  if (!bullet_trail_sprites) return false;

  CreateBulletTrailAnimations(bullet_trail_sprites, count);

  anim_health_high.frames = bullet_trail_sprites;
  anim_health_high.frame_count = 14;
  anim_health_high.duration = 1.4f;

  anim_health_medium.frames = bullet_trail_sprites + 14;
  anim_health_medium.frame_count = 14;
  anim_health_medium.duration = 1.4f;

  anim_health_low.frames = bullet_trail_sprites + 28;
  anim_health_low.frame_count = 14;
  anim_health_low.duration = 1.4f;

  repel_sprites = LoadTileSheet(renderer, "repel", Vector2f(96, 96), &count);
  if (!repel_sprites) return false;

  CreateRepelAnimations(repel_sprites, count);

  brick_sprites = LoadTileSheet(renderer, "wall", Vector2f(16, 16), &count);
  if (!brick_sprites) return false;

  CreateBrickAnimations(brick_sprites, count);

  portal_sprites = LoadTileSheet(renderer, "warppnt", Vector2f(16, 16), &count);
  if (!portal_sprites) return false;

  CreatePortalAnimations(portal_sprites, count);

  emp_spark_sprites = LoadTileSheet(renderer, "spark", Vector2f(40, 40), &count);
  if (!emp_spark_sprites) return false;

  CreateEmpSparkAnimations(emp_spark_sprites, count);

  super_sprites = LoadTileSheet(renderer, "super", Vector2f(16, 16), &count);
  if (!super_sprites) return false;

  anim_super.duration = 1.0f;
  anim_super.frames = super_sprites;
  anim_super.frame_count = count;

  shield_sprites = LoadTileSheet(renderer, "shield", Vector2f(16, 16), &count);
  if (!shield_sprites) return false;

  anim_shield.duration = 1.0f;
  anim_shield.frames = shield_sprites;
  anim_shield.frame_count = count;

  flag_indicator_sprites = LoadTileSheet(renderer, "dropflag", Vector2f(16, 16), &count);
  if (!flag_indicator_sprites) return false;

  anim_flag_indicator.duration = 1.0f;
  anim_flag_indicator.frames = flag_indicator_sprites;
  anim_flag_indicator.frame_count = count;

  return true;
}

void Graphics::CreatePortalAnimations(SpriteRenderable* renderables, int count) {
  anim_portal.duration = 1.0f;
  anim_portal.frames = portal_sprites;
  anim_portal.frame_count = count;
}

void Graphics::CreateBombAnimations(SpriteRenderable* renderables, int count) {
  for (size_t i = 0; i < 4; ++i) {
    anim_bombs[i].frames = renderables + i * 10;
    anim_bombs[i].frame_count = 10;
    anim_bombs[i].duration = kBombAnimDuration;
  }

  for (size_t i = 0; i < 4; ++i) {
    anim_emp_bombs[i].frames = renderables + i * 10 + 40;
    anim_emp_bombs[i].frame_count = 10;
    anim_emp_bombs[i].duration = kBombAnimDuration;
  }

  for (size_t i = 0; i < 4; ++i) {
    anim_bombs_bounceable[i].frames = renderables + i * 10 + 80;
    anim_bombs_bounceable[i].frame_count = 10;
    anim_bombs_bounceable[i].duration = kBombAnimDuration;
  }

  anim_thor.frames = renderables + 120;
  anim_thor.frame_count = 10;
  anim_thor.duration = kBombAnimDuration;
}

void Graphics::CreateMineAnimations(SpriteRenderable* renderables, int count) {
  for (size_t i = 0; i < 4; ++i) {
    anim_mines[i].frames = mine_sprites + i * 10;
    anim_mines[i].frame_count = 10;
    anim_mines[i].duration = kMineAnimDuration;
  }

  for (size_t i = 0; i < 4; ++i) {
    anim_emp_mines[i].frames = mine_sprites + i * 10 + 40;
    anim_emp_mines[i].frame_count = 10;
    anim_emp_mines[i].duration = kMineAnimDuration;
  }
}

void Graphics::CreateBombTrailAnimations(SpriteRenderable* renderables, int count) {
  for (size_t i = 0; i < 4; ++i) {
    anim_bomb_trails[i].frames = renderables + i * 10;
    anim_bomb_trails[i].frame_count = 10;
    anim_bomb_trails[i].duration = 0.35f;
  }
}

void Graphics::CreateBombExplodeAnimations(SpriteRenderable* renderables, int count) {
  anim_bomb_explode.frames = renderables;
  anim_bomb_explode.frame_count = count;
  anim_bomb_explode.duration = 1.25f;
}

void Graphics::CreateEmpExplodeAnimations(SpriteRenderable* renderables, int count) {
  anim_emp_explode.frames = renderables;
  anim_emp_explode.frame_count = count;
  anim_emp_explode.duration = 0.5f;
}

void Graphics::CreateEmpSparkAnimations(SpriteRenderable* renderables, int count) {
  anim_emp_spark.frames = renderables;
  anim_emp_spark.frame_count = count;
  anim_emp_spark.duration = 0.5f;
}

void Graphics::CreateBulletAnimations(SpriteRenderable* renderables, int count) {
  for (size_t i = 0; i < 4; ++i) {
    anim_bullets[i].frames = renderables + i * 4;
    anim_bullets[i].frame_count = 4;
    anim_bullets[i].duration = 0.15f;
  }

  for (size_t i = 0; i < 4; ++i) {
    anim_bullets_bounce[i].frames = renderables + i * 4 + 20;
    anim_bullets_bounce[i].frame_count = 4;
    anim_bullets_bounce[i].duration = 0.15f;
  }

  anim_burst_inactive.frames = renderables + 16;
  anim_burst_inactive.frame_count = 4;
  anim_burst_inactive.duration = 0.15f;

  anim_burst_active.frames = renderables + count - 4;
  anim_burst_active.frame_count = 4;
  anim_burst_active.duration = 0.15f;
}

void Graphics::CreateBulletTrailAnimations(SpriteRenderable* renderables, int count) {
  for (size_t i = 0; i < 3; ++i) {
    anim_bullet_trails[i].frames = renderables + i * 14 + 3 * 14;
    anim_bullet_trails[i].frame_count = 14;
    anim_bullet_trails[i].duration = 0.14f;
  }
  anim_bullet_trails[3] = anim_bullet_trails[2];
}

void Graphics::CreateRepelAnimations(SpriteRenderable* renderables, int count) {
  anim_repel.duration = 0.5f;
  anim_repel.frames = renderables;
  anim_repel.frame_count = count;
}

void Graphics::CreatePrizeAnimations(SpriteRenderable* renderables, int count) {
  anim_prize.duration = 1.0f;
  anim_prize.frames = renderables;
  anim_prize.frame_count = count;
}

void Graphics::CreateFlagAnimations(SpriteRenderable* renderables, int count) {
  anim_flag.duration = 1.0f;
  anim_flag.frames = renderables;
  anim_flag.frame_count = count / 2;

  anim_flag_team.duration = 1.0f;
  anim_flag_team.frames = renderables + count / 2;
  anim_flag_team.frame_count = count / 2;
}

void Graphics::CreateExhaustAnimations(SpriteRenderable* renderables, int count) {
  anim_ship_exhaust.duration = 0.3f;
  anim_ship_exhaust.frames = renderables;
  anim_ship_exhaust.frame_count = count / 2;
}

void Graphics::CreateRocketAnimations(SpriteRenderable* renderables, int count) {
  anim_ship_rocket.duration = 0.3f;
  anim_ship_rocket.frames = renderables;
  anim_ship_rocket.frame_count = count;
}

void Graphics::CreateBrickAnimations(SpriteRenderable* renderables, int count) {
  anim_enemy_brick.duration = 1.0f;
  anim_enemy_brick.frames = renderables;
  anim_enemy_brick.frame_count = count / 2;

  anim_team_brick.duration = 1.0f;
  anim_team_brick.frames = renderables + (count / 2);
  anim_team_brick.frame_count = count / 2;
}

bool Graphics::InitializeTiles(SpriteRenderer& renderer) {
  constexpr float kAsteroidDuration = 1.5f;

  int count;

  asteroid_small1_sprites = LoadTileSheet(renderer, "over1", Vector2f(16, 16), &count);
  if (!asteroid_small1_sprites) return false;

  anim_asteroid_small1.duration = kAsteroidDuration;
  anim_asteroid_small1.frames = asteroid_small1_sprites;
  anim_asteroid_small1.frame_count = count;

  asteroid_small2_sprites = LoadTileSheet(renderer, "over3", Vector2f(16, 16), &count);
  if (!asteroid_small2_sprites) return false;

  anim_asteroid_small2.duration = kAsteroidDuration;
  anim_asteroid_small2.frames = asteroid_small2_sprites;
  anim_asteroid_small2.frame_count = count;

  asteroid_large_sprites = LoadTileSheet(renderer, "over2", Vector2f(32, 32), &count);
  if (!asteroid_large_sprites) return false;

  anim_asteroid_large.duration = kAsteroidDuration;
  anim_asteroid_large.frames = asteroid_large_sprites;
  anim_asteroid_large.frame_count = count;

  space_station_sprites = LoadTileSheet(renderer, "over4", Vector2f(96, 96), &count);
  if (!space_station_sprites) return false;

  anim_space_station.duration = 1.0f;
  anim_space_station.frames = space_station_sprites;
  anim_space_station.frame_count = count;

  wormhole_sprites = LoadTileSheet(renderer, "over5", Vector2f(80, 80), &count);
  if (!wormhole_sprites) return false;

  anim_wormhole.duration = 2.5f;
  anim_wormhole.frames = wormhole_sprites;
  anim_wormhole.frame_count = count;

  prize_sprites = LoadTileSheet(renderer, "prizes", Vector2f(16, 16), &count);
  if (!prize_sprites) return false;

  CreatePrizeAnimations(prize_sprites, count);

  flag_sprites = LoadTileSheet(renderer, "flag", Vector2f(16, 16), &count);
  if (!flag_sprites) return false;

  CreateFlagAnimations(flag_sprites, count);

  goal_sprites = LoadTileSheet(renderer, "goal", Vector2f(16, 16), &count);
  if (!goal_sprites) return false;

  anim_goal.duration = 0.5f;
  anim_goal.frames = goal_sprites + count / 2;
  anim_goal.frame_count = count / 2;

  anim_goal_team.duration = 0.5f;
  anim_goal_team.frames = goal_sprites;
  anim_goal_team.frame_count = count / 2;

  return true;
}

void Graphics::DrawBorder(SpriteRenderer& renderer, Camera& camera, const Vector2f& center,
                          const Vector2f& half_extents) {
  SpriteRenderable renderable = colors.GetRenderable(ColorType::Border1, Vector2f(1, half_extents.y * 2 + 2));

  renderer.Draw(camera, renderable, center + Vector2f(-half_extents.x - 1, -half_extents.y - 1), Layer::TopMost);
  renderer.Draw(camera, renderable, center + Vector2f(half_extents.x, -half_extents.y - 1), Layer::TopMost);

  renderable.dimensions = Vector2f(half_extents.x * 2 + 2, 1);

  renderer.Draw(camera, renderable, center + Vector2f(-half_extents.x - 1, -half_extents.y - 1), Layer::TopMost);
  renderer.Draw(camera, renderable, center + Vector2f(-half_extents.x - 1, half_extents.y), Layer::TopMost);

  renderable = colors.GetRenderable(ColorType::Border2, Vector2f(1, half_extents.y * 2 + 4));

  renderer.Draw(camera, renderable, center + Vector2f(-half_extents.x - 2, -half_extents.y - 2), Layer::TopMost);
  renderer.Draw(camera, renderable, center + Vector2f(half_extents.x + 1, -half_extents.y - 2), Layer::TopMost);

  renderable.dimensions = Vector2f(half_extents.x * 2 + 4, 1);

  renderer.Draw(camera, renderable, center + Vector2f(-half_extents.x - 2, -half_extents.y - 2), Layer::TopMost);
  renderer.Draw(camera, renderable, center + Vector2f(-half_extents.x - 2, half_extents.y + 1), Layer::TopMost);

  renderable = colors.GetRenderable(ColorType::Border3, Vector2f(1, half_extents.y * 2 + 2));

  renderer.Draw(camera, renderable, center + Vector2f(-half_extents.x - 3, -half_extents.y - 1), Layer::TopMost);
  renderer.Draw(camera, renderable, center + Vector2f(half_extents.x + 2, -half_extents.y - 1), Layer::TopMost);

  renderable.dimensions = Vector2f(half_extents.x * 2 + 2, 1);

  renderer.Draw(camera, renderable, center + Vector2f(-half_extents.x - 1, -half_extents.y - 3), Layer::TopMost);
  renderer.Draw(camera, renderable, center + Vector2f(-half_extents.x - 1, half_extents.y + 2), Layer::TopMost);
}

}  // namespace zero
