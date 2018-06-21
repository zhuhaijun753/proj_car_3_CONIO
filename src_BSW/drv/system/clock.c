/**
 * \file 	clock.c
 * \author 	Thomas Barth	-	thomas.barth@h-da.de
 * \author	Willi Theiﬂ		HighTec EDV Systeme GmbH
 * \date 	14.12.2016
 * \version	0.2
 *
 * \brief Clock tree configuration and frequency information
 *
 * Changelog:\n
 * 		0.2 14.12.2016	Barth
 * 				- removed "wait" function and use #SYSTEM_DelayUs instead
 *      0.1 12.12.2016	Barth
 *              - Initial documentation
 *
 * \todo Add PLLeray setup
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

#include "clock.h"
#include "clock_cfg.h"

#include "register.h"
#include <machine/wdtcon.h>

//-------------------------------------------------------------------- [local functions]

/** \brief perform the Clock initialization
 *
 * \param 	p_clock_cfg	reference to PLL configuration set
 * \return 	always #RC_SUCCESS
 */
static RC_t Clock_write_cfg(const clock_cfg_t *p_clock_cfg){

	//K2 and K3 divider buffers
	uint32_t __k2,__k3;

// prepare flash waitstates
    unlock_wdtcon();

    // Wait States for Error Correction of PFlash: Error correction takes 2 FSI2 clock cycle.
    FLASH0_FCON.B.WSECPF = p_clock_cfg->valWSECPF;

    // Wait States for read access to PFlash:  Read access takes 6 FSI2 cycles
    FLASH0_FCON.B.WSPFLASH = p_clock_cfg->valWSPFLASH;

// flash waitstates written
    lock_wdtcon();

// set up clock control unit
    unlock_safety_wdtcon();

    //set SCU_OSCCON
    SCU_OSCCON.U = p_clock_cfg->valOSCCON.U;

    /* busy wait for CCUCON1 to accept new configuration*/
    while (SCU_CCUCON1.B.LCK);

    //Set SCU_CCUCON1
    SCU_CCUCON1.U = p_clock_cfg->valCCUCON1.U | (1 << IFX_SCU_CCUCON1_UP_OFF);

    // busy wait for CCUCON2 to accept new configuration
    while (SCU_CCUCON2.B.LCK) ;

    //Set SCU_CCUCON2
    SCU_CCUCON2.U = p_clock_cfg->valCCUCON2.U | (1 << IFX_SCU_CCUCON2_UP_OFF);

    //CPU Dividers
    SCU_CCUCON6.U = p_clock_cfg->valCCUCON6.U;
    SCU_CCUCON7.U = p_clock_cfg->valCCUCON7.U;
    SCU_CCUCON8.U = p_clock_cfg->valCCUCON8.U;

    //Set up PLL, mainly dividers
    SCU_PLLCON0.U |= ((1 << IFX_SCU_PLLCON0_VCOBYP_OFF) | (1 << IFX_SCU_PLLCON0_SETFINDIS_OFF));
    SCU_PLLCON1.U =  p_clock_cfg->valPLLCON1.U;
    SCU_PLLCON0.U =  p_clock_cfg->valPLLCON0.U  | ((1 << IFX_SCU_PLLCON0_VCOBYP_OFF) | (1 << IFX_SCU_PLLCON0_CLRFINDIS_OFF));

    // busy wait for CCUCON0 to accept new configuration
    while (SCU_CCUCON0.B.LCK) ;

    //Set SCU_CCUCON
    SCU_CCUCON0.U =  p_clock_cfg->valCCUCON0.U | (1 << IFX_SCU_CCUCON0_UP_OFF);

// clock control unit is set up
    lock_safety_wdtcon();

    //if no prescaler mode is requested
    if (0 == (p_clock_cfg->valPLLCON0.U & (1 << IFX_SCU_PLLCON0_VCOBYP_OFF))){
    	// busy wait for PLL to accept new configuration, then disable VCO Bypass
    	while (0 == SCU_PLLSTAT.B.VCOLOCK) ;
        unlock_safety_wdtcon();
        SCU_PLLCON0.B.VCOBYP = 0;
        lock_safety_wdtcon();
    }

    // update K dividers
    __k2 = SCU_PLLCON1.B.K2DIV;
    __k3 = SCU_PLLCON1.B.K3DIV;

    // wait some time (100 us)
    SYSTEM_DelayUs(100);

    //step up to final clock
    while (__k2 > p_clock_cfg->finalK2 || __k3 > p_clock_cfg->finalK3){

    	//reload PLLCON1
        Ifx_SCU_PLLCON1 pllcon1 = SCU_PLLCON1;

        //set next iteration
        if(__k2 > p_clock_cfg->finalK2)   	--__k2;
        if(__k3 > p_clock_cfg->finalK3)    	--__k3;

        //write new K value
        pllcon1.B.K2DIV = __k2;
        pllcon1.B.K3DIV = __k3;

        // wait until K2 operation is stable
        while (0 == SCU_PLLSTAT.B.K2RDY) ;

        // Set SCU_PLLCON1
        unlock_safety_wdtcon();
        SCU_PLLCON1 = pllcon1;
        lock_safety_wdtcon();

        // wait some time (100 us)
        SYSTEM_DelayUs(100);
    }

    return RC_SUCCESS;
}


//-------------------------------------------------------------------- [global functions]

