/******************************************************************************

                        Q U E C T E L _ U A R T _ A P I S . C

******************************************************************************/
/******************************************************************************

  @file    quectel_uart_apis.c
  @brief   quectel common interface file

******************************************************************************/
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include "string.h"

#include "qapi_uart.h"
#include "qapi_timer.h"
#include "qapi_diag.h"
#include "quectel_uart_apis.h"
#include "quectel_utils.h"

/**************************************************************************
*                                 GLOBAL
***************************************************************************/

/**************************************************************************
*                           FUNCTION DECLARATION
***************************************************************************/
static void uart_rx_cb(uint32_t num_bytes, void *cb_data);

/**************************************************************************
*                                 FUNCTION
***************************************************************************/
/*
@func
  uart_init
@brief
  [in] 	uart_handler
       	The handler which used to handle associated uart.
  [in] 	uart_port_id
       	The uart port id which customer want to use.
       	In QUECTEL_QUECOPEN_V1--->
		  QAPI_UART_PORT_001_E	:  Map to Chip's UART3, Module's Pin 34(RXD) & Pin 35(TXD)
		  QAPI_UART_PORT_002_E	:  Map to Chip's UART5, Module's Pin 22(RXD) & Pin 23(TXD)
		  QAPI_UART_PORT_003_E	:  Map to Chip's UART1, Module's Pin 28(RXD) & Pin 27(TXD)
		In QUECTEL_QUECOPEN_V2--->
		  QAPI_UART_PORT_001_E	:  Map to Chip's UART6, Module's Pin  6(RXD) & Pin	5(TXD)
		  QAPI_UART_PORT_002_E	:  Map to Chip's UART5, Module's Pin 22(RXD) & Pin 23(TXD)
		  QAPI_UART_PORT_003_E	:  Map to Chip's UART1, Module's Pin 28(RXD) & Pin 27(TXD)
  [in] 	baud_rate
       	The uart port communication baud_rate.  
*/
void uart_init(QT_UART_CONF_PARA *uart_conf)
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
	uart_cfg.rx_CB_ISR			= (qapi_UART_Callback_Fn_t)&uart_rx_cb;
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
  uart_print
@brief
  Start a uart transmit action.
*/
void uart_print(QT_UART_CONF_PARA *uart_conf, char *buff, uint16_t len)
{
	qapi_Status_t status;
	status = qapi_UART_Transmit(uart_conf->hdlr, buff, len, NULL);
	IOT_DEBUG("QT# qapi_UART_Transmit [%x] status %d", &uart_conf->port_id, status);
}

/*
@func
  uart_recv
@brief
  Start a uart receive action.
*/
void uart_recv(QT_UART_CONF_PARA *uart_conf)
{
	qapi_Status_t status;
	status = qapi_UART_Receive(uart_conf->hdlr, uart_conf->rx_buff, uart_conf->rx_len, (void*)uart_conf);
	IOT_DEBUG("QT# qapi_UART_Receive [%d] status %d", (qapi_UART_Port_Id_e)uart_conf->port_id, status);
}

/*
@func
  uart_rx_cb
@brief
  uart rx callback handler.
*/
static void uart_rx_cb(uint32_t num_bytes, void *cb_data)
{
	QT_UART_CONF_PARA *uart_conf = (QT_UART_CONF_PARA*)cb_data;

	if(num_bytes == 0)
	{
		uart_recv(uart_conf);
		return;
	}
	else if(num_bytes >= uart_conf->rx_len)
	{
		num_bytes = uart_conf->rx_len;
	}

#ifdef QT_UART_ECHO_ENABLE
	uart_print(uart_conf, uart_conf->rx_buff, num_bytes);
#endif
	IOT_DEBUG("QT# uart_rx_cb data [%d][%s]", num_bytes, uart_conf->rx_buff);
//	memset(uart_conf->rx_buff, 0, uart_conf->rx_len);
	uart_recv(uart_conf);
}

/*
@func
  qt_uart_dbg
@brief
  Output the debug log. 
*/
void qt_uart_dbg(qapi_UART_Handle_t uart_hdlr, const char* fmt, ...)
{
	char log_buf[256] = {0};

	va_list ap;
	va_start(ap, fmt);
	vsnprintf(log_buf, sizeof(log_buf), fmt, ap);
	va_end( ap );

    qapi_UART_Transmit(uart_hdlr, log_buf, strlen(log_buf), NULL);
    qapi_UART_Transmit(uart_hdlr, "\r\n", strlen("\r\n"), NULL);
    qapi_Timer_Sleep(50, QAPI_TIMER_UNIT_MSEC, true);
}

