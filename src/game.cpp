#include <iostream>

#include "game.h"

namespace pmt
{
    const std::string ASSETS_DIR = "assets/";

    Game::Game()
    {
        _window = std::make_unique<sf::RenderWindow>
            (sf::VideoMode(800, 450), "pimp-my-tank");

        _filenames = {
            "gun.png",
            "tank.png",
            "ground.png",
            "ground_sand.png",
            "ground_rock.png"
        };

        _tiles_map = {
            {1, "ground.png"},
            {2, "ground_sand.png"},
            {3, "ground_rock.png"}
        };

        for (std::string& filename : _filenames) {
            std::unique_ptr<sf::Texture> texture(new sf::Texture());

            std::string path = ASSETS_DIR + filename;

            if (! texture->loadFromFile(path)) {
                throw std::runtime_error("Unable to load " + path);
            }

            _textures[filename] = std::move(texture);
        }

        _player = std::make_shared<pmt::Tank>(_textures["tank.png"], 0, 100, 100);

        _map = std::make_unique<pmt::Map>(_tiles_map, _textures);
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
        _player->render(*_window);
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
