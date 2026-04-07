#ifndef QUATERNION_H
#define QUATERNION_H

#include "euler_angles.h"

class Quaternion
{
public:
    Quaternion();
    Quaternion(double a, double x, double y, double z);
    Quaternion(const euler_angles &ea);

    Quaternion operator+(const Quaternion& other);
    Quaternion operator-(const Quaternion& other);
    Quaternion operator*(const Quaternion& other);

    euler_angles toEuler();

    double magnitude();
    void normalize();
private:
    double m_A;
    double m_X;
    double m_Y;
    double m_Z;
};

#endif // QUATERNION_H