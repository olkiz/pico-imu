#ifndef IMU_H
#define IMU_H

#include <stdint.h>
#include "quaternion.h"
#include "euler_angles.h"

struct quaternion compute_quaternion_from_gyro(double gyro[3], double dt);

#endif // IMU_H