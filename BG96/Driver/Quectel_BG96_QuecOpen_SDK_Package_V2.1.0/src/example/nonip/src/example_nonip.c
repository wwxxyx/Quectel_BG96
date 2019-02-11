/******************************************************************************
*@file    example_nonip.c
*@brief   Establish Non-ip PDN connection to the network and Send Non-IP UL data.
*  ---------------------------------------------------------------------------
*
*  Copyright (c) 2017 Quectel Technologies, Inc.
*  All Rights Reserved.
*  Confidential and Proprietary - Quectel Technologies, Inc.
*  ---------------------------------------------------------------------------
*******************************************************************************/

#if defined(__EXAMPLE_NONIP__)
/*===========================================================================
						   Header file
===========================================================================*/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

#include "qapi_fs_types.h"
#include "qapi_status.h"
#include "qapi_socket.h"
#include "qapi_dss.h"
#include "qapi_netservices.h"

#include "qapi_dnsc.h"
#include "qapi_uart.h"
#include "qapi_timer.h"
#include "qapi_diag.h"
#include "quectel_utils.h"
#include "example_nonip.h"

/*===========================================================================
                             DEFINITION
===========================================================================*/
#define DSS_ADDR_INFO_SIZE	5
#define DSS_ADDR_SIZE       16

#define GET_ADDR_INFO_MIN(a, b) ((a) > (b) ? (b) : (a))

#define QUEC_NIDP_UART_DBG		//enable debug feature

#ifdef QUEC_NIDP_UART_DBG
#define NIDP_UART_DBG(...)	\
{\
	nipd_uart_debug_print(__VA_ARGS__);	\
}
#else
#define NIDP_UART_DBG(...)
#endif

#define THREAD_STACK_SIZE    (1024 * 16)
#define THREAD_PRIORITY      (180)
#define BYTE_POOL_SIZE       (1024 * 16)

#define NIPD_DATA_STACK_SIZE  (4 * 1024)
#define NIPD_BYTE_POOL_SIZE  (17408) 

#define CMD_BUF_SIZE  100

#define DEFAULT_PUB_TIME 5

/*===========================================================================
                           Global variable
===========================================================================*/
/* TCPClient dss thread handle */
#ifdef QAPI_TXM_MODULE
	static TX_THREAD *dss_thread_handle; 
#else
	static TX_THREAD _dss_thread_handle;
	static TX_THREAD *ts_thread_handle = &_dss_thread_handle;
#endif

/* Thread Stack size and memory pool size */
static unsigned char nipd_dss_stack[THREAD_STACK_SIZE];
ULONG         free_memory_nipd[NIPD_BYTE_POOL_SIZE];
TX_BYTE_POOL  byte_pool_nipd;

TX_EVENT_FLAGS_GROUP dss_signal_handle;

nipd_dss_net_hdl_t nipd_dss_handle;	            /* Related to DSS netctrl */

/* @Note: If netctrl library fail to initialize, set this value will be 1,
 * We should not release library when it is 1. 
 */
signed char nonip_netctl_lib_status = DSS_LIB_STAT_INVALID_E;
unsigned char nonip_datacall_status = DSS_EVT_INVALID_E;

/* Uart Dbg */
qapi_UART_Handle_t dbg_uart_handler;
static char quec_dbg_buffer[128];

/*===========================================================================
                               FUNCTION
===========================================================================*/
void nipd_uart_dbg_init(void)
{
	qapi_Status_t status;
	qapi_UART_Open_Config_t uart_cfg;

	uart_cfg.baud_Rate			= 115200;
	uart_cfg.enable_Flow_Ctrl	= QAPI_FCTL_OFF_E;
	uart_cfg.bits_Per_Char		= QAPI_UART_8_BITS_PER_CHAR_E;
	uart_cfg.enable_Loopback	= 0;
	uart_cfg.num_Stop_Bits		= QAPI_UART_1_0_STOP_BITS_E;
	uart_cfg.parity_Mode		= QAPI_UART_NO_PARITY_E;
	uart_cfg.rx_CB_ISR			= NULL;
	uart_cfg.tx_CB_ISR			= NULL;

	status = qapi_UART_Open(&dbg_uart_handler, QAPI_UART_PORT_001_E, &uart_cfg);
	if (QAPI_OK != status)
	{
		return;
	}
	status = qapi_UART_Power_On(dbg_uart_handler);
}

