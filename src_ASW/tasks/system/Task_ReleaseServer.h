#ifndef _TASK_RELEASESERVER_H_
#define _TASK_RELEASESERVER_H_

#include <pxdef.h>

#ifdef __cplusplus
  extern "C" {
#endif /* __cplusplus */

/*
Configuration of the task

Uncomment and set a value to use these macros in the task creation routine.
It would then look like

Task_ReleaseServer_Id = Task_ReleaseServerCreate(TASK_RELEASESERVER_PRIO, TASK_RELEASESERVER_ACTIVATION_EVENTS);
*/
#define TASK_RELEASESERVER_STACKSIZE 400
#define TASK_RELEASESERVER_INTR_STACKSIZE    32

#define TASK_RELEASESERVER_PRIO 14
#define TASK_RELEASESERVER_ACTIVATION_EVENTS 0


// defines for the nameserver
// it is recommended to move the next two lines to a common headerfile, which contains all defines for all nameserverids
#define __Task_ReleaseServer_NAMESERVERID   { 2, 2, 3, 2 }
#define Task_ReleaseServer_NAMESERVERID             ((PxNameId_t){  __Task_ReleaseServer_NAMESERVERID })


extern PxTask_t Task_ReleaseServerCreate(PxUInt_t core, PxUChar_t prio, PxEvents_t actevents);

extern void anotherTest();


#ifdef __cplusplus
  }
#endif /* __cplusplus */

#endif // _TASK_RELEASESERVER_H_
