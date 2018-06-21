/*
New PXROS-HR task class created on 2013-12-18-09.04.21 using the PXROS Wizard in HighTec's Code::Blocks.

*/
#ifndef _TASK_RELEASESERVERDATA_H_
#define _TASK_RELEASESERVERDATA_H_

#include <pxdef.h>

extern "C" {
extern void Task_ReleaseServer_Entry(PxTask_t myID, PxMbx_t myMailbox, PxEvents_t myActivationEvents);
}
/*
The Class of Task_ReleaseServer
This class defines all data and functions of the task Task_ReleaseServer
*/

class Task_ReleaseServer {
public:
    PxStackAligned_t Task_ReleaseServerInterruptStack[TASK_RELEASESERVER_INTR_STACKSIZE];
    PxStackAligned_t Task_ReleaseServerStack[TASK_RELEASESERVER_STACKSIZE];

// add 6 dummy bytes to avoid protection disruption by using "st.d" at the end of the data area
// normaly an access to this pad bytes will lead to an protection fault
// this 6 byte have to be always the last elements in the data area of a task
// don't at any members behind this protection pad
    PxChar_t	__protectionPad[6];
    void Task_ReleaseServer_Func(PxTask_t myID, PxMbx_t myMailbox, PxEvents_t myActivationEvents);

friend PxTask_t Task_ReleaseServerCreate(PxUChar_t prio, PxEvents_t actevents);
friend void Task_ReleaseServer_Entry(PxTask_t myID, PxMbx_t myMailbox, PxEvents_t myActivationEvents);
} __attribute__ ((aligned(64)));


//#define CORE_COUNT	3

extern Task_ReleaseServer * const ReleaseServer[PX_NO_OF_CORES];

#endif // _TASK_RELEASESERVERDATA_H_
