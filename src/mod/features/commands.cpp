#include "exlaunch.hpp"
#include "externals/Dpr/EvScript/EvCmdID.h"
#include "externals/Dpr/EvScript/EvDataManager.h"

#include "features/commands/commands.h"

static bool ACTIVATED_COMMANDS[(int32_t)Dpr::EvScript::EvCmdID::NAME::CUSTOM_CMD_END];

void SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME command)
{
    ACTIVATED_COMMANDS[(int32_t)command] = true;
}

// Handles overriden and new script commands, then calls the original method to handle the rest normally.
HOOK_DEFINE_TRAMPOLINE(RunEvCmdCustom) {
    static bool Callback(Dpr::EvScript::EvDataManager::Object* __this, int32_t index) {
        // Overriden/New Commands
        if (ACTIVATED_COMMANDS[index])
        {
            switch ((Dpr::EvScript::EvCmdID::NAME)index)
            {
                case Dpr::EvScript::EvCmdID::NAME::_SET_WEATHER:
                    return SetWeather(__this);
                case Dpr::EvScript::EvCmdID::NAME::_FIRST_POKE_SELECT_PROC:
                    return FirstPokeSelectProc(__this);
                case Dpr::EvScript::EvCmdID::NAME::_FIRST_POKE_NO_GET:
                    return FirstPokeNoGet(__this);
                case Dpr::EvScript::EvCmdID::NAME::_HONEY_TREE_BTL_SET:
                    return HoneyTreeBattleSet(__this);
                case Dpr::EvScript::EvCmdID::NAME::_STOP_EFFECT:
                    return StopEffect(__this);
                case Dpr::EvScript::EvCmdID::NAME::_TEMOTI_FORMNO:
                    return PartyFormNo(__this);
                case Dpr::EvScript::EvCmdID::NAME::_TEMOTI_BOX_FORMNO:
                    return PartyBoxFormNo(__this);
                case Dpr::EvScript::EvCmdID::NAME::_GET_BOX_POKE_SEIKAKU:
                    return PartyBoxNature(__this);
                case Dpr::EvScript::EvCmdID::NAME::_RELEASE_BOX_POKE:
                    return PartyBoxRelease(__this);
                case Dpr::EvScript::EvCmdID::NAME::_TOGGLE_COLLISION_BOX:
                    return ToggleCollisionBox(__this);
                case Dpr::EvScript::EvCmdID::NAME::_INSTALL_CHECK:
                    return InstallCheck(__this);
                case Dpr::EvScript::EvCmdID::NAME::_SET_PLAYER_COLOR_INDEX:
                    return SetPlayerColorIndex(__this);
                case Dpr::EvScript::EvCmdID::NAME::_RIVAL_POKE_NO_GET:
                    return RivalPokeNoGet(__this);
                case Dpr::EvScript::EvCmdID::NAME::_SUPPORT_POKE_NO_GET:
                    return SupportPokeNoGet(__this);
                case Dpr::EvScript::EvCmdID::NAME::_FIRST_MONSNO_FORMNO:
                    return PlayerStarterMonsNoFormNo(__this);
                case Dpr::EvScript::EvCmdID::NAME::_RIVAL_MONSNO_FORMNO:
                    return RivalStarterMonsNoFormNo(__this);
                case Dpr::EvScript::EvCmdID::NAME::_SUPPORT_MONSNO_FORMNO:
                    return SupportStarterMonsNoFormNo(__this);
                default:
                    break;
            }
        }

        // Call original method
        return Orig(__this, index);
    }
};

void exl_commands_main() {
    RunEvCmdCustom::InstallAtOffset(0x02c5b290);

    // Select which new commands/overrides are activated
    for (bool & i : ACTIVATED_COMMANDS)
        i = false;
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_SET_WEATHER);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_FIRST_POKE_SELECT_PROC);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_FIRST_POKE_NO_GET);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_HONEY_TREE_BTL_SET);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_STOP_EFFECT);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_TEMOTI_FORMNO);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_TEMOTI_BOX_FORMNO);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_GET_BOX_POKE_SEIKAKU);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_RELEASE_BOX_POKE);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_TOGGLE_COLLISION_BOX);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_INSTALL_CHECK);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_SET_PLAYER_COLOR_INDEX);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_RIVAL_POKE_NO_GET);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_SUPPORT_POKE_NO_GET);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_FIRST_MONSNO_FORMNO);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_RIVAL_MONSNO_FORMNO);
    SetActivatedCommand(Dpr::EvScript::EvCmdID::NAME::_SUPPORT_MONSNO_FORMNO);
}
