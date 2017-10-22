#include <MKL25Z4.h>
#include <Lib/Display/dsf_SerialDisplays.h>
#include <Lib/Multiplexer/Multiplexer.h>
#include <Lib/TPM/TPM_Delay/mkl_TPMDelay.h>
#include <Lib/Onoff/Onoff.h>
#include <Lib/MinuteTimer/MinuteTimer.h>
#include <Lib/FrequencyDivider/FrequencyDivider.h>
#include <Lib/RemoteControl/mkl_RemoteControl.h>
#include <Lib/Ventilation/ventilation.h>
#include <Lib/Temperature/Temperature.h>
#include <Lib/MonoEstavel/MonoEstavel.h>
#include <Lib/Control/ControlCompressor.h>

dsf_SerialDisplays display(gpio_PTE29, gpio_PTE22, gpio_PTE23);
Onoff onoffButton(gpio_PTE5, gpio_PTB19);
MinuteTimer timer(gpio_PTC8, gpio_PTC9, gpio_PTD1);
Multiplexer multiplex;
FrequencyDivider freqDiv(100, PIT_Ch0);
mkl_RemoteControl remoteControl(gpio_PTA2, PIT_Ch1, gpio_PTB3, gpio_PTC2);
Temperature temperature(tpm_TPM0, gpio_PTE20, tpm_TPM0);
Ventilation ventilator(tpm_PTB0);
MonoEstavel monoEstavel(tpm_TPM1);
ControlCompressor control;

mkl_GPIOPort compressorLed(gpio_PTB1);

//---------------  Testes    ------------------------

uint8_t nibble1;
uint8_t nibble2;
//--------------- FIM TESTES ------------------------

enum STATE {
  STATE0, STATE1, STATE2
};

extern "C" {

  void PORTA_IRQHandler() {
    remoteControl.getBitStreamIR();
  }

  void PIT_IRQHandler(void) {
    remoteControl.incrementTime();
    freqDiv.clockDiv(&timer);
  }
}

int hexToDecimal(uint8_t hex){
  switch(hex){
    case 0x68:
      return 0;
    case 0x30:
      return 1;
    case 0x18:
      return 2;
    case 0x7A:
      return 3;
    case 0x10:
      return 4;
    case 0x5A:
      return 6;
    case 0x42:
      return 7;
    case 0x4A:
      return 8;
    case 0x52:
      return 9;
    default:
      return 10;
  }
  return 10;
}

int main(void) {

  STATE s = STATE0;
  display.clearDisplays();

  compressorLed.setPortMode(gpio_output);
  compressorLed.writeBit(0);

  volatile uint8_t ir;
  bool showDisplay;
  uint8_t resultMultiplex;
  bool flag = true;

  while (true) {

    ir = 5;
    display.updateDisplays();
    showDisplay = timer.getSleepTime() > 0 ? 0 : 1;

    resultMultiplex = multiplex.select(showDisplay, timer.getSleepTime(), temperature.getProgTemp());

    monoEstavel.startTimer();
    temperature.getTemperature();

    control.controlCompressor(temperature.readTemperature(), temperature.getProgTemp());

    monoEstavel.detectEdge(control.getSignal());

    if(remoteControl.commandAvailable()) {

      ir = remoteControl.readCommand();
      Exception_t error = remoteControl.waitCommand();

      int digitoIR = hexToDecimal(ir);

      if (digitoIR < 10){
        nibble1 = (timer.getSleepTime() % 10) << 4;
        nibble2 = (nibble1 | digitoIR);

        if (nibble2 != 0){
          timer.setTimer(nibble2);
        }
        else{
          s = STATE1;
          timer.setTimer(0);
        }

        nibble1 = 0;
        nibble2 = 0;
      }

      if(error != mkl_ok) {

        if(error == mkl_parityError) {
          remoteControl.ledParityError();
        }
        else if(error == mkl_overWrite) {
          remoteControl.ledOverWriteError();
        }
      }
    }

    switch (s) {
      case STATE0:
        onoffButton.activeLed();
        timer.deactiveLed();
        compressorLed.writeBit(0);
        display.clearDisplays();
        timer.reset();
        freqDiv.disablePit();
        ventilator.stop();
        temperature.resetProgTemp();

        if(ir == 0xA2) {
          s = STATE1;
          ventilator.restart();

        }

        if(!onoffButton.read()) {
          s = STATE1;
          while (!onoffButton.read())
            ;
          ventilator.restart();
        }

        break;
      case STATE1:
        onoffButton.deactiveLed();
        timer.deactiveLed();
        display.writeWord(resultMultiplex, 2);
        display.writeWord(temperature.readTemperature(), 1);
        freqDiv.disablePit();

        if(monoEstavel.getSignal() || control.getSignal()) {
          compressorLed.writeBit(1);
        }
        else {
          compressorLed.writeBit(0);
        }

        if(ir == 0x90){
          temperature.setProgTemp(temperature.readTemperature());
        }

        if(ir == 0x62) {
          timer.increment();
          s = STATE2;
          freqDiv.enablePit();
        }

        if(ir == 0xA8) {
          temperature.incrementProgTemp();
        }

        if(ir == 0xE0) {
          temperature.decrementProgTemp();
        }

        if(timer.getSleepTime() > 0){
          s = STATE2;
          freqDiv.enablePit();
        }

        if(!onoffButton.read()) {
          s = STATE0;
          while (!onoffButton.read()) {
            display.writeWord(temperature.readTemperature(), 1);
            display.writeWord(resultMultiplex, 2);
            display.updateDisplays();
          }
        }

        if(ir == 0xE2) {
          timer.reset();
          ventilator.restart();
          temperature.resetProgTemp();
        }

        if(ir == 0x22) {
          ventilator.increment();
        }

        if(ir == 0xA2) {
          s = STATE0;
        }
        break;
      case STATE2:
        timer.activeLed();
        display.writeWord(resultMultiplex, 2);
        display.writeWord(temperature.readTemperature(), 1);

        if(monoEstavel.getSignal() || control.getSignal()) {
          compressorLed.writeBit(1);
        }
        else {
          compressorLed.writeBit(0);
        }

        if(ir == 0x90){
          temperature.setProgTemp(temperature.readTemperature());
        }

        if(!timer.getSleepTime()) {
          s = STATE0;
        }

        if(ir == 0x62) {
          timer.increment();
          if(!timer.getSleepTime())
            s = STATE1;
        }

        if(ir == 0xA8) {
          temperature.incrementProgTemp();
        }

        if(ir == 0xE0) {
          temperature.decrementProgTemp();
        }

        if(!onoffButton.read()) {
          s = STATE0;
          while (!onoffButton.read()) {
            display.writeWord(temperature.readTemperature(), 1);
            display.writeWord(resultMultiplex, 2);
            display.updateDisplays();
          }
        }
        if(ir == 0x22) {
          ventilator.increment();
        }

        if(ir == 0xE2) {
          timer.reset();
          ventilator.restart();
          temperature.resetProgTemp();
          s = STATE1;
        }

        if(ir == 0xA2) {
          s = STATE0;
        }
        break;
    }
  }

  return 0;
}

