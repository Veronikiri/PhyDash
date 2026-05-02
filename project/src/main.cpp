#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <random>
#include "obstacles.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "kubik");

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

    sf::Music music;
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
    
    sf::RectangleShape player(sf::Vector2f(50.f, 50.f));
    player.setFillColor(sf::Color::Green);
    player.setPosition(sf::Vector2f(100.f, 500.f));  

    sf::RectangleShape ground(sf::Vector2f(800.f, 200.f));
    ground.setFillColor(sf::Color::Blue);
    ground.setPosition(sf::Vector2f(0.f, 550.f));

    std::vector<Spike> spikes;
    std::vector<Block> blocks;
    std::vector<Orb> orbs;
    std::vector<Pad> pads;

    std::vector<obstacle> pool = createpool();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, pool.size() - 1);
    spikes.clear();
    blocks.clear();
    orbs.clear();
    pads.clear();
    int index = dist(gen);
    obstacle pattern = pool[index];
    spikes = pattern.spikes;
    blocks = pattern.blocks;
    orbs = pattern.orbs;
    pads = pattern.pads;
          
    float vy = 0.f;   
    float g = 1200.f;   
    float score = 0.f;
    int best = 0.f;

    bool alive = true;
    bool onGround = true;
    bool onBlock = false;
    bool onOrb = false;

    float timer = 0.f;
    const float delay = 1.f; 

    sf::Clock clock;
    
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        
        while (const auto event = window.pollEvent())
        {
            if (const auto* key = event->getIf<sf::Event::KeyPressed>())
            {
                if (key->code == sf::Keyboard::Key::Space && (onGround || onBlock || onOrb))
                {
                    vy = -550.f;
                    onOrb = false;
                    onGround = false;
                    onBlock = false;
                }
            }
            if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouse->button == sf::Mouse::Button::Left && (onGround || onBlock || onOrb))
                {
                    vy = -550.f;
                    onOrb = false;
                    onGround = false;
                    onBlock = false;
                }
            }
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
        {
            alive = true;
            onGround = true;
            onBlock = false;
            onOrb = false;
            score = 0.f;
            player.setFillColor(sf::Color::Green);
            player.setPosition(sf::Vector2f(200.f, 500.f));
            vy = 0.f;
            std::vector<obstacle> pool = createpool();
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dist(0, pool.size() - 1);
            spikes.clear();
            blocks.clear();
            orbs.clear();
            pads.clear();
            int index = dist(gen);
            obstacle pattern = pool[index];
            spikes = pattern.spikes;
            blocks = pattern.blocks;
            orbs = pattern.orbs;
            pads = pattern.pads;
            music.stop();
            music.play();
        }
        
        if (alive) 
        {
            score += dt * 10.f;
            scoreText.setString("Score: " + std::to_string((int)score));

            onGround = false;
            onBlock = false;
            onOrb = false;
            
            vy = vy + g * dt;                       
            player.move(sf::Vector2f(0.f, vy * dt)); 

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

            bool obsoff = true;
            for (const auto& spike : spikes)
            {
                if (spike.onscreen())
                {
                    obsoff = false;
                    break;
                }
            }

            for (const auto& block : blocks)
            {
                if (block.onscreen())
                {
                    obsoff = false;
                    break;
                }
            }

            for (const auto& orb : orbs)
            {
                if (orb.onscreen())
                {
                    obsoff = false;
                    break;
                }
            }

            for (const auto& pad : pads)
            {
                if (pad.onscreen())
                {
                    obsoff = false;
                    break;
                }
            }

            if (obsoff) {
                std::vector<obstacle> pool = createpool();
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> dist(0, pool.size() - 1);
                spikes.clear();
                blocks.clear();
                orbs.clear();
                pads.clear();
                int index = dist(gen);
                obstacle pattern = pool[index];
                spikes = pattern.spikes;
                blocks = pattern.blocks;
                orbs = pattern.orbs;
                pads = pattern.pads;
            }

            if (player.getPosition().y >= 500.f)
            {
                player.setPosition(sf::Vector2f(player.getPosition().x, 500.f));
                vy = 0.f;
                onGround = true;
            }

            sf::FloatRect playerBounds = player.getGlobalBounds();
            
            for (auto& block : blocks)
            {
                if (playerBounds.findIntersection(block.getBounds()))
                {
                    sf::FloatRect blockBounds = block.getBounds();
                    float playerBottom = playerBounds.position.y + playerBounds.size.y;
                    float blockTop = blockBounds.position.y;
                    
                    if (vy > 0 && playerBottom <= blockTop + 15.f)
                    {
                        player.setPosition(sf::Vector2f(player.getPosition().x, blockTop - playerBounds.size.y));
                        vy = 0.f;
                        onBlock = true;
                    }
                    else
                    {
                        if (!onBlock) {
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
                    onOrb = true;
                    break;
                }
            }
            for (auto& pad : pads)
            {
                if (playerBounds.findIntersection(pad.getBounds()))
                {
                    vy = -800.f;
                    break;
                }
            }
        }
        else
        {
            player.setFillColor(sf::Color::Red);
            
            timer += dt;
            if (timer >= delay)
            {
                alive = true;
                onGround = true;
                onBlock = false;
                onOrb = false;
                if ((int)score > best)
                {
                    best = score;
                    bestText.setString("Best: " + std::to_string(best));
                }
                score = 0.f;
                timer = 0.f;
                player.setFillColor(sf::Color::Green);
                player.setPosition(sf::Vector2f(200.f, 500.f));
                std::vector<obstacle> pool = createpool();
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> dist(0, pool.size() - 1);
                spikes.clear();
                blocks.clear();
                orbs.clear();
                pads.clear();
                int index = dist(gen);
                obstacle pattern = pool[index];
                spikes = pattern.spikes;
                blocks = pattern.blocks;
                orbs = pattern.orbs;
                pads = pattern.pads;
                vy = 0.f;
                music.stop();
                music.play();
            }
        }

        window.clear(sf::Color::Black);
        window.draw(player);
        for (auto& spike : spikes) spike.draw(window);
        for (auto& block : blocks) block.draw(window);
        for (auto& orb : orbs) orb.draw(window);
        for (auto& pad : pads) pad.draw(window);
        window.draw(ground);
        window.draw(scoreText);
        window.draw(bestText);
        window.display();
    }
    
    return 0;
}