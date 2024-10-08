#pragma once

#include "externals/il2cpp-api.h"

namespace UnityEngine::Rendering
{
    enum class StencilOp : int32_t {
        Keep = 0,
        Zero = 1,
        Replace = 2,
        IncrementSaturate = 3,
        DecrementSaturate = 4,
        Invert = 5,
        IncrementWrap = 6,
        DecrementWrap = 7,
    };
}