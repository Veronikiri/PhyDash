#include "Entities/PlayerSpider.h"

PlayerSpider::PlayerSpider() {
    m_shape.setSize({ 50.f, 50.f });
    m_shape.setOrigin({ 25.f, 25.f });
    m_shape.setFillColor(sf::Color::Cyan);
    m_shape.setPosition({ 200.f, 525.f });
}

void PlayerSpider::handleInput(const sf::Event& event) {
    if (auto* key = event.getIf<sf::Event::KeyPressed>()) {
        if (key->code == sf::Keyboard::Key::Space && !m_isJumping) {
            jump();
        }
    }
}

void PlayerSpider::update(sf::Time dt) {
    applyPhysics(dt);
}

void PlayerSpider::draw(sf::RenderWindow& window) {
    sf::CircleShape body(20.f);
    body.setOrigin({ 20.f, 20.f });
    body.setPosition(m_shape.getPosition());
    body.setFillColor(m_primaryColor);
    body.setOutlineThickness(2);
    body.setOutlineColor(m_secondaryColor);

    if (m_glowEnabled) drawGlow(window, body);
    window.draw(body);

    // Ноги
    sf::RectangleShape leg({ 20.f, 4.f });
    leg.setFillColor(m_secondaryColor);
    leg.setOrigin({ 0.f, 2.f });

    leg.setPosition(m_shape.getPosition() + sf::Vector2f(-10.f, -10.f));
    leg.setRotation(sf::degrees(-30.f));
    window.draw(leg);
    leg.setPosition(m_shape.getPosition() + sf::Vector2f(-10.f, 10.f));
    leg.setRotation(sf::degrees(30.f));
    window.draw(leg);
    leg.setPosition(m_shape.getPosition() + sf::Vector2f(10.f, -10.f));
    leg.setRotation(sf::degrees(30.f));
    window.draw(leg);
    leg.setPosition(m_shape.getPosition() + sf::Vector2f(10.f, 10.f));
    leg.setRotation(sf::degrees(-30.f));
    window.draw(leg);

    // Глаза
    sf::CircleShape eye(4.f);
    eye.setFillColor(sf::Color::White);
    eye.setOrigin({ 4.f, 4.f });
    eye.setPosition(m_shape.getPosition() + sf::Vector2f(-6.f, -6.f));
    window.draw(eye);
    eye.setPosition(m_shape.getPosition() + sf::Vector2f(6.f, -6.f));
    window.draw(eye);
}