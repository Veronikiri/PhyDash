#include "Scenes/CustomizeScene.h"
#include "Core/Game.h"
#include "Entities/PlayerCube.h"
#include "Entities/PlayerShip.h"
#include "Entities/PlayerBall.h"
#include "Entities/PlayerUfo.h"
#include "Entities/PlayerWave.h"
#include "Entities/PlayerRobot.h"
#include "Entities/PlayerSpider.h"
#include <sstream>

CustomizeScene::CustomizeScene(Game& game)
    : m_game(game)
    , m_title(game.getFont(), "CUSTOMIZE CHARACTER", 56)
    , m_backButton(game.getFont())
    , m_glowLabel(game.getFont(), "Glow", 24)
    , m_rgbValueText(game.getFont())
{
    m_title.setFillColor(sf::Color::Cyan);
    m_title.setOutlineThickness(2);
    sf::FloatRect tr = m_title.getLocalBounds();
    m_title.setOrigin({ tr.position.x + tr.size.x / 2.f, tr.position.y + tr.size.y / 2.f });
    m_title.setPosition({ 640.f, 40.f });

    m_previewBg.setSize({ 250.f, 250.f });
    m_previewBg.setFillColor(sf::Color(20, 20, 30));
    m_previewBg.setOutlineThickness(3);
    m_previewBg.setOutlineColor(sf::Color::Cyan);
    m_previewBg.setPosition({ 100.f, 150.f });

    // ========== КНОПКИ ФОРМ ==========
    std::vector<std::string> formNames = { "Cube","Ship","Ball","UFO","Wave","Robot","Spider" };
    float fx = 550, fy = 180, sx = 260, sy = 60;
    for (size_t i = 0; i < formNames.size(); ++i) {
        Button btn(game.getFont(), formNames[i], 22);
        int col = i % 2, row = i / 2;
        btn.setPosition({ fx + col * sx, fy + row * sy });
        btn.setColors(sf::Color::Transparent, sf::Color::Transparent, sf::Color::White, sf::Color::Cyan);
        btn.setCallback([this, form = i]() {
            m_selectedForm = static_cast<PlayerForm>(form);
            updatePreview();
            });
        m_formButtons.push_back(std::move(btn));
    }

    // ========== КНОПКИ ПАТТЕРНОВ ==========
    std::vector<std::string> patNames = { "Plain","Stripes","Checker" };
    float px = 480, py = 460, sp = 250;
    for (size_t i = 0; i < patNames.size(); ++i) {
        Button btn(game.getFont(), patNames[i], 18);
        btn.setPosition({ px + i * sp, py });
        btn.setColors(sf::Color::Transparent, sf::Color::Transparent, sf::Color::White, sf::Color::Cyan);
        btn.setCallback([this, idx = i]() {
            m_selectedPattern = idx;
            updatePreview();
            });
        m_patternButtons.push_back(std::move(btn));
    }

    // ========== КНОПКА BACK ==========
    m_backButton.setPosition({ 120.f, 680.f });
    m_backButton.setColors(sf::Color::Transparent, sf::Color::Transparent, sf::Color::White, sf::Color::Cyan);
    m_backButton.setCallback([this]() { m_shouldClose = true; });

    // ========== GLOW ЧЕКБОКС ==========
    m_glowLabel.setPosition({ 460.f, 540.f });
    m_glowCheckbox.setSize({ 25.f, 25.f });
    m_glowCheckbox.setOutlineThickness(2);
    m_glowCheckbox.setOutlineColor(sf::Color::White);
    m_glowCheckbox.setPosition({ 620.f, 540.f });
    m_glowCheckbox.setFillColor(m_glowEnabled ? sf::Color::Green : sf::Color::Red);

    // ========== МЕТКИ ЦВЕТОВЫХ СЛОТОВ ==========
    std::vector<std::string> slotNames = { "Primary", "Secondary", "Glow" };
    float labelStartX = 460, labelStartY = 590, labelSpacing = 210;
    for (int i = 0; i < 3; ++i) {
        sf::Text label(game.getFont(), slotNames[i], 18);
        label.setFillColor(sf::Color::White);
        label.setPosition({ labelStartX + i * labelSpacing, labelStartY });
        m_colorLabels.push_back(label);
    }

    // ========== СЛАЙДЕРЫ RGB ==========
    float rgbX = 460, rgbY = 630, w = 140, h = 12, sp2 = 180;
    std::vector<sf::Color> trackColors = { sf::Color::Red, sf::Color::Green, sf::Color::Blue };
    for (int i = 0; i < 3; ++i) {
        Slider slider({ rgbX + i * sp2, rgbY }, w, h, sf::Color(80, 80, 80), sf::Color::White);
        slider.setCallback([this, i](float val) {
            uint8_t v = static_cast<uint8_t>(val * 255);
            if (m_selectedColorSlot == 0) {
                if (i == 0) m_selectedColor1.r = v;
                else if (i == 1) m_selectedColor1.g = v;
                else m_selectedColor1.b = v;
            }
            else if (m_selectedColorSlot == 1) {
                if (i == 0) m_selectedColor2.r = v;
                else if (i == 1) m_selectedColor2.g = v;
                else m_selectedColor2.b = v;
            }
            else {
                if (i == 0) m_selectedGlowColor.r = v;
                else if (i == 1) m_selectedGlowColor.g = v;
                else m_selectedGlowColor.b = v;
            }
            updatePreview();
            updateCurrentEditColor();
            });
        m_rgbSliders.push_back(std::move(slider));

        sf::Text rgbLabel(game.getFont(), std::string(1, "RGB"[i]), 16);
        rgbLabel.setFillColor(trackColors[i]);
        rgbLabel.setPosition({ rgbX + i * sp2 - 25.f, rgbY - 5.f });
        m_rgbLabelTexts.push_back(rgbLabel);
    }

    // ========== ТЕКСТОВОЕ ЗНАЧЕНИЕ RGB ==========
    m_rgbValueText.setCharacterSize(14);
    m_rgbValueText.setFillColor(sf::Color::White);
    m_rgbValueText.setPosition({ rgbX, rgbY + 35.f });

    // ========== ПРЕВЬЮ ЦВЕТА ==========
    m_colorPreview.setSize({ 35.f, 35.f });
    m_colorPreview.setOutlineThickness(2);
    m_colorPreview.setOutlineColor(sf::Color::White);
    m_colorPreview.setPosition({ rgbX + 3 * sp2 + 40.f, rgbY - 5.f });

    updatePreview();
}

