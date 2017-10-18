#ifndef SOURCES_MINUTETIMER_H_
#define SOURCES_MINUTETIMER_H_

#include <stdint.h>
#include <math.h>
#include <Lib/GPIO/GPIO_Port/mkl_GPIOPort.h>

/**
 * Class MinuteTimer - Represent the controller time of air-conditioning
 * @author Salmiakki Team
 * @version 1.0, 2017-08-23
 */
class MinuteTimer{

  public:
    /**
     * Constructor class - Set the input pins to control the timer
     * @param incrementButton - Pin related to the increase of time
     * @param decrementButton - Pin related to the decrease of time
     * @param resetButton - Pin related to the reset time
     */
    MinuteTimer(gpio_Pin incrementButton, gpio_Pin resetButton, gpio_Pin blueLed);

    /**
     * Increment time by 10
     */
    void increment();

    /**
     * Decrement time by 1
     */
    void decrement();

    /**
     * Timer equal zero
     */
    void reset();

    /**
     * Get current time
     * @return Current time
     */
    uint8_t getSleepTime();

    /**
     * Read state of increment button
     * @return State of the button (HIGH or LOW)
     */
    uint8_t readIncrementButton();

    /**
     * Read state of reset button
     * @return State of the button (HIGH or LOW)
     */
    uint8_t readResetButton();

    /**
     * Actice Led while decrement counter
     */
    void activeLed();
    void deactiveLed();

    void setTimer(uint8_t time);

  private:

    /** Digital port associated with increment pin */
    mkl_GPIOPort _incrementButton;

    /** Digital port associated with reset pin */
    mkl_GPIOPort _resetButton;

    /** Current time of controller */
    uint8_t _sleepTime = 0;

    /** Digital port associated with Led counter */
    mkl_GPIOPort _blueLed;
};

#endif
