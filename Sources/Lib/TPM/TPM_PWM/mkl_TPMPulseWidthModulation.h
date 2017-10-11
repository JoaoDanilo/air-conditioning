/*!
 * @copyright   � 2017 UFAM - Universidade Federal do Amazonas.
 *
 * @brief       Interface da classe "mkl_TPMPulseWidthModulation.h".
 *
 * @file        mkl_TPMPulseWidthModulation.h
 * @version     1.0
 * @date        02 Agosto 2017
 *
 * @section     HARDWARES & SOFTWARES
 *              +board        FRDM-KL25Z da NXP.
 *              +processor    MKL25Z128VLK4 - ARM Cortex-M0+.
 *              +compiler     Kinetis� Design Studio IDE.
 *              +manual       L25P80M48SF0RM, Rev.3, September 2012.
 *              +revisions    Vers�o (data): Descri��o breve.
 *                             ++ 1.0 (20 Agosto 2017): Vers�o inicial.
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

#ifndef MKL_TPMPulseWidthModulation_H
#define MKL_TPMPulseWidthModulation_H

#include "../mkl_TPM.h"
#include <stdint.h>
#include <MKL25Z4.h>

/*!
 *  @class    mkl_TPMPulseWidthModulation.
 *
 *  @brief    A classe implementa o servi�o PWM do perif�rico TPM.
 *
 *  @details  Esta classe implementa o servi�o PWM utilizando os
 *            perif�ricos TPM0, TPM1 ou TPM2 e os pinos correspondentes e
 *            herdando da classe m�e "mkl_TPM".
 *
 *  @section  EXAMPLES USAGE
 *
 *            Uso dos m�todos para gera��o de sinal PWM.
 *
 *              +fn setFrequency(tpm_div16, 999);
 *              +fn setDutyCycle(750);
 *              +fn enableOperation();
 */
class mkl_TPMPulseWidthModulation : public mkl_TPM {
 public:
  /*!
   * M�todo construtor padr�o da classe.
   */
  explicit mkl_TPMPulseWidthModulation(tpm_Pin pin = tpm_PTC1);

  /*!
   * M�todos de configura��o de frequ�ncia e duty cycle do sinal.
   */
  void setFrequency(tpm_Div divBase, uint16_t MODRegister);
  void setDutyCycle(uint16_t CnVRegister);

  /*!
   * M�todo de habilitar a opera��o PWM.
   */
  void enableOperation();

  /*!
   * M�todo de desabilitar a opera��o PWM.
   */
  void disableOperation();

 private:
  /*!
   * M�todo de sele��o do modo de opera��o PWM.
   */
  void setPWMOperation();
};

#endif  //  MKL_TPMPulseWidthModulation_H_
