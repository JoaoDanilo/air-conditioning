/*!
 * @copyright   � 2017 Universidade Federal do Amazonas.
 *
 * @brief       Interface da classe m�e "mkl_TPMMeasure".
 *
 * @file        mkl_TPMMeasure.h
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

#ifndef MKL_TPMMEASURE_H
#define MKL_TPMMEASURE_H

#include "../mkl_TPM.h"
#include <stdint.h>
#include <MKL25Z4.h>

/*!
 *  @class    mkl_TPMMeasure.
 *
 *  @brief    A classe implementa o servi�o de medi��o do perif�rico TPM.
 *
 *  @details  Esta classe implementa o servi�o de medi��o utilizando os
 *            perif�ricos TPM0, TPM1 ou TPM2 e os pinos correspondentes e
 *            herdando da classe m�e "mkl_TPM".
 *
 *  @section  EXAMPLES USAGE
 *
 *            Uso dos m�todos para medi��o at� a detec��o da borda de descida.
 *              +fn setFrequency(PMDiv_t::Div128);
 *              +fn setEdge(TPMEdge_t::Falling);
 *              +fn waitMeasure();
 *              +fn readData();
 *              +fn resetMeasure();
 */
class mkl_TPMMeasure : public mkl_TPM {
 public:
  /*!
   * M�todo construtor padr�o da classe.
   */
  explicit mkl_TPMMeasure(tpm_Pin pin);

  /*!
   * M�todos de configura��o do perif�rico.
   */
  void setFrequency(tpm_Div divBase);
  void setEdge(tpm_Edge edge);

  /*!
   * M�todos de habilita��o de medi��o.
   */
  void enableMeasure();
  void waitMeasure();

  /*!
   * M�todo de cancelamento de medi��o corrente.
   */
  void disableMeasure();

  /*!
   * M�todo de reset.
   */
  void resetMeasure();

  /*!
   * M�todos de leitura da medi��o e contagem.
   */
  int readData();
  int getCounter();

 private:
  /*!
   * Atributo de valor de medi��o realizada.
   */
  uint16_t measure;
};

#endif  //  MKL_TPMMEASURE_H_
