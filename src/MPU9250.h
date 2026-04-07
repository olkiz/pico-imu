#ifndef MPU9250_H
#define MPU9250_H

#include "hardware/spi.h"

class MPU9250 {
public:
    MPU9250(spi_inst_t *spi_port, int cs_pin);

    void reset();

    void readAccelerometerRaw(int16_t acceleration[3]);
    void readGyroRaw(int16_t gyro[3]);

    void readMagRaw(int16_t mag[3]);
    void readGyro(double gyro[3]);
    void readAccelerometer(double acceleration[3]);

    int temperature();

private:
    void magInit(uint8_t scale, uint8_t mode);

    void read(uint8_t reg, uint8_t *buf, uint16_t len);
    void write(uint8_t reg, uint8_t byte);
    void slaveWrite(uint8_t i2c_addr, uint8_t reg, uint8_t byte);
    void slavePrepareToReadBytes(uint8_t i2c_addr, uint8_t reg, uint8_t num);

    void selectChip();
    void unSelectChip();

    spi_inst_t *m_pSpiPort;
    int m_CSPin;
};

#endif // MPU9250_H