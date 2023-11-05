#include "exlaunch.hpp"

#include "memory/string.h"
#include "memory/vector.h"

#include "externals/BadgeWork.h"
#include "externals/Dpr/Battle/Logic/MainModule.h"
#include "externals/Dpr/UI/UIBadgeGet.h"
#include "externals/FlagWork.h"
#include "externals/PlayerWork.h"
#include "externals/System/String.h"

#include "logger/logger.h"

HOOK_DEFINE_REPLACE(ObedienceMax) {
    static uint8_t Callback(Dpr::Battle::Logic::MainModule *__this) {
        return 100;
    }
};

HOOK_DEFINE_INLINE(CheckSurfFlags) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        bool result = true;
        result &= PlayerWork::GetSystemFlag((int32_t)FlagWork_SysFlag::BADGE_ID_C06);
        result &= PlayerWork::GetBool((int32_t)FlagWork_Flag::FE_HIDEN_03_GET);

        ctx->X[0] = result ? 1 : 0;
    }
};

HOOK_DEFINE_REPLACE(UIBadgeGet_GetBadgeAnimation) {
    static System::String::Object* Callback(Dpr::UI::UIBadgeGet::Object *__this) {
        system_load_typeinfo(0x99e2);

        switch (__this->fields.badgeNo)
        {
            case 2: // Getting Maylene's badge
                if (BadgeWork::IsGet(3)) // If you have Wake's badge
                    return System::String::Create("getbadge_3-2");
                else // If you DON'T have Wake's badge
                    return System::String::Create("getbadge_3-1");

            case 3: // Getting Wake's badge
                if (BadgeWork::IsGet(2)) // If you have Maylene's badge
                    return System::String::Create("getbadge_4-1");
                else // If you DON'T have Maylene's badge
                    return System::String::Create("getbadge_4-2");

            default:
                return System::String::Create("getbadge_" + nn::to_string(__this->fields.badgeNo + 1));
        }
    }
};

void exl_badge_check_main() {
    ObedienceMax::InstallAtOffset(0x020349e0);
    CheckSurfFlags::InstallAtOffset(0x01daca9c);
    UIBadgeGet_GetBadgeAnimation::InstallAtOffset(0x01858120);

    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p(0);
    auto inst = nn::vector<exl::patch::Instruction> {
        { 0x01dbc4cc, Movz(W1, (int32_t)FlagWork_SysFlag::BADGE_ID_C05) },
        { 0x01dbc4f8, Movz(W1, (int32_t)FlagWork_SysFlag::BADGE_ID_C06) },
    };
    p.WriteInst(inst);
}
