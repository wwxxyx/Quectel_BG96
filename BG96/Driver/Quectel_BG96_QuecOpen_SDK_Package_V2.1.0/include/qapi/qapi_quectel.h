#ifndef __QAPI_QUECTEL_H__
#define __QAPI_QUECTEL_H__

#include "tx_api.h"
#include "qapi_status.h"
#include "qapi_txm_base.h"
#include "qapi_adc_types.h"
#include "qapi/qapi_dss.h"

/********************* Quectel OEM error number defines **********************/
typedef enum{
	QAPI_QT_ERR_OK = 0x1000,
	QAPI_QT_ERR_NORMAL_FAIL,
	QAPI_QT_ERR_INVALID_PARAM,
	QAPI_QT_ERR_OPERATE_FAILED,
	QAPI_QT_ERR_NO_MEM,
	QAPI_QT_ERR_PORT_ID,
	QAPI_QT_ERR_STREAM_ID,
	QAPI_QT_ERR_LOC_STOPPED,
	QAPI_QT_ERR_LOC_STARTED,

	/*  end  */
	QAPI_QT_ERR_END = 0x2000
}QAPI_QT_ERR_NUM;


/********************* Quectel Release Information **********************/
typedef struct{
	char build_time[32];
	char build_date[32];
	char rel_time[32];
	char rel_date[32];
	char sw_version[128];
	char ql_sw_ver[64];
	char ql_sw_ver_sub[16];
}qapi_QT_Rel_Info;

/********************* Quectel OEM defines **********************/

typedef enum
{
	QAPI_FATAL_ERR_RESET = 0,
	QAPI_FATAL_ERR_SAHARA = 1,
	QAPI_FATAL_ERR_MAX
}QAPI_FATAL_ERR_MODE;

/* --- ATC Pipe --- */
typedef enum qapi_at_port_status_e
{
	QAPI_PORT_OK = 0,
	QAPI_PORT_ERROR,
	QAPI_PORT_INV_ARG,
	QAPI_PORT_ERROR_SIO,
	QAPI_PORT_OPEN_FAILURE,
	QAPI_PORT_MAX
} qapi_at_port_status_t;

typedef enum qpi_at_port_e
{
	QAPI_AT_PORT_0 = 0,
	QAPI_AT_PORT_1,
	QAPI_AT_PORT_2,	//Reserved
	QAPI_AT_PORT_3,	//Reserved

	QAPI_AT_PORT_MAX
} qapi_at_port_t;

typedef struct qapi_atc_pipe_data_s
{
	char   data[2048];
	int    len;
} qapi_at_pipe_data_t; 

typedef signed short int2;

typedef int2	qapi_at_stream_id_t;
typedef void  (*qapi_at_resp_func_cb_t)(qapi_at_pipe_data_t *data);	//response callback

/********** 	GNSS START 	**********/
typedef uint64_t qtLocEventRegMaskT;
#define QT_LOC_EVENT_MASK_POSITION_REPORT 		((qtLocEventRegMaskT)0x00000001ull)
#define QT_LOC_EVENT_MASK_GNSS_SV_INFO 			((qtLocEventRegMaskT)0x00000002ull)
#define QT_LOC_EVENT_MASK_NMEA 					((qtLocEventRegMaskT)0x00000004ull)

typedef void (*qapi_QT_Loc_CB_t)(char *nmea_data);	//response callback
/********** 	GNSS END 	**********/

/********** 	Fota START 	**********/
/* 
*error_id:[out]
*0         Download successful
*701       HTTP(S) unknown error
*others    unknown error
*/
typedef void(*qapi_Fota_Http_dl_CB_t)(int error_id);

/* 
*error_id:[out]
*0         upgrade success 
*504       firmware upgrade failed
*505       upgrade package not exist   
*506       upgrade package check failed
*others    unknown error
*/
typedef void (*qapi_QT_Fota_Response_CB_t)(short int error_id);	//response callback

/* 
*phase: [out]
*0  upgrading period
*1  restoring period
*Note: The fota upgrading start with upgrading period. When upgrading progress reach to 100%, 
*      it will become to restoring period. When restoring progress reach to 100%, It means that upgrading is successful. 
*      Then the module should be restarted.
*      
*percent:[out]
*upgrade progress, range:[1-100]
*/
typedef void (*qapi_QT_Fota_Upgrade_Progress_CB_t)(unsigned char phase, unsigned char percent);	//response callback

