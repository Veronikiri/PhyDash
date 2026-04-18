#include "Core/Game.h"
#include "Entities/Player.h"
#include "Entities/Obstacle.h"
#include <iostream>
#include <sstream>
#include <iomanip>

Game::Game()
    : m_window(sf::VideoMode({ 1280, 720 }), "Phy Dash"),
    m_state(GameState::MENU),
    m_score(0.0f)
{
    m_window.setFramerateLimit(60);

    if (!m_font.openFromFile("FZeroGbaText1.ttf")) {
        if (!m_font.openFromFile("assets/fonts/FZeroGbaText1.ttf")) {
            std::cerr << "FATAL: Font not found!" << std::endl;
            exit(1);
        }
    }

    m_menuTitle = std::make_unique<sf::Text>(m_font, "Phy DASH", 110);
    m_neonShadow = std::make_unique<sf::Text>(m_font, "Phy DASH", 110);
    m_scoreText = std::make_unique<sf::Text>(m_font, "", 35);
    m_customizeTitle = std::make_unique<sf::Text>(m_font, "CUSTOMIZE CHARACTER", 56);
    m_backButton = std::make_unique<sf::Text>(m_font, "BACK", 40);
    m_glowToggleText = std::make_unique<sf::Text>(m_font, "Glow", 30);
    m_rgbValueText = std::make_unique<sf::Text>(m_font, "", 20);

    m_obstacles.reserve(10);
    for (int i = 1; i <= 10; ++i) {
        m_obstacles.emplace_back(static_cast<float>(i) * 800.0f, 500.0f);
    }

    initMenu();
    initCustomize();

    m_player.setForm(m_selectedForm);
    m_player.setColors(m_selectedColor1, m_selectedColor2, m_selectedGlowColor, m_glowEnabled);
    m_player.setCubePattern(m_selectedCubePattern);
}

