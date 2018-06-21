/**
*  file    REMOTEMSG.h
*
*  brief   signal base class for signal REMOTEMSG
*
*  author  Francesco D'Aprea <francesco.daprea@linde-mh.de>
*          Peter Fromm       <peter.fromm@h-da.de>
*
*
* Version v5
* - Scaler get raw and application data by pointer, returncode RC_t
* - Timeout value deleted as attribute
* - Init function extended with return value
*
* Version v4
* - Version used for review with Linde / Still 23.7.15
* - Data class aggregates configuration class
*/

#ifndef REMOTEMSG_H_INCLUDED
#define REMOTEMSG_H_INCLUDED

/*================================================[ public includes  ]================================================*/
#include "global.h"
#include "signal_global.h"
#include "signal_stub.h"

#include "det.h"

#include "REMOTEMSG_type.h"



/*================================================[ public defines   ]================================================*/



#ifdef TOGETHER
class REMOTEMSG_scaler
{
	public:
#endif

/********************************************************************************************************************/
/* The scaler function block																						*/
/********************************************************************************************************************/

/**
 *  Todo: Add functionality to the generator for the following casting configurations:
 *  - Default behaviour: create inline cast function
 *  - Same type of raw and application data (especially structs): no cast call
 *  - User specific function: call name of user specific function
 */
/* This macro creates a type specific function which can be used as a default signal value conversion function
*  if there is no conversion needed.
*  Pass the created function to the signal class definition macro 'SIG_DEFINE_SIGNAL_CLASS'
*  as parameter 'CONV_TO_VALUE' and/or 'CONV_TO_RAW'
+  This is may needed to handle structure type castate.
*/
//#define SIG_NO_CONV( REMOTEMSG_data_t )
//STATIC_INLINE REMOTEMSG_data_t REMOTEMSG_data_t##_NO_CONV( REMOTEMSG_data_t val )
//{
//   return val;
//}




#ifdef TOGETHER
}; //class
#endif

/********************************************************************************************************************/
/* The driver API block																						*/
/********************************************************************************************************************/
                                                                                                                       
/**
 *  Definition of function types for driver interfaces.
 *  I-Port
 */
typedef RC_t       (*REMOTEMSG_refresh_func_t   )(       REMOTEMSG_data_t * const, const SIG_IDX_TYPE );

/**
 *  Definition of function types for driver interfaces.
 *  O-Port
 */
typedef RC_t       (*REMOTEMSG_flush_func_t     )( const REMOTEMSG_data_t * const, const SIG_IDX_TYPE );


/********************************************************************************************************************/
/* The signal configuration API block																				*/
/********************************************************************************************************************/
                                                                                                                       
/**
 *  Instances of structure REMOTEMSG_flush_channel_t represent a flushable driver output channel.
 * A channel consistate of a pointer to a callable driver function and an index which tells the driver the channel ID
 * where exactly the passed data has to be sent.
 *
 * Flush channels have to be collected in a list that is then called a flush table.
 * A flush table has to be terminated by setting 'flush_func' of the the last element in the list to 'NULL'.
 * The pointer to the signal specific flush table has to be configured in the signal configuration object.
 *
 * The flush table should be const declared by defining 'SIG_CONST' as 'SIG_CONST_ROM' inside the header
 * 'signal_cfg.h'. This ensures that those data are located to flash and cannot be changed during runtime.
 *
 * If created flush channel tables that are not used in the program and should not be sorted out by the linker
 * 'SIG_DATA_SIZE_OPTIMIZATION' has to be defined as 'SIG_SIZE_OPTIMIZATION_OFF' inside the header 'signal_cfg.h'.
 * Otherwise use SIG_SIZE_OPTIMIZATION_ON instead.
 */
struct REMOTEMSG_flush_channel_s
{
	/**
	 * Index, which will be passed to the output driver
	 */
    const SIG_IDX_TYPE        flush_idx;