/********** 	Fota END 	**********/


/********************* Quectel OEM ID defines **********************/
/* @Note: Each new OEM ID needs to be defined in this enum */
typedef enum
{
	TXM_OEM_API_QT_BASE = TXM_QC_OEM_API_BASE,	/**<-Quectel TXM API IDs Start with 0x20000->*/
	TXM_OEM_API_QT_REL_INFO,
	TXM_OEM_API_QT_RESET_DEVICE,

	/** ATC PIPE TXM ID */
	TXM_OEM_API_AT_PORT_OPEN,
	TXM_OEM_API_AT_SEND,
	TXM_OEM_API_AT_SEND_HEXBYTE,
	TXM_OEM_API_AT_SEND_GENERAL,
	TXM_OEM_API_AT_PORT_CLOSE,

	/** ADC TXM ID */
	TXM_OEM_API_QT_ADC_READ,

	/** GNSS TXM ID */
	TXM_OEM_API_QT_LOC_START,
	TXM_OEM_API_QT_LOC_STOP,

    /** FOTA TXM ID */
    TXM_OEM_API_QT_FOTA_REMOVE_PACKAGE,
    TXM_OEM_API_QT_FOTA_HTTP_DL_START,
    TXM_OEM_API_QT_FOTA_UPDATE_START,

	/** RAI TXM ID*/
	TXM_OEM_API_QT_RAI,

	TXM_OEM_API_QT_SAHARA_GET,
	TXM_OEM_API_QT_SAHARA_SET,

	/** PSM CORE CONFIG TXM ID*/
	TXM_OEM_API_QT_PSM_SRV_CFG_SET,
	TXM_OEM_API_QT_PSM_SRV_CFG_GET,

	/** SHUTDOWN TXM ID*/
	TXM_OEM_API_QT_SHUTDOWN_DEVICE,

	TXM_OEM_API_QT_END = TXM_QC_OEM_API_END		/**<-Quectel TXM API IDs End with 0x3000->*/
} TXM_OEM_API_ID_DEFINES_E;

/********************* Quectel OEM QAPI functions **********************/
#ifdef  QAPI_TXM_MODULE  

#define qapi_QT_Welcome(a)			((UINT)(_txm_module_kernel_call_dispatcher)(TXM_OEM_API_QT_BASE,(ULONG)a, (ULONG)0, (ULONG)0))
#define qapi_QT_Get_Rel_Info(a)		((UINT)(_txm_module_kernel_call_dispatcher)(TXM_OEM_API_QT_REL_INFO,(ULONG)a, (ULONG)0, (ULONG)0))
#define qapi_QT_Reset_Device(a)		((UINT)(_txm_module_kernel_call_dispatcher)(TXM_OEM_API_QT_RESET_DEVICE,(ULONG)a, (ULONG)0, (ULONG)0))

#define qapi_QT_Apps_AT_Port_Open(a, b, c)		((UINT)(_txm_module_kernel_call_dispatcher)(TXM_OEM_API_AT_PORT_OPEN,(ULONG)a, (ULONG)b, (ULONG)c))
#define qapi_QT_Apps_Send_AT(a, b)				((UINT)(_txm_module_kernel_call_dispatcher)(TXM_OEM_API_AT_SEND,(ULONG)a, (ULONG)b, (ULONG)0))
#define qapi_QT_Apps_Send_AT_HexByte(a, b)		((UINT)(_txm_module_kernel_call_dispatcher)(TXM_OEM_API_AT_SEND_HEXBYTE,(ULONG)a, (ULONG)b, (ULONG)0))
#define qapi_QT_Apps_Send_AT_General(a, b, c)	((UINT)(_txm_module_kernel_call_dispatcher)(TXM_OEM_API_AT_SEND_GENERAL,(ULONG)a, (ULONG)b, (ULONG)c))
#define qapi_QT_Apps_AT_Port_Close(a)			((UINT)(_txm_module_kernel_call_dispatcher)(TXM_OEM_API_AT_PORT_CLOSE,(ULONG)a, (ULONG)0, (ULONG)0))

