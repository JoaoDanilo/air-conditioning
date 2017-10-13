#include <Lib/Onoff/Onoff.h>

Onoff::Onoff(gpio_Pin pin, gpio_Pin led){

  _button = mkl_GPIOPort(pin);
  _button.setPortMode(gpio_input);
  _button.setPullResistor(gpio_pullUpResistor);

  _led = mkl_GPIOPort(led);
  _led.setPortMode(gpio_output);
  _led.writeBit(0);
}
uint8_t Onoff::read(){
  return _button.readBit();
}

void Onoff::activeLed(){
  _led.writeBit(0);
}

void Onoff::deactiveLed(){
  _led.writeBit(1);
}