    /**
     * Function pointer to the output driver
     */
    REMOTEMSG_flush_func_t const flush_func;
};                                                                                                                     
typedef SIG_CONST struct REMOTEMSG_flush_channel_s SIG_CONST_SIZE_OPTIMIZATION REMOTEMSG_flush_channel_t;
                                                                                                                       
/**
 *  Instances of the structure REMOTEMSG_cfg_t represent the signal configuration object.
 *
 * A signal configuration object consistate of signal specific pointers to scaling functions.
 * If a scaling between the application value and the driver raw value is needed those pointers have to point to
 * non inline functions which have to be implemented outside the signal layer.
 * Function 'scl_to_val_func' calculates an application value from a driver raw value.
 * Function 'scl_to_raw_func' calculates a driver raw value from an application value.
 * Configured scaling functions will overwrite the default conversion of the signal class.
 * If no scaling is needed the pointers have to be set to 'NULL'. In this case the default conversion will be used.
 *
 * A signal is only able to have one connection (channel) to an input driver.
 * To establish this connection the attribute 'refresh_func' has to point to a callable driver function which
 * delivers a new driver raw value. To tell the driver exactly which channel has to used the attribute 'refresh_idx'
 * has to indicate the channel ID. If no input channel has to be connected the pointer 'refresh_func' has to be set
 * to 'NULL'.
 *
 * If a signal should be able to send its value to output drivers the 'p_flush_channel_table' has to point to a list
 * of flushable channels. Flush channels have to be instances of the structure 'REMOTEMSG_flush_channel_t' (for further
 * information please check the definition). If no output channel should be connected the pointer
 * 'p_flush_channel_table' has to be 'NULL'.
 *
 * The instances should be const declared by defining 'SIG_CONST' as 'SIG_CONST_ROM' inside the header 'signal_cfg.h'
 * This ensures that those data are located to flash and cannot be changed during runtime.
 *
 * If created configuration instances that are not used in the program should not be sorted out by the linker
 * 'SIG_DATA_SIZE_OPTIMIZATION' has to be defined as 'SIG_SIZE_OPTIMIZATION_OFF' inside the header 'signal_cfg.h'.
 * Otherwise use SIG_SIZE_OPTIMIZATION_ON instead.
 **/

struct REMOTEMSG_cfg_s
{

    /**
     * Index value which will be passed to the input driver
     */
    const SIG_IDX_TYPE                     refresh_idx;
    /**
     * Function pointer to the input driver
     */
    REMOTEMSG_refresh_func_t const           refresh_func;

    /**
     * Pointer to the output driver table
     */
   const REMOTEMSG_flush_channel_t * const  p_flush_channel_table;

#ifdef TOGETHER
   /**
    * @link association 
    */
   REMOTEMSG_scaler lnkREMOTEMSG_scaler;
   /**@link aggregation*/
   REMOTEMSG_flush_channel_s lnkREMOTEMSG_flush_channel_s;
#endif
};                                                                                                                     
typedef SIG_CONST struct REMOTEMSG_cfg_s SIG_CONST_SIZE_OPTIMIZATION SIG_CFG_SECTION REMOTEMSG_cfg_t;
                              
/********************************************************************************************************************/
/* The data interface block																							*/
/********************************************************************************************************************/

/**
 *  Definition of the time type which will be used for timing related operations and attributes.
 **/
typedef uint16_t REMOTEMSG_time_t;
                                                                                                                       
/**
 *  Definiton of local signal types which can be used to avoid refering the real datatypes TYPE and $Type_raw\$.
 */
typedef REMOTEMSG_data_t REMOTEMSG_value_t;
							  
/**
 *  Instances of the struct REMOTEMSG_t represent the signal objects.
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
class REMOTEMSG_signal
{
	public:
#else
struct REMOTEMSG_s
{                                                                                                                      
#endif

	/**
	 * Data buffer for the application value
	 */
    REMOTEMSG_data_t                    value;

    /**
     * Current signal status
     */
    sig_sts_t                      status;

    /**
     * Current age of the signal (in calling cycles)
     */
    REMOTEMSG_time_t                 age;


   const REMOTEMSG_cfg_t * const    p_sig_cfg;
