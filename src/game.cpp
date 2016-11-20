#include <iostream>

#include "util.h"
#include "game.h"

namespace pmt
{
    const std::string ASSETS_DIR = "assets/";

    Game::Game()
    {
        _window = std::make_unique<sf::RenderWindow>
            (sf::VideoMode(pmt::config::WINDOW_W, pmt::config::WINDOW_H),
             "pimp-my-tank");

        _filenames = {
            "gun.png",
            "tank.png",
            "shell.png",

            "green.png",
            "red.png",
            "shield.png",

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

        _bullet_mgr = std::make_shared<pmt::BulletMgr>();
        _bullet_mgr->add_bullets(pmt::WeaponType::Missile,
                                 1,
                                 _textures["shell.png"]);

        _map = std::make_unique<pmt::Map>(_tiles_map, _textures);

        _tanks_count = std::stoi(_map->get_param("tanks_count"));

        for (unsigned i = 0; i < _tanks_count; i++) {
            std::string tank_id = "tank_" + std::to_string(i);

            _tanks.push_back(std::make_shared<pmt::Tank>(
                _bullet_mgr,
                _textures["tank.png"],
                _textures["gun.png"],
                _textures["green.png"],
                _textures["red.png"],
                _textures["shield.png"],
                _map->get_param(tank_id + "_human") == "true"
                    ? GameSide::Human : GameSide::Computer,
                _map->get_param(tank_id + "_side") == "left",
                std::stoi(_map->get_param(tank_id + "_x")),
                std::stoi(_map->get_param(tank_id + "_y"))
            ));
        }

        _tank_turn = 0;
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
        _bullet_mgr->update(delta);
    }

    void Game::render()
    {
        _window->clear();
        _map->render(*_window);

        for (unsigned i = 0; i < _tanks_count; i++) {
            _tanks[i]->render(*_window);
        }

        _bullet_mgr->render(*_window);
        _window->display();
    }

    void Game::handle_events()
    {
        sf::Event event;

        while(_window->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                _window->close();
            else if (event.type == sf::Event::KeyPressed) {
                auto& current_tank = _tanks[_tank_turn];

                if (event.key.code == sf::Keyboard::Escape)
                    _window->close();
                else if (current_tank->is_human()) {
                    if (event.key.code == sf::Keyboard::Up)
                        current_tank->gun_up();
                    else if (event.key.code == sf::Keyboard::Down)
                        current_tank->gun_down();
                    else if (event.key.code == sf::Keyboard::Space)
                        current_tank->shoot();
                }
            }
        }
    }
}
