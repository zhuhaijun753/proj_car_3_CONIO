/**
*  file    ENGINEDECODER_ASYNCBUFFER.h
*
*  brief   signal base class for signal ENGINEDECODER_ASYNCBUFFER
*
*  author  Francesco D'Aprea <francesco.daprea@linde-mh.de>
*          Peter Fromm       <peter.fromm@h-da.de>
*
* History:
* 	1.5.18
*		- Added API for getting the pointer to the application data
*
*/

#ifndef ENGINEDECODER_ASYNCBUFFER_H_INCLUDED
#define ENGINEDECODER_ASYNCBUFFER_H_INCLUDED

/*================================================[ public includes  ]================================================*/
#include "global.h"
#include "signal_global.h"
#include "signal_stub.h"

#include "det.h"

#include "ENGINEDECODER_ASYNCBUFFER_type.h"



/*================================================[ public defines   ]================================================*/



#ifdef TOGETHER
class ENGINEDECODER_ASYNCBUFFER_scaler
{
	public:
#endif

/********************************************************************************************************************/
/* The scaler function block																						*/
/********************************************************************************************************************/

//Not required


#ifdef TOGETHER
}; //class
#endif

/********************************************************************************************************************/
/* The driver API block																						        */
/********************************************************************************************************************/
                                                                                                                       
//Not required


/********************************************************************************************************************/
/* The signal configuration API block																				*/
/********************************************************************************************************************/
                                                                                                                       
//Not required

/********************************************************************************************************************/
/* The data interface block																							*/
/********************************************************************************************************************/

/**
 *  Definition of the time type which will be used for timing related operations and attributes.
 **/
typedef uint16_t ENGINEDECODER_ASYNCBUFFER_time_t;
                                                                                                                       
/**
 *  Definiton of local signal types which can be used to avoid refering the real datatypes TYPE and $Type_raw\$.
 */
typedef ENGINEDECODER_ASYNCBUFFER_data_t ENGINEDECODER_ASYNCBUFFER_value_t;
//typedef 0 ENGINEDECODER_ASYNCBUFFER_raw_value_t;
							  
/**
 *  Instances of the struct ENGINEDECODER_ASYNCBUFFER_t represent the signal objects.
 * A signal object contains the current application value 'value' and the driver raw value 'raw_value'.
 * It will be ensured that the application value and the driver raw value are consistent.
 * This requires that there is no direct access to those attributes either from the application nor from the drivers.
 * The interface functions which will be generated by defining the signal class have to be used instead.
 *
 * Furthermore, there is the information if those values are valid.
 * This information is provided by the attribute 'status'.
 *
 * Also there is an attribute 'age' which will be increased if a refreshing will fail.
 * In this case the last valid signal values will not be changed.
 * If a signal will be refreshed cyclically the age counter can be used for timeout monitoring on application level.
 *
 * For testing purpose it is possible to interrupt the signal flow in both directions (application <-> drivers).
 * To disconnect the application input from the signal layer the attribute 'discon_appl_value' has to be set to TRUE.
 * This allows to modify the attribute 'value' directly e.g. by using canape to stimulate the application input.
 * To disconnect the signal layer output to drivers the attribute 'discon_raw_value' has to be set to TRUE.
 * This allows to modify the attribute 'raw_value' directly e.g. by using canape to stimulate the driver input.
 *
 * The pointer 'p_sig_cfg' points to the signal specific configuration data object which may be located in ROM if it
 * is configured in header 'signal_cfg.h
 *
 * If created instances that are not used in the program and should not be sorted out by the linker
 * 'SIG_DATA_SIZE_OPTIMIZATION' has to be defined as 'SIG_SIZE_OPTIMIZATION_OFF' inside the header 'signal_cfg.h'.
 * Otherwise use SIG_SIZE_OPTIMIZATION_ON instead.
 *
 */
#ifdef TOGETHER
class ENGINEDECODER_ASYNCBUFFER_signal
{
	public:
#else
struct ENGINEDECODER_ASYNCBUFFER_s
{                                                                                                                      
#endif

	/**
	 * Data buffer for the application value
	 */
    ENGINEDECODER_ASYNCBUFFER_data_t                    value;

