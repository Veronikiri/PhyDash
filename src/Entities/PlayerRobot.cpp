#include "Entities/PlayerRobot.h"

PlayerRobot::PlayerRobot() {
    m_shape.setSize({ 50.f, 50.f });
    m_shape.setOrigin({ 25.f, 25.f });
    m_shape.setFillColor(sf::Color::Cyan);
    m_shape.setPosition({ 200.f, 525.f });
}

void PlayerRobot::handleInput(const sf::Event& event) {
    if (auto* key = event.getIf<sf::Event::KeyPressed>()) {
        if (key->code == sf::Keyboard::Key::Space && !m_isJumping) {
            jump();
        }
    }
}

void PlayerRobot::update(sf::Time dt) {
    applyPhysics(dt);
}

void PlayerRobot::draw(sf::RenderWindow& window) {
    sf::RectangleShape head({ 30.f, 30.f });
    head.setOrigin({ 15.f, 15.f });
    head.setPosition(m_shape.getPosition() + sf::Vector2f(0.f, -20.f));
    head.setFillColor(m_primaryColor);
    head.setOutlineThickness(2);
    head.setOutlineColor(m_secondaryColor);

    sf::RectangleShape body({ 40.f, 40.f });
    body.setOrigin({ 20.f, 20.f });
    body.setPosition(m_shape.getPosition());
    body.setFillColor(m_primaryColor);
    body.setOutlineThickness(2);
    body.setOutlineColor(m_secondaryColor);

    if (m_glowEnabled) {
        drawGlow(window, head);
        drawGlow(window, body);
    }
    window.draw(body);
    window.draw(head);

    // Глаза
    sf::CircleShape eye(5.f);
    eye.setFillColor(sf::Color::White);
    eye.setOrigin({ 5.f, 5.f });
    eye.setPosition(m_shape.getPosition() + sf::Vector2f(-8.f, -20.f));
    window.draw(eye);
    eye.setPosition(m_shape.getPosition() + sf::Vector2f(8.f, -20.f));
    window.draw(eye);
}