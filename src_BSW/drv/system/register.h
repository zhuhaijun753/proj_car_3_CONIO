/**
 * Project:	Student Car v3
 *
 * \file	system.h
 * \todo 	Add #ifdef directives for modules
 * \author	Dilip
 * 			Thomas Barth 	Hochschule Darmstadt - thomas.barth@h-da.de
 *
 * \version	0.1 	04.05.2016
 *
 * \brief System register include
 *
 *	Changelog:
 *
 * \copyright Copyright ©2016
 * Department of electrical engineering and information technology, Hochschule Darmstadt - University of applied sciences (h_da). All Rights Reserved.
 * Permission to use, copy, modify, and distribute this software and its documentation for educational, and research purposes in the context of non-commercial
 * (unless permitted by h_da) and official h_da projects, is hereby granted for enrolled students of h_da, provided that the above copyright notice,
 * this paragraph and the following paragraph appear in all copies, modifications, and distributions.
 * Contact Prof.Dr.-Ing. Peter Fromm, peter.fromm@h-da.de, Birkenweg 8 64295 Darmstadt - GERMANY for commercial requests.
 *
 * \warning This software is a PROTOTYPE version and is not designed or intended for use in production, especially not for safety-critical applications!
 * The user represents and warrants that it will NOT use or redistribute the Software for such purposes.
 * This prototype is for research purposes only. This software is provided "AS IS," without a warranty of any kind.
 */

#ifndef REGISTER_H_
#define REGISTER_H_

#include "global.h"

#include TC_INCLUDE(TCPATH/IfxScu_reg.h)

#include TC_INCLUDE(TCPATH/IfxAsclin_bf.h)
#include TC_INCLUDE(TCPATH/IfxAsclin_reg.h)
#include TC_INCLUDE(TCPATH/IfxAsclin_regdef.h)

#include TC_INCLUDE(TCPATH/IfxCpu_bf.h)
#include TC_INCLUDE(TCPATH/IfxCpu_reg.h)
#include TC_INCLUDE(TCPATH/IfxCpu_regdef.h)



#include TC_INCLUDE(TCPATH/IfxFlash_bf.h)
#include TC_INCLUDE(TCPATH/IfxFlash_reg.h)
#include TC_INCLUDE(TCPATH/IfxFlash_regdef.h)

#ifdef INT_INCLUDE
#include TC_INCLUDE(TCPATH/IfxInt_bf.h)
#include TC_INCLUDE(TCPATH/IfxInt_reg.h)
#include TC_INCLUDE(TCPATH/IfxInt_regdef.h)
#endif


#include TC_INCLUDE(TCPATH/IfxPort_bf.h)
#include TC_INCLUDE(TCPATH/IfxPort_reg.h)
#include TC_INCLUDE(TCPATH/IfxPort_regdef.h)


#include TC_INCLUDE(TCPATH/IfxQspi_bf.h)
#include TC_INCLUDE(TCPATH/IfxQspi_reg.h)
#include TC_INCLUDE(TCPATH/IfxQspi_regdef.h)


#include TC_INCLUDE(TCPATH/IfxScu_bf.h)
#include TC_INCLUDE(TCPATH/IfxScu_reg.h)
#include TC_INCLUDE(TCPATH/IfxScu_regdef.h)


#include TC_INCLUDE(TCPATH/IfxSrc_bf.h)
#include TC_INCLUDE(TCPATH/IfxSrc_reg.h)
#include TC_INCLUDE(TCPATH/IfxSrc_regdef.h)


#include TC_INCLUDE(TCPATH/IfxStm_bf.h)
#include TC_INCLUDE(TCPATH/IfxStm_reg.h)
#include TC_INCLUDE(TCPATH/IfxStm_regdef.h)

#include TC_INCLUDE(TCPATH/IfxCan_bf.h)
#include TC_INCLUDE(TCPATH/IfxCan_reg.h)
#include TC_INCLUDE(TCPATH/IfxCan_regdef.h)

#include TC_INCLUDE(TCPATH/IfxGpt12_bf.h)
#include TC_INCLUDE(TCPATH/IfxGpt12_reg.h)
#include TC_INCLUDE(TCPATH/IfxGpt12_regdef.h)

#include TC_INCLUDE(TCPATH/IfxI2c_bf.h)
#include TC_INCLUDE(TCPATH/IfxI2c_reg.h)
#include TC_INCLUDE(TCPATH/IfxI2c_regdef.h)

#endif /* REGISTER_H_ */
