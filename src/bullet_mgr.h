#ifndef BULLET_MGR_H
#define BULLET_MGR_H

#include <cmath>
#include <vector>
#include <unordered_map>
#include <memory>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

namespace pmt
{
    enum class WeaponType {
        Missile,
        MagnumMissile
    };

    enum class BulletType {
        Missile
    };

    class Bullet
    {
    public:
        Bullet(BulletType type, std::unique_ptr<sf::Texture>& texture);
        ~Bullet();

    private:
        std::unique_ptr<sf::Sprite> _sprite;
        bool _flying;
        BulletType _type;
    };

    class BulletMgr
    {
        public:
            BulletMgr(const BulletMgr&) = delete;
            BulletMgr& operator=(const BulletMgr&) = delete;

            BulletMgr();
            ~BulletMgr();

            void update(sf::Time delta);
            void render(sf::RenderWindow& window);
            void add_bullets(BulletType type,
                             unsigned count,
                             std::unique_ptr<sf::Texture>& texture);

        private:
            void _simulate();

            std::unordered_map
                <BulletType, std::vector<std::shared_ptr<pmt::Bullet> > >
                _bullets;
    };
}

#endif
