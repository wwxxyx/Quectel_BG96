/******************************************************************************

                        Q U E C T E L _ U T I L S . C

******************************************************************************/
/******************************************************************************

  @file    quectel_utils.c
  @brief   quectel common interface file

******************************************************************************/
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include "string.h"
#include "quectel_utils.h"

#define PRINT_SIZE 256

static char qt_log_buffer[PRINT_SIZE]; /*Print buffer*/

int qt_log_printf(int level, const char *fmt, ...)
{

	va_list arg_list;
	int len;

	va_start(arg_list, fmt);
	len = vsnprintf((char *)(qt_log_buffer),
							 sizeof(qt_log_buffer),
							 (char *)fmt,
							 arg_list
							 );
	va_end(arg_list);

	if(len >= PRINT_SIZE)
	{
		len = PRINT_SIZE - 1;
	}

	qt_log_buffer[len] = 0;

	if(level == MSG_LEGACY_ERROR)
	{
		QAPI_MSG_SPRINTF(MSG_SSID_LINUX_DATA, MSG_LEGACY_ERROR, "%s", qt_log_buffer);
	}
	else 
	{
		QAPI_MSG_SPRINTF(MSG_SSID_LINUX_DATA, MSG_LEGACY_HIGH, "%s", qt_log_buffer);
	}

	return 1;
}


