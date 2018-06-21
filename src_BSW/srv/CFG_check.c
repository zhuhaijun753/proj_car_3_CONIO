/**
 * \file 	CFG_check.c
 * \author 	Thomas Barth 	Hochschule Darmstadt - thomas.barth@h-da.de
 * \date 	18.12.2016
 *
 * \brief Configuration integrity check
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

#include "CFG_check.h"

//-------------------------------------------------------------------- [includes for checker functions]
#include "CAN.h"
#include "CANopen.h"
//#include "InterruptRouter.h"


//-------------------------------------------------------------------- [local declarations]
/**
 * \brief performs casts and checks if the succeeded by reading data back
 *
 * \return  #RC_SUCCESS casts are valid
 * 			#RC_ERROR	some casts are invalid
 */
RC_t	__CFG_check_cast(void);


/** \brief CFG check function pointer type */
typedef RC_t (*__cfg_chec_func)( void );


//-------------------------------------------------------------------- [Configuration]

/** \brief List of functions to be called during #CFG_check
 *
 * \warning	Do not remove the last entry (has to be #NULL)
 */
__cfg_chec_func __cfg_chec_funcs[]={
		__CFG_check_cast,
		//InterruptRouter_CfgValidation,

		//Do not remove next line!
		NULL
};

//-------------------------------------------------------------------- [Application]

RC_t	CFG_check(void){

	//result buffer
	RC_t	__result;

	//Check function iterator
	uint32_t	__cfi=0;

	//perform checks
	while(__cfg_chec_funcs[__cfi]!=NULL){

		//call check function
		__result=__cfg_chec_funcs[__cfi]();

		//check if check succeeded, otherwise return result
		if(__result!=RC_SUCCESS)
			return __result;

		//go to next check
		__cfi++;
	}

	return RC_SUCCESS;

}


RC_t	__CFG_check_cast(void){

//Check 1: CAN cast

	//CANopen CAN frame format
	volatile CANopen_CAN_MSG_t	__CO_CAN_frame;

	//AURIX CAN frame format pointer
	volatile CAN_Frame_t*		p__CAN_frame;

	//fill CAN frame
	__CO_CAN_frame.COB_ID=0x11;
	__CO_CAN_frame.DLC=0x8;
	__CO_CAN_frame.data.u8[0]=0x5;
	__CO_CAN_frame.data.u8[7]=0x7;


	//cast
	p__CAN_frame=(CAN_Frame_t*)&__CO_CAN_frame;

	//check if cast succeeded
	if(	(__CO_CAN_frame.COB_ID!=	p__CAN_frame->MsgID)||
		(__CO_CAN_frame.DLC!=		p__CAN_frame->DLC)||
		(__CO_CAN_frame.data.u8[0]!=p__CAN_frame->dataL.B.DB0)||
		(__CO_CAN_frame.data.u8[7]!=p__CAN_frame->dataH.B.DB7))
		return RC_ERROR;


	//all casts succeeded
	return RC_SUCCESS;

}
