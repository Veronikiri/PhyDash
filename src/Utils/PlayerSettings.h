#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "Entities/Player.h"
#include "Entities/PlayerCube.h"

class PlayerSettings {
public:
    static PlayerSettings& getInstance() {
        static PlayerSettings instance;
        return instance;
    }

    PlayerForm getForm() const { return m_form; }
    sf::Color getPrimaryColor() const { return m_primaryColor; }
    sf::Color getSecondaryColor() const { return m_secondaryColor; }
    sf::Color getGlowColor() const { return m_glowColor; }
    bool isGlowEnabled() const { return m_glowEnabled; }
    int getCubePattern() const { return m_cubePattern; }

    void setForm(PlayerForm form) { m_form = form; }
    void setPrimaryColor(sf::Color color) { m_primaryColor = color; }
    void setSecondaryColor(sf::Color color) { m_secondaryColor = color; }
    void setGlowColor(sf::Color color) { m_glowColor = color; }
    void setGlowEnabled(bool enabled) { m_glowEnabled = enabled; }
    void setCubePattern(int pattern) { m_cubePattern = pattern; }

    void applyToPlayer(Player* player) const {
        if (!player) return;
        player->setColors(m_primaryColor, m_secondaryColor, m_glowColor, m_glowEnabled);

        if (m_form == PlayerForm::Cube) {
            auto* cube = dynamic_cast<PlayerCube*>(player);
            if (cube) cube->setPattern(m_cubePattern);
        }
    }

    std::unique_ptr<Player> createPlayer() const {
        auto player = Player::createForm(m_form);
        applyToPlayer(player.get());
        return player;
    }

private:
    PlayerSettings() = default;

    PlayerForm m_form = PlayerForm::Cube;
    sf::Color m_primaryColor = sf::Color::Cyan;
    sf::Color m_secondaryColor = sf::Color::White;
    sf::Color m_glowColor = sf::Color::Cyan;
    bool m_glowEnabled = true;
    int m_cubePattern = 0;
};