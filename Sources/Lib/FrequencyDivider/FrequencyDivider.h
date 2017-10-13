#ifndef SOURCES_FREQUENCYDIVIDER_H_
#define SOURCES_FREQUENCYDIVIDER_H_

#include <stdint.h>
#include <Lib/PIT/PITPeriodicInterrupt/mkl_PITPeriodicInterrupt.h>
#include <Lib/MinuteTimer/MinuteTimer.h>

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
    FrequencyDivider(uint32_t divider, PIT_ChPIT channel);

    /**
     * Calculates the clock division
     */
    void clockDiv(MinuteTimer *timer);

    void disablePit();

    void enablePit();

  private:

    /** Number of times the clock is being divided */
    uint32_t _divider;

    /** Aux counter */
    uint32_t _counter = 1;

    mkl_PITInterruptInterrupt _pit;

    void clearPITFlag();

    bool isPITFlag();
};

#endif

