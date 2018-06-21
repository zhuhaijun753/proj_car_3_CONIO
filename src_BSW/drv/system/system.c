/**
 * Project:	Student Car v3
 *
 * \file	system.c
 *
 * \author	Willi Theiﬂ		HighTec EDV Systeme GmbH
 * \author	Thomas Barth 	Hochschule Darmstadt 	- thomas.barth@h-da.de
 * \author	Martin Musiol 	Infineon				- martin.musiol@infineon.com
 *
 * \version	0.3 	14.10.2016
 *
 * \brief Basic system control API definition (based on PLS/HighTec original sources)
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
#include "system.h"
#include "clock.h"

#include "pxdef.h"
#include "DTS.h"


#define SYSTEM_CFG_DELAY_CMP	1	/**< \brief Defines the compare register (0 or 1) to be used by #SYSTEM_DelayUs. */
#define SYSTEM_CFG_DELAY_MIN	2	/**< \brief Smallest delay of #SYSTEM_DelayUs in µs */


//reference to the SCU
static Ifx_SCU * const pSCU = (Ifx_SCU *)&MODULE_SCU;

//***********************************************************************************//
//****************** Local prototypes ***********************************************//
//***********************************************************************************//

/*! \brief Disable the TLF
 */
static void disable_external_watchdog(void);



//***********************************************************************************//
//****************** Basic System functions *****************************************//
//***********************************************************************************//

RC_t SYSTEM_Init(void){
	RC_t 	__result;

	//Initialize clock tree
	__result=Clock_Init();

	if(__result!=RC_SUCCESS)
		return __result;

	//disable TLF
    disable_external_watchdog();

    //DTS init
    __result=DTS_initModule();
	if(__result!=RC_SUCCESS)
		return __result;

    return RC_SUCCESS;
}


void SYSTEM_DelayUs(uint32_t us_delay){

	volatile Ifx_STM* p_STM_Base;

	//check parameter.
	if(us_delay<SYSTEM_CFG_DELAY_MIN)
		return;

	//get pointer to the CPU-STM
	switch(SYSTEM_GetCoreId()){
		case cpu0:
			p_STM_Base=&MODULE_STM0;
			break;
		case cpu1:
			p_STM_Base=&MODULE_STM1;
			break;
		case cpu2:
			p_STM_Base=&MODULE_STM2;
			break;
		default:
			return;
	}



#if (SYSTEM_CFG_DELAY_CMP==0)
	//use 32 bits for the comparison
	p_STM_Base->CMCON.B.MSIZE0=0x1f;

	//the shift for the compare value within the 64bit STM
	p_STM_Base->CMCON.B.MSTART0=0;

	//get current STM value and write the new compare value. Substract 1 uS because of configuration overhead
	p_STM_Base->CMP[SYSTEM_CFG_DELAY_CMP].B.CMPVAL=(uint32_t)p_STM_Base->TIM0.B.STM31_0+((Clock_get_Frequency(fSTM)/1000000)*(us_delay));

	//reset Interrupt flag
	p_STM_Base->ISCR.B.CMP0IRR=BIT_1;

	//busy wait for Interrupt flag/compare match
	while(p_STM_Base->ICR.B.CMP0IR==BIT_0);
#elif(SYSTEM_CFG_DELAY_CMP==1)
	//use 32 bits for the comparison
	p_STM_Base->CMCON.B.MSIZE1=0x1f;

	//the shift for the compare value within the 64bit STM
	p_STM_Base->CMCON.B.MSTART1=0;

	//get current STM value and write the new compare value. Substract 1 uS because of configuration overhead
	p_STM_Base->CMP[SYSTEM_CFG_DELAY_CMP].B.CMPVAL=(uint32_t)p_STM_Base->TIM0.B.STM31_0+((Clock_get_Frequency(fSTM)/1000000)*(us_delay));

	//reset Interrupt flag
	p_STM_Base->ISCR.B.CMP1IRR=BIT_1;

	//busy wait for Interrupt flag/compare match
	while(p_STM_Base->ICR.B.CMP1IR==BIT_0);
#else
#error "Illegal  value of SYSTEM_CFG_DELAY_CMP"
#endif

}

// -----------------[ Basic system local functions ]---------------------------------//

