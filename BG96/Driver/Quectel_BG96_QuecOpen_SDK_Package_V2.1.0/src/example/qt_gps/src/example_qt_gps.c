/******************************************************************************
*@file    example_qt_gps.c
*@brief   example of gps operation (Quectel QAPIs for GNSS)
*
*  ---------------------------------------------------------------------------
*
*  Copyright (c) 2018 Quectel Technologies, Inc.
*  All Rights Reserved.
*  Confidential and Proprietary - Quectel Technologies, Inc.
*  ---------------------------------------------------------------------------
*******************************************************************************/
#if defined(__EXAMPLE_QT_GPS__)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdarg.h"
#include "qapi_fs_types.h"
#include "qapi_uart.h"
#include "qapi_timer.h"
#include "qapi_diag.h"

#include "quectel_utils.h"
#include "qapi_location.h"
#include "txm_module.h"
#include "quectel_uart_apis.h"
#include "qapi_quectel.h"
#include "example_qt_gps.h"

/**************************************************************************
*                                 GLOBAL
***************************************************************************/

static char tx_buff[1024];
QT_UART_CONF_PARA uart1_conf =
{
	NULL,
	QT_UART_PORT_02,
	tx_buff,
	sizeof(tx_buff),
	NULL,
	NULL,
	115200
};

/*
@func
  quectel_loc_nmea_cb
@brief
  Callback of Location to get NMEA sentence.
*/
void quectel_loc_nmea_cb(char *nmea_data)
{
    qapi_UART_Transmit(uart1_conf.hdlr, nmea_data, strlen(nmea_data), NULL);
    qapi_Timer_Sleep(50, QAPI_TIMER_UNIT_MSEC, true);
}

/*
@func
  quectel_task_entry
@brief
  Entry function for task. 
*/
int quectel_task_entry(void)
{
	qapi_Status_t status = 0;

	uart_init(&uart1_conf);
    qt_uart_dbg(uart1_conf.hdlr,"Example for GPS by Quectel QAPIs");  
	
	status = qapi_QT_Loc_Start(QT_LOC_EVENT_MASK_NMEA, quectel_loc_nmea_cb);
	qt_uart_dbg(uart1_conf.hdlr,"START status %d", status);

	qapi_Timer_Sleep(120, QAPI_TIMER_UNIT_SEC, true);
	status = qapi_QT_Loc_Stop();
	qt_uart_dbg(uart1_conf.hdlr,"STOP status %d", status);

	qapi_Timer_Sleep(20, QAPI_TIMER_UNIT_SEC, true);	
	status = qapi_QT_Loc_Start(QT_LOC_EVENT_MASK_NMEA, quectel_loc_nmea_cb);
	qt_uart_dbg(uart1_conf.hdlr,"START status %d", status);

	qapi_Timer_Sleep(120, QAPI_TIMER_UNIT_SEC, true);
	status = qapi_QT_Loc_Stop();
	qt_uart_dbg(uart1_conf.hdlr,"STOP status %d", status);

	return 0;
}

#endif /*__EXAMPLE_QT_GPS__*/

