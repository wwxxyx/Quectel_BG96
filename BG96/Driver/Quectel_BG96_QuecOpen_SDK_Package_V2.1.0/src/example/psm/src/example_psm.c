/******************************************************************************
*@file    example_psm.c
*@brief   example of psm client
*
*  ---------------------------------------------------------------------------
*
*  Copyright (c) 2018 Quectel Technologies, Inc.
*  All Rights Reserved.
*  Confidential and Proprietary - Quectel Technologies, Inc.
*  ---------------------------------------------------------------------------
*******************************************************************************/
#if defined(__EXAMPLE_PSM__)
#include "txm_module.h"
#include "qapi_diag.h"
#include "qapi_timer.h"
#include "qapi_uart.h"
#include "quectel_utils.h"
#include "quectel_uart_apis.h"
#include "qapi_fs_types.h"
#include "qapi_fs.h"
#include "example_psm.h"
#include "qapi_psm.h"

/**************************************************************************
*                                 DEFINE
***************************************************************************/
#define QUEC_PSM_UART_DBG
#ifdef QUEC_PSM_UART_DBG
#define PSM_UART_DBG(...)	\
{\
	psm_uart_debug_print(__VA_ARGS__);	\
}
#else
#define PSM_UART_DBG(...)
#endif

/**************************************************************************
*                                 GLOBAL
***************************************************************************/

TX_MUTEX				c_mutex;
TX_EVENT_FLAGS_GROUP	c_cond;

TX_THREAD				*psm_data_thread;

int               client_id = -1;
//int               file_log_fd = -1;
int               is_modem_loaded = FALSE;
int               health_check_fail = FALSE;
int               backoff = FALSE;
int               psm_complete = FALSE;
int               nw_full_service = FALSE;
qapi_DSS_Hndl_t   dss_handle;
char              apn[128];
char              ip_addr[128];
int               data_event_freq;
int               is_data_event_cycle = TRUE;
int               test_active_timer = -1;
int               time_in_psm = PSM_DEFAULT_TEST_PSM_TIME;
int               is_modem_required = TRUE;
int               is_data_success_required = FALSE;
//static ULogHandle psm_test_log_handle = NULL;

/*===========================================================================

                           Static & global Variable Declarations

===========================================================================*/

UCHAR         psmt_stack[PSM_DATA_STACK_SIZE];
TX_THREAD     psm_data_thread_handle;
ULONG         free_memory_psmt[PSMT_BYTE_POOL_SIZE];
TX_BYTE_POOL  byte_pool_psmt;

/* Uart Dbg */
qapi_UART_Handle_t psm_dbg_uart_handler;
static char quec_dbg_buffer[128];

void psm_uart_dbg_init(void)
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

	status = qapi_UART_Open(&psm_dbg_uart_handler, QAPI_UART_PORT_002_E, &uart_cfg);
	if (QAPI_OK != status)
	{
		return;
	}
	status = qapi_UART_Power_On(psm_dbg_uart_handler);
}

void psm_uart_debug_print(const char* fmt, ...) 
{
	va_list arg_list;

	va_start(arg_list, fmt);
    vsnprintf((char *)(quec_dbg_buffer), sizeof(quec_dbg_buffer), (char *)fmt, arg_list);
    va_end(arg_list);
		
	qapi_UART_Transmit(psm_dbg_uart_handler, quec_dbg_buffer, strlen(quec_dbg_buffer), NULL);
	qapi_UART_Transmit(psm_dbg_uart_handler, "\r\n", strlen("\r\n"), NULL);
	qapi_Timer_Sleep(10, QAPI_TIMER_UNIT_MSEC, true);
}

void psm_test_net_event_cb( qapi_DSS_Hndl_t hndl,
                                     void * user_data,
                                     qapi_DSS_Net_Evt_t evt,
                                     qapi_DSS_Evt_Payload_t *payload_ptr )
{

    psm_data_test_event_params_type  params;

    memset(&params, 0, sizeof(params));
    params.evt = (dss_net_evt_t)evt;
    params.hndl = hndl;
	
	PSM_UART_DBG("psm_test_net_event(NET:1, NONET:2): %d", evt);

    switch (params.evt)
    {
        case QAPI_DSS_EVT_NET_NO_NET_E: // 0x02
		{
            tx_event_flags_set(&c_cond, PSMT_NO_NET_MASK, TX_OR);
            break;
        }
        case QAPI_DSS_EVT_NET_IS_CONN_E: // 0x01
		{
            tx_event_flags_set(&c_cond, PSMT_CONN_NET_MASK, TX_OR);
            break;
        }
        default:
		{
            break;
        }
    }

    PSM_UART_DBG("Data test event: %d sent to data test thread.", params.evt);

}