#define qapi_QT_ADC_Read(a,b)		((UINT)(_txm_module_kernel_call_dispatcher)(TXM_OEM_API_QT_ADC_READ,(ULONG)a, (ULONG)b, (ULONG)0))
#define qapi_QT_RAI_Set(a,b)						((UINT)(_txm_module_kernel_call_dispatcher)(TXM_OEM_API_QT_RAI,(ULONG)a, (ULONG)b, (ULONG)0))

/********** 	GNSS START 	**********/
#define qapi_QT_Loc_Start(a,b)		((UINT)(_txm_module_kernel_call_dispatcher)(TXM_OEM_API_QT_LOC_START,(ULONG)a, (ULONG)b, (ULONG)0))
#define qapi_QT_Loc_Stop()			((UINT)(_txm_module_kernel_call_dispatcher)(TXM_OEM_API_QT_LOC_STOP,(ULONG)0, (ULONG)0, (ULONG)0))
/********** 	GNSS END 	**********/

/********** 	FOTA START 	**********/
#define qapi_QT_Remove_Fota_Package()              ((UINT)(_txm_module_kernel_call_dispatcher)(TXM_OEM_API_QT_FOTA_REMOVE_PACKAGE,(ULONG)0, (ULONG)0, (ULONG)0)) 
#define qapi_QT_Fota_Http_Download_Start(a,b)      ((UINT)(_txm_module_kernel_call_dispatcher)(TXM_OEM_API_QT_FOTA_HTTP_DL_START,(ULONG)a, (ULONG)b, (ULONG)0))
#define qapi_QT_Fota_Update_Start(a,b)		       ((UINT)(_txm_module_kernel_call_dispatcher)(TXM_OEM_API_QT_FOTA_UPDATE_START,(ULONG)a, (ULONG)b, (ULONG)0))
/********** 	FOTA END 	**********/

#define qapi_QT_Sahara_Mode_Get(a)		((UINT)(_txm_module_kernel_call_dispatcher)(TXM_OEM_API_QT_SAHARA_GET,(ULONG)a, (ULONG)0, (ULONG)0))
#define qapi_QT_Sahara_Mode_Set(a)		((UINT)(_txm_module_kernel_call_dispatcher)(TXM_OEM_API_QT_SAHARA_SET,(ULONG)a, (ULONG)0, (ULONG)0))


#define qapi_QT_PSM_SrvCfg_Set(a)					((UINT)(_txm_module_kernel_call_dispatcher)(TXM_OEM_API_QT_PSM_SRV_CFG_SET,(ULONG)a, (ULONG)0, (ULONG)0))
#define qapi_QT_PSM_SrvCfg_Get(a)					((UINT)(_txm_module_kernel_call_dispatcher)(TXM_OEM_API_QT_PSM_SRV_CFG_GET,(ULONG)a, (ULONG)0, (ULONG)0))

#define qapi_QT_Shutdown_Device()					((UINT)(_txm_module_kernel_call_dispatcher)(TXM_OEM_API_QT_SHUTDOWN_DEVICE,(ULONG)0, (ULONG)0, (ULONG)0))
#else

/*
@func
  qapi_QT_Welcome
@brief
  An example for QAPI public.
*/
qapi_Status_t qapi_QT_Welcome(char *welcome);

/*
@func
  qapi_QT_Get_Rel_Info
@brief
  Get apps release information. 
*/
qapi_Status_t qapi_QT_Get_Rel_Info(qapi_QT_Rel_Info *rel_info);

/*
@func
  qapi_QT_Reset_Device
@brief
  reset module. 
*/
qapi_Status_t qapi_QT_Reset_Device(uint16_t mode);

/*
@func
  qapi_QT_Shutdown_Device
@brief
  Shutdown module. 
*/
qapi_Status_t qapi_QT_Shutdown_Device(void);

/*
@func
  qapi_QT_Apps_AT_Port_Open
@brief
  Open Apps AT command port and register a callback. 
*/
qapi_Status_t qapi_QT_Apps_AT_Port_Open(qapi_at_port_t port_id, qapi_at_stream_id_t *stream_id, qapi_at_resp_func_cb_t cb);

/*
@func
  qapi_QT_Apps_Send_AT
@brief
  Send AT command in specifial stream port id. 
*/
qapi_Status_t qapi_QT_Apps_Send_AT(qapi_at_stream_id_t stream_id, const char *command_name);

/*
@func
  qapi_QT_Apps_Send_AT_HexByte
@brief
  Send string in specifial stream port id with Hex format.
*/
qapi_Status_t qapi_QT_Apps_Send_AT_HexByte(qapi_at_stream_id_t stream_id, const char *hex_str);

