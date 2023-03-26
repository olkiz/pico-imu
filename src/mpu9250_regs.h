#ifndef MPU9250_REGS_H
#define MPU9250_REGS_H

#define SELF_TEST_X_GYRO                0x00
#define SELF_TEST_Y_GYRO                0x01
#define SELF_TEST_Z_GYRO                0x02

#define SELF_TEST_X_ACCEL               0x0D
#define SELF_TEST_Y_ACCEL               0x0E
#define SELF_TEST_Z_ACCEL               0x0F

#define I2C_MST_CTRL                    0x24
#define I2C_SLV0_ADDR                   0x25
#define I2C_SLV0_REG                    0x26
#define I2C_SLV0_CTRL                   0x27
#define I2C_SLV4_CTRL                   0x34
#define I2C_SLV0_DO                     0x63
#define I2C_MST_DELAY_CTRL              0x67

#define ACCEL_XOUT_H                    0x3B

#define TEMP_OUT_H                      0x41

#define GYRO_XOUT_H                     0x43

#define EXT_SENS_DATA_00                0x49

#define USER_CTRL                       0x6A

#define PWR_MGMT_1                      0x6B

#define MPU_9250_READ_BIT               0x80

#define MPU_9250_ENABLE_I2C             0x80

// MAGNETOMETER
#define MAG_ADDRESS                     0x0C

#define MAG_WHOAMI_DEFAULT_VALUE        0x48

#define MAG_ST1                         0x02
#define MAG_XOUT_L                      0x03
#define MAG_CNTL1                       0x0A
#define MAG_CNTL2                       0x0B
#define MAG_ASAX                        0x10

#define MAG_14_BIT_SCALE                0x00
#define MAG_16_BIT_SCALE                0x01

#define MAG_8HZ_MODE                    0x02
#define MAG_100HZ_MODE                  0x06

#endif // MPU9250_REGS_H