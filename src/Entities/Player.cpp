#include "Entities/Player.h"
#include <cmath>

Player::Player() : m_isJumping(false), m_gravity(2000.f), m_startRotation(sf::Angle::Zero),
m_form(PlayerForm::Cube),
m_primaryColor(sf::Color::Cyan),
m_secondaryColor(sf::Color::White),
m_glowColor(sf::Color::Cyan),
m_glowEnabled(true),
m_cubePattern(0)
{
    m_shape.setSize({ 50.f, 50.f });
    m_shape.setOrigin({ 25.f, 25.f });
    m_shape.setFillColor(sf::Color::Cyan);
    m_shape.setPosition({ 200.f, 525.f });
}

void Player::handleInput(const sf::Event& event) {
    if (auto* key = event.getIf<sf::Event::KeyPressed>()) {
        if (key->code == sf::Keyboard::Key::Space) {
            if (!m_isJumping) {
                m_velocity.y = -800.f;
                m_isJumping = true;
                m_startRotation = m_shape.getRotation();
            }
        }
    }
}

void Player::update(sf::Time deltaTime) {
    float dt = deltaTime.asSeconds();
    m_velocity.x = 450.f;
    m_velocity.y += m_gravity * dt;
    m_shape.move(m_velocity * dt);

    if (m_isJumping) {
        float progress = (m_velocity.y + 800.f) / 1600.f;
        if (progress < 0.f) progress = 0.f;
        if (progress > 1.f) progress = 1.f;
        m_shape.setRotation(m_startRotation + sf::degrees(progress * 90.f));
    }

    if (m_shape.getPosition().y >= 525.f) {
        m_shape.setPosition({ m_shape.getPosition().x, 525.f });
        m_velocity.y = 0.f;
        if (m_isJumping) {
            m_isJumping = false;
            float finalRotDeg = std::round((m_startRotation.asDegrees() + 90.f) / 90.f) * 90.f;
            m_shape.setRotation(sf::degrees(static_cast<float>(static_cast<int>(finalRotDeg) % 360)));
        }
    }
}

void Player::setColors(sf::Color primary, sf::Color secondary, sf::Color glow, bool glowEnabled) {
    m_primaryColor = primary;
    m_secondaryColor = secondary;
    m_glowColor = glow;
    m_glowEnabled = glowEnabled;
}

void Player::draw(sf::RenderWindow& window) {
    switch (m_form) {
    case PlayerForm::Cube:   drawCube(window); break;
    case PlayerForm::Ship:   drawShip(window); break;
    case PlayerForm::Ball:   drawBall(window); break;
    case PlayerForm::Ufo:    drawUfo(window);  break;
    case PlayerForm::Wave:   drawWave(window); break;
    case PlayerForm::Robot:  drawRobot(window); break;
    case PlayerForm::Spider: drawSpider(window); break;
    }
}

void drawGlow(sf::RenderWindow& window, const sf::Shape& shape, sf::Color glowColor) {
    auto cloneAndDraw = [&](auto* derived) {
        auto copy = std::unique_ptr<std::decay_t<decltype(*derived)>>(new auto(*derived));
        copy->setFillColor(sf::Color(glowColor.r, glowColor.g, glowColor.b, 80));
        copy->setOutlineThickness(6);
        copy->setOutlineColor(sf::Color(glowColor.r, glowColor.g, glowColor.b, 40));
        window.draw(*copy);
        };

    if (auto* rect = dynamic_cast<const sf::RectangleShape*>(&shape)) {
        cloneAndDraw(rect);
    }
    else if (auto* circ = dynamic_cast<const sf::CircleShape*>(&shape)) {
        cloneAndDraw(circ);
    }
    else if (auto* convex = dynamic_cast<const sf::ConvexShape*>(&shape)) {
        cloneAndDraw(convex);
    }
}

