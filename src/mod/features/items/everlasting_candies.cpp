#include "exlaunch.hpp"
#include "externals/Dpr/Item/ItemInfo.h"
#include "externals/Dpr/UI/UIBag.h"
#include "externals/PlayerWork.h"
#include "externals/ItemWork.h"
#include "externals/FlagWork.h"
#include "data/utils.h"
#include "data/items.h"
#include "romdata/romdata.h"

uint32_t isValidRareCandy(uint32_t level, Dpr::UI::UIBag::Object *bagRef) {

    // Is Level Cap enabled
    if (!PlayerWork::GetBool((int32_t) FlagWork_Flag::FLAG_DISABLE_LEVEL_CAP)) {
        return 100 - level;
    }

    Dpr::Item::ItemInfo::Object * item = bagRef->fields.bagItemPanel->fields._SelectedItemButton_k__BackingField->fields.item;

    if (item->get_Id() != array_index(ITEMS, "Everlasting Candy")) {
        return 100 - level;
    }

    uint32_t max_level = GetMaxLevel();

    if (level >= max_level) {
        return 0;
    } else {
        return max_level - level;
    }
}

int32_t ItemWork_SubItem(int32_t itemno, int32_t num) {
    if (itemno != array_index(ITEMS, "Everlasting Candy")) {
        return ItemWork::SubItem(itemno, num);
    }

    return 0;
}

void exl_items_everlasting_candies_main(){
    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;

    exl::patch::CodePatcher p(0x0185cc94);
    p.WriteInst(MovRegister(W0, W22));
    p.WriteInst(MovRegister(X1, X19));
    p.BranchLinkInst((void*)&isValidRareCandy);
    p.WriteInst(MovRegister(W22, W0));
    p.WriteInst(Nop());
    p.WriteInst(Nop());
    p.WriteInst(Nop());

    p.Seek(0x0185eb8c);
    p.BranchLinkInst((void*)&ItemWork_SubItem);
};