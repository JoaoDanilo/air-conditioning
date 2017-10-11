#include <Lib/Ventilation/ventilation.h>
#include <Lib/TPM/TPM_PWM/mkl_TPMPulseWidthModulation.h>

Ventilation::Ventilation(tpm_Pin pin) {

   //instancia do pwm
  _pwm = mkl_TPMPulseWidthModulation(pin);

}

void Ventilation::setFreq(int speed) {
  _pwm.setFrequency(tpm_div128, 799);
  _pwm.setDutyCycle(speed);
}

void Ventilation::increment() {

  _currentSpeed += 300;
  if(_currentSpeed > 900)
    _currentSpeed = 300;

  setFreq(_currentSpeed);

}

void Ventilation::stop(){
  _currentSpeed = 0;
  setFreq(_currentSpeed);
}

void Ventilation::start(){
  _pwm.enableOperation();
}

void Ventilation::restart(){
  _currentSpeed = 300;
  setFreq(_currentSpeed);
}

uint16_t Ventilation::getSpeed() {
  return _currentSpeed;
}
