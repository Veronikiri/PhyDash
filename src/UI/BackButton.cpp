#include "UI/BackButton.h"

BackButton::BackButton(const sf::Font& font)
    : Button(font, "BACK", 30)
{
    setColors(sf::Color::Transparent, sf::Color::Transparent, sf::Color::White, sf::Color::Cyan);
}