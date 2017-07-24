/*!
 * \file
 * \brief Instruments for calcularing geo coordinates.
 * \author Tyulenev Sergey (marleeeeeey@gmail.com)
 *
 * last version you can download https://github.com/marleeeeeey/StdPlus
*/


#pragma once

#include "StdInclude.h"
#include "MathPlus.hpp"

namespace stdplus
{
    const double g_earthRadius_m = 6372795;
    const double g_earthRadius_km = g_earthRadius_m / 1000;

    struct EarthPoint
    {
        EarthPoint(double lat_rad = 0, double lon_rad = 0)
        {
            this->lat_rad = lat_rad;
            this->lon_rad = lon_rad;
        }

        double lat_rad = 0;
        double lon_rad = 0;
    };

    struct EarthDirection
    {
        explicit EarthDirection(double dist_km = 0,
            double az_rad = 0, double um_rad = 0)
        {
            this->dist_km = dist_km;
            this->az_rad = az_rad;
            this->um_rad = um_rad;
        }

        double dist_km = 0;
        double az_rad = 0;
        double um_rad = 0;
    };


    /*!
     * \brief Calculate geo point from source point and direction.
     * \param [in] p  Source point
     * \param [in] ed Direction
     * \return geo point = source point + direction
    */
    inline EarthPoint getEarthPoint(const EarthPoint & p, const EarthDirection & ed)
    {
        const double & dist_m = ed.dist_km * 1000;
        const double & az_rad = ed.az_rad;


        const double dSig_rad = dist_m / g_earthRadius_m;

        double cosPhi1 = cos(p.lat_rad);
        double sinPhi1 = sin(p.lat_rad);

        double cosDSig = cos(dSig_rad);
        double sinDSig = sin(dSig_rad);

        double cosAz1 = cos(az_rad);
        double sinAz1 = sin(az_rad);



        double up = sinDSig * sinAz1;
        double down = cosPhi1 * cosDSig - sinPhi1 * sinDSig * cosAz1;
        double dLambda_rad = atan2(up, down);
        double lambda2_rad = p.lon_rad + dLambda_rad;



        double sinPhi2 = sinPhi1*cosDSig + cosPhi1*sinDSig*cosAz1;
        double phi2_rad = asin(sinPhi2);

        EarthPoint p2;
        p2.lat_rad = phi2_rad;
        p2.lon_rad = lambda2_rad;
        return p2;
    }

    inline EarthDirection getEarthDirection(const EarthPoint & p1, const EarthPoint & p2)
    {
        // cos and sin lat and lon
        double cosPhi1 = cos(p1.lat_rad);
        double cosPhi2 = cos(p2.lat_rad);
        double sinPhi1 = sin(p1.lat_rad);
        double sinPhi2 = sin(p2.lat_rad);
        double dLambda = p2.lon_rad - p1.lon_rad;
        double cosDLambda = cos(dLambda);
        double sinDLambda = sin(dLambda);

        EarthDirection result;

        {
            // length big circle
            double up = sqrt(pow(cosPhi2*sinDLambda, 2) + pow(cosPhi1*sinPhi2 - sinPhi1*cosPhi2*cosDLambda, 2));
            double down = sinPhi1*sinPhi2 + cosPhi1*cosPhi2*cosDLambda;
            double ad = atan2(up, down);    // угловое расстояние
            result.dist_km = ad * g_earthRadius_km;   // метрическое расстояние
        }

        {
            // begin az
            double up = (cosPhi1*sinPhi2) - (sinPhi1*cosPhi2*cosDLambda);
            double down = sinDLambda*cosPhi2;

            double z_rad = -atan(-down / up);
            z_rad = normal_rad(z_rad);
            result.az_rad = z_rad;
        }

        return result;
    }

}


inline bool operator==(const stdplus::EarthPoint & left, const stdplus::EarthPoint & right)
{
    return left.lat_rad == right.lat_rad && left.lon_rad == right.lon_rad;
}


inline stdplus::EarthPoint operator+(const stdplus::EarthPoint & left, const stdplus::EarthPoint & right)
{
    stdplus::EarthPoint result;
    result.lat_rad = left.lat_rad + right.lat_rad;
    result.lon_rad = left.lon_rad + right.lon_rad;
    return result;
}

inline stdplus::EarthPoint operator+(const stdplus::EarthPoint & ep, const stdplus::EarthDirection & ed)
{
    return stdplus::getEarthPoint(ep, ed);
}


inline std::ostream & operator<<(std::ostream & os, const stdplus::EarthPoint & ep)
{
    os
        << "["
        << std::setprecision(5) << ep.lat_rad
        << "(" 
        << std::setprecision(5) << stdplus::rad2deg(ep.lat_rad) 
        << ")"
        << ", "
        << std::setprecision(5) << ep.lon_rad
        << "(" 
        << std::setprecision(5) << stdplus::rad2deg(ep.lon_rad) 
        << ")"
        << "]";

    return os;
}

inline std::ostream & operator<<(std::ostream & os, const stdplus::EarthDirection & ed)
{
    os
        << "["
        << " d=" 
        << std::setprecision(5) << ed.dist_km 
        << ", "
        << "az=" 
        << std::setprecision(5) << ed.az_rad
        << "("   
        << std::setprecision(5) << stdplus::rad2deg(ed.az_rad) 
        << ")"
        << "]";

    return os;
}

