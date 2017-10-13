/*!
 * @copyright   � 2017 Universidade Federal do Amazonas.
 *
 * @brief       Implementa��o da classe em C++ do perif�rico DHT11.
 * @file        mkl_DHT11Sensor.cpp
 * @version     1.0
 * @date        06 Julho 2017
 *
 * @section     HARDWARES & SOFTWARES
 *              +board        FRDM-KL25Z da NXP.
 *              +processor    MKL25Z128VLK4 - ARM Cortex-M0+.
 *              +compiler     Kinetis� Design Studio IDE.
 *              +manual       L25P80M48SF0RM, Rev.3, September 2012.
 *              +revisions    Vers�o (data): Descri��o breve.
 *                             ++ 1.0 (03 Setembro 2017): Vers�o inicial.
 *
 * @section     AUTHORS & DEVELOPERS
 *              +institution  Universidade Federal do Amazonas.
 *              +courses      Engenharia da Computa��o / Engenharia El�trica.
 *              +teacher      Miguel Grimm <miguelgrimm@gmail.com>
 *              +student      Vers�o inicial:
 *                             ++ Hamilton Nascimento <hdan_neto@hotmail.com>
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

#include <stdint.h>
#include "mkl_DHT11Sensor.h"

/*!
 *  @fn       mkl_DHT11Sensor
 *
 *  @brief    Configura o perif�rico TPM e o GPIO para uso com o
 *            perif�rico off-chip.
 *
 *  @param    TPMNumber - perif�rico TPM desejado para uso com o sensor.
 *            pin - pino do GPIO desejado para uso com o sensor.
 */
mkl_DHT11Sensor::mkl_DHT11Sensor(tpm_TPMNumberMask TPMNumber,
                                         gpio_Pin pin)
                                        :tpm(TPMNumber),
                                         gpio(pin) {
  /*!
   * Ajusta a frequ�ncia de opera��o do TPM.
   */

}

mkl_DHT11Sensor::mkl_DHT11Sensor(){

}

/*!
 *  @fn       doAcquisition
 *
 *  @brief    Realiza uma aquisi��o de dados de temperatura e
 *            umidade no perif�rico do sensor DHT11 e armazena
 *            os dados nos atributos correspondentes
 *            no objeto de software.
 *
 *  @return   dht11_ok           - se a aquisi��o for realizada com sucesso.
 *            dht11_timeoutError - se n�o houver resposta do sensor.
 *            dht11_parityError  - se os dados enviados pelo sensor.
 *                                 n�o corresponderem ao byte de paridade.
 */
dht11_Exception mkl_DHT11Sensor::doAcquisition() {
  /*!
   * Envia o comando de start ao sensor de temperatura.
   */
  tpm.setFrequency(tpm_div1);
  startCommunication();

  /*!
   * Configura o pino para operar como entrada com resistor de Pull-Up.
   */
  gpio.setPortMode(gpio_input);
  gpio.setPullResistor(gpio_pullUpResistor);

  /*!
   * Aguarda a resposta do sensor ao comando enviado.
   */
  if (!waitResponse()) {
    return dht11_timeoutError;
  }

  /*!
   * Realiza a aquisi��o de dados de temperatura e umidade.
   */
  return readSensorData();
}

/*!
 *  @fn       readTemperature
 *
 *  @brief    L� a temperatura armazenada no atributo do objeto sensor.
 *
 *  @param    temp - vari�vel em que o valor de temperatura
 *            obtido ser� armzenado.
 */
void mkl_DHT11Sensor::readTemperature(uint8_t *temp) {
  *temp = temperature;
}

/*!
 *  @fn       readUmidity
 *
 *  @brief    L� a umidade armazenada no atributo do objeto sensor.
 *
 *  @param    umidity - vari�vel em que o valor de umidade
 *            obtido ser� armzenado.
 */
void mkl_DHT11Sensor::readUmidity(uint8_t *umid) {
  *umid = umidity;
}

/*!
 *  @fn       readSensorData
 *
 *  @brief    Armazena o tempo que o barramento ficou em n�vel l�gico alto
 *            em um vetor de inteiros.
 *
 *  @return   dht11_ok           - se as leituras forem realizadas com sucesso
 *            dht11_timeoutError - se n�o houver resposta do sensor.
 */
dht11_Exception mkl_DHT11Sensor::readSensorData() {
  int i;
  uint16_t time[40];

  /*!
   * Realiza a medi��o de 40 tempos (pois s�o 40 bits) em que o sinal
   * permaneceu em n�vel l�gico alto.
   */
  for (i = 0; i < 40; i++) {
    if (!wait(1)) {
      /*!
       * Houve transbordamento de tempo.
       */
      return dht11_timeoutError;
    }

    /*!
     * L� o tempo correspondente no perif�rico TPM.
     */
    tpm.getCounter(&time[i]);

    /*!
     * Aguarda enquanto o barramento permanece em '0'.
     */
    if (!wait(0)) {
      /*!
       * Houve transbordamento de tempo.
       */
      return dht11_timeoutError;
    }
  }
  return readPackage(time);
}