void nipd_uart_debug_print(const char* fmt, ...) 
{
	va_list arg_list;

	va_start(arg_list, fmt);
    vsnprintf((char *)(quec_dbg_buffer), sizeof(quec_dbg_buffer), (char *)fmt, arg_list);
    va_end(arg_list);
		
	qapi_UART_Transmit(dbg_uart_handler, quec_dbg_buffer, strlen(quec_dbg_buffer), NULL);
	qapi_Timer_Sleep(10, QAPI_TIMER_UNIT_MSEC, true);
}

static int nipd_send_nonip_data(void)
{
	int input_data_len = 0;
	int i = 0, cnt = 0;
	unsigned int nipdMTU = 0;
	char *data = NULL;
	unsigned char ex_data = 0; 

	/* data call should be started first */
	if (nipd_dss_handle.dss_handle)
	{
		/* query current pdn connection mtu size */
		if (qapi_DSS_Get_Link_Mtu(nipd_dss_handle.dss_handle, &nipdMTU) == 0)
		{
			NIDP_UART_DBG("Maximum non-ip data capability is %d bytes", nipdMTU);
		}
		else
		{
			NIDP_UART_DBG("Querying maximum non-ip data capability failed");
			return -1;
		}

		if (input_data_len > nipdMTU)
		{
			NIDP_UART_DBG("Input data length greater than maximum allowed limit of %d bytes", nipdMTU);
			return -1;
		}

		/* Non-IP packet data type */
		if (strcmp((char*)nipd_dss_handle.family, "ex_nonip") == 0)
		{
			ex_data = QAPI_DSS_MO_EXCEPTION_NONIP_DATA;      
		}	
		else if(strcmp((char*)nipd_dss_handle.family, "nonip") == 0)
		{
			ex_data = QAPI_DSS_MO_EXCEPTION_NONE;      
		}	
		else
		{
			NIDP_UART_DBG("Invalid option for non-ip exception type!!\n");
			return -1;    
		}

		/* parepare send data */
		tx_byte_allocate(&byte_pool_nipd, (VOID **)&data, (input_data_len * sizeof(char)), TX_NO_WAIT);
		if (!data)
		{
			NIDP_UART_DBG("Allocating memory for non-ip data buffer failed!\n");
			return -1;
		}

		/* fill in the test data */
		for (i = 0; i < input_data_len; i++)
		{
			if (cnt == 9)
				cnt = 0; 

			data[i] = cnt++;
		}

		/* send UL non-ip data */
		if (qapi_DSS_Nipd_Send(nipd_dss_handle.dss_handle, (void *)data, input_data_len, ex_data) == 0)
		{
			NIDP_UART_DBG("Non-ip UL data sent %s successfully", data);
		}
		else
		{
			if (data != NULL)
			{
				tx_byte_release(data);
			}
			
			NIDP_UART_DBG("Failed to send non-ip UL data");
			return -1;
		}
	}
	else
	{
		NIDP_UART_DBG("Data Call is invalid!");
		return -1;
	}

	return 0;
}

