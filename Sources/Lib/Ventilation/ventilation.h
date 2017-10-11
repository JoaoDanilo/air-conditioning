#ifndef SOURCES_VENTILATION_H_
#define SOURCES_VENTILATION_H_

#include <stdint.h>
#include <Lib/GPIO/GPIO_Port/mkl_GPIOPort.h>
#include <Lib/TPM/TPM_PWM/mkl_TPMPulseWidthModulation.h>




/**
 * Class Ventilation - Class to handle changes in motor speed
 * @author Salmiakki Team
 * @version 1.0, 2017-08-23
 */

class Ventilation{
	public:

	/**
	* Constructor class - Set the input pins to control the ventilation
	* @param[in] incrementButton - Pin to increment the speed
	* @param[in] pwmPin - Pin to output signal to motor
	*/

	Ventilation(tpm_Pin pwmPin);

	/**
	 * increment speed by 300
	 */
	void increment();

	void stop();
	void start();
	void restart();


	mkl_TPMPulseWidthModulation setPwm;



	/**
	 * get speed
	 * @return current speed
	 */
	uint16_t getSpeed();

	bool readButton();


	private:



	void setFreq(int speed);

	 /** Pin to increment the speed */
	 mkl_GPIOPort _incrementButton;

	 /** Pin to output signal to motor */
	 //tpm_Pin _pwmPin;

	 /** current speed of the motor */
	 int _currentSpeed = 0;

	 /** instance of pwm */
	 mkl_TPMPulseWidthModulation _pwm;



};

#endif
