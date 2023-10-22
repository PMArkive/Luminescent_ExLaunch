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
        Logger::log("Local Koukan - Get Index\n");
        return npcindex;
    }
};

HOOK_DEFINE_REPLACE(LocalKoukan_GetTargetData) {
    static XLSXContent::LocalKoukanData::Sheetdata::Object * Callback(int32_t npcindex, int32_t lang, MethodInfo *method) {

        GameData::DataManager::StaticFields *staticFields;
        system_load_typeinfo(0x5e91);
        GameData::DataManager::getClass()->initIfNeeded();
        staticFields = (GameData::DataManager::StaticFields *)GameData::DataManager_TypeInfo()->static_fields;
        XLSXContent::LocalKoukanData::Sheetdata::Object *data = staticFields->LocalKoukanData->get_Item(npcindex);
        Logger::log("Get Target Data - Data from Item Success\n");
        return data;
    }
};

int32_t LocalKoukan_Language(int32_t langId)
{
    int32_t playerLangId = PlayerWork::get_msgLangID();
    Logger::log("Local Koukan - Language\n");

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
    static Pml::PokePara::PokemonParam::Object * Callback(XLSXContent::LocalKoukanData::Sheetdata::Object *data) {

        system_load_typeinfo(0x5e90);
        //Pml::PokePara::InitialSpec::getClass()->initIfNeeded();
        auto initialSpec = Pml::PokePara::InitialSpec::newInstance();
        //initialSpec->ctor();

        int32_t formNo = (data->fields.monsno & 0xFFFF0000) >> 16;  // Bits 16-31
        int32_t monsNo = data->fields.monsno & 0x0000FFFF;         // Bits 0-15

        /*
        int32_t speedIV = (data->fields.tokusei & 0xF8000000) >> 27; // Bits 27-31
        int32_t spDefIV = (data->fields.tokusei & 0x07C00000) >> 22; // Bits 22-26
        int32_t spAtkIV = (data->fields.tokusei & 0x003E0000) >> 17; // Bits 17-21
        int32_t defIV = (data->fields.tokusei & 0x0001F000) >> 12; // Bits 12-16
        int32_t atkIV = (data->fields.tokusei & 0x00000F80) >> 7;  // Bits 7-11
        int32_t hpIV = (data->fields.tokusei & 0x0000007C) >> 2;  // Bits 2-6
        moved to JSON
        */
        int32_t tokusei = data->fields.tokusei & 0x00000003;        // Bits 0-1

        //int32_t ivFlag = (data->fields.seikaku & 0x00000800) >> 11; // Bit  11
        int32_t contestFlag = (data->fields.seikaku & 0x00000400) >> 10; // Bit  10
        int32_t ballId = (data->fields.seikaku & 0x000003E0) >> 5;  // Bits 5-9
        int32_t seikaku = data->fields.seikaku & 0x0000001F;        // Bits 0-4

        //if (ballId == 0) ballId = POKEBALL_BALLID;
        //Move to JSON

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

        /*
         * Moved to JSON

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
          */

        Logger::log("Local Koukan - Pokemon Param\n");
        //Pml::PokePara::PokemonParam::getClass()->initIfNeeded();
        //Logger::log("Poke param init if needed\n");
        auto pokeParam = Pml::PokePara::PokemonParam::newInstance(initialSpec);
        Logger::log("poke param new instance\n");
        //pokeParam->ctor(initialSpec);
        //Logger::log("Init spec .ctor works\n");
        Pml::PokePara::CoreParam::Object * coreParam = (Pml::PokePara::CoreParam::Object *)pokeParam;
        Logger::log("core param cast success\n");

        auto messageManager = Dpr::Message::MessageManager::instance();
        Logger::log("message manager instance\n");
        auto nickname = messageManager->GetNameMessage(System::String::Create("dp_scenario3"), data->fields.nickname_label);
        Logger::log("Local Koukan - nickname var set\n");
        coreParam->SetNickName(nickname);
        Logger::log("Local Koukan - set nickname\n");

        coreParam->SetItem(data->fields.itemno);
        Logger::log("Local Koukan -  data set\n");

        System::String::Object *trainerName = messageManager->GetNameMessage(System::String::Create("dp_scenario3"), data->fields.name_label);
        Logger::log("Local Koukan - trainer name retrieved\n");
        coreParam->SetParentName(trainerName);
        Logger::log("Local Koukan - trainer name set\n");

        uint32_t language = LocalKoukan_Language(data->fields.language);
        Logger::log("Local Koukan - language retrieved\n");
        coreParam->SetLangId(language);
        Logger::log("Local Koukan - language ID set\n");

        coreParam->SetGetBall(ballId);
        Logger::log("Local Koukan - ball ID set\n");

        if (contestFlag & 1) {
            Logger::log("Local Koukan - contest flag set\n");
            /*
            coreParam->SetCondition(0, 20);
            coreParam->SetCondition(1, 20);
            coreParam->SetCondition(2, 20);
            coreParam->SetCondition(3, 20);
            coreParam->SetCondition(4, 20);
             Moved to JSON
             */
            Logger::log("Local Koukan - conditions set\n");
        }

        for (int32_t i = 0; i < data->fields.waza->max_length && i < 4; i++) {
            coreParam->SetWaza(i, data->fields.waza->m_Items[i]);
            Logger::log("Local Koukan - waza set\n");
        }

        DPData::MYSTATUS::Object* playerStatus = PlayerWork::get_playerStatus();
        Pml::PokePara::OwnerInfo::getClass()->initIfNeeded();
        auto ownerInfo = Pml::PokePara::OwnerInfo::newInstance(playerStatus);
        //Pml::PokePara::OwnerInfo::Object* ownerInfoConverted = (Pml::PokePara::OwnerInfo::Object*)(ownerInfo);
        Logger::log("Local Koukan - owner info set\n");
        //ownerInfo->ctor(playerStatus);
        Logger::log("Local Koukan - get player status\n");

        coreParam->UpdateOwnerInfo(ownerInfo);
        Logger::log("Local Koukan - update owner info\n");
        poketool::poke_memo::poketool_poke_memo::Object::ClearPlaceTime(coreParam, 0);
        Logger::log("Local Koukan - clear place time\n");
        poketool::poke_memo::poketool_poke_memo::Object::SetPlaceTime(coreParam, 0x7531, 1);
        Logger::log("Local Koukan - set place time\n");
        poketool::poke_memo::poketool_poke_memo::Object::SetGetLevel(coreParam);
        Logger::log("Local Koukan - set get level\n");
        poketool::poke_memo::poketool_poke_memo::Object::SetVersion(coreParam);
        Logger::log("Local Koukan - set version\n");

        bool isValid = coreParam->StartFastMode();
        Logger::log("Local Koukan - start fast mode\n");
        coreParam->EndFastMode(isValid);
        Logger::log("Local Koukan - end fast mode\n");

        Logger::log("Local Koukan - Create Trade Poke Param - End\n");
        return pokeParam;
    }
};

void exl_extended_local_trades_main() {
    LocalKoukan_GetIndex::InstallAtOffset(0x01af3390);
    LocalKoukan_GetTargetData::InstallAtOffset(0x01af32a0);
    LocalKoukan_CreateTradePokeParam::InstallAtOffset(0x01af3510);

    /*
    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p(0x01af368c);
    p.BranchInst((void*)&LocalKoukan_Language);
    */
}