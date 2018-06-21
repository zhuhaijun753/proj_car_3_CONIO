/*
 * signal_global.h
 *
 *  Created on: 12.06.2015
 *      Author: Fromm
 */

#ifndef SIGNAL_GLOBAL_H_
#define SIGNAL_GLOBAL_H_


/*================================================[ public datatypes ]================================================*/

/**
 * Possible values of the current signal status
 */
/* Possible values of the current signal status                                                                                           */
enum sig_sts_e
{
   SIG_STS_VALID       = (sint16_t)-32768,   /* signal buffer contains valid values that can be used                                           */
   SIG_STS_INITIALIZED = (sint16_t)-21846,   /* signal has not been updated since initialization                                               */
   SIG_STS_STARTUP     = (sint16_t)     0,   /* signal has not been initialized nor updated since the ecu started                              */
   SIG_STS_INVALID     = (sint16_t) 21845    /* signal values are not valid and must not be used neither by driver nor application             */
};

typedef enum sig_sts_e sig_sts_t;



/*================================================[ public defines   ]================================================*/

/**
 *  Definition of refresh and flush index type
 */
#define SIG_IDX_TYPE            uint32_t

/**
 *  This define should be used as a driver index if there is no driver function such as flush or refresh configured.
 */
#define SIG_IDX_NOT_CONFIGURED  0xff


#endif /* SIGNAL_GLOBAL_H_ */
