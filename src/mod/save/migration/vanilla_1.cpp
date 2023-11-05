#include "romdata/romdata.h"
#include "save/migration/save_migration.h"
#include "save/save.h"

#include "logger/logger.h"

const uint64_t VANILLA_DEXSIZE = 493;

void migrateFromVanilla(PlayerWork::Object* playerWork) {
    Logger::log("Migrating from Vanilla...\n");
    CustomSaveData* save = getCustomSaveData();

    auto& savedata = playerWork->fields._saveData.fields;
    auto& zukan = playerWork->fields._saveData.fields.zukanData.fields;
    auto& kinomigrow = playerWork->fields._saveData.fields.kinomiGrowSaveData.fields;
    auto& myStatus = playerWork->fields._saveData.fields.playerData.fields.mystatus.fields;

    // Copy over vanilla data from PlayerWork into the custom save
    for (uint64_t i=0; i<VANILLA_DEXSIZE; i++)
    {
        save->dex.elements[i].get_status = zukan.get_status->m_Items[i];
        save->dex.elements[i].male_color_flag = zukan.male_color_flag->m_Items[i];
        save->dex.elements[i].female_color_flag = zukan.female_color_flag->m_Items[i];
        save->dex.elements[i].male_flag = zukan.male_flag->m_Items[i];
        save->dex.elements[i].female_flag = zukan.female_flag->m_Items[i];
        save->dex.elements[i].female_flag = zukan.female_flag->m_Items[i];
    }

    savedata.intValues->copyInto(save->works.items);
    savedata.boolValues->copyInto(save->flags.items);
    savedata.systemFlags->copyInto(save->sysflags.items);
    savedata.tr_battleData->copyInto(save->trainers.items);
    savedata.saveItem->copyInto(save->items.items);
    kinomigrow.kinomiGrows->copyInto(save->berries.items);

    // Set player color to the one in the vanilla save
    save->playerColorVariation.playerColorID = myStatus.body_type;

    // Set custom player color set to the one in the vanilla save
    auto set = GetColorSet(myStatus.body_type);
    save->playerColorVariation.fSkinFace =  { .fields = { set.fieldSkinFace.r, set.fieldSkinFace.g, set.fieldSkinFace.b, set.fieldSkinFace.a } };
    save->playerColorVariation.fSkinMouth = { .fields = { set.fieldSkinMouth.r, set.fieldSkinMouth.g, set.fieldSkinMouth.b, set.fieldSkinMouth.a } };
    save->playerColorVariation.fEyes =      { .fields = { set.fieldEyes.r, set.fieldEyes.g, set.fieldEyes.b, set.fieldEyes.a } };
    save->playerColorVariation.fEyebrows =  { .fields = { set.fieldEyebrows.r, set.fieldEyebrows.g, set.fieldEyebrows.b, set.fieldEyebrows.a } };
    save->playerColorVariation.fSkinBody =  { .fields = { set.fieldSkinBody.r, set.fieldSkinBody.g, set.fieldSkinBody.b, set.fieldSkinBody.a } };
    save->playerColorVariation.fHair =      { .fields = { set.fieldHair.r, set.fieldHair.g, set.fieldHair.b, set.fieldHair.a } };

    save->playerColorVariation.bSkinFace =  { .fields = { set.battleSkinFace.r, set.battleSkinFace.g, set.battleSkinFace.b, set.battleSkinFace.a } };
    save->playerColorVariation.bHairExtra = { .fields = { set.battleHairExtra.r, set.battleHairExtra.g, set.battleHairExtra.b, set.battleHairExtra.a } };
    save->playerColorVariation.bEyeLeft =   { .fields = { set.battleEyeLeft.r, set.battleEyeLeft.g, set.battleEyeLeft.b, set.battleEyeLeft.a } };
    save->playerColorVariation.bEyeRight =  { .fields = { set.battleEyeRight.r, set.battleEyeRight.g, set.battleEyeRight.b, set.battleEyeRight.a } };
    save->playerColorVariation.bSkinBody =  { .fields = { set.battleSkinBody.r, set.battleSkinBody.g, set.battleSkinBody.b, set.battleSkinBody.a } };
    save->playerColorVariation.bHair =      { .fields = { set.battleHair.r, set.battleHair.g, set.battleHair.b, set.battleHair.a } };

    // Set amount of boxes unlocked to 40
    playerWork->fields._saveData.fields.boxData.fields.trayMax = 40;

    Logger::log("Migration from Vanilla done!\n");
}