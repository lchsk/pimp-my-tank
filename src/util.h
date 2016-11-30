#ifndef UTIL_H
#define UTIL_H

#include <iomanip>
#include <random>

#include <SFML/Graphics.hpp>

namespace pmt
{
    enum class OfferType {
        Missile,
        MagnumMissile,
        Sheep,

        ShowShotAngle,
        Shield25,
        Shield50,
        Shield100,

        WeakerWind,
        StrongerWind,
        ZeroWind,

		Transparency,
        RedColor,
        BlueColor,
        PinkColor,
        NoColor
    };

    enum class WeaponType {
        Missile,
        MagnumMissile,
        Sheep
    };

    class Offer
    {
    public:
        Offer(OfferType type, std::string name, std::string desc, int price)
            : type(type),
            price(price),
            name(name),
            desc(desc) {}
        ~Offer()
        {
        }

        OfferType type;
        int price;
        std::string name;
        std::string desc;
    };

    static std::vector<Offer> offers = {
        Offer(OfferType::Missile, "Missile", "Basic tank missile", 40),
        Offer(
            OfferType::MagnumMissile,
            "Magnum missile",
            "Do more damage with a magnum missile, heavy",
            70
        ),
        Offer(
            OfferType::Sheep,
            "Sheep missile",
            "Nothing damages shields more, very light",
            90
        ),

        Offer(
            OfferType::ShowShotAngle,
            "Shot angle",
            "You will see angle value when you're preparing a shot",
            40
        ),

        Offer(
            OfferType::Shield25,
            "Shield +25",
            "Increases shield by 25 points",
            40
        ),
        Offer(
            OfferType::Shield50,
            "Shield +50",
            "Increases shield by 50 points",
            75
        ),
        Offer(
            OfferType::Shield100,
            "Shield +100",
            "Increases shield to the maximum value",
            120
        ),

        Offer(
            OfferType::WeakerWind,
            "Weaker Wind",
            "Lowers wind speed",
            75
        ),
        Offer(
            OfferType::StrongerWind,
            "Stronger Wind",
            "Increases wind speed",
            25
        ),
        Offer(
            OfferType::ZeroWind,
            "Zero Wind",
            "Resets wind to a near zero speed",
            100
        ),

        Offer(OfferType::RedColor, "Red tank color", "Paint your tank red", 20),
        Offer(OfferType::BlueColor, "Blue tank color", "Paint your tank blue", 20),
        Offer(OfferType::PinkColor, "Pink tank color", "Paint your tank pink", 20),
        Offer(
            OfferType::Transparency,
            "Transparency",
            "Tank will be semi-transparent",
            30
        ),
        Offer(OfferType::NoColor, "No tank color", "Default tank color", 0),
    };

    namespace config
    {
        const unsigned WINDOW_W = 800;
        const unsigned WINDOW_H = 448;

        const double MISSILE_SPEED = 2.5f;

        const int REWARD_BEING_HIT = 20;
        const int REWARD_HIT = 50;
        const int REWARD_TURN = 30;

        const double MIN_SHOT_POWER = 60;
        const double MAX_SHOT_POWER = 220;

        const double WIND_POWER_COEF = 30.0;

        const double WIND_MULTIPLIER = 8.0;

        const double G = 20.0;
        const double GAMMA_WIND = 180.0;
        const double C_AIR = 3.0;
        const double C_WIND = 50.0;
    }

    namespace util
    {
        const double PI = 3.14159265359;

        inline double radian(double a)
        {
            return a / 180.0 * PI;
        }

        inline double linear(
            double x,
            double target_min,
            double target_max,
            double input_min = 0,
            double input_max = 100)
        {
            return (target_max - target_min) *
                (x - input_min) / (input_max - input_min) + target_min;
        }

        inline std::string to_string_prec(double val, unsigned precision=2)
        {
            std::stringstream stream;
            stream << std::fixed << std::setprecision(precision) << val;

            return stream.str();
        }

        inline double get_random(double min, double max)
        {
            std::uniform_real_distribution<double> urd(min, max);
            std::random_device rd;
            std::mt19937 mt(rd());

            return urd(mt);
        }

        inline int get_random_int(int min, int max)
        {
            std::uniform_int_distribution<int> urd(min, max);
            std::random_device rd;
            std::mt19937 mt(rd());

            return urd(mt);
        }
    }

    namespace color
    {
        const sf::Color Blue = sf::Color(31, 58, 147, 255);
        const sf::Color Red = sf::Color(255, 41, 36, 255);
        const sf::Color Pink = sf::Color(219, 10, 91, 255);
        const sf::Color Overlay = sf::Color(0, 0, 0, 210);
        const sf::Color Transparent = sf::Color(255, 255, 255, 100);
    }
}

#endif
