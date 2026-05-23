#include "Scenes/FixedLevelScene.h"
#include "Core/Game.h"
#include "Utils/PlayerSettings.h"
#include <iostream>

FixedLevelScene::FixedLevelScene(Game& game, const std::string& levelFile)
    : m_game(game)
    , m_deathText(game.getFont())
{
    m_deathText.setString("You died!\nPress R to restart\nPress ESC to exit");
    m_deathText.setCharacterSize(36);
    m_deathText.setFillColor(sf::Color::Red);
    m_deathText.setPosition({ 640.f, 360.f });
    sf::FloatRect bounds = m_deathText.getLocalBounds();
    m_deathText.setOrigin({ bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f });

    buildLevel();

    auto& settings = PlayerSettings::getInstance();
    m_player = settings.createPlayer();
    m_player->setPosition({ 200.f, 525.f });
    m_player->setVelocity({ 0.f, 0.f });
}

FixedLevelScene::~FixedLevelScene() {
    m_spikes.clear();
    m_blocks.clear();
    m_orbs.clear();
    m_pads.clear();
    m_portals.clear();
    m_initialSpikePositions.clear();
    m_initialBlockPositions.clear();
}

void FixedLevelScene::buildLevel() {
    m_spikes.clear();
    m_blocks.clear();
    m_orbs.clear();
    m_pads.clear();
    m_portals.clear();
    m_initialSpikePositions.clear();
    m_initialBlockPositions.clear();

    float startX = 500.f;
    float groundY = 500.f;
    float cell = 50.f;

    int currentX = 0;

    currentX += 19;

    {
        float x = startX + currentX * cell;
        float y = groundY;
        m_spikes.push_back(Spike(x, y));
        m_initialSpikePositions.push_back({ x, y });
        currentX += 1;
    }

    currentX += 7;

    {
        float x = startX + currentX * cell;
        float y = groundY;
        m_spikes.push_back(Spike(x, y));
        m_initialSpikePositions.push_back({ x, y });
        m_spikes.push_back(Spike(x + cell, y));
        m_initialSpikePositions.push_back({ x + cell, y });
        currentX += 2;
    }

    currentX += 3;

    {
        float x = startX + currentX * cell;
        float y = groundY;
        m_spikes.push_back(Spike(x, y));
        m_initialSpikePositions.push_back({ x, y });
        m_spikes.push_back(Spike(x + cell, y));
        m_initialSpikePositions.push_back({ x + cell, y });
        currentX += 2;
    }

    currentX += 3;

    {
        float x = startX + currentX * cell;
        float y = groundY;
        m_spikes.push_back(Spike(x, y));
        m_initialSpikePositions.push_back({ x, y });
        m_spikes.push_back(Spike(x + cell, y));
        m_initialSpikePositions.push_back({ x + cell, y });
        currentX += 2;
    }

    {
        float x = startX + currentX * cell;
        float y = groundY;
        m_blocks.push_back(Block(x, y));
        m_initialBlockPositions.push_back({ x, y });
        m_blocks.push_back(Block(x + cell, y));
        m_initialBlockPositions.push_back({ x + cell, y });
        currentX += 2;
    }

    currentX += 1;

    {
        float x = startX + currentX * cell;
        m_spikes.push_back(Spike(x, groundY - 3 * cell));
        m_initialSpikePositions.push_back({ x, groundY - 3 * cell });
        m_spikes.push_back(Spike(x, groundY - 4 * cell));
        m_initialSpikePositions.push_back({ x, groundY - 4 * cell });
        currentX += 1;
    }

    {
        float x = startX + currentX * cell;
        m_blocks.push_back(Block(x, groundY - 3 * cell));
        m_initialBlockPositions.push_back({ x, groundY - 3 * cell });
        m_blocks.push_back(Block(x, groundY - 4 * cell));
        m_initialBlockPositions.push_back({ x, groundY - 4 * cell });
        currentX += 1;
    }

    currentX += 2;

    {
        float x = startX + currentX * cell;
        float y = groundY;
        m_spikes.push_back(Spike(x, y));
        m_initialSpikePositions.push_back({ x, y });
        currentX += 1;
    }

    int complexStart = currentX;

    for (int i = 0; i < 16; i++) {
        float x = startX + (complexStart + i) * cell;
        float y = groundY;
        m_blocks.push_back(Block(x, y));
        m_initialBlockPositions.push_back({ x, y });
    }

    for (int i = 0; i < 8; i++) {
        float x = startX + (complexStart + i) * cell;
        float y = groundY - cell;
        m_blocks.push_back(Block(x, y));
        m_initialBlockPositions.push_back({ x, y });
    }
    for (int i = 12; i < 16; i++) {
        float x = startX + (complexStart + i) * cell;
        float y = groundY - cell;
        m_blocks.push_back(Block(x, y));
        m_initialBlockPositions.push_back({ x, y });
    }

    {
        float x = startX + (complexStart + 15) * cell;
        float y = groundY - 2 * cell;
        m_spikes.push_back(Spike(x, y));
        m_initialSpikePositions.push_back({ x, y });
    }

    for (int i = 0; i < 11; i++) {
        float x = startX + (complexStart + i) * cell;
        float y = groundY - 4 * cell;
        m_spikes.push_back(Spike(x, y));
        m_initialSpikePositions.push_back({ x, y });
    }

    for (int i = 0; i < 11; i++) {
        float x = startX + (complexStart + i) * cell;
        float y = groundY - 5 * cell;
        m_blocks.push_back(Block(x, y));
        m_initialBlockPositions.push_back({ x, y });
    }

    for (int i = 0; i < 11; i++) {
        float x = startX + (complexStart + i) * cell;
        float y = groundY - 6 * cell;
        m_blocks.push_back(Block(x, y));
        m_initialBlockPositions.push_back({ x, y });
    }

    currentX += 16;

    for (int i = 0; i < 3; i++) {
        float x = startX + (currentX + i) * cell;
        float y = groundY;
        m_spikes.push_back(Spike(x, y));
        m_initialSpikePositions.push_back({ x, y });
    }
    currentX += 3;

    for (int i = 0; i < 6; i++) {
        float x = startX + (currentX + i) * cell;
        m_blocks.push_back(Block(x, groundY));
        m_initialBlockPositions.push_back({ x, groundY });
        m_blocks.push_back(Block(x, groundY - cell));
        m_initialBlockPositions.push_back({ x, groundY - cell });
    }
    currentX += 6;


    {
        float x = startX + (currentX - 6) * cell;
        m_blocks.push_back(Block(x, groundY - 5 * cell));
        m_initialBlockPositions.push_back({ x, groundY - 5 * cell });
        m_blocks.push_back(Block(x, groundY - 6 * cell));
        m_initialBlockPositions.push_back({ x, groundY - 6 * cell });
        m_blocks.push_back(Block(x, groundY - 7 * cell));
        m_initialBlockPositions.push_back({ x, groundY - 7 * cell });
    }

    int blockWidth = 31;
    std::vector<std::vector<int>> levelData(9, std::vector<int>(blockWidth, 0));

    for (int i = 0; i < 31; ++i) levelData[1][i] = 1;

    for (int i = 5; i < 7; ++i) levelData[2][i] = 2;

    int pos3 = 3;
    levelData[3][pos3] = 2; pos3 += 1;
    pos3 += 7;
    for (int i = 0; i < 2; ++i) levelData[3][pos3 + i] = 2; pos3 += 2;
    pos3 += 8;
    for (int i = 0; i < 3; ++i) levelData[3][pos3 + i] = 2; pos3 += 3;

    int pos4 = 9;
    levelData[4][pos4] = 2; pos4 += 1;
    pos4 += 6;
    levelData[4][pos4] = 2; pos4 += 1;
    pos4 += 1;
    levelData[4][pos4] = 2; pos4 += 1;
    pos4 += 8;
    levelData[4][pos4] = 3; pos4 += 1;

    levelData[5][18] = 1;

    levelData[6][11] = 1;

    levelData[7][9] = 1;
    levelData[7][11] = 2;

    levelData[8][9] = 2;

    for (int i = 0; i < blockWidth; ++i) {
        float x = startX + (currentX + i) * cell;
        for (int level = 1; level <= 8; ++level) {
            float y = groundY - (level - 1) * cell;
            int type = levelData[level][i];
            if (type == 1) {
                m_spikes.push_back(Spike(x, y));
                m_initialSpikePositions.push_back({ x, y });
            }
            else if (type == 2) {
                m_blocks.push_back(Block(x, y));
                m_initialBlockPositions.push_back({ x, y });
            }
            else if (type == 3) {
                m_orbs.push_back(Orb(x, y));
            }
        }
    }
    currentX += blockWidth;

    int blockWidth2 = 24;
    std::vector<std::vector<int>> levelData2(7, std::vector<int>(blockWidth2, 0));

    for (int i = 0; i < 24; ++i) levelData2[1][i] = 2;

    for (int i = 0; i < 24; ++i) levelData2[2][i] = 2;

    for (int i = 0; i < 19; ++i) levelData2[3][i] = 2;

    for (int i = 0; i < 14; ++i) levelData2[4][i] = 2;
    for (int i = 14; i < 17; ++i) levelData2[4][i] = 1;

    for (int i = 0; i < 14; ++i) levelData2[5][i] = 2;

    levelData2[6][0] = 4;
    for (int i = 1; i <= 4; ++i) levelData2[6][i] = 1;
    for (int i = 9; i <= 10; ++i) levelData2[6][i] = 1;

    for (int i = 0; i < blockWidth2; ++i) {
        float x = startX + (currentX + i) * cell;
        for (int level = 1; level <= 6; ++level) {
            float y = groundY - (level - 1) * cell;
            int type = levelData2[level][i];
            if (type == 1) {
                m_spikes.push_back(Spike(x, y));
                m_initialSpikePositions.push_back({ x, y });
            }
            else if (type == 2) {
                m_blocks.push_back(Block(x, y));
                m_initialBlockPositions.push_back({ x, y });
            }
            else if (type == 4) {
                m_pads.push_back(Pad(x, y));
            }
        }
    }
    currentX += blockWidth2;

    for (int i = 0; i < 3; ++i) {
        float x = startX + (currentX + i) * cell;
        float y = groundY;
        m_spikes.push_back(Spike(x, y));
        m_initialSpikePositions.push_back({ x, y });
    }
    currentX += 3;

    for (int i = 0; i < 3; ++i) {
        float x = startX + (currentX + i) * cell;
        for (int level = 1; level <= 3; ++level) {
            float y = groundY - (level - 1) * cell;
            m_blocks.push_back(Block(x, y));
            m_initialBlockPositions.push_back({ x, y });
        }
    }
    currentX += 3;

    for (int i = 0; i < 3; ++i) {
        float x = startX + (currentX - 3 + i) * cell;
        if (i == 1) {
            m_spikes.push_back(Spike(x, groundY - 5 * cell));
            m_initialSpikePositions.push_back({ x, groundY - 5 * cell });
            m_blocks.push_back(Block(x, groundY - 6 * cell));
            m_initialBlockPositions.push_back({ x, groundY - 6 * cell });
        }
    }

    for (int i = 0; i < 3; ++i) {
        float x = startX + (currentX + i) * cell;
        float y = groundY;
        m_spikes.push_back(Spike(x, y));
        m_initialSpikePositions.push_back({ x, y });
    }
    currentX += 3;

    {
        float x = startX + currentX * cell;
        for (int level = 1; level <= 3; ++level) {
            float y = groundY - (level - 1) * cell;
            m_blocks.push_back(Block(x, y));
            m_initialBlockPositions.push_back({ x, y });
        }
        currentX += 1;
    }

    for (int i = 0; i < 3; ++i) {
        float x = startX + (currentX + i) * cell;
        float y = groundY;
        m_spikes.push_back(Spike(x, y));
        m_initialSpikePositions.push_back({ x, y });
    }
    currentX += 3;

    for (int i = 0; i < 3; ++i) {
        float x = startX + (currentX + i) * cell;
        for (int level = 1; level <= 3; ++level) {
            float y = groundY - (level - 1) * cell;
            m_blocks.push_back(Block(x, y));
            m_initialBlockPositions.push_back({ x, y });
        }
    }
    currentX += 3;

    std::cout << "Level built! Spikes: " << m_spikes.size() << ", Blocks: " << m_blocks.size()
        << ", Orbs: " << m_orbs.size() << ", Pads: " << m_pads.size() << std::endl;
}

