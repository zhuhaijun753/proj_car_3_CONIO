/*
 * TaskServices.cpp
 *
 *  Created on: 06.05.2016
 *      Author: Fromm
 */

#include "Task_Services.h"


extern "C"
{

PxError_t WaitForService (PxUInt_t core, PxMbxReq_t req, PxInt_t tmo)
{
  PxTo_t to;
  PxError_t err = PXERR_NOERROR;

  to = PxToRequest (PXOpoolTaskdefault, 1, 1);
  if (!PxToIdIsValid (to))
    return PxToIdError (to);

  while (tmo >= 0)
    {
      tmo--;

      err = PxGetGlobalServerMbx (core, req);
      if (err == PXERR_NOERROR)
        return PXERR_NOERROR;
      if (tmo >= 0)
        {
          PxToStart (to);
          PxAwaitEvents (1);
        }
    }
  PxToRelease (to);
  return err;
}

} //extern C

