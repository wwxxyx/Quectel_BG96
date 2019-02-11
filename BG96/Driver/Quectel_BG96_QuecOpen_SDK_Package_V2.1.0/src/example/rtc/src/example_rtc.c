/******************************************************************************
*@file    example_rtc.c
*@brief   example of rtc operation
*
*  ---------------------------------------------------------------------------
*
*  Copyright (c) 2018 Quectel Technologies, Inc.
*  All Rights Reserved.
*  Confidential and Proprietary - Quectel Technologies, Inc.
*  ---------------------------------------------------------------------------
*******************************************************************************/
#if defined(__EXAMPLE_RTC__)
#include "txm_module.h"
#include "qapi_diag.h"
#include "qapi_timer.h"
#include "qapi_pmapp_rtc.h"
#include "qapi_uart.h"
#include "quectel_utils.h"
#include "quectel_uart_apis.h"
#include "example_rtc.h"

/**************************************************************************
*                                 DEFINE
***************************************************************************/


/**************************************************************************
*                                 GLOBAL
***************************************************************************/
/* uart rx tx buffer */
static char rx_buff[1024];
static char tx_buff[1024];

/* uart config para*/
static QT_UART_CONF_PARA uart1_conf =
{
	NULL,
	QT_UART_PORT_02,
	tx_buff,
	sizeof(tx_buff),
	rx_buff,
	sizeof(rx_buff),
	115200
};

/* conter used to count the total run times for main task */
static unsigned long main_thread_run_couter = 0;

/**************************************************************************
*                           FUNCTION DECLARATION
***************************************************************************/

/**************************************************************************
*                                 FUNCTION
***************************************************************************/
/*
@func
  qt_rtc_init
@brief
  Init rtc and set the display mode.
*/
void qt_rtc_init(void)
{
	qapi_Status_t status = QAPI_OK;

	status = qapi_PM_Rtc_Init();
	if(QAPI_OK != status)
	{
		qt_uart_dbg(uart1_conf.hdlr,"[rtc] qapi_PM_Rtc_Init failed [%d]", status);
		return;
	}

	status = qapi_PM_Set_Rtc_Display_Mode(QAPI_PM_RTC_24HR_MODE_E);
	if(QAPI_OK != status)
	{
		qt_uart_dbg(uart1_conf.hdlr,"[rtc] qapi_PM_Set_Rtc_Display_Mode failed [%d]", status);
		return;
	}
}


/*
@func
  qt_rtc_show_time
@brief
  Show rtc time. 
*/
void qt_rtc_show_time(qapi_PM_Rtc_Julian_Type_t *rtc_date)
{
	qt_uart_dbg(uart1_conf.hdlr,"[rtc] %04d/%02d/%02d-%02d:%02d:%02d",
								 rtc_date->year,
								 rtc_date->month,
								 rtc_date->day,
								 rtc_date->hour,
								 rtc_date->minute,
								 rtc_date->second
								 );
}

/*
@func
  qt_rtc_get_time
@brief
  Get rtc time. 
*/
void qt_rtc_get_time(qapi_PM_Rtc_Julian_Type_t *rtc_date)
{
	qapi_Status_t status = QAPI_OK;

	memset(rtc_date, 0, sizeof(qapi_PM_Rtc_Julian_Type_t));
	status = qapi_PM_Rtc_Read_Cmd(rtc_date);
	if(QAPI_OK != status)
	{
		qt_uart_dbg(uart1_conf.hdlr,"[rtc] qapi_PM_Rtc_Read_Cmd failed [%d]", status);
		return;
	}

	qt_rtc_show_time(rtc_date);
}

/*
@func
  qt_rtc_rw_alarm_time
@brief
  Set/get rtc alarm time. 
*/
void qt_rtc_rw_alarm_time(qapi_PM_Rtc_Cmd_Type_t cmd,
					 qapi_PM_Rtc_Alarm_Type_t what_alarm,
					 qapi_PM_Rtc_Julian_Type_t *rtc_date)
{
	qapi_Status_t status = QAPI_OK;

	status = qapi_PM_Rtc_Alarm_RW_Cmd(cmd, what_alarm, rtc_date);
	if(QAPI_OK != status)
	{
		qt_uart_dbg(uart1_conf.hdlr,"[rtc] qapi_PM_Rtc_Alarm_RW_Cmd failed [%d]", status);
	}

	if(QAPI_PM_RTC_GET_CMD_E == cmd)
	{
		qt_rtc_show_time(rtc_date);
	}
}

/*
@func
  quectel_task_entry
@brief
  Entry function for task. 
*/
int quectel_task_entry(void)
{
	qapi_PM_Rtc_Julian_Type_t rtc_date;
	
	qapi_Timer_Sleep(1, QAPI_TIMER_UNIT_SEC, true);

	/* uart 1 init */
	uart_init(&uart1_conf);

	/* start uart 1 receive */
	uart_recv(&uart1_conf);

	/* prompt task running */
	qt_uart_dbg(uart1_conf.hdlr,"[task_create] start task ~");

	/* initial RTC */
	qt_rtc_init();

	while (1)
	{
		main_thread_run_couter ++;
		qt_uart_dbg(uart1_conf.hdlr,"[task_create] task run times : %d", main_thread_run_couter);
		IOT_INFO("[task_create] task run times : %d", main_thread_run_couter);

		qt_rtc_get_time(&rtc_date);

		/* sleep 2 seconds */
		qapi_Timer_Sleep(1, QAPI_TIMER_UNIT_SEC, true);
	}
}

#endif /*__EXAMPLE_RTC__*/

