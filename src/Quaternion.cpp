#include "Quaternion.h"

#include <math.h>

Quaternion::Quaternion():
    m_A{0.0},
    m_X{0.0},
    m_Y{0.0},
    m_Z{0.0}
{

}

Quaternion::Quaternion(double a, double x, double y, double z):
    m_A{a},
    m_X{x},
    m_Y{y},
    m_Z{z}
{

}

// See: https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
Quaternion::Quaternion(const euler_angles &ea)
{
    Quaternion ans;
    double cos_of_half_roll = cos(ea.roll * .5);
    double sin_of_half_roll = sin(ea.roll * .5);
    double cos_of_half_pitch = cos(ea.pitch * .5);
    double sin_of_half_pitch = sin(ea.pitch * .5);
    double cos_of_half_yaw = cos(ea.yaw * .5);
    double sin_of_half_yaw = sin(ea.yaw * .5);

    m_A = cos_of_half_roll * cos_of_half_pitch * cos_of_half_yaw + sin_of_half_roll * sin_of_half_pitch * sin_of_half_yaw;
    m_X = sin_of_half_roll * sin_of_half_pitch * cos_of_half_yaw - cos_of_half_roll * sin_of_half_pitch * sin_of_half_yaw;
    m_Y = cos_of_half_roll * sin_of_half_roll * cos_of_half_yaw + sin_of_half_roll * cos_of_half_pitch * sin_of_half_yaw;
    m_Z = cos_of_half_roll * cos_of_half_pitch * sin_of_half_yaw - sin_of_half_roll * sin_of_half_pitch * cos_of_half_yaw;
}

Quaternion Quaternion::operator+(const Quaternion& other)
{
    Quaternion ans;
    ans.m_A = m_A + other.m_A;
    ans.m_X = m_X + other.m_X;
    ans.m_Y = m_Y + other.m_Y;
    ans.m_Z = m_Z + other.m_Z;

    return ans;
}

Quaternion Quaternion::operator-(const Quaternion& other)
{
    Quaternion ans;
    ans.m_A = m_A - other.m_A;
    ans.m_X = m_X - other.m_X;
    ans.m_Y = m_Y - other.m_Y;
    ans.m_Z = m_Z - other.m_Z;

    return ans;
}

Quaternion Quaternion::operator*(const Quaternion& other)
{
    Quaternion ans;
    ans.m_A = m_A * other.m_A - m_X * other.m_X - m_Y * other.m_Y - m_Z * other.m_Z;
    ans.m_X = m_A * other.m_X + m_X * other.m_A + m_Y * other.m_Z - m_Z * other.m_Y;
    ans.m_Y = m_A * other.m_Y - m_X * other.m_Z + m_Y * other.m_A + m_Z * other.m_X;
    ans.m_Z = m_A * other.m_Z + m_X * other.m_Y - m_Y * other.m_X + m_Z * other.m_A;
    return ans;
}

struct euler_angles Quaternion::toEuler()
{
    struct euler_angles ea;
    ea.roll = atan2(2 * (m_A * m_X + m_Y * m_Z), 1 - 2 * (m_X * m_X + m_Y * m_Y));
    ea.pitch = 2 * atan2(sqrt(1 + 2 * (m_A * m_Y - m_X * m_Z)), sqrt(1 - 2 * (m_A * m_Y - m_X * m_Z))) - M_PI_2;
    ea.yaw = atan2(2 * (m_A * m_Z + m_X * m_Y), 1 - 2 * (m_Y * m_Y + m_Z * m_Z));

    return ea;
}

double Quaternion::magnitude()
{
    return sqrt(powf(m_A, 2) + powf(m_X, 2) + powf(m_Y, 2) + powf(m_Z, 2));
}

void Quaternion::normalize()
{
    double mag = magnitude();
    m_A = m_A / mag;
    m_X = m_X / mag;
    m_Y = m_Y / mag;
    m_Z = m_Z / mag;
}