void Game::initMenu() {
    m_menuTitle->setFillColor(sf::Color::Cyan);
    m_menuTitle->setOutlineThickness(2.0f);
    m_menuTitle->setOutlineColor(sf::Color::White);

    sf::FloatRect tRect = m_menuTitle->getLocalBounds();
    m_menuTitle->setOrigin({ tRect.position.x + tRect.size.x / 2.f,
                            tRect.position.y + tRect.size.y / 2.f });
    m_menuTitle->setPosition({ 640.f, 130.f });

    m_neonShadow->setFillColor(sf::Color(255, 0, 255, 40));
    m_neonShadow->setOutlineColor(sf::Color(255, 0, 255, 80));
    m_neonShadow->setOutlineThickness(8.0f);
    m_neonShadow->setOrigin(m_menuTitle->getOrigin());
    m_neonShadow->setPosition(m_menuTitle->getPosition());

    m_scoreText->setFillColor(sf::Color::White);
    m_scoreText->setOutlineThickness(1.0f);
    m_scoreText->setOutlineColor(sf::Color::Cyan);

    std::vector<std::pair<std::string, GameState>> config = {
       {"START", GameState::PLAYING},
       {"MODES", GameState::MODES},
       {"SETTINGS", GameState::SETTINGS},
       {"CUSTOMIZE", GameState::CUSTOMIZE}
    };

    m_buttons.clear();
    for (size_t i = 0; i < config.size(); ++i) {
        m_buttons.emplace_back(m_font, config[i].first, config[i].second);
        auto& btn = m_buttons.back();

        btn.box.setPosition({ 640.f, 280.f + static_cast<float>(i) * 95.f });

        sf::FloatRect b = btn.text.getLocalBounds();
        btn.text.setOrigin({ b.position.x + b.size.x / 2.f,
                            b.position.y + b.size.y / 2.f });
        btn.text.setPosition(btn.box.getPosition());
    }
}
void Game::initCustomize() {
    m_customizeTitle->setFillColor(sf::Color::Cyan);
    m_customizeTitle->setOutlineThickness(2);
    m_customizeTitle->setOutlineColor(sf::Color::White);
    sf::FloatRect tr = m_customizeTitle->getLocalBounds();
    m_customizeTitle->setOrigin({ tr.position.x + tr.size.x / 2.f,
                                 tr.position.y + tr.size.y / 2.f });
    m_customizeTitle->setPosition({ 640.f, 40.f });

    m_previewBackground.setSize({ 300.f, 300.f });
    m_previewBackground.setFillColor(sf::Color(20, 20, 30));
    m_previewBackground.setOutlineThickness(3);
    m_previewBackground.setOutlineColor(sf::Color::Cyan);
    m_previewBackground.setPosition({ 100.f, 150.f });

    std::vector<std::string> formNames = { "Cube", "Ship", "Ball", "UFO", "Wave", "Robot", "Spider" };
    float formStartX = 600;
    float formStartY = 160;
    float formSpacingX = 400;
    float formSpacingY = 60;

    for (size_t i = 0; i < formNames.size(); ++i) {
        sf::Text btn(m_font, formNames[i], 30);
        btn.setFillColor(sf::Color::White);
        btn.setOutlineThickness(1);
        btn.setOutlineColor(sf::Color::Cyan);
        sf::FloatRect b = btn.getLocalBounds();
        btn.setOrigin({ b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f });
        int col = i % 2;
        int row = i / 2;
        btn.setPosition({ formStartX + col * formSpacingX, formStartY + row * formSpacingY });
        m_formButtons.push_back(btn);
    }

    std::vector<std::string> patternNames = { "Plain", "Stripes", "Checker" };
    float patStartX = 520;
    float patStartY = 440;
    float patSpacing = 260;
    for (size_t i = 0; i < patternNames.size(); ++i) {
        sf::Text btn(m_font, patternNames[i], 26);
        btn.setFillColor(sf::Color::White);
        btn.setOutlineThickness(1);
        btn.setOutlineColor(sf::Color::Cyan);
        sf::FloatRect b = btn.getLocalBounds();
        btn.setOrigin({ b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f });
        btn.setPosition({ patStartX + i * patSpacing, patStartY });
        m_patternButtons.push_back(btn);
    }

    m_backButton->setFillColor(sf::Color::White);
    m_backButton->setOutlineThickness(2);
    m_backButton->setOutlineColor(sf::Color::Cyan);
    sf::FloatRect bb = m_backButton->getLocalBounds();
    m_backButton->setOrigin({ bb.position.x + bb.size.x / 2.f, bb.position.y + bb.size.y / 2.f });
    m_backButton->setPosition({ 120.f, 660.f });

    m_glowToggleText->setFillColor(sf::Color::White);
    m_glowToggleText->setPosition({ 480.f, 520.f });

    m_glowCheckbox.setSize({ 30.f, 30.f });
    m_glowCheckbox.setFillColor(m_glowEnabled ? sf::Color::Green : sf::Color::Red);
    m_glowCheckbox.setOutlineThickness(2);
    m_glowCheckbox.setOutlineColor(sf::Color::White);
    m_glowCheckbox.setPosition({ 660.f, 520.f });

    std::vector<std::string> slotNames = { "Primary", "Secondary", "Glow" };
    float labelStartX = 480;
    float labelStartY = 580;
    float labelSpacing = 300;
    for (int i = 0; i < 3; ++i) {
        sf::Text label(m_font, slotNames[i], 26);
        label.setFillColor(sf::Color::White);
        label.setPosition({ labelStartX + i * labelSpacing, labelStartY });
        m_colorLabels.push_back(label);
    }

    float rgbStartX = 480;
    float rgbStartY = 630;
    float sliderWidth = 180;
    float sliderHeight = 20;
    float sliderSpacing = 200;

    for (int i = 0; i < 3; ++i) {
        sf::RectangleShape slider({ sliderWidth, sliderHeight });
        slider.setFillColor(sf::Color(80, 80, 80));
        slider.setOutlineThickness(1);
        slider.setOutlineColor(sf::Color::White);
        slider.setPosition({ rgbStartX + i * sliderSpacing, rgbStartY });
        m_rgbSliders.push_back(slider);

        sf::RectangleShape knob({ 10, sliderHeight + 6 });
        knob.setFillColor(sf::Color::White);
        knob.setOutlineThickness(1);
        knob.setOutlineColor(sf::Color::Black);
        knob.setOrigin({ 5.f, (sliderHeight + 6) / 2.f });
        knob.setPosition({ rgbStartX + i * sliderSpacing, rgbStartY + sliderHeight / 2.f });
        m_rgbKnobs.push_back(knob);

        sf::Text label(m_font, std::string(1, "RGB"[i]), 22);
        label.setFillColor(sf::Color::White);
        label.setPosition({ rgbStartX + i * sliderSpacing - 35.f, rgbStartY - 5.f });
        m_rgbLabels.push_back(label);
    }

    m_rgbValueText->setFillColor(sf::Color::White);
    m_rgbValueText->setPosition({ rgbStartX, rgbStartY + 50.f });

    m_colorPreview.setSize({ 50.f, 50.f });
    m_colorPreview.setOutlineThickness(2);
    m_colorPreview.setOutlineColor(sf::Color::White);
    m_colorPreview.setPosition({ rgbStartX + 3 * sliderSpacing + 30.f, rgbStartY - 10.f });
}

