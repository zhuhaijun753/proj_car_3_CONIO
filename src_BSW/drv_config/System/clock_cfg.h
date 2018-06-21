/**
 * Project:	Student Car v3
 *
 * \file	clock_cfg.h
 *
 * \author	Thomas Barth 	Hochschule Darmstadt - thomas.barth@h-da.de
 *
 * \version	0.2 	29.06.2016
 *
 * \todo Add configuration for PLLEray
 * \todo Maybe outsource to derivate-specific file
 *
 * @brief Clock configuration parameters
 *
 *
 *	Changelog:
 *	- 0.2	29.06.2016	Barth
 *	- 	Moved from hardcoded configuration values towards #defines that hold the desired value.\n
 *		This allows to check the clock configuration for violations during the build.
 */

#ifndef CLOCK_CFG_H_
#define CLOCK_CFG_H_

#include "system.h"
#include "register.h"

//***********************************************************************************//
//****************** Static configuration of the board ******************************//
//***********************************************************************************//
#define CLK_fOSC     	(20000000)  /**< \brief external oscillator clock (20MHz)*/

//***********************************************************************************//
//****************** Clock configuration sets ***************************************//
//***********************************************************************************//
#define CLK_OSCVAL		7			/**< \brief Divider for the OSC Watchdog. fOSCref=CLK_fOSC/(CLK_OSCVAL+1)*/
#define CLK_PDIV		1			/**< \brief Divider that divides the external clock before it is fed into the PLL. fREF=CLK_fOSC/(CLK_PDIV+1)*/
#define CLK_NDIV		59			/**< \brief Divider in the feedback path of the Pll. fVCO=fREF*(CLK_NDIV+1)*/

#define CLK_MODE		1			/**< \brief PLL Mode: 1="normal", 2="prescaler", 3="freerunning*/
#define CLK_K1			2			/**< \brief Divider for CLK_fPLL if in "bypass"-mode. fK2=fOSC(CLK_K1+1)*/
#define CLK_K2			1			/**< \brief Divider for CLK_fPLL if in "normal"-mode. fK2=fVCO/(CLK_K2+1)*/
#define CLK_K3			1			/**< \brief Divider for CLK_fPLL2 . CLK_fPLL2=fVCO/(CLK_K3+1)*/

#define CLK_fSOURCE		(CLK_fPLL)		/**< \brief fSource selector, can be: CLK_fPLL, CLK_fBACK*/

#define	CLK_BAUD1DIV	3			/**< \brief Baud1 Divider Reload Value. Can be 0(off), 1,2,3,4,5,6,8,10,12,15. fBAUD1=fSOURCE/CLK_BAUD1DIV */
#define	CLK_BAUD2DIV	1			/**< \brief Baud2 Divider Reload Value. Can be 0(off), 1,2,3,4,5,6,8,10,12,15. fBAUD2=fSOURCE/CLK_BAUD2DIV */
#define	CLK_SRIDIV		1			/**< \brief SRI Divider Reload Value. Can be 0(off), 1,2,3,4,5,6,8,10,12,15. fSRI=fSOURCE/CLK_SRIDIV */
#define	CLK_LPDIV		0			//TODO: LPDIV. For now we want fMAX, fSRI, fSPB, and fBBB to be controlled by the dedicated CCUCONx bit fields.
#define	CLK_SPBDIV		3			/**< \brief SPB Divider Reload Value. Can be 2,3,4,5,6,8,10,12,15. fSPB=fSOURCE/CLK_SPBDIV */
#define	CLK_FSI2DIV		1			/**< \brief FSI2 Divider Reload Value. Can be 0(off) 1,2,3,4.  fFSI2=fSRI/CLK_FSI2DIV if CLK_SRIDIV 1 or 2, else fFSI2=fSRI*/
#define	CLK_FSIDIV		3			/**< \brief FSI Divider Reload Value. Can be 0(off) 1,2,3,4.  fFSI=fSRI/CLK_FSI2DIV if CLK_SRIDIV 1 or 2, else fFSI=fSRI*/

