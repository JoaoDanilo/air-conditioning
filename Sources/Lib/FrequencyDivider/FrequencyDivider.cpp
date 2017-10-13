#include "FrequencyDivider.h"

FrequencyDivider::FrequencyDivider(uint32_t divider, PIT_ChPIT channel) {
  _divider = divider;

  _pit = mkl_PITInterruptInterrupt(channel);
  _pit.enablePeripheralModule();
  _pit.setPeriod(10485760 * 0.01); // 10 ms
  _pit.enablePeripheralClock();
  _pit.enableInterruptRequests();
  _pit.enableTimer();
}

void FrequencyDivider::disablePit() {
  _pit.disableTimer();
  _pit.resetCounter();
  _pit.disableInterruptRequests();
}

void FrequencyDivider::enablePit() {
  _pit.enableTimer();
  _pit.enableInterruptRequests();
}

void FrequencyDivider::clearPITFlag() {
  _pit.clearInterruptFlag();
}

bool FrequencyDivider::isPITFlag() {
  return _pit.isInterruptFlagSet();
}

void FrequencyDivider::clockDiv(MinuteTimer *timer) {

  if(isPITFlag()) {
    clearPITFlag();

    if(_counter == _divider) {
      _counter = 1;
      timer->decrement();
    }
    _counter++;
  }

}

