#pragma once

#ifndef ENUMS_H
#define ENUMS_H

namespace zero {
enum class UpdateState : short { Clear, Wait, Reload };
enum class ItemTransaction : short { Buy, Sell, DepotBuy, DepotSell, ListItems, None };
enum class Role : short { Anchor, Rusher, Bomber, Gunner, Flagger, Turret, EMP, PowerBaller, None };
enum class Area : short { Center, Base, None };
enum class BaseDuelState : short { Running, Paused, Ending, NotRunning };
//enum class CommandRequestType : short { ShipChange, ArenaChange, FreqChange, None };
enum class AnchorType : short { Summoner, Evoker, None };
enum class Zone {
  Local,
  Subgame,
  Hyperspace,
  Devastation,
  MetalGear,
  ExtremeGames,

  Unknown,
  Count
};
}
#endif