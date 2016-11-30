#include <iostream>
#include <fstream>

#include "util.h"
#include "game.h"

namespace pmt
{
    const std::string ASSETS_DIR = "assets/";

    Game::Game()
        : _in_menu(true),
          _game_on(false),
          _menu_selected(1),
          _map_selected(0),
          _extra_selected(0),
          _current_menu(MenuScreen::Main)
    {
        _window = std::make_unique<sf::RenderWindow>
            (sf::VideoMode(pmt::config::WINDOW_W, pmt::config::WINDOW_H),
             "pimp-my-tank");

        _menu_overlay = std::make_unique<sf::RectangleShape>();
        _menu_overlay->setSize(
            sf::Vector2f(pmt::config::WINDOW_W, pmt::config::WINDOW_H));
        _menu_overlay->setFillColor(pmt::color::Overlay);

        _filenames = {
            "bg.png",

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

        _bullet_mgr = std::make_shared<pmt::BulletMgr>(_textures);

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

        _menu_bg = std::make_unique<sf::Sprite>(*_textures["bg.png"].get());

        _init_maps();
        _init_menu();

        _hud = std::make_unique<pmt::Hud>(_textures, _font);

        if (! _font->loadFromFile("assets/Minecraftia-Regular.ttf")) {
            std::cout << "No font found\n";
        }
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
        if (_in_menu) {
            _update_menu();
        } else {
            _map->update(delta);

            if (! _hud->is_shop_open()) {
                _bullet_mgr->update(delta);

                _hud->update(delta, _bullet_mgr->get_wind());
            }

            bool tank_hit = false;
            bool env_hit = false;

            for (unsigned i = 0; i < _tanks_count; i++)
                _tanks[i]->update(delta);

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
    }

    void Game::render()
    {
        _window->clear();

        if (_in_menu) {
            _window->draw(*_menu_bg);

            _window->draw(*_menu_overlay);
            _draw_menu();
        } else {
            _map->render(*_window);

            _bullet_mgr->render(*_window);

            for (unsigned i = 0; i < _tanks_count; i++) {
                _tanks[i]->render(*_window);
            }

            _explosion->render(*_window);

            _hud->render(*_window);

            if (! _game_on) {
                _window->draw(*_text_end_game_1);
                _window->draw(*_text_end_game_2);
                _window->draw(*_text_end_game_3);
            }
        }

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
                if (_in_menu) {
                    if (_current_menu == MenuScreen::Maps) {
                        if (event.key.code == sf::Keyboard::Left) {
                            if (_extra_selected - 1 > -1) {
                                _extra_selected--;
                            }
                        } else if (event.key.code == sf::Keyboard::Right) {
                            if (_extra_selected + 1 < 4) {
                                _extra_selected++;
                            }
                        } else if (event.key.code == sf::Keyboard::Return) {
                            switch (_extra_selected) {
                            case 0:
                                _current_menu = MenuScreen::Main;
                                _menu[_menu_selected]->setColor(sf::Color::White);
                                _menu_selected = 1;
                                break;
                            case 1:
                                _menu_prev->setColor(pmt::color::Blue);

                                if (_map_selected - 1 > -1) {
                                    _map_selected--;
                                }

                                break;
                            case 2:
                                _menu_next->setColor(pmt::color::Blue);

                                if ((unsigned) _map_selected + 1 < _maps.size()) {
                                    _map_selected++;
                                }

                                break;
                            case 3:
                                _menu_play->setColor(pmt::color::Blue);
                                _start_map();
                                break;
                            }
                        }
                    } else if (_current_menu == MenuScreen::Help
                               || _current_menu == MenuScreen::Credits) {
                        if (_extra_selected == 0) {
                            _current_menu = MenuScreen::Main;
                            _menu[_menu_selected]->setColor(sf::Color::White);
                            _menu_selected = 1;
                        }

                    } else if (event.key.code == sf::Keyboard::Down
                        || event.key.code == sf::Keyboard::Key::N) {
                        if (_menu_selected + 1 < 5) {
                            _menu[_menu_selected]->setColor(sf::Color::White);
                            _menu_selected++;
                        }
                    } else if (event.key.code == sf::Keyboard::Up
                               || event.key.code == sf::Keyboard::Key::P) {
                        if (_menu_selected - 1 > (_game_on ? -1 : 0)) {
                            _menu[_menu_selected]->setColor(sf::Color::White);
                            _menu_selected--;
                        }
                    } else if (event.key.code == sf::Keyboard::Return) {
                        if (_menu_selected == 4)
                            _window->close();
                        else if (_menu_selected == 3) {
                            _current_menu = MenuScreen::Credits;
                            _extra_selected = 0;
                        } else if (_menu_selected == 2) {
                            _current_menu = MenuScreen::Help;
                            _extra_selected = 0;
                        } else if (_menu_selected == 1) {
                            _extra_selected = 3; // Select Play button
                            _current_menu = MenuScreen::Maps;
                        } else if (_menu_selected == 0 && _game_on) {
                            _in_menu = false;
                        }
                    } else if (event.key.code == sf::Keyboard::Escape
                               || event.key.code == sf::Keyboard::Key::Q) {

                        _current_menu = MenuScreen::Main;
                        _menu[_menu_selected]->setColor(sf::Color::White);
                        _menu_selected = 1;
                    }
                } else if (current_tank->is_human()) {
                    if (_hud->is_shop_open() && _game_on) {
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
                        if (_game_on) {
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
                        }

                        if (event.key.code == sf::Keyboard::Escape
                            || event.key.code == sf::Keyboard::Key::Q) {
                            _in_menu = true;
                            _current_menu = MenuScreen::Main;
                            _menu[_menu_selected]->setColor(sf::Color::White);
                            _menu_selected = _game_on ? 0 : 1;
                        }
                    }
                }
            } else if (event.type == sf::Event::KeyReleased) {
                if (! _in_menu) {
                    if (current_tank->can_shoot() && _game_on) {
                        current_tank->shoot();
                        _hud->show_cash(current_tank);
                    }
                }
            }
        }
    }