void Game::updatePreviewIcon() {
    m_player.setForm(m_selectedForm);
    m_player.setColors(m_selectedColor1, m_selectedColor2, m_selectedGlowColor, m_glowEnabled);
    m_player.setCubePattern(m_selectedCubePattern);
    m_player.setPosition({ 150.f, 150.f });
}

void Game::updateColorFromSlider(float mouseX) {
    if (m_activeSlider < 0 || m_activeSlider > 2) return;

    const auto& slider = m_rgbSliders[m_activeSlider];
    float left = slider.getPosition().x;
    float right = left + slider.getSize().x;
    float t = (mouseX - left) / (right - left);
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    uint8_t value = static_cast<uint8_t>(t * 255);

    switch (m_selectedColorSlot) {
    case 0:
        if (m_activeSlider == 0) m_selectedColor1.r = value;
        else if (m_activeSlider == 1) m_selectedColor1.g = value;
        else m_selectedColor1.b = value;
        break;
    case 1:
        if (m_activeSlider == 0) m_selectedColor2.r = value;
        else if (m_activeSlider == 1) m_selectedColor2.g = value;
        else m_selectedColor2.b = value;
        break;
    case 2:
        if (m_activeSlider == 0) m_selectedGlowColor.r = value;
        else if (m_activeSlider == 1) m_selectedGlowColor.g = value;
        else m_selectedGlowColor.b = value;
        break;
    }

    updatePreviewIcon();
}

void Game::processEventsCustomize(const sf::Event& event) {
    if (auto* mouseButton = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseButton->button != sf::Mouse::Button::Left) return;
        sf::Vector2f mousePos = m_window.mapPixelToCoords(mouseButton->position);

        if (m_backButton->getGlobalBounds().contains(mousePos)) {
            m_state = GameState::MENU;
            m_player.setForm(m_selectedForm);
            m_player.setColors(m_selectedColor1, m_selectedColor2, m_selectedGlowColor, m_glowEnabled);
            m_player.setCubePattern(m_selectedCubePattern);
            return;
        }

        for (size_t i = 0; i < m_formButtons.size(); ++i) {
            if (m_formButtons[i].getGlobalBounds().contains(mousePos)) {
                m_selectedForm = static_cast<PlayerForm>(i);
                updatePreviewIcon();
                break;
            }
        }

        if (m_selectedForm == PlayerForm::Cube) {
            for (size_t i = 0; i < m_patternButtons.size(); ++i) {
                if (m_patternButtons[i].getGlobalBounds().contains(mousePos)) {
                    m_selectedCubePattern = i;
                    updatePreviewIcon();
                    break;
                }
            }
        }

        if (m_glowCheckbox.getGlobalBounds().contains(mousePos)) {
            m_glowEnabled = !m_glowEnabled;
            m_glowCheckbox.setFillColor(m_glowEnabled ? sf::Color::Green : sf::Color::Red);
            updatePreviewIcon();
        }

        for (int i = 0; i < 3; ++i) {
            if (m_colorLabels[i].getGlobalBounds().contains(mousePos)) {
                m_selectedColorSlot = i;
                break;
            }
        }

        for (int i = 0; i < 3; ++i) {
            if (m_rgbSliders[i].getGlobalBounds().contains(mousePos)) {
                m_draggingSlider = true;
                m_activeSlider = i;
                updateColorFromSlider(mousePos.x);
                break;
            }
        }
    }
    else if (event.is<sf::Event::MouseButtonReleased>()) {
        m_draggingSlider = false;
        m_activeSlider = -1;
    }
    else if (auto* mouseMove = event.getIf<sf::Event::MouseMoved>()) {
        if (m_draggingSlider && m_activeSlider != -1) {
            sf::Vector2f mousePos = m_window.mapPixelToCoords(mouseMove->position);
            updateColorFromSlider(mousePos.x);
        }
    }
}