RC_t Clock_Init(){
    // Initialize PLL (only done by CPU0)
    if (cpu0 == (_mfcr(CPU_CORE_ID) & IFX_CPU_CORE_ID_CORE_ID_MSK))
        return Clock_write_cfg(&clk_cfg);
    else
    	return RC_ERROR;
}

unsigned long Clock_get_Frequency(Clock_frequency_t sub_clk){

	switch (sub_clk) {
		case fOSC:
			return CLK_fOSC;

		case fOSCref:
			return Clock_get_Frequency(fOSC)/(SCU_OSCCON.B.OSCVAL+1);

		case fREF:
			return Clock_get_Frequency(fOSC)/(SCU_PLLCON0.B.PDIV+1);

		case fVCO:
			return Clock_get_Frequency(fREF)*(SCU_PLLCON0.B.NDIV+1);

		case fK1:
			return Clock_get_Frequency(fOSC)/(SCU_PLLCON1.B.K1DIV+1);

		case fK2:
			return Clock_get_Frequency(fVCO)/(SCU_PLLCON1.B.K2DIV+1);

		case fK3:
			return Clock_get_Frequency(fVCO)/(SCU_PLLCON1.B.K3DIV+1);

		case fSource:
			switch(SCU_CCUCON0.B.CLKSEL){
				case 0:
					return Clock_get_Frequency(fBACK);

				case 1:
					return Clock_get_Frequency(fPLL);
			}
			return CLOCK_UNKNOWN;

		case fPLL:
			if (0 == (SCU_PLLSTAT.B.VCOBYST)) {
			    	//VCO is connected to fOSC
			        if (0 == (SCU_PLLSTAT.B.FINDIS))
			        	return Clock_get_Frequency(fK2);
			        else/* freerunning mode */
			        	return Clock_get_Frequency(fBASE)/(SCU_PLLCON1.B.K2DIV+1);
			    }
			    else /* Free-running mode VCO bypassed */
			    	return Clock_get_Frequency(fK1);
			break;

		case fPLLERAY:
			//TODO
			return CLOCK_UNKNOWN;

		case fBACK:
			return CLK_fBACK;

		case fBASE:
			return CLK_fBASE;

		case fBAUD1:
			return Clock_get_Frequency(fSource)/SCU_CCUCON0.B.BAUD1DIV;

		case fBAUD2:
			return Clock_get_Frequency(fSource)/SCU_CCUCON0.B.BAUD2DIV;

		case fSRI:
			return Clock_get_Frequency(fSource)/SCU_CCUCON0.B.SRIDIV;

		case fSPB:
			return Clock_get_Frequency(fSource)/SCU_CCUCON0.B.SPBDIV;

		case fFSI2:
			return Clock_get_Frequency(fSource)/SCU_CCUCON0.B.FSI2DIV;

		case fFSI:
			return Clock_get_Frequency(fSource)/SCU_CCUCON0.B.FSIDIV;

		case fCAN:
			return Clock_get_Frequency(fSource)/SCU_CCUCON1.B.CANDIV;

		case fERAY:
			return Clock_get_Frequency(fSource)/SCU_CCUCON1.B.ERAYDIV;

		case fSTM:
			return Clock_get_Frequency(fSource)/SCU_CCUCON1.B.STMDIV;

		case fGTM:
			return Clock_get_Frequency(fSource)/SCU_CCUCON1.B.GTMDIV;

		case fETH:
			return Clock_get_Frequency(fPLLERAY)/(SCU_CCUCON1.B.ETHDIV*4);

		case fASCLINF:
			return Clock_get_Frequency(fSource)/SCU_CCUCON1.B.ASCLINFDIV;

		case fASCLINS:
			return Clock_get_Frequency(fSource)/SCU_CCUCON1.B.ASCLINSDIV;

		case fMAX:
		    switch (SCU_CCUCON0.B.LPDIV) {
		    case 0:                    /*Not in low power mode */

		        if (SCU_CCUCON5.B.MAXDIV == 0)
		        	return Clock_get_Frequency(fSource);
		        else
		        	return Clock_get_Frequency(fSource) / SCU_CCUCON5.B.MAXDIV;

		    case 1:
		        return Clock_get_Frequency(fSource)/ 15;
		    case 2:
		    	 return Clock_get_Frequency(fSource)/ 30;
		    case 3:
		    	 return Clock_get_Frequency(fSource)/ 60;
		    case 4:
		    	 return Clock_get_Frequency(fSource)/ 120;
		    default:
		    	return 0;
		    }
		    return 0;
		    break;

		case fEBU:
			return Clock_get_Frequency(fSource)/SCU_CCUCON5.B.EBUDIV;

		case fBBB:
			return Clock_get_Frequency(fSource)/SCU_CCUCON2.B.BBBDIV;

		case fCPU0:
			return Clock_get_Frequency(fSRI)*((64-SCU_CCUCON6.B.CPU0DIV)/64);

		case fCPU1:
			return Clock_get_Frequency(fSRI)*((64-SCU_CCUCON7.B.CPU1DIV)/64);

		case fCPU2:
			return Clock_get_Frequency(fSRI)*((64-SCU_CCUCON8.B.CPU2DIV)/64);

	}

	//catch illegal input
	return CLOCK_UNKNOWN;

}

unsigned long Clock_GetCpuClock(void){
	return Clock_get_Frequency((Clock_frequency_t)(((uint8_t)fCPU0)+SYSTEM_GetCoreId()));
}


