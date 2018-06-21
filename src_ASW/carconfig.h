/**
 * \file 	carconfig.h
 * \author 	Prof.Dr.-Ing. Peter Fromm - Hochschule Darmstadt peter.fromm@h-da.de
 * \date 	19.10.2017
 *
 * \brief DEFAULT HEADER TEMPLATE
 *
 * <detailed description what the file (header and/or source it belongs to) does>
 *
 * \note <notes>
 * \todo <todos>
 * \warning <warnings, e.g. dependencies, order of execution etc.>
 *
 *  Changelog:\n
 *  - <version; data of change; author>
 *            - <description of the change>
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
#ifndef CARCONFIG_H_
#define CARCONFIG_H_

//#define CAR_3
//#define CAR_4
#define CAR_SIM

//========================================================================== The symbols CAR_3 and CAR_4 are defined on project level and selected through the build environment
//========================================================================== Based on these symbols, the car configuration is defined

#ifdef CAR_3
	#define CONFIG_REMOTE_CAR_ID			0x03		/**< \brief Car Id used for the remote communication protocol */
	#define CONFIG_SMARTPOWER 							/**< \brief Consider Smartpower Board for booting and  emergency halt escalation */
	#undef  CONFIG_DISCONNECT_CAN 						/**< \brief Disconnect signals from CanOpen communication */
#endif

#ifdef CAR_4
	#define CONFIG_REMOTE_CAR_ID			0x04		/**< \brief Car Id used for the remote communication protocol */
	#undef  CONFIG_SMARTPOWER							/**< \brief Do not consider Smartpower Board for booting and  emergency halt escalation */
	#undef  CONFIG_DISCONNECT_CAN 						/**< \brief Disconnect signals from CanOpen communication */
#endif

#ifdef CAR_SIM
	#define CONFIG_REMOTE_CAR_ID			0x00		/**< \brief Car Id used for the remote communication protocol */
	#undef  CONFIG_SMARTPOWER							/**< \brief Do not consider Smartpower Board for booting and  emergency halt escalation */
	#define CONFIG_DISCONNECT_CAN 						/**< \brief Disconnect signals from CanOpen communication */
#endif

#endif /* CARCONFIG_H_ */