void Player::drawCube(sf::RenderWindow& window) {
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

    if (m_glowEnabled) drawGlow(window, body, m_glowColor);
    window.draw(body);

    if (m_cubePattern == 1) {
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
    else if (m_cubePattern == 2) {
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

void Player::drawShip(sf::RenderWindow& window) {
    sf::ConvexShape ship(4);
    ship.setPoint(0, { 0.f, -30.f });
    ship.setPoint(1, { 25.f, 20.f });
    ship.setPoint(2, { 0.f, 10.f });
    ship.setPoint(3, { -25.f, 20.f });
    ship.setOrigin({ 0.f, 0.f });
    ship.setPosition(m_shape.getPosition());
    ship.setRotation(m_shape.getRotation());
    ship.setFillColor(m_primaryColor);
    ship.setOutlineThickness(3);
    ship.setOutlineColor(m_secondaryColor);

    if (m_glowEnabled) drawGlow(window, ship, m_glowColor);
    window.draw(ship);

    sf::CircleShape cockpit(8.f);
    cockpit.setFillColor(m_secondaryColor);
    cockpit.setOrigin({ 8.f, 8.f });
    cockpit.setPosition(m_shape.getPosition() + sf::Vector2f(0.f, -15.f));
    cockpit.setRotation(m_shape.getRotation());
    window.draw(cockpit);
}

void Player::drawBall(sf::RenderWindow& window) {
    sf::CircleShape ball(25.f);
    ball.setOrigin({ 25.f, 25.f });
    ball.setPosition(m_shape.getPosition());
    ball.setRotation(m_shape.getRotation());
    ball.setFillColor(m_primaryColor);
    ball.setOutlineThickness(3);
    ball.setOutlineColor(m_secondaryColor);

    if (m_glowEnabled) drawGlow(window, ball, m_glowColor);
    window.draw(ball);

    sf::RectangleShape stripe({ 30.f, 6.f });
    stripe.setFillColor(m_secondaryColor);
    stripe.setOrigin({ 15.f, 3.f });
    stripe.setPosition(m_shape.getPosition());
    stripe.setRotation(m_shape.getRotation());
    window.draw(stripe);
}

void Player::drawUfo(sf::RenderWindow& window) {
    sf::ConvexShape body(4);
    body.setPoint(0, { -30.f, 0.f });
    body.setPoint(1, { 30.f, 0.f });
    body.setPoint(2, { 20.f, 20.f });
    body.setPoint(3, { -20.f, 20.f });
    body.setOrigin({ 0.f, 10.f });
    body.setPosition(m_shape.getPosition());
    body.setRotation(m_shape.getRotation());
    body.setFillColor(m_primaryColor);
    body.setOutlineThickness(3);
    body.setOutlineColor(m_secondaryColor);

    if (m_glowEnabled) drawGlow(window, body, m_glowColor);
    window.draw(body);

    sf::CircleShape dome(12.f);
    dome.setFillColor(m_secondaryColor);
    dome.setOrigin({ 12.f, 12.f });
    dome.setPosition(m_shape.getPosition() + sf::Vector2f(0.f, -10.f));
    dome.setRotation(m_shape.getRotation());
    window.draw(dome);
}

void Player::drawWave(sf::RenderWindow& window) {
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

    if (m_glowEnabled) drawGlow(window, wave, m_glowColor);
    window.draw(wave);

    sf::CircleShape eye(5.f);
    eye.setFillColor(sf::Color::Black);
    eye.setOrigin({ 5.f, 5.f });
    eye.setPosition(m_shape.getPosition() + sf::Vector2f(0.f, -10.f));
    eye.setRotation(m_shape.getRotation());
    window.draw(eye);
}

void Player::drawRobot(sf::RenderWindow& window) {
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
        drawGlow(window, head, m_glowColor);
        drawGlow(window, body, m_glowColor);
    }
    window.draw(body);
    window.draw(head);

    sf::CircleShape eye(5.f);
    eye.setFillColor(sf::Color::White);
    eye.setOrigin({ 5.f, 5.f });
    eye.setPosition(m_shape.getPosition() + sf::Vector2f(-8.f, -20.f));
    eye.setRotation(m_shape.getRotation());
    window.draw(eye);
    eye.setPosition(m_shape.getPosition() + sf::Vector2f(8.f, -20.f));
    window.draw(eye);
}

void Player::drawSpider(sf::RenderWindow& window) {
    sf::CircleShape body(20.f);
    body.setOrigin({ 20.f, 20.f });
    body.setPosition(m_shape.getPosition());
    body.setFillColor(m_primaryColor);
    body.setOutlineThickness(2);
    body.setOutlineColor(m_secondaryColor);

    if (m_glowEnabled) drawGlow(window, body, m_glowColor);
    window.draw(body);

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

    sf::CircleShape eye(4.f);
    eye.setFillColor(sf::Color::White);
    eye.setOrigin({ 4.f, 4.f });
    eye.setPosition(m_shape.getPosition() + sf::Vector2f(-6.f, -6.f));
    eye.setRotation(m_shape.getRotation());
    window.draw(eye);
    eye.setPosition(m_shape.getPosition() + sf::Vector2f(6.f, -6.f));
    window.draw(eye);
}