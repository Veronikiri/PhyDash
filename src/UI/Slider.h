#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

class Slider {
public:
    Slider(sf::Vector2f position, float width, float height, sf::Color trackColor, sf::Color knobColor);
    ~Slider() = default;

    Slider(const Slider&) = delete;
    Slider& operator=(const Slider&) = delete;
    Slider(Slider&&) = default;
    Slider& operator=(Slider&&) = default;

    void setValue(float value); // 0..1
    float getValue() const;
    void setCallback(std::function<void(float)> callback);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void update();
    void draw(sf::RenderWindow& window) const;

private:
    sf::RectangleShape m_track;
    sf::RectangleShape m_knob;
    float m_value = 0.5f;
    bool m_dragging = false;
    std::function<void(float)> m_callback;
};