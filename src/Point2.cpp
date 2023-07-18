#include "Point2.hpp"

#include <cmath>

Point2& Point2::operator=(const Point2& rhs)
{
    if (this != &rhs)
        copy(rhs);

    return *this;
}



Point2 Point2::operator+(const Point2& rhs) const
{
    Point2 result(*this);
    return result += rhs;
}



Point2 Point2::operator-(const Point2& rhs) const
{
    Point2 result(*this);
    return result -= rhs;
}



Point2 Point2::operator*(const double& rhs) const
{
    Point2 result(*this);
    return result *= rhs;
}



Point2 Point2::operator/(const double& rhs) const
{
    Point2 result(*this);
    return result /= rhs;
}



Point2& Point2::operator+=(const Point2& rhs)
{
    m_x += rhs.m_x;
    m_y += rhs.m_y;
    return *this;
}



Point2& Point2::operator-=(const Point2& rhs)
{
    m_x -= rhs.m_x;
    m_y -= rhs.m_y;
    return *this;
}



Point2& Point2::operator*=(const double& rhs)
{
    m_x *= rhs;
    m_y *= rhs;
    return *this;
}



Point2& Point2::operator/=(const double& rhs)
{
    m_x /= rhs;
    m_y /= rhs;
    return *this;
}



void Point2::copy(const Point2& rhs)
{
    m_x = rhs.m_x;
    m_y = rhs.m_y;
}



double Point2::distance_to(const Point2& rhs) const
{
    double distance = sqrt((m_x - rhs.m_x) * (m_x - rhs.m_x) + (m_y - rhs.m_y) * (m_y - rhs.m_y));
    return distance;
}



Point2 operator*(const double& lhs, const Point2& rhs)
{
    Point2 result(rhs);
    return result *= lhs;
}
