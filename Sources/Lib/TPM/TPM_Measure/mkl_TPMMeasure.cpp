/*!
 * @copyright   � 2017 Universidade Federal do Amazonas.
 *
 * @brief       Implementa��o da classe m�e "mkl_TPMMeasure".
 *
 * @file        mkl_TPMMeasure.cpp
 * @version     1.0
 * @date        06 Agosto 2017
 *
 * @section     HARDWARES & SOFTWARES
 *              +board        FRDM-KL25Z da NXP.
 *              +processor    MKL25Z128VLK4 - ARM Cortex-M0+.
 *              +compiler     Kinetis� Design Studio IDE.
 *              +manual       L25P80M48SF0RM, Rev.3, September 2012.
 *              +revisions    Vers�o (data): Descri��o breve.
 *                             ++ 1.0 (06 Agosto 2017): Vers�o inicial.
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

#include <Lib/TPM/TPM_Measure/mkl_TPMMeasure.h>
#include <stdint.h>
#include <MKL25Z4.h>

/*!
 *   @fn         mkl_TPMMeasure.
 *
 *   @brief      Construtor da classe mkl_TPMMeasure.
 *
 *   Este m�todo � o construtor padr�o da classe, que realiza a associa��o
 *   f�sica do perif�rico ao objeto de software, inicializa o perif�rico e os
 *   pinos associados.
 */


mkl_TPMMeasure::mkl_TPMMeasure(tpm_Pin pin) {
  uint8_t muxAltMask;
  uint8_t pinNumber;
  uint8_t GPIONumber;
  uint8_t chnNumber;
  uint8_t TPMNumber;
  uint8_t *baseAddress;

  setTPMParameters(pin, pinNumber, GPIONumber, chnNumber,
                   TPMNumber, muxAltMask);
  setBaseAddress(TPMNumber, &baseAddress);
  bindPeripheral(baseAddress);
  bindChannel(baseAddress, chnNumber);
  bindPin(GPIONumber, pinNumber);
  enablePeripheralClock(TPMNumber);
  enableGPIOClock(GPIONumber);
  selectMuxAlternative(muxAltMask);
}

/*!
 *   @fn         setFrequency.
 *
 *   @brief      Ajusta a frequ�ncia de opera��o.
 *
 *   Ajusta a frequ�ncia de opera��o para valores 1/2, 1/4, 1/8, 1/16,
 *   1/32, 1/64, 1/128 da frequ�ncia base (20,94 MHz).
 *
 *   @param[in]  divBase - fator de divis�o.
 *
 *   @remarks    Sigla e pagina do Manual de Referencia KL25:
 *               - TPMxSC: Status Control Register. P�g.552.
 */
void mkl_TPMMeasure::setFrequency(tpm_Div divBase) {
  /*!
   * Desabilita medi��o (recomenda��o do fabricante).
   */
  disableMeasure();
  /*!
   * Ajusta a frequ�ncia.
   */
  *addressTPMxSC = divBase;
}

/*!
 *   @fn         setEdge.
 *
 *   @brief      Ajusta a borda de opera��o.
 *
 *   Ao detectar a borda indicada no par�metro, para a contagem.
 *
 *   @param[in]  edge - borda de opera��o.
 *
 *   @remarks    Sigla e pagina do Manual de Referencia KL25:
 *               - TPMxMOD: Modulo Register. P�g. 554.
 *               - TPMxCnSC: Channel Status and Control Register. P�g.555.
 */
void mkl_TPMMeasure::setEdge(tpm_Edge edge) {
  /*!
   * Desabilita medi��o (recomenda��o do fabricante).
   */
  disableMeasure();
  /*!
   * Ajusta o fundo de escala do contador para 0xFFFF.
   */
  *addressTPMxMOD = 0xFFFF;
  /*!
   * Ajusta a borda de detec��o no registrador CnSC.
   */
  *addressTPMxCnSC = edge << 2;
}