#ifdef TOGETHER
   /**@link aggregation*/
   REMOTEMSG_cfg_s lnkREMOTEMSG_cfg_s;
#endif

#ifndef TOGETHER
};                                                                                                                     
typedef struct REMOTEMSG_s SIG_DATA_SIZE_OPTIMIZATION REMOTEMSG_t;
#endif                                                                                                                       

/********************************************************************************************************************/
/* Signal initialisation   																							*/
/********************************************************************************************************************/

/**
 * Initializes a signal instance with a passed init value.
 * Depending on the configured conversion or scaling function the raw_value will be calculated.
 * Signal status will change from SIG_STS_STARTUP to SIG_STS_INITIALIZED.
 *
 * @param  REMOTEMSG_t * const signal: Pointer to the signal object which will be initialised
 * @param const REMOTEMSG_data_t value: Initial value of the application data
 * @return RC_t: standard return code, currently only RC_SUCCESS
 *
 **/
STATIC_INLINE RC_t REMOTEMSG_init(       REMOTEMSG_t * const signal
                             , const REMOTEMSG_data_t       value )
{                                                                                                                      

   RC_t ret = RC_SUCCESS;

   const REMOTEMSG_cfg_t * const p_sig_cfg = signal->p_sig_cfg;
   
   signal->value              = value;
   signal->status             = SIG_STS_INITIALIZED;
   signal->age				  = 0;

                                                                                                                       
   return ret;
}                                                                                                                      

/********************************************************************************************************************/
/* Status and timeout control																						*/
/********************************************************************************************************************/

/**
 *  Returns the current signal status of the passed signal instance.
 *
 * @param  REMOTEMSG_t * const signal: Pointer to the signal object
 * @return  sig_status_t: The current signal status
 *
 **/
STATIC_INLINE sig_sts_t REMOTEMSG_get_status( const REMOTEMSG_t * const signal )
{
   return signal->status;
}

/**
 *  Returns TRUE if status of passed signal instance is equal to 'SIG_STS_OUTOFDATE'. Otherwise returns 'FALSE'.
 *
 * @param  REMOTEMSG_t * const signal: Pointer to the signal object
 * @return boolean_t: TRUE if the signal status is invalid, FALSE otherwise
 *
 **/
STATIC_INLINE boolean_t REMOTEMSG_is_status_invalid( const REMOTEMSG_t * const signal )
{
   return (boolean_t)(signal->status == SIG_STS_INVALID);
}

/**
 *  Sets the signal status of the passed signal instance to 'SIG_STS_OUTOFDATE'.
 *
 * @param  REMOTEMSG_t * const signal: Pointer to the signal object
 * @return RC_t: standard return code, currently only RC_SUCCESS
 */
STATIC_INLINE RC_t REMOTEMSG_set_status_invalid( REMOTEMSG_t * const signal )
{
   signal->status = SIG_STS_INVALID;
   return RC_SUCCESS;
}

/**
 *  Sets a new signal status of the passed signal instance by passing the new status.
 *
 * @param  REMOTEMSG_t * const signal: Pointer to the signal object
 * @param  const sig_status_t status: New signal status
 * @return RC_t: standard return code, currently only RC_SUCCESS
 *
 */
STATIC_INLINE RC_t REMOTEMSG_set_status(        REMOTEMSG_t * const signal
                               , const sig_sts_t        status )
{
   signal->status = status;
   return RC_SUCCESS;
}


/**
 * Returns the count of refreshing trials (refresh, set or set_raw) that failed.
 * Prerquisite: The signal is refreshed cyclically
 *
 * @param  REMOTEMSG_t * const signal: Pointer to the signal object
 * @return REMOTEMSG_time_t: Number of cycles since the signal has bee refreshed for the last time
 */
STATIC_INLINE REMOTEMSG_time_t REMOTEMSG_get_age( const REMOTEMSG_t * const signal )
{
   return signal->age;
}