/*
@func
  qapi_QT_Apps_Send_AT_General
@brief
  Send typeless data in secifial stream port id. Caller can tranmit any type of data through stream port. And
  response will be notified in callback which registed in qapi_QT_Apps_AT_Port_Open(). 
*/
qapi_Status_t qapi_QT_Apps_Send_AT_General(qapi_at_stream_id_t stream_id, void *trans_data, uint32 trans_len);

/*
@func
  qapi_QT_Apps_AT_Port_Close
@brief
  Close AT command port with stream port id. 
*/
void qapi_QT_Apps_AT_Port_Close(qapi_at_stream_id_t stream_id);

/*
@func
  qapi_QT_ADC_Read
@brief
  Read the value of ADC.
*/
qapi_Status_t qapi_QT_ADC_Read(const char *pChn_Name,qapi_ADC_Read_Result_t *result);

/*
@func
  qapi_QT_Loc_Start
@brief
  Start GNSS Location.
*/
qapi_Status_t qapi_QT_Loc_Start(qtLocEventRegMaskT evt_mask, qapi_QT_Loc_CB_t cb);

/*
@func
  qapi_QT_Loc_Stop
@brief
  Stop GNSS Location.
*/
qapi_Status_t qapi_QT_Loc_Stop(void);

/*
@func
  qapi_QT_Remove_Fota_Package
@brief
  Remove fota package.
*/
qapi_Status_t qapi_QT_Remove_Fota_Package(void);

/*
@func
  qapi_QT_Fota_Http_Download_Start
@param[in]
url  fota package path. for example: HTTP://220.180.239.212:8005/BG96_112A_119.zip. The url must be started with "http://" or "https://", not case sensitive
response_cb  response callback, This callback will come when download failed or success.
@brief
  Start Fota package download.
*/
qapi_Status_t qapi_QT_Fota_Http_Download_Start(char* url, qapi_Fota_Http_dl_CB_t response_cb);

/*
@func
  qapi_QT_Fota_Update_Start
@param [in]
  response_cb  response callback,This callback will come when fota upgrade failed or success.
  upgrade_progress_cb   upgrade progress callback, This callback will come every one percent.
@brief
  Start Fota Update.
  
  Note:
    Before use this interface, the delta firmware should be uploded to the efs.
    The storage path for delta firmware package is /fota/update.zip. Both the path and the package name cannot be modified. 
    Customers can use the QEFS Explorer tool provided by Quectel to upload the upgrade package
*/
qapi_Status_t qapi_QT_Fota_Update_Start(qapi_QT_Fota_Response_CB_t response_cb, qapi_QT_Fota_Upgrade_Progress_CB_t upgrade_progress_cb);

/*
@func
  qapi_QT_RAI_Set
@brief
  Set the Release Assistant Information indicator.
@param[in]
  hndl  Pointer to data service handle
  rai_type  Type of Release Assistant Information indicator
  	     0  No information available
  	     1  No further uplink or downlink data transmission
  	     2  Only a single downlink data transmission and no further uplink data 
*/
qapi_Status_t qapi_QT_RAI_Set(qapi_DSS_Hndl_t hndl, uint8_t rai_type);

/*
@func
  qapi_QT_Sahara_Mode_Get
@brief
  Get the NV Item value of Sahara mode setting.
*/
qapi_Status_t qapi_QT_Sahara_Mode_Get(QAPI_FATAL_ERR_MODE *mode);

/*
@func
  qapi_QT_Sahara_Mode_Set
@brief
  Enable or disable module enter sahara dump mode when module crash.
  It will take effect after the module restarted.
*/
qapi_Status_t qapi_QT_Sahara_Mode_Set(QAPI_FATAL_ERR_MODE mode);

/*
@func
  qapi_QT_PSM_SrvCfg_Set
@brief
  Set PSM core server configuration
*/
qapi_Status_t qapi_QT_PSM_SrvCfg_Set(int gpio_enable);

/*
@func
  qapi_QT_PSM_SrvCfg_Get
@brief
  Get PSM core server configuration
*/
qapi_Status_t qapi_QT_PSM_SrvCfg_Get(int *gpio_enable);

#endif

#endif /*__QAPI_QUECTEL_H__*/
