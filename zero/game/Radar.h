#ifndef ZERO_RADAR_H_
#define ZERO_RADAR_H_

#include <zero/Math.h>
#include <zero/Types.h>
#include <zero/game/render/Colors.h>

namespace zero {

struct Camera;
struct Player;
struct PlayerManager;
struct PrizeGreen;
struct SpriteRenderer;
struct TileRenderer;

struct IndicatorRenderable {
  Vector2f dim;
  ColorType color;
};

enum {
  RadarIndicatorFlag_SmallMap = (1 << 0),
  RadarIndicatorFlag_FullMap = (1 << 1),

  RadarIndicatorFlag_All = RadarIndicatorFlag_SmallMap | RadarIndicatorFlag_FullMap,
};
using RadarIndicatorFlags = u32;

struct TemporaryRadarIndicator {
  IndicatorRenderable indicator;
  Vector2f world_position;
  u32 end_tick;
  RadarIndicatorFlags flags;
};

constexpr size_t kMaxTemporaryRadarIndicators = 256;

struct Radar {
  PlayerManager& player_manager;

  Radar(PlayerManager& player_manager) : player_manager(player_manager) {}

  void Update(Camera& ui_camera, short map_zoom, u16 team_freq, u16 spec_id);

  void Render(Camera& ui_camera, SpriteRenderer& renderer, TileRenderer& tile_renderer, u16 map_zoom,
              PrizeGreen* greens, size_t green_count);
  void RenderDecoy(Camera& ui_camera, SpriteRenderer& renderer, Player& self, Player& player, const Vector2f& position);

  void AddTemporaryIndicator(const Vector2f& world_position, u32 end_tick, const Vector2f& dimensions, ColorType color,
                             RadarIndicatorFlags flags = RadarIndicatorFlag_SmallMap);

  bool InRadarView(const Vector2f& position);

 private:
  struct Context {
    Vector2f radar_position;
    Vector2f radar_dim;
    Vector2f world_min;
    Vector2f world_max;
    Vector2f world_dim;

    Vector2f min_uv;
    Vector2f max_uv;

    float team_freq;
    u16 spec_id;
  };

  Context ctx;

  size_t temporary_indicator_count = 0;
  TemporaryRadarIndicator temporary_indicators[kMaxTemporaryRadarIndicators];

  void RenderPlayer(Camera& ui_camera, SpriteRenderer& renderer, Player& self, Player& player);
  void RenderPlayers(Camera& ui_camera, SpriteRenderer& renderer, Player& self);

  void RenderIndicator(Camera& ui_camera, SpriteRenderer& renderer, const Vector2f& position,
    const IndicatorRenderable& indicator);

  void RenderTime(Camera& ui_camera, SpriteRenderer& renderer, const Vector2f& radar_position, Player& self);

  IndicatorRenderable GetIndicator(Player& self, Player& player);
};

}  // namespace zero

#endif
