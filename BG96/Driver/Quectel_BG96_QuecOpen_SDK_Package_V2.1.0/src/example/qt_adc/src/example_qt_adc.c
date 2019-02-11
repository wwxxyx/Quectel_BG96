/******************************************************************************
*@file    quectel_adc.c
*@brief   Use the API(qapi_QT_ADC_Read) to read the values of ADC0 and ADC1
*
*  ---------------------------------------------------------------------------
*
*  Copyright (c) 2018 Quectel Technologies, Inc.
*  All Rights Reserved.
*  Confidential and Proprietary - Quectel Technologies, Inc.
*  ---------------------------------------------------------------------------
*******************************************************************************/
#if defined(__EXAMPLE_QT_ADC__)
/*===========================================================================
						   Header file
===========================================================================*/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

#include "qapi_fs_types.h"
#include "qapi_status.h"
#include "qapi_dss.h"
#include "qapi_adc.h"

#include "qapi_uart.h"
#include "qapi_timer.h"
#include "qapi_diag.h"
#include "quectel_utils.h"
#include "quectel_uart_apis.h"
#include "example_qt_adc.h"
#include "qapi_quectel.h"

/**************************************************************************
*								  GLOBAL
***************************************************************************/
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

/*ADC handle*/
qapi_ADC_Handle_t adc_handle;


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
    const char *Channel_Name_ADC0 = ADC_INPUT_ADC0;
    const char *Channel_Name_ADC1 = ADC_INPUT_ADC1;

    qapi_ADC_Read_Result_t result;
	qapi_Status_t status = QAPI_ERROR;

	/* wait 10sec for device startup */
	qapi_Timer_Sleep(10, QAPI_TIMER_UNIT_SEC, true);
	
	/* uart 1 init */
	uart_init(&uart1_conf);
	/* start uart 1 receive */
	uart_recv(&uart1_conf);
	/* prompt task running */
	qt_uart_dbg(uart1_conf.hdlr,"[uart 1] QT ADC Example");

    while(1)
    {
        /*read channel ADC0*/
        memset(&result, 0, sizeof(result));
        status = qapi_QT_ADC_Read(Channel_Name_ADC0, &result);
        if (QAPI_OK == status)
        {
            if(ADC_RESULT_VALID == result.eStatus)
            {  
                qt_uart_dbg(uart1_conf.hdlr,"Input %s Voltage: %d mV\n",
                            Channel_Name_ADC0,
                            result.nMicrovolts/1000);
            }
        }

        /*read channel ADC1*/
        memset(&result, 0, sizeof(result));
        status = qapi_QT_ADC_Read(Channel_Name_ADC1, &result);
        if (QAPI_OK == status)
        {
            if(ADC_RESULT_VALID == result.eStatus)
            {  
                qt_uart_dbg(uart1_conf.hdlr,"Input %s Voltage: %d mV\n",
                            Channel_Name_ADC1,
                            result.nMicrovolts/1000);
            }
        }

	    /* wait 5sec for reading again */
	    qapi_Timer_Sleep(5, QAPI_TIMER_UNIT_SEC, true);
    }

    return 0;
}
#endif/*end of __EXAMPLE_QT_ADC__*/

