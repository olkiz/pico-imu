#ifndef MPU9250_REGS_H
#define MPU9250_REGS_H

#define SELF_TEST_X_GYRO    0x00
#define SELF_TEST_Y_GYRO    0x01
#define SELF_TEST_Z_GYRO    0x02

#define SELF_TEST_X_ACCEL   0x0D
#define SELF_TEST_Y_ACCEL   0x0E
#define SELF_TEST_Z_ACCEL   0x0F

#define ACCEL_XOUT_H        0x3B

#define TEMP_OUT_H          0x41

#define GYRO_XOUT_H         0x43

#define PWR_MGMT_1          0x6B

#define MPU_9250_READ_BIT   0x80

#endif // MPU9250_REGS_H