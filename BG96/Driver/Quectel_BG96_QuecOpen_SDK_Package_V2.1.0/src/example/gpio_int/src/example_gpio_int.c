/******************************************************************************
*@file    example_gpio_int.c
*@brief   example for gpio interrupt operation
*
*  ---------------------------------------------------------------------------
*
*  Copyright (c) 2018 Quectel Technologies, Inc.
*  All Rights Reserved.
*  Confidential and Proprietary - Quectel Technologies, Inc.
*  ---------------------------------------------------------------------------
*******************************************************************************/
#if defined(__EXAMPLE_GPIO_INT__)
#include "qapi_tlmm.h"
#include "qapi_timer.h"
#include "qapi_uart.h"
#include "qapi_diag.h"
#include "qapi_gpioint.h"
#include "quectel_utils.h"
#include "quectel_uart_apis.h"
#include "example_gpio_int.h"
#include "quectel_gpio.h"

/**************************************************************************
*                                 DEFINE
***************************************************************************/

/**************************************************************************
*                                 GLOBAL
***************************************************************************/
/*  !!! This Pin Enumeration Only Applicable BG96-OPEN Project !!!
 */
static GPIO_MAP_TBL gpio_map_tbl[PIN_E_GPIO_MAX] = {
/* PIN NUM,     PIN NAME,    GPIO ID  GPIO FUNC */
	{  4, 		"GPIO01",  		23, 	 0},
	{  5, 		"GPIO02",  		20, 	 0},
	{  6, 		"GPIO03",  		21, 	 0},
	{  7, 		"GPIO04",  		22, 	 0},
	{ 18, 		"GPIO05",  		11, 	 0},
	{ 19, 		"GPIO06",  		10, 	 0},
	{ 22, 		"GPIO07",  		 9, 	 0},
	{ 23, 		"GPIO08",  	 	 8, 	 0},
	{ 26, 		"GPIO09",  		15, 	 0},
	{ 27, 		"GPIO10",  		12, 	 0},
	{ 28, 		"GPIO11",  		13, 	 0},
	{ 40, 		"GPIO19",  		19, 	 0},
	{ 41, 		"GPIO20",  		18, 	 0},
	{ 64, 		"GPIO21",  		07, 	 0},
};

/* uart1 rx tx buffer */
static char rx1_buff[1024];
static char tx1_buff[1024];

/* uart config para*/
static QT_UART_CONF_PARA uart1_conf =
{
	NULL,
	QT_UART_PORT_02,
	tx1_buff,
	sizeof(tx1_buff),
	rx1_buff,
	sizeof(rx1_buff),
	115200
};

static MODULE_PIN_ENUM test_pin = PIN_E_GPIO_03;
static qapi_Instance_Handle_t pGpioIntHdlr;
static int gpio_int_counter = 0;

/**************************************************************************
*                           FUNCTION DECLARATION
***************************************************************************/


/**************************************************************************
*                                 FUNCTION
***************************************************************************/
void pfnCallback(qapi_GPIOINT_Callback_Data_t data)
{
	//could not print log in this callback
	gpio_int_counter++;
}

/*
@func
  quectel_task_demo_entry
@brief
  Entry function for task. 
*/
/*=========================================================================*/
int quectel_task_entry(void)
{
	int cnt = 0;
	qapi_Status_t status;
	qbool_t is_pending = 0;

	/* UART 1 init */
	uart_init(&uart1_conf);

	/* prompt, task running */
	qt_uart_dbg(uart1_conf.hdlr,"[GPIO INT] start task ~");

	status = qapi_GPIOINT_Register_Interrupt(&pGpioIntHdlr,
											 gpio_map_tbl[test_pin].gpio_id,
											 pfnCallback,
											 NULL,
											 QAPI_GPIOINT_TRIGGER_LEVEL_HIGH_E,
											 QAPI_GPIOINT_PRIO_LOWEST_E,
											 false);
	qt_uart_dbg(uart1_conf.hdlr,"[GPIO INT] status[%x]", status);

	status = qapi_GPIOINT_Is_Interrupt_Pending(&pGpioIntHdlr, gpio_map_tbl[test_pin].gpio_id, &is_pending);
	qt_uart_dbg(uart1_conf.hdlr,"[GPIO INT] status[%x] is_pending[%x]", status, is_pending);

	while(1)
	{
		qt_uart_dbg(uart1_conf.hdlr,"[GPIO INT][loop %d]", ++cnt & 0xffffffff);
		qt_uart_dbg(uart1_conf.hdlr,"[GPIO INT][int %d]", gpio_int_counter & 0xffffffff);
		qapi_Timer_Sleep(1, QAPI_TIMER_UNIT_SEC, true);
	}

	return 0;
}

#endif /*__EXAMPLE_GPIO_INT__*/