void Game::updateCustomize() {
    for (size_t i = 0; i < m_formButtons.size(); ++i) {
        if (static_cast<PlayerForm>(i) == m_selectedForm) {
            m_formButtons[i].setFillColor(sf::Color::Cyan);
            m_formButtons[i].setOutlineColor(sf::Color::Magenta);
        }
        else {
            m_formButtons[i].setFillColor(sf::Color::White);
            m_formButtons[i].setOutlineColor(sf::Color::Cyan);
        }
    }

    bool isCube = (m_selectedForm == PlayerForm::Cube);
    for (auto& btn : m_patternButtons) {
        btn.setFillColor(isCube ? sf::Color::White : sf::Color::Transparent);
        btn.setOutlineColor(isCube ? sf::Color::Cyan : sf::Color::Transparent);
    }
    if (isCube) {
        for (size_t i = 0; i < m_patternButtons.size(); ++i) {
            if (i == m_selectedCubePattern) {
                m_patternButtons[i].setFillColor(sf::Color::Cyan);
                m_patternButtons[i].setOutlineColor(sf::Color::Magenta);
            }
            else {
                m_patternButtons[i].setFillColor(sf::Color::White);
                m_patternButtons[i].setOutlineColor(sf::Color::Cyan);
            }
        }
    }

    for (int i = 0; i < 3; ++i) {
        m_colorLabels[i].setFillColor(i == m_selectedColorSlot ? sf::Color::Yellow : sf::Color::White);
    }

    m_currentEditColor = (m_selectedColorSlot == 0) ? m_selectedColor1 :
        (m_selectedColorSlot == 1) ? m_selectedColor2 : m_selectedGlowColor;

    for (int i = 0; i < 3; ++i) {
        float value = (i == 0) ? m_currentEditColor.r :
            (i == 1) ? m_currentEditColor.g : m_currentEditColor.b;
        float ratio = value / 255.0f;
        float knobX = m_rgbSliders[i].getPosition().x + ratio * m_rgbSliders[i].getSize().x;
        m_rgbKnobs[i].setPosition({ knobX, m_rgbKnobs[i].getPosition().y });

        sf::Color sliderColor = (i == 0) ? sf::Color::Red :
            (i == 1) ? sf::Color::Green : sf::Color::Blue;
        sliderColor.a = 100;
        m_rgbSliders[i].setFillColor(sliderColor);
    }

    std::stringstream ss;
    ss << static_cast<int>(m_currentEditColor.r) << ", "
        << static_cast<int>(m_currentEditColor.g) << ", "
        << static_cast<int>(m_currentEditColor.b);
    m_rgbValueText->setString(ss.str());

    m_colorPreview.setFillColor(m_currentEditColor);
    m_glowCheckbox.setFillColor(m_glowEnabled ? sf::Color::Green : sf::Color::Red);
}

void Game::drawCustomize() {
    m_window.setView(m_window.getDefaultView());
    m_window.draw(*m_customizeTitle);
    m_window.draw(m_previewBackground);

    sf::View oldView = m_window.getView();
    sf::View previewView({ 150.f, 150.f }, { 300.f, 300.f });
    previewView.setViewport(sf::FloatRect({ 100.f / 1280.f, 150.f / 720.f }, { 300.f / 1280.f, 300.f / 720.f }));
    m_window.setView(previewView);

    sf::Vector2f oldPos = m_player.getPosition();
    m_player.setPosition({ 150.f, 150.f });
    m_player.draw(m_window);
    m_player.setPosition(oldPos);

    m_window.setView(oldView);

    for (auto& btn : m_formButtons) m_window.draw(btn);
    for (auto& btn : m_patternButtons) {
        if (m_selectedForm == PlayerForm::Cube || btn.getFillColor() != sf::Color::Transparent)
            m_window.draw(btn);
    }
    for (auto& label : m_colorLabels) m_window.draw(label);
    for (auto& slider : m_rgbSliders) m_window.draw(slider);
    for (auto& knob : m_rgbKnobs) m_window.draw(knob);
    for (auto& label : m_rgbLabels) m_window.draw(label);
    m_window.draw(*m_rgbValueText);
    m_window.draw(m_colorPreview);
    m_window.draw(*m_glowToggleText);
    m_window.draw(m_glowCheckbox);
    m_window.draw(*m_backButton);
}

