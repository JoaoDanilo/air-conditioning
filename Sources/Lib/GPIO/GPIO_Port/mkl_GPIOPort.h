/*!
 * @copyright   � 2017 UFAM - Universidade Federal do Amazonas.
 *
 * @brief       Interface de programa��o de aplica��es em C++ para GPIO.
 *
 * @file        mkl_GPIOPort.h
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

#ifndef MKL_GPIOPORT_H_
#define MKL_GPIOPORT_H_

#include <Lib/GPIO/mkl_GPIO.h>

/*!
 *  @class    mkl_GPIOPort.
 *
 *  @brief    A classe implementa o perif�rico GPIO sem interrup��o.
 *
 *  @details  Esta classe � derivada da classe m�e "mkl_GPIO" e implementa
 *            o perif�rico GPIO com o modo de opera��o sem interrup��o.
 *
 */
class mkl_GPIOPort : public mkl_GPIO {
 public:
  /*!
   * Construtor padr�o da classe.
   */
  explicit mkl_GPIOPort(gpio_Pin pin = gpio_PTA1);

  /*!
   * M�todos que tratam da interrup��o.
   */
  void clearInterruptFlag();
  void enableInterrupt(gpio_interrupt interrupt);
  void disableInterrupt();
  bool isInterruptFlagSet();

 private:
  uint32_t _pinNumber = 0;
};

#endif  //  MKL_GPIOPORT_H_
