/*!
 * @copyright   � 2017 UFAM - Universidade Federal do Amazonas.
 *
 * @brief       Interface de programa��o de aplica��es em C++ para  o Periodic interrupt Timer (MKL25Z).
 *
 * @file        mkl_PITPeriodicInterrupt.cpp
 * @version     1.0
 * @date        6 Julho 2017
 *
 * @section     HARDWARES & SOFTWARES
 *              +board        FRDM-KL25Z da NXP.
 *              +processor    MKL25Z128VLK4 - ARM Cortex-M0+.
 *              +peripheral   PIT - Periodic Interrupt Timer.
 *              +compiler     Kinetis� Design Studio IDE
 *              +manual       L25P80M48SF0RM, Rev.3, September 2012
 *              +revisions    Vers�o (data): Descri��o breve.
 *                             ++ 1.0 (6 Julho 2017): Vers�o inicial.
 *
 * @section     AUTHORS & DEVELOPERS
 *              +institution  Universidade Federal do Amazonas
 *              +courses      Engenharia da Computa��o / Engenharia El�trica
 *              +teacher      Miguel Grimm <miguelgrimm@gmail.com>
 *              +student      Vers�o inicial:
 *                             ++ Jose Luis da Silva e Silva <joseluis70123@gmail.com>
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

#include <Lib/PIT/PITPeriodicInterrupt/mkl_PITPeriodicInterrupt.h>
#include <stdint.h>
#include <MKL25Z4.h>

/*!
 *   @fn         mkl_PITInterruptInterrupt
 *
 *   @brief      Inicializa o perif�rico PIT.
 *
 *   Este m�todo construtor associa ao objeto o canal do parametro "channel"
 *
 *   @param[in]  channel - canal dsf_Ch0_PIT ou canal dsf_Ch1_PIT usado.
 */

mkl_PITInterruptInterrupt::mkl_PITInterruptInterrupt(PIT_ChPIT channel){
	 /*!
	   *  Inicializa os atributos do canal.
	   */
		bindChannel(channel);

	  /*!
	   *  Habilita a opera��o do m�dulo.
	   */
	  enablePeripheralClock();
}

mkl_PITInterruptInterrupt::mkl_PITInterruptInterrupt(){


}


/*!
 *   @fn         enableInterruptRequests
 *
 *   @brief      Habilita pedidos de interrup��es de um canal
 *
 *   Este m�todo habilita a gera��o de pedidos de interrup��es de um canal e habilita tamb�m
 *   a entrada de interrup��o do PIT no gerenciador de interrup��es (NVIC), o que ativa a
 *   chamada da rotina de servi�o de interrup��o (ISR) do vetor de interrup��es do NVIC.
 *
 *   @remarks  Siglas e p�ginas do Manual de Refer�ncia KL25:
 *	           - PIT_TCTRLn: Timer Control Register. P�g. 579.
 *             - NVIC: Nested Vectored Interrupt Controller. P�g. 51.
 */

void mkl_PITInterruptInterrupt::enableInterruptRequests(){
	/*!
	   * Habilita a gera��o de pedidos de interrp��es.
	   */
	  *addrTCTRLn |= PIT_TCTRL_TIE_MASK;

	  /*!
	   * Habilita a entrada de interrup��es do PIT no NVIC.
	   */
	  NVIC_EnableIRQ(PIT_IRQn);
}

/*!
 *   @fn         disableInterruptRequests
 *
 *   @brief      Desabilita pedidos de interrup��es de um canal
 *
 *   Este m�todo desabilita o pedido de interrup��es de um canal e desabilita tamb�m a entrada
 *   de interrup��o do PIT no gerenciador de interrup��es (NVIC), o que desativa a chamada da
 *   rotina de servi�o de interrup��o (ISR) do vetor de interrup��es do NVIC.
 *
 *   @remarks  Siglas e p�ginas do Manual de Refer�ncia KL25:
 *             - PIT_TCTRLn: Timer Control Register. P�g. 579.
 *             - NVIC: Nested Vectored Interrupt Controller. P�g. 51.
 */

void mkl_PITInterruptInterrupt::disableInterruptRequests(){
	/*!
	   *  Desabilita a gera��o de pedidos de interrp��es.
	   */
	  *addrTCTRLn &= ~PIT_TCTRL_TIE_MASK;

	  /*!
	   * Desabilita a entrada de interrup��es do PIT no NVIC.
	   */
	  //NVIC_DisableIRQ(PIT_IRQn);
}

