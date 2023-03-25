#include "imu.h"

struct quaternion compute_quaternion_from_gyro(double gyro[3], double dt)
{
    return (struct quaternion) {.a = 1., .x = gyro[0] * dt / 2., .y = gyro[1] * dt / 2., .z = gyro[2] * dt / 2.};
}
