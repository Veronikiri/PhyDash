#include "Utils/LevelLoader.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <functional>

using json = nlohmann::json;

// Расшифровка типов Geometry Dash -> наши препятствия
static const std::unordered_map<int, std::function<void(float x, float y,
    std::vector<Spike>&, std::vector<Block>&, std::vector<Orb>&,
    std::vector<Pad>&, std::vector<Portal>&,
    std::vector<ChainObstacle>&, std::vector<SlopeObstacle>&,
    std::vector<HalfSpikeObstacle>&)>> typeMap = {

        // Шипы (Spike)
        {9, [](float x, float y, auto& spikes, auto&, auto&, auto&, auto&, auto&, auto&, auto&) { spikes.emplace_back(x, y + 12); }},
        {39, [](float x, float y, auto& spikes, auto&, auto&, auto&, auto&, auto&, auto&, auto&) { spikes.emplace_back(x, y - 8.5f); }},
        {61, [](float x, float y, auto& spikes, auto&, auto&, auto&, auto&, auto&, auto&, auto&) { spikes.emplace_back(x, y + 11); }},
        {62, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},
        {64, [](float x, float y, auto&, auto&, auto&, auto&, auto&, auto&, auto&, auto&) { /* wavy corner – пропускаем */ }},
        {178, [](float x, float y, auto& spikes, auto&, auto&, auto&, auto&, auto&, auto&, auto&) { spikes.emplace_back(x, y - 8.5f); }},
        {243, [](float x, float y, auto& spikes, auto&, auto&, auto&, auto&, auto&, auto&, auto&) { spikes.emplace_back(x, y + 11); }},
        {244, [](float x, float y, auto& spikes, auto&, auto&, auto&, auto&, auto&, auto&, auto&) { spikes.emplace_back(x, y + 11); }},

        // Цепи (Chain)
        {40, [](float x, float y, auto&, auto&, auto&, auto&, auto&, auto& chains, auto&, auto&) { chains.emplace_back(x, y); }},
        {41, [](float x, float y, auto&, auto&, auto&, auto&, auto&, auto& chains, auto&, auto&) { chains.emplace_back(x, y); }},

        // Склон (Slope)
        {42, [](float x, float y, auto&, auto&, auto&, auto&, auto&, auto&, auto& slopes, auto&) { slopes.emplace_back(x, y); }},
        {43, [](float x, float y, auto&, auto&, auto&, auto&, auto&, auto&, auto& slopes, auto&) { slopes.emplace_back(x, y); }},

        // Маленький шип (HalfSpike)
        {39, [](float x, float y, auto&, auto&, auto&, auto&, auto&, auto&, auto&, auto& halfSpikes) { halfSpikes.emplace_back(x, y); }},

        // Блоки (Block)
        {1, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},
        {2, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},
        {3, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},
        {4, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},
        {5, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},
        {6, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},
        {7, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},
        {8, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},
        {16, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},
        {22, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},
        {23, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},
        {35, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},
        {36, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},
        {40, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},
        {85, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},
        {88, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},
        {106, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},
        {129, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},
        {130, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},

        // Пады (Pad) – автоматический прыжок
        {35, [](float x, float y, auto&, auto&, auto&, auto& pads, auto&, auto&, auto&, auto&) { pads.emplace_back(x, y); }},
        {67, [](float x, float y, auto&, auto&, auto&, auto& pads, auto&, auto&, auto&, auto&) { pads.emplace_back(x, y); }},
        {140, [](float x, float y, auto&, auto&, auto&, auto& pads, auto&, auto&, auto&, auto&) { pads.emplace_back(x, y); }},
        {1332, [](float x, float y, auto&, auto&, auto&, auto& pads, auto&, auto&, auto&, auto&) { pads.emplace_back(x, y); }},

        // Орбы (Orb)
        // В JSON орбы обычно имеют тип 45, 46 и т.д. Добавим основные
        {45, [](float x, float y, auto&, auto&, auto& orbs, auto&, auto&, auto&, auto&, auto&) { orbs.emplace_back(x, y); }},
        {46, [](float x, float y, auto&, auto&, auto& orbs, auto&, auto&, auto&, auto&, auto&) { orbs.emplace_back(x, y); }},
        {47, [](float x, float y, auto&, auto&, auto& orbs, auto&, auto&, auto&, auto&, auto&) { orbs.emplace_back(x, y); }},

        // Портал (Portal) – смена формы
        {12, [](float x, float y, auto&, auto&, auto&, auto&, auto& portals, auto&, auto&, auto&) { portals.emplace_back(x, y, PlayerForm::Cube); }},
        {13, [](float x, float y, auto&, auto&, auto&, auto&, auto& portals, auto&, auto&, auto&) { portals.emplace_back(x, y, PlayerForm::Ship); }},
        {47, [](float x, float y, auto&, auto&, auto&, auto&, auto& portals, auto&, auto&, auto&) { portals.emplace_back(x, y, PlayerForm::Ball); }},
        {111, [](float x, float y, auto&, auto&, auto&, auto&, auto& portals, auto&, auto&, auto&) { portals.emplace_back(x, y, PlayerForm::Ufo); }},
        {660, [](float x, float y, auto&, auto&, auto&, auto&, auto& portals, auto&, auto&, auto&) { portals.emplace_back(x, y, PlayerForm::Wave); }},
        {745, [](float x, float y, auto&, auto&, auto&, auto&, auto& portals, auto&, auto&, auto&) { portals.emplace_back(x, y, PlayerForm::Robot); }},
        {1331, [](float x, float y, auto&, auto&, auto&, auto&, auto& portals, auto&, auto&, auto&) { portals.emplace_back(x, y, PlayerForm::Spider); }},

        // Большие порталы
        {203, [](float x, float y, auto&, auto&, auto&, auto&, auto& portals, auto&, auto&, auto&) { portals.emplace_back(x, y, PlayerForm::Ship); }},
        {1334, [](float x, float y, auto&, auto&, auto&, auto&, auto& portals, auto&, auto&, auto&) { portals.emplace_back(x, y, PlayerForm::Cube); }},
};

bool LevelLoader::loadLevel(const std::string& filename,
    std::vector<Spike>& spikes,
    std::vector<Block>& blocks,
    std::vector<Orb>& orbs,
    std::vector<Pad>& pads,
    std::vector<Portal>& portals,
    std::vector<ChainObstacle>& chains,
    std::vector<SlopeObstacle>& slopes,
    std::vector<HalfSpikeObstacle>& halfSpikes) {

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open level file: " << filename << std::endl;
        return false;
    }

    json data;
    try {
        file >> data;
    }
    catch (const json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        return false;
    }

    float scale = 1.0f;

    for (const auto& obj : data["objects"]) {
        int type = obj["type"];
        float x = obj["x"] * scale;
        float y = (1050.0f - obj["y"]) * scale;

        auto it = typeMap.find(type);
        if (it != typeMap.end()) {
            it->second(x, y, spikes, blocks, orbs, pads, portals, chains, slopes, halfSpikes);
        }
    }

    std::cout << "Level loaded: spikes=" << spikes.size()
        << ", blocks=" << blocks.size()
        << ", orbs=" << orbs.size()
        << ", pads=" << pads.size()
        << ", portals=" << portals.size()
        << ", chains=" << chains.size()
        << ", slopes=" << slopes.size()
        << ", halfSpikes=" << halfSpikes.size() << std::endl;
    return true;
}