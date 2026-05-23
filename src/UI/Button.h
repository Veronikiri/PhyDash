#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

class Button {
public:
    Button(const sf::Font& font, const std::string& text, unsigned int size = 34);
    ~Button() = default;

    Button(const Button&) = delete;
    Button& operator=(const Button&) = delete;
    Button(Button&&) = default;
    Button& operator=(Button&&) = default;

    void setPosition(sf::Vector2f pos);
    void setColors(sf::Color normal, sf::Color hover, sf::Color textNormal, sf::Color textHover);
    void setCallback(std::function<void()> callback);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void update(sf::Vector2f mousePos);
    void draw(sf::RenderWindow& window) const;

    sf::FloatRect getGlobalBounds() const { return m_box.getGlobalBounds(); }

    void setFixedSize(sf::Vector2f size);
    void setOutlineThickness(float thickness);

private:
    sf::RectangleShape m_box;
    sf::Text m_text;
    sf::Color m_normalColor, m_hoverColor;
    sf::Color m_textNormal, m_textHover;
    std::function<void()> m_callback;
    bool m_isHovered = false;
};