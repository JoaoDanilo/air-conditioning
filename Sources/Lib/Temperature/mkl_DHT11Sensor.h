/*!
 * @copyright   � 2017 Universidade Federal do Amazonas.
 *
 * @brief       Interface da API em C++ do perif�rico sensor DHT11.
 * @file        mkl_DHT11Sensor.h
 * @version     1.0
 * @date        16 Julho 2017
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

#include <Lib/GPIO/GPIO_Port/mkl_GPIOPort.h>
#include <Lib/TPM/TPM_Delay/mkl_TPMDelay.h>
#include <stdint.h>

#ifndef MKL_DHT11Sensor_H_
#define MKL_DHT11Sensor_H_

typedef enum {
  dht11_parityError,
  dht11_timeoutError,
  dht11_ok
}dht11_Exception;

/*!
 *  @class    mkl_DHT11Sensor
 *
 *  @brief    A classe mkl_DHT11Sensor representa o perif�rico off-chip
 *            Sensor de Temperatura DHT11.
 *
 *  @details  Esta classe � usada a aquisi��o de dados de temperatura e umidade
 *            proveniente do sensor.
 *
 *  @section  EXAMPLES USAGE
 *
 *            Uso dos m�todos como porta de entrada
 *	           +fn doAcquisition();
 *	           +fn readUmidity(&umidade);
 *             +fn readTemperature(&temperatura);
 */
class mkl_DHT11Sensor {
 public:
  /*!
   * M�todo construtor padr�o da classe.
   */
  mkl_DHT11Sensor(tpm_TPMNumberMask TPMNumber, gpio_Pin pin);
  mkl_DHT11Sensor();
  /*!
   * M�todo de aquisi��o de dados.
   */
  dht11_Exception doAcquisition();
 /*!
  * M�todos de leitura de dados j� adquiridos.
  */
  void readTemperature(uint8_t *temp);
  void readUmidity(uint8_t *umid);

 private:
  /*!
   * Objeto de software da classe dsf_Delay_OCP.
   */
  mkl_TPMDelay tpm;

  /*!
   * Objeto de software da classe mkl_GPIOPort.
   */
  mkl_GPIOPort gpio;

  /*!
   * Atributos de umidade, temperatura e byte de paridade.
   */
  uint8_t umidity, temperature, verifyByte;

  /*!
   * M�todo de leitura do sensor.
   */
  dht11_Exception readSensorData();

  /*!
   * M�todos de aguardar a resposta do sensor.
   */
  int wait(int bit);
  int waitResponse();

  /*!
   * M�todo de iniciar a comunica��o com o sensor.
   */
  void startCommunication();

  /*!
   * M�todo de verificar a paridade dos dados enviados pelo sensor.
   */
  int checkSum();
  /*!
   * M�todo de associa��o do tempo em que o sinal permaneceu em n�vel alto
   * ao bit correspondente.
   */
  dht11_Exception readPackage(uint16_t time[40]);
};

#endif  // MKL_DHT11Sensor_H_
