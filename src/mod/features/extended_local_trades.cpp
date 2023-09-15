#include "exlaunch.hpp"

#include "externals/Dpr/Message/MessageManager.h"
#include "externals/GameData/DataManager.h"
#include "externals/PlayerWork.h"
#include "externals/Pml/PokePara/InitialSpec.h"
#include "externals/Pml/PokePara/OwnerInfo.h"
#include "externals/Pml/PokePara/PokemonParam.h"
#include "externals/poketool/poke_memo/poketool_poke_memo.h"
//#include "externals/SmartPoint/AssetAssistant/SingletonMonoBehaviour.h"
#include "externals/XLSXContent/LocalKoukanData.h"

#include "utils/utils.h"
#include "logger/logger.h"

const int32_t JAPANESE_LANGID = 1;
const int32_t ENGLISH_LANGID = 2;

const uint32_t POKEBALL_BALLID = 4;

const int32_t HP_POWERID = 0;
const int32_t ATK_POWERID = 1;
const int32_t DEF_POWERID = 2;
const int32_t SPATK_POWERID = 3;
const int32_t SPDEF_POWERID = 4;
const int32_t SPEED_POWERID = 5;


HOOK_DEFINE_REPLACE(LocalKoukan_GetIndex) {
    static int32_t Callback(int32_t npcindex, int32_t lang, MethodInfo *method) {
        return npcindex;
    }
};

HOOK_DEFINE_REPLACE(LocalKoukan_GetTargetData) {
    static XLSXContent::LocalKoukanData::Sheetdata::Object * Callback(int32_t npcindex, int32_t lang, MethodInfo *method) {
        system_load_typeinfo(0x5e91);
        auto dataManagerTI = GameData::DataManager_TypeInfo();
        GameData::DataManager::Object *dataManagerObj = (GameData::DataManager::Object *) dataManagerTI;
        XLSXContent::LocalKoukanData::Sheetdata::Object *data = dataManagerObj->getClass()->static_fields->LocalKoukanData->get_Item(
                npcindex);
        return data;
    }
};

int32_t LocalKoukan_Language(int32_t langId, MethodInfo *method)
{
    int32_t playerLangId = PlayerWork::get_msgLangID();

    if (langId == 0)
    {
        // No set language, so set to same language as player.
        return playerLangId;
    }
    else if (langId != playerLangId)
    {
        // Set language that doesn't match player, so set to that language.
        return langId;
    }
    else if (langId != ENGLISH_LANGID)
    {
        // Set language that matches player, so set to english.
        return ENGLISH_LANGID;
    }
    else
    {
        // Set language that matches player, but it's english, so set to japanese.
        return JAPANESE_LANGID;
    }
}

