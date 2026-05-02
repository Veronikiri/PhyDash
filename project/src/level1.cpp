#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <random>
#include "obstacles.hpp"
#include "player.hpp"

float score = 0.f;
int best = 0.f;

bool alive = true;

float timer = 0.f;
const float delay = 1.f;
std::vector<Spike> spikes;
std::vector<Block> blocks;
std::vector<Orb> orbs;
std::vector<Pad> pads;
std::vector<Portal> portals;

Player player;

sf::Music music;

void reset() {
    spikes.clear();
    blocks.clear();
    orbs.clear();
    pads.clear();
    portals.clear();
    alive = true;
    
    score = 0.f;
    player.setpos(100.f, 500.f);
    player.setvy(0.f);
    player.setmode(gamemode::cube);
    player.setonGround(true);
    player.setonBlock(false);
    player.setonOrb(false);
    
    music.stop();
    music.play();

    obstacle pattern;
    pattern.addPortal(650.f, 400.f, gamemode::ship);
    pattern.addSpike(850.f, 500.f);
    pattern.addSpike(900.f, 500.f);
    pattern.addSpike(950.f, 500.f);
    pattern.addSpike(1000.f, 500.f);

    spikes = pattern.spikes;
    blocks = pattern.blocks;
    orbs = pattern.orbs;
    pads = pattern.pads;
    portals = pattern.portals;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "kubik");

    reset();

    sf::RectangleShape ground(sf::Vector2f(800.f, 200.f));
    ground.setFillColor(sf::Color::Blue);
    ground.setPosition(sf::Vector2f(0.f, 550.f));

    sf::Font font;
    sf::Text scoreText(font, "Score: 0", 36);
    sf::Text bestText(font, "Best: 0", 36);
    if (font.openFromFile("assets/fonts/arial.ttf"))
    {
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(sf::Vector2f(20.f, 20.f));

        bestText.setFillColor(sf::Color::White);
        bestText.setPosition(sf::Vector2f(20.f, 60.f));
    }

    
    if (music.openFromFile("assets/music/music1.ogg")) 
    {
        music.setLooping(true);
        music.setVolume(100.f);
        music.play();
    }

    sf::Music deathsound;
    if (deathsound.openFromFile("assets/music/deathsound.ogg")) 
    {
        deathsound.setLooping(false);
        deathsound.setVolume(50.f);
    }

    

    sf::Clock clock;
    
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        player.setonGround(player.getpos().y >= 500.f);
        
        while (const auto event = window.pollEvent())
        {
            if (const auto* key = event->getIf<sf::Event::KeyPressed>())
            {
                if (key->code == sf::Keyboard::Key::Space)
                {
                    if (player.getmode() == gamemode::cube) {
                        player.clickaction();
                    }
                    if (player.getmode() == gamemode::ship) {
                        player.clickaction();
                    }
                }
            }
            if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouse->button == sf::Mouse::Button::Left)
                {
                    if (player.getmode() == gamemode::cube) {
                        player.clickaction();
                    }
                    if (player.getmode() == gamemode::ship) {
                        player.clickaction();
                    }
                }
            }
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
        {
            reset();
        }
        
        if (alive) 
        {
            score += dt * 10.f;
            scoreText.setString("Score: " + std::to_string((int)score));
            
            player.update(dt);
            if (player.getpos().y >= 500.f)
            {
                player.setpos(player.getpos().x, 500.f);
            }

            for (auto& spike : spikes)
            {
                spike.update(dt);
            }
                
            for (auto& block : blocks)
            {
                block.update(dt);
            }

            for (auto& orb : orbs)
            {
                orb.update(dt);
            }

            for (auto& pad : pads)
            {
                pad.update(dt);
            }
            for (auto& portal : portals)
            {
                portal.update(dt);
            }

            sf::FloatRect playerBounds = player.getBounds();
            
            for (auto& block : blocks)
            {
                if (playerBounds.findIntersection(block.getBounds()))
                {
                    sf::FloatRect blockBounds = block.getBounds();
                    float playerBottom = playerBounds.position.y + playerBounds.size.y;
                    float blockTop = blockBounds.position.y;
                    
                    if (player.getvy() > 0 && playerBottom <= blockTop + 15.f)
                    {
                        player.setpos(player.getpos().x, blockTop - playerBounds.size.y);
                        player.setvy(0.f);
                        player.setonBlock(true);
                    }
                    else
                    {
                        if (!player.getonBlock()) {
                            alive = false;
                            deathsound.play();
                            break;
                        }
                    }
                }
            }
            for (const auto& spike : spikes)
            {
                if (playerBounds.findIntersection(spike.getBounds()))
                {
                    alive = false;
                    deathsound.play();
                    break;
                }
            }
            for (auto& orb : orbs)
            {
                if (playerBounds.findIntersection(orb.getBounds()))
                {
                    player.setonOrb(true);
                    break;
                }
            }
            for (auto& pad : pads)
            {
                if (playerBounds.findIntersection(pad.getBounds()))
                {
                    player.setvy(-800.f);
                    break;
                }
            }
            for (auto& portal : portals)
            {
                if (playerBounds.findIntersection(portal.getBounds()))
                {
                    player.setmode(portal.getmode());
                    break;
                }
            }
        }
        else
        {
            timer += dt;
            if (timer >= delay)
            {
                if ((int)score > best)
                {
                    best = score;
                    bestText.setString("Best: " + std::to_string(best));
                }
                reset();
            }
        }

        window.clear(sf::Color::Black);
        player.draw(window);
        for (auto& spike : spikes) spike.draw(window);
        for (auto& block : blocks) block.draw(window);
        for (auto& orb : orbs) orb.draw(window);
        for (auto& pad : pads) pad.draw(window);
        for (auto& portal : portals) portal.draw(window);
        window.draw(ground);
        window.draw(scoreText);
        window.draw(bestText);
        window.display();
    }
    
    return 0;
}