#define	CLK_CANDIV		3			/**< \brief CAN Divider Reload Value. Can be 0(off), 1,2,3,4,5,6,8,10,12,15. fCAN=fSOURCE/CLK_CANDIV */
#define	CLK_ERAYDIV		4			/**< \brief ERAY Divider Reload Value. Can be 0(off), 1,2,3,4,5,6,8,10,12,15. fERAY=fSOURCE/CLK_ERAYDIV */
#define	CLK_STMDIV		3			/**< \brief STM Divider Reload Value. Can be 0(off), 1,2,3,4,5,6,8,10,12,15. fSTM=fSOURCE/CLK_STMDIV */
#define	CLK_GTMDIV		3			/**< \brief GTM Divider Reload Value. Can be 0(off), 1,2,3,4,5,6,8,10,12,15. fSTM=fSOURCE/CLK_GTMDIV */
#define	CLK_ETHDIV		1			/**< \brief Ethernet Divider Reload Value. Can be 0(off), 1,2,3,4,5,6,8,10,12,15. fETH=CLK_fPLLERAY)/(CLK_ETHDIV*4) */
#define	CLK_ASCLINFDIV	1			/**< \brief ASCLIN Fast Divider Reload Value. Can be 0(off), 1,2,3,4,5,6,8,10,12,15. fASCLINF=fSOURCE/CLK_ASCLINFDIV */
#define	CLK_ASCLINSDIV	3			/**< \brief ASCLIN Slow Divider Reload Value. Can be 0(off), 1,2,3,4,5,6,8,10,12,15. fASCLINS=fSOURCE/CLK_ASCLINSDIV */

#define	CLK_MAXDIV		10			/**< \brief MAX Divider Reload Value. Can be 0(equals 1), 1,2,3,4,5,6,8,10,12,15. fMAX=fSOURCE/CLK_MAXDIV */
#define	CLK_EBUDIV		10			/**< \brief EBU Divider Reload Value. Can be 0(off), 1,2,3,4,5,6,8,10,12,15. fEBU=fSOURCE/CLK_EBUDIV */
#define	CLK_BBBDIV		2			/**< \brief BBB Divider Reload Value. Can be 0(off), 1,2,3,4,5,6,8,10,12,15. fBBB=fSOURCE/CLK_BBBDIV */

#define	CLK_CPU0DIV		0			/**< \brief CPU0 Divider Reload Value. fCPU0=fSRI*(64-CLK_CPU0DIV)/64 */
#define	CLK_CPU1DIV		0			/**< \brief CPU1 Divider Reload Value. fCPU1=fSRI*(64-CLK_CPU0DIV)/64 */
#define	CLK_CPU2DIV		0			/**< \brief CPU2 Divider Reload Value. fCPU2=fSRI*(64-CLK_CPU0DIV)/64 */


//***********************************************************************************//
//****************** Configuration Check and Clock Calculation **********************//
//***********************************************************************************//

#define CLK_fOSCref		(CLK_fOSC/(CLK_OSCVAL+1))				/**< \brief Resulting Frequency fOSCref*/
#define CLK_fREF		(CLK_fOSC/(CLK_PDIV+1))					/**< \brief Resulting Frequency fREF*/
#define CLK_fVCO		(CLK_fREF*(CLK_NDIV+1))					/**< \brief Resulting Frequency fVCO*/
#define CLK_fK1			(CLK_fOSC/(CLK_K1+1))					/**< \brief Resulting Frequency fK1*/
#define CLK_fK2			(CLK_fVCO/(CLK_K2+1))					/**< \brief Resulting Frequency fK2*/
#define CLK_fK3			(CLK_fVCO/(CLK_K3+1))					/**< \brief Resulting Frequency fK3*/

