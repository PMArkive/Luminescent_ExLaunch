#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/String.h"
#include "externals/DPData/MYSTATUS.h"

namespace Pml::PokePara {
    struct OwnerInfo : ILClass<OwnerInfo, 0x04c5fbe8> {
        struct Fields {
            uint32_t trainerId;
            uint8_t sex;
            uint8_t langID;
            System::String::Object* name;
        };

        inline void ctor(DPData::MYSTATUS::Object* ownerStatus) {
            external<void>(0x02054ea0, this, ownerStatus);
        }
    };

    static Il2CppClass* OwnerInfo_TypeInfo() {
        return *reinterpret_cast<Il2CppClass**>(exl::util::modules::GetTargetOffset(0x04c5fbe8));
    }
}