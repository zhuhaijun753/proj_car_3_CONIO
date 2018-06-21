/*****************************************************************************
 * Bertrandt Ing Buro & Hochschule Darmstadt
 *
 * Author: Dilip
 *
 * File: DTS.h
 * Desc: Die temperature driver
 ******************************************************************************/

#ifndef DTS_H
#define DTS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "global.h"
#include "register.h"

//#include "SRC.h"

#define ISR_DTS_PRIO 63

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** Initialise the DTS with supplied configuration.
 * @return: None
 */
EXTERN RC_t DTS_initModule();

/** Start Die temperature conversion
 * @return: none
 */
EXTERN void DTS_startSensor(void);

/** Returns the current READY status of the Sensor.
 * @return: TRUE if Sensor is ready for measuring temperature otherwise FALSE
 */
EXTERN boolean_t DTS_isReady(void);

/** ISR for DTS
 */
EXTERN void DTS_isr(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DTS_H */
