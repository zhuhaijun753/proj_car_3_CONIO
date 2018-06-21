#ifndef __PXVIEW_H__
#define __PXVIEW_H__
/*************************************************************************
 *
 *  Project:
 *	PxView
 *  Function:
 *	Header for PxView
 '
 *************************************************************************
 *
 *  Copyright 2006 by HighTec EDV Systeme GmbH
 *
 *************************************************************************/


/*************************************************************************
 *
 *  Defines, macros, etc.
 *
 ************************************************************************/

#ifdef __C166__
#define HUGE	huge
#else /* __C166__ */
#define HUGE
#endif /* __C166__ */

/*
 *	Pxview Types
 */

#define PXV_INTS		((unsigned long) 1 << 0x0)
#define PXV_EVENT		((unsigned long) 1 << 0x1)
#define PXV_MSG			((unsigned long) 1 << 0x2)
#define PXV_BLOCK		((unsigned long) 1 << 0x3)
#define PXV_DELAY		((unsigned long) 1 << 0x4)
#define PXV_PE			((unsigned long) 1 << 0x5)
#define PXV_TO			((unsigned long) 1 << 0x6)
#define PXV_OBJ			((unsigned long) 1 << 0x7)
#define PXV_MC			((unsigned long) 1 << 0x8)
#define PXV_DATA		((unsigned long) 1 << 0x9)
#define PXV_TASK_SWITCH	((unsigned long) 1 << 0xa)
#define PXV_USER		((unsigned long) 1 << 0x10)

#define PXV_ALL	(PXV_INTS | PXV_EVENT | PXV_MSG | PXV_BLOCK | PXV_DELAY | PXV_PE | PXV_TO | \
				 PXV_OBJ | PXV_MC | PXV_DATA | PXV_TASK_SWITCH | PXV_USER)

/*
 *	Pxview PXROS Trace Types
 */
#define PXVT_INTS		((unsigned long) 1 << 0x0)
#define PXVT_EVENT		((unsigned long) 1 << 0x1)
#define PXVT_MSG		((unsigned long) 1 << 0x2)
#define PXVT_DELAY		((unsigned long) 1 << 0x3)
#define PXVT_PE			((unsigned long) 1 << 0x4)
#define PXVT_TO			((unsigned long) 1 << 0x5)
#define PXVT_OBJ		((unsigned long) 1 << 0x6)
#define PXVT_MC			((unsigned long) 1 << 0x7)
#define PXVT_REM		((unsigned long) 1 << 0x8)
#define PXVT_MODE		((unsigned long) 1 << 0x9)
#define PXVT_DIE		((unsigned long) 1 << 0xa)
#define PXVT_APPINFO	((unsigned long) 1 << 0xb)
#define PXVT_TASK		((unsigned long) 1 << 0xc)
#define PXVT_MBX		((unsigned long) 1 << 0xd)
#define PXVT_OPOOL		((unsigned long) 1 << 0xe)
#define PXVT_SEMA		((unsigned long) 1 << 0xf)
#define PXVT_TIMESLICE	((unsigned long) 1 << 0x10)
#define PXVT_PREF		((unsigned long) 1 << 0x11)
#define PXVT_CHANNEL	((unsigned long) 1 << 0x12)
#define PXVT_ERROR		((unsigned long) 1 << 0x13)
#define PXVT_GENID		((unsigned long) 1 << 0x14)
#define PXVT_GSREF		((unsigned long) 1 << 0x15)
#define PXVT_NAME		((unsigned long) 1 << 0x16)
#define PXVT_HND		((unsigned long) 1 << 0x17)
#define PXVT_PRIV		((unsigned long) 1 << 0x18)
#define PXVT_TRACE		((unsigned long) 1 << 0x19)
#define PXVT_TICKS		((unsigned long) 1 << 0x1A)

#define PXVT_ALL (PXVT_INTS | PXVT_EVENT | PXVT_MSG | PXVT_DELAY | PXVT_PE | PXVT_TO | PXVT_OBJ | PXVT_MC |	\
				  PXVT_REM | PXVT_MODE | PXVT_DIE | PXVT_APPINFO | PXVT_TASK | PXVT_MBX | PXVT_OPOOL |		\
				  PXVT_SEMA | PXVT_TIMESLICE | PXVT_PREF | PXVT_CHANNEL | PXVT_ERROR | PXVT_GENID |			\
				  PXVT_GSREF | PXVT_NAME | PXVT_HND | PXVT_PRIV | PXVT_TRACE | PXVT_TICKS)

/*
 *	Size of one element in pxview ringbuffer
 */
#define PXV_ENTRY_SIZE		24
/*
 *	calculate size of pxview ringbuffer
 */
#define	PXV_ARRAY_SIZE(x)	((x) * PXV_ENTRY_SIZE)


#define TTY0	0
#define TTY1	1
#define TTY2	2
#define TTY3	3

#define TRACE_START	1
#define TRACE_STOP	0

/*************************************************************************
 *
 *  Global includes
 *
 ************************************************************************/

#include <pxdef.h>

/*************************************************************************
 *
 *  Local includes
 *
 ************************************************************************/


/*************************************************************************
 *
 *  External variables and functions
 *
 ************************************************************************/

