/*!
 * @copyright   � 2017 UFAM - Universidade Federal do Amazonas.
 *
 * @brief       Implementa��o da API em C++ para GPIO.
 *
 * @file        mkl_GPIOPort.cpp
 * @version     1.0
 * @date        6 Julho 2017
 *
 * @section     HARDWARES & SOFTWARES
 *              +board        FRDM-KL25Z da NXP.
 *              +processor    MKL25Z128VLK4 - ARM Cortex-M0+
 *              +compiler     Kinetis� Design Studio IDE
 *              +manual       L25P80M48SF0RM, Rev.3, September 2012
 *              +revisions    Vers�o (data): Descri��o breve.
 *                             ++ 1.0 (30 Julho 2017): Vers�o inicial.
 *
 * @section     AUTHORS & DEVELOPERS
 *              +institution  Universidade Federal do Amazonas
 *              +courses      Engenharia da Computa��o / Engenharia El�trica
 *              +teacher      Miguel Grimm <miguelgrimm@gmail.com>
 *              +student      Vers�o inicial:
 *                             ++ Hamilton Nascimento <hdan_neto@hotmail.com>
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

#include <Lib/GPIO/GPIO_Port/mkl_GPIOPort.h>

/*!
 *   @fn       mkl_GPIOPort
 *
 *   @brief    Construtor padr�o da classe.
 *
 *   O construtor padr�o da classe obt�m o n�mero do pino e do GPIO a partir
 *   do pino desejado, associa fisicamento o objeto de software ao perif�rico
 *   de hardware, habilita o clock do GPIO e seleciona o modo GPIO de opera��o
 *   do pino.
 *
 *   @remarks  Siglas e p�ginas do Manual de Refer�ncia KL25:
 *             - PortxPCRn: Pin Control Register.P�g. 183 (Mux) and 185 (Pull).
 */
mkl_GPIOPort::mkl_GPIOPort(gpio_Pin pin) {
  uint32_t pinNumber;
  uint32_t gpio;

  setGPIOParameters(pin, gpio, pinNumber);

  _pinNumber = pinNumber;
  bindPeripheral(gpio, pinNumber);
  enableModuleClock(gpio);
  selectMuxAlternative();
}

/*!
 *   @fn         disableInterrupt
 *
 *   @brief      Desabilita as interrup��es das GPIOS para pararem de funcionar.
 *
 *   @remarks    Sigla e pagina do Manual de Referencia KL25:
 *               - PortxPCRn: Pin Control Register.P�g. 183 (Mux) and 185 (Pull).
 */
void mkl_GPIOPort::disableInterrupt(){
  *addressPortxPCRn &= ~0xF0000;
}

/*!
 *   @fn         enableInterrupt
 *
 *   @brief      habilita as interrup��es das GPIOS para operar.
 *
 *   @remarks    Sigla e pagina do Manual de Referencia KL25:
 *               - PortxPCRn: Pin Control Register.P�g. 183 (Mux) and 185 (Pull).
 */
void mkl_GPIOPort::enableInterrupt(gpio_interrupt interrupt){
  disableInterrupt();
  *addressPortxPCRn |= interrupt;
}

/*!
 *   @fn         clearInterruptFlag
 *
 *   @brief      Limpa a flag de interrup��o
 *
 *   @remarks    Sigla e pagina do Manual de Referencia KL25:
 *                - PortxPCRn: Pin Control Register.P�g. 183(Mux) and 185(Pull).
 */
void mkl_GPIOPort::clearInterruptFlag(){
  *addressPortxISFR = 1 << _pinNumber;
}

bool mkl_GPIOPort::isInterruptFlagSet(){

  if(*addressPortxISFR & (1 << _pinNumber)){
    return true;
  }

  return false;
}
