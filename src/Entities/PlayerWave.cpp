#include "Entities/PlayerWave.h"

PlayerWave::PlayerWave() {
    m_shape.setSize({ 50.f, 50.f });
    m_shape.setOrigin({ 25.f, 25.f });
    m_shape.setFillColor(sf::Color::Cyan);
    m_shape.setPosition({ 200.f, 525.f });
}

void PlayerWave::handleInput(const sf::Event& event) {
    if (auto* key = event.getIf<sf::Event::KeyPressed>()) {
        if (key->code == sf::Keyboard::Key::Space && !m_isJumping) {
            jump();
        }
    }
}

void PlayerWave::update(sf::Time dt) {
    applyPhysics(dt);
}

void PlayerWave::draw(sf::RenderWindow& window) {
    sf::ConvexShape wave(3);
    wave.setPoint(0, { 0.f, -30.f });
    wave.setPoint(1, { 30.f, 20.f });
    wave.setPoint(2, { -30.f, 20.f });
    wave.setOrigin({ 0.f, 0.f });
    wave.setPosition(m_shape.getPosition());
    wave.setRotation(m_shape.getRotation());
    wave.setFillColor(m_primaryColor);
    wave.setOutlineThickness(3);
    wave.setOutlineColor(m_secondaryColor);

    if (m_glowEnabled) drawGlow(window, wave);
    window.draw(wave);

    // Глаз
    sf::CircleShape eye(5.f);
    eye.setFillColor(sf::Color::Black);
    eye.setOrigin({ 5.f, 5.f });
    eye.setPosition(m_shape.getPosition() + sf::Vector2f(0.f, -10.f));
    window.draw(eye);
}