/**
 * Should be called by application or driver if no new value can be provided.
 * @param  REMOTEMSG_t * const signal: Pointer to the signal object
 * @param REMOTEMSG_time_t increment: incrementvalue, typically in [ms]
 * @return void
 */
STATIC_INLINE void REMOTEMSG_inc_age(  REMOTEMSG_t * const signal , REMOTEMSG_time_t increment)
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
STATIC_INLINE REMOTEMSG_time_t REMOTEMSG_get_time_remaining( const REMOTEMSG_t * const signal, REMOTEMSG_time_t timeout )
{
   //Todo: Use saturated_sub_##TYPE_TIME instead
   return (timeout - signal->age);
}

/**
 * Returns TRUE if the signal was just refreshed. Otherwise returns 'FALSE'.
 *
**/
STATIC_INLINE boolean_t REMOTEMSG_is_new( const REMOTEMSG_t * const signal )
{
	   //Todo: Added status check, correct?
	   return (boolean_t)(signal->status == SIG_STS_VALID && signal->age == (REMOTEMSG_time_t)0);
}

/********************************************************************************************************************/
/* Application Data read/write Interface																			*/
/********************************************************************************************************************/

/**
 * Returns the current application value of the passed signal instance.
 *
 * @param  REMOTEMSG_t * const signal: Pointer to the signal object
 * @return REMOTEMSG_data_t: current value of the application data buffer
 *
 **/
STATIC_INLINE REMOTEMSG_data_t REMOTEMSG_get( const REMOTEMSG_t * const signal )
{                                                                                                                      
   return signal->value;                                                                                               
}                                                                                                                      
                                                                                                                       
/**
 * Returns the current signal status of the passed signal instance and writes the current application value into a
 * buffer which has to be provided by the caller. A pointer to the buffer has to be passed.
 *
 * @param  REMOTEMSG_t * const signal: Pointer to the signal object
 * @param  REMOTEMSG_data_t * const value: Pointer for the application data value
 * @return sig_status_t: signal status
 *
 **/
STATIC_INLINE sig_sts_t REMOTEMSG_get_with_status( const REMOTEMSG_t * const signal
                                           ,       REMOTEMSG_data_t * const value  )
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
 * @param  REMOTEMSG_t * const signal: Pointer to the signal object
 * @param  REMOTEMSG_data_t const value: value for the application data buffer
 * @return RC_t: standard return code, currently only RC_SUCCESS
 *
 **/
STATIC_INLINE RC_t REMOTEMSG_set(       REMOTEMSG_t * const signal
                           , const REMOTEMSG_data_t value  )
{

   RC_t ret = RC_SUCCESS;

   const REMOTEMSG_cfg_t * const p_sig_cfg = signal->p_sig_cfg;

   //Increment the age
   signal->value = value;
   signal->age    = (REMOTEMSG_time_t)0;
   signal->status = SIG_STS_VALID;
                                                                                                                       
   return ret;
}                                                                                                                      
                                                                                                                       
                                                                                                                       
/********************************************************************************************************************/
/* Call functions to the connected drivers																			*/
/********************************************************************************************************************/

                                                                                                                       
/**
 *  Calls the connected driver interface to get a new value. If a driver is connected and the driver returns a new
 * raw value the corresponding application value will be calculated immediately by using the default conversion or
 * calling the signal specific scaling function.
 * The signal age will be set to 0 and the new status will be 'SIG_STS_UPTODATE'.
 * If the driver is not returning a new raw value the current signal values will not be changed but the age counter
 * will be incremented. If the current signal values will be older than the configured timeout the signal status will
 * be set to 'SIG_STS_OUTOFDATE'.
 *
 * @param REMOTEMSG_t * const signal: Pointer to the signal object
 * @return  RC_t: Return value of the driver call
 *
 */
