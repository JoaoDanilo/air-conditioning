#ifndef SOURCES_FREQUENCYDIVIDER_H_
#define SOURCES_FREQUENCYDIVIDER_H_

#include <stdint.h>

/**
 * Class FrequencyDivider - Represent a frequency divider device
 * @author Salmiakki Team
 * @version 1.0, 2017-08-23
 */
class FrequencyDivider{

  public:

    /**
     * Constructor class - Enter with the number of times the clock will be divided
     * @param divider Clock divider
     */
    FrequencyDivider(uint32_t divider);

    /**
     * Calculates the clock division
     */
    uint8_t clockDiv();

  private:

    /** Number of times the clock is being divided */
    uint32_t _divider;

    /** Aux counter */
    uint32_t _counter = 1;
};

#endif

