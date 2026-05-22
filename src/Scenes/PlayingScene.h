#pragma once
#include "Core/Scene.h"
#include "Entities/Player.h"
#include "Entities/obstacles.hpp"
#include <vector>
#include <memory>
#include <random>          // <-- добавлено!
#include <SFML/Audio.hpp>

class Game;

class PlayingScene : public Scene {
public:
    PlayingScene(Game& game);
    ~PlayingScene();

    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void update(sf::Time dt) override;
    void render(sf::RenderWindow& window) override;

private:
    void reset();
    void generateNewPattern();
    bool allObstaclesOffscreen() const;

    Game& m_game;
    std::unique_ptr<Player> m_player;

    std::vector<Spike> m_spikes;
    std::vector<Block> m_blocks;
    std::vector<Orb> m_orbs;
    std::vector<Pad> m_pads;
    std::vector<Portal> m_portals;

    bool m_alive;
    float m_score;
    int m_best;
    float m_deathTimer;
    static constexpr float DEATH_DELAY = 1.0f;

    sf::Text m_scoreText;
    sf::Text m_bestText;

    sf::Music m_music;
    sf::Music m_deathSound;

    std::vector<ObstaclePattern> m_patternPool;
    std::mt19937 m_rng;
};