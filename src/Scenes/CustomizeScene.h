#pragma once
#include "Core/Scene.h"
#include "UI/Button.h"
#include "UI/Slider.h"
#include "Entities/Player.h"
#include <vector>
#include <memory>
#include <UI/BackButton.h>

class Game;

class CustomizeScene : public Scene {
public:
    CustomizeScene(Game& game);
    void handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    void update(sf::Time dt) override;
    void render(sf::RenderWindow& window) override;
    void onEnter() override;

private:
    void saveSettings();
    void updatePreview();
    void updateColorSlidersFromCurrentColor();
    void updateCurrentEditColor();
    void updateUiState();

    Game& m_game;
    std::unique_ptr<Player> m_previewPlayer;
    sf::Text m_title;
    BackButton m_backButton;
    std::vector<Button> m_formButtons;
    std::vector<Button> m_patternButtons;
    sf::RectangleShape m_previewBg;
    sf::RectangleShape m_glowCheckbox;
    sf::Text m_glowLabel;
    std::vector<sf::Text> m_colorLabels;
    std::vector<Slider> m_rgbSliders;
    std::vector<sf::Text> m_rgbLabelTexts;
    sf::Text m_rgbValueText;
    sf::RectangleShape m_colorPreview;

    PlayerForm m_selectedForm = PlayerForm::Cube;
    sf::Color m_selectedColor1 = sf::Color::Cyan;
    sf::Color m_selectedColor2 = sf::Color::White;
    sf::Color m_selectedGlowColor = sf::Color::Cyan;
    bool m_glowEnabled = true;
    int m_selectedPattern = 0;
    int m_selectedColorSlot = 0;
    sf::Color m_currentEditColor;

    bool m_shouldClose = false;
    bool m_isClosed = false;
};