HOOK_DEFINE_REPLACE(LocalKoukan_CreateTradePokeParam) {
    static Pml::PokePara::PokemonParam::Object *
    Callback(XLSXContent::LocalKoukanData::Sheetdata::Object *data, MethodInfo *method) {
        system_load_typeinfo(0x5e90);

        Pml::PokePara::InitialSpec::Object *initialSpec = (Pml::PokePara::InitialSpec::Object *) Pml::PokePara::InitialSpec_TypeInfo;
        initialSpec->ctor();

        int32_t formNo = (data->fields.monsno & 0xFFFF0000) >> 16;  // Bits 16-31
        int32_t monsNo = data->fields.monsno & 0x0000FFFF;         // Bits 0-15

        int32_t speedIV = (data->fields.tokusei & 0xF8000000) >> 27; // Bits 27-31
        int32_t spDefIV = (data->fields.tokusei & 0x07C00000) >> 22; // Bits 22-26
        int32_t spAtkIV = (data->fields.tokusei & 0x003E0000) >> 17; // Bits 17-21
        int32_t defIV = (data->fields.tokusei & 0x0001F000) >> 12; // Bits 12-16
        int32_t atkIV = (data->fields.tokusei & 0x00000F80) >> 7;  // Bits 7-11
        int32_t hpIV = (data->fields.tokusei & 0x0000007C) >> 2;  // Bits 2-6
        int32_t tokusei = data->fields.tokusei & 0x00000003;        // Bits 0-1

        int32_t ivFlag = (data->fields.seikaku & 0x00000800) >> 11; // Bit  11
        int32_t contestFlag = (data->fields.seikaku & 0x00000400) >> 10; // Bit  10
        int32_t ballId = (data->fields.seikaku & 0x000003E0) >> 5;  // Bits 5-9
        int32_t seikaku = data->fields.seikaku & 0x0000001F;        // Bits 0-4

        if (ballId == 0) ballId = POKEBALL_BALLID;

        initialSpec->fields.monsno = monsNo;
        initialSpec->fields.formno = formNo;
        initialSpec->fields.level = (uint16_t) data->fields.level;
        initialSpec->fields.rareRnd = 0x1ffffffff;
        initialSpec->fields.id = (uint64_t) data->fields.trainerid;
        initialSpec->fields.sex = (uint16_t) data->fields.sex;
        initialSpec->fields.seikaku = (uint16_t) seikaku;
        initialSpec->fields.tokuseiIndex = (uint8_t) tokusei;
        initialSpec->fields.personalRnd = (uint64_t) data->fields.rand;
        initialSpec->fields.randomSeed = (uint64_t) data->fields.rand;
        initialSpec->fields.isRandomSeedEnable = true;

        if (ivFlag & 1) {
            if (initialSpec->fields.talentPower->max_length > HP_POWERID)
                initialSpec->fields.talentPower->m_Items[HP_POWERID] = (uint16_t) hpIV;
            if (initialSpec->fields.talentPower->max_length > ATK_POWERID)
                initialSpec->fields.talentPower->m_Items[ATK_POWERID] = (uint16_t) atkIV;
            if (initialSpec->fields.talentPower->max_length > DEF_POWERID)
                initialSpec->fields.talentPower->m_Items[DEF_POWERID] = (uint16_t) defIV;
            if (initialSpec->fields.talentPower->max_length > SPATK_POWERID)
                initialSpec->fields.talentPower->m_Items[SPATK_POWERID] = (uint16_t) spAtkIV;
            if (initialSpec->fields.talentPower->max_length > SPDEF_POWERID)
                initialSpec->fields.talentPower->m_Items[SPDEF_POWERID] = (uint16_t) spDefIV;
            if (initialSpec->fields.talentPower->max_length > SPEED_POWERID)
                initialSpec->fields.talentPower->m_Items[SPEED_POWERID] = (uint16_t) speedIV;
        }

        Pml::PokePara::PokemonParam::Object *pokeParam = (Pml::PokePara::PokemonParam::Object *) Pml::PokePara::PokemonParam_TypeInfo;
        pokeParam->ctor(initialSpec);
        Pml::PokePara::CoreParam::Object *coreParam = (Pml::PokePara::CoreParam::Object *) pokeParam;

        Dpr::Message::MessageManager::Object *messageManager = (Dpr::Message::MessageManager::Object *) Dpr::Message::MessageManager::instance();
        System::String::Object *nickname = messageManager->GetNameMessage(System::String::Create("dp_scenario3"), data->fields.nickname_label);
        coreParam->SetNickName(nickname);

        coreParam->SetItem(data->fields.itemno);

        System::String::Object *trainerName = messageManager->GetNameMessage(System::String::Create("dp_scenario3"), data->fields.name_label);
        coreParam->SetParentName(trainerName);

        uint32_t language = LocalKoukan_Language(data->fields.language, nullptr);
        coreParam->SetLangId(language);

        coreParam->SetGetBall(ballId);

        if (contestFlag & 1) {
            coreParam->SetCondition(0, 20);
            coreParam->SetCondition(1, 20);
            coreParam->SetCondition(2, 20);
            coreParam->SetCondition(3, 20);
            coreParam->SetCondition(4, 20);
        }

        for (int32_t i = 0; i < data->fields.waza->max_length && i < 4; i++) {
            coreParam->SetWaza(i, data->fields.waza->m_Items[i]);
        }

        Pml::PokePara::OwnerInfo::Object *ownerInfo = (Pml::PokePara::OwnerInfo::Object *) Pml::PokePara::OwnerInfo_TypeInfo;
        ownerInfo->ctor(PlayerWork::get_playerStatus());

        coreParam->UpdateOwnerInfo(ownerInfo);
        poketool::poke_memo::poketool_poke_memo::Object::ClearPlaceTime(coreParam, 0);
        poketool::poke_memo::poketool_poke_memo::Object::SetPlaceTime(coreParam, 0x7531, 1);
        poketool::poke_memo::poketool_poke_memo::Object::SetGetLevel(coreParam);
        poketool::poke_memo::poketool_poke_memo::Object::SetVersion(coreParam);

        bool isValid = coreParam->StartFastMode();
        coreParam->EndFastMode(isValid);

        return pokeParam;
    }
};

void exl_extended_local_trades_main() {
    LocalKoukan_GetIndex::InstallAtOffset(0x01af3390);
    LocalKoukan_GetTargetData::InstallAtOffset(0x01af32a0);
    LocalKoukan_CreateTradePokeParam::InstallAtOffset(0x01af3510);

    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p(0x01af368c);
    p.BranchLinkInst((void*)&LocalKoukan_Language);
}