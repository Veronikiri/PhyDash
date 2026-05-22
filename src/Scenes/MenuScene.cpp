#include "Scenes/MenuScene.h"
#include "Core/Game.h"
#include "Scenes/PlayingScene.h"
#include "Scenes/CustomizeScene.h"
#include "Scenes/FixedLevelScene.h"
#include "Scenes/SettingsScene.h"

MenuScene::MenuScene(Game& game) : m_game(game) {
    const sf::Font& font = game.getFont();

    m_title = std::make_unique<sf::Text>(font, "Phy DASH", 110);
    m_title->setFillColor(sf::Color::Cyan);
    m_title->setOutlineThickness(2);
    m_title->setOutlineColor(sf::Color::White);
    sf::FloatRect tRect = m_title->getLocalBounds();
    m_title->setOrigin({ tRect.position.x + tRect.size.x / 2.f, tRect.position.y + tRect.size.y / 2.f });
    m_title->setPosition({ 640.f, 130.f });

    m_shadow = std::make_unique<sf::Text>(*m_title);
    m_shadow->setFillColor(sf::Color(255, 0, 255, 40));
    m_shadow->setOutlineColor(sf::Color(255, 0, 255, 80));
    m_shadow->setOutlineThickness(8);

    std::vector<std::pair<std::string, std::function<void()>>> items = {
        {"START", [this]() { m_game.pushScene(std::make_unique<PlayingScene>(m_game)); }},
        {"MODES", [this]() {
            auto levelScene = std::make_unique<FixedLevelScene>(m_game, "assets/levels/level1.json");
            m_game.pushScene(std::move(levelScene));
        }},
        {"SETTINGS", [this]() { m_game.pushScene(std::make_unique<SettingsScene>(m_game)); }},
        {"CUSTOMIZE", [this]() { m_game.pushScene(std::make_unique<CustomizeScene>(m_game)); }}
    };

    for (size_t i = 0; i < items.size(); ++i) {
        Button btn(font, items[i].first, 34);
        btn.setPosition({ 640.f, 280.f + i * 95.f });
        btn.setColors(sf::Color(0, 255, 255, 20), sf::Color(255, 0, 255, 45), sf::Color::White, sf::Color::Cyan);
        btn.setCallback(items[i].second);
        m_buttons.push_back(std::move(btn));
    }
}

void MenuScene::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    for (auto& btn : m_buttons)
        btn.handleEvent(event, window);
}

void MenuScene::update(sf::Time dt) {
    sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(m_game.getWindow()));
    updateHover(mousePos);
}

void MenuScene::render(sf::RenderWindow& window) {
    window.draw(*m_shadow);
    window.draw(*m_title);
    for (auto& btn : m_buttons)
        btn.draw(window);
}

void MenuScene::updateHover(sf::Vector2f mousePos) {
    for (auto& btn : m_buttons)
        btn.update(mousePos);
}