//HACK: helper function. Needs to be replaced with QSPI driver
static void __send_SPI_msg(unsigned short data){

	QSPI2_DATAENTRY0.U = (unsigned int)data;
	/* wait until transfer is complete */
	while (!QSPI2_STATUS.B.TXF)
		;
	/* clear TX flag */
	QSPI2_FLAGSCLEAR.U = 1 << 9;
	/* wait for receive is finished */
	while (!QSPI2_STATUS.B.RXF)
		;
	/* clear RX flag */
	QSPI2_FLAGSCLEAR.U = 1 << 10;
	/* read and discard value */
	(void)QSPI2_RXEXIT.U;
}

static void disable_external_watchdog(void)
{

	int i;
	uint32_t tlf_version_response=0;


	// command sequence for disabling TLF A-Step
	const unsigned short wdtdiscmd_a_step[10] =
	{
		0x8756, 0x87de, 0x86ad, 0x8625, 0x8d27,
		0x8811, 0x87be, 0x8668, 0x877d, 0x8795
	};

	// command sequence for disabling TLF B-Step
	const unsigned short wdtdiscmd_b_step[10] =
	{
		0x8756, 0x87de, 0x86ad, 0x8625, 0x8d27,
		0x8A01, 0x87be, 0x8668, 0x877d, 0x8795
	};


    /* check that this disabling has not been already done (e.g. by the debugger) */
    if (QSPI2_GLOBALCON.B.EN)
    {
        /* don't do it again */
        return;
    }

	/* initialise QSPI2 interface */
	unlock_wdtcon();                /* remove ENDINIT protection */
	QSPI2_CLC.U = 0x8;              /* activate module, disable sleep mode */
	(void)QSPI2_CLC.U;              /* read back to get effective */
	P15_PDR0.U = 0x00000000;        /* fast speed (all pins) */
	P14_PDR0.U = 0x00000000;        /* fast speed (all pins) */
	QSPI2_PISEL.U = 1;              /* MRIS=1 ==> use MRST2B pin */
	lock_wdtcon();                  /* re-enable ENDINIT protection */

	/* configure port pins */
	P14_IOCR0.B.PC2 = 0x13;         /* SLSO21 */
	P15_IOCR0.B.PC3 = 0x13;         /* SCLK2 */
	P15_IOCR4.B.PC6 = 0x13;         /* MTSR2 */
	P15_IOCR4.B.PC7 = 0x02;         /* MRST2B */

	/* program QSPI2 parameters */
	QSPI2_GLOBALCON.U = 0x00003C04; /* EXPECT=15,SI=0, TQ=4 */
	QSPI2_GLOBALCON1.U = 0x14000000;/* RXFM=1,TXFM=1 (Single Move Mode for RX/TX) */
	QSPI2_SSOC.U = 0x00020000;      /* enable SLSO21, low active */
	QSPI2_ECON1.U = 0x501;          /* Q=1,A=0,B=1,C=1 */

	do
	{
		QSPI2_FLAGSCLEAR.U = 0xFFF; /* PT2F,PT1F,RXF,TXF,ERRORFLAGS */
	} while (QSPI2_STATUS.U & 0xFFF);

	/* prepare data transfer format */
	QSPI2_BACONENTRY.U = 0x17A10001;    /* CS=1,DL=15,MSB=1,TRAIL=1,LAST=1 */

	QSPI2_GLOBALCON.B.EN = 1;       /* ... and enable the module */

	//Check TLF Version

	__send_SPI_msg(0x6801);


	QSPI2_DATAENTRY0.U = 0x3e01;
	/* wait until transfer is complete */
	while (!QSPI2_STATUS.B.TXF)	;

	/* clear TX flag */
	QSPI2_FLAGSCLEAR.U = 1 << 9;

	/* wait for receive is finished */
	while (!QSPI2_STATUS.B.RXF)	;

	/* clear RX flag */
	QSPI2_FLAGSCLEAR.U = 1 << 10;

	/* read and discard value */
	tlf_version_response=QSPI2_RXEXIT.B.E;


	/* transfer all data */
	for (i = 0; i < 10; ++i)
	{
		if(tlf_version_response==0x8008)
			__send_SPI_msg((unsigned int)wdtdiscmd_b_step[i]);
		else
			__send_SPI_msg((unsigned int)wdtdiscmd_a_step[i]);

	}

	SYSTEM_DelayUs(60);

	//B/C Step
	if(tlf_version_response==0x8008){
		__send_SPI_msg(0xabd5);
		__send_SPI_msg(0xac2b);
	}
	//A-Step
	else{

		__send_SPI_msg(0x9dd5);
	}

}