void psm_test_init(void)
{

    if (TRUE == is_data_event_cycle)
    {
        PSM_UART_DBG("just before dss_init");

        if (QAPI_DSS_SUCCESS !=  qapi_DSS_Init(QAPI_DSS_MODE_GENERAL))
        {
            PSM_UART_DBG("%s","dss_init failed !!");
        }
        else
        {
            PSM_UART_DBG("dss_init successful");
        }

        do
        {
            /* obtain data service handle */
            PSM_UART_DBG("Registering Callback DSS_Handle");
            qapi_DSS_Get_Data_Srvc_Hndl(psm_test_net_event_cb, NULL, &dss_handle);
            PSM_UART_DBG("dss_handle %d", dss_handle);

            if ( dss_handle != NULL )
            {
                break;
            }
			
            qapi_Timer_Sleep(100, QAPI_TIMER_UNIT_MSEC, true);

        } while(1);
    }
    else
    {
        PSM_UART_DBG("Data service handle registration not required");
    }
}

void psm_test_app_call_back_func(psm_status_msg_type *psm_status)
{
	PSM_UART_DBG("PSM registed callback Start!!!, psm_status: %d", psm_status->status);
	
    if (psm_status)
    {
        // to avoid fludding of logs, don't print health check.
        if ( psm_status->status != PSM_STATUS_HEALTH_CHECK &&
             psm_status->status != PSM_STATUS_NONE )
        {
            PSM_UART_DBG("PSM STATUS:    %s", status_string[psm_status->status].err_name);
            PSM_UART_DBG("REJECT REASON: %s\n", reject_string[psm_status->reason].err_name);
        }

		/* support OOS scene */
		backoff = TRUE;

        switch(psm_status->status)
        {
            // special handling for some cases like modem loaded, not loaded etc.
            case PSM_STATUS_MODEM_LOADED:	//6
			{
                is_modem_loaded = TRUE;
                break;
            }
            case PSM_STATUS_MODEM_NOT_LOADED:	//7
			{
                is_modem_loaded = FALSE;
                break;
            }
            case PSM_STATUS_HEALTH_CHECK:	//10
			{
                if ( !health_check_fail )
                {
                    qapi_PSM_Client_Hc_Ack(client_id);
                }
                break;
            }
            case PSM_STATUS_NW_OOS:	//8
			{
                if ( backoff )
                {
					PSM_UART_DBG("PSM Stauts network OOS, enter backoff!");
                    qapi_PSM_Client_Enter_Backoff(client_id);
                }
                break;
            }
            case PSM_STATUS_NW_LIMITED_SERVICE:	//9
			{
                if ( backoff )
                {
                    qapi_PSM_Client_Enter_Backoff(client_id);
                }
                break;
            }
            case PSM_STATUS_COMPLETE:	// 4
			{
                psm_complete = TRUE;
                tx_event_flags_set(&c_cond, 0x1, TX_OR);
				
                PSM_UART_DBG("Received PSM complete");
                break;
            }
            default:
			{
                break;
            }
        }
    }
}

static int psm_test_register_application(void)
{
    qapi_Status_t  result = QAPI_OK;
    do
    {
        result = qapi_PSM_Client_Register(&client_id, psm_test_app_call_back_func);
        if ( result == QAPI_OK )
        {
            break;
        }
		
        qapi_Timer_Sleep(100, QAPI_TIMER_UNIT_MSEC, true);
    } while(1);
	
    PSM_UART_DBG("Result: %s", result ? "FAIL": "SUCCESS");
    PSM_UART_DBG("Application Id: %d", client_id);
	
    return result;
}

