/*
New PXROS-HR taskconfiguration created on 2007-06-05-08.16.15 using the PXROS Wizard in HighTec's Code::Blocks.

*/

#include "pxdef.h"
#include "Task_ReleaseServer.h"
#include "Task_ReleaseServerData.h"



#pragma section ".Task_C0_ReleaseServer.data" //awBc0
Task_ReleaseServer Task0_ReleaseServerObj;
#pragma section
#pragma section ".Task_C1_ReleaseServer.data" //awBc1
Task_ReleaseServer Task1_ReleaseServerObj;
#pragma section
#pragma section ".Task_C2_ReleaseServer.data" //awBc2
Task_ReleaseServer Task2_ReleaseServerObj;
#pragma section

extern Task_ReleaseServer * const ReleaseServer[PX_NO_OF_CORES] =
{
		&Task0_ReleaseServerObj,
		&Task1_ReleaseServerObj,
		&Task2_ReleaseServerObj,
};

PxTask_t Task_ReleaseServerCreate(PxUInt_t core, PxUChar_t prio,PxEvents_t events)
{
	PxTaskContext_T task_Context;
	PxTaskSpec_T task_Spec;

	if (core >= PX_NO_OF_CORES)
	{
		return PxTaskIdInvalidate();
	}
	PxBzero((PxUChar_t *)&task_Spec,sizeof(task_Spec));

	task_Context.protection[0].lowerBound = 0;
	task_Context.protection[0].upperBound = 0;
	task_Context.protection[0].prot = NoAccessProtection,
	task_Context.protection[1].lowerBound = (PxUInt_t)ReleaseServer[core];
	task_Context.protection[1].upperBound = ((PxUInt_t)ReleaseServer[core]) + ((PxUInt_t)(&(ReleaseServer[core]->__protectionPad[0])) - (PxUInt_t)(ReleaseServer[core]));
	task_Context.protection[1].prot = WRProtection;

	task_Spec.ts_name			= (const PxChar_t*)"Task_ReleaseServer";
	task_Spec.ts_fun			= Task_ReleaseServer_Entry;
	task_Spec.ts_mc				= PXMcTaskdefault;
	task_Spec.ts_opool			= PXOpoolTaskdefault;
	task_Spec.ts_privileges			= PXUser0Privilege;
    task_Spec.ts_accessrights	= 0|PXACCESS_HANDLERS|PXACCESS_INSTALL_HANDLERS|PXACCESS_INSTALL_SERVICES|PXACCESS_REGISTERS|PXACCESS_SYSTEMDEFAULT|PXACCESS_RESOURCES|PXACCESS_NEW_RESOURCES|PXACCESS_SYSTEM_CONTROL;
	task_Spec.ts_context			= &task_Context;
	task_Spec.ts_taskstack.stk_type		= PXStackFall;
	task_Spec.ts_taskstack.stk_size 	= PXStackDontCheck;
	task_Spec.ts_taskstack.stk_src.stk 	= &(ReleaseServer[core]->Task_ReleaseServerStack[TASK_RELEASESERVER_STACKSIZE]);
	task_Spec.ts_inttaskstack.stk_type 	= PXStackFall;
	task_Spec.ts_inttaskstack.stk_size 	= PXStackDontCheck;
	task_Spec.ts_inttaskstack.stk_src.stk 	= &(ReleaseServer[core]->Task_ReleaseServerInterruptStack[TASK_RELEASESERVER_INTR_STACKSIZE]);
	task_Spec.ts_abortstacksize		= 0;

    return PxTaskCreate(PXOpoolTaskdefault,&task_Spec,prio,events);
}
