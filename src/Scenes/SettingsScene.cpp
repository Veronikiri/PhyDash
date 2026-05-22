#include "Scenes/SettingsScene.h"
#include "Core/Game.h"

bool SettingsScene::s_autoRestart = false;
bool SettingsScene::s_hideAttempts = false;
bool SettingsScene::s_showHitboxes = false;

SettingsScene::SettingsScene(Game& game)
    : m_game(game)
    , m_title(game.getFont(), "SETTINGS", 56)
    , m_backButton(game.getFont())
    , m_autoRestartLabel(game.getFont(), "Auto Restart", 28)
    , m_hideAttemptsLabel(game.getFont(), "Hide Attempts", 28)
    , m_showHitboxesLabel(game.getFont(), "Show Hitboxes", 28)
{
    m_title.setFillColor(sf::Color::Cyan);
    m_title.setOutlineThickness(2.f);
    m_title.setOutlineColor(sf::Color::White);
    sf::FloatRect tr = m_title.getLocalBounds();
    m_title.setOrigin({ tr.position.x + tr.size.x / 2.f, tr.position.y + tr.size.y / 2.f });
    m_title.setPosition({ 640.f, 80.f });

    m_backButton.setPosition({ 100.f, 680.f });
    m_backButton.setCallback([this]() { m_shouldClose = true; });

    float startY = 200.f;
    float labelX = 300.f;
    float checkboxX = 800.f;
    float spacing = 80.f;
    float checkboxSize = 30.f;

    // Auto Restart
    m_autoRestartLabel.setPosition({ labelX, startY });
    m_autoRestartLabel.setFillColor(sf::Color::White);

    m_autoRestartCheckbox.setSize({ checkboxSize, checkboxSize });
    m_autoRestartCheckbox.setOutlineThickness(2.f);
    m_autoRestartCheckbox.setOutlineColor(sf::Color::White);
    m_autoRestartCheckbox.setPosition({ checkboxX, startY });
    m_autoRestartCheckbox.setFillColor(s_autoRestart ? sf::Color::Green : sf::Color::Red);

    // Hide Attempts
    m_hideAttemptsLabel.setPosition({ labelX, startY + spacing });
    m_hideAttemptsLabel.setFillColor(sf::Color::White);

    m_hideAttemptsCheckbox.setSize({ checkboxSize, checkboxSize });
    m_hideAttemptsCheckbox.setOutlineThickness(2.f);
    m_hideAttemptsCheckbox.setOutlineColor(sf::Color::White);
    m_hideAttemptsCheckbox.setPosition({ checkboxX, startY + spacing });
    m_hideAttemptsCheckbox.setFillColor(s_hideAttempts ? sf::Color::Green : sf::Color::Red);

    // Show Hitboxes
    m_showHitboxesLabel.setPosition({ labelX, startY + spacing * 2 });
    m_showHitboxesLabel.setFillColor(sf::Color::White);

    m_showHitboxesCheckbox.setSize({ checkboxSize, checkboxSize });
    m_showHitboxesCheckbox.setOutlineThickness(2.f);
    m_showHitboxesCheckbox.setOutlineColor(sf::Color::White);
    m_showHitboxesCheckbox.setPosition({ checkboxX, startY + spacing * 2 });
    m_showHitboxesCheckbox.setFillColor(s_showHitboxes ? sf::Color::Green : sf::Color::Red);
}

void SettingsScene::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    m_backButton.handleEvent(event, window);

    if (auto* mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouse->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(mouse->position);

            if (m_autoRestartCheckbox.getGlobalBounds().contains(mousePos)) {
                s_autoRestart = !s_autoRestart;
                m_autoRestartCheckbox.setFillColor(s_autoRestart ? sf::Color::Green : sf::Color::Red);
            }
            else if (m_hideAttemptsCheckbox.getGlobalBounds().contains(mousePos)) {
                s_hideAttempts = !s_hideAttempts;
                m_hideAttemptsCheckbox.setFillColor(s_hideAttempts ? sf::Color::Green : sf::Color::Red);
            }
            else if (m_showHitboxesCheckbox.getGlobalBounds().contains(mousePos)) {
                s_showHitboxes = !s_showHitboxes;
                m_showHitboxesCheckbox.setFillColor(s_showHitboxes ? sf::Color::Green : sf::Color::Red);
            }
        }
    }
}

void SettingsScene::update(sf::Time dt) {
    if (m_shouldClose && !m_isClosed) {
        m_isClosed = true;
        m_game.popScene();
        return;
    }

    sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(m_game.getWindow()));
    m_backButton.update(mousePos);
}

void SettingsScene::render(sf::RenderWindow& window) {
    window.draw(m_title);
    window.draw(m_autoRestartLabel);
    window.draw(m_autoRestartCheckbox);
    window.draw(m_hideAttemptsLabel);
    window.draw(m_hideAttemptsCheckbox);
    window.draw(m_showHitboxesLabel);
    window.draw(m_showHitboxesCheckbox);

    sf::Text autoRestartDesc(m_game.getFont(), "Instantly respawn at the start of the level", 18);
    autoRestartDesc.setPosition({ 300.f, 245.f });
    autoRestartDesc.setFillColor(sf::Color(180, 180, 180));

    sf::Text hideAttemptsDesc(m_game.getFont(), "Hide attempt counter (when implemented)", 18);
    hideAttemptsDesc.setPosition({ 300.f, 325.f });
    hideAttemptsDesc.setFillColor(sf::Color(180, 180, 180));

    sf::Text showHitboxesDesc(m_game.getFont(), "Show collision boxes for debugging", 18);
    showHitboxesDesc.setPosition({ 300.f, 405.f });
    showHitboxesDesc.setFillColor(sf::Color(180, 180, 180));

    window.draw(autoRestartDesc);
    window.draw(hideAttemptsDesc);
    window.draw(showHitboxesDesc);

    m_backButton.draw(window);
}