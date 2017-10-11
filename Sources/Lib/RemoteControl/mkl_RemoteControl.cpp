/*!
 * @copyright   � 2017 Universidade Federal do Amazonas.
 *
 * @brief       Uma API em C++ para Controle Remoto.
 *
 * @file        mkl_RemoteControl.cpp
 * @version     1.0
 * @date        6 Julho 2017
 *
 * @section     HARDWARES & SOFTWARES
 *              +board        FRDM-KL25Z da NXP.
 *              +processor    MKL25Z128VLK4 - ARM Cortex-M0+
 *              +compiler     Kinetis� Design Studio IDE
 *              +manual       L25P80M48SF0RM, Rev.3, September 2012
 *              +revisions    Vers�o (data): Descri��o breve.
 *                             ++ 1.0 (12 agosto 2017): Vers�o inicial.
 *
 * @section     AUTHORS & DEVELOPERS
 *              +institution  Universidade Federal do Amazonas
 *              +courses      Engenharia da Computa��o / Engenharia El�trica
 *              +teacher      Miguel Grimm <miguelgrimm@gmail.com>
 *              +student      Vers�o inicial:
 *                             ++ Jose Luis da Silva <joseluis70123@gmail.com>
 *
 * @section     LICENSE
 *
 *              GNU General Public License (GNU GPL)
 *
 *              Este programa � um software livre; Voc� pode redistribu�-lo
 *              e/ou modific�-lo de acordo com os termos do "GNU General Public
 *              License" como publicado pela Free Software Foundation; Seja a
 *              vers�o 3 da licen�a, ou qualquer vers�o posterior.
 *
 *              Este programa � distribu�do na esperan�a de que seja �til,
 *              mas SEM QUALQUER GARANTIA; Sem a garantia impl�cita de
 *              COMERCIALIZA��O OU USO PARA UM DETERMINADO PROP�SITO.
 *              Veja o site da "GNU General Public License" para mais detalhes.
 *
 * @htmlonly    http://www.gnu.org/copyleft/gpl.html
 */

#include "mkl_RemoteControl.h"

/*!
 *   @fn       mkl_RemoteControl
 *
 *   @brief    M�todo construtor da classe.
 *
 *   Este � o m�todo construtor padr�o da classe e realiza a associa��o
 *   entre o objeto de software e os perif�ricos de hardware usados pelo controle.
 *
 *   @param[in]  gpioName - GPIO;
 *               pin - pino do TPM Measure
 */
mkl_RemoteControl::mkl_RemoteControl(gpio_Pin pin, PIT_ChPIT channel) {
  _gpio = mkl_GPIOPort(pin);
  _gpio.setPortMode(gpio_input);
  _gpio.enableInterrupt(gpio_falling_edge);

  enableNVICInterrupt(pin);
  _pit = mkl_PITInterruptInterrupt(channel);
  _pit.enablePeripheralModule();
  _pit.setPeriod(10485760 * 0.0001); // 100 ns
  _pit.enablePeripheralClock();
  _pit.enableInterruptRequests();
  _pit.enableTimer();
}

void mkl_RemoteControl::enablePit() {
  _pit.enableTimer();
}

void mkl_RemoteControl::disablePit() {
  _pit.disableTimer();
}

void mkl_RemoteControl::enableNVICInterrupt(gpio_Pin pin) {

  uint32_t port = pin & 0xFFFFFF00;

  if(port == gpio_GPIOA) {
    NVIC_EnableIRQ(PORTA_IRQn);
  }
  else if(port == gpio_GPIOD) {
    NVIC_EnableIRQ(PORTD_IRQn);
  }
}

void mkl_RemoteControl::clearFlagGPIO() {
  _gpio.clearInterruptFlag();
}

bool mkl_RemoteControl::isGPIOFlag() {
  return _gpio.isInterruptFlagSet();
}

void mkl_RemoteControl::clearPITFlag() {
  _pit.clearInterruptFlag();
}

bool mkl_RemoteControl::isPITFlag() {
  return _pit.isInterruptFlagSet();
}

void mkl_RemoteControl::getBitStreamIR() {

  if(isGPIOFlag()) {
    clearFlagGPIO();
    _timerValue = _nsTimer;
    _nsTimer = 0;

    if(_timerValue >= 500) {
      enablePit();
      _pulseCount = -2;
      _bitStream = 0;
    }
    else if((_pulseCount >= 0) && (_pulseCount < 32)) {
      if(_timerValue >= 19) {
        _bitStream |= (uint32_t) 1 << (31 - _pulseCount);
      }
      _pulseCount++;
    }
    else if(_pulseCount >= 32) {
      _newCommand = _bitStream;
      _pulseCount = 0;
      _flagCommandAvailable = true;

      if(!_flagOverWrite)
        _flagOverWrite = true;
      else
        _flagOverWrite = false;

    }
    else {
      _pulseCount++;
    }
  }

}

void mkl_RemoteControl::incrementTime() {

  if(isPITFlag()) {
    clearPITFlag();

    if(_nsTimer < 500) {
      _nsTimer++;
    }
    else {
      disablePit();
    }
  }

}

uint8_t mkl_RemoteControl::readCommand() {
  _flagCommandAvailable = false;
  _flagOverWrite = false;

  uint16_t command = bitShiftRight(_newCommand);


  return (command & 0xFF00) >> 8;
}

Exception_t mkl_RemoteControl::waitCommand() {

  if(isParityError(_newCommand)){
    return mkl_parityError;
  }
  else if(isOverWriteError()){
    return mkl_overWrite;
  }

  return mkl_ok;

}

bool mkl_RemoteControl::commandAvailable(){
  return _flagCommandAvailable;
}

bool mkl_RemoteControl::isParityError(uint32_t value) {


  uint16_t command = bitShiftRight(value);

  uint8_t _command = (command & 0xFF00) >> 8;
  uint8_t _notCommand = command & 0x00FF;

  if(_command != (~_notCommand & 0xFF)) {
    return true;
  }

  return false;
}

bool mkl_RemoteControl::isOverWriteError(){

  if(!_flagOverWrite){
    return true;
  }
  return false;
}

uint16_t mkl_RemoteControl::bitShiftRight(uint32_t value) {

  uint8_t flag = value & 0x00000001;

  value = (value >> 1);
  value |= (flag << 31);

  return value & 0x0000FFFF;
}

