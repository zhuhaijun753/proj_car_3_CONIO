/**
 * \file 	swc_gfx_diagnostic.cpp
 * \author 	Rahul
 * \date 	24-Jun-2017
 *
 * \brief DEFAULT SOURCE FILE TEMPLATE
 *
 * \copyright Copyright ©2016
 * Department of electrical engineering and information technology, Hochschule Darmstadt - University of applied sciences (h_da). All Rights Reserved.
 * Permission to use, copy, modify, and distribute this software and its documentation for educational, and research purposes in the context of non-commercial
 * (unless permitted by h_da) and official h_da projects, is hereby granted for enrolled students of h_da, provided that the above copyright notice,
 * this paragraph and the following paragraph appear in all copies, modifications, and distributions.
 * Contact Prof.Dr.-Ing. Peter Fromm, peter.fromm@h-da.de, Birkenweg 8 64295 Darmstadt - GERMANY for commercial requests.
 *
 * \warning This software is a PROTOTYPE version and is not designed or intended for use in production, especially not for safety-critical applications!
 * The user represents and warrants that it will NOT use or redistribute the Software for such purposes.
 * This prototype is for research purposes only. This software is provided "AS IS," without a warranty of any kind.
 */

#include <swc_gfx_diagnostic.h>

#include "swc_view_controller.h"
#include "swc_touch_controller.h"

#include "Task_C2_TFT.h"
#include "SystemEvents.h"

#include "signal_bb.h"

/******************************************************************/
#pragma section ".data.ui"

ui::CViewController viewCtrl;
ui::CTouchController touchCtrl;

#pragma section
/******************************************************************/

void GFXDIAGNOSTIC_display_init()
{
	viewCtrl.init();
}

void GFXDIAGNOSTIC_touch_init()
{
	touchCtrl.init();
}

void GFXDIAGNOSTIC_display_run()
{
	viewCtrl.run();
}

void GFXDIAGNOSTIC_touch_run()
{
	EVENT_data_t event = EVENT_INIT_DATA;

	if (RC_SUCCESS == touchCtrl.run())
	{
		event.receiverTask = Task_C2_TFT_getTaskId();
		event.eventMask = ev_Task_C2_TFT_0_Touch;
		asm("nop");
		EVENT_write(&so_touchEvent, event);
	}

}

void GFXDIAGNOSTIC_display_onTouch()
{
	TOUCH_data_t touchData;
	ui::CTouchNotification notification;

	touchData = TOUCH_get(&so_in_touch);

	notification.set(touchData);
	viewCtrl.notify(notification);
}
