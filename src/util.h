#ifndef UTIL_H
#define UTIL_H

namespace pmt
{
    namespace util
    {
        const double PI = 3.1415926535;

        inline double radian(double a)
        {
            return a / 180.0 * PI;
        }
    }
}

#endif