#define CLK_fBAUD1		(CLK_fSOURCE/CLK_BAUD1DIV)				/**< \brief Resulting Frequency fBaud1*/
#define CLK_fBAUD2		(CLK_fSOURCE/CLK_BAUD2DIV)				/**< \brief Resulting Frequency fBaud2*/
#define CLK_fSRI		(CLK_fSOURCE/CLK_SRIDIV)				/**< \brief Resulting Frequency fSRI*/
#define CLK_fSPB		(CLK_fSOURCE/CLK_SPBDIV)				/**< \brief Resulting Frequency fSPB*/
#define CLK_fFSI2		(CLK_fSOURCE/CLK_FSI2DIV)				/**< \brief Resulting Frequency fFSI2*/
#define CLK_fFSI		(CLK_fSOURCE/CLK_FSIDIV)				/**< \brief Resulting Frequency fFSI*/
#define CLK_fCAN		(CLK_fSOURCE/CLK_CANDIV)				/**< \brief Resulting Frequency fCAN*/
#define CLK_fERAY		(CLK_fSOURCE/CLK_ERAYDIV)				/**< \brief Resulting Frequency fERAY*/
#define CLK_fSTM		(CLK_fSOURCE/CLK_STMDIV)				/**< \brief Resulting Frequency fSTM*/
#define CLK_fGTM		(CLK_fSOURCE/CLK_GTMDIV)				/**< \brief Resulting Frequency fGTM*/
#define CLK_fETH		(CLK_fPLLERAY/(CLK_ETHDIV*4))			/**< \brief Resulting Frequency fETH*/
#define CLK_fASCLINF	(CLK_fSOURCE/CLK_ASCLINFDIV)			/**< \brief Resulting Frequency fASCLINF*/
#define CLK_fASCLINS	(CLK_fSOURCE/CLK_ASCLINSDIV)			/**< \brief Resulting Frequency fASCLINS*/

#define CLK_fMAX	   	(CLK_fSOURCE/CLK_MAXDIV)				/**< \brief Resulting Frequency fMAX*/
#define CLK_fEBU		(CLK_fSOURCE/CLK_EBUDIV)				/**< \brief Resulting Frequency fEBU*/
#define CLK_fBBB		(CLK_fSOURCE/CLK_BBBDIV)				/**< \brief Resulting Frequency fBBB*/

#define CLK_fCPU0		CLK_fSRI * ((64-CLK_CPU0DIV)/64)		/**< \brief Resulting Frequency fCPU0*/
#define CLK_fCPU1		CLK_fSRI * ((64-CLK_CPU1DIV)/64)		/**< \brief Resulting Frequency fCPU1*/
#define CLK_fCPU2		CLK_fSRI * ((64-CLK_CPU2DIV)/64)		/**< \brief Resulting Frequency fCPU2*/

/**\brief PFlash read access delay
 *
 * Minimum value: ceil(tPF*fFSI2)-1*/
#define CLK_WSPFLASH	_ceil_div(((CLK_fFSI2/100)*CLK_tPF),10000000)-1

/**\brief PFlash ECC decode delay
 *
 * Minimum value: ceil(tPFECC*fFSI2)-1 */
#define CLK_WSECPF		_ceil_div(((CLK_fFSI2/100)*CLK_tPFECC),10000000)-1

//"normal" Mode
#if CLK_MODE == 1

	#if CLK_fK2 > CLK_fPLL_MAX
		#error "Clocking error: CLK_fPLL to high!"
	#elif CLK_fK2 < CLK_fPLL_MIN
		#error "Clocking error: CLK_fPLL to low!"
	#else
		#define CLK_fPLL	CLK_fK2
	#endif

	#if CLK_fK3 > CLK_fPLL_MAX
		#error "Clocking error: CLK_fPLL to high!"
	#elif CLK_fK3 < CLK_fPLL_MIN
		#error "Clocking error: CLK_fPLL to low!"
	#else
		#define CLK_fPLL2	CLK_fK3
	#endif

