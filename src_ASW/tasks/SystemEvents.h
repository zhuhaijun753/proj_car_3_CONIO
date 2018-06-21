/*************************************
 * System event definitions		 *
 *************************************/

#ifndef SYSTEMEVENTS_H_
#define SYSTEMEVENTS_H_


//Events of task: Task_C0_Safety_Input
//10ms tick event
#define ev_Task_C0_Safety_Input_0_1ms  (1<<0)

//Activation events of task: Task_C0_Safety_Input
//Activation event of the task
#define ev_Task_C0_Safety_Input_0_Activate  (1<<0)

//Events of task: Task_C0_Safety_Logic
//100ms tick event for timeout logic
#define ev_Task_C0_Safety_Logic_0_100ms  (1<<0)
//New targetspeed has been calculated by the control logic
#define ev_Task_C0_Safety_Logic_1_newTargetSpeed  (1<<1)
//Safety logic requires emergency stop
#define ev_Task_C0_Safety_Logic_2_requestEmergencyStop  (1<<2)
//Request regular Stop
#define ev_Task_C0_Safety_Logic_3_requestStop  (1<<3)

//Activation events of task: Task_C0_Safety_Logic
//Activation event of the task
#define ev_Task_C0_Safety_Logic_0_Activate  (1<<0)

//Events of task: Task_C1_Control
//10ms time tick for copntrol loop
#define ev_Task_C1_Control_0_1ms  (1<<0)

//Event will be fired from Task_C0_Communication to notify that a complete lidar scan has been recieved
#define ev_Task_C1_Control_1_lidarScanReady  (1<<1) //@PF added

//Activation events of task: Task_C1_Control
//Activation event of the task
#define ev_Task_C1_Control_0_Activate  (1<<0)

//Events of task: Task_C0_Communication
//Event will be fired from UART ISR to notify that a complete protocol from the remote control has been received
#define ev_Task_C0_Communication_0_Uart_Remote_Rx_completeProtocol  (1<<0)
//Event will be fired from UART ISR to notify that a complete protocol from the RTE timing control has been received
#define ev_Task_C0_Communication_1_Uart_RteTiming_Rx_completeProtocol  (1<<1)
//Event will be fired from CAN ISR to notify that a complete protocol has been received
#define ev_Task_C0_Communication_2_Can_Rx_SDO  (1<<2)
//Event will be fired from UART ISR to notify that a complete LIDAR saftey protocol has been received
#define ev_Task_C0_Communication_3_Uart_Rx_Lidar  (1<<3)
//Event will be fired from UART ISR to notify that a complete LIDAR saftey protocol has been received
#define ev_Task_C0_Communication_4_Can_Rx_PDO  (1<<4) //@PF added
//Event will be fired from CAN Rx ISR to notify that a CAN Frame was received
#define ev_Task_C0_Communication_5_Can_Rx	  	(1<<5) //@PF added

//Activation events of task: Task_C0_Communication
//Activation event of the task
#define ev_Task_C0_Communication_0_Activate  (1<<0)

//Events of task: Task_C0_State
//Smartpower has communicated that all devices have reached the NMT state preoperational
#define ev_Task_C0_State_0_NMTHasBooted  (1<<0)
//All devices are initialised and have booted
#define ev_Task_C0_State_1_DevicesHaveBooted  (1<<1)
//Car received Request Presence Command from GUI / Remote
#define ev_Task_C0_State_2_Present  (1<<2)
//Car received Connect Command from GUI / Remote
#define ev_Task_C0_State_3_Connect  (1<<3)
//Car received Feature Request Command from GUI / Remote
#define ev_Task_C0_State_4_FeatureRequest  (1<<4)
//Car received Feature Activate Command from GUI / Remote
#define ev_Task_C0_State_5_FeatureActivate  (1<<5)
//All pre-run tests have been passed
#define ev_Task_C0_State_6_PreRunCheckOk  (1<<6)
//Car received Stop Command from GUI / Remote
#define ev_Task_C0_State_7_Stop  (1<<7)
//Car received Disconnect Command from GUI / Remote
#define ev_Task_C0_State_8_Disconnect  (1<<8)
//Driver Requests Emergency Brake
#define ev_Task_C0_State_9_EmergencyBrake  (1<<9)
//Safety module: all safetey tests ok
#define ev_Task_C0_State_10_ErrorAllOk  (1<<10)
//Safety module: at least 1 safety test requests safety mode 1 (slow motion)
#define ev_Task_C0_State_11_ErrorRequestError1  (1<<11)
//Safety module: at least 1 safety test requests safety mode 2 (temporal stop)
#define ev_Task_C0_State_12_ErrorRequestError2  (1<<12)
//Safety module: at least 1 safety test requests safety mode 3 (permanent stop)
#define ev_Task_C0_State_13_ErrorRequestError3  (1<<13)

//Activation events of task: Task_C0_State
//Activation event of the task
#define ev_Task_C0_State_0_Activate  (1<<0)

//Events of task: Task_C0_Safety_System
//1ms tick event for timeout supervision
#define ev_Task_C0_Safety_System_0_1ms  (1<<0)

//Activation events of task: Task_C0_Safety_System
//Activation event of the task
#define ev_Task_C0_Safety_System_0_Activate  (1<<0)

//Events of task: Task_C2_TFT
//Received touch event
#define ev_Task_C2_TFT_0_Touch  (1<<0)
//Timing event for cyclic activities like data update
#define ev_Task_C2_TFT_1_100ms  (1<<1)

//Activation events of task: Task_C2_TFT
//Actvation event
#define ev_Task_C2_TFT_0_Activate  (1<<0)


#endif /* SYSTEMEVENTS_H_ */



