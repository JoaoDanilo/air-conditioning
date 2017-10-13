#ifndef SOURCES_LIB_ONOFF_ONOFF_H_
#define SOURCES_LIB_ONOFF_ONOFF_H_

#include <Lib/GPIO/GPIO_Port/mkl_GPIOPort.h>
#include <Lib/Display/dsf_SerialDisplays.h>

class Onoff{

  public:
    Onoff(gpio_Pin pin, gpio_Pin led);

    void onOff();
    uint8_t read();
    void activeLed();
    void deactiveLed();

  private:
    mkl_GPIOPort _button;
    mkl_GPIOPort _led;

};

#endif /* SOURCES_LIB_ONOFF_ONOFF_H_ */
