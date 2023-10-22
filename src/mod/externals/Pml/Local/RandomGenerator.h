#pragma once

#include "externals/il2cpp-api.h"

#include "externals/System/Primitives.h"

namespace Pml::Local {
    struct RandomGenerator : ILClass<RandomGenerator, 0x04c59c30> {
        struct Fields {
            void* m_rng; // Xoroshiro128Plus_Xoroshiro128Plus_o*
            uint64_t m_seed;
        };

        inline uint32_t GetRand() {
            return external<uint32_t>(0x024a0420, this);
        }
    };
}
