#include <iostream>

#include "game.h"

namespace ks
{
    Game::Game()
    {
        _window = std::make_unique<sf::RenderWindow>(
            sf::VideoMode(800, 600),
            "pimp-my-tank"
            );
    }

    Game::~Game()
    {
    }

    void Game::run()
    {
        sf::Time per_frame = sf::seconds(1.0f / 60.0f);
        sf::Clock clock;
        sf::Time last_update;

        while (_window->isOpen()) {
            sf::Time delta = clock.getElapsedTime() - last_update;

            if (delta > per_frame) {
                handle_events();
                update(delta);
                render();
                last_update = clock.getElapsedTime();
            }
        }
    }

    void Game::update(sf::Time delta)
    {
    }

    void Game::render()
    {
        _window->clear();
        _window->display();
    }

    void Game::handle_events()
    {
        sf::Event event;

        while(_window->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                _window->close();
            else if (event.type == sf::Event::KeyPressed
                     and event.key.code == sf::Keyboard::Escape)
                _window->close();
        }
    }
}
