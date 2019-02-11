/******************************************************************************
*@file    example_task.h
*@brief   example of new task creation
*
*  ---------------------------------------------------------------------------
*
*  Copyright (c) 2018 Quectel Technologies, Inc.
*  All Rights Reserved.
*  Confidential and Proprietary - Quectel Technologies, Inc.
*  ---------------------------------------------------------------------------
*******************************************************************************/
#ifndef __QUECTEL_TASK_H__
#define __QUECTEL_TASK_H__

#if defined(__EXAMPLE_TASK_CREATE__)
#include "qapi_fs_types.h"
#include "txm_module.h"

#define QT_Q_MAX_INFO_NUM		16

typedef struct TASK_COMM_S{
	ULONG f1;
	ULONG f2;
	ULONG f3;
	ULONG f4;
	ULONG f5;
	ULONG f6;
	ULONG f7;
	ULONG f8;
}TASK_COMM;

#endif /*__EXAMPLE_TASK_CREATE__*/
#endif /*__QUECTEL_TASK_H__*/

