/******************************************************************************
*@file    example_atfwd.c
*@brief   example of AT forward service
*
*  ---------------------------------------------------------------------------
*
*  Copyright (c) 2018 Quectel Technologies, Inc.
*  All Rights Reserved.
*  Confidential and Proprietary - Quectel Technologies, Inc.
*  ---------------------------------------------------------------------------
*******************************************************************************/
#if defined(__EXAMPLE_ATFWD__)
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
#include "qapi_atfwd.h"

#include "qapi_uart.h"
#include "qapi_timer.h"
#include "quectel_utils.h"
#include "quectel_uart_apis.h"
#include "example_atfwd.h"

/**************************************************************************
*								  GLOBAL
***************************************************************************/
/* uart rx tx buffer */
static char rx_buff[1024];
static char tx_buff[1024];

/* uart config para*/
static QT_UART_CONF_PARA uart_conf =
{
	NULL,
	QT_UART_PORT_02,
	tx_buff,
	sizeof(tx_buff),
	rx_buff,
	sizeof(rx_buff),
	115200
};

static int qexample_val = 0;

/**************************************************************************
*                                 FUNCTION
***************************************************************************/
static int qt_atoi(char *str)
{
    int res = 0, i;

    for (i = 0; str[i] != '\0' && str[i] != '.'; ++i)
    {
        res = res*10 + (str[i] - '0');
    }

    return res;
}
static int strncasecmp(const char * s1, const char * s2, size_t n)
{
  unsigned char c1, c2;
  int diff;

  if (n > 0)
  {
    do
    {
      c1 = (unsigned char)(*s1++);
      c2 = (unsigned char)(*s2++);
      if (('A' <= c1) && ('Z' >= c1))
      {
        c1 = c1 - 'A' + 'a';
      }
      if (('A' <= c2) && ('Z' >= c2))
      {
        c2 = c2 - 'A' + 'a';
      }
      diff = c1 - c2;

      if (0 != diff)
      {
        return diff;
      }

      if ('\0' == c1)
      {
        break;
      }
    } while (--n);
  }
  return 0;
}

void atfwd_cmd_handler_cb(boolean is_reg, char *atcmd_name,
                                 uint8* at_fwd_params, uint8 mask,
                                 uint32 at_handle)
{
    char buff[32] = {0};
    int  tmp_val = 0;
    qapi_Status_t ret = QAPI_ERROR;
    
    qt_uart_dbg(uart_conf.hdlr,"atfwd_cmd_handler_cb is called, atcmd_name:[%s] mask:[%d]\n", atcmd_name, mask);

    if( !strncasecmp(atcmd_name, "+QEXAMPLE",strlen(atcmd_name)) )
    {
        //Execute Mode
        if ((QUEC_AT_MASK_NA_V01) == mask)//AT+QEXAMPLE
        {
            ret = qapi_atfwd_send_resp(atcmd_name, "", QUEC_AT_RESULT_OK_V01);
        }
        //Read Mode
        else if ((QUEC_AT_MASK_NA_V01 | QUEC_AT_MASK_QU_V01) == mask)//AT+QEXAMPLE?
        {
            snprintf(buff, sizeof(buff), "+QEXAMPLE: %d", qexample_val);
            ret = qapi_atfwd_send_resp(atcmd_name, buff, QUEC_AT_RESULT_OK_V01);
        }
        //Test Mode
        else if ((QUEC_AT_MASK_NA_V01 | QUEC_AT_MASK_EQ_V01 | QUEC_AT_MASK_QU_V01) == mask)//AT+QEXAMPLE=?
        {
            snprintf(buff, sizeof(buff), "+QEXAMPLE: (0-2)");
            ret = qapi_atfwd_send_resp(atcmd_name, buff, QUEC_AT_RESULT_OK_V01);
        }
        //Write Mode
        else if ((QUEC_AT_MASK_NA_V01 | QUEC_AT_MASK_EQ_V01 | QUEC_AT_MASK_AR_V01) == mask)//AT+QEXAMPLE=<value>
        {
            tmp_val = qt_atoi((char*)at_fwd_params);
            if(tmp_val >= 0 && tmp_val <= 2)
            {
                qexample_val = tmp_val;
                ret = qapi_atfwd_send_resp(atcmd_name, "", QUEC_AT_RESULT_OK_V01);
            }
            else
            {
                ret = qapi_atfwd_send_resp(atcmd_name, "", QUEC_AT_RESULT_ERROR_V01);
            }  
        }
    }
    else
    {
        ret = qapi_atfwd_send_resp(atcmd_name, "", QUEC_AT_RESULT_ERROR_V01);
    }

    qt_uart_dbg(uart_conf.hdlr,"[%s] send resp, ret = %d\n", atcmd_name, ret);
}

/*
@func
	quectel_task_entry
@brief
	Entry function for task. 
*/
int quectel_task_entry(void)
{
	qapi_Status_t retval = QAPI_ERROR;

	/* wait 5sec for device startup */
	qapi_Timer_Sleep(5, QAPI_TIMER_UNIT_SEC, true);

	/* uart init */
	uart_init(&uart_conf);
	/* start uart receive */
	uart_recv(&uart_conf);
	/* prompt task running */
    
	qt_uart_dbg(uart_conf.hdlr,"ATFWD Example entry...\n");
        
    retval = qapi_atfwd_reg("+QEXAMPLE", atfwd_cmd_handler_cb);
    if(retval != QAPI_OK)
    {
        qt_uart_dbg(uart_conf.hdlr,"ATFWD register [%s] faild! ret = %d\n", "+QEXAMPLE", retval);
    }
    else
    {
        qt_uart_dbg(uart_conf.hdlr,"ATFWD register [%s] complete!\n", "+QEXAMPLE");
    }
    
    while(1)
    {
	    /* wait 5sec */
	    qapi_Timer_Sleep(5, QAPI_TIMER_UNIT_SEC, true);
    }

    return 0;
}

#endif/*end of __EXAMPLE_ATFWD__*/


