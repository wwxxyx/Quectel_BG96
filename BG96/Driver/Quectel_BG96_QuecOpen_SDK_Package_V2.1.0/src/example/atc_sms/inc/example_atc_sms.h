/******************************************************************************
*@file    example_atc_sms.h
*@brief   example of atc sms for complete operation
*
*  ---------------------------------------------------------------------------
*
*  Copyright (c) 2018 Quectel Technologies, Inc.
*  All Rights Reserved.
*  Confidential and Proprietary - Quectel Technologies, Inc.
*  ---------------------------------------------------------------------------
*******************************************************************************/
#ifndef __EXAMPLE_ATC_SMS_H__
#define __EXAMPLE_ATC_SMS_H__

#if defined(__EXAMPLE_ATC_SMS__)
typedef enum{
	SIG_EVG_ATPIPE_STANDBY_E 		= 0x00000001,
	SIG_EVG_ATPIPE_SENDING_E 		= 0x00000010,
	SIG_EVG_ATPIPE_RECING_E  		= 0x00000100,
	SIG_EVG_ATPIPE_REC_OK_E  		= 0x00001000,
	SIG_EVG_ATPIPE_SENDING_SMS 		= 0x00010000,
	SIG_EVG_ATPIPE_SENDED_SMS		= 0x00010000,
	SIG_EVG_ATPIPE_RECED_SMS		= 0x00100000,

	SIG_EVG_ATPIPE_END
}SIG_EVG_ATPIPE_E;

typedef enum{
	NW_CPIN_NOT_READY = 0,
	NW_CPIN_READY,
	NW_REG_NOTREG,
	NW_REG_REGED,
	NW_REG_DENIED,
	NW_ERG_UNKNOWN,
	NW_REG_ROAMING,
	NW_END
}NW_STATUS_E;

typedef enum{
	AT_EXE_OK = 0,
	AT_EXE_ERR,
	AT_EXE_END
}AT_EXE_E;

#endif /*__EXAMPLE_ATC_SMS__*/

#endif /*__EXAMPLE_ATC_SMS_H__*/

