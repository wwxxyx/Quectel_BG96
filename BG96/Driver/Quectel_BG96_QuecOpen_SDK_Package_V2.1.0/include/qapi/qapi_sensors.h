/**
  @file qapi_sensors.h

 EXTERNALIZED FUNCTIONS
  none

 INITIALIZATION AND SEQUENCING REQUIREMENTS
  none

 Copyright (c) 2017  by Qualcomm Technologies, Inc.  All Rights Reserved.
 Confidential and Proprietary - Qualcomm Technologies, Inc.
 ======================================================================*/
/*======================================================================
 *
 *        EDIT HISTORY FOR FILE
 *
 *	 This section contains comments describing changes made to the
 *	 module. Notice that changes are listed in reverse chronological
 *	 order.
 *
 *	
 *
 *
 * when 				who 		what, where, why
 * ---------- 	--- 		------------------------------------------------
 ======================================================================*/
#ifndef _QAPI_SENSOR_H
#define _QAPI_SENSOR_H

#include "qapi_status.h"
#include "qapi_txm_base.h" 

#define  TXM_QAPI_SENSOR_BASE                (TXM_QAPI_DATA_SERVICES_BASE + TXM_QAPI_DATA_SERVICES_NUM_IDS - TXM_QAPI_SENSOR_NUM_IDS) 
#define  TXM_QAPI_SENSOR_API                 TXM_QAPI_SENSOR_BASE + 1
#define  TXM_QAPI_SENSOR_API_SET_THRESHOLD   TXM_QAPI_SENSOR_BASE + 2
#define  TXM_QAPI_SENSOR_API_SET_CALLBACK    TXM_QAPI_SENSOR_BASE + 3
#define  TXM_QAPI_SENSOR_API_GET_DATA        TXM_QAPI_SENSOR_BASE + 4

#define TXM_QAPI_SENSOR_NUM_IDS 5


typedef enum {
 QAPI_SENSOR_BMI160_E,/**< BMI 160 Accelometer*/
 QAPI_SENSOR_TMD2772_E,/**<TMD2772 Light sensor*/
} /** @cond */ qapi_Sensor_Type_t /** @endcond */;

typedef enum {
  QAPI_SENSOR_MOTION_X_POS_E,  /**< Motion in positive x direction */
  QAPI_SENSOR_MOTION_X_NEG_E,  /**< Motion in negative x direction */
  QAPI_SENSOR_MOTION_Y_POS_E,  /**< Motion in positive y direction */
  QAPI_SENSOR_MOTION_Y_NEG_E,  /**< Motion in negative y direction */
  QAPI_SENSOR_MOTION_Z_POS_E,  /**< Motion in positive z direction */
  QAPI_SENSOR_MOTION_Z_NEG_E,  /**< Motion in negative z direction */
} /** @cond */ qapi_Sensor_BMI160_Motion_Direction_t/** @endcond */;

typedef enum {
  QAPI_SENSOR_INIT_E, /**< Sensor Initialize*/
  QAPI_SENSOR_SHUTDOWN_E, /**<Sensor Shutdown*/
  QAPI_SENSOR_ENABLE_EVENTS_E, /**<Sensor Enable event*/
  QAPI_SENSOR_DISABLE_EVENTS_E, /**<Sensor Disable event*/
}/** @cond */ qapi_Sensor_API_ID_t /** @endcond */;


typedef struct {
 union {
   struct {
    uint8_t threshold; /**< BMI160 motion threshold*/
   }accel;
   struct {
     float threshold_low; /**< TMD2772 threshold low*/
     float threshold_high; /**< TMD2772 threshold high*/
   }lux; 
 }data;
}/** @cond */ qapi_Sensor_Threshold_t /** @endcond */;

typedef struct {
 qapi_Sensor_Type_t sensor; /**< Sensor type*/
 union {
   struct {
    float x; /**< BMI160 motion x coordinate*/
    float y; /**< BMI160 motion y coordinate*/
    float z; /**< BMI160 motion z coordinate*/
   }accel;
   float lux; /**< TMD2772 lux data*/
 }data;
}/** @cond */ qapi_Sensor_Data_t /** @endcond */;


/**Sensor API Callback*/
/*For QAPI_SENSOR_BMI160 data will be a integer value indicating motion direction(x ,y ,z)*/
/*For QAPI_SENSOR_TMD2772 data will be NULL*/
typedef void (*qapi_Sensor_Callback_t)(void *data);


#ifdef  QAPI_TXM_MODULE

#define qapi_Sensor_Intf(sensor, api_id)                ((qapi_Status_t)  (_txm_module_system_call12)(TXM_QAPI_SENSOR_API, (ULONG)sensor, (ULONG) api_id, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Sensor_Set_Callback(sensor, callback)      ((qapi_Status_t)  (_txm_module_system_call12)(TXM_QAPI_SENSOR_API_SET_CALLBACK, (ULONG)sensor, (ULONG) callback, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Sensor_Set_Threshold(sensor,  threshold)   ((qapi_Status_t)  (_txm_module_system_call12)(TXM_QAPI_SENSOR_API_SET_THRESHOLD, (ULONG)sensor, (ULONG) threshold, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))
#define qapi_Sensor_Get_Data(sensor,  data)             ((qapi_Status_t)  (_txm_module_system_call12)(TXM_QAPI_SENSOR_API_GET_DATA, (ULONG)sensor, (ULONG) data, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0, (ULONG) 0))


#else

UINT qapi_Sesnor_API_Handler(UINT id, UINT sensor, UINT b, UINT c, UINT d, UINT e, UINT f, UINT g, UINT h, UINT i, UINT j, UINT k, UINT l);

/**
 * @brief API interface to sensor
 * @param sensor Sensor ID
 * @param api_id API ID
 * @return QAPI_OK on Success or QAPI_ERROR on Failure
 */
qapi_Status_t qapi_Sensor_Intf(qapi_Sensor_Type_t sensor, qapi_Sensor_API_ID_t api_id);

/**
 * @brief Set Sensor callback
 * @param sensor Sensor ID
 * @param callback Callback
 * @return QAPI_OK on Success or QAPI_ERROR on Failure
 */
qapi_Status_t qapi_Sensor_Set_Callback(qapi_Sensor_Type_t sensor, qapi_Sensor_Callback_t callback);

/**
 * @brief Set Sensor Threshold
 * @param sensor Sensor ID
 * @param data Threshold data
 * @return QAPI_OK on Success or QAPI_ERROR on Failure
 */
qapi_Status_t qapi_Sensor_Set_Threshold(qapi_Sensor_Type_t sensor, const qapi_Sensor_Threshold_t *data);

/**
 * @brief Get Sensor Data
 * @param sensor Sensor ID
 * @param data Sensor Data pointer
 * @return QAPI_OK on Success or QAPI_ERROR on Failure
 */
qapi_Status_t qapi_Sensor_Get_Data(qapi_Sensor_Type_t sensor, qapi_Sensor_Data_t *data);
#endif

#endif
