#include <iostream>

#include "tank.h"
#include "util.h"

namespace pmt
{
    Tank::Tank(
        unsigned tank_id,
        int initial_health,
        int initial_shield,
        int cash,
        std::unordered_map<WeaponType, unsigned>& weapons,
        std::shared_ptr<pmt::BulletMgr>& bullet_mgr,
        std::unordered_map<std::string, std::unique_ptr<sf::Texture> >&
        textures,
        std::shared_ptr<sf::Font>& font,
        GameSide side, bool left, int x, int y)
        : _side(side),
          _left(left),
          _has_turn(false),
          _show_angle(false),
          _health(initial_health),
          _shield(initial_shield),
          _cash(cash),
          _tank_id(tank_id),
          _gun_rotation(7.0f),
          _shot_power(-1.0f),
          _current_weapon(WeaponType::Missile)
    {
        _tank = std::make_unique<sf::Sprite>(*textures["tank.png"].get());
        _gun = std::make_unique<sf::Sprite>(*textures["gun.png"].get());
        _excl = std::make_unique<sf::Sprite>(*textures["excl.png"].get());

        _dummy_missile = std::make_shared<pmt::Bullet>(
            WeaponType::Missile,
            textures["missile.png"]);

        _dummy_magnum = std::make_shared<pmt::Bullet>(
            WeaponType::Missile,
            textures["magnum.png"]);

        _dummy_sheep = std::make_shared<pmt::Bullet>(
            WeaponType::Missile,
            textures["sheep.png"]);

        _bullet_mgr = bullet_mgr;
        _font = font;

        _explosion = std::make_unique<pmt::Animation>(
            textures["explosion.png"], 12, 96, 96);

        _tank->setPosition(x, y);

        _text_angle = std::make_unique<sf::Text>();
        _text_angle->setFont(*_font);
        _text_angle->setCharacterSize(16);
        _text_angle->setColor(pmt::color::Blue);
        _text_angle->setPosition(2, 32);

        _text_tank_control = std::make_unique<sf::Text>();
        _text_tank_control->setString(is_human() ? "Human" : "AI");
        _text_tank_control->setFont(*_font);
        _text_tank_control->setCharacterSize(8);

        for (int i = 0; i < 10; i++) {
            _greens.push_back(
                std::make_unique<sf::Sprite>(*textures["green.png"].get()));
            _reds.push_back(
                std::make_unique<sf::Sprite>(*textures["red.png"].get()));
            _shields.push_back(
                std::make_unique<sf::Sprite>(*textures["shield.png"].get()));
        }

        _change_view();

        for (unsigned row = 0; row < SHOT_BAR_ROWS; row ++) {
            for (unsigned col = 0; col < SHOT_BAR_COLS; col++) {
                _green_power.push_back(
                    std::make_unique<sf::Sprite>(
                        (row >= 0 && row < 4)
                        ? *textures["green_rect.png"].get()
                        : (row >= 4 && row < 8)
                        ? *textures["orange_rect.png"].get()
                        : *textures["red_rect.png"].get()
                        )
                    );

                _green_power[row * SHOT_BAR_COLS + col]->setPosition(
                    10 + col * 6, pmt::config::WINDOW_H / 2.0 - row * 6);
            }
        }

        // Set initial gun rotation
        _rotate_gun(0);

        // Initial weapons
        for (auto it = weapons.begin(); it != weapons.end(); it++)
            _weapons[it->first] = it->second;
    }

    std::string Tank::get_weapon_name() const
    {
        return _bullet_mgr->get_weapon_name(_current_weapon);
    }

    unsigned Tank::get_weapon_count()
    {
        return _weapons[_current_weapon];
    }

    bool Tank::is_human() const
    {
        return _side == GameSide::Human;
    }

    bool Tank::is_alive() const
    {
        return _health > 0;
    }

    bool Tank::has_turn() const
    {
        return _has_turn;
    }

    bool Tank::can_shoot() const
    {
        return _shot_power > 0;
    }

    unsigned Tank::get_id()
    {
        return _tank_id;
    }