void CustomizeScene::onEnter() {
    updatePreview();
}

void CustomizeScene::updatePreview() {
    m_previewPlayer = Player::createForm(m_selectedForm);
    m_previewPlayer->setColors(m_selectedColor1, m_selectedColor2, m_selectedGlowColor, m_glowEnabled);
    if (m_selectedForm == PlayerForm::Cube) {
        auto* cube = dynamic_cast<PlayerCube*>(m_previewPlayer.get());
        if (cube) cube->setPattern(m_selectedPattern);
    }
    m_previewPlayer->setPosition({ 150.f, 150.f });
    updateColorSlidersFromCurrentColor();
}

void CustomizeScene::updateColorSlidersFromCurrentColor() {
    updateCurrentEditColor();
    float r = m_currentEditColor.r / 255.f;
    float g = m_currentEditColor.g / 255.f;
    float b = m_currentEditColor.b / 255.f;
    if (m_rgbSliders.size() >= 3) {
        m_rgbSliders[0].setValue(r);
        m_rgbSliders[1].setValue(g);
        m_rgbSliders[2].setValue(b);
    }
    std::stringstream ss;
    ss << (int)m_currentEditColor.r << ", " << (int)m_currentEditColor.g << ", " << (int)m_currentEditColor.b;
    m_rgbValueText.setString(ss.str());
    m_colorPreview.setFillColor(m_currentEditColor);
}

void CustomizeScene::updateCurrentEditColor() {
    switch (m_selectedColorSlot) {
    case 0: m_currentEditColor = m_selectedColor1; break;
    case 1: m_currentEditColor = m_selectedColor2; break;
    case 2: m_currentEditColor = m_selectedGlowColor; break;
    }
}

