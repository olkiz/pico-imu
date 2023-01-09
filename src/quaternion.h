#ifndef QUATERNION_H
#define QUATERNION_H

struct Quaternion
{
    float x, y, z, a;
};

struct Quaternion add_quaternion(struct Quaternion *q1, struct Quaternion *q2);
struct Quaternion sub_quaternion(struct Quaternion *q1, struct Quaternion *q2);
struct Quaternion mul_quaternion(struct Quaternion *q1, struct Quaternion *q2);

#endif // QUATERNION_H