//***********************************************************************************//
//****************** System *********************************************************//
//***********************************************************************************//

int SYSTEM_Reset(void){
	unlock_safety_wdtcon();
	SCU_RSTCON.B.SW = 0b01;			//System Reset Todo: To be checked if this works properly
    SCU_SWRSTCON.B.SWRSTREQ = 1;	//request a reset from the SCU
    while (1){/*wait for reset*/};
    return 0;
}

int SYSTEM_ResetFromPxros(void)
{
	PxArg_t err = _PxHndcall(SYSTEM_Reset, PxGetId(), 0);
	return 0;
}

int SYSTEM_Idle(void){
    return SYSTEM_IdleExt(_mfcr(CPU_CORE_ID) & IFX_CPU_CORE_ID_CORE_ID_MSK);
}

int SYSTEM_IdleExt(int CoreId){
    unlock_wdtcon();

	if(CoreId<DERIVATE_NUM_CORES)
		pSCU->PMCSR[CoreId].U=1;

    lock_wdtcon();
    return 0;
}

int SYSTEM_Sleep(void)
{
    unlock_wdtcon();
	pSCU->PMCSR[SYSTEM_GetCoreId()].U=2;
    lock_wdtcon();
    return 0;
}

void SYSTEM_DbgBreak(void){
    __asm volatile ("debug");
}


//***********************************************************************************//
//****************** Cache **********************************************************//
//***********************************************************************************//

int SYSTEM_IsCacheEnabled(void){
    unsigned int ui = _mfcr(CPU_PCON0);		//get PCON0 of current CPU
    if (ui & 2)
        return 0;   /* Cache is in bypass mode */

    ui = _mfcr(CPU_PCON2);					//get PCON2 of current CPU

    if (0 == (ui & (IFX_CPU_PCON2_PCACHE_SZE_MSK << IFX_CPU_PCON2_PCACHE_SZE_OFF)))
        return 0;   /* Cache size is 0 */
    return 1;
}

void SYSTEM_EnaDisCache(int Enable)
{
    unlock_wdtcon();					//Unlock ENDINIT

    //enable data and instruction cache
    if (Enable){
        _mtcr(CPU_PCON0, 0);
        _mtcr(CPU_DCON0, 0);
    }
    //disable data and instruction cache
    else{
        _mtcr(CPU_PCON0, 2);
        _mtcr(CPU_PCON1, 3);
        _mtcr(CPU_DCON0, 2);
    }

    lock_wdtcon();						//Lock ENDINIT
}

//***********************************************************************************//
//****************** Module Control *************************************************//
//***********************************************************************************//

RC_t SYSTEM_Module_CLC_Start(volatile CLC_Bits* p_module_clc, uint16_t timeout){
	uint16_t __timeout_ctr=0;

	//if no timeout is given, set default timeout
	if(timeout==0)
		timeout=CFG_SYSTEM_CLC_TIMEOUT;

	SYSTEM_UnlockENDINIT();
	p_module_clc->DISR=BIT_0;

	//wait for CLC to become enable (ENDINIT kicks in if it takes too long)
	while(p_module_clc->DISS!=BIT_0){
    	if(__timeout_ctr++>timeout)
    		return RC_ERROR_TIME_OUT;
    };

    //disable sleep mode
    p_module_clc->EDIS = BIT_1;
	SYSTEM_LockENDINIT();

	//Module was started
	return RC_SUCCESS;
}

RC_t SYSTEM_Module_KRESET(volatile KRST0_Bits* p_module_krst0, KRST1_Bits* p_module_krst1,  KRSTCLR_Bits* p_module_krstclr, uint16_t timeout){
	uint16_t __timeout_ctr=0;

	//if no timeout is given, set default timeout
	if(timeout==0)
		timeout=CFG_SYSTEM_KRST_TIMEOUT;

	SYSTEM_UnlockENDINIT();
	p_module_krst0->RST=BIT_1;
	p_module_krst1->RST=BIT_1;

	//wait for Kernel reset (ENDINIT kicks in if it takes too long)
	while(p_module_krst0->RSTSTAT!=BIT_1){
    	if(__timeout_ctr++>timeout)
    		return RC_ERROR_TIME_OUT;
    }

	//clear reset status
	p_module_krstclr->CLR = BIT_1;

	SYSTEM_LockENDINIT();

	//Module was reset
	return RC_SUCCESS;
}