//"Prescaler" Mode
#elif CLK_MODE == 2

	#if CLK_fK1 > CLK_fPLL_MAX
		#error "Clocking error: CLK_fPLL to high!"
	#elif CLK_fK2 < CLK_fPLL_MIN
		#error "Clocking error: CLK_fPLL to low!"
	#else
		#define CLK_fPLL	CLK_fK2
	#endif

	#if CLK_fK3 > CLK_fPLL_MAX
		#error "Clocking error: CLK_fPLL to high!"
	#elif CLK_fK3 < CLK_fPLL_MIN
		#error "Clocking error: CLK_fPLL to low!"
	#else
		#define CLK_fPLL2	CLK_fK3
	#endif

//"Freerunning" Mode
#elif CLK_MODE == 3
	#if (CLK_fBASE/CLK_K2) > CLK_fPLL_MAX
		#error "Clocking error: CLK_fPLL to high!"
	#elif (CLK_fBASE/CLK_K2) < CLK_fPLL_MIN
		#error "Clocking error: CLK_fPLL to low!"
	#else
		#define CLK_fPLL	(CLK_fBASE/CLK_K2)
	#endif

	#if (CLK_fBASE/CLK_K3) > CLK_fPLL_MAX
		#error "Clocking error: CLK_fPLL to high!"
	#elif (CLK_fBASE/CLK_K3) < CLK_fPLL_MIN
		#error "Clocking error: CLK_fPLL to low!"
	#else
		#define CLK_fPLL2	(CLK_fBASE/CLK_K3)
	#endif
#else
	#error "Unknown PLL mode"
#endif

#if CLK_fSOURCE !=CLK_fPLL && CLK_fSOURCE !=CLK_fBACK
	#error "Illegal fSOURCE selected"
#endif


#if CLK_fBAUD1 > CLK_fBAUD1_MAX
	#error "Clocking error: fBaud1 to high!"
#endif

#if CLK_fBAUD2 > CLK_fBAUD2_MAX
	#error "Clocking error: fBaud2 to high!"
#endif

#if CLK_fSRI > CLK_fSRI_MAX
	#error "Clocking error: fSRI to high!"
#endif

#if CLK_fSPB > CLK_fSPB_MAX
	#error "Clocking error: fSPB to high!"
#endif

#if CLK_fFSI2 > CLK_fFSI2_MAX
	#error "Clocking error: fFSI2 to high!"
#endif

#if CLK_fFSI > CLK_fFSI_MAX
	#error "Clocking error: fFSI to high!"
#endif

#if CLK_fCAN > CLK_fCAN_MAX
	#error "Clocking error: fCAN to high!"
#endif

#if CLK_fERAY > CLK_fERAY_MAX
	#error "Clocking error: fERAY to high!"
#endif

#if CLK_fSTM > CLK_fSTM_MAX
	#error "Clocking error: fSTM to high!"
#endif

#if CLK_fGTM > CLK_fGTM_MAX
	#error "Clocking error: fGTM to high!"
#endif

#if CLK_fASCLINF > CLK_fASCLINF_MAX
	#error "Clocking error: fASCLINF to high!"
#endif

#if CLK_fASCLINS > CLK_fASCLINS_MAX
	#error "Clocking error: fASCLINS to high!"
#endif

#if CLK_fMAX > CLK_fMAX_MAX
	#error "Clocking error: fMAX to high!"
#endif

#if CLK_fEBU > CLK_fEBU_MAX
	#error "Clocking error: fEBU to high!"
#endif

#if CLK_fBBB > CLK_fBBB_MAX
	#error "Clocking error: fBBB to high!"
#endif

#if CLK_fCPU0 > CLK_fCPU0_MAX
	#error "Clocking error: fCPU0 to high!"
#endif

#if CLK_fCPU1 > CLK_fCPU1_MAX
	#error "Clocking error: fCPU1 to high!"
#endif

#if CLK_fCPU2 > CLK_fCPU2_MAX
	#error "Clocking error: fCPU2 to high!"
#endif

//***********************************************************************************//
//****************** PLL Configuration structure  ***********************************//
//***********************************************************************************//

