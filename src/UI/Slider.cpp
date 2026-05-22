#include "UI/Slider.h"
#include <algorithm>

Slider::Slider(sf::Vector2f position, float width, float height, sf::Color trackColor, sf::Color knobColor) {
    m_track.setSize({ width, height });
    m_track.setFillColor(trackColor);
    m_track.setOutlineThickness(1);
    m_track.setOutlineColor(sf::Color::White);
    m_track.setPosition(position);

    m_knob.setSize({ 10.f, height + 6.f });
    m_knob.setFillColor(knobColor);
    m_knob.setOrigin({ 5.f, (height + 6.f) / 2.f });
    m_knob.setPosition({ position.x + width / 2.f, position.y + height / 2.f });
}

void Slider::setValue(float value) {
    m_value = std::clamp(value, 0.f, 1.f);
    float knobX = m_track.getPosition().x + m_value * m_track.getSize().x;
    m_knob.setPosition({ knobX, m_knob.getPosition().y });
}

float Slider::getValue() const { return m_value; }

void Slider::setCallback(std::function<void(float)> callback) { m_callback = callback; }

void Slider::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (auto* mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouse->button == sf::Mouse::Button::Left && m_track.getGlobalBounds().contains(window.mapPixelToCoords(mouse->position))) {
            m_dragging = true;
        }
    }
    else if (event.is<sf::Event::MouseButtonReleased>()) {
        m_dragging = false;
    }
    else if (auto* move = event.getIf<sf::Event::MouseMoved>()) {
        if (m_dragging) {
            float mouseX = window.mapPixelToCoords(move->position).x;
            float left = m_track.getPosition().x;
            float right = left + m_track.getSize().x;
            float t = (mouseX - left) / (right - left);
            t = std::clamp(t, 0.f, 1.f);
            setValue(t);
            if (m_callback) m_callback(m_value);
        }
    }
}

void Slider::update() {}

void Slider::draw(sf::RenderWindow& window) const {
    window.draw(m_track);
    window.draw(m_knob);
}