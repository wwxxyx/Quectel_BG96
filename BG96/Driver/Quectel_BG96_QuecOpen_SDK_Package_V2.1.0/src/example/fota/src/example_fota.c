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
#if defined(__EXAMPLE_FOTA__)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdarg.h"
#include "qapi_fs_types.h"
#include "qapi_uart.h"
#include "qapi_timer.h"
#include "qapi_diag.h"

#include "quectel_utils.h"
#include "quectel_uart_apis.h"
#include "txm_module.h"
#include "qapi_quectel.h"
#include "example_fota.h"

/**************************************************************************
*                                 GLOBAL
***************************************************************************/

static char tx_buff[1024];
static char rx_buff[1024];
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

static void fota_uart_rx_cb(uint32_t num_bytes, void *cb_data);
void fota_uart_init(QT_UART_CONF_PARA *uart_conf)
{
	qapi_Status_t status;
	qapi_UART_Open_Config_t uart_cfg;
	QAPI_Flow_Control_Type uart_fc_type = QAPI_FCTL_OFF_E;

	uart_cfg.baud_Rate			= uart_conf->baudrate;
	uart_cfg.enable_Flow_Ctrl	= QAPI_FCTL_OFF_E;
	uart_cfg.bits_Per_Char		= QAPI_UART_8_BITS_PER_CHAR_E;
	uart_cfg.enable_Loopback 	= 0;
	uart_cfg.num_Stop_Bits		= QAPI_UART_1_0_STOP_BITS_E;
	uart_cfg.parity_Mode 		= QAPI_UART_NO_PARITY_E;
	uart_cfg.rx_CB_ISR			= (qapi_UART_Callback_Fn_t)&fota_uart_rx_cb;
	uart_cfg.tx_CB_ISR			= NULL;

	status = qapi_UART_Open(&uart_conf->hdlr, uart_conf->port_id, &uart_cfg);
	IOT_DEBUG("QT# qapi_UART_Open [%d] status %d", uart_conf->port_id, status);

	status = qapi_UART_Power_On(uart_conf->hdlr);
	IOT_DEBUG("QT# qapi_UART_Power_On [%d] status %d", uart_conf->port_id, status);

	status = qapi_UART_Ioctl(uart_conf->hdlr, QAPI_SET_FLOW_CTRL_E, &uart_fc_type);
	IOT_DEBUG("QT# qapi_UART_Ioctl [%d] status %d", uart_conf->port_id, status);	
}


/*
@func
  uart_recv
@brief
  Start a uart receive action.
*/
void fota_uart_recv(QT_UART_CONF_PARA *uart_conf)
{
	qapi_Status_t status;
	status = qapi_UART_Receive(uart_conf->hdlr, uart_conf->rx_buff, uart_conf->rx_len, (void*)uart_conf);
	IOT_DEBUG("QT# qapi_UART_Receive [%d] status %d", (qapi_UART_Port_Id_e)uart_conf->port_id, status);
}



void fota_uart_dbg(qapi_UART_Handle_t uart_hdlr, const char* fmt, ...)
{
	char log_buf[256] = {0};

	va_list ap;
	va_start(ap, fmt);
	vsnprintf(log_buf, sizeof(log_buf), fmt, ap);
	va_end( ap );

    qapi_UART_Transmit(uart_hdlr, log_buf, strlen(log_buf), NULL);
    qapi_UART_Transmit(uart_hdlr, "\r\n", strlen("\r\n"), NULL);
    qapi_Timer_Sleep(50, QAPI_TIMER_UNIT_MSEC, false);
}

/*
@func
  quectel_fota_update_resp_cb
@brief
  This callback will come when fota upgrade failed or success.
*/
void quectel_fota_update_resp_cb(short int error_id)
{
    if(error_id == 0)
    {
        qapi_QT_Remove_Fota_Package();
        qapi_QT_Reset_Device(0); //restart
    }
    else
    {
        fota_uart_dbg(uart1_conf.hdlr,"fota update failed:%d",error_id);  
    }
}

