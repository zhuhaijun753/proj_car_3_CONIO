/*
 * Task_Time.c
 *
 *  Created on: Jan 6, 2015
 *      Author: fromm
 */

#include "core.h"
#include "pxdef.h"

#include "det.h"

#include <machine/intrinsics.h>
#include <machine/wdtcon.h>

#include "system.h"
#include "system_configuration.h"

#include "clock.h"

#include "register.h"


#define STM0_BASE    0xF0000000
#define STM1_BASE    0xF0000100
#define STM2_BASE    0xF0000200

#define STMx_CLC(base)     (*((volatile unsigned int*) (base+0x00)))
#define STMx_CMP0(base)    (*((volatile unsigned int*) (base+0x30)))
#define STMx_TIM0(base)    (*((volatile unsigned int*) (base+0x10)))
#define STMx_CMCON(base)   (*((volatile unsigned int*) (base+0x38)))
#define STMx_ICR(base)     (*((volatile unsigned int*) (base+0x3C)))

#define SRC_STMxSR0(id)    (*((volatile unsigned int*) (0xF0038490+(id*8))))





static unsigned int systime_GetStmBase(void)
{
        switch (_mfcr(0xfe1C))  // core ID
        {
                default:
                case 0: return STM0_BASE;
                case 1: return STM1_BASE;
                case 2: return STM2_BASE;
        }
}



/* timer interrupt service routine */
static void systime_Isr(PxArg_t systime_Reload)
{
    Ifx_STM *StmBase = systime_GetStmBase();

    StmBase->CMP[0].U += (unsigned int)systime_Reload;
    PxTickDefine_Hnd();
}

/* enable service request for STM interrupt */
static int __set_STM_SRC(unsigned int prio, unsigned int coreId)
{
    MODULE_SRC.STM.STM[coreId].SR0.B.TOS  = coreId;
    MODULE_SRC.STM.STM[coreId].SR0.B.SRPN = prio;
    MODULE_SRC.STM.STM[coreId].SR0.B.SRE  = 1;
    return 0;
}

void SYSTICK_TicksInit(unsigned int hz)
{
    unsigned int systime_Reload;
    Ifx_STM *StmBase = systime_GetStmBase();
    unsigned int frequency = Clock_get_Frequency(fSTM);
    unsigned int coreId = _mfcr(CPU_CORE_ID) & IFX_CPU_CORE_ID_CORE_ID_MSK;

    systime_Reload = (frequency / hz);

    PxIntInstallFastHandler(SYSTIME_ISR_PRIO, systime_Isr, (PxArg_t)systime_Reload);
    PxTickSetTicksPerSecond(hz);

    /* activate Service Request Control (SRC) */
    _PxHndcall(__set_STM_SRC, PxGetId(), 2 * sizeof(unsigned int), SYSTIME_ISR_PRIO, coreId);

    /* reset interrupt flag */
    StmBase->ISCR.U = (IFX_STM_ISCR_CMP0IRR_MSK << IFX_STM_ISCR_CMP0IRR_OFF);
    /* prepare compare register */
    StmBase->CMP[0].U = StmBase->TIM0.U + systime_Reload;
    StmBase->CMCON.U  = 31;
    StmBase->ICR.B.CMP0EN = 1;
#if defined(PXVIEW) || (defined(PXMON) && defined(PXMonitorsTaskTimings))
    /* remember this value for subtick calculation */
    reload_value = systime_Reload;
#endif /* PXVIEW || (PXMON && PXMonitorsTaskTimings) */
}
