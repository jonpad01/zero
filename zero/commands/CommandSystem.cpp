#include "CommandSystem.h"

#include <zero/Common.h>
#include <zero/ZeroBot.h>
#include <zero/commands/CombatRoleCommands.h>
#include <zero/commands/ConsumableCommands.h>
#include <zero/commands/CoreCommands.h>
#include <zero/commands/DevastationCommands.h>
#include <zero/commands/HyperSpaceCommands.h>
#include <zero/commands/LockCommand.h>
#include <zero/commands/LockFreqCommand.h>
#include <zero/commands/SetArenaCommand.h>
#include <zero/commands/SetFreqCommand.h>
#include <zero/commands/SetShipCommand.h>
#include <zero/commands/StatusCommands.h>
#include <zero/commands/SayCommand.h>
#include <zero/game/Game.h>
#include <zero/game/net/PacketDispatcher.h>

namespace zero {

constexpr int kArenaSecurityLevel = 5;

const std::unordered_map<std::string, int> kOperators = {
    {"tm_master", 10}, {"baked cake", 10}, {"x-demo", 10}, {"lyra.", 5}, {"profile", 5}, {"monkey", 10},
    {"neostar", 5},    {"geekgrrl", 5},    {"sed", 5},     {"sk", 5},    {"b.o.x.", 10}};

static void RawOnChatPacket(void* user, u8* pkt, size_t size) {
  CommandSystem* cmd = (CommandSystem*)user;

  cmd->OnChatPacket(pkt, size);
}

CommandSystem::CommandSystem(ZeroBot& bot, PacketDispatcher& dispatcher) : bot(bot) {
  dispatcher.Register(ProtocolS2C::Chat, RawOnChatPacket, this);

  RegisterCommand(std::make_shared<HelpCommand>());
  RegisterCommand(std::make_shared<CommandsCommand>());
  RegisterCommand(std::make_shared<DelimiterCommand>());
  RegisterCommand(std::make_shared<ModListCommand>());

  RegisterCommand(std::make_shared<SayCommand>());

  RegisterCommand(std::make_shared<LockCommand>());
  RegisterCommand(std::make_shared<UnlockCommand>());

  RegisterCommand(std::make_shared<SetShipCommand>());
  RegisterCommand(std::make_shared<SetFreqCommand>());
  RegisterCommand(std::make_shared<SetArenaCommand>());

  RegisterCommand(std::make_shared<LockFreqCommand>());
  RegisterCommand(std::make_shared<UnlockFreqCommand>());

  RegisterCommand(std::make_shared<RepelCommand>());
  RegisterCommand(std::make_shared<RepelOffCommand>());
  RegisterCommand(std::make_shared<BurstCommand>());
  RegisterCommand(std::make_shared<BurstOffCommand>());
  RegisterCommand(std::make_shared<DecoyCommand>());
  RegisterCommand(std::make_shared<DecoyOffCommand>());
  RegisterCommand(std::make_shared<RocketCommand>());
  RegisterCommand(std::make_shared<RocketOffCommand>());
  RegisterCommand(std::make_shared<BrickCommand>());
  RegisterCommand(std::make_shared<BrickOffCommand>());
  RegisterCommand(std::make_shared<PortalCommand>());
  RegisterCommand(std::make_shared<PortalOffCommand>());
  RegisterCommand(std::make_shared<ThorCommand>());
  RegisterCommand(std::make_shared<ThorOffCommand>());

  RegisterCommand(std::make_shared<MultiCommand>());
  RegisterCommand(std::make_shared<MultiOffCommand>());
  RegisterCommand(std::make_shared<CloakCommand>());
  RegisterCommand(std::make_shared<CloakOffCommand>());
  RegisterCommand(std::make_shared<StealthCommand>());
  RegisterCommand(std::make_shared<StealthOffCommand>());
  RegisterCommand(std::make_shared<XRadarCommand>());
  RegisterCommand(std::make_shared<XRadarOffCommand>());
  RegisterCommand(std::make_shared<AntiWarpCommand>());
  RegisterCommand(std::make_shared<AntiWarpOffCommand>());

  switch (bot.server_info.zone) {
    case Zone::Devastation: {
      RegisterCommand(std::make_shared<BDPublicCommand>());
      RegisterCommand(std::make_shared<BDPrivateCommand>());
      RegisterCommand(std::make_shared<StartBDCommand>());
      RegisterCommand(std::make_shared<StopBDCommand>());
      RegisterCommand(std::make_shared<HoldBDCommand>());
      RegisterCommand(std::make_shared<ResumeBDCommand>());
      RegisterCommand(std::make_shared<AnchorCommand>());
      RegisterCommand(std::make_shared<RushCommand>());
      break;
    }
    case Zone::Hyperspace: {
      RegisterCommand(std::make_shared<HSFlagCommand>());
      RegisterCommand(std::make_shared<HSFlagOffCommand>());
      RegisterCommand(std::make_shared<HSBuyCommand>());
      RegisterCommand(std::make_shared<HSSellCommand>());
      RegisterCommand(std::make_shared<HSShipStatusCommand>());
      break;
    }
    default: {
      break;
    }
  }
}

int CommandSystem::GetSecurityLevel(const std::string& player) {
  auto iter = kOperators.find(player);

  if (iter != kOperators.end()) {
    return iter->second;
  }

  return 0;
}

void CommandSystem::OnChatPacket(const u8* pkt, size_t size) {
  ChatType type = (ChatType) * (pkt + 1);
  u16 sender_id = *(u16*)(pkt + 3);
  char* msg_raw = (char*)(pkt + 5);
  size_t msg_size = size - 6;

  std::string_view msg(msg_raw, msg_size);

  if (msg.empty()) return;
  if (msg[0] != '!' && msg[0] != '.') return;

  msg = msg.substr(1);

  Player* player = bot.game->player_manager.GetPlayerById(sender_id);

  if (!player) return;

  // Ignore self
  if (player->id == bot.game->player_manager.player_id) return;

  std::vector<std::string_view> tokens = Tokenize(msg, ';');

  for (std::string_view current_msg : tokens) {
    std::size_t split = current_msg.find(' ');
    std::string trigger = Lowercase(current_msg.substr(0, split));
    std::string arg;

    if (split != std::string::npos) {
      arg = current_msg.substr(split + 1);
    }

    auto iter = commands_.find(trigger);
    if (iter != commands_.end()) {
      CommandExecutor& command = *iter->second;
      u32 access_request = (1 << (u32)type);

      /*
       * bitfield evaluation example (showing 4 bits):
       * chat type is 0000 for arena message
       * request_access evaluates to 0001
       * command access can be 0001 for arena or 0101 for both arena and public
       * 0001 & 0101 = 0001 which evaluates to true or non zero
       */
      if (access_request & command.GetAccess()) {
        int security_level = 0;

        if (type == ChatType::Arena) {
          security_level = kArenaSecurityLevel;
        } else {
          auto op_iter = kOperators.find(Lowercase(player->name));

          if (op_iter != kOperators.end()) {
            security_level = op_iter->second;
          }
        }

        if (security_level >= command.GetSecurityLevel()) {
          auto& bb = bot.execute_ctx.blackboard;

          // If the command is lockable, bot is locked, and requester isn't an operator then ignore it.
          if (!(command.GetFlags() & CommandFlag_Lockable) || !bb.ValueOr<bool>("CmdLock", false) ||
              security_level > 0) {
            command.Execute(*this, bot, std::string(player->name), arg);
          }
        }
      }
    }
  }
}

const Operators& CommandSystem::GetOperators() const {
  return kOperators;
}

}  // namespace zero
