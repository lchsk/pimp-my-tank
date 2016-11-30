#ifndef GAME_H
#define GAME_H

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "tank.h"
#include "map.h"
#include "bullet_mgr.h"
#include "hud.h"
#include "animation.h"

namespace pmt
{
    enum class MenuScreen {
        Main,
        Maps,
        Credits,
        Help
    };

    class Game
    {
        public:
            Game(const Game&) = delete;
            Game& operator=(const Game&) = delete;

            Game();
            ~Game();

            void run();

        private:
            void handle_events();
            void update(sf::Time delta);
            void render();

            void _start_map();
            void _next_turn();
            unsigned _next_tank_id(unsigned tank_id);

            std::shared_ptr<sf::Font> _font;

            std::vector<std::shared_ptr<pmt::Tank> > _tanks;

            std::shared_ptr<pmt::Map> _map;
            std::unique_ptr<pmt::Hud> _hud;

            std::unique_ptr<sf::RenderWindow> _window;

            std::vector<std::string> _filenames;
            std::unordered_map<std::string, std::unique_ptr<sf::Texture> >
                _textures;

            // Map tiles IDs to texture filenames
            std::unordered_map<int, std::string> _tiles_map;

            std::shared_ptr<pmt::BulletMgr> _bullet_mgr;

            std::unique_ptr<pmt::Animation> _explosion;

            std::unique_ptr<sf::Sprite> _menu_bg;

            void _run_ai_turn();

            unsigned _tanks_count;
            unsigned _tank_turn;

            bool _in_menu;
            bool _game_on;
            std::unique_ptr<sf::RectangleShape> _menu_overlay;

            std::unique_ptr<sf::Text> _menu_logo;

            std::shared_ptr<sf::Text> _menu_continue;
            std::shared_ptr<sf::Text> _menu_new_game;
            std::shared_ptr<sf::Text> _menu_help;
            std::shared_ptr<sf::Text> _menu_credits;
            std::shared_ptr<sf::Text> _menu_quit;

            std::shared_ptr<sf::Text> _menu_text_credits;
            std::shared_ptr<sf::Text> _menu_text_help;

            std::shared_ptr<sf::Text> _menu_text_maps_stats;
            std::shared_ptr<sf::Text> _menu_text_map_info;

            std::unique_ptr<sf::Text> _text_end_game_1;
            std::unique_ptr<sf::Text> _text_end_game_2;
            std::unique_ptr<sf::Text> _text_end_game_3;

            std::shared_ptr<sf::Text> _menu_back;
            std::shared_ptr<sf::Text> _menu_prev;
            std::shared_ptr<sf::Text> _menu_next;
            std::shared_ptr<sf::Text> _menu_play;

            std::vector<std::shared_ptr<sf::Text> > _menu;

            std::vector<std::string> _maps_meta;
            std::vector<std::shared_ptr<pmt::Map> > _maps;

            int _menu_selected;
            int _map_selected;
            int _extra_selected;
            MenuScreen _current_menu;

            void _init_menu();
            void _init_maps();
            void _draw_menu();
            void _update_menu();
    };
}
#endif