/*
@func
	nipd_dss_net_event_cb
@brief
	Initializes the DSS netctrl library for the specified operating mode.
*/
static void nipd_dss_net_event_cb
( 
	qapi_DSS_Hndl_t 		hndl,
	void 				   *user_data,
	qapi_DSS_Net_Evt_t 		evt,
	qapi_DSS_Evt_Payload_t *payload_ptr 
)
{
	qapi_Status_t status = QAPI_OK;
	
	NIDP_UART_DBG("Data test event callback, event: %d\n", evt);

	switch (evt)
	{
		case QAPI_DSS_EVT_NET_IS_CONN_E:
		{
			NIDP_UART_DBG("Data Call connected! start sending UL nonip data.\n");			
			/* Signal main task */
  			tx_event_flags_set(&dss_signal_handle, DSS_SIG_EVT_CONN_E, TX_OR);
			nonip_datacall_status = DSS_EVT_NET_IS_CONN_E;
			
			break;
		}
		case QAPI_DSS_EVT_NET_NO_NET_E:
		{
			NIDP_UART_DBG("Data Call Disconnected.\n");
			
			if (DSS_EVT_NET_IS_CONN_E == nonip_datacall_status)
			{
				/* Release Data service handle and netctrl library */
				if (nipd_dss_handle.dss_handle)
				{
					status = qapi_DSS_Rel_Data_Srvc_Hndl(nipd_dss_handle.dss_handle);
					if (QAPI_OK == status)
					{
						NIDP_UART_DBG("Release data service handle success\n");
						tx_event_flags_set(&dss_signal_handle, DSS_SIG_EVT_EXIT_E, TX_OR);
					}
				}
				
				if (DSS_LIB_STAT_SUCCESS_E == nonip_netctl_lib_status)
				{
					qapi_DSS_Release(QAPI_DSS_MODE_GENERAL);
				}
			}
			else
			{
				/* DSS status maybe QAPI_DSS_EVT_NET_NO_NET_E before data call establishment */
				tx_event_flags_set(&dss_signal_handle, DSS_SIG_EVT_NO_CONN_E, TX_OR);
			}

			break;
		}
		default:
		{
			NIDP_UART_DBG("Data Call status is invalid.\n");
			
			/* Signal main task */
  			tx_event_flags_set(&dss_signal_handle, DSS_SIG_EVT_INV_E, TX_OR);
			nonip_datacall_status = DSS_EVT_INVALID_E;
			break;
		}
	}
}

/*
@func
	nipd_set_data_param
@brief
	Set special parameter for non-ip pdn type before sending PDN connection request.
*/
static int nidp_set_data_param(unsigned char ex_data)
{
    qapi_DSS_Call_Param_Value_t param_info;

	NIDP_UART_DBG("set data parameter for nipd");
	
	/* Initial Data Call Parameter for NIDP, it should supported by network */
    if (nipd_dss_handle.dss_handle)
    {
        /* Request ext Non-ip pdn type */
		if (ex_data == 0)
		{
	        param_info.buf_val = NULL;
	        param_info.num_val = QAPI_DSS_EXT_RADIO_TECH_NONIP;
	        qapi_DSS_Set_Data_Call_Param(nipd_dss_handle.dss_handle, QAPI_DSS_CALL_INFO_EXT_TECH_E, &param_info);

			strlcpy(nipd_dss_handle.family, "nonip", sizeof(nipd_dss_handle.family));
		}
		else if (ex_data == 1)
		{
			param_info.buf_val = NULL;
			param_info.num_val = QAPI_DSS_MO_EXCEPTION_NONIP_DATA;
			qapi_DSS_Set_Data_Call_Param(nipd_dss_handle.dss_handle, QAPI_DSS_CALL_INFO_MO_EXCEPTION_DATA_E, &param_info);

			param_info.buf_val = NULL;
			param_info.num_val = QAPI_DSS_EXT_RADIO_TECH_NONIP;
			qapi_DSS_Set_Data_Call_Param(nipd_dss_handle.dss_handle, QAPI_DSS_CALL_INFO_EXT_TECH_E, &param_info);

			strlcpy(nipd_dss_handle.family, "ex_nonip", sizeof(nipd_dss_handle.family));
		}
    }
    else
    {
        NIDP_UART_DBG("Dss handler is NULL!!!\n");
		return -1;
    }
	
    return 0;
}