STATIC_INLINE RC_t REMOTEMSG_refresh( REMOTEMSG_t * const signal )
{                                                                                                                      
   RC_t ret = RC_ERROR;

   const REMOTEMSG_cfg_t * const p_sig_cfg = signal->p_sig_cfg;

   REMOTEMSG_data_t value;

   //REMOTEMSG_inc_age(signal); //Taken out to support time triggered increment of age
                                                                                                                       

   if ( NULL != p_sig_cfg->refresh_func )                                                                              
   {                                                                                                                   

      ret = p_sig_cfg->refresh_func( &value, p_sig_cfg->refresh_idx);
                                                                                                                       
      if ( ret == RC_SUCCESS )                                                                                         
      {

    	  signal->age    = (REMOTEMSG_time_t)0;
    	  signal->status = SIG_STS_VALID;
    	  signal->value  = value;

      }                                                                                                                
   }                                                                                                                   
                                                                                                                       
   return ret;                                                                                                         
}                                                                                                                      
                                                                                                                       
/**
 *  Calls the connected driver interfaces to send a value. Value will not be sent if it is INVALID.
 *
 * @param REMOTEMSG_t * const signal: Pointer to the signal object
 * @return  RC_t: Return value of the driver call
 *
 **/
STATIC_INLINE RC_t REMOTEMSG_flush( const REMOTEMSG_t * const signal )
{                                                                                                                      
   RC_t ret = RC_SUCCESS;
   const REMOTEMSG_flush_channel_t * p_flush_channel =
                                            (const REMOTEMSG_flush_channel_t *)(signal->p_sig_cfg->p_flush_channel_table);
                                                                                                                       
   if ( ! REMOTEMSG_is_status_invalid( signal ) )
   {                                                                                                                   
      if ( NULL != p_flush_channel )                                                                                   
      {                                                                                                                
         while ( NULL != p_flush_channel->flush_func )                                                                 
         {
        	 //Todo: add exception handling for other error codes
            if ( p_flush_channel->flush_func( (const REMOTEMSG_data_t * const)&(signal->value)
                                            , p_flush_channel->flush_idx                   ) != RC_SUCCESS )             
            {                                                                                                          
               ret = RC_ERROR;                                                                                         
            }                                                                                                          
            p_flush_channel++;                                                                                         
         }                                                                                                             
      }                                                                                                                
   }                                                                                                                   
   else                                                                                                                
   {                                                                                                                   
      ret = RC_ERROR;                                                                                                  
   }                                                                                                                   
   return ret;                                                                                                         
}                                                                                                                      
                                                                                                                       
/**
 *  Refreshes the signal values before delivering the application value to the caller.
 *
 * @param REMOTEMSG_t * const signal: Pointer to the signal object
 * @param REMOTEMSG_data_t * const value: Pointer for returning the application value
 * @return  RC_t: Return value of the driver call
 *
 **/
STATIC_INLINE RC_t REMOTEMSG_read( REMOTEMSG_t * const signal
                            ,     REMOTEMSG_data_t * const value  )
{                                                                                                                      
   RC_t ret = REMOTEMSG_refresh( signal );
   *value = REMOTEMSG_get( signal );
   return ret;                                                                                                         
}                                                                                                                      
                                                                                                                       
/**
 *  Refreshes the signal values before delivering the application value and the signal status information to the
 * caller.
 *
 * @param REMOTEMSG_t * const signal: Pointer to the signal object
 * @param REMOTEMSG_data_t * const value: Pointer for returning the application value
 * @param sig_status_t* const status: Pointer for returning the signal status
 * @return  RC_t: Return value of the driver call
 *
 **/
STATIC_INLINE RC_t REMOTEMSG_read_with_status(  REMOTEMSG_t * const signal
                                     , REMOTEMSG_data_t * const value
                                     , sig_sts_t* const status    )
{                                                                                                                      
   RC_t ret = REMOTEMSG_refresh( signal );
   *status   = REMOTEMSG_get_with_status( signal, value );
   return ret;                                                                                                         
}                                                                                                                      
                                                                                                                       
                                                                                                                       
/**
 * First sets a new application value to the passed signal instance by passing the new value.
 * The corresponding driver raw value will be calculated immediately by using the default conversion or calling the
 * signal specific scaling function.
 * The signal age will be set to 0 and the new status will be 'SIG_STS_UPTODATE'.
 * After setting those values the driver raw value will be flushed immediately to the configured flush channels.
 *
 * @param REMOTEMSG_t * const signal: Pointer to the signal object
 * @param const REMOTEMSG_data_t value: The new value for the application data buffer
 * @return  RC_t: Return value of the driver call
 *
 **/