/*
@func
  quectel_upgrade_progress_cb
@brief
  This callback will be come every one percent.
*/
void quectel_upgrade_progress_cb(uint8 phase, uint8 percent)
{
    fota_uart_dbg(uart1_conf.hdlr,"phase:%u, percent:%u",phase,percent);  
}

/*
@func
  quectel_fota_http_dl_cb
@brief
  This callback will come when download failed or success.
*/
void quectel_fota_http_dl_cb(int error_id)
{
    qapi_Status_t status = 0;
    if(error_id == 0)
    {
        fota_uart_dbg(uart1_conf.hdlr,"fota download success");
        status = qapi_QT_Fota_Update_Start(quectel_fota_update_resp_cb, quectel_upgrade_progress_cb);
        if(status != QAPI_QT_ERR_OK)
        {
            IOT_DEBUG("qapi_QT_Fota_Update_Startfialed:%d",status); 
        }
    }
    else
    {
        fota_uart_dbg(uart1_conf.hdlr,"htpp download failed:%d",error_id); 
    }
    
}

/*
@func
  uart_rx_cb
@brief
  uart rx callback handler.
*/
static void fota_uart_rx_cb(uint32_t num_bytes, void *cb_data)
{
    qapi_Status_t status = 0;
	QT_UART_CONF_PARA *uart_conf = (QT_UART_CONF_PARA*)cb_data;
	if(num_bytes == 0)
	{
		fota_uart_recv(uart_conf);
		return;
	}
	else if(num_bytes >= uart_conf->rx_len)
	{
		num_bytes = uart_conf->rx_len;
	}
    if(strncmp(uart_conf->rx_buff, "FOTAURL=HTTP", strlen("FOTAURL=HTTP")) == 0) // fota upgrade by http(s). for example:FOTAURL=HTTP://220.180.239.212:8005/BG96_112A_119.zip.
    {
        fota_uart_dbg(uart1_conf.hdlr,"Fota upgrade by HTTP(S),URL:%s",uart_conf->rx_buff+8);
        status = qapi_QT_Fota_Http_Download_Start(uart_conf->rx_buff+8, quectel_fota_http_dl_cb);
        if(status != QAPI_QT_ERR_OK)
        {
             IOT_DEBUG("qapi_QT_Fota_Http_Download_Start failed:%d", status);  
        }
    }
    /*
    fota upgrade by local. for example:FOTAURL=LCOAL. Before sending this command, the delta firmware package should be uploded to the efs first.
    The storage path for delta firmware package is /fota/update.zip. Both the path and the package name cannot be modified. 
    Customers can use the QEFS Explorer tool provided by Quectel to upload the upgrade package
    */
    else if(strncmp(uart_conf->rx_buff, "FOTAURL=LOCAL",strlen("FOTAURL=LOCAL")) == 0)
    {
        fota_uart_dbg(uart1_conf.hdlr, "Fota upgrade by local");
        status = qapi_QT_Fota_Update_Start(quectel_fota_update_resp_cb, quectel_upgrade_progress_cb);
        if(status != QAPI_QT_ERR_OK)
        {
            IOT_DEBUG("qapi_QT_Fota_Update_Start fialed:%d",status); 
        }
    }
    else
    {
        fota_uart_dbg(uart1_conf.hdlr, "Input command error");
    }
 	memset(uart_conf->rx_buff, 0, uart_conf->rx_len);
	fota_uart_recv(uart_conf);
}
/*
@func
  quectel_task_entry
@brief
  Entry function for task. 
*/
int quectel_task_entry(void)
{
	fota_uart_init(&uart1_conf);
    fota_uart_recv(&uart1_conf);
    fota_uart_dbg(uart1_conf.hdlr, "Fota example start up");
	while(1)
	{
        qapi_Timer_Sleep(5, QAPI_TIMER_UNIT_SEC, true);
	}
	return 0;
}

#endif /*__EXAMPLE_FOTA__*/

