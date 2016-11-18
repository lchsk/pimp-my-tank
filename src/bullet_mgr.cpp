#include "bullet_mgr.h"
#include "util.h"

namespace pmt
{
    Bullet::Bullet() :
        _active(false)
    {
    }

    Bullet::~Bullet()
    {

    }

    BulletMgr::BulletMgr()
    {
    }

    BulletMgr::~BulletMgr()
    {
    }

    void BulletMgr::update(sf::Time delta)
    {
    }

    void BulletMgr::render(sf::RenderWindow& window)
    {
    }

    void BulletMgr::_simulate()
    {
        double g = 9.81;
        double Gamma_wind = 0.0;
        double C_air = 1.0;
        double C_wind = 50.0;
        double V_wind = 0.0;
        double mass = 100.0;

        double cosX, cosY;
        double xe, ze;
        double b, Lx, Ly;
        double sx1, vx1;
        double sy1, vy1;
        double tx1, tx2, ty1, ty2;
        static double time;

        double Vm = 100;

        double Alpha, Gamma;
        Alpha = 50;
        Gamma = 20;

        time += 0.1;

        b = 10.0 * cos(pmt::util::radian(90-Alpha) );
        Lx = b * cos(pmt::util::radian(Gamma));

        Ly = 10.0 * cos(pmt::util::radian(Alpha));

        cosX = Lx / 10;
        cosY = Ly / 10;

        xe = 10 * cos(pmt::util::radian(90-Alpha) ) * cos(pmt::util::radian(Gamma) );

        sx1 = xe; //wspolrzednia x konca lufy
        vx1 = Vm * cosX; //skadlowa vx predkosci
        sy1 = 350.0 * cos(pmt::util::radian(Alpha)); //wspolrzedna y konca lufy
        vy1 = Vm * cosY; //skladowa vy predkosci

        //obliczanie wspolrzendych pocisku
        double x, y;
        x = ( (mass/C_air) * exp(-(C_air*time)/mass) * ((-C_wind * V_wind * cos(pmt::util::radian(Gamma_wind)))/C_air - vx1) - (C_wind * V_wind * cos(pmt::util::radian(Gamma_wind)) * time)/C_air ) - ( (mass/C_air) * ((-C_wind * V_wind * cos(pmt::util::radian(Gamma_wind)))/C_air - vx1)) + sx1;
        y = (sy1 + ( -(vy1 + (mass*g)/C_air) * (mass/C_air) * exp(-(C_air*time)/mass) - (mass * g * time)/C_air) + ( (mass/C_air) * (vy1 + (mass * g)/C_air)));

    }
}
