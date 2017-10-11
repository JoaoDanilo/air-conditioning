/*!
 * @copyright   � 2017 Universidade Federal do Amazonas.
 *
 * @brief       API em C++ para o perif�rico TPM, no modo delay.
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

#ifndef MKL_DELAY_OCP
#define MKL_DELAY_OCP

#include <stdint.h>
#include <MKL25Z4.h>
#include "../mkl_TPM.h"

/*!
 *  @class    mkl_TPMDelay.
 *
 *  @brief    A classe implementa o modo de opera��o delay, do perif�rico TPM.
 *
 *  @details  Esta classe � derivada da classe m�e "dsf_TPMPeripheral_ocp" e
 *            implementa o modo de opera��o delay, podendo o usu�rio escolher
 *            entre um delay que "prende" e um delay que "n�o prende".
 *
 *
 */
class mkl_TPMDelay : public mkl_TPM {
 public:
  /*!
   * Construtor padr�o da classe.
   */
  explicit mkl_TPMDelay(tpm_TPMNumberMask pin);
  /*!
   * M�todo de configura��o da classe.
   */
  void setFrequency(tpm_Div divBase);

  /*!
   * M�todos de inicializa��o de temporiza��o.
   */
  void waitDelay(uint16_t cycles);
  void startDelay(uint16_t cycles);

  /*!
   * M�todos de checagem da temporiza��o.
   */
  int timeoutDelay();
  void getCounter(uint16_t *value);

  /*!
   * M�todo de cancelamento de temporiza��o.
   */
  void cancelDelay();
};

#endif
