#pragma once

#include "externals/il2cpp-api.h"
#include "memory/json.h"
#include "memory/vector.h"
#include "memory/string.h"

namespace RomData
{
    struct BallInfo
    {
        uint32_t ballNo;
        nn::string ballName;
    };

    struct BallData
    {
        nn::vector<BallInfo> ballInfo;
    };

    JSON_TEMPLATE
    void to_json(GENERIC_JSON& j, const BallInfo& b) {
        j = nn::json {
                {"ballNo", b.ballNo},
                {"ballName", b.ballName},
        };
    }

    JSON_TEMPLATE
    void from_json(const GENERIC_JSON& j, BallInfo& b) {
        j.at("ballNo").get_to(b.ballNo);
        j.at("ballName").get_to(b.ballName);
    }

    JSON_TEMPLATE
    void to_json(GENERIC_JSON& j, const BallData& bd) {
        j = nn::json {
                {"ballInfo", bd.ballInfo},
        };
    }

    JSON_TEMPLATE
    void from_json(const GENERIC_JSON& j, BallData& bd) {
        j.at("ballInfo").get_to(bd.ballInfo);
    }
}