/*
 * MonoEstavel.h
 *
 *  Created on: 13/10/2017
 *      Author: joao
 */

#ifndef SOURCES_LIB_MONOESTAVEL_MONOESTAVEL_H_
#define SOURCES_LIB_MONOESTAVEL_MONOESTAVEL_H_

#include <Lib/TPM/TPM_Delay/mkl_TPMDelay.h>

class MonoEstavel {
  public:
    MonoEstavel(tpm_TPMNumberMask TPMDelay);
    void detectEdge(bool edge);
    void startTimer();
    bool getSignal();

  private:
    mkl_TPMDelay _delay;
    bool _startTimerFlag = false;
    bool _edge = false;
    uint8_t _counter = 0;
    bool _signal = false;

};

#endif /* SOURCES_LIB_MONOESTAVEL_MONOESTAVEL_H_ */
