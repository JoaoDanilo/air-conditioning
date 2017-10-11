/*!
 * @copyright   � 2017 UFAM - Universidade Federal do Amazonas.
 *
 * @brief       Interface de programa��o de aplica��es em C++ para Display Multiplexado - M�dulo 74HC595.
 *
 * @file        dsf_SerialDisplays.h
 * @version     1.0
 * @date        2 Agosto 2017
 *
 * @section     HARDWARES & SOFTWARES
 *              +board        FRDM-KL25Z da NXP.
 *              +processor    MKL25Z128VLK4 - ARM Cortex-M0+
 *              +peripherals  GPIO, PIT e M�dulo 74HC595 com Display 4 D�gitos.
 *              +manual       L25P80M48SF0RM, Rev.3, September 2012
 *              +revisions    Vers�o (data): Descri��o breve.
 *                             ++ 0.9 (17 Julho 2017): Vers�o inicial.
 *                             ++ 1.0 (2 Agosto 2017): Generaliza��o dos perif�ricos
 *
 * @section     AUTHORS & DEVELOPERS
 *              +institution  Universidade Federal do Amazonas
 *              +courses      Engenharia El�trica / Engenharia da Computa��o.
 *              +teacher      Miguel Grimm <miguelgrimm@gmail.com>
 *              +student      Vers�o inicial:
 *                             ++ Magno Aguiar de Carvalho <carvalhodemagno@gmail.com>
 *                             ++ Phillipp de Souza Gama Lavor <lavorphillipp@gmail.com>
 *                             ++ Jadir Campos Barbosa Junior <jadirjr96@gmail.com>
 *              +student      Revis�es - Nome revisor <e-mail do revisor>
 *                             ++v1.0 - Jose Luis da Silva e Silva <joseluis70123@gmail.com>
 *
 * @section     LICENSE
 *
 *              GNU General Public License (GNU GPL).
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

#ifndef DSF_SERIALDISPLAYS_H
#define DSF_SERIALDISPLAYS_H

#include <Lib/GPIO/mkl_GPIO.h>
#include <Lib/GPIO/GPIO_Port/mkl_GPIOPort.h>
#include <stdint.h>

/*!
 *  @class    dsf_MuxDisplays
 *
 *  @brief    A classe dsf_SerialDisplays representa o perif�rico Off-Chip do Display Multiplexado.
 *
 *  @details  Esta classe � usada para escrita de dados no Display Multiplexado Serial.
 *
 *  @section  EXAMPLES USAGE
 *
 *
 *      Uso dos m�todos para escrita
 *        +fn dsf_SerialDisplays(OCP_t gpio, int dio, int sclk, int rclk);
 *	      +fn writeNibble(uint8_t bin, uint8_t number);
 *        +fn writeWord(uint16_t bcd)
 *	      +fn clearDisplays();
 *	      +fn showZerosLeft();
 *	      +fn hideZerosLeft();
 */

class dsf_SerialDisplays {
 public:
  dsf_SerialDisplays(gpio_Pin Pin_DIO, gpio_Pin Pin_SCLK, gpio_Pin Pin_RCLK);
  void updateDisplays();
  void setupPeripheral();
  void writeNibble(uint8_t bin, uint8_t number);
  void writeWord(uint8_t number, uint8_t port);
  void writeWord(uint16_t bcd);
  void clearDisplays();
  void showZerosLeft();
  void hideZerosLeft();

 private:
  uint8_t storeData[4];
  uint8_t nibble[10];
  mkl_GPIOPort DIO,SCLK, RCLK;
  void setNibble();
  void sendNibble(char digit);
};

#endif
