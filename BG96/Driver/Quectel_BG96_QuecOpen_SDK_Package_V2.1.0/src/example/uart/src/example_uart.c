/******************************************************************************
*@file    example_uart.c
*@brief   example of uart operation
*
*  ---------------------------------------------------------------------------
*
*  Copyright (c) 2018 Quectel Technologies, Inc.
*  All Rights Reserved.
*  Confidential and Proprietary - Quectel Technologies, Inc.
*  ---------------------------------------------------------------------------
*******************************************************************************/
#if defined(__EXAMPLE_UART__)
/**************************************************************************
*                                 INCLUDE
***************************************************************************/
#include "qapi_uart.h"
#include "qapi_timer.h"
#include "qapi_diag.h"
#include "quectel_utils.h"
#include "quectel_uart_apis.h"
#include "example_uart.h"
#include "qapi_device_info.h"


/**************************************************************************
*                                 GLOBAL
***************************************************************************/
#ifdef QT_UART_ENABLE_1ST
/* uart1 rx tx buffer */
static char rx1_buff[4096];	/*!!! should keep this buffer as 4K Bytes */
static char tx1_buff[4096];

/* uart config para*/
static QT_UART_CONF_PARA uart1_conf =
{
	NULL,
	QT_UART_PORT_01,
	tx1_buff,
	sizeof(tx1_buff),
	rx1_buff,
	sizeof(rx1_buff),
	115200
};
#endif

#ifdef QT_UART_ENABLE_2ND
/* uart2 rx tx buffer */
static char rx2_buff[4096]; /*!!! should keep this buffer as 4K Bytes */
static char tx2_buff[4096];

/* uart config para*/
static QT_UART_CONF_PARA uart2_conf =
{
	NULL,
	QT_UART_PORT_02,
	tx2_buff,
	sizeof(tx2_buff),
	rx2_buff,
	sizeof(rx2_buff),
	115200
};
#endif

#ifdef QT_UART_ENABLE_3RD
/* uart3 rx tx buffer */
static char rx3_buff[4096]; /*!!! should keep this buffer as 4K Bytes */
static char tx3_buff[4096];

/* uart config para*/
static QT_UART_CONF_PARA uart3_conf =
{
	NULL,
	QT_UART_PORT_03,
	tx3_buff,
	sizeof(tx3_buff),
	rx3_buff,
	sizeof(rx3_buff),
	115200
};
#endif

/* conter used to count the total run times*/
static unsigned long task_run_couter = 0;


/**************************************************************************
*                                 FUNCTION
***************************************************************************/

/*
@func
  quectel_task_entry
@brief
  Entry function for task. 
*/
int quectel_task_entry(void)
{
	qapi_Timer_Sleep(2, QAPI_TIMER_UNIT_SEC, true);
	IOT_INFO("QT# quectel_task_entry");

#ifdef QT_UART_ENABLE_1ST
	/* uart 1 init */
	uart_init(&uart1_conf);
	/* start uart 1 receive */
	uart_recv(&uart1_conf);
	/* prompt task running */
	qt_uart_dbg(uart1_conf.hdlr,"[uart 1] UART Example");
#endif

#ifdef QT_UART_ENABLE_2ND
	/* uart 2 init */
	uart_init(&uart2_conf);
	/* start uart 2 receive */
	uart_recv(&uart2_conf);
	/* prompt task running */
	qt_uart_dbg(uart2_conf.hdlr,"[uart 2] UART Example");
#endif

#ifdef QT_UART_ENABLE_3RD
	/* uart 3 init */
	uart_init(&uart3_conf);
	/* start uart 3 receive */
	uart_recv(&uart3_conf);
	/* prompt task running */
	qt_uart_dbg(uart3_conf.hdlr,"[uart 3] UART Example");
#endif

	while (1)
	{
		task_run_couter ++;

#ifdef QT_UART_ENABLE_1ST
		/* print log to uart */
		qt_uart_dbg(uart1_conf.hdlr,"[uart 1] task run times : %d", task_run_couter);
#endif

#ifdef QT_UART_ENABLE_2ND
		qt_uart_dbg(uart2_conf.hdlr,"[uart 2] task run times : %d", task_run_couter);
#endif

#ifdef QT_UART_ENABLE_3RD
		qt_uart_dbg(uart3_conf.hdlr,"[uart 3] task run times : %d", task_run_couter);
#endif
		/* sleep 2 seconds */
		qapi_Timer_Sleep(2, QAPI_TIMER_UNIT_SEC, true);

		//After 24 hour, reset the module;
		if(task_run_couter >= 43200)
		{
			qapi_Device_Info_Reset();
		}
	}
}

#endif /*__EXAMPLE_UART__*/

