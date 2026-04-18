#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <memory>
#include "Entities/Player.h"
#include "Entities/Obstacle.h"

enum class GameState { MENU, PLAYING, SETTINGS, CUSTOMIZE, MODES };

struct MenuButton {
    sf::RectangleShape box;
    sf::Text text;
    GameState target;

    MenuButton(const sf::Font& font, const std::string& str, GameState t)
        : text(font, str, 34), target(t)
    {
        box.setSize({ 400.f, 65.f });
        box.setOutlineThickness(2.f);
        box.setOutlineColor(sf::Color::Cyan);
        box.setFillColor(sf::Color(0, 255, 255, 20));
        box.setOrigin({ 200.f, 32.5f });
    }
};

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(sf::Time deltaTime);
    void render();
    void handleMouseHover(sf::Vector2f mousePos);
    void initMenu();
    void drawMenu();

    void initCustomize();
    void processEventsCustomize(const sf::Event& event);
    void updateCustomize();
    void drawCustomize();
    void updatePreviewIcon();
    void updateColorFromSlider(float mouseX);

    float m_score = 0.0f;

    std::unique_ptr<sf::Text> m_menuTitle;
    std::unique_ptr<sf::Text> m_neonShadow;
    std::unique_ptr<sf::Text> m_scoreText;
    std::unique_ptr<sf::Text> m_customizeTitle;
    std::unique_ptr<sf::Text> m_backButton;
    std::unique_ptr<sf::Text> m_glowToggleText;
    std::unique_ptr<sf::Text> m_rgbValueText;

    sf::RenderWindow m_window;
    sf::Font m_font;
    GameState m_state;

    Player m_player;
    std::vector<Obstacle> m_obstacles;
    std::vector<MenuButton> m_buttons;

    sf::RectangleShape m_previewBackground;
    std::vector<sf::Text> m_formButtons;
    std::vector<sf::Text> m_colorLabels;
    std::vector<sf::Text> m_patternButtons;
    sf::RectangleShape m_glowCheckbox;
    bool m_glowEnabled = true;

    int m_selectedCubePattern = 0;
    sf::RectangleShape m_colorPreview;
    std::vector<sf::RectangleShape> m_rgbSliders;
    std::vector<sf::RectangleShape> m_rgbKnobs;
    std::vector<sf::Text> m_rgbLabels;

    bool m_draggingSlider = false;
    int m_activeSlider = -1;
    sf::Color m_currentEditColor;

    PlayerForm m_selectedForm = PlayerForm::Cube;
    sf::Color m_selectedColor1 = sf::Color::Cyan;
    sf::Color m_selectedColor2 = sf::Color::White;
    sf::Color m_selectedGlowColor = sf::Color::Cyan;

    std::vector<sf::Color> m_colorPresets;
    int m_selectedColorSlot = 0;
};