/*
 * TaskServices.h
 *
 *  Created on: 06.05.2016
 *      Author: Fromm
 */

#ifndef TASKSERVICES_H_
#define TASKSERVICES_H_

#include "pxdef.h"

#ifdef __cplusplus

extern "C"
{

#endif


/** Checks if a specific service already is existing
 * @param core: core on which the service is running
 * @param req: mailbox of the service
 * @param tmo: number of retries
 */
PxError_t WaitForService (PxUInt_t core, PxMbxReq_t req, PxInt_t tmo);


#ifdef __cplusplus

}  //extern C
#endif


#endif /* TASKSERVICES_H_ */
