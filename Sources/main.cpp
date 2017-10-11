#include <MKL25Z4.h>
#include <Lib/Display/dsf_SerialDisplays.h>
#include <Lib/GPIO/GPIO_Port/mkl_GPIOPort.h>
#include <Lib/Multiplexer/Multiplexer.h>
#include <Lib/TPM/TPM_Delay/mkl_TPMDelay.h>
#include <Lib/Onoff/Onoff.h>
#include <Lib/MinuteTimer/MinuteTimer.h>
#include <Lib/PIT/PITPeriodicInterrupt/mkl_PITPeriodicInterrupt.h>
#include <Lib/FrequencyDivider/FrequencyDivider.h>
#include <Lib/RemoteControl/mkl_RemoteControl.h>
#include <Lib/Ventilation/ventilation.h>

dsf_SerialDisplays display(gpio_PTC7, gpio_PTC0, gpio_PTC3);
Onoff onoffButton(gpio_PTA5, gpio_PTB19);
MinuteTimer timer(gpio_PTC8, gpio_PTC9);
Multiplexer multiplex;
mkl_TPMDelay delay(tpm_TPM0);
mkl_GPIOPort ledBlue(gpio_PTD1);
mkl_GPIOPort ledRed(gpio_PTB18);
mkl_GPIOPort ledWhite(gpio_PTC9);
mkl_GPIOPort ledYellow(gpio_PTC8);
mkl_PITInterruptInterrupt pit(PIT_Ch0);
FrequencyDivider fd(100);
mkl_RemoteControl remoteControl(gpio_PTA17, PIT_Ch1);
Ventilation ventilator(tpm_PTB0);

uint32_t debounceTime = 24576;
uint32_t pitTime = 10485760 * 0.01;   // 10485760 hz = 1 s

enum STATE {
  STATE0, STATE1, STATE2, STATE3
};

enum {
  LOW, HIGH
};

extern "C" {

  void PORTA_IRQHandler() {
    remoteControl.getBitStreamIR();
  }

  void PIT_IRQHandler(void) {

    remoteControl.incrementTime();

    if(pit.isInterruptFlagSet()) {
      pit.clearInterruptFlag();
      if(fd.clockDiv()) {
        timer.decrement();
      }
    }

  }
}

int main(void) {

  delay.setFrequency(tpm_div128);
  delay.startDelay(10);

  ledBlue.setPortMode(gpio_output);
  ledBlue.writeBit(HIGH);

  ledRed.setPortMode(gpio_output);
  ledRed.writeBit(HIGH);

  ledWhite.setPortMode(gpio_output);
  ledWhite.writeBit(HIGH);

  ledYellow.setPortMode(gpio_output);
  ledYellow.writeBit(HIGH);

  pit.enablePeripheralModule();
  pit.setPeriod(pitTime);

  STATE s = STATE0;
  int i = 0;

  display.clearDisplays();

  while (true) {
    volatile uint16_t ir = 5;
    display.updateDisplays();
    uint8_t resultMultiplex = multiplex.select(0, timer.getSleepTime(), 0x12);

    if(remoteControl.commandAvailable()) {

      ir = remoteControl.readCommand();
      ledWhite.writeBit(HIGH);
      ledYellow.writeBit(HIGH);

      if(remoteControl.waitCommand() != mkl_ok) {
        if(remoteControl.waitCommand() == mkl_parityError) {

          ledWhite.writeBit(LOW);

        }
        else if(remoteControl.waitCommand() == mkl_overWrite) {
          ledYellow.writeBit(LOW);
        }
      }
    }

    switch (s) {
      case STATE0:
        onoffButton.ledStandby(LOW);
        ledBlue.writeBit(HIGH);
        display.clearDisplays();
        timer.reset();
        pit.disableTimer();
        pit.resetCounter();
        pit.disableInterruptRequests();
        ventilator.stop();
        ventilator.start();

        if(ir == 0xA2) {
          s = STATE1;
          ventilator.restart();
          ventilator.start();
        }

        if(!onoffButton.read() && delay.timeoutDelay()) {
          s = STATE1;
          while (!onoffButton.read())
                  ;   //-------
          delay.startDelay(debounceTime);
          ventilator.restart();
          ventilator.start();
        }



        break;
      case STATE1:
        onoffButton.ledStandby(HIGH);
        ledBlue.writeBit(HIGH);
        display.writeWord(0x0, 2);
        pit.disableTimer();
        pit.resetCounter();
        pit.disableInterruptRequests();

        if(ir == 0x62) {
          timer.increment();
          s = STATE2;
          pit.enableTimer();
          pit.enableInterruptRequests();
        }

        if(!onoffButton.read() && delay.timeoutDelay()) {
          s = STATE0;
          delay.startDelay(debounceTime);
        }

        if(ir == 0xE2) {
          timer.reset();
          ventilator.restart();
          ventilator.start();
        }

        if(ir == 0x22) {
          ventilator.increment();
          ventilator.start();
        }

        if(ir == 0xA2) {
          s = STATE0;
        }
        break;
      case STATE2:
        ledBlue.writeBit(LOW);
        display.writeWord(resultMultiplex, 2);

        if(!timer.getSleepTime()) {
          s = STATE0;
        }

        if(ir == 0x62) {
          timer.increment();
          if(!timer.getSleepTime())
            s = STATE1;
        }

        if(!onoffButton.read() && delay.timeoutDelay()) {
          s = STATE0;
          delay.startDelay(debounceTime);
        }
        if(ir == 0x22) {
          ventilator.increment();
          ventilator.start();
        }

        if(ir == 0xE2) {
          timer.reset();
          ventilator.restart();
          ventilator.start();
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