STATIC_INLINE RC_t REMOTEMSG_write(       REMOTEMSG_t * const signal
                             , const REMOTEMSG_data_t             value  )
{                                                                                                                      
   RC_t ret = REMOTEMSG_set( signal, value );
                                                                                                                       
   if ( ret == RC_SUCCESS )                                                                                            
   {                                                                                                                   
      ret = REMOTEMSG_flush( signal );
   }                                                                                                                   
                                                                                                                       
   return ret;                                                                                                         
}                                                                                                                      
                                                                                                                       
                                                                                                                       
/**
 * Refreshes all signals of the signal list to which the passed pointer points to.
 * Last element in the list has to be 'NULL'.
 *
 * @param REMOTEMSG_t * const * const p_signal_list: Pointer to the signal list
 * @return RC_t: RC_SUCCESS if all calls have been successful, RC_ERROR otherwise
 */
STATIC_INLINE RC_t REMOTEMSG_refresh_all( REMOTEMSG_t * const * const p_signal_list )
{                                                                                                                      
   RC_t ret = RC_SUCCESS;
   REMOTEMSG_t * const * p_elem = (REMOTEMSG_t * const *)p_signal_list;
                                                                                                                       
   while ( NULL != *p_elem )                                                                                           
   {                                                                                                                   
      if ( REMOTEMSG_refresh( (REMOTEMSG_t * const)(*p_elem) ) != RC_SUCCESS )
      {                                                                                                                
         ret = RC_ERROR;                                                                                               
      }                                                                                                                
      p_elem++;                                                                                                        
   }                                                                                                                   
                                                                                                                       
   return ret;
}                                                                                                                      
                                                                                                                       
/**
 *  Flushes all signals of the signal list to which the passed pointer points to.
 * Last element in the list has to be 'NULL'.
 *
 * @param REMOTEMSG_t * const * const p_signal_list: Pointer to the signal list
 * @return RC_t: RC_SUCCESS if all calls have been successful, RC_ERROR otherwise
 *
 */
STATIC_INLINE RC_t REMOTEMSG_flush_all( REMOTEMSG_t * const * const p_signal_list )
{                                                                                                                      
   RC_t ret = RC_SUCCESS;
   const REMOTEMSG_t * const * p_elem = (const REMOTEMSG_t * const *)p_signal_list;
                                                                                                                       
   while ( NULL != *p_elem )                                                                                           
   {
	   //Todo: add other return code error handling
      if ( REMOTEMSG_flush( (const REMOTEMSG_t * const)(*p_elem) ) != RC_SUCCESS )
      {                                                                                                                
         ret = RC_ERROR;                                                                                               
      }                                                                                                                
      p_elem++;                                                                                                        
   }                                                                                                                   
                                                                                                                       
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
*/
#define REMOTEMSG_CREATE_SIGNAL(																	    \
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
static const REMOTEMSG_cfg_t SIGNAL_NAME##_cfg __attribute__ ((section ( #MEMROM )))=				    \
{                                                                                                   \
   REFRESH_IDX,                                                                                     \
   REFRESH_FCT,                                                                                     \
   FLUSH_CHANNEL_TABLE                                                                              \
};                                                                                                  \
																									\
REMOTEMSG_t SIGNAL_NAME __attribute__ ((section ( #MEMRAM )))=										\
{                                                                                                   \
   INIT_VALUE,                                                                                      \
   SIG_STS_STARTUP,                                                                                 \
   (REMOTEMSG_time_t)0,                                                                               \
   &SIGNAL_NAME##_cfg																				\
};																									\



#endif

