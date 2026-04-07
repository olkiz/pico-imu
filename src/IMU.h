#ifndef IMU_H
#define IMU_H

#include <stdint.h>
#include "Quaternion.h"
#include "euler_angles.h"

class IMU {
public:
    explicit IMU();
    IMU(Quaternion quaternion);
    void update(double gyro[3], double dt);

    Quaternion orientation() const;

private:
    Quaternion m_Quaternion;
};

#endif // IMU_H