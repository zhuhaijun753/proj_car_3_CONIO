/*****************************************************************************
 * Bertrandt Ing Buro & Hochschule Darmstadt
 *
 * Author: Dilip
 *
 * File: UART_config.h
 * Desc: UART driver configuration for Infineon Tricore Aurix
 ******************************************************************************/

#ifndef UART_CONFIG_H_
#define UART_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "UART.h"

EXTERN uartConfig_t const UART_Config[];	/* Array to store Configuration of channels*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* UART_CONFIG_H_ */