    /**
     * Data buffer for the raw value
     */
    // 0                    raw_value;

    /**
     * Current signal status
     */
    sig_sts_t                      status;

    /**
     * Current age of the signal (in calling cycles)
     */
    ENGINEDECODER_ASYNCBUFFER_time_t                 age;


    /**
     * Disconnect the application value (for developer diagnostic services)
     */
     //boolean_t                             discon_appl_value;

     /**
      * Disconnect the raw data value (for developer diagnostic services)
      */
     //boolean_t                             discon_raw_value;
   //const ENGINEDECODER_ASYNCBUFFER_cfg_t * const    p_sig_cfg;
#ifdef TOGETHER
   /**@link aggregation*/
   ENGINEDECODER_ASYNCBUFFER_cfg_s lnkENGINEDECODER_ASYNCBUFFER_cfg_s;
#endif

#ifndef TOGETHER
};                                                                                                                     
typedef struct ENGINEDECODER_ASYNCBUFFER_s SIG_DATA_SIZE_OPTIMIZATION ENGINEDECODER_ASYNCBUFFER_t;
#endif                                                                                                                       

/********************************************************************************************************************/
/* Signal initialisation   																							*/
/********************************************************************************************************************/

/**
 * Initializes a signal instance with a passed init value.
 * Depending on the configured conversion or scaling function the raw_value will be calculated.
 * Signal status will change from SIG_STS_STARTUP to SIG_STS_INITIALIZED.
 *
 * @param  ENGINEDECODER_ASYNCBUFFER_t * const signal: Pointer to the signal object which will be initialised
 * @param const ENGINEDECODER_ASYNCBUFFER_data_t value: Initial value of the application data
 * @return RC_t: standard return code, currently only RC_SUCCESS
 *
 **/
STATIC_INLINE RC_t ENGINEDECODER_ASYNCBUFFER_init(       ENGINEDECODER_ASYNCBUFFER_t * const signal
                             , const ENGINEDECODER_ASYNCBUFFER_data_t       value )
{                                                                                                                      

   RC_t ret = RC_SUCCESS;

   signal->value              = value;
// signal->raw_value          = raw_value;
   signal->status             = SIG_STS_INITIALIZED;
// signal->discon_appl_value  = FALSE;
// signal->discon_raw_value   = FALSE;
   signal->age				  = 0;

   return ret;
}                                                                                                                      

/********************************************************************************************************************/
/* Status and timeout control																						*/
/********************************************************************************************************************/

/**
 *  Returns the current signal status of the passed signal instance.
 *
 * @param  ENGINEDECODER_ASYNCBUFFER_t * const signal: Pointer to the signal object
 * @return  sig_status_t: The current signal status
 *
 **/
STATIC_INLINE sig_sts_t ENGINEDECODER_ASYNCBUFFER_get_status( const ENGINEDECODER_ASYNCBUFFER_t * const signal )
{
   return signal->status;
}

/**
 *  Returns TRUE if status of passed signal instance is equal to 'SIG_STS_OUTOFDATE'. Otherwise returns 'FALSE'.
 *
 * @param  ENGINEDECODER_ASYNCBUFFER_t * const signal: Pointer to the signal object
 * @return boolean_t: TRUE if the signal status is invalid, FALSE otherwise
 *
 **/
STATIC_INLINE boolean_t ENGINEDECODER_ASYNCBUFFER_is_status_invalid( const ENGINEDECODER_ASYNCBUFFER_t * const signal )
{
   return (boolean_t)(signal->status == SIG_STS_INVALID);
}

/**
 *  Sets the signal status of the passed signal instance to 'SIG_STS_OUTOFDATE'.
 *
 * @param  ENGINEDECODER_ASYNCBUFFER_t * const signal: Pointer to the signal object
 * @return RC_t: standard return code, currently only RC_SUCCESS
 */
STATIC_INLINE RC_t ENGINEDECODER_ASYNCBUFFER_set_status_invalid( ENGINEDECODER_ASYNCBUFFER_t * const signal )
{
   signal->status = SIG_STS_INVALID;
   return RC_SUCCESS;
}

/**
 *  Sets a new signal status of the passed signal instance by passing the new status.
 *
 * @param  ENGINEDECODER_ASYNCBUFFER_t * const signal: Pointer to the signal object
 * @param  const sig_status_t status: New signal status
 * @return RC_t: standard return code, currently only RC_SUCCESS
 *
 */
STATIC_INLINE RC_t ENGINEDECODER_ASYNCBUFFER_set_status(        ENGINEDECODER_ASYNCBUFFER_t * const signal
                               , const sig_sts_t        status )
{
   signal->status = status;
   return RC_SUCCESS;
}


/**
 * Returns the count of refreshing trials (refresh, set or set_raw) that failed.
 * Prerquisite: The signal is refreshed cyclically
 *
 * @param  ENGINEDECODER_ASYNCBUFFER_t * const signal: Pointer to the signal object
 * @return ENGINEDECODER_ASYNCBUFFER_time_t: Number of cycles since the signal has bee refreshed for the last time
 */
STATIC_INLINE ENGINEDECODER_ASYNCBUFFER_time_t ENGINEDECODER_ASYNCBUFFER_get_age( const ENGINEDECODER_ASYNCBUFFER_t * const signal )
{
   return signal->age;
}

/**
 * Should be called by application or driver if no new value can be provided.
 * @param  ENGINEDECODER_ASYNCBUFFER_t * const signal: Pointer to the signal object
 * @param ENGINEDECODER_ASYNCBUFFER_time_t increment: incrementvalue, typically in [ms]
 * @return void
 */
STATIC_INLINE void ENGINEDECODER_ASYNCBUFFER_inc_age(  ENGINEDECODER_ASYNCBUFFER_t * const signal , ENGINEDECODER_ASYNCBUFFER_time_t increment)
{
	   if ( uint16_t_MAX > (signal->age + increment) )
	   {
	      signal->age += increment;
	   }
}


/**
 *  Returns the remaining time until the signal value will be out of date.
 *
 * @param SIGNAL_t * const signal: Pointer to the signal object
 * @return  SIGNAL_time_t: Time (cycles) until timeout will be reached
 */
STATIC_INLINE ENGINEDECODER_ASYNCBUFFER_time_t ENGINEDECODER_ASYNCBUFFER_get_time_remaining( const ENGINEDECODER_ASYNCBUFFER_t * const signal, ENGINEDECODER_ASYNCBUFFER_time_t timeout )
{
   //Todo: Use saturated_sub_##TYPE_TIME instead
   return (timeout - signal->age);
}

/**
 * Returns TRUE if the signal was just refreshed. Otherwise returns 'FALSE'.
 *
**/
STATIC_INLINE boolean_t ENGINEDECODER_ASYNCBUFFER_is_new( const ENGINEDECODER_ASYNCBUFFER_t * const signal )
{
	   //Todo: Added status check, correct?
	   return (boolean_t)(signal->status == SIG_STS_VALID && signal->age == (ENGINEDECODER_ASYNCBUFFER_time_t)0);
}

/********************************************************************************************************************/
/* Application Data read/write Interface																			*/
/********************************************************************************************************************/

/**
 * Returns the current application value of the passed signal instance.
 *
 * @param  ENGINEDECODER_ASYNCBUFFER_t * const signal: Pointer to the signal object
 * @return ENGINEDECODER_ASYNCBUFFER_data_t: current value of the application data buffer
 *
 **/
STATIC_INLINE ENGINEDECODER_ASYNCBUFFER_data_t ENGINEDECODER_ASYNCBUFFER_get( const ENGINEDECODER_ASYNCBUFFER_t * const signal )
{                                                                                                                      
   return signal->value;                                                                                               
}                                                                                                                      

/**
 * Returns the pointer to the current application value of the passed signal instance.
 *
 * @param  ENGINEDECODER_ASYNCBUFFER_t * const signal: Pointer to the signal object
 * @return ENGINEDECODER_ASYNCBUFFER_data_t* const: pointer to current value of the application data buffer
 *
 **/
STATIC_INLINE ENGINEDECODER_ASYNCBUFFER_data_t* const ENGINEDECODER_ASYNCBUFFER_get_by_pointer( const ENGINEDECODER_ASYNCBUFFER_t * const signal )
{                                                                                                                      
   return (ENGINEDECODER_ASYNCBUFFER_data_t* const) &(signal->value);                                                                                               
} 
                                                                                                                       
/**
 * Returns the current signal status of the passed signal instance and writes the current application value into a
 * buffer which has to be provided by the caller. A pointer to the buffer has to be passed.
 *
 * @param  ENGINEDECODER_ASYNCBUFFER_t * const signal: Pointer to the signal object
 * @param  ENGINEDECODER_ASYNCBUFFER_data_t * const value: Pointer for the application data value
 * @return sig_status_t: signal status
 *
 **/
STATIC_INLINE sig_sts_t ENGINEDECODER_ASYNCBUFFER_get_with_status( const ENGINEDECODER_ASYNCBUFFER_t * const signal
                                           ,       ENGINEDECODER_ASYNCBUFFER_data_t * const value  )
{                                                                                                                      
   *value = signal->value;                                                                                             
   return signal->status;
}                                                                                                                      
                                                                                                                       
/**
 * Sets a new application value to the passed signal instance by passing the new value.
 * The corresponding driver raw value will be calculated immediately by using the default conversion or calling the
 * signal specific scaling function.
 * The signal age will be set to 0 and the new status will be 'SIG_STS_UPTODATE'.
 *
 * @param  ENGINEDECODER_ASYNCBUFFER_t * const signal: Pointer to the signal object
 * @param  ENGINEDECODER_ASYNCBUFFER_data_t const value: value for the application data buffer
 * @return RC_t: standard return code, currently only RC_SUCCESS
 *
 **/
STATIC_INLINE RC_t ENGINEDECODER_ASYNCBUFFER_set(       ENGINEDECODER_ASYNCBUFFER_t * const signal
                           , const ENGINEDECODER_ASYNCBUFFER_data_t value  )
{

   RC_t ret = RC_SUCCESS;


   signal->value = value;
   signal->age    = (ENGINEDECODER_ASYNCBUFFER_time_t)0;
   signal->status = SIG_STS_VALID;

                                                                                                                       
   return ret;
}                                                                                                                      
                                                                                                                       

#ifdef TOGETHER
}; //class
#endif