/**
 * \brief PLL configuration structure with all registers for SCU_OSCCON, SCU_PLLCON0/1, SCU_CCUCON0/1/2
 */
typedef struct
{
	Ifx_SCU_OSCCON 		valOSCCON;		/**< configuration for SCU_OSCCON*/
    Ifx_SCU_PLLCON0 	valPLLCON0;		/**< configuration for SCU_PLLCON0 */
    Ifx_SCU_PLLCON1 	valPLLCON1;    	/**< configuration for SCU_PLLCON1*/
    Ifx_SCU_CCUCON0 	valCCUCON0;		/**< configuration for SCU_CCUCON0*/
    Ifx_SCU_CCUCON1 	valCCUCON1;		/**< configuration for SCU_CCUCON1*/
    Ifx_SCU_CCUCON2 	valCCUCON2;		/**< configuration for SCU_CCUCON2*/
    Ifx_SCU_CCUCON6 	valCCUCON6;		/**< configuration for SCU_CCUCON6*/
    Ifx_SCU_CCUCON7 	valCCUCON7;		/**< configuration for SCU_CCUCON7*/
    Ifx_SCU_CCUCON8 	valCCUCON8;		/**< configuration for SCU_CCUCON8*/
    uint8_t 			finalK2;        /**< final K2DIV value */
    uint8_t 			finalK3;        /**< final K3DIV value */
    uint8_t				valWSPFLASH;	/**< Wait States for read access to PFlash (+1)*/
    uint8_t				valWSECPF;		/**< Wait States for Error Correction of PFlash (+1)*/
}clock_cfg_t;


//***********************************************************************************//
//****************** Global structure definition  ***********************************//
//***********************************************************************************//

#pragma section ".rodata"

/**
 * @brief Configuration for the PLL and the CCU (Clock control unit)
 */
