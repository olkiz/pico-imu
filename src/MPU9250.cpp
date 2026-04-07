#include "MPU9250.h"
#include "mpu9250_regs.h"

#include "pico/stdlib.h"

MPU9250::MPU9250(spi_inst_t *spi_port, int cs_pin):
    m_pSpiPort{spi_port},
    m_CSPin{cs_pin}
{
    write(USER_CTRL, 0x20);
    write(I2C_MST_CTRL, 0x1D);
    write(I2C_MST_DELAY_CTRL, 0x81);
    write(I2C_SLV4_CTRL, 0x01);

    write(PWR_MGMT_1, 0x00);
    sleep_ms(100);
    write(PWR_MGMT_1, 0x01);
    sleep_ms(200);

    magInit(MAG_16_BIT_SCALE, MAG_100HZ_MODE);
}

void MPU9250::readGyro(double gyroRads[3])
{
    int16_t gyro_raw[3];
    readGyroRaw(gyro_raw);
    gyroRads[0] = gyro_raw[0] * 0.000133061;
    gyroRads[1] = gyro_raw[1] * 0.000133061;
    gyroRads[2] = gyro_raw[2] * 0.000133061;
}


void MPU9250::readAccelerometer(double acceleration[3])
{
    int16_t acceleration_raw[3];
    readAccelerometerRaw(acceleration_raw);
    acceleration[0] = acceleration_raw[0] / 16384.0 * 9.80665;
    acceleration[1] = acceleration_raw[1] / 16384.0 * 9.80665;
    acceleration[2] = acceleration_raw[2] / 16384.0 * 9.80665;
}

void MPU9250::readAccelerometerRaw(int16_t acceleration[3])
{
    uint8_t buf[6];

    read(ACCEL_XOUT_H, buf, 6);

    for (int i = 0; i < 3; i++)
    {
        acceleration[i] = ((int16_t)buf[i * 2] << 8 | buf[(i * 2) + 1]);
    }
}

void MPU9250::readGyroRaw(int16_t gyro[3])
{
    uint8_t buf[6];

    read(GYRO_XOUT_H, buf, 6);

    for (int i = 0; i < 3; i++)
    {
        gyro[i] = ((int16_t)buf[i * 2] << 8 | buf[(i * 2) + 1]);
    }
}

void MPU9250::slavePrepareToReadBytes(uint8_t i2c_addr, uint8_t reg, uint8_t num)
{
    write(I2C_SLV0_ADDR, i2c_addr | MPU_9250_READ_BIT);
    write(I2C_SLV0_REG, reg);
    write(I2C_SLV0_CTRL, MPU_9250_ENABLE_I2C | num);
}

void MPU9250::slaveWrite(uint8_t i2c_addr, uint8_t reg, uint8_t byte)
{
    write(I2C_SLV0_ADDR, i2c_addr);
    write(I2C_SLV0_REG, reg);
    write(I2C_SLV0_DO, byte);
    write(I2C_SLV0_CTRL, MPU_9250_ENABLE_I2C | 0x01);
}

void MPU9250::readMagRaw(int16_t mag[3])
{
    uint8_t buf[7];
    uint8_t data_ready;
    slavePrepareToReadBytes(MAG_ADDRESS, MAG_ST1, 1);
    sleep_ms(2);
    read(EXT_SENS_DATA_00, &data_ready, 1);

    slavePrepareToReadBytes(MAG_ADDRESS, MAG_XOUT_L, 7);
    sleep_ms(2);

    read(EXT_SENS_DATA_00, buf, 7);

    bool is_overflow_set = buf[6] & 0x08;

    if (!is_overflow_set)
    {
        for (int i = 0; i < 3; i++)
        {
            mag[i] = ((int16_t)buf[i * 2 + 1] << 8 | buf[(i * 2)]);
        }
    }
}

void MPU9250::magInit(uint8_t scale, uint8_t mode)
{
    uint8_t calibration_data[3];
    slaveWrite(MAG_ADDRESS, MAG_CNTL2, 0x01); // Mag reset
    sleep_ms(100);
    slaveWrite(MAG_ADDRESS, MAG_CNTL1, 0x00); // Mag power down
    sleep_ms(100);
    slaveWrite(MAG_ADDRESS, MAG_CNTL1, 0x0F); // Fuze ROM
    sleep_ms(100);

    slavePrepareToReadBytes(MAG_ADDRESS, MAG_ASAX, 3);
    sleep_ms(100);
    read(EXT_SENS_DATA_00, calibration_data, 3); // Read calibration data

    // m_CalibrationData[0] = calibration_data[0];
    // m_CalibrationData[1] = calibration_data[1];
    // m_CalibrationData[2] = calibration_data[2];

    slaveWrite(MAG_ADDRESS, MAG_CNTL1, 0x00); // Mag power down
    sleep_ms(100);
    slaveWrite(MAG_ADDRESS, MAG_CNTL1, scale << 4 | mode); // Setting resolution and sample ODR
    sleep_ms(100);
}

void MPU9250::selectChip()
{
    gpio_put(m_CSPin, 0);
}

void MPU9250::unSelectChip()
{
    gpio_put(m_CSPin, 1);
}

void MPU9250::reset()
{
    uint8_t buf[] = {PWR_MGMT_1, 0x00};
    selectChip();
    spi_write_blocking(m_pSpiPort, buf, 2);
    unSelectChip();
}

void MPU9250::read(uint8_t reg, uint8_t *buf, uint16_t len)
{
    reg |= MPU_9250_READ_BIT;
    selectChip();
    spi_write_blocking(m_pSpiPort, &reg, 1);
    spi_read_blocking(m_pSpiPort, 0, buf, len);
    unSelectChip();
}

void MPU9250::write(uint8_t reg, uint8_t byte)
{
    selectChip();
    spi_write_blocking(m_pSpiPort, &reg, 1);
    spi_write_blocking(m_pSpiPort, &byte, 1);
    unSelectChip();
}

int MPU9250::temperature()
{
    uint8_t buf[2];
    read(TEMP_OUT_H, buf, 2);
    return buf[0] << 8 | buf[1];
}
