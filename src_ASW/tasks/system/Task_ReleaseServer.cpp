/*
New PXROS-HR task created on 2013-12-18-09.04.21 using the PXROS Wizard in HighTec's Code::Blocks.

Project: Task_ReleaseServer 
Projectdir: C:\Dokumente und Einstellungen\Administrator\Desktop\tcv\Task_ReleaseServer\

To start this task call PxTaskCreate()
To register this task call PxNameRegister()

*/

#include <pxdef.h>
#include "Task_ReleaseServer.h"
#include "Task_ReleaseServerData.h"


extern "C" {
void Task_ReleaseServer_Entry(PxTask_t myID, PxMbx_t myMailbox, PxEvents_t myActivationEvents);
}

extern Task_ReleaseServer * const ReleaseServer[PX_NO_OF_CORES];


void Task_ReleaseServer_Entry(PxTask_t myId, PxMbx_t myMailbox, PxEvents_t myActivationEvents)
{
	PxUInt_t core = PxGetCoreId();
    ReleaseServer[core]->Task_ReleaseServer_Func(myId,myMailbox,myActivationEvents);
}

void Task_ReleaseServer::Task_ReleaseServer_Func(PxTask_t myID, PxMbx_t myMailbox, PxEvents_t myActivationEvents)
{
	//get rid of warnings

	(void) myID;
	(void) myMailbox;
	(void) myActivationEvents;

	PxServiceTaskInit();

	PxMsgrelServiceInit(myMailbox);

	PxMsg_t err;

	while(1)
	{
		PxMsgEvent_t msg_ev;

		msg_ev.msg = PxMsgIdInvalidate();

		msg_ev.events = 0;

		msg_ev = PxMsgReceive_EvWait(myMailbox, PXSERVICE_HND_MSGREL);

		if(PxMsgIdIsValid(msg_ev.msg))
		{
			err = PxMsgRelease(msg_ev.msg);

			if(err.error != PXERR_NOERROR)
				PxPanic();
		}
		if (msg_ev.events & PXSERVICE_HND_MSGREL)
			PxMsgrelService();
	}
}
