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
 *                             ++ Jose Luis da Silva <joseluis70123@gmail.com>
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

#include <Lib/PIT/PIT_Delay/mkl_PITDelay.h>
#include <stdint.h>
#include <MKL25Z4.h>

mkl_PITDelay::mkl_PITDelay(PIT_ChPIT channel){
	      /*!
		   *Inicializa os atributos do canal.
		   */
			bindChannel(channel);

		  /*!
		   *Habilita a opera��o do m�dulo.
		   */
		  enablePeripheralClock();
}

void mkl_PITDelay::startDelay(){
	  /*!
	   *  Zera e inicializa a contagem de espera.
	   */
	  resetCounter();

	  /*!
	   *  Espera a contagem programada expirar.
	   */
	  waitInterruptFlag();

	  /*!
	   *  Limpa pedidos de interrup��o, por seguran�a.
	   */
	  clearInterruptFlag();
}

/*!
 *   @fn         cancelDelay
 *
 *   @brief      desabilita a opera��o delay.
 *
 *   Este m�todo desabilita a opera��o de contagem de um canal do Timer.
 *
 *   @remarks  Siglas e p�ginas do Manual de Refer�ncia KL25:
 *	       - PIT_TCTRLn: Timer Control Register. P�g. 579.
 */

void mkl_PITDelay::cancelDelay(){

*addrTCTRLn &= ~PIT_TCTRL_TEN_MASK;
}
/*!
 *   @fn       timeoutDelay
 *
 *   @brief    retorna verdadeiro quando ocorre uma interrup��o de um canal.
 *
 *   Este m�todo retorna verdadeiro caso o tempo de delay tenha chegado.
 *
 *   @return   True, caso haja uma interrup��o.
 *             False, caso ainda n�o tenha ocorrido uma interrup��o.
 *
 *   @remarks  Siglas e p�ginas do Manual de Refer�ncia KL25:
 *             - PIT_TFLGn: Timer Flag Register. P�g.580.
 */
bool mkl_PITDelay::timeoutDelay(){
	if ( *addrTFLGn && PIT_TFLG_TIF_MASK ) {
		    return true;
		 }
		    return false;
}

/*!
 *   @fn       getCounter
 *
 *   @brief    L� a contagem de um canal
 *
 *   Este m�todo retorna o valor corrente da contagem de um canal do timer.
 *
 *   @return   O valor do registrador que possui o valor corrente (CVALn) do canal.
 *
 *   @remarks  Siglas e p�ginas do Manual de Refer�ncia KL25:
 *             - PIT_CVALn: Current Timer Value Register. P�g.578.
 */
uint32_t mkl_PITDelay::getCounter(){
	 return *addrCVALn;
}

