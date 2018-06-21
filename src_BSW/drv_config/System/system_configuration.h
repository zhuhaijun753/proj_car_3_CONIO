/**
 * \file system_configuration.h
 * \author Dilip
 * \date 	17.09.2016
 *
 * \version	0.2 	06.10.2016
 *
 * \brief system_configuration
 *
 * In this file ASCLIN and CAN ports enabling configuration is available
 *
 *  Changelog:\n
 *	- 0.2	06.10.2016	- Barth
 *		- Added define to select TLF Version
 *  - 0.1 	17.8.2016	- Gunaseelan\n
 *            -  ASCLIN1 port is assigned to UART2 channel
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

#ifndef SYSTEM_CONFIGURATION_H_
#define SYSTEM_CONFIGURATION_H_

/************************
 * Global Configuration
 ***********************/
#define CFG_SYSTEM_CLC_TIMEOUT	0x200		/**< \brief Timeout (in cycles) for CLC (Clock control) activation. */
#define CFG_SYSTEM_KRST_TIMEOUT	0x200		/**< \brief Timeout (in cycles) for Module kernel reset. */

/************************
 * General defines
 ***********************/
#define ENABLE 		0x0F

#define DISABLE 	0xFF

#define UARTUSB 	0
#define UART1 		1
#define UART2 		2
#define UART3 		3
#define UART4 		4
#define UART5 		5
#define UART6 		6
/************************
 * UART Configuration
 ***********************/
/*
 * Steps to configure UART PORT
 * Files - Port_cfg.c , System_configuration.h, UART_Config.cpp
 * 1. Enable the UART port you wish to use below. CHeck the hardware pin configuration on the board for possible conflicts.
 * 2. Based your UART channel preference here, Pre-configured PORT-PINS are selected using Preprocessors in Port_cfg.c
 * 	  For details about the ports and pins please refer UART.H or Port_cfg.c
 * 3. Please configure the baud rates, Parity type, stop bits etc. in Uart_config.cpp
 * 	  NOTE: Configuration you do in UART_Config.cpp will reflect only if you enable the corresponding UART channel here
 * 4. Check UART.h for board pinning and port_cfg.cpp for MCU pinning
 */
/* UARTUSB */
#define ASCLIN0 	UARTUSB				//used for RTE Timing

/* UART1 or UART2 or UART3 */
//#define ASCLIN1 	UART2				//used for the REMOTE / ZIGBEE connection
#define ASCLIN1 	UART2				//new configuration for LIDAR ANTI Collission


/* UART4 or UART5*/
//#define ASCLIN2 	UART4				//used for the LIDAR collission / heartbeat signal
#define ASCLIN2 	UART4				//new configuration for REMOTE / ZIGBEE 1

/* UART 6 */
#define ASSCLIN3	UART6				//new configuration for MAP / ZIGBEE 2

/* UART6 */
#define ASCLIN3 	DISABLE

/************************
 * STM Priority for OS
 ***********************/

#define SYSTIME_ISR_PRIO 		33

/*********************************************************************************
 * Error checks
 *********************************************************************************/

/************************
 * UART checks
 ***********************/

#if ((ASCLIN0 != UARTUSB) && (ASCLIN0 != DISABLE))
#error "ASCLIN0 configuration error"
#endif

#if ((ASCLIN1 != UART1) && (ASCLIN1 != UART2) && (ASCLIN1 != UART3) && (ASCLIN1 != DISABLE))
#error "ASCLIN1 configuration error"
#endif

#if ((ASCLIN2 != UART4) && (ASCLIN2 != UART5) && (ASCLIN2 != DISABLE))
#error "ASCLIN2 configuration error"
#endif

#if ((ASCLIN3 != UART6) && (ASCLIN3 != DISABLE))
#error "ASCLIN3 configuration error"
#endif


#endif /* SYSTEM_CONFIGURATION_H_ */
