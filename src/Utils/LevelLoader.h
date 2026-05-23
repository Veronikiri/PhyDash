#pragma once
#include <vector>
#include <string>
#include "Entities/obstacles.hpp"
#include "Entities/NewObstacles.h"

class LevelLoader {
public:
    static bool loadLevel(const std::string& filename,
        std::vector<Spike>& spikes,
        std::vector<Block>& blocks,
        std::vector<Orb>& orbs,
        std::vector<Pad>& pads,
        std::vector<Portal>& portals,
        std::vector<ChainObstacle>& chains,
        std::vector<SlopeObstacle>& slopes,
        std::vector<HalfSpikeObstacle>& halfSpikes);
};