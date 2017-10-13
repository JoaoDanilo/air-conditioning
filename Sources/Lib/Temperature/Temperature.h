#ifndef SOURCES_LIB_TEMPERATURE_TEMPERATURE_H_
#define SOURCES_LIB_TEMPERATURE_TEMPERATURE_H_

#include "mkl_DHT11Sensor.h"

class Temperature {
  public:
    Temperature(tpm_TPMNumberMask TPMNumber, gpio_Pin pin, tpm_TPMNumberMask TPMDelay);
    void getTemperature();
    uint8_t readTemperature();
    uint8_t getProgTemp();
    void incrementProgTemp();
    void decrementProgTemp();
    void resetProgTemp();
  private:
    mkl_DHT11Sensor _sensor;
    mkl_TPMDelay _delay;
    uint8_t _counter = 0;
    uint8_t _temperature;
    uint8_t _programTemp = 24;

};

#endif /* SOURCES_LIB_TEMPERATURE_TEMPERATURE_H_ */
