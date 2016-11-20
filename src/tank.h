#ifndef TANK_H
#define TANK_H

#include <memory>
#include <unordered_map>

#include <SFML/Graphics.hpp>

#include "bullet_mgr.h"

namespace pmt
{
    const double ROTATION_UNIT = 0.5f;

    enum class GameSide {
        Human,
        Computer
    };

    class Tank
    {
    public:
        Tank(
            std::shared_ptr<pmt::BulletMgr>& bullet_mgr,
            std::unique_ptr<sf::Texture>& tank,
            std::unique_ptr<sf::Texture>& gun,
            std::unique_ptr<sf::Texture>& green,
            std::unique_ptr<sf::Texture>& red,
            std::unique_ptr<sf::Texture>& shield,
            GameSide side, bool left, int x, int y);
        ~Tank();

        void render(sf::RenderWindow& window);

        bool is_human() const;
        void gun_up();
        void gun_down();
        void shoot();

    private:
        void _rotate_gun(double val);
        void _render_health(sf::RenderWindow& window);
        void _render_shield(sf::RenderWindow& window);

        std::unique_ptr<sf::Sprite> _tank;
        std::unique_ptr<sf::Sprite> _gun;
        std::vector<std::unique_ptr<sf::Sprite> > _greens;
        std::vector<std::unique_ptr<sf::Sprite> > _reds;
        std::vector<std::unique_ptr<sf::Sprite> > _shields;
        GameSide _side;
        bool _left;

        int _health;
        int _shield;

        std::shared_ptr<pmt::BulletMgr> _bullet_mgr;

        double _gun_rotation;

        std::unordered_map<WeaponType, unsigned> _weapons;
        WeaponType _current_weapon;
    };
}

#endif
