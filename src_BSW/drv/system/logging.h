/**
 * \file 	logging.h
 * \author 	Thomas Barth	- Hochschule Darmstadt thomas.barth@h-da.de
 * \date 	21.09.2016
 * \version	0.1
 *
 * \brief Logging mechanism
 *
 * The user can use the macro #LOGGING_SEND to send logging messages to a queue. This queue can be used to print the messages e.g. to UART
 *
 *
 *  Changelog:\n
 *  - 0.1   21.09.2016  Barth
 *            - Initial documentation
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
#ifndef SRC_BSW_DRV_SYSTEM_LOGGING_H_
#define SRC_BSW_DRV_SYSTEM_LOGGING_H_

#include <stdio.h>
#include "global.h"


//#define LOGGING_USE_N_T     0               /**< \brief Do not add date or time to logging messages*/
//#define LOGGING_USE_T_T     1               /**< \brief Add time to logging messages*/
//#define LOGGING_USE_DT_T    2               /**< \brief Add date and time to logging messages*/

#define	CFG_LOGGING_LEVEL	LOGGING_LEVEL_INFO

/**
 * \brief Debugging Level.
 *
 * The debugging level defines the character of an event and is used to define which events are send to the log using #LOGGING_LEVEL
 */
typedef enum{
    LOGGING_LEVEL_DEBUG,                    /**< \brief Low Level Information */
    LOGGING_LEVEL_INFO,                     /**< \brief High level information during normal operation */
    LOGGING_LEVEL_WARNING,                  /**< \brief abnormal operation, which might be fixed */
    LOGGING_LEVEL_ERROR,                    /**< \brief abnormal operation, which can not be fixed */
    LOGGING_LEVEL_OFF                       /**< \brief Event logging is switched off */
}logging_level_t;


 /**
 * \brief Macro for sending messages to the logging system
 * \param x     Logging level, see #logging_level_t
 * \param y     Sender identification string
 * \param ...   The message to be printed, can be used in #sprintf fassion.
 * \warning     There is no check if the message was send to the queue. If the user wants to be sure that the message is send,
 *              then the macro below can be modified to retry the queue send operation or another mechanism has to be used.
 */
//TODO
#define LOGGING_SEND(x,y,...)                       \
{if(x>=CFG_LOGGING_LEVEL){}}                        \

/*
logging_t msg;                                      \
msg.level=x;                                        \
sprintf (msg.sender, "%s",y);                       \
sprintf (msg.message, __VA_ARGS__);                 \
vLOGG_send(&msg);                                   \
}}                                                  \
*/

///**
// * \brief Logging structure
// *
// * Each entry in the log has the following structure
// */
//typedef struct{
//#if(CFG_LOGGING_DT_T==LOGGING_USE_N_T)
//    //no timestamp for log
//#elif(CFG_LOGGING_DT_T==LOGGING_USE_T_T)
//    vRTC_time_arr_t     timestamp;                          /**< \brief Timestamp with time only*/
//#elif(CFG_LOGGING_DT_T==LOGGING_USE_DT_T)
//    vRTC_datetime_arr_t timestamp[vRTC_TIME_DT_L];          /**< \brief Timestamp with date and time */
//#endif
//    logging_level_t     level;                              /**< \brief Logging level, see #logging_level_t */
//    char                sender [CFG_LOGGING_SENDER_L+1];    /**< \brief sender of the message */
//    char                message[CFG_LOGGING_L_M+1];         /**< \brief message content */
//}logging_t;



/**
 * \brief   Send a new logging message to the logging-queue. It is recommended to use #LOGGING_SEND instead of this function directly.
 * \param   entry   Logging message, see #logging_t.
 * \return  #RC_ERROR_INVALID_STATE if the logging service is not available.
 *          #RC_SUCCESS if the message was added to the queue.
 *          #RC_ERROR_BUFFER_FULL if the queue is full and the message was not added.
 */
//RC_t vLOGG_send(logging_t* entry);


#endif /* SRC_BSW_DRV_SYSTEM_LOGGING_H_ */
