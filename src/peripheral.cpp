#include "peripheral.h"
#include <iostream>

#include <wiringPi.h>


Peripheral::Peripheral(){}


Peripheral::Peripheral(int bus)
{
    i2c_bus = bus;
}

int Peripheral::init()
{
    // WiringPI and GPIO
    setupPi();
    k_sensor_init();
}

void Peripheral::k_sensor_init()
{
    k_sensor = new KellerLD(i2c_bus);
    k_sensor->init();
    if (k_sensor->isInitialized())
    {
        std::cout << "Sensor isInitialized\n" << std::endl;
    }
    else
    {
        std::cout << "Sensor NOT connected\n" << std::endl;
    }
}

void Peripheral::setupPi()
{
    wiringPiSetup();

    pinMode(LED_FAULT_PIN, INPUT);

    pinMode(LED_PIN, OUTPUT);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(LED_EN_PIN, OUTPUT);
    digitalWrite(LED_EN_PIN, LOW);
}


void Peripheral::triggerOn() 
{
    digitalWrite(TRIG_PIN, HIGH);
}


void Peripheral::triggerOff()
{
    digitalWrite(TRIG_PIN, LOW);
}


void Peripheral::ledOn()
{
    digitalWrite(LED_PIN, HIGH);
}


void Peripheral::ledOff()
{
    digitalWrite(LED_PIN, LOW);
}


void Peripheral::readData()
{
    k_sensor->readData();
}


float Peripheral::getPressure()
{
    return k_sensor->pressure();
}


float Peripheral::getTemperature()
{
    return k_sensor->temperature();
}

