#ifndef ZERO_RENDER_ANIMATEDTILERENDERER_H_
#define ZERO_RENDER_ANIMATEDTILERENDERER_H_

#include <zero/game/render/Animation.h>

namespace zero {

struct Camera;
struct Map;
struct Soccer;
struct SpriteRenderer;
struct TileRenderer;
struct Vector2f;

struct AnimatedTileRenderer {
  // Store these here instead of in the animation system to keep them all in sync
  Animation anim_prize;
  Animation anim_flag;
  Animation anim_flag_team;
  Animation anim_goal;
  Animation anim_goal_team;
  Animation anim_asteroid_small1;
  Animation anim_asteroid_small2;
  Animation anim_asteroid_large;
  Animation anim_space_station;
  Animation anim_wormhole;
  Animation anim_doors[2];

  SpriteRenderable door_renderables[8];
  AnimatedSprite door_sprites[2];

  void Initialize();
  void InitializeDoors(TileRenderer& tile_renderer);
  void Update(float dt);

  void Render(SpriteRenderer& renderer, Map& map, Camera& camera, const Vector2f& screen_dim, struct GameFlag* flags,
              size_t flag_count, struct PrizeGreen* greens, size_t green_count, u32 freq, Soccer& soccer);
};

}  // namespace zero

#endif
