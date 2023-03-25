#ifndef QUATERNION_H
#define QUATERNION_H

#include "euler_angles.h"

struct quaternion
{
    double x, y, z, a;
};

struct quaternion add_quaternion(struct quaternion *q1, struct quaternion *q2);
struct quaternion sub_quaternion(struct quaternion *q1, struct quaternion *q2);
struct quaternion mul_quaternion(struct quaternion *q1, struct quaternion *q2);
void normalize(struct quaternion *q);
struct quaternion euler_angles_to_quaternion(struct euler_angles *ea);
struct euler_angles quaternion_to_euler_angles(struct quaternion *q);

#endif // QUATERNION_H