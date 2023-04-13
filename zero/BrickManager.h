#ifndef ZERO_BRICKMANAGER_H_
#define ZERO_BRICKMANAGER_H_

#include <zero/Camera.h>
#include <zero/HashMap.h>
#include <zero/Math.h>
#include <zero/Types.h>

namespace zero {

struct Camera;
struct Connection;
struct Map;
struct PacketDispatcher;
struct PlayerManager;
struct SpriteRenderer;

struct BrickTile {
  u16 x;
  u16 y;

  BrickTile() : x(0), y(0) {}
  BrickTile(u16 x, u16 y) : x(x), y(y) {}

  bool operator==(const BrickTile& other) { return x == other.x && y == other.y; }
};

struct Brick {
  BrickTile tile;
  u16 id;
  u16 team;
  u32 end_tick;

  struct Brick* next;
};

struct BrickHasher {
  inline u32 operator()(const BrickTile& tile) { return Hash(tile); }
  inline u32 Hash(const BrickTile& tile) { return (tile.y << 16) | tile.x; }
};

constexpr size_t kBrickMapBuckets = (1 << 8);
struct BrickMap : public HashMap<BrickTile, Brick*, BrickHasher, kBrickMapBuckets> {
  BrickMap(MemoryArena& arena) : HashMap<BrickTile, Brick*, BrickHasher, kBrickMapBuckets>(arena) {}
};

struct BrickManager {
  MemoryArena& arena;
  Connection& connection;
  PlayerManager& player_manager;

  Brick* bricks = nullptr;
  Brick* free = nullptr;

  BrickMap brick_map;

  BrickManager(MemoryArena& arena, Connection& connection, PlayerManager& player_manager, PacketDispatcher& dispatcher);

  void Update(Map& map, u32 frequency, float dt);
  void InsertBrick(u16 x, u16 y, u16 team, u16 id, u32 timestamp);
  void Clear();

  inline Brick* GetBrick(u16 x, u16 y) {
    Brick** brick = brick_map.Find(BrickTile(x, y));
    return brick ? *brick : nullptr;
  }
};

}  // namespace zero

#endif
