#include "quaternion.h"

#include <math.h>

struct Quaternion add_quaternion(struct Quaternion *q1, struct Quaternion *q2)
{
    struct Quaternion ans;
    ans.a = q1->a + q1->a;
    ans.x = q1->x + q1->x;
    ans.y = q1->y + q1->y;
    ans.z = q1->z + q1->z;

    return ans;
}

struct Quaternion sub_quaternion(struct Quaternion *q1, struct Quaternion *q2)
{
    struct Quaternion ans;
    ans.a = q1->a - q1->a;
    ans.x = q1->x - q1->x;
    ans.y = q1->y - q1->y;
    ans.z = q1->z - q1->z;

    return ans;
}

struct Quaternion mul_quaternion(struct Quaternion *q1, struct Quaternion *q2)
{
    struct Quaternion ans;
    ans.a = q1->a * q2->a - q1->x * q2->x - q1->y * q2->y - q1->z * q2->z;
    ans.x = q1->a * q2->x + q1->x * q2->a + q1->y * q2->z - q1->z * q2->y;
    ans.y = q1->a * q2->y - q1->x * q2->z + q1->y * q2->a + q1->z * q2->x;
    ans.z = q1->a * q2->z + q1->x * q2->y - q1->y * q2->x + q1->z * q2->a;
    return ans;
}

float magnitude(struct Quaternion *q)
{
    return sqrt(powf(q->a, 2) + powf(q->x, 2) + powf(q->y, 2) + powf(q->z, 2));
}
