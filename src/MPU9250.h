#ifndef MPU9250_H
#define MPU9250_H

#include "hardware/spi.h"

class MPU9250 {
public:
    enum GYRO_SCALE {
        GYRO_SCALE_250DPS,
        GYRO_SCALE_500DPS,
        GYRO_SCALE_1000DPS,
        GYRO_SCALE_2000DPS
    };

    enum ACCELEROMETER_SCALE {
        ACCELEROMETER_SCALE_2G,
        ACCELEROMETER_SCALE_4G,
        ACCELEROMETER_SCALE_8G,
        ACCELEROMETER_SCALE_16G
    };

    MPU9250(spi_inst_t *spi_port, int cs_pin);

    void reset();

    void readMagRaw(int16_t mag[3]);

    void readGyro(double gyro[3]);
    void readAccelerometer(double acceleration[3]);

    void setGyroScale(GYRO_SCALE scale);
    void setAccelerometerScale(ACCELEROMETER_SCALE scale);


    int temperature();

private:
    void readAccelerometerRaw(int16_t acceleration[3]);
    void readGyroRaw(int16_t gyro[3]);

    void magInit(uint8_t scale, uint8_t mode);

    void read(uint8_t reg, uint8_t *buf, uint16_t len);
    void write(uint8_t reg, uint8_t byte);
    void slaveWrite(uint8_t i2c_addr, uint8_t reg, uint8_t byte);
    void slavePrepareToReadBytes(uint8_t i2c_addr, uint8_t reg, uint8_t num);

    void selectChip();
    void unSelectChip();

    spi_inst_t *m_pSpiPort;
    int m_CSPin;

    GYRO_SCALE m_CurrentGyroScale;
    ACCELEROMETER_SCALE m_CurrentAccScale;
};

#endif // MPU9250_H