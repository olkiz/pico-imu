#include "IMU.h"

IMU::IMU()
{

}

IMU::IMU(Quaternion quaternion):
    m_Quaternion{quaternion}
{
    
}

void IMU::update(double gyro[3], double dt)
{
    m_Quaternion = m_Quaternion * Quaternion{1., gyro[0] * dt / 2., gyro[1] * dt / 2., gyro[2] * dt / 2.};
    m_Quaternion.normalize();
}

Quaternion IMU::orientation() const
{
    return m_Quaternion;
}