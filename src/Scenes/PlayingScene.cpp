#include "Scenes/PlayingScene.h"
#include "Core/Game.h"
#include "Entities/PlayerCube.h"
#include "Entities/PlayerShip.h"
#include <random>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "Scenes/SettingsScene.h"

PlayingScene::PlayingScene(Game& game)
    : m_game(game)
    , m_scoreText(game.getFont())
    , m_bestText(game.getFont())
    , m_rng(std::random_device{}())
{
    m_scoreText.setCharacterSize(36);
    m_scoreText.setFillColor(sf::Color::White);
    m_scoreText.setPosition({ 20.f, 20.f });

    m_bestText.setCharacterSize(36);
    m_bestText.setFillColor(sf::Color::White);
    m_bestText.setPosition({ 20.f, 60.f });

    if (!m_music.openFromFile("assets/music/music1.ogg"))
        std::cerr << "Warning: Could not load music1.ogg" << std::endl;
    else {
        m_music.setLooping(true);
        m_music.setVolume(100.f);
        m_music.play();
    }

    if (!m_deathSound.openFromFile("assets/music/deathsound.ogg"))
        std::cerr << "Warning: Could not load deathsound.ogg" << std::endl;
    else {
        m_deathSound.setLooping(false);
        m_deathSound.setVolume(50.f);
    }

    m_patternPool = createPatternPool();

    m_player = Player::createForm(PlayerForm::Cube);
    m_player->setPosition({ 200.f, 525.f });

    reset();
}

PlayingScene::~PlayingScene() {
    m_music.stop();
}

void PlayingScene::reset() {
    m_alive = true;
    m_score = 0.f;
    m_deathTimer = 0.f;

    m_player = Player::createForm(PlayerForm::Cube);
    m_player->setPosition({ 200.f, 525.f });
    m_player->resetPosition();

    m_spikes.clear();
    m_blocks.clear();
    m_orbs.clear();
    m_pads.clear();
    m_portals.clear();

    generateNewPattern();

    m_music.stop();
    m_music.play();
}

void PlayingScene::generateNewPattern() {
    std::uniform_int_distribution<> dist(0, static_cast<int>(m_patternPool.size()) - 1);
    const auto& pattern = m_patternPool[dist(m_rng)];

    float baseX = m_player->getPosition().x + 800.f;

    for (const auto& s : pattern.spikes) {
        Spike newSpike = s;
        newSpike.shape.move({ baseX, 0.f });
        newSpike.hitbox.move({ baseX, 0.f });
        m_spikes.push_back(newSpike);
    }
    for (const auto& b : pattern.blocks) {
        Block newBlock = b;
        newBlock.shape.move({ baseX, 0.f });
        newBlock.hitbox.move({ baseX, 0.f });
        m_blocks.push_back(newBlock);
    }
    for (const auto& o : pattern.orbs) {
        Orb newOrb = o;
        newOrb.shape.move({ baseX, 0.f });
        newOrb.hitbox.move({ baseX, 0.f });
        m_orbs.push_back(newOrb);
    }
    for (const auto& p : pattern.pads) {
        Pad newPad = p;
        newPad.shape.move({ baseX, 0.f });
        newPad.hitbox.move({ baseX, 0.f });
        m_pads.push_back(newPad);
    }
    for (const auto& p : pattern.portals) {
        Portal newPortal = p;
        newPortal.hitbox.move({ baseX, 0.f });
        m_portals.push_back(newPortal);
    }
}

void PlayingScene::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (!m_alive) return;

    m_player->handleInput(event);

    if (auto* key = event.getIf<sf::Event::KeyPressed>()) {
        if (key->code == sf::Keyboard::Key::R)
            reset();
    }
}

