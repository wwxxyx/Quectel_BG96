/******************************************************************************
*@file    example_time.c
*@brief   example of real time operation
*
*  ---------------------------------------------------------------------------
*
*  Copyright (c) 2018 Quectel Technologies, Inc.
*  All Rights Reserved.
*  Confidential and Proprietary - Quectel Technologies, Inc.
*  ---------------------------------------------------------------------------
*******************************************************************************/
#if defined(__EXAMPLE_TIME__)
#include "txm_module.h"
#include "qapi_diag.h"
#include "qapi_timer.h"
#include "qapi_uart.h"
#include "quectel_utils.h"
#include "quectel_uart_apis.h"
#include "example_time.h"

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
  qt_show_real_time
@brief
  Show real time. 
*/
void qt_show_real_time(qapi_time_get_t *time_info)
{
	qt_uart_dbg(uart1_conf.hdlr,
				"[time][ %04d/%02d/%02d %02d:%02d:%02d ]",
				time_info->time_julian.year,
				time_info->time_julian.month,
				time_info->time_julian.day,
				time_info->time_julian.hour,
				time_info->time_julian.minute,
				time_info->time_julian.second
				);
}

/*
@func
  qt_get_real_time
@brief
  Get real time. 
*/
void qt_get_real_time(qapi_time_get_t *time_info)
{
	qapi_Status_t status = QAPI_OK;

	memset(time_info, 0, sizeof(qapi_time_get_t));

	status =  qapi_time_get(QAPI_TIME_JULIAN, time_info);
	if(QAPI_OK != status)
	{
		qt_uart_dbg(uart1_conf.hdlr,"[time] qapi_time_get failed [%d]", status);
		return;
	}

	qt_show_real_time(time_info);

	status =  qapi_time_get(QAPI_TIME_SECS, time_info);
	if(QAPI_OK != status)
	{
		qt_uart_dbg(uart1_conf.hdlr,"[time] qapi_time_get failed [%d]", status);
		return;
	}
	qt_uart_dbg(uart1_conf.hdlr, "[time sec][ %lld ]", time_info->time_secs);

	status =  qapi_time_get(QAPI_TIME_MSECS, time_info);
	if(QAPI_OK != status)
	{
		qt_uart_dbg(uart1_conf.hdlr,"[time] qapi_time_get failed [%d]", status);
		return;
	}
	qt_uart_dbg(uart1_conf.hdlr, "[time msec][ %lld ]", time_info->time_msecs);

	status =  qapi_time_get(QAPI_TIME_STAMP, time_info);
	if(QAPI_OK != status)
	{
		qt_uart_dbg(uart1_conf.hdlr,"[time] qapi_time_get failed [%d]", status);
		return;
	}
	qt_uart_dbg(uart1_conf.hdlr, "[time stamp][ %lld, %lld ]", time_info->time_ts[0],time_info->time_ts[1]);
}

/*
@func
  quectel_task_entry
@brief
  Entry function for task. 
*/
int quectel_task_entry(void)
{
	qapi_time_get_t time_info;
	qapi_Timer_Sleep(1, QAPI_TIMER_UNIT_SEC, true);

	/* uart 1 init */
	uart_init(&uart1_conf);

	/* start uart 1 receive */
	uart_recv(&uart1_conf);

	/* prompt task running */
	qt_uart_dbg(uart1_conf.hdlr,"[task_create] start task ~");


	while (1)
	{
		main_thread_run_couter ++;
		qt_uart_dbg(uart1_conf.hdlr,"[task_create] task run times : %d", main_thread_run_couter);
		IOT_INFO("[task_create] task run times : %d", main_thread_run_couter);

		qt_get_real_time(&time_info);

		/* sleep 1 seconds */
		qapi_Timer_Sleep(1, QAPI_TIMER_UNIT_SEC, true);
	}
}

#endif /*__EXAMPLE_TIME__*/

