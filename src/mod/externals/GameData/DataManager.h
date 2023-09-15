#pragma once

#include "externals/il2cpp-api.h"
#include "externals/XLSXContent/CharacterDressData.h"
#include "externals/XLSXContent/ShopTable.h"
#include "externals/XLSXContent/LocalKoukanData.h"

namespace GameData {
    struct DataManager : ILClass<DataManager, 0x04c59d70> {
        struct StaticFields {
            void* CharacterGraphics;
            void* TrainerTable;
            void* PlaceData;
            void* MapWarpData;
            void* StopData;
            void* GimmickGraphics;
            void* PokemonInfo;
            void* ContestWazaInfo;
            void* ShopTable;
            void* CharacterDressData;
            void* KinomiData;
            void* KinomiPlaceData;
            void* HoneyTree;
            void* MonohiroiTable;
            void* UgJumpPos;
            void* TowerTrainerTable;
            void* TowerMatchingTable;
            void* TowerSingleStockTable;
            void* TowerDoubleStockTable;
            void* AdventureNoteData;
            void* AdventureNoteDataDict; //System::Collections::Generic::Dictionary$$int$$List_AdventureNoteData_SheetData::Object*
            void* TowerBattlePoint;
            void* TagPlaceData;
            XLSXContent::LocalKoukanData::Object* LocalKoukanData;
            void* ContestCommonData;
            void* TvDataTable;
            void* TvSchedule;
            void* onPostLoadData;
            void* FieldCommonParam;
            void* FieldWazaCutInParam;
            void* ZenmetuZone;
            void* MoveAfterSaveGrid;
            void* _comparerCatalog;
            void* _comparerKinomi;
            void* statueEffectRawData;
        };

        static inline XLSXContent::ShopTable::SheetBoutiqueShop::Object * GetBoutiqueShopData(int32_t dressId) {
            return external<XLSXContent::ShopTable::SheetBoutiqueShop::Object *>(0x02ccd550, dressId);
        }

        static inline XLSXContent::CharacterDressData::SheetData::Object * GetCharacterDressData(int32_t dressId) {
            return external<XLSXContent::CharacterDressData::SheetData::Object *>(0x02ccd460, dressId);
        }
    };

    static Il2CppClass* DataManager_TypeInfo() {
        return *reinterpret_cast<Il2CppClass**>(exl::util::modules::GetTargetOffset(0x04c59d70));
    }
}