void PlayingScene::update(sf::Time dt) {
    float delta = std::min(dt.asSeconds(), 0.033f);

    if (m_alive) {
        m_score += delta * 10.f;
        std::stringstream ss;
        ss << "Score: " << static_cast<int>(m_score);
        m_scoreText.setString(ss.str());

        m_player->update(sf::seconds(delta));

        for (auto& s : m_spikes) s.update(delta);
        for (auto& b : m_blocks) b.update(delta);
        for (auto& o : m_orbs)   o.update(delta);
        for (auto& p : m_pads)   p.update(delta);
        for (auto& p : m_portals) p.update(delta);

        sf::FloatRect playerBounds = m_player->getBounds();

        for (const auto& spike : m_spikes) {
            if (playerBounds.findIntersection(spike.getBounds())) {
                m_alive = false;
                m_deathSound.play();
                break;
            }
        }

        bool orbTouched = false;
        for (auto& orb : m_orbs) {
            if (playerBounds.findIntersection(orb.getBounds())) {
                m_player->setOnOrb(true);
                orbTouched = true;
                orb.shape.setFillColor(sf::Color(255, 200, 50, 80));
                break;
            }
        }
        if (!orbTouched) {
            m_player->setOnOrb(false);
        }

        for (auto& pad : m_pads) {
            if (playerBounds.findIntersection(pad.getBounds())) {
                m_player->setVelocity({ m_player->getVelocity().x, -800.f });
                pad.shape.setFillColor(sf::Color(0, 255, 100, 100));
                break;
            }
        }

        for (auto& portal : m_portals) {
            if (playerBounds.findIntersection(portal.getBounds())) {
                PlayerForm newForm = portal.getForm();
                if (newForm == PlayerForm::Cube || newForm == PlayerForm::Ship) {
                    auto newPlayer = Player::createForm(newForm);
                    newPlayer->setPosition(m_player->getPosition());
                    newPlayer->setColors(m_player->getPrimaryColor(), m_player->getSecondaryColor(),
                        m_player->getGlowColor(), m_player->isGlowEnabled());
                    newPlayer->setOnOrb(m_player->isOnOrb());
                    m_player = std::move(newPlayer);
                }
                break;
            }
        }

        static float lastGenerationX = 0.f;
        if (m_player->getPosition().x > lastGenerationX + 400.f) {
            generateNewPattern();
            lastGenerationX = m_player->getPosition().x;
        }

        auto removeOffscreen = [](auto& vec) {
            vec.erase(std::remove_if(vec.begin(), vec.end(),
                [](const auto& obj) { return obj.getBounds().position.x + obj.getBounds().size.x < -200.f; }),
                vec.end());
            };
        removeOffscreen(m_spikes);
        removeOffscreen(m_blocks);
        removeOffscreen(m_orbs);
        removeOffscreen(m_pads);
        removeOffscreen(m_portals);

    }
    else {
        if (SettingsScene::isAutoRestartEnabled()) {
            reset();
        }
        else {
            m_deathTimer += delta;
            if (m_deathTimer >= 1.0f) {
                int intScore = static_cast<int>(m_score);
                if (intScore > m_best) {
                    m_best = intScore;
                    std::stringstream ss;
                    ss << "Best: " << m_best;
                    m_bestText.setString(ss.str());
                }
                reset();
            }
        }
    }
}

void PlayingScene::render(sf::RenderWindow& window) {
    sf::View gameView({ m_player->getPosition().x + 400.f, 360.f }, { 1280.f, 720.f });
    window.setView(gameView);

    sf::RectangleShape floor({ 100000.f, 4.f });
    floor.setPosition({ 0.f, 550.f });
    floor.setFillColor(sf::Color(100, 100, 100));
    window.draw(floor);

    for (auto& s : m_spikes) s.draw(window);
    for (auto& b : m_blocks) b.draw(window);
    for (auto& o : m_orbs)   o.draw(window);
    for (auto& p : m_pads)   p.draw(window);
    for (auto& p : m_portals) p.draw(window);

    if (SettingsScene::isShowHitboxesEnabled()) {
        sf::RectangleShape debugBounds;
        debugBounds.setOutlineThickness(2.f);
        debugBounds.setOutlineColor(sf::Color::Red);
        debugBounds.setFillColor(sf::Color::Transparent);

        debugBounds.setSize({ m_player->getBounds().size.x, m_player->getBounds().size.y });
        debugBounds.setPosition(m_player->getBounds().position);
        window.draw(debugBounds);
    }

    m_player->draw(window);

    window.setView(window.getDefaultView());
    window.draw(m_scoreText);
    window.draw(m_bestText);
}