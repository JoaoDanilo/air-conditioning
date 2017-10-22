#include <Lib/Temperature/Temperature.h>

Temperature::Temperature(tpm_TPMNumberMask TPMNumber, gpio_Pin pin,
    tpm_TPMNumberMask TPMDelay) {

  _sensor = mkl_DHT11Sensor(TPMNumber, pin);
  _delay = mkl_TPMDelay(TPMDelay);

  _sensor.doAcquisition();
  _sensor.readTemperature(&_temperature);

  _delay.setFrequency(tpm_div128);
  _delay.startDelay(10);

}

uint8_t Temperature::getProgTemp(){
  return _programTemp;
}

void Temperature::setProgTemp(uint8_t temperature){
  _programTemp = temperature;
}

void Temperature::incrementProgTemp(){
  _programTemp++;

  if(_programTemp >= 38){
    _programTemp = 38;
  }
}

void Temperature::decrementProgTemp(){
  _programTemp--;

  if(_programTemp <= 16){
    _programTemp = 16;
  }
}

void Temperature::resetProgTemp(){
  _programTemp = 24;
}

void Temperature::getTemperature() {

  if(_delay.timeoutDelay()) {
    _delay.setFrequency(tpm_div128);
    _delay.startDelay(0xFFFF);
    _counter++;
  }

  if(_counter == 20) {
    _sensor.doAcquisition();
    _sensor.readTemperature(&_temperature);
    _counter = 0;
  }
}

uint8_t Temperature::readTemperature() {
  return _temperature;
}