    int Tank::get_cash() const
    {
        return _cash;
    }

    void Tank::gun_up()
    {
        _rotate_gun(ROTATION_UNIT);
    }

    void Tank::gun_down()
    {
        _rotate_gun(-ROTATION_UNIT);
    }

    void Tank::activate()
    {
        _cash += pmt::config::REWARD_TURN;
        _has_turn = true;
    }

    void Tank::deactivate()
    {
        _has_turn = false;
    }

    void Tank::add_cash(unsigned amount)
    {
        _cash += amount;
    }

    void Tank::buy(const Offer& offer)
    {
        _cash -= offer.price;

        switch (offer.type) {
        case OfferType::Missile:
            _weapons[WeaponType::Missile] += 1;
            _current_weapon = WeaponType::Missile;
            break;
        case OfferType::MagnumMissile:
            _weapons[WeaponType::MagnumMissile] += 1;
            _current_weapon = WeaponType::MagnumMissile;
            break;
        case OfferType::Sheep:
            _weapons[WeaponType::Sheep] += 1;
            _current_weapon = WeaponType::Sheep;
            break;

        case OfferType::Transparency:
            _change_color(pmt::color::Transparent);
            break;
        case OfferType::RedColor:
            _change_color(sf::Color::Red);
            break;
        case OfferType::BlueColor:
            _change_color(sf::Color::Blue);
            break;
        case OfferType::PinkColor:
            _change_color(pmt::color::Pink);
            break;
        case OfferType::NoColor:
            _change_color(sf::Color::White);
            break;

        case OfferType::Shield25:
            _add_shield(25);
            break;
        case OfferType::Shield50:
            _add_shield(50);
            break;
        case OfferType::Shield100:
            _add_shield(100);
            break;

        case OfferType::ZeroWind:
            _bullet_mgr->set_wind(0.01 * _bullet_mgr->get_wind());
            break;
        case OfferType::WeakerWind:
            _bullet_mgr->set_wind(_bullet_mgr->get_wind() / 3.0);
            break;

        case OfferType::StrongerWind:
            _bullet_mgr->set_wind(_bullet_mgr->get_wind() * 3.0);
            break;

        case OfferType::ShotPower:
        case OfferType::ShowShotAngle:
            _show_angle = true;
            break;
        case OfferType::Crosshairs:
            // TODO
            break;

        }
    }

    void Tank::next_weapon()
    {
        _shot_power = -1;

        std::vector<WeaponType>::iterator it = std::find(
            pmt::WeaponsOrder.begin(),
            pmt::WeaponsOrder.end(),
            _current_weapon);

        while (++it != pmt::WeaponsOrder.end()) {
            if (_weapons[*it] > 0) {
                _current_weapon = *it;

                break;
            }
        }
    }

    void Tank::previous_weapon()
    {
        _shot_power = -1;

        std::vector<WeaponType>::reverse_iterator it = std::find(
            pmt::WeaponsOrder.rbegin(),
            pmt::WeaponsOrder.rend(),
            _current_weapon);

        while (++it != pmt::WeaponsOrder.rend()) {
            if (_weapons[*it] > 0) {
                _current_weapon = *it;

                break;
            }
        }
    }

    void Tank::shoot()
    {
        switch(_current_weapon) {
        case WeaponType::Missile:
        case WeaponType::Sheep:
        case WeaponType::MagnumMissile:
            if (_weapons[_current_weapon] > 0) {
                _weapons[_current_weapon]--;

                double power = pmt::util::linear(_shot_power, 60, 140);

                std::cout << "Shot Power, p:" << _shot_power << " l: " << power << "\n";
                std::cout << "Shot Angle: " << _gun_rotation << "\n";

                _bullet_mgr->shoot(
                    _tank_id,
                    _left,
                    _current_weapon,
                    _gun_rotation,
                    power,
                    _gun->getPosition().x,
                    _gun->getPosition().y);

                _shot_power = -1.0f;
            }

            break;
        }
    }

    Tank::~Tank()
    {
    }

