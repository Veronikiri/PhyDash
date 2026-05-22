#include "Scenes/FixedLevelScene.h"
#include "Core/Game.h"
#include "Utils/LevelLoader.h"
#include <sstream>
#include <iostream>
#include "Scenes/SettingsScene.h"

FixedLevelScene::FixedLevelScene(Game& game, const std::string& levelFile)
    : m_game(game)
    , m_deathText(game.getFont())
{
    if (!m_music.openFromFile("assets/music/music1.ogg"))
        std::cerr << "Warning: music not found" << std::endl;
    else
        m_music.play();

    if (!m_deathSound.openFromFile("assets/music/deathsound.ogg"))
        std::cerr << "Warning: death sound not found" << std::endl;

    m_deathText.setString("You died!\nPress R to restart");
    m_deathText.setCharacterSize(36);
    m_deathText.setFillColor(sf::Color::Red);
    m_deathText.setPosition({ 640.f, 360.f });
    sf::FloatRect bounds = m_deathText.getLocalBounds();
    m_deathText.setOrigin({ bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f });

    if (!LevelLoader::loadLevel(levelFile, m_spikes, m_blocks, m_orbs, m_pads, m_portals)) {
        std::cerr << "Failed to load level, falling back to empty" << std::endl;
    }

    m_player = Player::createForm(PlayerForm::Cube);
    m_player->setPosition({ 200.f, 500.f });

    reset();
}

FixedLevelScene::~FixedLevelScene() {
    m_music.stop();
}

void FixedLevelScene::reset() {
    m_alive = true;
    m_deathTimer = 0.f;
    m_player = Player::createForm(PlayerForm::Cube);
    m_player->setPosition({ 200.f, 500.f });
    m_player->resetPosition();
    m_music.stop();
    m_music.play();
}

void FixedLevelScene::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (!m_alive) {
        if (auto* key = event.getIf<sf::Event::KeyPressed>()) {
            if (key->code == sf::Keyboard::Key::R) {
                reset();
            }
        }
        return;
    }

    m_player->handleInput(event);
    if (auto* key = event.getIf<sf::Event::KeyPressed>()) {
        if (key->code == sf::Keyboard::Key::R)
            reset();
    }
}

void FixedLevelScene::update(sf::Time dt) {
    float delta = std::min(dt.asSeconds(), 0.033f);

    if (m_alive) {
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
        if (!orbTouched) m_player->setOnOrb(false);

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
                auto newPlayer = Player::createForm(newForm);
                newPlayer->setPosition(m_player->getPosition());
                newPlayer->setColors(m_player->getPrimaryColor(), m_player->getSecondaryColor(),
                    m_player->getGlowColor(), m_player->isGlowEnabled());
                newPlayer->setOnOrb(m_player->isOnOrb());
                m_player = std::move(newPlayer);
                break;
            }
        }

        if (m_player->getPosition().y > 600.f) {
            m_alive = false;
            m_deathSound.play();
        }

    }
    else {
        if (SettingsScene::isAutoRestartEnabled()) {
            reset();
        }
        else {
            m_deathTimer += delta;
            if (m_deathTimer >= 1.0f) {
                reset();
            }
        }
    }
}

void FixedLevelScene::render(sf::RenderWindow& window) {
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
    if (!m_alive) {
        window.draw(m_deathText);
    }
}