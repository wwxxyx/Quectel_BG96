#ifndef __QL_LOG_H__
#define __QL_LOG_H__
#ifdef USE_NDK
#include <android/log.h>

#ifndef RLOGV
#define RLOGV(fmt, arg...) ((void)__android_log_print(ANDROID_LOG_VERBOSE, "RIL " LOG_TAG, fmt, ##arg))
#endif
#ifndef RLOGD
#define RLOGD(fmt, arg...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "RIL "LOG_TAG, fmt, ##arg))
#endif
#ifndef RLOGI
#define RLOGI(fmt, arg...) ((void)__android_log_print(ANDROID_LOG_INFO, "RIL "LOG_TAG, fmt, ##arg))
#endif
#ifndef RLOGW
#define RLOGW(fmt, arg...) ((void)__android_log_print(ANDROID_LOG_WARN, "RIL "LOG_TAG, fmt, ##arg))
#endif
#ifndef RLOGE
#define RLOGE(fmt, arg...) ((void)__android_log_print(ANDROID_LOG_ERROR, "RIL "LOG_TAG, fmt, ##arg))
#endif

#define PROPERTY_KEY_MAX   32
#define PROPERTY_VALUE_MAX  92
extern const char *(*requestToString)(int request);
extern int (*ifc_init)(void);
extern void (*ifc_close)(void);
extern int (*do_dhcp)(const char *iname);
extern void (*get_dhcp_info)(uint32_t *ipaddr, uint32_t *gateway, uint32_t *prefixLength,
                   uint32_t *dns1, uint32_t *dns2, uint32_t *server,
                   uint32_t *lease);  
extern int (*property_set)(const char *key, const char *value);
extern int (*property_get)(const char *key, char *value, const char *default_value);
extern void ql_ndk_init(void);
#else
#include <utils/Log.h>
#include <cutils/properties.h>
#include <netutils/ifc.h>
extern const char * requestToString(int request);

#ifndef RLOGV
#define RLOGV(fmt, arg...) ((void)__android_log_buf_print(LOG_ID_RADIO, ANDROID_LOG_VERBOSE, LOG_TAG, fmt, ##arg))
#endif
#ifndef RLOGD
#define RLOGD(fmt, arg...) ((void)__android_log_buf_print(LOG_ID_RADIO, ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##arg))
#endif
#ifndef RLOGI
#define RLOGI(fmt, arg...) ((void)__android_log_buf_print(LOG_ID_RADIO, ANDROID_LOG_INFO, LOG_TAG, fmt, ##arg))
#endif
#ifndef RLOGW
#define RLOGW(fmt, arg...) ((void)__android_log_buf_print(LOG_ID_RADIO, ANDROID_LOG_WARN, LOG_TAG, fmt, ##arg))
#endif
#ifndef RLOGE
#define RLOGE(fmt, arg...) ((void)__android_log_buf_print(LOG_ID_RADIO, ANDROID_LOG_ERROR, LOG_TAG, fmt, ##arg))
#endif
#endif

#undef LOGV
#undef LOGD
#undef LOGI
#undef LOGW
#undef LOGE
#define LOGV RLOGV
#define LOGD RLOGD
#define LOGI RLOGI
#define LOGW RLOGW
#define LOGE RLOGE
#endif