const clock_cfg_t clk_cfg ={
//OSCCON
	{.B={
		0b0,  		/**< reserved_0:1;               \internal Reserved */
		0b0,  		/**< PLLLV:1;                    [1:1] Oscillator for PLL Valid Low Status Bit (rh) */
		0b1,  		/**< OSCRES:1;                   [2:2] Oscillator Watchdog Reset (w) 					1   		-> The Oscillator Watchdog of the PLL is cleared and restarted*/
		0b11,  		/**< GAINSEL:2;                  [4:3] Oscillator Gain Selection (rw) 					11  		-> The gain control is configured for frequencies from 4 MHz to 25 MHz*/
		0b00,  		/**< MODE:2;                     [6:5] Oscillator Mode (rw) 							00  		-> External Crystal / Ceramic Resonator Mode and External Input Clock Mode. The oscillator Power-Saving Mode is not entered.*/
		0b0,  		/**< SHBY:1;                     [7:7] Shaper Bypass (rw) 								0  			-> The shaper is not bypassed.*/
		0b0,  		/**< PLLHV:1;                    [8:8] Oscillator for PLL Valid High Status Bit (rh) */
		0b0,  		/**< reserved_9:1;               \internal Reserved */
		0b0,  		/**< X1D:1;                      [10:10] XTAL1 Data Value (rh) */
		0b0,  		/**< X1DEN:1;                    [11:11] XTAL1 Data Enable (rw) 						0  			-> Bit X1D is not updated.*/
		0b0000,		/**< reserved_12:4;              \internal Reserved */
		CLK_OSCVAL, /**< OSCVAL:5;                   [20:16] OSC Frequency Value (rw) 						#CLK_OSCVAL	-> This bit field defines the divider value that generates the reference clock that is supervised by the oscillator watchdog. fOSC is divided by OSCVAL + 1 in order to generate fOSCREF.*/
		0b00,  		/**< reserved_21:2;              \internal Reserved */
		0b0,  		/**< APREN:1;                    [23:23] Amplitude Regulation Enable (rw) 				0  			-> Amplitude Regulation is disabled*/
		0b0,  		/**< CAP0EN:1;                   [24:24] Capacitance 0 Enable (rw) 						0  			-> Capacitance CL0 is disabled*/
		0b0,  		/**< CAP1EN:1;                   [25:25] Capacitance 1 Enable (rw) 						0  			-> Capacitance CL1 is disabled*/
		0b0,  		/**< CAP2EN:1;                   [26:26] Capacitance 2 Enable (rw) 						0  			-> Capacitance CL2 is disabled*/
		0b0,  		/**< CAP3EN:1;                   [27:27] Capacitance 3 Enable (rw) 						0  			-> Capacitance CL3 is disabled*/
		0b0000, 	/**< reserved_28:4;              \internal Reserved */
	}},

//PLLCON0
	{.B={
		0b0,  		/**< VCOBYP:1;                   [0:0] VCO Bypass (rw) 									0  			-> Normal operation, VCO is not bypassed. CLK_fPLL=fK2	//TODO, make this variable based on the mode stated in CLK_MODE*/
		0b0,  		/**< VCOPWD:1;                   [1:1] VCO Power Saving Mode (rw)						0  			-> Normal behavior */
		0b0,  		/**< MODEN:1;                    [2:2] Modulation Enable (rw) 							0  			-> Frequency modulation is not activated*/
		0b0,  		/**< reserved_3:1;               \internal Reserved */
		0b0,  		/**< SETFINDIS:1;                [4:4] Set Status Bit PLLSTAT.FINDIS (w) 				0  			-> Bit PLLSTAT.FINDIS is left unchanged*/
		0b0,  		/**< CLRFINDIS:1;                [5:5] Clear Status Bit PLLSTAT.FINDIS (w) 				0  			-> Bit PLLSTAT.FINDIS is left unchanged*/
		0b0,  		/**< OSCDISCDIS:1;               [6:6] Oscillator Disconnect Disable (rw) 				0  			-> In case of a PLL loss-of-lock bit PLLSTAT.FINDIS is set*/
		0b00,  		/**< reserved_7:2;               \internal Reserved */
		CLK_NDIV,  	/**< NDIV:7;                     [15:9] N-Divider Value (rw) 							#CLK_NDIV 	-> The value the N-Divider operates is NDIV+1.*/
		0b1,  		/**< PLLPWD:1;                   [16:16] PLL Power Saving Mode (rw) 					1  			-> Normal behavior*/
		0b0,  		/**< reserved_17:1;              \internal Reserved */
		0b0,  		/**< RESLD:1;                    [18:18] Restart VCO Lock Detection (w) 				0  			-> Don't restart VCO Lock Detection*/
		0b00000,	/**< reserved_19:5;              \internal Reserved */
		CLK_PDIV,  	/**< PDIV:4;                     [27:24] P-Divider Value (rw) 							#CLK_PDIV 	-> The value the P-Divider operates is PDIV+1.*/
		0b0000,  	/**< reserved_28:4;              \internal Reserved */
	}},

//PLLCON1, note that K2 and K3 are only initial values here. Use #finalK2 and #finalK3 for the final K values.
	{.B={
		0x5,  		/**< K2DIV:7;                    [6:0] K2-Divider Value (rw)*/
		0b0,  		/**< reserved_7:1;               \internal Reserved */
		0x5,  		/**< K3DIV:7;                    [14:8] K3-Divider Value (rw) */
		0b0,  		/**< reserved_15:1;              \internal Reserved */
		CLK_K1,  	/**< K1DIV:7;                    [22:16] K1-Divider Value (rw) */
		0b0,  		/**< reserved_23:9;              \internal Reserved */
	}},

// CCUCON0
	{.B={
		CLK_BAUD1DIV,	/**< BAUD1DIV:4;                 [3:0] Baud1 Divider Reload Value (rw) */
		CLK_BAUD2DIV,  	/**< BAUD2DIV:4;                 [7:4] Baud2 Divider Reload Value (rw) */
		CLK_SRIDIV,  	/**< SRIDIV:4;                   [11:8] SRI Divider Reload Value (rw) */
		CLK_LPDIV,  	/**< LPDIV:4;                    [15:12] Low Power Divider Reload Value (rw) */
		CLK_SPBDIV,  	/**< SPBDIV:4;                   [19:16] SPB Divider Reload Value (rw) */
		CLK_FSI2DIV,  	/**< FSI2DIV:2;                  [21:20] FSI2 Divider Reload Value (rw) */
		0b00,  			/**< reserved_22:2;              \internal Reserved */
		CLK_FSIDIV,  	/**< FSIDIV:2;                   [25:24] FSI Divider Reload Value (rw) */
		0x00,  			/**< ADCCLKSEL:2;                [27:26] ADC Clock Selection (rw) TODO: not found in B-Step Manual! */
		0b01,  			/**< CLKSEL:2;                   [29:28] Clock Selection (rw) 							01  	-> CLK_fPLL is used as clock source fsource //TODO: make this based on the configuration */
		0b0,  			/**< UP:1;                       [30:30] Update Request (w) */
		0b0,  			/**< LCK:1;                      [31:31] Lock Status (rh) */
	}},

//CCUCON1
	{.B={
		CLK_CANDIV,  	/**< CANDIV:4;                   [3:0] MultiCAN Divider Reload Value (rw) */
		CLK_ERAYDIV,  	/**< ERAYDIV:4;                  [7:4] ERAY Divider Reload Value (rw) */
		CLK_STMDIV,  	/**< STMDIV:4;                   [11:8] STM Divider Reload Value (rw) */
		CLK_GTMDIV,  	/**< GTMDIV:4;                   [15:12] GTM Divider Reload Value (rw) */
		CLK_ETHDIV,  	/**< ETHDIV:4;                   [19:16] Ethernet Divider Reload Value (rw) */
		CLK_ASCLINFDIV, /**< ASCLINFDIV:4;               [23:20] ASCLIN Fast Divider Reload Value (rw) */
		CLK_ASCLINSDIV, /**< ASCLINSDIV:4;               [27:24] ASCLIN Slow Divider Reload Value (rw) */
		0b01,  			/**< INSEL:2;                    [29:28] Input Selection (rw) 							01  				-> fOSC0 is used as clock source for PLL and CLK_ERAY	//TODO: Try to understand this and set the value according to the configuration */
		0b0,  			/**< UP:1;                       [30:30] Update Request (w) */
		0b0,  			/**< LCK:1;                      [31:31] Lock Status (rh) */
	}},

//CCUCON2
	{.B={
		CLK_BBBDIV, 	/**< BBBDIV:4;              	[3:0] BBB Divider Reload Value (rw) */
		0b0,  			/**< reserved_4:26;              \internal Reserved */
		0b0,  			/**< UP:1;                       [30:30] Update Request (w) */
		0b0,  			/**< LCK:1;                      [31:31] Lock Status (rh) */
	}},

//CCUCON6
	{.B={
		CLK_CPU0DIV,  	/**< CPU0DIV:6;                 [5:0] CPU0 Divider Reload Value (rw) */
		0b0,  			/**< reserved_6:26;             \internal Reserved */
	}},

//CCUCON7
	{.B={
		CLK_CPU1DIV,  	/**< CPU1DIV:6;                 [5:0] CPU1 Divider Reload Value (rw) */
		0b0,  			/**< reserved_6:26;             \internal Reserved */
	}},

//CCUCON8
	{.B={
		CLK_CPU2DIV,  	/**< CPU2DIV:6;                 [5:0] CPU2 Divider Reload Value (rw) */
		0b0,  			/**< reserved_6:26;             \internal Reserved */
	}},

	CLK_K2,				/**< final K2 Value */
	CLK_K3,				/**< final K3 Value */
	CLK_WSPFLASH,
	CLK_WSECPF,

};

#pragma section
#endif /* CLOCK_CFG_H_ */