void Game::processEvents() {
    while (auto event = m_window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            m_window.close();

        if (m_state == GameState::MENU) {
            if (auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButton->button == sf::Mouse::Button::Left) {
                    sf::Vector2f mousePos = m_window.mapPixelToCoords(mouseButton->position);
                    for (auto& btn : m_buttons) {
                        if (btn.box.getGlobalBounds().contains(mousePos)) {
                            m_state = btn.target;
                            if (m_state == GameState::CUSTOMIZE) {
                                m_selectedForm = m_player.getForm();
                                m_selectedColor1 = m_player.getPrimaryColor();
                                m_selectedColor2 = m_player.getSecondaryColor();
                                m_selectedGlowColor = m_player.getGlowColor();
                                m_glowEnabled = m_player.isGlowEnabled();
                                m_selectedCubePattern = m_player.getCubePattern();
                                updatePreviewIcon();
                            }
                        }
                    }
                }
            }
        }
        else if (m_state == GameState::PLAYING) {
            m_player.handleInput(*event);
        }
        else if (m_state == GameState::CUSTOMIZE) {
            processEventsCustomize(*event);
        }
    }
}

void Game::handleMouseHover(sf::Vector2f mousePos) {
    for (auto& btn : m_buttons) {
        if (btn.box.getGlobalBounds().contains(mousePos)) {
            btn.box.setOutlineColor(sf::Color::Magenta);
            btn.box.setFillColor(sf::Color(255, 0, 255, 45));
            btn.text.setFillColor(sf::Color::Cyan);
        }
        else {
            btn.box.setOutlineColor(sf::Color::Cyan);
            btn.box.setFillColor(sf::Color(0, 255, 255, 20));
            btn.text.setFillColor(sf::Color::White);
        }
    }
}

void Game::update(sf::Time deltaTime) {
    if (m_state == GameState::MENU) {
        handleMouseHover(m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window)));
    }
    else if (m_state == GameState::PLAYING) {
        m_player.update(deltaTime);
        m_score = m_player.getPosition().x / 100.0f;
        m_scoreText->setString("SCORE: " + std::to_string(static_cast<int>(m_score)));
        for (auto& obs : m_obstacles) {
            if (m_player.getBounds().findIntersection(obs.getBounds())) {
                m_player.resetPosition();
                m_score = 0.0f;
            }
        }
    }
    else if (m_state == GameState::CUSTOMIZE) {
        updateCustomize();
    }
}

void Game::drawMenu() {
    m_window.setView(m_window.getDefaultView());
    m_window.draw(*m_neonShadow);
    m_window.draw(*m_menuTitle);
    for (const auto& btn : m_buttons) {
        m_window.draw(btn.box);
        m_window.draw(btn.text);
    }
}

void Game::render() {
    m_window.clear(sf::Color(10, 10, 18));

    if (m_state == GameState::MENU) {
        drawMenu();
    }
    else if (m_state == GameState::PLAYING) {
        sf::View gameView({ m_player.getPosition().x + 400.f, 360.f }, { 1280.f, 720.f });
        m_window.setView(gameView);

        sf::RectangleShape floor({ 100000.f, 4.f });
        floor.setPosition({ 0.f, 550.f });
        floor.setFillColor(sf::Color(100, 100, 100));
        m_window.draw(floor);

        for (auto& obs : m_obstacles) obs.draw(m_window);
        m_player.draw(m_window);

        m_window.setView(m_window.getDefaultView());
        m_scoreText->setPosition({ 30.f, 30.f });
        m_window.draw(*m_scoreText);
    }
    else if (m_state == GameState::CUSTOMIZE) {
        drawCustomize();
    }

    m_window.display();
}

void Game::run() {
    sf::Clock clock;
    while (m_window.isOpen()) {
        processEvents();
        update(clock.restart());
        render();
    }
}