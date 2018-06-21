/*
 * swc_dead_reackoning.cpp
 *
 *  Created on: 04.06.2016
 *      Author: Fromm
 */

#include "global.h"

#include "math.h"
#include "string.h"
#include "stdio.h"

#include "swc_dead_reackoning.h"
#include "runnable_table_type.h"

#include "carstate_type.h"
#include "carstate_signal.h"
#include "statemachine_config.h"

#include "cardataprotocol_type.h"
#include "remotemsg_signal.h"
#include "remotemsg_type.h"

#include "enginedecoder_type.h"
#include "enginedecoder_signal.h"

#include "carposition_type.h"
#include "carposition_signal.h"

#include "signal_bb.h"


#define DEADRECKON_KX     0.250       	//Experimental value
#define DEADRECKON_KY     0.255       	//Experimental value
#define DEADRECKON_KTHETA 0.480 		//Experimental value

/*****************************************************************************************
 * Init Function Implementations
 *****************************************************************************************/

void DR_init()
{

}

void DR_deinit()
{

}

/*****************************************************************************************
 * Safety Function Implementations
 *****************************************************************************************/

//This variant calls the CANopen driver through a message
//The runnable will only send the CanOpen Request
//The update of the signal raw data will happen through the messsage handler
RT_safety_t DR_run_getDecoder()
{

	RC_t resultEngineDecoder = RC_ERROR;

	ENGINEDECODER_data_t engineDecoderValue = ENGINEDECODER_INIT_DATA;

	//read the engine decoder value (i.e) displacement of 4 wheels in 'cm'
	resultEngineDecoder = ENGINEDECODER_refresh(&so_in_enginedecoder);

	if (resultEngineDecoder == RC_SUCCESS)
	{
		return SAFEOK;
	}
	else
	{
		return SAFEERROR1;
	}
}



/*****************************************************************************************
 * Non-Safety Function Implementations
 *****************************************************************************************/

void DR_run_calculatePosition()
{

	RC_t resultCarPosition = RC_SUCCESS;

	ENGINEDECODER_data_t engineDecoderValue = ENGINEDECODER_INIT_DATA;
	CARPOSITION_data_t carPosition = CARPOSITION_get(&so_carposition);

	CARSTATE_data_t carstate = CARSTATE_get(&so_carstate);

	if (carstate.state_normal == IS_RUNNING)
	{
		//First value of decode contains rubbish data
		//Therefore, position is only calculated in running mode, decode signal should be stable until then

		float64_t dx;		//delta X distance of car (cm)
		float64_t dy;		//delta Y distance of the car (cm)
		float64_t dphi;		//delta angular distance of the car (degree)

		float64_t w_FL;		//motion of wheel number 1
		float64_t w_FR;		//motion of wheel number 2
		float64_t w_RR;		//motion of wheel number 3
		float64_t w_RL;		//motion of wheel number 4

		engineDecoderValue = ENGINEDECODER_get(&so_in_enginedecoder);

		if(ENGINEDECODER_get_status(&so_in_enginedecoder) == SIG_STS_VALID)	//reading of decoder value for all 4 wheels is success
		{
			/*
			 * convert the displacement values of each wheel into linear velocity 'v' (cm/s)
			 * as [Es]*r = linear velocity 'v', where [Es] has unit rad/s and 'r' has unit 'cm'
			 */
			w_FL = (engineDecoderValue.delta_displacementFL);
			w_FR = (engineDecoderValue.delta_displacementFR);
			w_RR = (engineDecoderValue.delta_displacementRR);
			w_RL = (engineDecoderValue.delta_displacementRL);


			/*
			 * PeFr': Updated matrix due to changed engine kinematics


			   >>  m = [-1 -1 1 1; 1 -1 1 -1; 1 1 1 1]

			m =

				-1    -1     1     1
				 1    -1     1    -1
				 1     1     1     1

			Pseudo Inverse matrix lets us calculate the position from the engine movement
			>> b=pinv(m)

			b =

			   -0.2500    0.2500    0.2500
			   -0.2500   -0.2500    0.2500
				0.2500    0.2500    0.2500
				0.2500   -0.2500    0.2500

			 */
			// Factor of 0.25 is included in the experimental scaling factors
			// Scaling is based in the forward matrix on 0..100% values, not physical values

			dx =    (-w_FR - w_FL + w_RR + w_RL) * DEADRECKON_KX;
			dy =   (w_FR - w_FL + w_RR - w_RL) * DEADRECKON_KY;
			dphi =  (w_FR + w_FL + w_RR + w_RL) * DEADRECKON_KTHETA;

			//calculate the current car position
			carPosition.theta += dphi;

			//Limit to +/- 360
			while (carPosition.theta > 360)
			{
				carPosition.theta -= 360;
			}
			while (carPosition.theta < -360)
			{
				carPosition.theta += 360;
			}

			double thetaInRad = carPosition.theta * M_PI / 180; //Helper Value

			carPosition.yPos += (dx * sin(thetaInRad) + dy * cos(thetaInRad));
			carPosition.xPos += (dx * cos(thetaInRad) - dy * sin(thetaInRad));

			//Set the current car position in the signal 'so_carposition'
			resultCarPosition = CARPOSITION_set(&so_carposition,carPosition);

			if(resultCarPosition != RC_SUCCESS)	//failed setting the car position
			{
				//error handling
			}
		}
		else	//failed the reading decoder values
		{
			//error handling
		}
	}
}


void DR_run_reportPosition()
{
	CARPOSITION_data_t carPosition = CARPOSITION_get(&so_carposition);
	CARDATAPROTOCOL_data_t remote = REMOTE_INIT_DATA;
	CARSTATE_data_t carstate = CARSTATE_get(&so_carstate);

	if (carstate.state_normal != IS_RUNNING)
	{
		return;
	}

	char message[REMOTE_MAX_DATASIZE] = {0};


	//Translate position into string
	snprintf(message,REMOTE_MAX_DATASIZE,"%4.2f %4.2f %4.2f",carPosition.xPos, carPosition.yPos, carPosition.theta);


	uint8_t slen = strlen(message);

	//Limit length  to maxsize of protocol
	slen = _minf(slen, REMOTE_MAX_DATASIZE);

	//Send position to UART channel
	remote.msgID = REMOTE_MID_CONNECT;
	remote.featureID = REMOTE_FID_INFO;
	remote.length = slen + REMOTE_ACK_LENGTH;

	for (int i = 0; i < slen; i++)
	{
		remote.featureData[i] = message[i];
	}

	REMOTEMSG_write(&so_out_remoteMsgFromControl, remote);

}


