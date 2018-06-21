/**
 * Project:	Student Car v3
 *
 * \file	trap_handler_runtime.h
 * \author	Thomas Barth	thomas.barth@h-da.de
 * \version	0.5 	12.08.2016
 *
 *  @brief Trap-Handler prototypes for OS-runtime traps
 *
 *  A Trap is the reaction of the system to a exception. \n
 *  If a CPU encounters a problem with the user-code or a illegal access, a trap is raised and the corresponding trap handler
 *  (a function to handle the error) is called. \n \n
 *
 *  During OS runtime, PxROS will handle every trap as there are certain traps that occur during normal operation. \n
 *  If PxROS can not handle a trap, a user-trap-handler will be called. User-Trap-Handlers have to installed using #PxTrapInstallHandler.
 *  In case there is no user-trap-handler defined for a certain trap-class or the user-trap-handler can not handle a trap (signaled by returning false),
 *  #PxTrapAbort is called. \n \n
 *
 *  Changelog:\n
 *  - 0.5	12.08.2016	Barth
 *  	- Using Ifx Types for DSTR, DATR, DEADD, DIEAR, DIETR
 *  - 0.4	02.07.2016	Barth	
 *		- Overloaded #PxTrapAbort. PxTrapAbort handles traps during OS runtime, if no user-handler was specified (at that point in time) or if the user-handler could not handle the trap.
 *  	- Renamed p_csa to p_CSA
 *
 *  - 0.3	25.06.2016	Barth 	
 *		- Extended Trap-Documentation in the comments.
 *  	- Extended _last_trap_details to store DSTR, DATR, DEADD, DIEAR, DIETR
 *
 *  - 0.2	18.05.2016	Barth 	
 * 		- Added global Structure that stores the details of the last trap.*/

#ifndef TRAP_HANDLER_RUNTIME_H_
#define TRAP_HANDLER_RUNTIME_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "pxdef.h"

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
/** \brief Trap Class 0 Handler in OS context
 * Note: The content of the registers DSTR and DEAD are not longer valid upon entering the trap function
 *
 * \param trapTin	Structure for Trap-Class and Tin that occurred
 * \param arg		User-Argument that was passed to #PxTrapInstallHandler
 * \param runtaskId	Task-ID in which the trap occurred
 * \param dstr		Content of the core register DSTR
 * \param deadd		Content of the core register DEADD (trap adress)
 * \param p_CSA		Pointer to the current Context in the CSA.
 * \return			True if the trap was handled successfully, false if not (trap will be passed to #PxTrapAbort).  */
PxBool_t Px_trap_hdl_0(PxTrapTin_t trapTin, PxUInt_t arg, PxUInt_t runtaskId, PxUInt_t dstr, PxUInt_t* deadd, TC_CSA_t* p_CSA);

/** \brief Trap Class 1 Handler in OS context
 * Note: The content of the registers DSTR and DEAD are not longer valid upon entering the trap function
 *
 * \param trapTin	Structure for Trap-Class and Tin that occurred
 * \param arg		User-Argument that was passed to #PxTrapInstallHandler
 * \param runtaskId	Task-ID in which the trap occurred
 * \param dstr		Content of the core register DSTR
 * \param deadd		Content of the core register DEADD (trap adress)
 * \param p_CSA		Pointer to the current Context in the CSA.
 * \return			True if the trap was handled successfully, false if not (trap will be passed to #PxTrapAbort).  */
PxBool_t Px_trap_hdl_1(PxTrapTin_t trapTin, PxUInt_t arg, PxUInt_t runtaskId, PxUInt_t dstr, PxUInt_t* deadd, TC_CSA_t* p_CSA);

/** \brief Trap Class 2 Handler in OS context
 * Note: The content of the registers DSTR and DEAD are not longer valid upon entering the trap function
 *
 * \param trapTin	Structure for Trap-Class and Tin that occurred
 * \param arg		User-Argument that was passed to #PxTrapInstallHandler
 * \param runtaskId	Task-ID in which the trap occurred
 * \param dstr		Content of the core register DSTR
 * \param deadd		Content of the core register DEADD (trap adress)
 * \param p_CSA		Pointer to the current Context in the CSA.
 * \return			True if the trap was handled successfully, false if not (trap will be passed to #PxTrapAbort).  */
PxBool_t Px_trap_hdl_2(PxTrapTin_t trapTin, PxUInt_t arg, PxUInt_t runtaskId, PxUInt_t dstr, PxUInt_t* deadd, TC_CSA_t* p_CSA);