void FixedLevelScene::reset() {
    m_alive = true;
    m_deathTimer = 0.f;

    buildLevel();

    auto& settings = PlayerSettings::getInstance();
    m_player = settings.createPlayer();
    m_player->setPosition({ 200.f, 525.f });
    m_player->setVelocity({ 0.f, 0.f });
    m_player->setOnGround(true);
    m_player->setOnOrb(false);

    std::cout << "Game reset!" << std::endl;
}

void FixedLevelScene::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (!m_alive) {
        if (auto* key = event.getIf<sf::Event::KeyPressed>()) {
            if (key->code == sf::Keyboard::Key::R) {
                reset();
            }
            if (key->code == sf::Keyboard::Key::Escape) {
                m_game.popScene();
            }
        }
        return;
    }

    if (m_player) {
        m_player->handleInput(event);
    }

    if (auto* key = event.getIf<sf::Event::KeyPressed>()) {
        if (key->code == sf::Keyboard::Key::R) {
            reset();
        }
        if (key->code == sf::Keyboard::Key::Escape) {
            m_game.popScene();
        }
    }
}

void FixedLevelScene::update(sf::Time dt) {
    float delta = std::min(dt.asSeconds(), 0.033f);

    if (m_alive && m_player) {
        m_player->update(sf::seconds(delta));

        for (auto& s : m_spikes) s.update(delta);
        for (auto& b : m_blocks) b.update(delta);
        for (auto& o : m_orbs) o.update(delta);
        for (auto& p : m_pads) p.update(delta);
        for (auto& p : m_portals) p.update(delta);

        sf::FloatRect playerBounds = m_player->getBounds();
        sf::Vector2f playerPos = m_player->getPosition();

        // Проверка коллизий с блоками
        bool onGround = false;
        for (const auto& block : m_blocks) {
            sf::FloatRect blockBounds = block.getBounds();
            if (playerBounds.findIntersection(blockBounds)) {
                float playerBottom = playerBounds.position.y + playerBounds.size.y;
                float blockTop = blockBounds.position.y;

                if (m_player->getVelocity().y >= 0 &&
                    playerBottom <= blockTop + 15.f) {
                    m_player->setPosition({ playerPos.x, blockTop - 50.f });
                    m_player->setVelocity({ m_player->getVelocity().x, 0.f });
                    onGround = true;
                    m_player->setOnGround(true);
                }
                else {
                    m_alive = false;
                    std::cout << "Player died by block!" << std::endl;
                    return;
                }
            }
        }

        if (!onGround && playerPos.y >= 525.f - 5.f) {
            m_player->setPosition({ playerPos.x, 525.f });
            m_player->setVelocity({ m_player->getVelocity().x, 0.f });
            onGround = true;
            m_player->setOnGround(true);
        }

        if (!onGround && playerPos.y < 525.f - 5.f) {
            m_player->setOnGround(false);
        }

        // Смерть от шипов
        for (const auto& spike : m_spikes) {
            if (playerBounds.findIntersection(spike.getBounds())) {
                m_alive = false;
                std::cout << "Player died by spike!" << std::endl;
                break;
            }
        }

        // Орбы
        if (m_alive) {
            bool orbTouched = false;
            for (auto& orb : m_orbs) {
                if (playerBounds.findIntersection(orb.getBounds())) {
                    m_player->setOnOrb(true);
                    orbTouched = true;
                    break;
                }
            }
            if (!orbTouched) m_player->setOnOrb(false);
        }

        // Пады
        if (m_alive) {
            for (auto& pad : m_pads) {
                if (playerBounds.findIntersection(pad.getBounds())) {
                    m_player->setVelocity({ m_player->getVelocity().x, -800.f });
                    m_player->setOnGround(false);
                    break;
                }
            }
        }

        if (m_alive && m_player->getPosition().y > 700.f) {
            m_alive = false;
            std::cout << "Player fell into void!" << std::endl;
        }
    }
    else if (!m_alive) {
        m_deathTimer += delta;
        if (m_deathTimer >= DEATH_DELAY) {
            reset();
        }
    }
}

void FixedLevelScene::render(sf::RenderWindow& window) {
    if (!m_player) return;

    sf::View defaultView = window.getDefaultView();

    sf::View gameView;
    gameView.setCenter({ m_player->getPosition().x + 300.f, 360.f });
    gameView.setSize({ 1280.f, 720.f });
    window.setView(gameView);

    sf::RectangleShape floor({ 20000.f, 10.f });
    floor.setPosition({ -5000.f, 550.f });
    floor.setFillColor(sf::Color(80, 80, 100));
    window.draw(floor);

    for (auto& s : m_spikes) s.draw(window);
    for (auto& b : m_blocks) b.draw(window);
    for (auto& o : m_orbs) o.draw(window);
    for (auto& p : m_pads) p.draw(window);
    for (auto& p : m_portals) p.draw(window);

    m_player->draw(window);

    window.setView(defaultView);

    if (!m_alive) {
        window.draw(m_deathText);
    }
}