/******************************************************************************
*@file    example_device_info.c
*@brief   example of get device information
*
*  ---------------------------------------------------------------------------
*
*  Copyright (c) 2018 Quectel Technologies, Inc.
*  All Rights Reserved.
*  Confidential and Proprietary - Quectel Technologies, Inc.
*  ---------------------------------------------------------------------------
*******************************************************************************/
#if defined(__EXAMPLE_DEVICE_INFO__)
#include <stdbool.h>
#include "qapi_uart.h"
#include "qapi_timer.h"
#include "qapi_diag.h"
#include "qapi_device_info.h"
#include "quectel_utils.h"
#include "quectel_uart_apis.h"
#include "example_device_info.h"

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

/* conter used to count the total run times*/
static unsigned long task_run_couter = 0;

/* device info */
qapi_Device_Info_t dev_info;

/**************************************************************************
*                           FUNCTION DECLARATION
***************************************************************************/

/**************************************************************************
*                                 FUNCTION
***************************************************************************/
/*
@func
  qt_device_info_data_output
@brief
  Output the device information data
*/
void qt_device_info_data_output(qapi_Device_Info_ID_t dev_info_id)
{
	switch(dev_info.info_type)
	{
		case QAPI_DEVICE_INFO_TYPE_BOOLEAN_E:
			qt_uart_dbg(uart1_conf.hdlr, "~[b][%d][%d]", dev_info_id, dev_info.u.valuebool);
		break;

		case QAPI_DEVICE_INFO_TYPE_INTEGER_E:
			qt_uart_dbg(uart1_conf.hdlr, "~[i][%d][%d]", dev_info_id, dev_info.u.valueint);
		break;

		case QAPI_DEVICE_INFO_TYPE_BUFFER_E:
			qt_uart_dbg(uart1_conf.hdlr, "~[s][%d][%s]", dev_info_id, dev_info.u.valuebuf.buf);
		break;

		default:
			qt_uart_dbg(uart1_conf.hdlr, "~[Invalid][%d]", dev_info_id);
		break;
	}
}

/*
@func
  qt_get_defice_info_item
@brief
  get the device information data
*/
void qt_get_defice_info_item(qapi_Device_Info_ID_t dev_info_id)
{
	qapi_Status_t status = 0;

	memset(&dev_info, 0, sizeof(dev_info));
	status = qapi_Device_Info_Get(dev_info_id, &dev_info);
	if(status != QAPI_OK)
	{
		qt_uart_dbg(uart1_conf.hdlr, "! Get %d Fail, status %d", dev_info_id, status);
	}
	else
	{
		qt_device_info_data_output(dev_info_id);
	}
}

/*
@func
  qt_get_defice_info_all
@brief
  get the device ALL information data
*/
void qt_get_defice_info_all(void)
{
	qapi_Device_Info_ID_t dev_info_id;

	for(dev_info_id = QAPI_DEVICE_INFO_BUILD_ID_E;
		dev_info_id <= QAPI_DEVICE_INFO_CHIPID_STRING_E;
		dev_info_id ++)
	{
		qt_get_defice_info_item(dev_info_id);
		qapi_Timer_Sleep(2000, QAPI_TIMER_UNIT_MSEC, true);
	}
}

/*
@func
  qt_init_device_info
@brief
  initial the device informaiton
*/
void qt_init_device_info(void)
{
	qapi_Status_t status = 0;

	status = qapi_Device_Info_Init();
	if(status != QAPI_OK)
	{
		qt_uart_dbg(uart1_conf.hdlr, "~ qapi_Device_Info_Init fail [%d]", status);
	}
	else
	{
		qt_uart_dbg(uart1_conf.hdlr, "~ qapi_Device_Info_Init OK [%d]", status);
	}
}

/*
@func
  qt_qapi_callback
@brief
  device info callback
*/
void qt_qapi_callback(const qapi_Device_Info_t *dev_info)
{
	qt_uart_dbg(uart1_conf.hdlr, "~ qt_qapi_callback OK [%d]", dev_info->id);

	switch(dev_info->info_type)
	{
		case QAPI_DEVICE_INFO_TYPE_BOOLEAN_E:
			qt_uart_dbg(uart1_conf.hdlr, "~[b][%d][%d]", dev_info->id, dev_info->u.valuebool);
		break;

		case QAPI_DEVICE_INFO_TYPE_INTEGER_E:
			qt_uart_dbg(uart1_conf.hdlr, "~[i][%d][%d]", dev_info->id, dev_info->u.valueint);
		break;

		case QAPI_DEVICE_INFO_TYPE_BUFFER_E:
			qt_uart_dbg(uart1_conf.hdlr, "~[s][%d][%s]", dev_info->id, dev_info->u.valuebuf.buf);
		break;

		default:
			qt_uart_dbg(uart1_conf.hdlr, "~[Invalid][%d]", dev_info->id);
		break;
	}
}

/*
@func
  quectel_task_entry
@brief
  Entry function for task. 
*/
/*=========================================================================*/
int quectel_task_entry(void)
{
	qapi_Status_t status = QAPI_OK;

	qapi_Timer_Sleep(1, QAPI_TIMER_UNIT_SEC, true);
	IOT_INFO("QT# quectel_task_entry[device_info] ~~~");

	/* uart 1 init */
	uart_init(&uart1_conf);

	/* start uart 1 receive */
	uart_recv(&uart1_conf);

	/* print log to app uart 1 */
    qt_uart_dbg(uart1_conf.hdlr,"quectel_task_entry[device_info] running...");

	qt_init_device_info();

	status = qapi_Device_Info_Set_Callback(QAPI_DEVICE_INFO_NETWORK_IND_E, qt_qapi_callback);
	qt_uart_dbg(uart1_conf.hdlr,"[dev_info] qapi_Device_Info_Set_Callback, status %d", status);

	while (1)
	{
		qt_get_defice_info_all();

		task_run_couter ++;

	    qt_uart_dbg(uart1_conf.hdlr,"[dev_info] task run times : %d", task_run_couter);

		/* sleep 2 seconds */
		qapi_Timer_Sleep(5, QAPI_TIMER_UNIT_SEC, true);
	}
}

#endif /*__EXAMPLE_DEVICE_INFO__*/

