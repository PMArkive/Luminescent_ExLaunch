#include "exlaunch.hpp"

#include "externals/Dpr/Battle/Logic/EventVar.h"
#include "externals/Dpr/Battle/Logic/Handler/Waza.h"
#include "data/moves.h"
#include "data/utils.h"
#include "move_handlers.h"

using namespace Dpr::Battle::Logic;

void Handlers_Move_SilverWind(Handler::Waza::GET_FUNC_TABLE_ELEM::Array* getFuncTable) {
    SetMoveFunctionTable(getFuncTable, getExtraMoveHandlers()->currentIndex, array_index(MOVES, "Silver Wind"), *Handler::Waza::PTR_Method$$ADD_GensiNoTikara);
    getExtraMoveHandlers()->currentIndex++;
}