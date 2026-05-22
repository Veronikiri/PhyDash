#include "Utils/LevelLoader.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <functional>

using json = nlohmann::json;

// Расшифровка типов Geometry Dash -> наши препятствия
// Основано на предоставленных данных пользователя
static const std::unordered_map<int, std::function<void(float x, float y,
    std::vector<Spike>&, std::vector<Block>&, std::vector<Orb>&,
    std::vector<Pad>&, std::vector<Portal>&)>> typeMap = {

        // Шипы (Spike)
        {9, [](float x, float y, auto& spikes, auto&, auto&, auto&, auto&) { spikes.emplace_back(x, y + 12); }},      // spike floor
        {39, [](float x, float y, auto& spikes, auto&, auto&, auto&, auto&) { spikes.emplace_back(x, y - 8.5f); }}, // half spike
        {61, [](float x, float y, auto& spikes, auto&, auto&, auto&, auto&) { spikes.emplace_back(x, y + 11); }},   // wavy spike floor
        {62, [](float x, float y, auto& spikes, auto& blocks, auto&, auto&, auto&) { /* wavy block – интерпретируем как блок */ blocks.emplace_back(x, y); }},
        {64, [](float x, float y, auto& spikes, auto&, auto&, auto&, auto&) { /* wavy corner – можно пропустить или сделать блок */ }},
        {178, [](float x, float y, auto& spikes, auto&, auto&, auto&, auto&) { spikes.emplace_back(x, y - 8.5f); }}, // half electro spike
        {243, [](float x, float y, auto& spikes, auto&, auto&, auto&, auto&) { spikes.emplace_back(x, y + 11); }},   // wavy spikes right
        {244, [](float x, float y, auto& spikes, auto&, auto&, auto&, auto&) { spikes.emplace_back(x, y + 11); }},   // wavy spikes left

        // Блоки (Block) – большинство твёрдых объектов
        {1, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},
        {2, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},
        {3, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},
        {4, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},
        {5, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},
        {6, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},
        {7, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},
        {8, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},
        {16, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},
        {22, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},
        {23, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},
        {35, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},
        {36, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},
        {40, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},
        {41, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},
        {85, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},   // sawblade deco – можно как блок
        {88, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},   // large sawblade
        {106, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},  // chain
        {129, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},  // 3-wide cloud
        {130, [](float x, float y, auto&, auto& blocks, auto&, auto&, auto&) { blocks.emplace_back(x, y); }},  // 4-wide cloud

        // Пады (Pad) – автоматический прыжок
        {35, [](float x, float y, auto&, auto&, auto&, auto& pads, auto&) { pads.emplace_back(x, y); }},   // желтый пад
        {67, [](float x, float y, auto&, auto&, auto&, auto& pads, auto&) { pads.emplace_back(x, y); }},   // синий пад
        {140, [](float x, float y, auto&, auto&, auto&, auto& pads, auto&) { pads.emplace_back(x, y); }},  // розовый пад
        {1332, [](float x, float y, auto&, auto&, auto&, auto& pads, auto&) { pads.emplace_back(x, y); }}, // красный пад

        // Орбы (Orb) – дают возможность прыгнуть при нажатии
        // В GD орбы имеют тип? Не указан в расшифровке. В JSON они обычно типа 45? Упростим: если объект с типом 45,46? Но нет. Лучше оставить пустыми.

        // Портал (Portal) – смена формы
        {12, [](float x, float y, auto&, auto&, auto&, auto&, auto& portals) { portals.emplace_back(x, y, PlayerForm::Cube); }},
        {13, [](float x, float y, auto&, auto&, auto&, auto&, auto& portals) { portals.emplace_back(x, y, PlayerForm::Ship); }},
        {47, [](float x, float y, auto&, auto&, auto&, auto&, auto& portals) { portals.emplace_back(x, y, PlayerForm::Ball); }},
        {111, [](float x, float y, auto&, auto&, auto&, auto&, auto& portals) { portals.emplace_back(x, y, PlayerForm::Ufo); }},
        {660, [](float x, float y, auto&, auto&, auto&, auto&, auto& portals) { portals.emplace_back(x, y, PlayerForm::Wave); }},
        {745, [](float x, float y, auto&, auto&, auto&, auto&, auto& portals) { portals.emplace_back(x, y, PlayerForm::Robot); }},
        {1331, [](float x, float y, auto&, auto&, auto&, auto&, auto& portals) { portals.emplace_back(x, y, PlayerForm::Spider); }},

        // Большие порталы (3x,4x) – тоже смена формы (например, 203,1334)
        {203, [](float x, float y, auto&, auto&, auto&, auto&, auto& portals) { portals.emplace_back(x, y, PlayerForm::Ship); }}, // пример
        {1334, [](float x, float y, auto&, auto&, auto&, auto&, auto& portals) { portals.emplace_back(x, y, PlayerForm::Cube); }},
};

bool LevelLoader::loadLevel(const std::string& filename,
    std::vector<Spike>& spikes,
    std::vector<Block>& blocks,
    std::vector<Orb>& orbs,
    std::vector<Pad>& pads,
    std::vector<Portal>& portals) {
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

    float scale = 1.0f; // масштаб, при необходимости

    for (const auto& obj : data["objects"]) {
        int type = obj["type"];
        float x = obj["x"] * scale;
        float y = (1050.0f - obj["y"]) * scale;

        auto it = typeMap.find(type);
        if (it != typeMap.end()) {
            it->second(x, y, spikes, blocks, orbs, pads, portals);
        }
    }

    std::cout << "Level loaded: spikes=" << spikes.size()
        << ", blocks=" << blocks.size()
        << ", pads=" << pads.size()
        << ", portals=" << portals.size() << std::endl;
    return true;
}