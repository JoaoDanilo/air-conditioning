/*!
 * @copyright   � 2017 UFAM - Universidade Federal do Amazonas.
 *
 * @brief       Interface de programa��o de aplica��es em C++ para Display Multiplexado - M�dulo 74HC595.
 *
 * @file        dsf_SerialDisplays.cpp
 * @version     1.0
 * @date        2 Agosto 2017
 *
 * @section     HARDWARES & SOFTWARES
 *              +board        FRDM-KL25Z da NXP.
 *              +processor    MKL25Z128VLK4 - ARM Cortex-M0+
 *              +compiler     Kinetis� Design Studio IDE
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

#include <Lib/Display/dsf_SerialDisplays.h>
#include <stdint.h>

// dsf_GPIO_ocp  DIO;

/*!
 *  Seta o perif�rico, considerando os pinos de sa�da referentes ao componente
 *  DIO (dado), SCLK (desloca), RCLK (transfere de um registrador para o outro)
 */
dsf_SerialDisplays::dsf_SerialDisplays(gpio_Pin Pin_DIO, gpio_Pin Pin_SCLK, gpio_Pin Pin_RCLK){

  DIO = mkl_GPIOPort(Pin_DIO);
  SCLK = mkl_GPIOPort(Pin_SCLK);
  RCLK = mkl_GPIOPort(Pin_RCLK);
  DIO.setPortMode(gpio_output);
  SCLK.setPortMode(gpio_output);
  RCLK.setPortMode(gpio_output);

  setNibble();
}

/*!
 *  Atualiza o dado nos registradores internos.
 */
void dsf_SerialDisplays::updateDisplays(){
  /*!
   *  Envia o valor do vetor storeData na posi��o 0 para o primeiro
   *  display, que � referido com o bin�rio.
   */
  sendNibble(storeData[0]);
  sendNibble(0b0001);

  /*!
   *  Cria um pulso de Low-High.
   */
  RCLK.writeBit(0);
  RCLK.writeBit(1);

  sendNibble(storeData[1]);
  sendNibble(0b0010);
  RCLK.writeBit(0);
  RCLK.writeBit(1);

  sendNibble(storeData[2]);
  sendNibble(0b0100);
  RCLK.writeBit(0);
  RCLK.writeBit(1);

  sendNibble(storeData[3]);
  sendNibble(0b1000);
  RCLK.writeBit(0);
  RCLK.writeBit(1);
}

/*!
 *  Armazena o valor do n�mero e a posi��o do display a ser mostrada.
 */
void dsf_SerialDisplays::writeNibble(uint8_t bin, uint8_t number){
  storeData[number] = nibble[bin];
}

void dsf_SerialDisplays::writeWord(uint8_t number, uint8_t port){

  uint8_t unit = (uint8_t) (number % 10);
  int8_t ten = (uint8_t) (number / 10);

  uint8_t multiplier = 0;

  if(port == 1)
    multiplier = 1;
  else if(port == 2)
    multiplier = 2;
  else
    port = 0;

  if(port > 0){
    sendNibble(nibble[unit]);
    sendNibble(multiplier * (1 << (port - 1)));
    RCLK.writeBit(0);
    RCLK.writeBit(1);
  }

  if((number > 9) && (port > 0)){
    sendNibble(nibble[ten]);
    sendNibble(multiplier * (1 << port));
    RCLK.writeBit(0);
    RCLK.writeBit(1);
  }
}

void dsf_SerialDisplays::writeWord(uint16_t bcd) {
  /*!
   *  Variáveis auxiliares
   */
  int A = 0, B = 0, C = 0, D = 0;

  /*!
   *  Retirar a casa dos milhares.
   */
  A = static_cast<int>(bcd/1000);

  /*!
   *  Retirar a casa das centenas.
   */
  B = static_cast<int>(bcd - A*1000)/100;

  /*!
   * Retirar a casa das dezenas
   */
  C = static_cast<int>(bcd - A*1000 - B*100)/10;

  /*!
   *  Retirar a casa das unidades.
   */
  D = static_cast<int>(bcd - A*1000 - B*100 - C*10);

  /*!
   *  Armazena na posição 3 o valor númerico de A.
   */
  storeData[3] = nibble[A];

  /*!
   *  Armazena na posição 2 o valor númerico de B.
   */
  storeData[2] = nibble[B];

  /*!
   *  Armazena na posição 1 o valor númerico de C.
   */
  storeData[1] = nibble[C];

  /*!
   *  Armazena na posição 0 o valor númerico de D.
   */
  storeData[0] = nibble[D];
}

void dsf_SerialDisplays::clearDisplays(){
  /*!
   *  Vari�vel auxiliar.
   */
  int i = 0;
  for(i = 0; i <= 3; i++){
    /*!
     *  Limpa o registrador de acordo com a posi��o "i".
     */
    storeData[i] = 0xFF;
  }
}

/*!
 *  Mostra os Zeros a esquerda
 */
void dsf_SerialDisplays::showZerosLeft(){
  /*!
   *  Se a posi��o 3 for zero o dado ser� mostrado.
   */
  if(storeData[3] == 0xFF){
    /*!
     *  Dado sendo mostrado
     */
    storeData[3] = 0xC0;
    /*!
     *  Se a posi��o 2 for zero o dado ser� mostrado.
     */
    if(storeData[2] == 0xFF){
      /*!
       *  Dado sendo mostrado
       */
      storeData[2] = 0xC0;

      /*!
       *  Se a posi��o 2 for zero o dado ser� mostrado.
       */
      if(storeData[1] == 0xFF){
        /*!
         *  Dado sendo mostrado.
         */
        storeData[1] = 0xC0;
      }
    }
  }
}

/*!
 *  Apaga os Zeros a esquerda.
 */
void dsf_SerialDisplays::hideZerosLeft(){
  /*!
   *  Se o dado na posi��o 3 for 0, logo ele ser� apagado,
   *  se n�o continua normalmente.
   */
  if(storeData[3] == 0xC0){
    /*!
     *  Dado sendo apagado
     */
    storeData[3] = 0xFF;
    /*!
     *  Se o dado na posi��o 2 for 0, logo ele ser� apagado,
     *  se n�o continua normalmente.
     */
    if(storeData[2] == 0xC0){
      /*!
       *  Dado sendo apagado.
       */
      storeData[2] = 0xFF;
      /*!
       *  Se o dado na posi��o 1 for 0, logo ele ser� apagado,
       *  se n�o continua normalmente.
       */
      if(storeData[1] == 0xC0){
        /*!
         *  Dado sendo apagado.
         */
        storeData[1] = 0xFF;
      }
    }
  }
}

void dsf_SerialDisplays::sendNibble(char digit){
  /*!
   * Valor auxiliar
   */
  int t = 0;
  for(t = 8; t >= 1; t--){
    if(digit & 0x80){
      DIO.writeBit(1);
    }
    else{
      DIO.writeBit(0);
    }
    digit <<= 1;
    SCLK.writeBit(0);
    SCLK.writeBit(1);
  }
}

/*!
 *  Cria��o de um vetor de constantes com seus respectivos valores decodificados
 */
void dsf_SerialDisplays::setNibble(){
  /*!
   *  Valores 0,1,2,3,4,5,6,7,8 e 9
   */
  nibble[0] = 0xC0;
  nibble[1] = 0xF9;
  nibble[2] = 0xA4;
  nibble[3] = 0xB0;
  nibble[4] = 0x99;
  nibble[5] = 0x92;
  nibble[6] = 0x82;
  nibble[7] = 0xF8;
  nibble[8] = 0x80;
  nibble[9] = 0x90;
}

