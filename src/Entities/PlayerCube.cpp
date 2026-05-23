#include "Entities/PlayerCube.h"
#include <cmath>
#include <algorithm>

PlayerCube::PlayerCube() {
    m_shape.setSize({ 50.f, 50.f });
    m_shape.setOrigin({ 25.f, 25.f });
    m_shape.setFillColor(sf::Color::Cyan);
}

void PlayerCube::handleInput(const sf::Event& event) {
    if (auto* key = event.getIf<sf::Event::KeyPressed>()) {
        if (key->code == sf::Keyboard::Key::Space) {
            if (m_onOrb) {
                jump();
            }
            else if (!m_isJumping) {
                jump();
                m_startRotation = m_shape.getRotation();
            }
        }
    }
    if (auto* mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouse->button == sf::Mouse::Button::Left) {
            if (m_onOrb) {
                jump();
            }
            else if (!m_isJumping) {
                jump();
                m_startRotation = m_shape.getRotation();
            }
        }
    }
}

void PlayerCube::update(sf::Time dt) {
    applyPhysics(dt);
    float dtSec = dt.asSeconds();
    m_velocity.y += m_gravity * dtSec;
    m_shape.move(m_velocity * dtSec);

    if (m_isJumping) {
        float progress = (m_velocity.y + 800.f) / 1600.f;
        progress = std::clamp(progress, 0.f, 1.f);
        m_shape.setRotation(m_startRotation + sf::degrees(progress * 90.f));
    }

    if (m_shape.getPosition().y >= 525.f) {
        m_shape.setPosition({ m_shape.getPosition().x, 525.f });
        m_velocity.y = 0.f;
        if (m_isJumping) {
            m_isJumping = false;
            float finalRotDeg = std::round((m_startRotation.asDegrees() + 90.f) / 90.f) * 90.f;
            m_shape.setRotation(sf::degrees(std::fmod(finalRotDeg, 360.f)));
        }
    }
}

void PlayerCube::draw(sf::RenderWindow& window) {
    sf::Vector2f center = m_shape.getPosition();
    sf::Angle angle = m_shape.getRotation();
    sf::Transform rot;
    rot.rotate(angle);

    sf::RectangleShape body({ 50.f, 50.f });
    body.setOrigin({ 25.f, 25.f });
    body.setPosition(center);
    body.setRotation(angle);
    body.setFillColor(m_primaryColor);
    body.setOutlineThickness(3);
    body.setOutlineColor(m_secondaryColor);

    if (m_glowEnabled) drawGlow(window, body);
    window.draw(body);

    // Паттерны
    if (m_cubePattern == 1) { // Stripes
        sf::RectangleShape stripe({ 50.f, 8.f });
        stripe.setFillColor(m_secondaryColor);
        stripe.setOrigin({ 25.f, 4.f });
        stripe.setPosition(center);
        stripe.setRotation(angle);
        window.draw(stripe);
        sf::Vector2f posUp = center + rot.transformPoint(sf::Vector2f(0.f, -12.f));
        sf::Vector2f posDown = center + rot.transformPoint(sf::Vector2f(0.f, 12.f));
        stripe.setPosition(posUp);
        window.draw(stripe);
        stripe.setPosition(posDown);
        window.draw(stripe);
    }
    else if (m_cubePattern == 2) { // Checker
        sf::RectangleShape sq({ 20.f, 20.f });
        sq.setFillColor(m_secondaryColor);
        sq.setOrigin({ 10.f, 10.f });
        sf::Vector2f pos1 = center + rot.transformPoint(sf::Vector2f(-10.f, -10.f));
        sf::Vector2f pos2 = center + rot.transformPoint(sf::Vector2f(10.f, 10.f));
        sq.setPosition(pos1);
        sq.setRotation(angle);
        window.draw(sq);
        sq.setPosition(pos2);
        window.draw(sq);
    }

    // Глаза
    sf::CircleShape eye(6.f);
    eye.setFillColor(sf::Color::White);
    eye.setOrigin({ 6.f, 6.f });
    sf::Vector2f eyePos1 = center + rot.transformPoint(sf::Vector2f(-12.f, -15.f));
    sf::Vector2f eyePos2 = center + rot.transformPoint(sf::Vector2f(12.f, -15.f));
    eye.setPosition(eyePos1);
    eye.setRotation(angle);
    window.draw(eye);
    eye.setPosition(eyePos2);
    window.draw(eye);

    sf::CircleShape pupil(3.f);
    pupil.setFillColor(sf::Color::Black);
    pupil.setOrigin({ 3.f, 3.f });
    sf::Vector2f pupilPos1 = center + rot.transformPoint(sf::Vector2f(-10.f, -15.f));
    sf::Vector2f pupilPos2 = center + rot.transformPoint(sf::Vector2f(14.f, -15.f));
    pupil.setPosition(pupilPos1);
    pupil.setRotation(angle);
    window.draw(pupil);
    pupil.setPosition(pupilPos2);
    window.draw(pupil);
}