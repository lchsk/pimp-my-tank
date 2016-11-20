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

namespace pmt
{
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
            void _draw_hud(sf::RenderWindow& window);

            void _update_hud();

            void _next_turn();
            unsigned _next_tank_id();

            std::shared_ptr<sf::Font> _font;

            std::vector<std::shared_ptr<pmt::Tank> > _tanks;

            std::unique_ptr<pmt::Map> _map;

            std::unique_ptr<sf::RenderWindow> _window;

            std::vector<std::string> _filenames;
            std::unordered_map<std::string, std::unique_ptr<sf::Texture> >
                _textures;

            // Map tiles IDs to texture filenames
            std::unordered_map<int, std::string> _tiles_map;

            std::shared_ptr<pmt::BulletMgr> _bullet_mgr;

            unsigned _tanks_count;
            int _tank_turn;

            std::unique_ptr<sf::Text> _text_turn_name;
    };
}
#endif