    void Tank::ai_turn(std::shared_ptr<pmt::Tank>& enemy, double wind)
    {
        sf::Vector2f enemy_pos = enemy->get_position();
        sf::Vector2f my_pos = get_position();

        bool enemy_on_the_left = enemy_pos.x < my_pos.x;

        if (enemy_on_the_left != _left) {
            spin_around();
        }

        // Change weapon
        for (unsigned i = 0; i < pmt::WeaponsOrder.size(); i++) {
            // double r = pmt::util::get_random(0, 1);

            // if (r > 0.5)
            // if (_weapons[pmt::WeaponsOrder[i]] > 0)
                // _current_weapon = pmt::WeaponsOrder[i];
        }

        _current_weapon = pmt::WeaponType::Missile;

        std::shared_ptr<pmt::Bullet> dummy_bullet = _dummy_missile;

        // Buy

        _ai_target = enemy->get_id();

        bool found = false;
        double shot_angle = 1;
        double shot_power = 1;
        double power;

        sf::Vector2f pos = _gun->getPosition();

        for (double power_base = 5; power_base <= 100; power_base++) {
            power = pmt::util::linear(
                power_base,
                pmt::config::MIN_SHOT_POWER,
                pmt::config::MAX_SHOT_POWER
            );

            for (int angle = -15; angle < 85; angle++) {
                sf::Time delta = sf::milliseconds(20);

                dummy_bullet->hit();
                dummy_bullet->shoot(
                    get_id(),
                    enemy_on_the_left,
                    -angle,
                    power,
                    pos.x,
                    pmt::config::WINDOW_H - pos.y
                );

                for (unsigned i = 0; i < 100; i++) {
                    dummy_bullet->simulate(delta, wind);

                    if (! dummy_bullet->on_screen())
                        break;

                    if (enemy->check_dummy_collision(dummy_bullet)) {
                        found = true;
                        shot_power = power_base + pmt::util::get_random(-10, 10);
                        shot_angle = angle;

                        break;
                    }
                }

                if (found)
                    break;
            }

            if (found)
                break;
        }

        std::cout << "FOUND: " << found << "\n";

        if (! found) {
            shot_power = pmt::util::get_random(10, 100);
            shot_angle = pmt::util::get_random(1, 80);
        }

        _shot_power = shot_power;
        _gun_rotation = shot_angle;
        _rotate_gun(0);

        shoot();
    }

    unsigned Tank::get_ai_target() const
    {
        return _ai_target;
    }

    void Tank::update(sf::Time delta)
    {
        _explosion->update(delta);
    }

    void Tank::render(sf::RenderWindow& window)
    {
        _explosion->render(window);
        // _dummy_bullet->render(window);

        if (! is_alive())
            return;

        window.draw(*_tank);
        window.draw(*_gun);

        if (_has_turn) {
            window.draw(*_excl);
            window.draw(*_text_tank_control);

            if (_show_angle)
                window.draw(*_text_angle);
        }

        _render_health(window);
        _render_shield(window);
        _render_shot_power(window);
    }

    bool Tank::check_dummy_collision(std::shared_ptr<pmt::Bullet>& bullet)
    {
        return _tank->getGlobalBounds().intersects(
            bullet->get_sprite()->getGlobalBounds()) && _tank_id != bullet->get_origin_tank();
    }

    bool Tank::check_collision(std::shared_ptr<pmt::Bullet>& bullet)
    {
        if (bullet->is_flying()
            && _tank->getGlobalBounds().intersects(
                bullet->get_sprite()->getGlobalBounds())
            // Do not hit yourself
            && _tank_id != bullet->get_origin_tank()) {
            _hit(bullet);

            // Cash for being hit
            _cash += pmt::config::REWARD_BEING_HIT;

            sf::Vector2f pos = get_position();
            double scale = pmt::util::get_random(0.6, 0.9);
            _explosion->set_scale(scale, scale);

            sf::Vector2f expl_size = _explosion->get_size();

            _explosion->set_position(
                pos.x + get_middle_x() - expl_size.x / 2,
                pos.y - expl_size.y / 2);
            _explosion->play();

            return true;
        }

        return false;
    }