void CustomizeScene::updateUiState() {
    for (size_t i = 0; i < m_formButtons.size(); ++i) {
        if (static_cast<PlayerForm>(i) == m_selectedForm) {
            m_formButtons[i].setColors(sf::Color::Transparent, sf::Color::Transparent, sf::Color::Cyan, sf::Color::Magenta);
        }
        else {
            m_formButtons[i].setColors(sf::Color::Transparent, sf::Color::Transparent, sf::Color::White, sf::Color::Cyan);
        }
    }
    bool isCube = (m_selectedForm == PlayerForm::Cube);
    for (auto& btn : m_patternButtons) {
        if (isCube) {
            btn.setColors(sf::Color::Transparent, sf::Color::Transparent, sf::Color::White, sf::Color::Cyan);
        }
        else {
            btn.setColors(sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent);
        }
    }
    if (isCube) {
        for (size_t i = 0; i < m_patternButtons.size(); ++i) {
            if (i == m_selectedPattern) {
                m_patternButtons[i].setColors(sf::Color::Transparent, sf::Color::Transparent, sf::Color::Cyan, sf::Color::Magenta);
            }
            else {
                m_patternButtons[i].setColors(sf::Color::Transparent, sf::Color::Transparent, sf::Color::White, sf::Color::Cyan);
            }
        }
    }
    for (int i = 0; i < 3; ++i) {
        m_colorLabels[i].setFillColor(i == m_selectedColorSlot ? sf::Color::Yellow : sf::Color::White);
    }
    m_glowCheckbox.setFillColor(m_glowEnabled ? sf::Color::Green : sf::Color::Red);
    updateColorSlidersFromCurrentColor();
}

void CustomizeScene::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    for (auto& btn : m_formButtons)
        btn.handleEvent(event, window);
    for (auto& btn : m_patternButtons)
        if (m_selectedForm == PlayerForm::Cube)
            btn.handleEvent(event, window);
    m_backButton.handleEvent(event, window);
    for (auto& slider : m_rgbSliders)
        slider.handleEvent(event, window);

    if (auto* mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouse->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(mouse->position);
            if (m_glowCheckbox.getGlobalBounds().contains(mousePos)) {
                m_glowEnabled = !m_glowEnabled;
                updatePreview();
            }
            for (int i = 0; i < 3; ++i) {
                if (m_colorLabels[i].getGlobalBounds().contains(mousePos)) {
                    m_selectedColorSlot = i;
                    updateColorSlidersFromCurrentColor();
                    break;
                }
            }
        }
    }
}

void CustomizeScene::update(sf::Time dt) {
    if (m_shouldClose && !m_isClosed) {
        m_isClosed = true;
        m_game.popScene();
        return;
    }

    sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(m_game.getWindow()));
    for (auto& btn : m_formButtons)
        btn.update(mousePos);
    if (m_selectedForm == PlayerForm::Cube) {
        for (auto& btn : m_patternButtons)
            btn.update(mousePos);
    }
    m_backButton.update(mousePos);
    for (auto& slider : m_rgbSliders)
        slider.update();
    updateUiState();
}

void CustomizeScene::render(sf::RenderWindow& window) {
    window.draw(m_title);
    window.draw(m_previewBg);

    sf::View oldView = window.getView();
    sf::View previewView({ 150.f, 150.f }, { 250.f, 250.f });
    previewView.setViewport(sf::FloatRect({ 100.f / 1280.f, 150.f / 720.f }, { 250.f / 1280.f, 250.f / 720.f }));
    window.setView(previewView);
    sf::Vector2f oldPos = m_previewPlayer->getPosition();
    m_previewPlayer->setPosition({ 150.f, 150.f });
    m_previewPlayer->draw(window);
    m_previewPlayer->setPosition(oldPos);
    window.setView(oldView);

    for (auto& btn : m_formButtons) btn.draw(window);
    if (m_selectedForm == PlayerForm::Cube) {
        for (auto& btn : m_patternButtons) btn.draw(window);
    }
    for (auto& label : m_colorLabels) window.draw(label);
    for (auto& slider : m_rgbSliders) slider.draw(window);
    for (auto& label : m_rgbLabelTexts) window.draw(label);
    window.draw(m_rgbValueText);
    window.draw(m_colorPreview);
    window.draw(m_glowLabel);
    window.draw(m_glowCheckbox);
    m_backButton.draw(window);
}