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
FrequencyDivider fd(100, PIT_Ch0);
mkl_RemoteControl remoteControl(gpio_PTA2, PIT_Ch1, gpio_PTB3, gpio_PTC2);
Temperature temperature(tpm_TPM0, gpio_PTE20, tpm_TPM0);
Ventilation ventilator(tpm_PTB0);
MonoEstavel monoEstavel(tpm_TPM1);
ControlCompressor control;

mkl_GPIOPort red(gpio_PTB1);

enum STATE {
  STATE0, STATE1, STATE2
};

extern "C" {

  void PORTA_IRQHandler() {
    remoteControl.getBitStreamIR();
  }

  void PIT_IRQHandler(void) {
    remoteControl.incrementTime();
    fd.clockDiv(&timer);
  }
}

int main(void) {

  STATE s = STATE0;
  display.clearDisplays();

  red.setPortMode(gpio_output);
  red.writeBit(0);

  while (true) {

    volatile uint8_t ir = 5;
    display.updateDisplays();
    uint8_t resultMultiplex = multiplex.select(timer.getSleepTime() > 0 ? 0 : 1,
        timer.getSleepTime(), temperature.getProgTemp());
    monoEstavel.startTimer();
    temperature.getTemperature();

    control.controlCompressor(temperature.readTemperature(),
        temperature.getProgTemp());
    monoEstavel.detectEdge(control.getSignal());

    if(remoteControl.commandAvailable()) {

      ir = remoteControl.readCommand();
      Exception_t error = remoteControl.waitCommand();

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
        red.writeBit(0);
        display.clearDisplays();
        timer.reset();
        fd.disablePit();
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
        fd.disablePit();

        if(monoEstavel.getSignal() || control.getSignal()) {
          red.writeBit(1);
        }
        else {
          red.writeBit(0);
        }

        if(ir == 0x62) {
          timer.increment();
          s = STATE2;
          fd.enablePit();
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
          red.writeBit(1);
        }
        else {
          red.writeBit(0);
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

