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
            // Background
            "trees1.png",
            "trees2.png",
            "trees3.png",
            "trees4.png",

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

        _font = std::make_shared<sf::Font>();

        _map = std::make_unique<pmt::Map>(_tiles_map, _textures);
        _hud = std::make_unique<pmt::Hud>(_font);

        _tanks_count = std::stoi(_map->get_param("tanks_count"));

        if (! _font->loadFromFile("assets/Armata-Regular.ttf")) {
            std::cout << "No font found\n";
        }

        for (unsigned i = 0; i < _tanks_count; i++) {
            std::string tank_str = "tank_" + std::to_string(i);

            _tanks.push_back(std::make_shared<pmt::Tank>(i,
                _bullet_mgr,
                _textures["tank.png"],
                _textures["gun.png"],
                _textures["green.png"],
                _textures["red.png"],
                _textures["shield.png"],
                _font,
                _map->get_param(tank_str + "_human") == "true"
                    ? GameSide::Human : GameSide::Computer,
                _map->get_param(tank_str + "_side") == "left",
                std::stoi(_map->get_param(tank_str + "_x")),
                std::stoi(_map->get_param(tank_str + "_y"))
            ));
        }

        _tank_turn = 0;
        _tanks[_tank_turn]->activate();

        _hud->show_turn(_tanks[_tank_turn]);
        _hud->show_cash(_tanks[_tank_turn]);
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
        _map->update(delta);
        _hud->update(delta);

        bool tank_hit = false;
        bool env_hit = false;

        for (auto& bullet : _bullet_mgr->get_flying_bullets()) {
            for (auto& tank : _tanks) {
                tank_hit = tank->check_collision(bullet);

                if (tank_hit) {
                    _tanks[bullet->get_origin_tank()]
                        ->add_cash(pmt::config::REWARD_HIT);

					break;
				}
            }

            if (tank_hit) {
                break;
            } else {
                env_hit = _map->check_collision(bullet);

                if (env_hit)
                    break;
            }
        }

        if (tank_hit || env_hit)
            _next_turn();

        _hud->show_cash(_tanks[_tank_turn]);
    }

    void Game::render()
    {
        _window->clear();
        _map->render(*_window);

        for (unsigned i = 0; i < _tanks_count; i++) {
            _tanks[i]->render(*_window);
        }

        _bullet_mgr->render(*_window);

        _hud->render(*_window);
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

    unsigned Game::_next_tank_id()
    {
        if (_tank_turn + 1 >= _tanks_count)
            return 0;
        else
            return _tank_turn + 1;
    }

    void Game::_next_turn()
    {
        auto& tank = _tanks[_tank_turn];
        tank->deactivate();

        bool found_next_tank = false;

        for (int i = 0; i < _tanks_count; i++) {
            unsigned next_tank_id = _next_tank_id();

            auto& tank = _tanks[next_tank_id];

            if (tank->is_alive()) {
                found_next_tank = true;
                _tank_turn = next_tank_id;
                tank->activate();
                _hud->show_turn(_tanks[_tank_turn]);

                break;
            }
        }

        if (found_next_tank) {
            std::cout << "NEW TANK ID: " << _tank_turn << "\n";
        } else {
            std::cout << "Game Over\n";
        }
    }
}
