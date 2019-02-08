/******************************************************************************
*@file    example_atc_sms.c
*@brief   example of atc sms for complete operation
*
*  ---------------------------------------------------------------------------
*
*  Copyright (c) 2018 Quectel Technologies, Inc.
*  All Rights Reserved.
*  Confidential and Proprietary - Quectel Technologies, Inc.
*  ---------------------------------------------------------------------------
*******************************************************************************/
#if defined(__EXAMPLE_ATC_SMS__)
#include "txm_module.h"
#include "qapi_diag.h"
#include "qapi_timer.h"
#include "qapi_uart.h"
#include "qapi_quectel.h"
#include "quectel_utils.h"
#include "quectel_uart_apis.h"
#include "example_atc_sms.h"

/**************************************************************************
*                                 DEFINE
***************************************************************************/


/**************************************************************************
*                           FUNCTION DECLARATION
***************************************************************************/
static int my_atoi(char *str);

/**************************************************************************
*                                 GLOBAL
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

static qapi_at_stream_id_t stream_id;
static char at_cmd_rsp[3*1024];
static int sms_rec_index = 0;
static int sms_send_status = 0;
static char test_phone_number[] = "15256085937";	//test phone number

static TX_EVENT_FLAGS_GROUP atc_pipe_sig_handle;
static NW_STATUS_E nw_status;
static AT_EXE_E at_exe_status = AT_EXE_OK;

/*===========================================================================

                           Static & global Variable Declarations

===========================================================================*/
/*
@func
	callback_atc_pipe
@brief
	Callback for the ATC Pipe.
*/
static void callback_atc_pipe(qapi_at_pipe_data_t *data)
{
	ULONG sig_event = 0;

	IOT_DEBUG("@@QT# callback_atc_pipe data # [%d] %s ", data->len, data->data);

/*** URC Handle*/
	/* SMS and other special ATC handle first */
	/* SMS handle */
	if(!strncmp("\r\n> ", data->data, 4))
	{
		if(sms_send_status == 1)
		{
			tx_event_flags_set(&atc_pipe_sig_handle, SIG_EVG_ATPIPE_SENDING_SMS, TX_OR);
		}
		return;
	}
	else if(!strncmp("\r\n+CMGS:", data->data, 8))
	{
		qt_uart_dbg(uart1_conf.hdlr, "[SMS Send] %s", data->data);
		tx_event_flags_set(&atc_pipe_sig_handle, SIG_EVG_ATPIPE_SENDED_SMS, TX_OR);
	}
	else if(!strncmp("\r\n+CMTI:", data->data, 8))
	{
		qt_uart_dbg(uart1_conf.hdlr, "[SMS Rec] %s", data->data);
		memset(at_cmd_rsp, 0, sizeof(at_cmd_rsp));
		strcat(at_cmd_rsp, data->data);
		tx_event_flags_set(&atc_pipe_sig_handle, SIG_EVG_ATPIPE_RECED_SMS, TX_OR);
	}
	else if(strstr(data->data,"\r\n+CME ERROR:") != NULL)
	{
		qt_uart_dbg(uart1_conf.hdlr, "ERROR URC");
	}

/*** Normal ATC response handle */
	tx_event_flags_get(&atc_pipe_sig_handle, SIG_EVG_ATPIPE_SENDING_E, TX_OR, &sig_event, TX_NO_WAIT);
	if(sig_event & SIG_EVG_ATPIPE_SENDING_E)
	{
		IOT_DEBUG("@@ exec memset");
		tx_event_flags_set(&atc_pipe_sig_handle, ~SIG_EVG_ATPIPE_SENDING_E, TX_AND);
		tx_event_flags_set(&atc_pipe_sig_handle,  SIG_EVG_ATPIPE_RECING_E, TX_OR);
		memset(at_cmd_rsp, 0, sizeof(at_cmd_rsp));
	}

	strcat(at_cmd_rsp, data->data);
	IOT_DEBUG("@@QT# at_cmd_rsp # %d, %s", strlen(at_cmd_rsp), at_cmd_rsp);

	if(strstr(data->data,"\r\nOK") != NULL)
	{
		at_exe_status = AT_EXE_OK;
		qt_uart_dbg(uart1_conf.hdlr, "AT RSP OK");
		tx_event_flags_set(&atc_pipe_sig_handle, ~SIG_EVG_ATPIPE_RECING_E, TX_AND);
		tx_event_flags_set(&atc_pipe_sig_handle,  SIG_EVG_ATPIPE_REC_OK_E, TX_OR);
	}
	else if(strstr(data->data,"\r\nERROR") != NULL)
	{
		at_exe_status = AT_EXE_ERR;
		qt_uart_dbg(uart1_conf.hdlr, "AT RSP ERROR");
		tx_event_flags_set(&atc_pipe_sig_handle, ~SIG_EVG_ATPIPE_RECING_E, TX_AND);
		tx_event_flags_set(&atc_pipe_sig_handle,  SIG_EVG_ATPIPE_REC_OK_E, TX_OR);
	}
}

