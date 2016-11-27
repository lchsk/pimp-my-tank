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

            "pole.png",
            "flag_1.png",
            "flag_2.png",

            "explosion.png",

            "gun.png",
            "tank.png",
            "missile.png",
            "magnum.png",
            "sheep.png",
            "excl.png",

            "green.png",
            "red.png",
            "shield.png",

            "red_rect.png",
            "green_rect.png",
            "orange_rect.png",

            "green_tile.png",
            "brown_tile.png",
            "gray_tile.png"
        };

        _tiles_map = {
            {1, "green_tile.png"},
            {2, "brown_tile.png"},
            {3, "gray_tile.png"}
        };

        for (std::string& filename : _filenames) {
            std::unique_ptr<sf::Texture> texture(new sf::Texture());

            std::string path = ASSETS_DIR + filename;

            if (! texture->loadFromFile(path)) {
                throw std::runtime_error("Unable to load " + path);
            }

            _textures[filename] = std::move(texture);
        }

        _explosion = std::make_unique<pmt::Animation>(
            _textures["explosion.png"], 12, 96, 96);

        _map = std::make_unique<pmt::Map>(_tiles_map, _textures);

        double wind, wind_change_period;

        if (_map->get_param("random_wind") == "true") {
            wind = pmt::util::get_random(-1, 1);
        } else {
            wind = std::stod(_map->get_param("initial_wind"))
                / pmt::config::WIND_MULTIPLIER;
        }

        wind_change_period = std::stod(_map->get_param("wind_change_period"));

        _bullet_mgr = std::make_shared<pmt::BulletMgr>(
            _textures,
            wind,
            wind_change_period);

        _bullet_mgr->add_bullets(pmt::WeaponType::Missile,
                                 1,
                                 _textures["missile.png"]);
        _bullet_mgr->add_bullets(pmt::WeaponType::MagnumMissile,
                                 1,
                                 _textures["magnum.png"]);
        _bullet_mgr->add_bullets(pmt::WeaponType::Sheep,
                                 1,
                                 _textures["sheep.png"]);

        _font = std::make_shared<sf::Font>();
        _hud = std::make_unique<pmt::Hud>(_textures, _font);

        _tanks_count = std::stoi(_map->get_param("tanks_count"));

        if (! _font->loadFromFile("assets/Minecraftia-Regular.ttf")) {
            std::cout << "No font found\n";
        }

        std::unordered_map<WeaponType, unsigned> weapons;

        for (unsigned i = 0; i < _tanks_count; i++) {
            std::string tank_str = "tank_" + std::to_string(i);

            weapons[WeaponType::Missile] =
                std::stoi(_map->get_param(tank_str + "_missiles"));

            weapons[WeaponType::MagnumMissile] =
                std::stoi(_map->get_param(tank_str + "_magnums"));

            weapons[WeaponType::Sheep] =
                std::stoi(_map->get_param(tank_str + "_sheeps"));

            _tanks.push_back(std::make_shared<pmt::Tank>(
                i,
                std::stoi(_map->get_param(tank_str + "_health")),
                std::stoi(_map->get_param(tank_str + "_shield")),
                std::stoi(_map->get_param(tank_str + "_cash")),
                weapons,
                _bullet_mgr,
                _textures,
                _font,
                _map->get_param(tank_str + "_human") == "true"
                    ? GameSide::Human : GameSide::Computer,
                _map->get_param(tank_str + "_side") == "left",
                std::stoi(_map->get_param(tank_str + "_x")),
                std::stoi(_map->get_param(tank_str + "_y"))
            ));
        }

        std::string first_turn_param = _map->get_param("first_turn");

        unsigned first_turn = 0;

        if (first_turn_param == "random") {
            first_turn = pmt::util::get_random_int(0, _tanks_count - 1);
        } else {
            first_turn = std::stoi(first_turn_param);
        }

        _tank_turn = first_turn;
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
        _hud->update(delta, _bullet_mgr->get_wind());

        bool tank_hit = false;
        bool env_hit = false;

        for (unsigned i = 0; i < _tanks_count; i++) {
            _tanks[i]->update(delta);
        }

        if (_tanks[_tank_turn]->can_shoot())
            _tanks[_tank_turn]->increase_shot_power(delta);

        for (auto& bullet : _bullet_mgr->get_flying_bullets()) {
            for (auto& tank : _tanks) {
                if (! tank->is_alive())
                    continue;

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

                if (env_hit) {
                    sf::Vector2f expl_size = _explosion->get_size();

                    _explosion->set_position(
                        bullet->get_position().x - expl_size.x / 2,
                        bullet->get_position().y - expl_size.y / 2
                    );
                    double scale = pmt::util::get_random(0.6, 0.9);
                    _explosion->set_scale(scale, scale);

                    _explosion->play();

                    break;
                }
            }
        }

        if (tank_hit || env_hit)
            _next_turn();

        _hud->show_cash(_tanks[_tank_turn]);

        _explosion->update(delta);
    }

    void Game::render()
    {
        _window->clear();
        _map->render(*_window);

        _bullet_mgr->render(*_window);

        for (unsigned i = 0; i < _tanks_count; i++) {
            _tanks[i]->render(*_window);
        }

        _explosion->render(*_window);

        _hud->render(*_window);
        _window->display();
    }

    void Game::handle_events()
    {
        sf::Event event;

        while (_window->pollEvent(event)) {
            auto& current_tank = _tanks[_tank_turn];

            if (event.type == sf::Event::Closed)
                _window->close();
            else if (event.type == sf::Event::KeyPressed) {
                if (current_tank->is_human()) {
                    if (_hud->is_shop_open()) {
                        if (event.key.code == sf::Keyboard::Escape
                            || event.key.code == sf::Keyboard::Key::S
                            || event.key.code == sf::Keyboard::Key::Q)
                            _hud->close_shop();
                        else if (event.key.code == sf::Keyboard::Up
                            || event.key.code == sf::Keyboard::Key::P)
                            _hud->shop_up();
                        else if (event.key.code == sf::Keyboard::Down
                            || event.key.code == sf::Keyboard::Key::N)
                            _hud->shop_down();
                        else if (event.key.code == sf::Keyboard::Return)
                            _hud->buy();
                    } else {
                        if (event.key.code == sf::Keyboard::Up)
                            current_tank->gun_up();
                        else if (event.key.code == sf::Keyboard::Down)
                            current_tank->gun_down();
                        else if (event.key.code == sf::Keyboard::Space) {
                            if (! current_tank->can_shoot())
                                current_tank->init_shot();
                        } else if (event.key.code == sf::Keyboard::Right) {
                            current_tank->next_weapon();
                            _hud->show_cash(current_tank);
                        } else if (event.key.code == sf::Keyboard::Left) {
                            current_tank->previous_weapon();
                            _hud->show_cash(current_tank);
                        } else if (event.key.code == sf::Keyboard::Key::S)
                            _hud->open_shop(current_tank);
                        else if (event.key.code == sf::Keyboard::Key::N)
                            _next_turn();
                        else if (event.key.code == sf::Keyboard::LControl) {
                            current_tank->spin_around();
                        }
                        else if (event.key.code == sf::Keyboard::Escape
                            || event.key.code == sf::Keyboard::Key::Q)
                            _window->close();
                    }
                }
            } else if (event.type == sf::Event::KeyReleased) {
                if (current_tank->can_shoot()) {
                    current_tank->shoot();
                    _hud->show_cash(current_tank);
                }
            }
        }
    }

    unsigned Game::_next_tank_id(unsigned tank_id)
    {
        if (tank_id + 1 >= _tanks_count)
            return 0;
        else
            return tank_id + 1;
    }

    void Game::_next_turn()
    {
        auto& tank = _tanks[_tank_turn];
        tank->deactivate();

        bool found_next_tank = false;
        unsigned next_tank_id = _tank_turn;

        for (unsigned i = 0; i < _tanks_count; i++) {
            next_tank_id = _next_tank_id(next_tank_id);

            auto& tank = _tanks[next_tank_id];

            if (tank->is_alive() && _tank_turn != next_tank_id) {
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
            std::cout << "WINNER: " << _tank_turn << "\n";
        }
    }
}
