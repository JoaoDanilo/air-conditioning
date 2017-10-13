/*
 * ControlCompressor.cpp
 *
 *  Created on: 13/10/2017
 *      Author: joao
 */

#include <Lib/Control/ControlCompressor.h>

void ControlCompressor::controlCompressor(uint8_t Ta, uint8_t Tp){

  if(Ta >= Tp){
    _signal = true;
  }
  else{
    _signal = false;
  }
}

bool ControlCompressor::getSignal(){
  return _signal;
}

