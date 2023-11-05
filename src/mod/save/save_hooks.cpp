#include "exlaunch.hpp"

#include "externals/PlayerWork.h"
#include "externals/System/Primitives.h"
#include "save/save.h"
#include "save/migration/save_migration.h"

static CustomSaveData gCustomSaveData {
    .main = {},
    .dex = {},
    .works = {},
    .flags = {},
    .sysflags = {},
    .trainers = {},
    .items = {},
    .berries = {},
    .playerColorVariation = {},
};

CustomSaveData* getCustomSaveData() {
    return &gCustomSaveData;
}

HOOK_DEFINE_TRAMPOLINE(PatchExistingSaveData__Load) {
    static bool Callback(PlayerWork::Object* playerWork) {
        bool success = Orig(playerWork);

        bool isBackup = playerWork->fields._isBackupSave;

        if (success)
        {
            // Start as vanilla save
            getCustomSaveData()->main.version = ModVersion::Vanilla;

            // Load version data
            loadMain(isBackup);

            // Load all other data
            loadZukan(isBackup);
            loadWorks(isBackup);
            loadFlags(isBackup);
            loadSysFlags(isBackup);
            loadTrainers(isBackup);
            loadItems(isBackup);
            loadBerries(isBackup);
            loadColorVariations(isBackup);

            // Perform migration loop
            migrate(playerWork);

            // Put our custom-length data into PlayerWork for the game to access
            linkZukan(playerWork);
            linkWorks(playerWork);
            linkFlags(playerWork);
            linkSysFlags(playerWork);
            linkTrainers(playerWork);
            linkItems(playerWork);
            linkBerries(playerWork);
            linkColorVariations(playerWork);
        }

        playerWork->fields._isBackupSave = false;

        return success;
    }
};

HOOK_DEFINE_TRAMPOLINE(PatchExistingSaveData__Save) {
    static void Callback(PlayerWork::Object* playerWork, void* param_2, void* param_3, void* param_4) {
        bool isMain = playerWork->fields._isMainSave;
        bool isBackup = playerWork->fields._isBackupSave;

        // Remove the custom-length PlayerWork data with the vanilla save's
        unlinkZukan(playerWork);
        unlinkWorks(playerWork);
        unlinkFlags(playerWork);
        unlinkSysFlags(playerWork);
        unlinkTrainers(playerWork);
        unlinkItems(playerWork);
        unlinkBerries(playerWork);
        unlinkColorVariations(playerWork);

#ifndef DEBUG_DISABLE_SAVE  // Allow disabling the saving to test the save migration code
        // Save version data to file
        saveMain(isMain, isBackup);

        // Save rest of data to files
        saveZukan(isMain, isBackup);
        saveWorks(isMain, isBackup);
        saveFlags(isMain, isBackup);
        saveSysFlags(isMain, isBackup);
        saveTrainers(isMain, isBackup);
        saveItems(isMain, isBackup);
        saveBerries(isMain, isBackup);
        saveColorVariations(isMain, isBackup);
#endif

        // Save base save file
        Orig(playerWork, param_2, param_3, param_4);

        // Re-replace the PlayerWork data with our custom-length one
        relinkZukan(playerWork);
        relinkWorks(playerWork);
        relinkFlags(playerWork);
        relinkSysFlags(playerWork);
        relinkTrainers(playerWork);
        relinkItems(playerWork);
        relinkBerries(playerWork);
        relinkColorVariations(playerWork);
    }
};

HOOK_DEFINE_REPLACE(PatchExistingSaveData__Verify) {
    static bool Callback(System::Byte_array* playerWork) {
        return true;
    }
};

void exl_save_main() {
    PatchExistingSaveData__Load::InstallAtOffset(0x02ceb850);
    PatchExistingSaveData__Save::InstallAtOffset(0x01a8c2f0);
    PatchExistingSaveData__Verify::InstallAtOffset(0x02ceba00);

    // Backup save patches
    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p(0);
    auto inst = nn::vector<exl::patch::Instruction> {
        { 0x02ceb9dc, Nop() },
    };
    p.WriteInst(inst);
}
