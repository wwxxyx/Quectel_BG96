/******************************************************************************
*@file    example_task.c
*@brief   example of new task creation
*
*  ---------------------------------------------------------------------------
*
*  Copyright (c) 2018 Quectel Technologies, Inc.
*  All Rights Reserved.
*  Confidential and Proprietary - Quectel Technologies, Inc.
*  ---------------------------------------------------------------------------
*******************************************************************************/
#if defined(__EXAMPLE_TASK_CREATE__)
#include "txm_module.h"
#include "qapi_diag.h"
#include "qapi_timer.h"
#include "qapi_uart.h"
#include "quectel_utils.h"
#include "quectel_uart_apis.h"
#include "example_task.h"

/**************************************************************************
*                                 DEFINE
***************************************************************************/
#define QT_SUB1_THREAD_PRIORITY   	180
#define QT_SUB1_THREAD_STACK_SIZE 	(1024 * 16)

/**************************************************************************
*                                 GLOBAL
***************************************************************************/
/* uart rx tx buffer */
static char rx_buff[1024];
static char tx_buff[1024];

/* uart config para*/
QT_UART_CONF_PARA uart1_conf =
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
/* conter used to count the total run times for sub1 task */
static unsigned long sub1_thread_run_couter = 0;

/* thread handle */
static TX_THREAD* qt_sub1_thread_handle; 
static unsigned char qt_sub1_thread_stack[QT_SUB1_THREAD_STACK_SIZE];

/* TX QUEUE handle */
static TX_QUEUE tx_queue_handle;

/* TX QUEUE buffer */
static TASK_COMM task_comm[QT_Q_MAX_INFO_NUM];

/**************************************************************************
*                           FUNCTION DECLARATION
***************************************************************************/
void quectel_sub1_task_entry(ULONG para);

/**************************************************************************
*                                 FUNCTION
***************************************************************************/
/*
@func
  uart_print
@brief
  Start a uart transmit action.
*/


/*
@func
  quectel_task_entry
@brief
  Entry function for task. 
*/
/*=========================================================================*/
int quectel_task_entry(void)
{
	int ret = -1;
	UINT status = 0;
    uint32 message_size;
	TASK_COMM qt_main_task_comm;

	qapi_Timer_Sleep(1, QAPI_TIMER_UNIT_SEC, true);

	/* uart 1 init */
	uart_init(&uart1_conf);

	/* start uart 1 receive */
	uart_recv(&uart1_conf);

	/* prompt task running */
	qt_uart_dbg(uart1_conf.hdlr,"[task_create] start task ~");

	/* create a new task */
	if(TX_SUCCESS != txm_module_object_allocate((VOID *)&qt_sub1_thread_handle, sizeof(TX_THREAD))) 
	{
		qt_uart_dbg(uart1_conf.hdlr,"[task_create] txm_module_object_allocate failed ~");
		IOT_INFO("[task_create] txm_module_object_allocate failed ~");
		return - 1;
	}

	/* create a new task : sub1 */
	ret = tx_thread_create(qt_sub1_thread_handle,
						   "Quectel Main Task Thread",
						   quectel_sub1_task_entry,
						   NULL,
						   qt_sub1_thread_stack,
						   QT_SUB1_THREAD_STACK_SIZE,
						   QT_SUB1_THREAD_PRIORITY,
						   QT_SUB1_THREAD_PRIORITY,
						   TX_NO_TIME_SLICE,
						   TX_AUTO_START
						   );
	      
	if(ret != TX_SUCCESS)
	{
		qt_uart_dbg(uart1_conf.hdlr,"[task_create] : Thread creation failed");
		IOT_INFO("[task_create] : Thread creation failed");
	}

	message_size = sizeof(TASK_COMM)/sizeof(uint32);

	/* create a new queue : q_task_comm */
	status = tx_queue_create(&tx_queue_handle,
							 "q_task_comm",
							 message_size,
							 task_comm,
							 QT_Q_MAX_INFO_NUM * sizeof(TASK_COMM)
							 );
	if (TX_SUCCESS != status)
	{
		qt_uart_dbg(uart1_conf.hdlr, "tx_queue_create failed with status %d", status);
	}
	else
	{
		qt_uart_dbg(uart1_conf.hdlr, "tx_queue_create ok with status %d", status);
	}

	while (1)
	{
		main_thread_run_couter ++;
		qt_uart_dbg(uart1_conf.hdlr,"[task_create] task run times : %d", main_thread_run_couter);
		IOT_INFO("[task_create] task run times : %d", main_thread_run_couter);

		qt_main_task_comm.f1 = qt_main_task_comm.f2 = main_thread_run_couter;
		qt_main_task_comm.f3 = qt_main_task_comm.f4 = main_thread_run_couter;
		qt_main_task_comm.f5 = qt_main_task_comm.f6 = main_thread_run_couter;
		qt_main_task_comm.f7 = qt_main_task_comm.f8 = main_thread_run_couter;

		/* send data to sub1 task by queue */
		status = tx_queue_send(&tx_queue_handle, &qt_main_task_comm, 10);
		if (TX_SUCCESS != status)
		{
			qt_uart_dbg(uart1_conf.hdlr, "[task_create] tx_queue_send failed with status %d", status);
		}
		else
		{
		  //qt_uart_dbg(uart1_conf.hdlr, "tx_queue_send ok with status %d", status);
		}

		/* sleep 2 seconds */
		qapi_Timer_Sleep(5, QAPI_TIMER_UNIT_SEC, true);
	}

	return 0;
}

/*
@func
  quectel_sub1_task_entry
@brief
  A sub task will create by Quectel task entry. 
*/
void quectel_sub1_task_entry(ULONG para)
{
	UINT status = 0;
	TASK_COMM qt_sub1_task_comm;

	qt_uart_dbg(uart1_conf.hdlr,"[task_create] start sub1 task ~");

	while (1)
	{
		memset(&qt_sub1_task_comm, 0, sizeof(qt_sub1_task_comm));

		/* rec data from main task by queue */
		status = tx_queue_receive(&tx_queue_handle, &qt_sub1_task_comm, TX_WAIT_FOREVER);

		if(TX_SUCCESS != status)
		{
			qt_uart_dbg(uart1_conf.hdlr, "[task_create] tx_queue_receive failed with status %d", status);
			sub1_thread_run_couter = 0;
		}
		else
		{
		  //qt_uart_dbg(uart1_conf.hdlr, "tx_queue_receive ok with status %d", status);
			sub1_thread_run_couter = qt_sub1_task_comm.f1;
		}

		qt_uart_dbg(uart1_conf.hdlr,"[task_create] sub1 task run times : %d", sub1_thread_run_couter);
	}
}

#endif /*__EXAMPLE_TASK_CREATE__*/


