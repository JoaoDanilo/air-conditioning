/*
 * ControlCompressor.h
 *
 *  Created on: 13/10/2017
 *      Author: joao
 */

#ifndef SOURCES_LIB_CONTROL_CONTROLCOMPRESSOR_H_
#define SOURCES_LIB_CONTROL_CONTROLCOMPRESSOR_H_

#include <stdint.h>

class ControlCompressor {
  public:

    void controlCompressor(uint8_t Ta, uint8_t Tp);
    bool getSignal();

  private:
    bool _signal = false;

};

#endif /* SOURCES_LIB_CONTROL_CONTROLCOMPRESSOR_H_ */