/*
@func
	quectel_send_atc_hdlr
@brief
	Send ATC throuth ATC Pipe.
*/
int quectel_send_atc_hdlr(char *at_command)
{
	qapi_Status_t status = QAPI_OK;
	ULONG sig_event = 0;

	tx_event_flags_set(&atc_pipe_sig_handle, SIG_EVG_ATPIPE_SENDING_E, TX_OR);
	status = qapi_QT_Apps_Send_AT(stream_id, at_command);
	if(status != QAPI_QT_ERR_OK)
	{
		qt_uart_dbg(uart1_conf.hdlr,"[quectel_send_atc_hdlr] status[0x%x]", status);
		IOT_DEBUG("> Send ATC %s Failed", at_command);
		return -1;
	}
	tx_event_flags_get(&atc_pipe_sig_handle, SIG_EVG_ATPIPE_REC_OK_E, TX_OR, &sig_event, TX_WAIT_FOREVER);
	if(sig_event & SIG_EVG_ATPIPE_REC_OK_E)
	{
		tx_event_flags_set(&atc_pipe_sig_handle, ~SIG_EVG_ATPIPE_REC_OK_E, TX_AND);
		tx_event_flags_set(&atc_pipe_sig_handle,  SIG_EVG_ATPIPE_STANDBY_E, TX_OR);
	}

	return at_exe_status;
}

/*
@func
	quectel_send_sms_hdlr
@brief
	Send SMS throuth ATC Pipe.
*/
int quectel_send_sms_hdlr(char *call_num, char *text)
{
	qapi_Status_t status = QAPI_OK;
	ULONG sig_event = 0;
	char send_buff[256];

	memset(send_buff, 0, sizeof(send_buff));
	sprintf(send_buff, "AT+CMGS=\"%s\"\r\n", call_num);

	sms_send_status = 1;
	tx_event_flags_set(&atc_pipe_sig_handle, SIG_EVG_ATPIPE_SENDING_E, TX_OR);
	status = qapi_QT_Apps_Send_AT(stream_id, send_buff);
	if(status != QAPI_QT_ERR_OK)
	{
		qt_uart_dbg(uart1_conf.hdlr,"[quectel_send_sms_hdlr] status[0x%x]", status);
		IOT_DEBUG("> Send ATC CGMS Failed");
		return -1;
	}
	tx_event_flags_get(&atc_pipe_sig_handle, SIG_EVG_ATPIPE_SENDING_SMS, TX_OR, &sig_event, TX_WAIT_FOREVER);
	if(sig_event & SIG_EVG_ATPIPE_SENDING_SMS)
	{
		tx_event_flags_set(&atc_pipe_sig_handle, ~SIG_EVG_ATPIPE_SENDING_SMS, TX_AND);
		status = qapi_QT_Apps_Send_AT(stream_id, text);
		if(status != QAPI_QT_ERR_OK)
		{
			qt_uart_dbg(uart1_conf.hdlr,"[quectel_send_sms_hdlr] status[0x%x]", status);
			IOT_DEBUG("> Send SMS text Failed");
			return -2;
		}

		//must need wait a moment for send 1a
		qapi_Timer_Sleep(1, QAPI_TIMER_UNIT_SEC, true);
		status = qapi_QT_Apps_Send_AT_HexByte(stream_id, "1A");
		if(status != QAPI_QT_ERR_OK)
		{
			qt_uart_dbg(uart1_conf.hdlr,"[quectel_send_sms_hdlr] status[0x%x]", status);
			IOT_DEBUG("> Send SMS text Failed");
			return -2;
		}

		tx_event_flags_get(&atc_pipe_sig_handle, SIG_EVG_ATPIPE_SENDED_SMS, TX_OR, &sig_event, TX_WAIT_FOREVER);
		if(sig_event & SIG_EVG_ATPIPE_SENDED_SMS)
		{
			tx_event_flags_set(&atc_pipe_sig_handle, ~SIG_EVG_ATPIPE_SENDED_SMS, TX_AND);
			tx_event_flags_get(&atc_pipe_sig_handle,  SIG_EVG_ATPIPE_REC_OK_E, TX_OR, &sig_event, TX_WAIT_FOREVER);
			if(sig_event & SIG_EVG_ATPIPE_REC_OK_E)
			{
				tx_event_flags_set(&atc_pipe_sig_handle, ~SIG_EVG_ATPIPE_REC_OK_E, TX_AND);
				tx_event_flags_set(&atc_pipe_sig_handle, SIG_EVG_ATPIPE_STANDBY_E, TX_OR);
			}
		}
	}
	sms_send_status = 0;
	return at_exe_status;
}

