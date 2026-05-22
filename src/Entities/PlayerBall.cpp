#include "Entities/PlayerBall.h"

PlayerBall::PlayerBall() {
    m_shape.setSize({ 50.f, 50.f });
    m_shape.setOrigin({ 25.f, 25.f });
    m_shape.setFillColor(sf::Color::Cyan);
    m_shape.setPosition({ 200.f, 525.f });
}

void PlayerBall::handleInput(const sf::Event& event) {
    if (auto* key = event.getIf<sf::Event::KeyPressed>()) {
        if (key->code == sf::Keyboard::Key::Space && !m_isJumping) jump();
    }
}

void PlayerBall::update(sf::Time dt) { applyPhysics(dt); }

void PlayerBall::draw(sf::RenderWindow& window) {
    sf::CircleShape ball(25.f);
    ball.setOrigin({ 25.f, 25.f });
    ball.setPosition(m_shape.getPosition());
    ball.setRotation(m_shape.getRotation());
    ball.setFillColor(m_primaryColor);
    ball.setOutlineThickness(3);
    ball.setOutlineColor(m_secondaryColor);
    if (m_glowEnabled) drawGlow(window, ball);
    window.draw(ball);

    sf::RectangleShape stripe({ 30.f, 6.f });
    stripe.setFillColor(m_secondaryColor);
    stripe.setOrigin({ 15.f, 3.f });
    stripe.setPosition(m_shape.getPosition());
    stripe.setRotation(m_shape.getRotation());
    window.draw(stripe);
}