int psm_test_start_lte_data_call(void)
{
	qapi_Status_t rc = 0;
    qapi_DSS_Call_Param_Value_t param_info;

    PSM_UART_DBG("LTE Data call function Entered...");

    if ( dss_handle != NULL )
    {
        /* set data call param */
        param_info.buf_val = NULL;
        param_info.num_val = QAPI_DSS_RADIO_TECH_UNKNOWN;
        PSM_UART_DBG("Setting tech to Automatic");
        qapi_DSS_Set_Data_Call_Param(dss_handle, QAPI_DSS_CALL_INFO_TECH_PREF_E, &param_info);

        param_info.buf_val = apn;
        param_info.num_val = strlen(apn);
        PSM_UART_DBG("Setting APN - %s", apn);
        qapi_DSS_Set_Data_Call_Param(dss_handle, QAPI_DSS_CALL_INFO_APN_NAME_E, &param_info);

        param_info.buf_val = NULL;
        param_info.num_val = QAPI_DSS_IP_VERSION_4;
        PSM_UART_DBG("Setting family to IP");
        qapi_DSS_Set_Data_Call_Param(dss_handle, QAPI_DSS_CALL_INFO_IP_VERSION_E, &param_info);

#if 1	//@Fixme: use UMTS profile 1
        /* set data call param for UMTS*/
        param_info.buf_val = NULL;
        param_info.num_val = 1;
        PSM_UART_DBG("setting profile number to: %d", 1);
        qapi_DSS_Set_Data_Call_Param(dss_handle, QAPI_DSS_CALL_INFO_UMTS_PROFILE_IDX_E, &param_info);
#endif

		/* Start Data Call */
        rc = qapi_DSS_Start_Data_Call(dss_handle);
        PSM_UART_DBG("Start data call executed, rc: %d", rc);
		if (rc != QAPI_OK)
		{
			IOT_DEBUG("Start Data Call failed!");
			return -1;
		}
    }
    else
    {
        PSM_UART_DBG("dss_handle is NULL");
    }
	
    return 0;
}

int psm_test_enter_psm_auto(void)
{
    int             result = FAIL;
    psm_info_type   psm_info;

    PSM_UART_DBG("Enter PSM process executing. Test app work is done.");

    memset(&psm_info, 0, sizeof(psm_info));
    psm_info.psm_time_info.time_format_flag = PSM_TIME_IN_SECS;
    psm_info.psm_wakeup_type = PSM_WAKEUP_MEASUREMENT_NW_ACCESS;

	test_active_timer = 60;	// set defuatl awake timer, 5min
	PSM_UART_DBG("psm_test_enter_psm_auto: test_active_timer: %d", test_active_timer);

    if (test_active_timer >= 0)
    {
        psm_info.active_time_in_secs = test_active_timer;
    }
    else
    {
        psm_info.active_time_in_secs = PSM_DEFAULT_TEST_ACTIVE_TIMER;
    }

    psm_info.psm_time_info.psm_duration_in_secs = time_in_psm;
    if (is_modem_required == FALSE)
    {
        psm_info.psm_wakeup_type = PSM_WAKEUP_MEASUREMENT_ONLY;
    }

    PSM_UART_DBG("Enter into Power save Mode");
    result = qapi_PSM_Client_Enter_Psm(client_id, &psm_info);

    PSM_UART_DBG("Result: %s", result ? "FAIL":"SUCCESS");
    if ( result != PSM_ERR_NONE )
    {
        PSM_UART_DBG("Reason: %s", error_string[result].err_name);
    }

    return 0;
}

/* @func:
 *	 psm_test_auto
 *
 * @desc:
 *	Start a data call and enter into PSM mode automatically
 */
int psm_test_auto(void)
{
    int    result = FAIL;
    ULONG   c_actual_flags;

    PSM_UART_DBG("<========= PSM Test Application Auto =========>");
    //psm_test_read_config_file();

	/* Register a PSM test Client */
    result = psm_test_register_application();

    PSM_UART_DBG("Register application: %d", result);

    //psm_test_evaluate_update_psm_cycle();

    if (TRUE == is_modem_required)
    {
        PSM_UART_DBG("Wait for Device to acquire LTE service");
        psm_test_init();

		/* wait here till service is avaiable */
		qapi_Timer_Sleep(15, QAPI_TIMER_UNIT_SEC, true);

		PSM_UART_DBG("qapi_DSS_Get_Data_Srvc_Hndl success, start data call.");
		
        if (TRUE == is_data_event_cycle)
        {
            PSM_UART_DBG("Data event cycle. Performing Data call.");
            psm_test_start_lte_data_call();
        }
        else
        {
            PSM_UART_DBG("Not a Data event cycle. No Data call");
            psm_test_enter_psm_auto();
        }

    }
    else
    {
        PSM_UART_DBG("Modem not required in this cycle. Send ready");
        psm_test_enter_psm_auto();
    }

	PSM_UART_DBG("Ready to exit the PSM, psm_complete: %d", psm_complete);
	
    do
    {
		/* lock */
		//tx_mutex_get(&c_mutex, TX_WAIT_FOREVER);
		tx_event_flags_get(&c_cond, 0x1, TX_OR_CLEAR, &c_actual_flags, TX_WAIT_FOREVER);
		//tx_mutex_put(&c_mutex);
    } while (!psm_complete);

    PSM_UART_DBG("Exiting as PSM is complete");
    //qapi_FS_Close(file_log_fd);

    return result;
}