/*
@func
	nipd_dss_netctrl_init
@brief
	Initializes the DSS netctrl library for the specified operating mode.
*/
static int nipd_dss_netctrl_init(void)
{
	int ret_val = 0;
	qapi_Status_t status = QAPI_OK;

	NIDP_UART_DBG("Initializes the DSS netctrl library\n");

	/* Initializes the DSS netctrl library */
	if (QAPI_OK == qapi_DSS_Init(QAPI_DSS_MODE_GENERAL))
	{
		nonip_netctl_lib_status = DSS_LIB_STAT_SUCCESS_E;
		NIDP_UART_DBG("qapi_DSS_Init success\n");
	}
	else
	{
		/* @Note: netctrl library has been initialized */
		nonip_netctl_lib_status = DSS_LIB_STAT_FAIL_E;
		NIDP_UART_DBG("DSS netctrl library has been initialized.\n");
	}
	
	/* Registering callback tcp_dss_handleR */
	do
	{
		NIDP_UART_DBG("Registering Callback dss_handle\n");
		
		/* Obtain data service handle */
		status = qapi_DSS_Get_Data_Srvc_Hndl(nipd_dss_net_event_cb, NULL, &(nipd_dss_handle.dss_handle));
		NIDP_UART_DBG("dss_handle %d, status %d\n", nipd_dss_handle.dss_handle, status);
		
		if (NULL != nipd_dss_handle.dss_handle)
		{
			NIDP_UART_DBG("Registed tcp_dss_handler success\n");
			break;
		}

		/* Obtain data service handle failure, try again after 10ms */
		qapi_Timer_Sleep(10, QAPI_TIMER_UNIT_MSEC, true);
	} while(1);

	return ret_val;
}

/*
@func
	dss_netctrl_start
@brief
	Start the DSS netctrl library, and startup data call.
*/
int nipd_dss_netctrl_start(void)
{
	int rc = 0;
	qapi_Status_t status = QAPI_OK;
		
	rc = nipd_dss_netctrl_init();
	if (0 == rc)
	{
		/* Get valid DSS handler and set the data call parameter */
		nidp_set_data_param(0);
	}
	else
	{
		NIDP_UART_DBG("quectel_dss_init fail.\n");
		return -1;
	}

	NIDP_UART_DBG("qapi_DSS_Start_Data_Call start!!!.\n");
	status = qapi_DSS_Start_Data_Call(nipd_dss_handle.dss_handle);
	if (QAPI_OK == status)
	{
		NIDP_UART_DBG("Start Data service success.\n");
		return 0;
	}
	else
	{
		return -1;
	}
}

/*
@func
	nipd_dss_netctrl_stop
@brief
	Cleans up the DSS netctrl library and close data service.
*/
int nipd_dss_netctrl_stop(void)
{
	qapi_Status_t stat = QAPI_OK;
	
	if (nipd_dss_handle.dss_handle)
	{
		stat = qapi_DSS_Stop_Data_Call(nipd_dss_handle.dss_handle);
		if (QAPI_OK == stat)
		{
			memset(&nipd_dss_handle, 0, sizeof(nipd_dss_handle));
			NIDP_UART_DBG("Stop data call success\n");
		}
	}
	
	return 0;
}	

/*
@func
	nipd_dss_thread
@brief
	The entry of data service task.
*/
void nipd_dss_thread(ULONG param)
{
	ULONG dss_event = 0;
	
	/* Start data call */
	nipd_dss_netctrl_start();

	while (1)
	{
		/* Wait disconnect signal */
		tx_event_flags_get(&dss_signal_handle, DSS_SIG_EVT_DIS_E, TX_OR, &dss_event, TX_WAIT_FOREVER);
		if (dss_event & DSS_SIG_EVT_DIS_E)
		{
			/* Stop data call and release resource */
			nipd_dss_netctrl_stop();
			NIDP_UART_DBG("Data service task exit.\n");
			break;
		}
	}

	NIDP_UART_DBG("Data Service Thread Exit!\n");
	return;
}