#ifdef USE_SCHEDULING_EXTENSION
extern PxGlobalSchedExt_T PxvGlobalSchedExt;
#endif /* USE_SCHEDULING_EXTENSION */

/*************************************************************************
 *
 *  Type definitions
 *
 ************************************************************************/

typedef enum
{
	PXVObj_Obj			= 0x0001,
	PXVObj_Msg			= 0x0002,
	PXVObj_Delay		= 0x0003,
	PXVObj_Mc			= 0x0004,
	PXVObj_Mbx			= 0x0005,
	PXVObj_Opool		= 0x0006,
	PXVObj_SndChan		= 0x0007,
	PXVObj_RecChan		= 0x0008,
	PXVObj_RelayChan	= 0x0009,
	PXVObj_Sem			= 0x000a,
#ifdef TARGET_HAS_ONLY_SHORT_ENUMS
	PXVObj_LastObjType	= 0xFFFF
#else
	PXVObj_LastObjType	= 0x7FFFFFFF
#endif
} PxvObjType_t;

typedef struct
{
	unsigned long	Id;
	char			*Name;
	char			*Folder;
	char			*Tip;
} PxvNameDefinition_t;


/*************************************************************************
 *
 *  Function prototypes
 *
 ************************************************************************/

#ifdef __cplusplus
  extern "C" {
#endif /* __cplusplus */

PxError_t	PxvInit(PxUInt_t (*get_subticks)(void), PxUInt_t subticks_per_tick, PxUInt_t no_of_elems, PxAligned_t *startof_ringbuffer);

void 		PxvStart(void);
PxBool_t	PxvStop(void);

void	PxvEnable(PxULong_t PxvMask);
void	PxvDisable(PxULong_t PxvMask);

void 	PxvSetPxrosTrace(char Init);

void	PxvTraceEnable(PxULong_t PxvMask);
void	PxvTraceDisable(PxULong_t PxvMask);

void	PxvAddUserEntry(PxTask_t From, unsigned long Data1, unsigned long Data2);
void	PxvAddUserEvent(PxTask_t From, unsigned long Type, unsigned long Data1, unsigned long Data2);
void	PxvSetTicksperSecond(unsigned long ticks);

void	PxvIntEntry(int intno);
void	PxvIntExit(int intno);

void	PxvMsgSend(PxTask_t From, PxMsg_t Msg, PxMbx_t Mbx);
void	PxvMsgReceived(PxTask_t From, PxMsg_t Msg, unsigned long Data);
void	PxvMsgReceive(PxTask_t From, PxMbx_t Mbx, unsigned long Data);
void	PxvMsgEnvelop(PxTask_t From, PxMsg_t Msg, unsigned long Data);

void	PxvDataSend(PxTask_t From, unsigned long Data1, unsigned long Data2);
void	PxvDataReceived(PxTask_t From, unsigned long Data1, unsigned long Data2);
void 	PxvDataWait(PxTask_t From, unsigned long Data1, unsigned long Data2);

void	PxvConnected(PxTask_t From, unsigned long Data1, unsigned long Data2);
void	PxvDisconnected(PxTask_t From, unsigned long Data1, unsigned long Data2);

void	PxvSignalEvents(PxTask_t From, PxTask_t To, PxEvents_t ev);
void	PxvAwaitEvents(PxTask_t From, PxEvents_t evMask);
void	PxvReceivedEvents(PxTask_t From, PxEvents_t evMask);

void	PxvMcTakeBlk_Size(PxTask_t From, PxMc_t Mc, PxSize_t Size);
void	PxvMcTakeBlk_Block(PxTask_t From, PxMc_t Mc, PxAligned_t Block);
void	PxvMcReturnBlk(PxTask_t From, PxMc_t Mc, PxAligned_t Block);

void	PxvDelayStart(PxTask_t From, PxDelay_t Delay, PxTicks_t Ticks);
void	PxvDelayStop(PxTask_t From, PxDelay_t Delay);
void	PxvDelayExpired(PxTask_t From, PxDelay_t Delay);

void	PxvPeStart(PxTask_t From, PxPe_t Pe);
void	PxvPeStop(PxTask_t From, PxPe_t Pe);
void	PxvPeExpired(PxTask_t From, PxPe_t Pe);

void	PxvToStart(PxTask_t From, PxTo_t To);
void	PxvToStop(PxTask_t From, PxTo_t To);
void	PxvToExpired(PxTask_t From, PxTo_t To);

void	PxvObjRequest(PxTask_t From, PxvObjType_t ObjType, PxOpool_t Opool);
void	PxvObjRelease(PxTask_t From, PxvObjType_t ObjType, unsigned long Data);

PxError_t	PxvSetTaskSuspendable(PxTask_t Task);
PxError_t	PxvUnSetTaskSuspendable(PxTask_t Task);
PxBool_t	PxvTaskSuspendable(PxTask_t Task);

void	PxvSetNameDefinition(PxvNameDefinition_t *NameDefinition);

PxTask_t PxviewOpen_tcp(int prio, short port, int stacksize);
PxTask_t PxviewOpen_uart(int prio, char port, long baudrate, int stacksize);

#ifdef __cplusplus
  }
#endif /* __cplusplus */

#endif /* __PXVIEW_H__ */
