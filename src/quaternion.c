#include "quaternion.h"

#include <math.h>

struct quaternion add_quaternion(struct quaternion *q1, struct quaternion *q2)
{
    struct quaternion ans;
    ans.a = q1->a + q1->a;
    ans.x = q1->x + q1->x;
    ans.y = q1->y + q1->y;
    ans.z = q1->z + q1->z;

    return ans;
}

struct quaternion sub_quaternion(struct quaternion *q1, struct quaternion *q2)
{
    struct quaternion ans;
    ans.a = q1->a - q1->a;
    ans.x = q1->x - q1->x;
    ans.y = q1->y - q1->y;
    ans.z = q1->z - q1->z;

    return ans;
}

struct quaternion mul_quaternion(struct quaternion *q1, struct quaternion *q2)
{
    struct quaternion ans;
    ans.a = q1->a * q2->a - q1->x * q2->x - q1->y * q2->y - q1->z * q2->z;
    ans.x = q1->a * q2->x + q1->x * q2->a + q1->y * q2->z - q1->z * q2->y;
    ans.y = q1->a * q2->y - q1->x * q2->z + q1->y * q2->a + q1->z * q2->x;
    ans.z = q1->a * q2->z + q1->x * q2->y - q1->y * q2->x + q1->z * q2->a;
    return ans;
}

float magnitude(struct quaternion *q)
{
    return sqrt(powf(q->a, 2) + powf(q->x, 2) + powf(q->y, 2) + powf(q->z, 2));
}

void normalize(struct quaternion *q)
{
    double mag = magnitude(q);
    q->a = q->a / mag;
    q->x = q->x / mag;
    q->y = q->y / mag;
    q->z = q->z / mag;
}

// See: https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
struct quaternion euler_angles_to_quaternion(struct euler_angles *ea)
{
    struct quaternion ans;
    double cos_of_half_roll = cos(ea->roll * .5);
    double sin_of_half_roll = sin(ea->roll * .5);
    double cos_of_half_pitch = cos(ea->pitch * .5);
    double sin_of_half_pitch = sin(ea->pitch * .5);
    double cos_of_half_yaw = cos(ea->yaw * .5);
    double sin_of_half_yaw = sin(ea->yaw * .5);

    ans.a = cos_of_half_roll * cos_of_half_pitch * cos_of_half_yaw + sin_of_half_roll * sin_of_half_pitch * sin_of_half_yaw;
    ans.x = sin_of_half_roll * sin_of_half_pitch * cos_of_half_yaw - cos_of_half_roll * sin_of_half_pitch * sin_of_half_yaw;
    ans.y = cos_of_half_roll * sin_of_half_roll * cos_of_half_yaw + sin_of_half_roll * cos_of_half_pitch * sin_of_half_yaw;
    ans.z = cos_of_half_roll * cos_of_half_pitch * sin_of_half_yaw - sin_of_half_roll * sin_of_half_pitch * cos_of_half_yaw;

    return ans;
}

struct euler_angles quaternion_to_euler_angles(struct quaternion *q)
{
    struct euler_angles ea;
    ea.roll = atan2(2 * (q->a * q->x + q->y * q->z), 1 - 2 * (q->x * q->x + q->y * q->y));
    ea.pitch = 2 * atan2(sqrt(1 + 2 * (q->a * q->y - q->x * q->z)), sqrt(1 - 2 * (q->a * q->y - q->x * q->z))) - M_PI_2;
    ea.yaw = atan2(2 * (q->a * q->z + q->x * q->y), 1 - 2 * (q->y * q->y + q->z * q->z));

    return ea;
}