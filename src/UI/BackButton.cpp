#include "UI/BackButton.h"

BackButton::BackButton(const sf::Font& font)
    : Button(font, "BACK", 30)
{
    // Автоматический размер под текст "BACK"
    sf::Text tempText(font, "BACK", 30);
    sf::FloatRect bounds = tempText.getLocalBounds();
    sf::Vector2f buttonSize = { bounds.size.x + 40.f, bounds.size.y + 20.f };

    setFixedSize(buttonSize);
    setOutlineThickness(1.5f);
    setColors(sf::Color::Transparent, sf::Color::Transparent, sf::Color::White, sf::Color::Cyan);
}