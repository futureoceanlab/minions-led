#ifndef PERIPHERAL_H
#define PERIPHERAL_H

#include "KellerLD.h"

#define I2C_BUS 1 
#define LED_EN_PIN 17
#define LED_FAULT_PIN 18
#define LED_PIN 4 //23
#define TRIG_PIN 5 //24

class Peripheral
{
public:
    Peripheral();
    Peripheral(int i2c_bus);
    
    int init();
    void triggerOn();
    void triggerOff();
    void ledOn();
    void ledOff();
    void setup();
    float getPressure();
    float getTemperature();
    void readData();

private:
    int i2c_bus = I2C_BUS;
    KellerLD *k_sensor;

    void setupPi();
    void k_sensor_init();
};



#endif