/*
@func
	quectel_sms_read_hdlr
@brief
	Read SMS throuth ATC Pipe.
*/
void quectel_sms_read_hdlr(char* rsp)
{
	char command[128];
	char index[32];
	char *p1 = NULL;
	char *p2 = NULL;

	memset(command, 0, sizeof(command));
	memset(index, 0, sizeof(index));

	if(strstr(rsp,"\r\n+CMTI:") != NULL)
	{
		p1 = strstr(rsp, ",");
		p2 = strstr(p1,"\r\n");
		if(p1 && p2)
		{
			strncpy(index,p1+1, p2-p1-1);
			sms_rec_index = my_atoi(index);
		}
	}

	sprintf(command, "AT+CMGR=%d\r\n", sms_rec_index);
	if(AT_EXE_OK == quectel_send_atc_hdlr(command))
	{
		qt_uart_dbg(uart1_conf.hdlr,"[REC SMS][%d] [%s]", sms_rec_index, at_cmd_rsp);
		IOT_DEBUG("@@ sms_rec %s", at_cmd_rsp);
	}
	else
	{
		IOT_DEBUG("@@ sms_rec %s", at_cmd_rsp);
	}
}

/*
@func
	quectel_task_entry
@brief
	Entry function for task.
*/
int quectel_task_entry
(
    void
)
{
	qapi_Status_t status = QAPI_OK;
	ULONG sig_event = 0;

	qapi_Timer_Sleep(5, QAPI_TIMER_UNIT_SEC, true);

	/* Create event signal handle and clear signals */
	tx_event_flags_create(&atc_pipe_sig_handle, "atc_pipe_sig_handle");
	tx_event_flags_set(&atc_pipe_sig_handle, 0, TX_AND);

	/* uart 1 init */
	uart_init(&uart1_conf);
	/* start uart 1 receive */
	uart_recv(&uart1_conf);
	
	/* prompt, task running */
	qt_uart_dbg(uart1_conf.hdlr,"[atc pipe] start task ~");

    /* Open AT port on ATC pipe 0 */
 	status = qapi_QT_Apps_AT_Port_Open(0, &stream_id, callback_atc_pipe);
	if(status != QAPI_QT_ERR_OK)
	{
		qt_uart_dbg(uart1_conf.hdlr,"[atc pipe] status[0x%x]", status);
	}

	if(AT_EXE_OK == quectel_send_atc_hdlr("ATE0\r\n"))
	{
		qt_uart_dbg(uart1_conf.hdlr,"[ATE0] %s", at_cmd_rsp);
		IOT_DEBUG("< [ATE0] %s", at_cmd_rsp);
	}

	if(AT_EXE_OK == quectel_send_atc_hdlr("ATI\r\n"))
	{
		qt_uart_dbg(uart1_conf.hdlr,"[ATI] %s", at_cmd_rsp);
		IOT_DEBUG("< [ATI] %s", at_cmd_rsp);
	}

	if(AT_EXE_OK == quectel_send_atc_hdlr("AT+CPIN?\r\n"))
	{
		qt_uart_dbg(uart1_conf.hdlr,"[CPIN] %s", at_cmd_rsp);
		IOT_DEBUG("< [CPIN] %s", at_cmd_rsp);

		if(strstr(at_cmd_rsp, "+CPIN: READY"))
		{
			qt_uart_dbg(uart1_conf.hdlr,"[CPIN] CPIN Ready");
			IOT_DEBUG("< [CPIN] CPIN Ready");
			nw_status = NW_CPIN_READY;
		}
		else
		{
			nw_status = NW_CPIN_NOT_READY;
		}
	}

	do 
	{
		switch(nw_status)
		{
			case NW_CPIN_NOT_READY:
			{
				if(AT_EXE_OK == quectel_send_atc_hdlr("AT+CPIN?\r\n"))
				{
					qt_uart_dbg(uart1_conf.hdlr,"[CPIN] %s", at_cmd_rsp);
					IOT_DEBUG("< [CPIN] %s", at_cmd_rsp);
				
					if(strstr(at_cmd_rsp, "+CPIN: READY"))
					{
						qt_uart_dbg(uart1_conf.hdlr,"[CPIN] CPIN Ready");
						IOT_DEBUG("< [CPIN] CPIN Ready");
						nw_status = NW_CPIN_READY;
					}
					else
					{
						nw_status = NW_CPIN_NOT_READY;
					}
				}
			}break;

			case NW_CPIN_READY:
			case NW_REG_NOTREG:
			{
				/* CEREG for LTE NW */
				if(nw_status == NW_REG_NOTREG)
				{
					if(AT_EXE_OK == quectel_send_atc_hdlr("AT+CEREG?\r\n"))
					{
						qt_uart_dbg(uart1_conf.hdlr,"[CEREG] %s", at_cmd_rsp);
						IOT_DEBUG("< [CEREG] %s", at_cmd_rsp);
						if(strstr(at_cmd_rsp, "+CEREG: 0,1"))
						{
							qt_uart_dbg(uart1_conf.hdlr,"[CEREG] NW Registered");
							IOT_DEBUG("< [CEREG] NW Registered");
							nw_status = NW_REG_REGED;
						}
						else
						{
							nw_status = NW_REG_NOTREG;
						}
					}
				}

				qapi_Timer_Sleep(500, QAPI_TIMER_UNIT_MSEC, true);
				/* CRREG for GSM NW */
				if(AT_EXE_OK == quectel_send_atc_hdlr("AT+CGREG?\r\n"))
				{
					qt_uart_dbg(uart1_conf.hdlr,"[CGREG] %s", at_cmd_rsp);
					IOT_DEBUG("< [CGREG] %s", at_cmd_rsp);
					if(strstr(at_cmd_rsp, "+CGREG: 0,1"))
					{
						qt_uart_dbg(uart1_conf.hdlr,"[CGREG] NW Registered");
						IOT_DEBUG("< [CGREG] NW Registered");
						nw_status = NW_REG_REGED;
					}
					else
					{
						nw_status = NW_REG_NOTREG;
					}
				}
			}break;

			case NW_REG_DENIED:
				IOT_DEBUG("< [CGREG] NW_REG_DENIED");
			break;

			case NW_ERG_UNKNOWN:
				IOT_DEBUG("< [CGREG] NW_REG_DENIED");
			break;

			default: break;
		}
		qapi_Timer_Sleep(1000, QAPI_TIMER_UNIT_MSEC, true);
	} while(nw_status != NW_REG_REGED && nw_status != NW_REG_ROAMING);

	qapi_Timer_Sleep(1, QAPI_TIMER_UNIT_SEC, true);

	/* Config URC port map to ATC Pipe port */
	if(AT_EXE_OK == quectel_send_atc_hdlr("AT+QURCCFG=\"urcport\",\"DAM\"\r\n"))
	{
		qt_uart_dbg(uart1_conf.hdlr,"[QURCCFG] Set OK");
		IOT_DEBUG("< [QURCCFG] Set OK");
	}

	if(AT_EXE_OK == quectel_send_atc_hdlr("AT+CMGF=1\r\n"))
	{
		qt_uart_dbg(uart1_conf.hdlr,"[CMGF] Set OK");
		IOT_DEBUG("< [CMGF] Set OK");
	}

	/* Config SMS event report mode */
	if(AT_EXE_OK == quectel_send_atc_hdlr("AT+CNMI=2,1\r\n"))
	{
		qt_uart_dbg(uart1_conf.hdlr,"[CNMI] Set OK");
		IOT_DEBUG("< [CNMI] Set OK");
	}

	if(AT_EXE_OK == quectel_send_atc_hdlr("AT+CPMS=\"SM\",\"SM\",\"SM\"\r\n"))
	{
		qt_uart_dbg(uart1_conf.hdlr,"[CPMS] Set OK");
		IOT_DEBUG("< [CPMS] Set OK");
	}

	if(AT_EXE_OK == quectel_send_sms_hdlr(test_phone_number, "abcdefsgijklomopqrstuvwxyz"))
	{
		qt_uart_dbg(uart1_conf.hdlr,"SMS send OK");
		IOT_DEBUG("< SMS send OK");
	}

	while(1)
	{
		qapi_Timer_Sleep(1, QAPI_TIMER_UNIT_SEC, true);
		tx_event_flags_get(&atc_pipe_sig_handle, SIG_EVG_ATPIPE_RECED_SMS, TX_OR, &sig_event, TX_WAIT_FOREVER);
		quectel_sms_read_hdlr(at_cmd_rsp);
		tx_event_flags_set(&atc_pipe_sig_handle, ~SIG_EVG_ATPIPE_RECED_SMS, TX_AND);
	}
}

/*
@func
	my_atoi
@brief
	Local atoi function.
*/
static int my_atoi(char *str)
{
    int i=0,j=0;
    char temp[32];
    int num=0;

    for(i=0; str[i]!='\0'; i++)
	{
        if(str[i]>='0' && str[i]<='9')
		{
            temp[j++] = str[i];
        }
    }

    i=0;
    while(i<j)
	{
        num = num*10+temp[i]-'0';
	    i++;
    }
	return num;
}

#endif /*__EXAMPLE_ATC_SMS__*/