/*!
 *  @fn       wait
 *
 *  @brief    Mede o tempo em que o barramento ficou no n�vel l�gico passado
 *            por par�metro.
 *
 *  @param    bit - o bit que se deseja medir o tempo.
 *
 *  @return   0 - se houver transbordamento do fundo de escala
 *            do temporizador.
 *            1 - se n�o houver transbordamento do fundo de escala
 *            do temporizador.
 */
int mkl_DHT11Sensor::wait(int bit) {
  uint8_t data;

  /*!
   * Inicia uma temporiza��o.
   */
  tpm.startDelay(0xFFFF);

  /*!
   * Prende at� que o dado lido seja diferente daquele especificado
   * no par�metro ou at� que haja transbordamento do fundo de escala
   * do perif�rico TPM.
   */
  do {
    data = gpio.readBit();
  }while (!tpm.timeoutDelay() && data == bit);

  /*!
   * Verifica se ocorreu o transbordamento do tempo.
   */
  if (tpm.timeoutDelay() == 1) {
    return 0;
  }
  return 1;
}

/*!
 *  @fn       waitResponse
 *
 *  @brief    Aguarda a resposta do sensor ao comando inicial enviado.
 *
 *  @return   0 - se n�o resposta do sensor ao comando enviado.
 *            1 - se o sensor responder ao comando enviado.
 */
int mkl_DHT11Sensor::waitResponse() {
  if (!wait(1)) {
    return 0;
  }
  if (!wait(0)) {
    return 0;
  }
  if (!wait(1)) {
    return 0;
  }
  if (!wait(0)) {
    return 0;
  }
  return 1;
}

/*!
 *  @fn       startCommunication
 *
 *  @brief    Inicia a comunica��o com o sensor, escrevendo
 *            '0' no barramento por 20 ms.
 */
void mkl_DHT11Sensor::startCommunication() {
  int i;

  /*!
   * Configura o pino do GPIO para atuar como sa�da.
   */
  gpio.setPortMode(gpio_output);

  /*!
   * Escreve '0' na sa�da do GPIO e aguarda 20 ms.
   */
  gpio.writeBit(0);
  for (i = 0; i < 7; i++) {
    tpm.waitDelay(0xFFFF);
  }

  /*!
   * Escreve '1' na sa�da do GPIO e aguarda 1 ms.
   */
  gpio.writeBit(1);
  tpm.waitDelay(1000);
}

/*!
 *  @fn       checkSum
 *
 *  @brief    Verifica se a soma dos bytes de temperatura e umidade
 *            corresponde ao byte de paridade enviado.
 *
 *  @return   0 - se a soma dos bytes de temperatura e umidade
 *            corresponder ao byte de paridade enviado.
 *            1 - se a soma dos bytes de temperatura e umidade
 *            n�o corresponder ao byte de paridade enviado.
 */
int mkl_DHT11Sensor::checkSum() {
  if (temperature + umidity != verifyByte) {
    return 0;
  }
  return 1;
}
/*!
 *  @fn       readPackage
 *
 *  @brief    L� os tempos medidos, atribui o bit equivalente a cada tempo
 *            e armazena o valor de temperatura e umidade correspondente.
 *
 *  @return   dht11_ok           - se a paridade dos bytes de umidade e
 *                               temperatura corresponderem ao byte de
 *                               paridade enviado.
 *            dht11_parityError  - se a paridade dos bytes de umidade e
 *                               temperatura n�o corresponderem ao byte de
 *                               paridade enviado.
 */
dht11_Exception mkl_DHT11Sensor::readPackage(uint16_t time[40]) {
  int i;

  /*!
   * Inicializa as vari�veis de temperatura, umidade e de paridade.
   */
  umidity = 0;
  temperature = 0;
  verifyByte = 0;
  /*!
   * Verifica se o tempo armazenado � superior a 30 us.
   * Caso seja, o bit � assinalado como '1' e � realizado o deslocamento
   * para a esquerda na vari�vel de umidade e o bit '1' � inserido na �ltimo
   * bit da vari�vel.
   */
  for (i = 0; i < 8; i++) {
    umidity = umidity << 1;
    if (time[i] >= 1000) {
      umidity |= 1;
    }
  }
  /*!
   * Verifica se o tempo armazenado � superior a 30 us.
   * Caso seja, o bit � assinalado como '1' e � realizado o deslocamento
   * para a esquerda na vari�vel de temperatura e o bit '1' � inserido na �ltimo
   * bit da vari�vel.
   */
  for (i = 16; i < 24; i++) {
    temperature = temperature << 1;
    if (time[i] >= 1000) {
      temperature |= 1;
    }
  }

  /*!
   * Verifica se o tempo armazenado � superior a 30 us.
   * Caso seja, o bit � assinalado como '1' e � realizado o deslocamento
   * para a esquerda na vari�vel de paridade e o bit '1' � inserido na �ltimo
   * bit da vari�vel.
   */
  for (i = 32; i < 40; i++) {
    verifyByte = verifyByte << 1;
    if (time[i] >= 1000) {
      verifyByte |= 1;
    }
  }

  /*!
   * Realiza a verifica��o da paridade dos dados enviados pelo sensor.
   */
  if (!checkSum()) {
	/*!
	 * A paridade dos dados enviados n�o corresponde ao byte de paridade.
	 */
    return dht11_parityError;
  }
  return dht11_ok;
}
