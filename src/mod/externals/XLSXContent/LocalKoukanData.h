#pragma once

#include "externals/il2cpp-api.h"
#include "externals/Pml/WazaNo.h"

#include "externals/UnityEngine/ScriptableObject.h"

namespace XLSXContent {
    struct LocalKoukanData : ILClass<LocalKoukanData> {
        struct Sheetdata : ILClass<Sheetdata> {
            struct Fields {
                int32_t target;
                System::String::Object* name_label;
                int32_t trainerid;
                int32_t monsno;
                System::String::Object* nickname_label;
                int32_t level;
                int32_t seikaku;
                int32_t tokusei;
                uint16_t itemno;
                int32_t rand;
                uint8_t sex;
                int32_t language;
                Pml_WazaNo_array* waza;
            };
        };

        struct Fields : UnityEngine::ScriptableObject::Fields {
            struct XLSXContent::LocalKoukanData::Sheetdata::Array* data;
        };

        static inline XLSXContent::LocalKoukanData::Sheetdata::Object * get_Item(int32_t index) {
            return external<XLSXContent::LocalKoukanData::Sheetdata::Object *>(0x017d5670, index);
        }
    };
}