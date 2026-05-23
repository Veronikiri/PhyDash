#pragma once
#include "Core/Scene.h"
#include "Entities/Player.h"
#include "Entities/obstacles.hpp"
#include <vector>
#include <memory>
#include <iostream>

class Game;

class FixedLevelScene : public Scene {
public:
    FixedLevelScene(Game& game, const std::string& levelFile);
    ~FixedLevelScene();

    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void update(sf::Time dt) override;
    void render(sf::RenderWindow& window) override;

private:
    void buildLevel();
    void reset();

    Game& m_game;
    std::unique_ptr<Player> m_player;

    std::vector<Spike> m_spikes;
    std::vector<Block> m_blocks;
    std::vector<Orb> m_orbs;
    std::vector<Pad> m_pads;
    std::vector<Portal> m_portals;

    std::vector<sf::Vector2f> m_initialSpikePositions;
    std::vector<sf::Vector2f> m_initialBlockPositions;

    bool m_alive = true;
    float m_deathTimer = 0.f;
    static constexpr float DEATH_DELAY = 1.0f;

    sf::Text m_deathText;
};