/*!
 *   @fn         enableMeasure.
 *
 *   @brief      Habilita a opera��o de medi��o.
 *
 *   Este m�todo inicia a medi��o do tempo em que o sinal de entrada permanece
 *   em n�vel alto ou baixo, conforme o par�metro "edge".
 *   Note que este m�todo n�o "prende", i.e., o programa n�o fica preso neste
 *   m�todo at� a identifica��o da borda indicada.
 *
 *   @remarks    Sigla e pagina do Manual de Referencia KL25:
 *               - TPMxSC: Status Control Register. P�g.552.
 */
void mkl_TPMMeasure::enableMeasure() {
  /*!
   * Habilita a contagem escrevendo "10" no campo CMOD (Clock Mode Selection)
   * do registrador SC.
   */
  *addressTPMxSC |= 0x08;
}

/*!
 *   @fn         waitMeasure.
 *
 *   @brief      Habilita a opera��o de medi��o e aguarda a transi��o indicada.
 *
 *   Este m�todo inicia a medi��o do tempo em que o sinal de entrada permanece
 *   em n�vel alto ou baixo, conforme o par�metro "edge".
 *   Note que este m�todo "prende", i.e., o programa fica preso neste
 *   m�todo at� a identifica��o da borda indicada.
 *
 *   @remarks    Sigla e pagina do Manual de Referencia KL25:
 *               - TPMxSC: Status Control Register. P�g.552.
 *               - TPMxCnSC: Channel Status and Control Register. P�g.555.
 */
void mkl_TPMMeasure::waitMeasure() {
  enableMeasure();
  /*!
   * Aguarda at� que a flag CHF de detec��o de borda seja sinalizada.
   */
  while (!(*addressTPMxCnSC & 0x80)) {}
  /*!
   * Limpa a flag CHF.
   */
  *addressTPMxCnSC |= 0x80;
  /*!
   * Armazena o valor medido.
   */
  measure = *addressTPMxCnV;
}

/*!
 *   @fn         disableMeasure.
 *
 *   @brief      Desabilita a opera��o de medi��o.
 *
 *   @remarks    Sigla e pagina do Manual de Referencia KL25:
 *               - TPMxSC: Status Control Register. P�g.552.
 */
void mkl_TPMMeasure::disableMeasure() {
  *addressTPMxSC &= ~0x08;
}

/*!
 *   @fn         resetMeasure.
 *
 *   @brief      Reseta a contagem.
 *
 *   Este m�todo desabilita a opera��o de medi��o e ap�s reseta o contador
 *   de medi��o.
 *
 *   @remarks    Sigla e pagina do Manual de Referencia KL25:
 *               - TPMxSC: Status Control Register. P�g.552.
 *               - TPMxCNT: Counter Register. P�g.553.
 *
 *   @warning    Este m�todo desabilita a opera��o de medi��o.
 */
void mkl_TPMMeasure::resetMeasure() {
  /*!
   * Desabilita a opera��o de contagem.
   */
  *addressTPMxSC &= ~0x08;
  /*!
   * Limpa o contador.
   */
  *addressTPMxCNT = 0;
}

/*!
 *   @fn         readData.
 *
 *   @brief      L� o valor da medi��o iniciada no m�todo "waitMeasure".
 *
 *   Este m�todo retorna o valor da medi��o que o usu�rio iniciou no m�todo
 *   "waitMeasure".
 *
 *   @return     measure - valor da medi��o.
 */
int mkl_TPMMeasure::readData() {
  return measure;
}

/*!
 *   @fn         getCounter.
 *
 *   @brief      L� o valor da medi��o iniciada no m�todo "enableMeasure".
 *
 *   Este m�todo retorna o valor da medi��o que o usu�rio iniciou no m�todo
 *   "enableMeasure".
 *
 *   @return     addressTPMxCNT - valor de contagem do contador CNT.
 */
int mkl_TPMMeasure::getCounter() {
  return *addressTPMxCNT;
}
