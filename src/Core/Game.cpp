#include "Core/Game.h"
#include "Scenes/MenuScene.h"
#include <iostream>

Game::Game()
    : m_window(sf::VideoMode({ 1280, 720 }), "Phy Dash")
{
    m_window.setFramerateLimit(60);
    if (!m_font.openFromFile("FZeroGbaText1.ttf")) {
        if (!m_font.openFromFile("assets/fonts/FZeroGbaText1.ttf")) {
            std::cerr << "FATAL: Font not found!" << std::endl;
            exit(1);
        }
    }
    pushScene(std::make_unique<MenuScene>(*this));
}

void Game::run() {
    sf::Clock clock;
    while (m_window.isOpen()) {
        while (auto event = m_window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                m_window.close();
            if (!m_scenes.empty())
                m_scenes.top()->handleEvent(*event, m_window);
        }
        if (!m_scenes.empty())
            m_scenes.top()->update(clock.restart());

        m_window.clear(sf::Color(10, 10, 18));
        if (!m_scenes.empty())
            m_scenes.top()->render(m_window);
        m_window.display();
    }
}

void Game::pushScene(std::unique_ptr<Scene> scene) {
    if (!m_scenes.empty())
        m_scenes.top()->onExit();
    m_scenes.push(std::move(scene));
    m_scenes.top()->onEnter();
}

void Game::popScene() {
    if (m_scenes.empty()) return;
    m_scenes.top()->onExit();
    m_scenes.pop();
    if (!m_scenes.empty())
        m_scenes.top()->onEnter();
}

void Game::changeScene(std::unique_ptr<Scene> scene) {
    popScene();
    pushScene(std::move(scene));
}

const sf::Font& Game::getFont() const {
    return m_font;
}

sf::RenderWindow& Game::getWindow() {
    return m_window;
}