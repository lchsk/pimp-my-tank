#ifndef TANK_H
#define TANK_H

#include <memory>
#include <unordered_map>

#include <SFML/Graphics.hpp>

#include "bullet_mgr.h"
#include "util.h"
#include "animation.h"

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
            unsigned tank_id,
            int initial_health,
            int initial_shield,
            int cash,
            int ai_level,
            std::unordered_map<WeaponType, unsigned>& weapons,
            std::shared_ptr<pmt::BulletMgr>& bullet_mgr,
            std::unordered_map<std::string, std::unique_ptr<sf::Texture> >&
                textures,
            std::shared_ptr<sf::Font>& font,
            GameSide side, bool left, int x, int y);
        ~Tank();

        void render(sf::RenderWindow& window);
        void update(sf::Time delta);

        sf::Vector2f get_position() const;
        double get_middle_x() const;
        bool is_human() const;
        bool is_alive() const;
        bool has_turn() const;
        unsigned get_id();
        int get_cash() const;
        void gun_up();
        void gun_down();
        void shoot();
        bool can_shoot() const;
        bool check_collision(std::shared_ptr<pmt::Bullet>& bullet);
        bool check_dummy_collision(std::shared_ptr<pmt::Bullet>& bullet);

        void add_cash(unsigned amount);
        void next_weapon();
        void previous_weapon();
        void increase_shot_power(sf::Time delta);
        void init_shot();
        void spin_around();

        void activate();
        void deactivate();

        void ai_turn(std::shared_ptr<pmt::Tank>& target, double wind);
        unsigned get_ai_target() const;

        void buy(const Offer& offer);

        std::string get_weapon_name() const;
        unsigned get_weapon_count();

    private:
        void _rotate_gun(double val);

        void _render_health(sf::RenderWindow& window);
        void _render_shield(sf::RenderWindow& window);
        void _render_shot_power(sf::RenderWindow& window);
        void _hit(std::shared_ptr<pmt::Bullet>& bullet);
        void _change_color(const sf::Color color);
        void _add_shield(int value);
        void _change_view();

        std::unique_ptr<sf::Sprite> _tank;
        std::unique_ptr<sf::Sprite> _gun;
        std::unique_ptr<sf::Sprite> _excl;

        std::vector<std::unique_ptr<sf::Sprite> > _greens;
        std::vector<std::unique_ptr<sf::Sprite> > _reds;
        std::vector<std::unique_ptr<sf::Sprite> > _shields;

        std::vector<std::unique_ptr<sf::Sprite> > _green_power;
        std::vector<std::unique_ptr<sf::Sprite> > _red_power;
        std::vector<std::unique_ptr<sf::Sprite> > _orange_power;

        std::shared_ptr<pmt::BulletMgr> _bullet_mgr;

        std::shared_ptr<sf::Font> _font;
        std::unique_ptr<sf::Text> _text_tank_control;
        std::unique_ptr<sf::Text> _text_angle;

        std::unique_ptr<pmt::Animation> _explosion;

        std::shared_ptr<pmt::Bullet> _dummy_missile;
        std::shared_ptr<pmt::Bullet> _dummy_magnum;
        std::shared_ptr<pmt::Bullet> _dummy_sheep;

        GameSide _side;
        bool _left;
        bool _has_turn;

        bool _show_angle;

        int _health;
        int _shield;

        int _cash;

        int _ai_level;

        unsigned _tank_id;
        double _gun_rotation;

        double _shot_power;

        std::unordered_map<WeaponType, unsigned> _weapons;
        WeaponType _current_weapon;

        const unsigned SHOT_BAR_ROWS = 12;
        const unsigned SHOT_BAR_COLS = 6;
        const unsigned SHOT_BAR_ITEMS = SHOT_BAR_ROWS * SHOT_BAR_COLS;

        unsigned _ai_target;
    };
}

#endif
