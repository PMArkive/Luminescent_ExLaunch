#pragma once

#include "helpers.h"
#include "memory/json.h"
#include "memory/string.h"

#include "romdata/data/ExtLocalTrades.h"

#include "logger/logger.h"

const char* BallTypeFilePath = "rom:/Data/ExtraData/MonData/BallType/";

int32_t GetBallTypeNo(int32_t index) {

    nn::string filePath(BallTypeFilePath);
    filePath.append("balltype.json");

    nn::json j = FsHelper::loadJsonFileFromPath(filePath.c_str());
    if (j != nullptr && !j.is_discarded())
    {
        RomData::BallInfo BallInfo = {};
        BallInfo = j.get<RomData::BallInfo>();

        Logger::log("Parsed ball info success\n");

        switch(index) {
            case 0:
                Logger::log("Set Pokeball Info - Success\n");
                BallInfo.ballNo = 4;
                BallInfo.ballName = "Poké Ball";
                break;
        }
        return BallInfo.ballNo;
    }
    else
    {
        Logger::log("Error when parsing Ball Info data!\n");
    }
    Logger::log("Returning default ball type\n");
    return 4;
}