int psm_data_damtest_task_entry(ULONG thread_input)
{

    ULONG   actual_flags = 0;
    UINT    status;

	PSM_UART_DBG("=== PSM data task entry ===");

    do
    {
        status = tx_event_flags_get(&c_cond, (PSMT_NO_NET_MASK | PSMT_CONN_NET_MASK), TX_OR_CLEAR, &actual_flags, TX_WAIT_FOREVER);
		PSM_UART_DBG("@tx_event_flags_get status: %d", status);

        if (status != TX_SUCCESS)
        {
            break;
        }

		PSM_UART_DBG("@actual_flags: 0x%x", actual_flags);
		
        switch (actual_flags)
        {
            case PSMT_NO_NET_MASK:
			{
				PSM_UART_DBG("DSS Event Not Connected. Waitting...");
                break;
            }
            case PSMT_CONN_NET_MASK:
			{
                PSM_UART_DBG("DSS Event Connected. Attempting enter psm test");
                psm_test_enter_psm_auto();

                break;
            }
            default:
			{
                break;
            }
        }

    } while(TRUE);

    return 0;
}

#if 0
static void psm_test_log_init(void)
{
    int   result;
    struct qapi_FS_Stat_Type_s log_file;

    //Hardcoded Mechanism to limit the logfile to 10KB or reset the log
    result = qapi_FS_Stat(PSM_DAM_TEST_LOG_FILE, &log_file);
	IOT_INFO("Open Syslog file %s, %d", PSM_DAM_TEST_LOG_FILE, result);
	
    if (QAPI_OK == result)
    {
        if (log_file.st_size > 10000)
        {
            qapi_FS_Unlink(PSM_DAM_TEST_LOG_FILE);
        }
    }
	
    qapi_FS_Open(PSM_DAM_TEST_LOG_FILE, QAPI_FS_O_WRONLY_E|QAPI_FS_O_APPEND_E|QAPI_FS_O_CREAT_E, &file_log_fd);
}
#endif

/* DAM PSM Client */
int quectel_task_entry
(
    void
)
{
    CHAR *data_thread_stack_pointer = NULL;

	/* Initialize debug UART and syslog */
	psm_uart_dbg_init();
	//psm_test_log_init();
	
	PSM_UART_DBG("\r\n===quectel_psm_damtest_task_entry start===\r\n");

    tx_mutex_create(&c_mutex, "PSM Test conditional Mutex", TX_INHERIT);
    tx_event_flags_create(&c_cond, "event flag PSMT 2");

    tx_byte_pool_create(&byte_pool_psmt, "byte pool PSM", free_memory_psmt, PSMT_BYTE_POOL_SIZE);

    /* Allocate the stack for data thread.  */
    tx_byte_allocate(&byte_pool_psmt, (VOID **) &data_thread_stack_pointer, PSM_DATA_STACK_SIZE, TX_NO_WAIT);
    
    /* Create the data thread. */
#ifdef QAPI_TXM_MODULE
		if (TX_SUCCESS != txm_module_object_allocate((VOID *)&psm_data_thread, sizeof(TX_THREAD))) 
		{
			PSM_UART_DBG("Create psm data task error.");
			return -1;
		}
#endif

    tx_thread_create(psm_data_thread, "PSMT Data Thread", psm_data_damtest_task_entry,
                          0, data_thread_stack_pointer, PSM_DATA_STACK_SIZE,
                          148, 148, TX_NO_TIME_SLICE, TX_AUTO_START);

	/* Start psm auto test via sigs? */
    psm_test_auto();
	
	PSM_UART_DBG("\r\nquectel_psm_damtest_task_entry Exit!!!\r\n");

    return 0;
}

#endif /*__EXAMPLE_PSM__*/