/*
@func
	quectel_task_entry
@brief
	The entry of data service task.
*/
int quectel_task_entry(void)
{

	int ret = 0;
	ULONG dss_event = 0;
	int32 sig_mask;

	/* wait 10sec for device startup */
	qapi_Timer_Sleep(10, QAPI_TIMER_UNIT_SEC, true);
	
	/* Initial uart for debug */
	nipd_uart_dbg_init();

	/* Create a memory pool for thread */
    tx_byte_pool_create(&byte_pool_nipd, "byte pool for NIPD", free_memory_nipd, NIPD_BYTE_POOL_SIZE);

	/* Create event signal handle and clear signals */
	tx_event_flags_create(&dss_signal_handle, "dss_signal_event");
	tx_event_flags_set(&dss_signal_handle, 0x0, TX_AND);

	/* Start DSS thread, and detect iface status */
#ifdef QAPI_TXM_MODULE
	if (TX_SUCCESS != txm_module_object_allocate((VOID *)&dss_thread_handle, sizeof(TX_THREAD))) 
	{
		return -1;
	}
#endif
	ret = tx_thread_create(dss_thread_handle, "dataservice thread", nipd_dss_thread, NULL,
							nipd_dss_stack, THREAD_STACK_SIZE, THREAD_PRIORITY, 
							THREAD_PRIORITY, TX_NO_TIME_SLICE, TX_AUTO_START);
	if (ret != TX_SUCCESS)
	{
		IOT_INFO("Thread creation failed\n");
	}

	sig_mask = DSS_SIG_EVT_INV_E | DSS_SIG_EVT_NO_CONN_E | DSS_SIG_EVT_CONN_E | DSS_SIG_EVT_EXIT_E;
	
	while (1)
	{
		/* TCPClient signal process */
		tx_event_flags_get(&dss_signal_handle, sig_mask, TX_OR, &dss_event, TX_WAIT_FOREVER);
		NIDP_UART_DBG("SIGNAL EVENT IS [%d]\n", dss_event);
		
		if (dss_event & DSS_SIG_EVT_INV_E)
		{
			NIDP_UART_DBG("DSS_SIG_EVT_INV_E Signal\n");
			tx_event_flags_set(&dss_signal_handle, ~DSS_SIG_EVT_INV_E, TX_AND);
		}
		else if (dss_event & DSS_SIG_EVT_NO_CONN_E)
		{
			NIDP_UART_DBG("DSS_SIG_EVT_NO_CONN_E Signal\n");
			tx_event_flags_set(&dss_signal_handle, ~DSS_SIG_EVT_NO_CONN_E, TX_AND);
		}
		else if (dss_event & DSS_SIG_EVT_CONN_E)
		{
			NIDP_UART_DBG("DSS_SIG_EVT_CONN_E Signal\n");
			nipd_send_nonip_data();
			tx_event_flags_set(&dss_signal_handle, ~DSS_SIG_EVT_CONN_E, TX_AND);
		}
		else if (dss_event & DSS_SIG_EVT_EXIT_E)
		{
			NIDP_UART_DBG("DSS_SIG_EVT_EXIT_E Signal\n");
			tx_event_flags_set(&dss_signal_handle, ~DSS_SIG_EVT_EXIT_E, TX_AND);
			tx_event_flags_delete(&dss_signal_handle);
			break;
		}
		else
		{
			NIDP_UART_DBG("Unkonw Signal\n");
		}

		/* Clear all signals and wait next notification */
		tx_event_flags_set(&dss_signal_handle, 0x0, TX_AND);	//@Fixme:maybe not need
	}
	
	NIDP_UART_DBG("Quectel Non-ip example is Over!");
	
	return 0;
}
#endif /*__EXAMPLE_DNSCLINET__*/
