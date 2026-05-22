#include "UI/Button.h"

Button::Button(const sf::Font& font, const std::string& txt, unsigned int size)
    : m_text(font, txt, size)
{
    m_box.setSize({ 400.f, 65.f });
    m_box.setOutlineThickness(2.f);
    m_box.setOutlineColor(sf::Color::Cyan);
    m_box.setFillColor(sf::Color(0, 255, 255, 20));

    m_normalColor = sf::Color(0, 255, 255, 20);
    m_hoverColor = sf::Color(255, 0, 255, 45);
    m_textNormal = sf::Color::White;
    m_textHover = sf::Color::Cyan;

    m_box.setFillColor(m_normalColor);
    m_text.setFillColor(m_textNormal);

    sf::FloatRect b = m_text.getLocalBounds();
    m_text.setOrigin({ b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f });
}

void Button::setPosition(sf::Vector2f pos) {
    m_box.setOrigin({ 200.f, 32.5f });
    m_box.setPosition(pos);
    m_text.setPosition(pos);
}

void Button::setColors(sf::Color normal, sf::Color hover, sf::Color txtNormal, sf::Color txtHover) {
    m_normalColor = normal;
    m_hoverColor = hover;
    m_textNormal = txtNormal;
    m_textHover = txtHover;
    m_box.setFillColor(m_isHovered ? m_hoverColor : m_normalColor);
    m_text.setFillColor(m_isHovered ? m_textHover : m_textNormal);
}

void Button::setCallback(std::function<void()> callback) {
    m_callback = callback;
}

void Button::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (auto* mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouse->button == sf::Mouse::Button::Left && m_isHovered && m_callback)
            m_callback();
    }
}

void Button::update(sf::Vector2f mousePos) {
    m_isHovered = m_box.getGlobalBounds().contains(mousePos);
    m_box.setFillColor(m_isHovered ? m_hoverColor : m_normalColor);
    m_text.setFillColor(m_isHovered ? m_textHover : m_textNormal);
}

void Button::draw(sf::RenderWindow& window) const {
    window.draw(m_box);
    window.draw(m_text);
}