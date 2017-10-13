/*
 * MonoEstavel.cpp
 *
 *  Created on: 13/10/2017
 *      Author: joao
 */

#include <Lib/MonoEstavel/MonoEstavel.h>

MonoEstavel::MonoEstavel(tpm_TPMNumberMask TPMDelay) {

  _delay = mkl_TPMDelay(TPMDelay);

  _delay.setFrequency(tpm_div128);
  _delay.startDelay(10);

}

void MonoEstavel::detectEdge(bool edge) {

  if(edge != _edge) {
    _edge = edge;
    _startTimerFlag = true;
  }

}

void MonoEstavel::startTimer() {

  if(_startTimerFlag) {
    _signal = true;

    if(_delay.timeoutDelay()) {
      _delay.setFrequency(tpm_div128);
      _delay.startDelay(0xFFFF);
      _counter++;
    }

    if(_counter == 8){
      _counter = 0;
      _startTimerFlag = false;
      _signal = false;
    }
  }
}

bool MonoEstavel::getSignal(){
  return _signal;
}

