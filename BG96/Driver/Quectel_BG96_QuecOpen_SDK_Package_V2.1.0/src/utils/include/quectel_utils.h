/******************************************************************************

            quectel_utils.h

******************************************************************************/

/******************************************************************************/
#ifndef __QUECTEL_UTILS_IF_H__
#define __QUECTEL_UTILS_IF_H__
#include "stdio.h"
#include "string.h"
#include "stdarg.h"
#include "qapi_diag.h"

int qt_log_printf(int level, const char* fmt, ...);

/**
 *    @brief Debug level logging macro.
 *
 *     Macro to expose function, line number as well as desired log message.
 */
#ifdef ENABLE_IOT_DEBUG
#define IOT_DEBUG(...)    \
{\
    qt_log_printf(MSG_LEGACY_HIGH,__VA_ARGS__); \
}
#else
#define IOT_DEBUG(...)  
#endif

/**
 *    @brief Info level logging macro.
 *
 *    Macro to expose desired log message.  Info messages do not include automatic function names and line numbers.
 */
#ifdef ENABLE_IOT_INFO
#define IOT_INFO(...)    \
{\
    qt_log_printf(MSG_LEGACY_HIGH, __VA_ARGS__); \
}
#else
#define IOT_INFO(...)
#endif  
#endif /* __QUECTEL_UTILS_IF_H__ */

