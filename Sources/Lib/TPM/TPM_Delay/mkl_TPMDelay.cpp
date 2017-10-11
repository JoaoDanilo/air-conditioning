/*!
 * @copyright   � 2017 Universidade Federal do Amazonas.
 *
 * @brief       Implementa��o da API em C++ para o TPM, no modo delay.
 *
 * @file        mkl_TPMDelay.h
 * @version     1.0
 * @date        31 Julho 2017
 *
 * @section     HARDWARES & SOFTWARES
 *              +board        FRDM-KL25Z da NXP.
 *              +processor    MKL25Z128VLK4 - ARM Cortex-M0+.
 *              +compiler     Kinetis� Design Studio IDE.
 *              +manual       L25P80M48SF0RM, Rev.3, September 2012.
 *              +revisions    Vers�o (data): Descri��o breve.
 *                             ++ 1.0 (6 Julho 2017): Vers�o inicial.
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

#include <Lib/TPM/TPM_Delay/mkl_TPMDelay.h>
#include <stdint.h>

  /*!
   *   @fn       mkl_TPMDelay
   *
   *   @brief    M�todo construtor da classe.
   *
   *   M�todo construtor da classe, que inicializa os atributos do objeto.
   *
   *   @param[in]  tpm - perif�rico TPM a ser associado ao objeto de software.
   */
mkl_TPMDelay::mkl_TPMDelay(tpm_TPMNumberMask tpmMask) {
  uint8_t *baseAddress;
  uint8_t tpm;

  tpm = tpmMask << 11;
  baseAddress = (uint8_t *)(TPM0_BASE + 0x1000*tpm);
  bindPeripheral(baseAddress);
  enablePeripheralClock(tpm);
}

  /*!
   *   @fn       setFrequency
   *
   *   @brief    M�todo de ajuste da frequ�ncia de opera��o.
   *
   *   M�todo que ajusta a frequ�ncia de opera��o do delay.
   *
   *   @param[in]  divBase - constante de divis�o do divisor de frequ�ncia.
   */
void mkl_TPMDelay::setFrequency(tpm_Div divBase) {
  cancelDelay();
  *addressTPMxSC = divBase;
}

  /*!
   *   @fn       startDelay
   *
   *   @brief    Inicia a opera��o do perif�rico TPM no modo delay.
   *
   *   M�todo que inicia um delay de dura��o especificada.
   *
   *   @param[in] cycles - n�mero de ciclos de rel�gio da opera��o.
   *
   *   @details   Este m�todo n�o "prende", i.e., o m�todo apenas inicia a
   *              contagem do temporizador.
   *
   *              O par�metro "cycles" n�o corresponde ao tempo, em segundos,
   *              da temporiza��o iniciada.
   *              A equa��o que relaciona o par�metro "cycles" ao tempo,
   *              em segundos, � dada por:
   *
   *              cycles = T*(20.97*10^6)/divBase,
   *
   *              onde:
   *              - T       : tempo em segundos;
   *              - divBase : fator de divis�o configurado no m�todo
   *                          setFrequency;
   *              - cycles  : ciclos de rel�gio.
   *
   *              Note o maior valor do par�metro dever� ser de 65535, que
   *              � o maior valor em decimal que se obt�m com 16 bits.
   *              65535 � o fundo de escala do registrador TPMCNT.
   */
void mkl_TPMDelay::startDelay(uint16_t cycles) {
  /*!
  * Desabilita a contagem.
  */
  *addressTPMxSC &= ~0x08;
  /*!
  * Reseta o contador CNT.
  */
  *addressTPMxCNT = 0;
  /*!
  * Ajusta o fundo de escala do registrador "Modulo".
  */
  *addressTPMxMOD = cycles;
  /*!
  * Limpa a flag de t�rmino TOF do registrador Status Counter.
  */
  *addressTPMxSC |= 0x80;
  /*!
  * Habilita a contagem.
  */
  *addressTPMxSC |= 0x08;
}

  /*!
   *   @fn       timeoutDelay
   *
   *   @brief    Indica a ocorr�ncia do t�rmino de temporiza��o.
   *
   *   M�todo que indica se o tempo especificado pelo
   *
   *   @param[in]  divBase - constante de divis�o do divisor de frequ�ncia.
   */
int mkl_TPMDelay::timeoutDelay() {
  if (*addressTPMxSC & 0x80) {
    return 1;
  }
  return 0;
}

  /*!
   *   @fn       waitDelay
   *
   *   @brief    Inicia a opera��o do perif�rico TPM no modo delay.
   *
   *   M�todo que inicia um delay de dura��o especificada.
   *
   *   @param[in] cycles - n�mero de ciclos de rel�gio da opera��o.
   *
   *   @details   Este m�todo aguarda at� que o valor de temporiza��o desejado
   *              se esgote.
   *
   *              O par�metro "cycles" n�o corresponde ao tempo, em segundos,
   *              da temporiza��o iniciada.
   *              A equa��o que relaciona o par�metro "cycles" ao tempo,
   *              em segundos, � dada por:
   *
   *              cycles = T*(20.97*10^6)/divBase,
   *
   *              onde:
   *              - T       : tempo em segundos;
   *              - divBase : fator de divis�o configurado no m�todo
   *                          setFrequency;
   *              - cycles  : ciclos de rel�gio.
   *
   *              Note o maior valor do par�metro dever� ser de 65535, que
   *              � o maior valor em decimal que se obt�m com 16 bits.
   *              65535 � o fundo de escala do registrador TPMCNT.
   */
void mkl_TPMDelay::waitDelay(uint16_t cycles) {
  startDelay(cycles);
  do {} while (timeoutDelay() != 1);
}

  /*!
   *   @fn       cancelDelay
   *
   *   @brief    Cancela uma temporiza��o em andamento.
   *
   *   M�todo que cancela uma temporiza��o iniciada, parando o contador.
   */
void mkl_TPMDelay::cancelDelay() {
  /*!
  * Desabilita a contagem.
  */
  *addressTPMxSC &= ~0x08;
}

  /*!
   *   @fn       getCounter
   *
   *   @brief    M�todo que informa o valor atual da contagem.
   *
   *   M�todo que informa o valor atual do contador do temporizador.
   *
   *   @param[in]  value - vari�vel passada por refer�ncia que ser� atualizada
   *                       com o valor corrente do contador do temporizador.
   */
void mkl_TPMDelay::getCounter(uint16_t *value) {
  *value = *addressTPMxCNT;
}
