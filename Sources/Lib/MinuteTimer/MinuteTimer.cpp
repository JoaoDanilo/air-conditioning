#include "MinuteTimer.h"

MinuteTimer::MinuteTimer(gpio_Pin incrementButton, gpio_Pin resetButton,
    gpio_Pin blueLed) {

  _resetButton = mkl_GPIOPort(resetButton);
  _incrementButton = mkl_GPIOPort(incrementButton);
  _blueLed = mkl_GPIOPort(blueLed);

  _incrementButton.setPortMode(gpio_input);
  _incrementButton.setPullResistor(gpio_pullUpResistor);

  _resetButton.setPortMode(gpio_input);
  _resetButton.setPullResistor(gpio_pullUpResistor);

  _blueLed.setPortMode(gpio_output);
  _blueLed.writeBit(1);

}

void MinuteTimer::increment() {

  _sleepTime = (_sleepTime / 10) * 10 + 10;

  if(_sleepTime > 90)
    _sleepTime = 0;
}

uint8_t MinuteTimer::getSleepTime() {
  return _sleepTime;
}

void MinuteTimer::setTimer(uint8_t time) {

  int result = 0;
  result = (time >> 4) & 0x0F;
  result *= 10;

  result += (time & 0xF);

  if(result > 99) {
    _sleepTime = 99;
  }
  else if(result < 0) {
    _sleepTime = 0;
  }
  else {
    _sleepTime = result;
  }

}

void MinuteTimer::decrement() {

  _sleepTime -= 1;

  if(_sleepTime <= 0) {
    _sleepTime = 0;
  }
}

void MinuteTimer::reset() {
  _sleepTime = 0;
}

uint8_t MinuteTimer::readIncrementButton() {
  return _incrementButton.readBit();
}

uint8_t MinuteTimer::readResetButton() {
  return _resetButton.readBit();
}

void MinuteTimer::activeLed() {
  _blueLed.writeBit(0);
}

void MinuteTimer::deactiveLed() {
  _blueLed.writeBit(1);
}
