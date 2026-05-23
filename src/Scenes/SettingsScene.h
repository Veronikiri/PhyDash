#pragma once
#include "Core/Scene.h"
#include "UI/Button.h"
#include "UI/BackButton.h"
#include <vector>
#include <memory>

class Game;

class SettingsScene : public Scene {
public:
    SettingsScene(Game& game);
    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void update(sf::Time dt) override;
    void render(sf::RenderWindow& window) override;

    static bool isAutoRestartEnabled() { return s_autoRestart; }
    static bool isHideAttemptsEnabled() { return s_hideAttempts; }
    static bool isShowHitboxesEnabled() { return s_showHitboxes; }

private:
    Game& m_game;
    sf::Text m_title;
    BackButton m_backButton;

    sf::RectangleShape m_autoRestartCheckbox;
    sf::RectangleShape m_hideAttemptsCheckbox;
    sf::RectangleShape m_showHitboxesCheckbox;

    sf::Text m_autoRestartLabel;
    sf::Text m_hideAttemptsLabel;
    sf::Text m_showHitboxesLabel;

    static bool s_autoRestart;
    static bool s_hideAttempts;
    static bool s_showHitboxes;

    bool m_shouldClose = false;
    bool m_isClosed = false;
};