/********************************************************************************************************************/
/* Signal generation macro  																						*/
/********************************************************************************************************************/

/**
* The macro 'SIG_CREATE_SIGNAL' creates a signal instance including the configuration object.
*
* param SIGNAL_CLASS         Name of the signal class of which the signal instance has to be created.
*        SIGNAL_NAME          Name of the signal instance that has to be created e.g. KL7.
*        INIT_VALUE           Initial value for the application value.
*        INIT_RAW_VALUE       Initial value for the driver raw value.
*        SCL_RAW_TO_VAL       Pointer to a scaling function to calculate an application value from a driver raw value
*                             or NULL.
*        SCL_VAL_TO_RAW       Pointer to a scaling function to calculate the driver raw value from an application value
*                             or NULL.
*        REFRESH_IDX          Input driver channel ID.
*        REFRESH_FCT          Pointer to the input driver function or NULL.
*        FLUSH_CHANNEL_TABLE  Pointer to the flush table which contains the flush channels or NULL.
*
*        Note: As this is an interrunnable variable only, some signal parts are ommitted. The interface
*        however is identical to other signals to support maintainability
*
*/
#define ENGINEDECODER_ASYNCBUFFER_CREATE_SIGNAL(																	    \
                           SIGNAL_NAME																\
                         , INIT_VALUE                                                               \
                         , INIT_RAW_VALUE                                                           \
                         , SCL_RAW_TO_VAL                                                           \
                         , SCL_VAL_TO_RAW                                                           \
                         , REFRESH_IDX                                                              \
                         , REFRESH_FCT                                                              \
                         , FLUSH_CHANNEL_TABLE														\
						 , MEMRAM																	\
						 , MEMROM)																	\
                                                                                                    \
ENGINEDECODER_ASYNCBUFFER_t SIGNAL_NAME __attribute__ ((section ( #MEMRAM )))=										\
{                                                                                                   \
   INIT_VALUE,                                                                                      \
   SIG_STS_STARTUP,                                                                                 \
   (ENGINEDECODER_ASYNCBUFFER_time_t)0,                                                                               \
};																									\



#endif

