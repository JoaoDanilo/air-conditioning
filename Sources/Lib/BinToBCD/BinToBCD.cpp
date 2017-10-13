/*
 * BinToBCD.cpp
 *
 *  Created on: 13/10/2017
 *      Author: joao
 */

#include <Lib/BinToBCD/BinToBCD.h>

uint16_t BinToBCD::binToBCD(uint8_t bin){
  uint16_t bcd = 0;
  uint8_t  counter = 0;

  while(bin > 0){
    bcd |= (bin % 10) << (counter++ << 2);
    bin /= 10;
  }

  return bcd;
}

