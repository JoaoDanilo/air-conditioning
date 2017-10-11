#ifndef SOURCES_LIB_MULTIPLEXER_MULTIPLEXER_H_
#define SOURCES_LIB_MULTIPLEXER_MULTIPLEXER_H_

#include <stdint.h>

/**
 * Class Multiplexer - Represent a multiplexer 2:1
 * @author Salmiakki Team
 * @version 1.0, 2017-08-23
 */
class Multiplexer{

  public:

    /**
     * Based in 'select', the OUTPUT is 'inputCounter' or 'inputTemperature'
     * @param select Selection variable of the multiplexer
     * @param inputCounter Data of counter
     * @param inputTemperature Data of DHT11
     * @return Output of multiplexer
     */
    uint8_t select(uint8_t select, uint8_t inputCounter, uint8_t inputTemperature);

};

#endif
