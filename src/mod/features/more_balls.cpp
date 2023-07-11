#include "exlaunch.hpp"

#include "data/balls.h"
#include "data/utils.h"

#include "externals/Dpr/Battle/View/Systems/BattleViewSystem.h"
#include "externals/Dpr/Field/FieldEncount/ENC_FLD_SPA.h"
#include "externals/Dpr/SealPreview/SealPreviewViewSystem.h"
#include "externals/System/String.h"

#include "logger/logger.h"

System::String::Object * GenerateBallModelPath(uint8_t ballId)
{
    if (ballId < BALL_COUNT)
    {
        std::string objectId = std::to_string(((int32_t)ballId) + 200);
        Logger::log("GenerateBallModelPath: ob0%03d_00\n", ((int32_t)ballId) + 200);
        return System::String::Create("objects/ob0" + objectId + "_00");
    }
    else
    {
        Logger::log("GenerateBallModelPath: ob0200_00\n");
        return System::String::Create("objects/ob0200_00");
    }
}

System::String::Object * GenerateSubstituteModelPath()
{
    Logger::log("GenerateSubstituteModelPath: ob0501_00\n");
    return System::String::Create("objects/ob0501_00");
}

HOOK_DEFINE_REPLACE(IsStrangeBall) {
    static bool Callback(uint8_t ballid) {
        Logger::log("IsStrangeBall for ballid %d is %d\n", ballid, ballid >= BALL_COUNT || ballid == 0);
        return ballid >= BALL_COUNT || ballid == 0;
    }
};

HOOK_DEFINE_INLINE(InlineGetBallModelPath) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        uint8_t ballid = ctx->W[24];
        Logger::log("InlineGetBallModelPath with ballId %d\n", ballid);
        ctx->X[0] = (uint64_t)GenerateBallModelPath(ballid);
    }
};

HOOK_DEFINE_REPLACE(BattleViewSystemGetBallModelPath) {
    static System::String::Object * Callback(Dpr::Battle::View::Systems::BattleViewSystem::Object *__this, int32_t idx) {
        if (idx < __this->fields.m_effectBallInfo->max_length)
        {
            uint8_t ballid = __this->fields.m_effectBallInfo->m_Items[idx].fields.ballId;
            Logger::log("BattleViewSystemGetBallModelPath with ballId %d\n", ballid);
            return GenerateBallModelPath(ballid);
        }
        else
        {
            Logger::log("BattleViewSystemGetBallModelPath with 0\n");
            return GenerateBallModelPath(0);
        }
    }
};

HOOK_DEFINE_REPLACE(SealPreviewViewSystemGetBallModelPath) {
    static System::String::Object * Callback(Dpr::SealPreview::SealPreviewViewSystem::Object *__this, int32_t idx) {
        if (idx < __this->fields.m_effectBallInfo->max_length)
        {
            uint8_t ballid = __this->fields.m_effectBallInfo->m_Items[idx].fields.ballId;
            Logger::log("SealPreviewViewSystemGetBallModelPath with ballId %d\n", ballid);
            return GenerateBallModelPath(ballid);
        }
        else
        {
            Logger::log("SealPreviewViewSystemGetBallModelPath with 0\n");
            return GenerateBallModelPath(0);
        }
    }
};

HOOK_DEFINE_REPLACE(UtilsGetBallModelPath) {
    static System::String::Object * Callback(uint8_t ballId) {
        Logger::log("UtilsGetBallModelPath with ballId %d\n", ballId);
        return GenerateBallModelPath(ballId);
    }
};

HOOK_DEFINE_REPLACE(UtilsGetBallModelPath2) {
    static System::String::Object * Callback(uint8_t ballId) {
        Logger::log("UtilsGetBallModelPath2 with ballId %d\n", ballId);
        return GenerateBallModelPath(ballId);
    }
};

HOOK_DEFINE_INLINE(InlineGetSubstituteModelPath) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        ctx->X[0] = (uint64_t)GenerateSubstituteModelPath();
    }
};

HOOK_DEFINE_INLINE(InlineSubstituteModelPathEquality) {
    static void Callback(exl::hook::nx64::InlineCtx* ctx) {
        ctx->W[0] = (uint32_t)System::String::op_Equality((System::String::Object*)ctx->X[0], GenerateSubstituteModelPath());
    }
};

void exl_balls_main() {
    IsStrangeBall::InstallAtOffset(0x0249f130);
    InlineGetBallModelPath::InstallAtOffset(0x01db6cb4);
    BattleViewSystemGetBallModelPath::InstallAtOffset(0x01c92790);
    SealPreviewViewSystemGetBallModelPath::InstallAtOffset(0x01bd9aa0);
    UtilsGetBallModelPath::InstallAtOffset(0x0186b4a0);
    UtilsGetBallModelPath2::InstallAtOffset(0x01e4df50);
    //InlineGetSubstituteModelPath::InstallAtOffset(0x0173c81c);
    //InlineGetSubstituteModelPath::InstallAtOffset(0x017b3fb0);
    //InlineGetSubstituteModelPath::InstallAtOffset(0x01e2be40);
    //InlineSubstituteModelPathEquality::InstallAtOffset(0x01e54ec4);

    // Assembly Patches
    using namespace exl::armv8::inst;
    using namespace exl::armv8::reg;
    exl::patch::CodePatcher p(0);
    auto inst = std::vector {
            std::make_pair<uint32_t, Instruction>(0x01db6a40, Movz(W20, BALL_COUNT + 2)),
            std::make_pair<uint32_t, Instruction>(0x01db7488, Movz(W10, array_index(BALLS, "--BALL ZERO--"))),
            std::make_pair<uint32_t, Instruction>(0x01db6e78, Movz(W8, array_index(BALLS, "--BALL ZERO--"))),
            std::make_pair<uint32_t, Instruction>(0x01db7244, Movz(W10, array_index(BALLS, "--BALL ZERO--"))),
            std::make_pair<uint32_t, Instruction>(0x01db6f18, Movz(W28, array_index(BALLS, "--BALL ZERO--"))),
            std::make_pair<uint32_t, Instruction>(0x01db6c08, Movz(W8, array_index(BALLS, "--BALL ZERO--"))),
            std::make_pair<uint32_t, Instruction>(0x01db6c38, CmpImmediate(W9, array_index(BALLS, "--BALL ZERO--"))),
            std::make_pair<uint32_t, Instruction>(0x01abc8dc, Movz(W8, array_index(BALLS, "--BALL ZERO--") + 200)),
    };
    p.WriteInst(inst);
}
