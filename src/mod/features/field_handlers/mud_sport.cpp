#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/Common.h"
#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/FX32.h"
#include "externals/Dpr/Battle/Logic/Handler/Field.h"
#include "externals/Dpr/Battle/Logic/Section_FieldEffect_End.h"
#include "externals/Dpr/Battle/Logic/ServerCommandPutter.h"
#include "data/field_effects.h"
#include "data/types.h"
#include "data/utils.h"
#include "field_handlers.h"

using namespace Dpr::Battle::Logic;

void HandlerFieldEffectMudSportFieldWazaPower(EventFactor::EventHandlerArgs::Object** args, uint8_t subParam) {
    if (Common::GetEventVar(args, EventVar::Label::WAZA_TYPE) != array_index(TYPES, "Electric"))
        return;

    // 1/3 power
    Common::MulEventVar(args, EventVar::Label::WAZA_POWER_RATIO, FX32::CONST_0_333);
}

EventFactor::EventHandlerTable::Array* ADD_FieldEffect_MudSport() {
    EventFactor::EventHandlerTable::Array* table = getExtraFieldEffectHandlers()->HandlerTable_MudSport;
    if (table == nullptr) {
        table = CreateFieldEffectEventHandlerTable(1);
        table->m_Items[0] = CreateFieldEffectEventHandler(EventID::WAZA_POWER, (Il2CppMethodPointer)&HandlerFieldEffectMudSportFieldWazaPower);
        getExtraFieldEffectHandlers()->HandlerTable_MudSport = table;
    }
    return table;
}

void Handlers_FieldEffect_MudSport(Handler::Field::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetFieldEffectFunctionTable(getFuncTable, getExtraFieldEffectHandlers()->currentIndex, array_index(FIELD_EFFECTS, "Mud Sport"), (Il2CppMethodPointer)&ADD_FieldEffect_MudSport);
    getExtraFieldEffectHandlers()->currentIndex++;
}

void FieldEffect_End_MudSport(Section_FieldEffect_End::Object* __this, Section_FieldEffect_End::Result::Object* pResult, Section_FieldEffect_End::Description::Object** description) {
    ServerCommandPutter::Object* serverCommandPutter = __this->fields.m_pServerCmdPutter->instance();
    serverCommandPutter->Message_Std(132);
}