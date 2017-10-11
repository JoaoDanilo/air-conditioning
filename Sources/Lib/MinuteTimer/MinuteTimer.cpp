#include "MinuteTimer.h"

MinuteTimer::MinuteTimer(gpio_Pin incrementButton, gpio_Pin resetButton) {

	_resetButton = mkl_GPIOPort(resetButton);
	_incrementButton = mkl_GPIOPort(incrementButton);

	_incrementButton.setPortMode(gpio_input);
	_incrementButton.setPullResistor(gpio_pullUpResistor);

	_resetButton.setPortMode(gpio_input);
	_resetButton.setPullResistor(gpio_pullUpResistor);

}

void MinuteTimer::increment() {

 _sleepTime = (_sleepTime/10)*10 + 10;

	if (_sleepTime > 90)
		_sleepTime = 0;
}

uint8_t MinuteTimer::getSleepTime() {
	return _sleepTime;
}

void MinuteTimer::decrement() {

	_sleepTime -= 1;

	if (_sleepTime <= 0) {
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