    void Tank::init_shot()
    {
        _shot_power = 1.0f;
    }

    void Tank::increase_shot_power(sf::Time delta)
    {
        if (_weapons[_current_weapon] <= 0)
            return;

        _shot_power += (delta.asMilliseconds() / 5.0);

        if (_shot_power > 100)
            _shot_power = 100.0f;
    }

    void Tank::spin_around()
    {
        _left = !_left;
        _change_view();
    }

    sf::Vector2f Tank::get_position() const
    {
        return _tank->getPosition();
    }

    double Tank::get_middle_x() const
    {
        double tank_width = _tank->getTexture()->getSize().x
            * _tank->getScale().x;

        return tank_width / 2;
    }

    void Tank::_add_shield(int value)
    {
        _shield += value;

        if (_shield > 100)
            _shield = 100;
    }

    void Tank::_change_color(const sf::Color color)
    {
        _tank->setColor(color);
        _gun->setColor(color);
    }

    void Tank::_hit(std::shared_ptr<pmt::Bullet>& bullet)
    {
        bullet->hit();

        switch(bullet->get_type()) {
        case WeaponType::Missile:
            if (_shield > 0)
                _shield -= 15;

            if (_shield <= 0)
                _health -= 10;

            break;

        case WeaponType::MagnumMissile:
            if (_shield > 0) {
                _shield -= 20;
                _health -= 15;
            }

            if (_shield <= 0)
                _health -= 25;

            break;

        case WeaponType::Sheep:
            if (_shield > 0) {
                _shield -= 50;
                _health -= 10;
            }

            if (_shield <= 0)
                _health -= 20;

            break;
        }
    }

    void Tank::_render_health(sf::RenderWindow& window)
    {
        int g_cnt = ceil(_health / 10.0);

        for (int i = 0; i < g_cnt; i++)
            window.draw(*_greens[i]);

        for (int i = g_cnt; i < 10; i++)
            window.draw(*_reds[i]);
    }

    void Tank::_render_shield(sf::RenderWindow& window)
    {
        int g_sh = ceil(_shield / 10.0);

        for (int i = 0; i < g_sh; i++)
            window.draw(*_shields[i]);
    }

    void Tank::_render_shot_power(sf::RenderWindow& window)
    {
        if (_shot_power > 0) {
            int items = ceil(pmt::util::linear(_shot_power, 0, SHOT_BAR_ITEMS));

            items = items - (items % SHOT_BAR_COLS);

            for (int i = 0; i < items; i++)
                window.draw(*_green_power[i]);
        }
    }

    void Tank::_rotate_gun(double val)
    {
        int next_val = static_cast<int>(_gun_rotation + val) % 360;

        if (((next_val >= -15 && next_val < 85)))
            _gun_rotation += val;

        _text_angle->setString(pmt::util::to_string_prec((_gun_rotation), 1) + "*");

        _gun->setRotation(_left ? _gun_rotation : -_gun_rotation);
    }

    void Tank::_change_view()
    {
        int scale = _left ? -1 : 1;
        int tank_width = _tank->getTexture()->getSize().x * scale;

        double x, y;
        const int health_diff = -7;
        const int shield_diff = -10;

        y = _tank->getPosition().y;

        _tank->setPosition(_tank->getPosition().x - tank_width, y);

        x = _tank->getPosition().x;

        const int tank_middle_x = x + tank_width / 2 - 10;

        _excl->setPosition(tank_middle_x, y - 30);
        _text_tank_control->setPosition(tank_middle_x + 24, y - 23);

        for (int i = 0; i < 10; i++) {
            _greens[i]->setPosition(tank_middle_x + i * 2, y + health_diff);
            _reds[i]->setPosition(tank_middle_x + i * 2, y + health_diff);
            _shields[i]->setPosition(tank_middle_x + i * 2, y + shield_diff);
        }

        _tank->setScale(scale, 1);
        _gun->setScale(scale, 1);
        _gun->setPosition(x + scale * 28, y + 5);
        _gun->setRotation(-scale * _gun_rotation);
    }
}