/** \brief Trap Class 3 Handler in OS context
 * Note: The content of the registers DSTR and DEAD are not longer valid upon entering the trap function
 *
 * \param trapTin	Structure for Trap-Class and Tin that occurred
 * \param arg		User-Argument that was passed to #PxTrapInstallHandler
 * \param runtaskId	Task-ID in which the trap occurred
 * \param dstr		Content of the core register DSTR
 * \param deadd		Content of the core register DEADD (trap adress)
 * \param p_CSA		Pointer to the current Context in the CSA.
 * \return			True if the trap was handled successfully, false if not (trap will be passed to #PxTrapAbort).  */
PxBool_t Px_trap_hdl_3(PxTrapTin_t trapTin, PxUInt_t arg, PxUInt_t runtaskId, PxUInt_t dstr, PxUInt_t* deadd, TC_CSA_t* p_CSA);

/** \brief Trap Class 4 Handler in OS context
 * Note: The content of the registers DSTR and DEAD are not longer valid upon entering the trap function
 *
 * \param trapTin	Structure for Trap-Class and Tin that occurred
 * \param arg		User-Argument that was passed to #PxTrapInstallHandler
 * \param runtaskId	Task-ID in which the trap occurred
 * \param dstr		Content of the core register DSTR
 * \param deadd		Content of the core register DEADD (trap adress)
 * \param p_CSA		Pointer to the current Context in the CSA.
 * \return			True if the trap was handled successfully, false if not (trap will be passed to #PxTrapAbort).  */
PxBool_t Px_trap_hdl_4(PxTrapTin_t trapTin, PxUInt_t arg, PxUInt_t runtaskId, PxUInt_t dstr, PxUInt_t* deadd, TC_CSA_t* p_CSA);

/** \brief Trap Class 5 Handler in OS context
 * Note: The content of the registers DSTR and DEAD are not longer valid upon entering the trap function
 *
 * \param trapTin	Structure for Trap-Class and Tin that occurred
 * \param arg		User-Argument that was passed to #PxTrapInstallHandler
 * \param runtaskId	Task-ID in which the trap occurred
 * \param dstr		Content of the core register DSTR
 * \param deadd		Content of the core register DEADD (trap adress)
 * \param p_CSA		Pointer to the current Context in the CSA.
 * \return			True if the trap was handled successfully, false if not (trap will be passed to #PxTrapAbort).  */
PxBool_t Px_trap_hdl_5(PxTrapTin_t trapTin, PxUInt_t arg, PxUInt_t runtaskId, PxUInt_t dstr, PxUInt_t* deadd, TC_CSA_t* p_CSA);

/** \brief Trap Class 6 Handler in OS context
 * Note: The content of the registers DSTR and DEAD are not longer valid upon entering the trap function
 *
 * \param trapTin	Structure for Trap-Class and Tin that occurred
 * \param arg		User-Argument that was passed to #PxTrapInstallHandler
 * \param runtaskId	Task-ID in which the trap occurred
 * \param dstr		Content of the core register DSTR
 * \param deadd		Content of the core register DEADD (trap adress)
 * \param p_CSA		Pointer to the current Context in the CSA.
 * \return			True if the trap was handled successfully, false if not (trap will be passed to #PxTrapAbort).  */
PxBool_t Px_trap_hdl_6(PxTrapTin_t trapTin, PxUInt_t arg, PxUInt_t runtaskId, PxUInt_t dstr, PxUInt_t* deadd, TC_CSA_t* p_CSA);

/** \brief Trap Class 7 Handler in OS context
 * Note: The content of the registers DSTR and DEAD are not longer valid upon entering the trap function
 *
 * \param trapTin	Structure for Trap-Class and Tin that occurred
 * \param arg		User-Argument that was passed to #PxTrapInstallHandler
 * \param runtaskId	Task-ID in which the trap occurred
 * \param dstr		Content of the core register DSTR
 * \param deadd		Content of the core register DEADD (trap adress)
 * \param p_CSA		Pointer to the current Context in the CSA.
 * \return			True if the trap was handled successfully, false if not (trap will be passed to #PxTrapAbort).  */
PxBool_t Px_trap_hdl_7(PxTrapTin_t trapTin, PxUInt_t arg, PxUInt_t runtaskId, PxUInt_t dstr, PxUInt_t* deadd, TC_CSA_t* p_CSA);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TRAP_HANDLER_RUNTIME_H_ */