    void Game::_run_ai_turn()
    {
        if (! _tanks[_tank_turn]->is_human()) {
            int target = -1;

            while (true) {
                for (unsigned i = 0; i < _tanks_count; i++) {
                    if (_tanks[i]->is_alive() && _tank_turn != i) {
                        double r = pmt::util::get_random(0, 1);

                        if (r > 0.5) {
                            target = i;
                            break;
                        }
                    }
                }

                if (target > -1)
                    break;
            }

            _tanks[_tank_turn]->ai_turn(_tanks[target], _bullet_mgr->get_wind());
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
            _run_ai_turn();
        } else {
            _game_on = false;

            _text_end_game_2->setString(
                "player "
                + std::to_string(_tank_turn + 1)
                + " won ("
                + (_tanks[_tank_turn]->is_human() ? "human" : "ai")
                + ")"
            );
        }
    }

    void Game::_start_map()
    {
        _map = _maps[_map_selected];

        _tanks.clear();
        _tanks_count = std::stoi(_map->get_param("tanks_count"));

        double wind, wind_change_period;

        if (_map->get_param("random_wind") == "true") {
            wind = pmt::util::get_random(-1, 1);
        } else {
            wind = std::stod(_map->get_param("initial_wind"))
                / pmt::config::WIND_MULTIPLIER;
        }

        wind_change_period = std::stod(_map->get_param("wind_change_period"));

        _bullet_mgr->set_wind(wind);
        _bullet_mgr->set_wind_change_period(wind_change_period);

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
                std::stoi(_map->get_param(tank_str + "_level")),
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
        _run_ai_turn();

        _hud->show_turn(_tanks[_tank_turn]);
        _hud->show_cash(_tanks[_tank_turn]);

        // Start!
        _in_menu = false;
        _game_on = true;
    }

    void Game::_init_maps()
    {
        std::ifstream maps_file(pmt::DIR_MAPS + "maps");

        std::string line;

        if (maps_file.is_open()) {
            std::string map_id;

            while (! maps_file.eof()) {
                maps_file >> map_id;

                _maps_meta.push_back(map_id);
            }

            maps_file.close();
        } else {
            throw std::runtime_error("<maps> file not found");
        }

        for (auto &name : _maps_meta) {
            _maps.push_back(
                std::make_shared<pmt::Map>(
                    name,
                    _tiles_map,
                    _textures
                )
            );
        }
    }

    void Game::_init_menu()
    {
        _menu_logo = std::make_unique<sf::Text>("pimp my tank", *_font, 64);
        _menu_logo->setColor(pmt::color::Red);
        _menu_logo->setPosition(50, 70);

        _menu_continue = std::make_shared<sf::Text>("continue", *_font, 32);
        _menu_continue->setColor(sf::Color::White);
        _menu_continue->setPosition(90, 180);

        _menu_new_game = std::make_shared<sf::Text>("new game", *_font, 32);
        _menu_new_game->setColor(sf::Color::White);
        _menu_new_game->setPosition(90, 220);

        _menu_help = std::make_shared<sf::Text>("help", *_font, 32);
        _menu_help->setColor(sf::Color::White);
        _menu_help->setPosition(90, 260);

        _menu_credits = std::make_shared<sf::Text>("credits", *_font, 32);
        _menu_credits->setColor(sf::Color::White);
        _menu_credits->setPosition(90, 300);

        _menu_quit = std::make_shared<sf::Text>("quit", *_font, 32);
        _menu_quit->setColor(sf::Color::White);
        _menu_quit->setPosition(90, 340);

        _menu.push_back(_menu_continue);
        _menu.push_back(_menu_new_game);
        _menu.push_back(_menu_help);
        _menu.push_back(_menu_credits);
        _menu.push_back(_menu_quit);

        const std::string credits_1
            = "Author: Maciej Lechowski\nmjlechowski@gmail.com\ngithub.com/lchsk\n\n";

        const std::string credits_2
            = "Graphics: \nBleddyuffles, www.bleddy.xyz\n"
            "GrafxKid, http://opengameart.org/content/items-and-elements\n"
            "J-Robot, http://opengameart.org/content/pixel-explosion-12-frames\n"
            "mrnannings, http://opengameart.org/content/pixel-tank\n"
            "Kenney, Kenney.nl\n"
            "Euro-t-guide.com, https://commons.wikimedia.org/wiki/M4_Sherman#/media/File:Utah_Beach_2006-Sherman.jpg\n";

        _menu_text_credits_1 = std::make_shared<sf::Text>(credits_1, *_font, 16);
        _menu_text_credits_1->setPosition(50, 170);
        _menu_text_credits_2 = std::make_shared<sf::Text>(credits_2, *_font, 8);
        _menu_text_credits_2->setPosition(50, 250);

        const std::string help
            =
            "<space>                             press to shoot\n"
            "<left> / <right>             change weapon\n"
            "<up> / <down>                alter angle of the gun\n"
            "S                                              open / close shop\n"
            "<return>                           buy in a shop / confirm\n"
            "N                                              move to the next turn\n"
            "<control>                         spin around the tank\n"
            "<esc>                                   open menu / go back";

        _menu_text_help = std::make_shared<sf::Text>(help, *_font, 16);
        _menu_text_help->setPosition(50, 170);

        _menu_text_maps_stats = std::make_shared<sf::Text>(
            std::to_string(_maps.size())
            , *_font, 16);
        _menu_text_maps_stats->setPosition(50, 170);
        _menu_text_maps_stats->setColor(pmt::color::Blue);

        _menu_text_map_info = std::make_shared<sf::Text>("", *_font, 16);
        _menu_text_map_info->setPosition(50, 210);

        _menu_back = std::make_shared<sf::Text>("back", *_font, 16);
        _menu_back->setPosition(50, 350);
        _menu_prev = std::make_shared<sf::Text>("prev", *_font, 16);
        _menu_prev->setPosition(110, 350);
        _menu_next = std::make_shared<sf::Text>("next", *_font, 16);
        _menu_next->setPosition(170, 350);
        _menu_play = std::make_shared<sf::Text>("play", *_font, 16);
        _menu_play->setPosition(230, 350);

        // End game messages
        _text_end_game_1 = std::make_unique<sf::Text>("game over", *_font, 64);
        _text_end_game_1->setColor(pmt::color::Red);
        _text_end_game_1->setPosition(200, 130);
        _text_end_game_2 = std::make_unique<sf::Text>("", *_font, 32);
        _text_end_game_2->setColor(pmt::color::Blue);
        _text_end_game_2->setPosition(195, 205);
        _text_end_game_3 = std::make_unique<sf::Text>(
            "press ESC to go back to menu",
            *_font,
            16
        );
        _text_end_game_3->setColor(sf::Color::White);
        _text_end_game_3->setPosition(250, 260);
    }

    void Game::_draw_menu()
    {
        _window->draw(*_menu_logo);

        switch (_current_menu) {
        case MenuScreen::Main:
            if (_game_on)
                _window->draw(*_menu_continue);
            _window->draw(*_menu_new_game);
            _window->draw(*_menu_help);
            _window->draw(*_menu_credits);
            _window->draw(*_menu_quit);

            break;

        case MenuScreen::Maps:
            _window->draw(*_menu_text_maps_stats);
            _window->draw(*_menu_text_map_info);

            _window->draw(*_menu_back);
            _window->draw(*_menu_prev);
            _window->draw(*_menu_next);
            _window->draw(*_menu_play);

            break;

        case MenuScreen::Credits:
            _window->draw(*_menu_text_credits_1);
            _window->draw(*_menu_text_credits_2);

            _window->draw(*_menu_back);

            break;

        case MenuScreen::Help:
            _window->draw(*_menu_text_help);

            _window->draw(*_menu_back);

            break;
        }
    }

    void Game::_update_menu()
    {
        _menu_text_maps_stats->setString(
            "Selected map "
            + std::to_string(_map_selected + 1)
            + " out of "
            + std::to_string(_maps.size())
        );

        if ((unsigned) _map_selected < _maps.size()) {
            auto& map = _maps[_map_selected];

            const std::string map_info
                = "Map name: " + map->get_param("name") + "\n"
                + "Human tanks: " + std::to_string(map->get_human_tanks_count()) + "\n"
                + "AI tanks: " + std::to_string(map->get_ai_tanks_count()) + "\n";

            _menu_text_map_info->setString(map_info);
        }

        _menu[_menu_selected]->setColor(pmt::color::Blue);

        _menu_back->setColor(sf::Color::White);
        _menu_prev->setColor(sf::Color::White);
        _menu_next->setColor(sf::Color::White);
        _menu_play->setColor(sf::Color::White);

        switch (_extra_selected) {
        case 0:
            _menu_back->setColor(pmt::color::Blue);
            break;
        case 1:
            _menu_prev->setColor(pmt::color::Blue);
            break;
        case 2:
            _menu_next->setColor(pmt::color::Blue);
            break;
        case 3:
            _menu_play->setColor(pmt::color::Blue);
            break;
        }

    }
}
