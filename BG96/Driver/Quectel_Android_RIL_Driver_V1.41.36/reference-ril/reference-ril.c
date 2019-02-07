/* //device/system/reference-ril/reference-ril.c
**
** Copyright 2006, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#include <ctype.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <pthread.h>
#include <alloca.h>
#include "atchannel.h"
#include "at_tok.h"
#include "misc.h"
#include <getopt.h>
#include <linux/sockios.h>
#include <termios.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/statfs.h>
#include "../include/telephony/ril.h"
#include "ql-usb.h"
#include "ql-conf.h"
#ifdef RIL_REQUEST_GET_CELL_INFO_LIST
#include <math.h> //for floor
#endif

#define LOG_NDEBUG 0
#define LOG_TAG "RILC"
#include "ql-log.h"

#define MAX_AT_RESPONSE 0x1000

#if 1 //quectel
/* pathname returned from RIL_REQUEST_SETUP_DATA_CALL / RIL_REQUEST_SETUP_DEFAULT_PDP */
//#define PPP_TTY_PATH "ppp0"
//һЩר?�����??ע?Ϲ???????ֻ????????APN/USER/PASSWD??????????ע??ר??
//ģ???ڲ?Ĭ??ʹ?õ?һ·��ע???
//????????ʹ?õ?һ·??????pdp��???ţ???ô?ͻᵼ?µ?һ·??????·ͬʱער???
//??ר???ǲ???ͬʱ??????·pdp???????
#define CONFIG_DEFAULT_PDP 1 // 5
static const char *PPP_TTY_PATH = "ppp0";
static int s_default_pdp = 1;
static int quectel_cimi = 0;
static int g_card_state_change_flg = 0;
#define CUSD_USE_UCS2_MODE
//#define USB_HOST_USE_RESET_RESUME //like s5pv210 donnot support usb suspend and wakeup
//#define USB_HOST_USE_Autochips_ac83xx
//#define USB_HOST_FORCE_OPEN_GPS_PORT
//#define USB_HOST_FORCE_OPEN_PPP_PORT
#ifdef USB_HOST_USE_Autochips_ac83xx
#define USB_HOST_FORCE_OPEN_PPP_PORT
#endif
#ifdef USB_HOST_FORCE_OPEN_GPS_PORT
static int s_gps_fd = -1;
#endif
#ifdef USB_HOST_FORCE_OPEN_PPP_PORT
static int s_ppp_fd = -1;
#endif
//#define QUECTEL_REPORT_CDMA_AS_UMTS_TO_RILJ
//#define QUECTEL_RESTART_RIL_WHEN_NO_SETUP_DATA_CALL 10

#define REFERENCE_RIL_VERSION    "Quectel_Android_RIL_Driver_V1.41.36"
#define QUECTEL_REPORT_SIGNAL_STRENGTH
#ifdef QUECTEL_REPORT_SIGNAL_STRENGTH
int poll_signal_started = 0;
#endif

static  char ql_ttyAT[20];
char * ql_get_ttyGPS(char *out_ttyname) ;
extern char * ql_get_ttyAT(char *out_ttyname);
extern char *  ql_get_ttyPPP(char *out_ttyname);
//void ql_set_autosuspend(int enable);
#ifdef USE_NDIS
extern int ql_pppd_start(const char *modemport, const char *user, const char *password, const char *auth_type, const char *ppp_number);
extern int ql_pppd_stop(int signo);
extern char * ql_get_ndisname(char *p_usbnet_adapter, int i);
extern int ql_ndis_start(const char *modemport, const char *user, const char *password, const char *auth_type, int default_pdp);
extern int ql_ndis_stop(int signo);
#else
static char * ql_get_ndisname(char *p_usbnet_adapter, int i) { return NULL; }
static int ql_ndis_start(const char *apn, const char *user, const char *password, const char *auth_type) { return 0; }
static int ql_ndis_stop(int signo) { return 0; }
#endif
static const char *ql_product_version = NULL;
#define ql_is_XX(__ql_module_name) (!strncmp(ql_product_version, __ql_module_name, strlen(__ql_module_name)))
static int ql_is_9X07(void) {
    const char *products[] = {"EC21", "EC25", "AG35", "EG91", "EG95", "EM05", "EM06", "EG06", "EP06", "BG95", "BG96", NULL};
    int i = 0;

    while (products[i]) {
        if (ql_is_XX(products[i])) return 1;
        i++;
    }
    return 0;
}
static int ql_is_UC20 = 0;
static int ql_is_mdm9215 = 0;
static int ql_is_mdm9x07 = 0;
static int ql_is_UG95 = 0;
static int ql_is_GSM = 0;
static int ql_is_device_support_cdma = 0;
static int ql_is_device_and_sim_both_support_cdma = 0;
#define ql_cdmaphone (ql_get_phonetype() == PHONE_TYPE_CDMA)
#define NETWORK_DEBOUNCE_TIMEOUT 20
static int network_debounce_time = 0;
static int onRequestCount = 0;
static int time_zone_report = 0;
static int s_lastPreferredNetworkType = -1;
static int wait_for_powerdown = 0;
static int bSetupDataCallCompelete = 0;
static int nSetupDataCallFailTimes = 0;
static int bVoiceCallInCTNw = 0;
static char sKeepLocalip[PROPERTY_VALUE_MAX]={0};
extern const char s_ql_buildinfo[];

extern int ql_tty2tcp(const char *s, const char *d, int fifo_size);
extern void ql_gps_init(const char *module_version);
extern void ql_datafwd_init(void);
extern void ql_datafwd_send_to_apk(unsigned char *hex_data, int length);
extern void ql_dtmf_init(void);
extern void ql_dtmf_send_to_apk(unsigned char dtmf);

static int quectel_at_cops(int response[4]);
static void get_local_ip(char *local_ip);

//neo
enum {
    PHONE_TYPE_NONE,
    PHONE_TYPE_GSM,
    PHONE_TYPE_CDMA,
    PHONE_TYPE_SIP,
};
#define PROP_CURRENT_ACTIVE_PHONE "gsm.current.phone-type"
static int dialing_phone_type = PHONE_TYPE_NONE;

static int ql_get_phonetype(void)
{
    char prop_value[PROPERTY_VALUE_MAX] = {0};
    property_get(PROP_CURRENT_ACTIVE_PHONE, prop_value, "0");
    return atoi(prop_value);
}

static int ql_is_cdma(void)
{
    int cops_response[4] = {0};
    int err = 0;
    int is_cdma = 0;
    ATResponse *p_response = NULL;
    ATLine *p_cur = NULL;
    char *line = NULL;
    char *ret = NULL;
    unsigned int i = 0;
    char *cdma_array[] = {"CDMA", "HDR"};

    if (ql_is_device_and_sim_both_support_cdma == 0)
        return 0;

    quectel_at_cops(cops_response);
    if (100 == cops_response[3]) {
        is_cdma = 1;
        goto out;
    }

    LOGD("cops may canot display cdma mode, try qcsysmode!\n");
    err = at_send_command_singleline("at$qcsysmode", "\0", &p_response);
    if ((err != 0) || !p_response || (p_response->success == 0)) {
        LOGE("%s: at_send_command qcsysmode failed!!!\n", __func__);
        goto out;
    }

    p_cur = p_response->p_intermediates;
    if (NULL == p_cur) {
        LOGE("%s: null pointer -> p_cur!\n", __func__);
        goto out;
    }

    line = p_cur->line;

    err = at_tok_nextstr(&line, &ret);
    if (err < 0) {
        LOGE("%s: at_tok_nextstr failed!!!\n", __func__);
        goto out;
    }

    for (i = 0; i < sizeof(cdma_array)/sizeof(cdma_array[0]); i++) {
        if (strstr((const char *)ret, (const char *)cdma_array[i])) {
            is_cdma = 1;
            goto out;
        }
    }

out:
    return is_cdma;
}

#if 1 //quectel //for debug-purpose, record logcat msg to file
//how to use:
//create dir /data/quectel_debug_log/ on your board, then reboot your board.
//quectel ril will auto catch android/radio log when your board boot up, and save to dir /data/quectel_debug_log/
//then you can fetch logfiles to host-pc by adb tools using command "adb pull /data/quectel_debug_log/"
//remember to delete this dir /data/quectel_debug_log/ when you donot need this function.
#define QL_DEBUG_LOG_PATH "/data/quectel_debug_log"
static int ql_debug_space_available(void) {
        struct statfs stat;
        if (!statfs(QL_DEBUG_LOG_PATH, &stat)) {
            int free_space = (((stat.f_bfree*100)/stat.f_blocks));
            LOGD("%s %d%%", __func__, free_space);
            return (free_space > 20);
    }
        return 0;
}
static void log_dmesg(const char *tag) {
#if 1 // may take long time
    if (access(QL_DEBUG_LOG_PATH, W_OK) || !ql_debug_space_available())
        return;

    if (fork() == 0)
    {
        char logcat_cmd[256];
        void *pbuf;;
        size_t len;
        time_t rawtime;
        struct tm *timeinfo;
        FILE *dmesgfp, *logfilefp;
        time(&rawtime);
        timeinfo=localtime(&rawtime );
        sprintf(logcat_cmd, QL_DEBUG_LOG_PATH"/%02d%02d_%02d%02d%02d_dmesg_%s.txt",
            timeinfo->tm_mon+1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, tag);
        logfilefp = NULL; //fopen(logcat_cmd, "wb");
        dmesgfp = popen("dmesg -c", "r");
        pbuf = malloc(64*1024);
        if (/*logfilefp && */dmesgfp && pbuf) {
            while((len = fread(pbuf, 1, 64*1024, dmesgfp)) > 0) {
                LOGD("%s len = %zd\n", __func__, len);
                if (logfilefp == NULL)
                    logfilefp = fopen(logcat_cmd, "wb");
                if (logfilefp != NULL)
                    fwrite(pbuf, 1, len, logfilefp);
            }
        } else {
            LOGD("logfilefp=%p, dmesgfp=%p, errno: %d (%s)", logfilefp, dmesgfp, errno, strerror(errno));
        }
        if (logfilefp)
            fclose(logfilefp);
        if (dmesgfp)
            fclose(dmesgfp);
        if (pbuf)
            free(pbuf);
        exit(0);
    }
#endif
}
#endif
static int gprsState = -1; // 1 ~in service, 0 ~ out of service
static void setDataServiceState(int newState) {
    newState = (newState != 0);
    if (newState != gprsState) {
        const char *gprsStateString[] = {"0", "1"};
        gprsState = newState;
        //property_set("net.gprs.gprsState", gprsStateString[gprsState]);
    }
}
static int currentDataServiceState(void) {
    return (gprsState == 1);
}

#define MUX_CHN_DIR "/data/misc/rild/" //selinux donot allow rild create files in /dev/chn
int ql_mux_enabled = 0;
#define CMUX_AT_PORT MUX_CHN_DIR "1" //"/dev/ttygsm1"
#define CMUX_PPP_PORT MUX_CHN_DIR "2" //"/dev/ttygsm2"
static int cmux_speed = 115200;
static int cmux_ctsrts = 0;
#ifdef USE_MUXD
extern int gsm0710muxd(const char *serialname, int speed, int ctsrts);
#else
int gsm0710muxd(const char *serialname, int speed, int ctsrts) { return -EEXIST; }
#endif
static int QL_RIL_VERSION = RIL_VERSION;
#endif

typedef enum {
    SIM_ABSENT = 0,
    SIM_NOT_READY = 1,
    SIM_READY = 2, /* SIM_READY means the radio state is RADIO_STATE_SIM_READY */
    SIM_PIN = 3,
    SIM_PUK = 4,
    SIM_NETWORK_PERSONALIZATION = 5
} SIM_Status;

static void onRequest (int request, void *data, size_t datalen, RIL_Token t);
static RIL_RadioState currentState();
static int onSupports (int requestCode);
static void onCancel (RIL_Token t);
static const char *getVersion();
static int isRadioOn();
static SIM_Status getSIMStatus();
static int getCardStatus(RIL_CardStatus_v6 **pp_card_status);
static void freeCardStatus(RIL_CardStatus_v6 *p_card_status);
static void onDataCallListChanged(void *param);
static void requestDeactivateDefaultPDP(void *data, size_t datalen, RIL_Token t);

/*** Static Variables ***/
static RIL_RadioFunctions s_callbacks = {
    RIL_VERSION,
    onRequest,
    currentState,
    onSupports,
    onCancel,
    getVersion
};

#ifdef RIL_SHLIB
static const struct RIL_Env *s_rilenv;

#define RIL_onRequestComplete(t, e, response, responselen) s_rilenv->OnRequestComplete(t,e, response, responselen)
#define RIL_onUnsolicitedResponse(a,b,c) s_rilenv->OnUnsolicitedResponse(a,b,c)
#define RIL_requestTimedCallback(a,b,c) s_rilenv->RequestTimedCallback(a,b,c)
void RILC_requestTimedCallback (RIL_TimedCallback callback, void *param,
                                const struct timeval *relativeTime) {
    RIL_requestTimedCallback (callback, param, relativeTime);
}
#endif

static RIL_RadioState sState = RADIO_STATE_UNAVAILABLE;

static pthread_mutex_t s_state_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t s_state_cond = PTHREAD_COND_INITIALIZER;

static int s_port = -1;
static const char * s_device_path = NULL;
static int          s_device_socket = 0;

/* trigger change to this with s_state_cond */
static int s_closed = 0;
static int s_recovery_mode = 0;

static int sFD;     /* file desc of AT channel */
static char sATBuffer[MAX_AT_RESPONSE+1];
static char *sATBufferCur = NULL;

static const struct timeval TIMEVAL_SIMPOLL = {1,0};
static const struct timeval TIMEVAL_CALLSTATEPOLL = {0,500000};
static const struct timeval TIMEVAL_0 = {0,0};
static const struct timeval TIMEVAL_1 = {1,0};
static const struct timeval TIMEVAL_3 = {3,0};
static const struct timeval TIMEVAL_10 = {10,0};
static const struct timeval TIMEVAL_20 = {20,0};
static const struct timeval TIMEVAL_40 = {40,0};

static void onSIMStateChange (void *param);
static void onSMSReady(void *param);
static void pollSIMState (void *param);
static void setRadioState(RIL_RadioState newState);

#if 1 //quectel
static unsigned int ussd_pending_index = 0;
static const struct timeval ussd_timeout_timeval = {30,0}; //seconds
static void onUssdResponse(char *mode) {
    char *response[2];
    response[0] = mode;

    switch (mode[0] - '0') {
        case 2:
            response[1] = "USSD terminated by network";
        break;
        case 3:
            response[1] = "Other local client has responded";
        break;
        case 4:
            response[1] = "Operation not supported";
        break;
        case 5:
        default:
            response[1] = "Network time out";
        break;
    }
    RIL_onUnsolicitedResponse(RIL_UNSOL_ON_USSD, response, sizeof(response[0]) + sizeof(response[1]));
}
static void onUssdTimedCallback(void *param) {
    if (ussd_pending_index != *((unsigned int *) param))
        return;
    at_send_command("AT+CUSD=2", NULL); //cancel
    onUssdResponse("5");
}

static int gsm_hexchar_to_int( char  c ) {
    if ((unsigned)(c - '0') < 10)
        return c - '0';
    if ((unsigned)(c - 'a') < 6)
        return 10 + (c - 'a');
    if ((unsigned)(c - 'A') < 6)
        return 10 + (c - 'A');
    return -1;
}

static int gsm_hex2_to_byte( const char*  hex ) {
    int  hi = gsm_hexchar_to_int(hex[0]);
    int  lo = gsm_hexchar_to_int(hex[1]);

    if (hi < 0 || lo < 0)
        return -1;

    return ( (hi << 4) | lo );
}

typedef unsigned char  byte_t;
typedef byte_t*        bytes_t;
typedef const byte_t*  cbytes_t;
static  int utf8_write( bytes_t  utf8, int  offset, int  v ) {
    int  result;

    if (v < 128) {
        result = 1;
        if (utf8)
            utf8[offset] = (byte_t) v;
    } else if (v < 0x800) {
        result = 2;
        if (utf8) {
            utf8[offset+0] = (byte_t)( 0xc0 | (v >> 6) );
            utf8[offset+1] = (byte_t)( 0x80 | (v & 0x3f) );
        }
    } else if (v < 0x10000) {
        result = 3;
        if (utf8) {
            utf8[offset+0] = (byte_t)( 0xe0 |  (v >> 12) );
            utf8[offset+1] = (byte_t)( 0x80 | ((v >> 6) & 0x3f) );
            utf8[offset+2] = (byte_t)( 0x80 |  (v & 0x3f) );
        }
    } else {
        result = 4;
        if (utf8) {
            utf8[offset+0] = (byte_t)( 0xf0 | ((v >> 18) & 0x7) );
            utf8[offset+1] = (byte_t)( 0x80 | ((v >> 12) & 0x3f) );
            utf8[offset+2] = (byte_t)( 0x80 | ((v >> 6) & 0x3f) );
            utf8[offset+3] = (byte_t)( 0x80 |  (v & 0x3f) );
        }
    }
    return  result;
}

/* convert a UCS2 string into a UTF8 byte string, assumes 'buf' is correctly sized */
static int ucs2_to_utf8( cbytes_t  ucs2, int       ucs2len, bytes_t   buf ) {
    int  nn;
    int  result = 0;

    for (nn = 0; nn < ucs2len; ucs2 += 2, nn++) {
        int  c= (ucs2[0] << 8) | ucs2[1];
        result += utf8_write(buf, result, c);
    }
    return result;
}

static int quectel_at_cops(int response[4]) {
    int err;
    ATResponse *p_response = NULL;
    char *line;
    char *oper;

    response[0] = response[1] = response[2] = response[3] = 0;

    err = at_send_command_singleline("AT+COPS=3,2;+COPS?", "+COPS:", &p_response);
    if ((err < 0) ||  (p_response == NULL) || (p_response->success == 0))
        goto error;

    line = p_response->p_intermediates->line;

    err = at_tok_start(&line);
    if (err < 0) goto error;

//+COPS:<mode>[,<format>[,<oper>][,<Act>]]
    err = at_tok_nextint(&line, &response[0]);
    if (err < 0) goto error;

    if (!at_tok_hasmore(&line)) goto error;

    err = at_tok_nextint(&line, &response[1]);
    if (err < 0) goto error;

    if (!at_tok_hasmore(&line)) goto error;

    err = at_tok_nextstr(&line, &oper);
    if (err < 0) goto error;
    response[2] = atoi(oper);

    if (!at_tok_hasmore(&line)) goto error;

    err = at_tok_nextint(&line, &response[3]);
    if (err < 0) goto error;

error:
     at_response_free(p_response);
     return response[3];
}

static int quectel_at_qnwinfo(void) {
    int err;
    ATResponse *p_response = NULL;
    char *line;
    int radio_tech = RADIO_TECH_1xRTT;
    char *act_str;

    err = at_send_command_singleline("AT+QNWINFO", "+QNWINFO:", &p_response);
    if ((err < 0) ||  (p_response == NULL) || (p_response->success == 0))
        goto error;

    line = p_response->p_intermediates->line;

    err = at_tok_start(&line);
    if (err < 0) goto error;

//+QNWINFO: <Act>,<oper>,<band>,<channel>
    err = at_tok_nextstr(&line, &act_str);
    if (err < 0) goto error;

    if (!strcmp(act_str, "NONE")) {
        radio_tech = RADIO_TECH_UNKNOWN;
    } else if (!strcmp(act_str, "CDMA1X")) {
        radio_tech = RADIO_TECH_1xRTT;
    } else if (!strcmp(act_str, "CDMA1X AND HDR")) {
        radio_tech = RADIO_TECH_EVDO_A;
    } else if (!strcmp(act_str, "CDMA1X AND EHRPD")) {
        radio_tech = RADIO_TECH_EHRPD;
    } else if (!strcmp(act_str, "HDR")) {
        radio_tech = RADIO_TECH_EVDO_A;
    } else if (!strcmp(act_str, "HDR-EHRPD")) {
        radio_tech = RADIO_TECH_EHRPD;
    } else if (!strcmp(act_str, "GSM")) {
        radio_tech = RADIO_TECH_GPRS;
    } else if (!strcmp(act_str, "GPRS")) {
        radio_tech = RADIO_TECH_GPRS;
    } else if (!strcmp(act_str, "EDGE")) {
        radio_tech = RADIO_TECH_EDGE;
    } else if (!strcmp(act_str, "WCDMA")) {
        radio_tech = RADIO_TECH_UMTS;
    } else if (!strcmp(act_str, "HSDPA")) {
        radio_tech = RADIO_TECH_HSDPA;
    } else if (!strcmp(act_str, "HSUPA")) {
        radio_tech = RADIO_TECH_HSUPA;
    } else if (!strcmp(act_str, "HSPA+")) {
        radio_tech = RADIO_TECH_HSPAP;
    } else if (!strcmp(act_str, "TDSCDMA")) {
        radio_tech = RADIO_TECH_UMTS;
    } else if (!strcmp(act_str, "TDD LTE")) {
        radio_tech = RADIO_TECH_LTE;
    } else if (!strcmp(act_str, "FDD LTE")) {
        radio_tech = RADIO_TECH_LTE;
    } else {
        radio_tech = RADIO_TECH_1xRTT;
    }

error:
     at_response_free(p_response);
     return radio_tech;
}

static int quectel_at_sysinfo(void) {
    int err;
    ATResponse *p_response = NULL;
    char *line;
    int radio_tech = RADIO_TECH_UNKNOWN;
    int response[5];

    err = at_send_command_singleline("AT^SYSINFO", "^SYSINFO:", &p_response);
    if ((err < 0) ||  (p_response == NULL) || (p_response->success == 0))
        goto error;

    line = p_response->p_intermediates->line;

    err = at_tok_start(&line);
    if (err < 0) goto error;

    err = at_tok_nextint(&line, &response[0]);
    if (err < 0) goto error;

    err = at_tok_nextint(&line, &response[1]);
    if (err < 0) goto error;

    err = at_tok_nextint(&line, &response[2]);
    if (err < 0) goto error;

    err = at_tok_nextint(&line, &response[3]);
    if (err < 0) goto error;

//^SYSINFO: 2,2,0,9,1
    switch (response[3]) {
        case 2:
              radio_tech = RADIO_TECH_1xRTT;
        break;

        case 4:
        case 8:
            radio_tech = RADIO_TECH_EVDO_A;
        break;

        default:
            radio_tech = RADIO_TECH_UNKNOWN;
        break;
    }

error:
     at_response_free(p_response);
     return radio_tech;
}

static int quectel_at_cgreg(int response[4])  {
    int err;
    ATResponse *p_response = NULL;
    char *line;
    int commas;
    int skip;
    int count = 3;

    response[0] = response[1] = response[2] = response[3] = 0;

    err = at_send_command_singleline("AT+CGREG?", "+CGREG:", &p_response);
    if ((err < 0) ||  (p_response == NULL) || (p_response->success == 0))
        goto error;

    line = p_response->p_intermediates->line;

    err = at_tok_start(&line);
    if (err < 0) goto error;

    err = at_tok_nextint(&line, &skip);
    if (err < 0) goto error;

    err = at_tok_nextint(&line, &response[0]);
    if (err < 0) goto error;

    err = at_tok_nexthexint(&line, &response[1]);
    if (err < 0) goto error;

    err = at_tok_nexthexint(&line, &response[2]);
    if (err < 0) goto error;

    err = at_tok_nexthexint(&line, &response[3]);
    if (err < 0) goto error;

error:
    at_response_free(p_response);
    return 0;
}

//Only can be used on EC20 now, to check the status of PPP link layer
static int quectel_at_datamode(void)
{
	int err;
	ATResponse *p_response = NULL;
	char *line;
	int ppp_status = 0; // 0-> disconnect, 1->connect

	err = at_send_command_singleline("AT^DATAMODE", "^DATAMODE:",&p_response);
	if((err < 0) || (p_response == NULL) || (p_response->success == 0))
            goto error;

	line = p_response->p_intermediates->line;

	err = at_tok_start(&line);
	if(err < 0) goto error;

	err = at_tok_nextint(&line, &ppp_status);
	if(err < 0) goto error;

	if(!ppp_status)
        {
            LOGE("!!!PPP link is disconnected after sleep mode");
            //ql_pppd_stop(SIGTERM);
        }

error:
	at_response_free(p_response);
	return ppp_status;
}

static int quectel_is_ip_change_in_cdma_ppp(void)
{
    char curIpAddress[64]={0};

    get_local_ip(curIpAddress);

    if(!strcmp(curIpAddress, "0.0.0.0"))
    {
        LOGE("!!!PPP link is disabled in CDMA mode after sleep");
        //ql_pppd_stop(SIGTERM);
        return 1;
    }

    if(strcmp(curIpAddress, sKeepLocalip))
    {
        LOGE("!!!PPP ip was changed due to NW resync PPP after UE exit from sleep");
        //ql_pppd_stop(SIGTERM);
        return 1;
    }

    return 0;
}

static void poll_cdma_evdo_ppp_state(void *param) {
    if (ql_is_device_and_sim_both_support_cdma && !strcmp(PPP_TTY_PATH, "ppp0") && !access("/sys/class/net/ppp0", R_OK)) {
     	if (quectel_is_ip_change_in_cdma_ppp()) {
            ql_pppd_stop(SIGTERM);
        } else {
            RIL_requestTimedCallback(poll_cdma_evdo_ppp_state, NULL, &TIMEVAL_3);
        }
     }
}

#ifdef QUECTEL_RESTART_RIL_WHEN_NO_SETUP_DATA_CALL
static int ql_setup_data_call = 0;
static void onSetupDataCallTimeout (void *param) 
{
    if (!ql_setup_data_call) {
        int cgreg_response[4];
        LOGD("%s", __func__);
        quectel_at_cgreg(cgreg_response);
        if (cgreg_response[0] == 1 || cgreg_response[0] == 5) {
            exit(1);
        }
    }
}
#endif
#endif

static int clccStateToRILState(int state, RIL_CallState *p_state)

{
    switch(state) {
        case 0: *p_state = RIL_CALL_ACTIVE;   return 0;
        case 1: *p_state = RIL_CALL_HOLDING;  return 0;
        case 2: *p_state = RIL_CALL_DIALING;  return 0;
        case 3: *p_state = RIL_CALL_ALERTING; return 0;
        case 4: *p_state = RIL_CALL_INCOMING; return 0;
        case 5: *p_state = RIL_CALL_WAITING;  return 0;
        default: return -1;
    }
}

/**
 * Note: directly modified line and has *p_call point directly into
 * modified line
 */
static int callFromCLCCLine(char *line, RIL_Call *p_call)
{
        //+CLCC: 1,0,2,0,0,\"+18005551212\",145
        //     index,isMT,state,mode,isMpty(,number,TOA)?

    int err;
    int state;
    int mode;

    err = at_tok_start(&line);
    if (err < 0) goto error;

    err = at_tok_nextint(&line, &(p_call->index));
    if (err < 0) goto error;

    err = at_tok_nextbool(&line, &(p_call->isMT));
    if (err < 0) goto error;

    err = at_tok_nextint(&line, &state);
    if (err < 0) goto error;

    err = clccStateToRILState(state, &(p_call->state));
    if (err < 0) goto error;

    err = at_tok_nextint(&line, &mode);
    if (err < 0) goto error;

    p_call->isVoice = (mode == 0);

    err = at_tok_nextbool(&line, &(p_call->isMpty));
    if (err < 0) goto error;

    if (at_tok_hasmore(&line)) {
        err = at_tok_nextstr(&line, &(p_call->number));

        /* tolerate null here */
        if (err < 0) return 0;

        // Some lame implementations return strings
        // like "NOT AVAILABLE" in the CLCC line
        if (p_call->number != NULL
            && 0 == strspn(p_call->number, "+0123456789")
        ) {
            p_call->number = NULL;
        }

        err = at_tok_nextint(&line, &p_call->toa);
        if (err < 0) goto error;
    }

    p_call->uusInfo = NULL;

    return 0;

error:
    LOGE("invalid CLCC line\n");
    return -1;
}


/** do post-AT+CFUN=1 initialization */
static void onRadioPowerOn()
{
    pollSIMState(NULL);
}

/** do post- SIM ready initialization */
static void onSIMReady()
{
    onSMSReady(NULL);

    /*  Call Waiting notifications */
    at_send_command("AT+CCWA=1", NULL);

    /*  Alternating voice/data off */
    at_send_command("AT+CMOD=0", NULL);

    /*  Not muted */
    at_send_command("AT+CMUT=0", NULL);

    /*  +CSSU unsolicited supp service notifications */
    at_send_command("AT+CSSN=0,1", NULL);

    /*  no connected line identification */
    at_send_command("AT+COLP=0", NULL);

    at_send_command("AT+CSCS=\"UCS2\"", NULL);
    if (ql_is_UC20)
        at_send_command("at+qcfg=\"ussd/cause\",1", NULL);

    /*  USSD unsolicited */
    at_send_command("AT+CUSD=1", NULL);

    at_send_command("AT+CGEREP=0", NULL); //carl tmp disable

    at_send_command("AT+CGEREP=0", NULL); //carl tmp disable

    at_send_command("AT+CTZU=1", NULL);
    at_send_command("AT+CTZR=2", NULL);

    RIL_onUnsolicitedResponse (RIL_UNSOL_RESPONSE_SIM_STATUS_CHANGED, NULL, 0);
}

static void requestRadioPower(void *data, size_t datalen, RIL_Token t)
{
    int onOff;

    int err;
    ATResponse *p_response = NULL;

	LOGE("%s begin.\n", __func__);
    assert (datalen >= sizeof(int *));
    onOff = ((int *)data)[0];

    if (onOff == 0 && sState != RADIO_STATE_OFF) {
        const char SHUTDOWN_ACTION_PROPERTY[PROPERTY_VALUE_MAX] = "sys.shutdown.requested";
        char shutdownAction[PROPERTY_VALUE_MAX];
        if (!ql_mux_enabled && (property_get(SHUTDOWN_ACTION_PROPERTY, shutdownAction, NULL) > 0)) {
            int timeout = 0;
            LOGD("%s shutdownAction is %s", __func__, shutdownAction);
#if 0 //quectel, i guess nobody like this, because it take long time waiting for moderm shutdown
            err = at_send_command("AT+QPOWD", &p_response);
            if (err < 0 || p_response->success == 0) goto error;
            wait_for_powerdown = 1;
            //timeout in ShutdownThread.java less than 15
            while ((timeout < (15*10)) && (wait_for_powerdown != 3)) { //wait for urc "POWERED DOWN" and usb disconect
                //LOGD("%s wait_for_powerdown %d %d", __func__, timeout++, wait_for_powerdown);
                usleep(100*1000);
            }
            setRadioState(RADIO_STATE_OFF);
            at_response_free(p_response);
            RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
            return;
#endif
        }
#if 1 //quectel
       /**
         *  Wythe modify on 2013-10-8
         *  just switch off the RF. CFUN=0 will shutdown the simcard.
         */
        err = at_send_command("AT+CFUN=4", &p_response);
#else
        err = at_send_command("AT+CFUN=0", &p_response);
#endif
       if (err < 0 || ((p_response->success == 0) && strncmp(p_response->finalResponse, "NO CARRIER", strlen("NO CARRIER")))) goto error;
        setRadioState(RADIO_STATE_OFF);
        //requestDeactivateDefaultPDP(NULL, 0, NULL);
    } else if (onOff > 0 && sState == RADIO_STATE_OFF) {
        err = at_send_command("AT+CFUN=1", &p_response);
        if (err < 0|| p_response->success == 0) {
            // Some stacks return an error when there is no SIM,
            // but they really turn the RF portion on
            // So, if we get an error, let's check to see if it
            // turned on anyway

            if (isRadioOn() != 1) {
                goto error;
            }
        }
        setRadioState(RADIO_STATE_SIM_NOT_READY);
    }

    at_response_free(p_response);
    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
	LOGE("%s end.\n", __func__);
    return;
error:
    LOGE("[%s] error, finalResponse = %s\n", __func__, p_response->finalResponse);
    at_response_free(p_response);
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
}

static void requestOrSendDataCallList(RIL_Token *t, int setup_data_call);

static void onDataCallListChanged(void *param)
{
    requestOrSendDataCallList(NULL, 0);
}

static void requestDataCallList(void *data, size_t datalen, RIL_Token t)
{
    requestOrSendDataCallList(&t, 0);
}

static void onDataCallExit(void *param) {
    if (bSetupDataCallCompelete == 0/* && ql_is_mdm9215*/) {
        int cgreg_response[4];
        quectel_at_cgreg(cgreg_response);
        if (cgreg_response[0] != 1 && cgreg_response[0] != 5) {
            int cops_response[4];
            quectel_at_cops(cops_response);
            if (cops_response[3] == 7) { //lte moe
#if 0
//??Щר????ע???Ϲ???????ֻ??????????ר??APN???????£?????ע?Ϲ?????
//???????ﲻ????????ױ????APN??Ϣ
                at_send_command("AT+CGDCONT=1,\"IPV4V6\",\"\"",  NULL);
                at_send_command("AT+CGATT=0",  NULL);
                at_send_command("AT+CGATT=1",  NULL);
#endif
           }
        }
    }
    RIL_onUnsolicitedResponse(RIL_UNSOL_DATA_CALL_LIST_CHANGED, NULL, 0);
}

static void onDeactivateDataCall(void *param) {
    if (bSetupDataCallCompelete && ql_is_mdm9215) {
        if (!strncmp(PPP_TTY_PATH, "ppp", 3))
            ql_pppd_stop(SIGTERM);
        else
            ql_ndis_stop(SIGTERM);
    }
}

static void get_local_ip(char *local_ip) {
    int inet_sock;
    struct ifreq ifr;
    char *ip = NULL;
    struct in_addr addr;

    inet_sock = socket(AF_INET, SOCK_DGRAM, 0);
    strcpy(ifr.ifr_name, PPP_TTY_PATH);

    if (ioctl(inet_sock, SIOCGIFADDR, &ifr) < 0) {
        strcpy(local_ip, "0.0.0.0");
        goto error;
    }
    memcpy (&addr, &((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr, sizeof (struct in_addr));
    ip = inet_ntoa(addr);
    strcpy(local_ip, ip);
error:
    close(inet_sock);
}

static void requestOrSendDataCallList(RIL_Token *t, int setup_data_call)
{
    ATResponse *p_response = NULL;
    ATLine *p_cur;
    int err;
    char *out;
    char propKey[PROPERTY_VALUE_MAX];
    char propValue[PROPERTY_VALUE_MAX];
    char address[64] = "\0";
    char dnses[128] = "\0";
    char gateways[64] = "\0";

    RIL_Data_Call_Response_v6 *responses = (RIL_Data_Call_Response_v6 *) malloc(sizeof(RIL_Data_Call_Response_v6) + 128);

    responses[0].status = -1;
    responses[0].suggestedRetryTime = -1;
    responses[0].cid = 1;
    responses[0].active = 0;
    responses[0].type = (char *)"IP";
    responses[0].ifname = (char *)PPP_TTY_PATH;
    responses[0].addresses = (char *)"";
    responses[0].dnses = (char *)"";
    responses[0].gateways = (char *)"";

    err = at_send_command_multiline ("AT+CGACT?", "+CGACT:", &p_response);
    if (err != 0 || p_response == NULL || p_response->success == 0) {
        if (ql_is_device_and_sim_both_support_cdma) { //cdma donot support this at
             responses[0].cid = s_default_pdp;
             responses[0].active = 0;
             get_local_ip(propValue);
             if (strcmp(propValue, "0.0.0.0")) {
                responses[0].active = 1;
            }
            goto __skip_CGACT;
        }
        if (t != NULL)
            RIL_onRequestComplete(*t, RIL_E_GENERIC_FAILURE, NULL, 0);
        else
            RIL_onUnsolicitedResponse(RIL_UNSOL_DATA_CALL_LIST_CHANGED,
                                      NULL, 0);
        return;
    }

    for (p_cur = p_response->p_intermediates; p_cur != NULL; p_cur = p_cur->p_next) {
        char *line = p_cur->line;
        int cid;

        err = at_tok_start(&line);
        if (err < 0)
            goto error;

        err = at_tok_nextint(&line, &cid);
        if (err < 0)
            goto error;

        if (cid != s_default_pdp)
            continue;

        responses[0].cid = cid;

        err = at_tok_nextint(&line, &responses[0].active);
        if (err < 0)
            goto error;

#if 1 //EC20 bug, if use same profile settings for pdp 1 & 5 and active pdp 5, but AT+CGACT? show pdp 5 is de-active
        if ((ql_is_mdm9215 || ql_is_mdm9x07) && responses[0].active == 0) {
             get_local_ip(propValue);
             if (strcmp(propValue, "0.0.0.0")) {
                responses[0].active = 1;
            }
        }
#endif
    }

__skip_CGACT:

    at_response_free(p_response);

    err = at_send_command_multiline ("AT+CGDCONT?", "+CGDCONT:", &p_response);
    if (err != 0 || p_response == NULL || p_response->success == 0) {
        if (t != NULL)
            RIL_onRequestComplete(*t, RIL_E_GENERIC_FAILURE, NULL, 0);
        else
            RIL_onUnsolicitedResponse(RIL_UNSOL_DATA_CALL_LIST_CHANGED,
                                      NULL, 0);
        return;
    }

    for (p_cur = p_response->p_intermediates; p_cur != NULL; p_cur = p_cur->p_next) {
        char *line = p_cur->line;
        int cid;

        err = at_tok_start(&line);
        if (err < 0)
            goto error;

        err = at_tok_nextint(&line, &cid);
        if (err < 0)
            goto error;

        if (cid != s_default_pdp)
            continue;

        // Assume no error
        responses[0].status = 0;

        // type
        err = at_tok_nextstr(&line, &out);
        if (err < 0)
            goto error;
        responses[0].type = alloca(strlen(out) + 1);
        strcpy(responses[0].type, out);

        // APN ignored for v5
        err = at_tok_nextstr(&line, &out);
        if (err < 0)
            goto error;
    }

    at_response_free(p_response);

    responses[0].ifname = (char *)PPP_TTY_PATH;

    get_local_ip(address);
    responses[0].addresses = address;

    if (responses[0].active == 1 && strcmp(address, "0.0.0.0")) {
        responses[0].dnses = dnses;
        sprintf(propKey, "net.%s.dns1", PPP_TTY_PATH);
        if (property_get(propKey, propValue, NULL) <= 0) {
            sprintf(propKey, "net.%s.dns1", "gprs");
            property_get(propKey, propValue, "8.8.8.8");
        }
        strcpy(responses[0].dnses, propValue);
        strcat(responses[0].dnses, " ");

        if (ql_is_mdm9215 && !strcmp(propValue, "202.96.128.86")) {
            static int bad_guangdong_dianxin_dns = 1;
            if (bad_guangdong_dianxin_dns-- > 0) {
                RIL_requestTimedCallback (onDeactivateDataCall, NULL, &TIMEVAL_3);
            }
        }

        sprintf(propKey, "net.%s.dns2", PPP_TTY_PATH);
        if (property_get(propKey, propValue, NULL) <= 0) {
            sprintf(propKey, "net.%s.dns2", "gprs");
            property_get(propKey, propValue, "8.8.4.4");
        }
        strcat(responses[0].dnses, propValue);

        responses[0].gateways = gateways;
        sprintf(propKey, "net.%s.gw", PPP_TTY_PATH);
        if (property_get(propKey, propValue, NULL) <= 0) {
            sprintf(propKey, "net.%s.remote-ip", PPP_TTY_PATH);
            if (property_get(propKey, propValue, NULL) <= 0) {
                sprintf(propKey, "net.%s.gw", "gprs");
                if (property_get(propKey, propValue, NULL) <= 0) {
                    sprintf(propKey, "net.%s.remote-ip", "gprs");
                    property_get(propKey, propValue, "0.0.0.0"); //quectel, gateways is no mean for wwan
                }
            }
        }
        strcpy(responses[0].gateways, propValue);

        LOGD("addresses: %s", responses[0].addresses);
        LOGD("dnses: %s", responses[0].dnses);
        LOGD("gateways: %s", responses[0].gateways);
   } else {
        responses[0].active = 0;
        responses[0].status= -1;
   }

    if (t != NULL) {
        int responses_len = sizeof(RIL_Data_Call_Response_v6);
#if RIL_VERSION >= 10
        if (s_callbacks.version == 10) {
            ((RIL_Data_Call_Response_v9 *)responses)->pcscf = 0;
            responses_len = sizeof(RIL_Data_Call_Response_v9);
        }
#if RIL_VERSION >= 11
	else if (s_callbacks.version > 10) {
            ((RIL_Data_Call_Response_v11 *)responses)->pcscf = 0;
            ((RIL_Data_Call_Response_v11 *)responses)->mtu = 0;
            responses_len = sizeof(RIL_Data_Call_Response_v11);
        }
#endif
#endif
        if (QL_RIL_VERSION < 6) {
            if (setup_data_call) {
                char *response_v4[5];

                response_v4[0] = "1"; //the Connection ID, CID, which is generated by RIL.
                response_v4[1] = responses[0].ifname; //the network interface name.
                response_v4[2] = responses[0].addresses;
                response_v4[3] = responses[0].dnses;
                response_v4[4] = responses[0].gateways;
                
                RIL_onRequestComplete(*t, RIL_E_SUCCESS, response_v4, sizeof(response_v4));
            } else {
                RIL_Data_Call_Response_v4 response_v4;

                response_v4.cid = responses[0].cid;
                response_v4.active= responses[0].active;
                response_v4.type= responses[0].type;
                response_v4.apn = NULL;
                response_v4.address = responses[0].addresses;

                RIL_onRequestComplete(*t, RIL_E_SUCCESS, &response_v4, sizeof(response_v4));
           }
        } else {
            RIL_onRequestComplete(*t, RIL_E_SUCCESS, responses, responses_len);
        }
    }
    else
        RIL_onUnsolicitedResponse(RIL_UNSOL_DATA_CALL_LIST_CHANGED,
                                  NULL, 0);

    return;

error:
    LOGE("%s error", __func__);
    if (t != NULL)
        RIL_onRequestComplete(*t, RIL_E_GENERIC_FAILURE, NULL, 0);
    else
        RIL_onUnsolicitedResponse(RIL_UNSOL_DATA_CALL_LIST_CHANGED,
                                  NULL, 0);

    at_response_free(p_response);
}

static void requestQueryNetworkSelectionMode(
                void *data, size_t datalen, RIL_Token t)
{
    int err;
    ATResponse *p_response = NULL;
    int response = 0;
    char *line;

    err = at_send_command_singleline("AT+COPS?", "+COPS:", &p_response);

    if (err < 0 || p_response->success == 0) {
        goto error;
    }

    line = p_response->p_intermediates->line;

    err = at_tok_start(&line);

    if (err < 0) {
        goto error;
    }

    err = at_tok_nextint(&line, &response);

    if (err < 0) {
        goto error;
    }

    RIL_onRequestComplete(t, RIL_E_SUCCESS, &response, sizeof(int));
    at_response_free(p_response);
    return;
error:
    LOGE("%s error", __func__);
    at_response_free(p_response);
    LOGE("requestQueryNetworkSelectionMode must never return error when radio is on");
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
}

static void sendCallStateChanged(void *param)
{
    RIL_onUnsolicitedResponse (
        RIL_UNSOL_RESPONSE_CALL_STATE_CHANGED,
        NULL, 0);
}

static time_t ql_voice_over_usb_time = 0;    
static int ql_voice_over_usb_state = 0;
static const char *ql_voice_over_usb_path = "/system/bin/ql_voice_over_usb";
static void *ql_voice_over_usb(void *para) {
    char shell_cmd[128];
    char *mode = "usb2pcm";
    char *nmea = "/dev/ttyUSB1";
    time_t call_time = *((time_t *)para);

    if (s_ril_conf_init.Voice_Over_Usb_Mode[0])
        mode = s_ril_conf_init.Voice_Over_Usb_Mode;

    snprintf(shell_cmd, sizeof(shell_cmd), "%s -d %s -m %s", ql_voice_over_usb_path, nmea, mode);

    while (ql_voice_over_usb_state && call_time == ql_voice_over_usb_time && !access(nmea, R_OK)) {
        sleep(1);
        if (ql_voice_over_usb_state && call_time == ql_voice_over_usb_time) {
            LOGD("%s enter\n", shell_cmd);
            system(shell_cmd);
            LOGD("%s exit\n", __func__);
        }
    }

    return NULL;
}

static void ql_voice_over_usb_start(void) {
    pthread_attr_t thread_attr;
    pthread_t thread_id;

    ql_voice_over_usb_state = 1;
    pthread_attr_init(&thread_attr);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
    ql_voice_over_usb_time = time(NULL);
    pthread_create(&thread_id, &thread_attr, ql_voice_over_usb, &ql_voice_over_usb_time);
}

static void ql_voice_over_usb_stop(void) {
    if (!access(ql_voice_over_usb_path, X_OK) && ql_voice_over_usb_state) {
        char prop_value[PROPERTY_VALUE_MAX] = {'\0'};

        ql_voice_over_usb_state = 0;
        if (property_get("gsm.voice.ql-pid", prop_value, "") > 0 && prop_value[0]) {
            int pid = atoi(prop_value);
            LOGD("ql_voice_over_usb kill %d", pid);
            kill(pid, SIGTERM);
        }    
    }
}

static void requestGetCurrentCalls(void *data, size_t datalen, RIL_Token t)
{
    int err;
    ATResponse *p_response;
    ATLine *p_cur;
    int countCalls;
    int countValidCalls;
    RIL_Call *p_calls;
    RIL_Call **pp_calls;
    int i;
    int needRepoll = 0;

    err = at_send_command_multiline ("AT+CLCC", "+CLCC:", &p_response);

    if (err != 0 || p_response->success == 0) {
        ql_voice_over_usb_stop();
        RLOGE("%s response generic failure", __func__);
        at_send_command("AT+CHUP", NULL);
        RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
        return;
    }

    /* count the calls */
    for (countCalls = 0, p_cur = p_response->p_intermediates
            ; p_cur != NULL
            ; p_cur = p_cur->p_next
    ) {
        countCalls++;
    }

    /* yes, there's an array of pointers and then an array of structures */

    pp_calls = (RIL_Call **)alloca(countCalls * sizeof(RIL_Call *));
    p_calls = (RIL_Call *)alloca(countCalls * sizeof(RIL_Call));
    memset (p_calls, 0, countCalls * sizeof(RIL_Call));

    /* init the pointer array */
    for(i = 0; i < countCalls ; i++) {
        pp_calls[i] = &(p_calls[i]);
    }

    for (countValidCalls = 0, p_cur = p_response->p_intermediates
            ; p_cur != NULL
            ; p_cur = p_cur->p_next
    ) {
        err = callFromCLCCLine(p_cur->line, p_calls + countValidCalls);

        if (err != 0) {
            continue;
        }

        if (p_calls[countValidCalls].state != RIL_CALL_ACTIVE
            && p_calls[countValidCalls].state != RIL_CALL_HOLDING
        ) {
            needRepoll = 1;
        }
#if 1 //quectel
        if(p_calls[countValidCalls].isVoice)
#endif
        countValidCalls++;
    }

    LOGI("Calls=%d,Valid=%d",countCalls,countValidCalls);

    RIL_onRequestComplete(t, RIL_E_SUCCESS, pp_calls,
            countValidCalls * sizeof (RIL_Call *));

    at_response_free(p_response);

#ifdef POLL_CALL_STATE
    if (countValidCalls) {  // We don't seem to get a "NO CARRIER" message from
                            // smd, so we're forced to poll until the call ends.
#else
    if (needRepoll) {
#endif
        RIL_requestTimedCallback (sendCallStateChanged, NULL, &TIMEVAL_CALLSTATEPOLL);
    }

    // listen to the calls
    if (!access(ql_voice_over_usb_path, X_OK)) {
        int new_state = countValidCalls && (p_calls[0].state != RIL_CALL_INCOMING);
        
        if (new_state && !ql_voice_over_usb_state) {
            ql_voice_over_usb_start();
        } else if (!new_state && ql_voice_over_usb_state) {
            ql_voice_over_usb_stop();
        }
    }
    return;
error:
    RLOGD("%s error", __func__);
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    at_response_free(p_response);
}

static void requestDial(void *data, size_t datalen, RIL_Token t)
{
    RIL_Dial *p_dial;
    char *cmd;
    const char *clir;
    int ret;
    int cops_response[4] = {0};

    p_dial = (RIL_Dial *)data;

    switch (p_dial->clir) {
        case 1: clir = "I"; break;  /*invocation*/
        case 2: clir = "i"; break;  /*suppression*/
        default:
        case 0: clir = ""; break;   /*subscription default*/
    }

    quectel_at_cops(cops_response);
    if (100 == cops_response[3]) {
        asprintf(&cmd, "AT+CDV=%s", p_dial->address);
    } else {
        if(46011 == cops_response[2])
            bVoiceCallInCTNw = 1;
        asprintf(&cmd, "ATD%s%s;", p_dial->address, clir);
    }

    ret = at_send_command(cmd, NULL);

    free(cmd);

    /* success or failure is ignored by the upper layer here.
       it will call GET_CURRENT_CALLS and determine success that way */
    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
}

static void requestWriteSmsToSim(void *data, size_t datalen, RIL_Token t)
{
    RIL_SMS_WriteArgs *p_args;
    char *cmd;
    int length;
    int err;
    ATResponse *p_response = NULL;

    p_args = (RIL_SMS_WriteArgs *)data;

    length = strlen(p_args->pdu)/2;
    asprintf(&cmd, "AT+CMGW=%d,%d", length, p_args->status);

    err = at_send_command_sms(cmd, p_args->pdu, "+CMGW:", &p_response);

    if (err != 0 || p_response->success == 0) goto error;

    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
    at_response_free(p_response);

    return;
error:
    LOGE("%s error\n", __func__);
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    at_response_free(p_response);
}

static void requestHangup(void *data, size_t datalen, RIL_Token t)
{
    int *p_line;
    int err;
    ATResponse *p_response = NULL;
    char *cmd;

    p_line = (int *)data;

    // 3GPP 22.030 6.5.5
    // "Releases a specific active call X"
    asprintf(&cmd, "AT+CHLD=1%d", p_line[0]);

    if (!access(ql_voice_over_usb_path, X_OK)) { //by now, 9x07 bug, will continue output PCM on /dev/ttyUSB1
        free(cmd);
        asprintf(&cmd, "ATH");
    }

    err = at_send_command(cmd, &p_response);

    free(cmd);

    if (err || p_response == NULL || p_response->success == 0) {
        //by now, 9x07 donot support use AT+CHLD to hangup VOLTE(voice call on lte)
        if (ql_is_mdm9x07 && 46000 == quectel_cimi) {
            at_send_command("ATH", NULL);
        }
    }
    at_response_free(p_response);

    /* success or failure is ignored by the upper layer here.
       it will call GET_CURRENT_CALLS and determine success that way */
    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
}

static void requestSignalStrength(void *data, size_t datalen, RIL_Token t)
{
    ATResponse *p_response = NULL;
    int err;
    int response[2];
    char *line;
    int cdma_rssi;
    int lte_rssi = 0;
    int lte_rsrp = 0;
    int lte_sinr = 0;
    int lte_rsrq = 0;
    ATLine *p_cur = NULL;
    char *tmp = NULL;

    int cops_response[4] = {0};
    RIL_SignalStrength_v6 signalStrength;
    memset(&signalStrength, 0, sizeof(RIL_SignalStrength_v6));

#define CDMA_RSSI_THRESH	125
#define CDMA_RSSI_SPAN	    (CDMA_RSSI_THRESH - 75)

    if (QL_RIL_VERSION >= 7) {
        signalStrength.LTE_SignalStrength.signalStrength = 99;
        signalStrength.LTE_SignalStrength.rsrp = 0x7FFFFFFF;
        signalStrength.LTE_SignalStrength.rsrq = 0x7FFFFFFF;
        signalStrength.LTE_SignalStrength.rssnr = 0x7FFFFFFF;
        signalStrength.LTE_SignalStrength.cqi = 0x7FFFFFFF;
    } else {
        signalStrength.LTE_SignalStrength.signalStrength = -1;
        signalStrength.LTE_SignalStrength.rsrp = -1;
        signalStrength.LTE_SignalStrength.rsrq = -1;
        signalStrength.LTE_SignalStrength.rssnr = -1;
        signalStrength.LTE_SignalStrength.cqi = -1;
    }

    if (s_ril_conf_init.LTE_SignalStrength) {
        signalStrength.LTE_SignalStrength.signalStrength = s_ril_conf_init.LTE_SignalStrength;
    }

    if (s_closed && (currentState() == RADIO_STATE_SIM_READY || currentState() == RADIO_STATE_RUIM_READY)) {
        /**
        *  In Android Jelly Bean, the invalid value for
        *  LET signalStrength should be 99 depending on the SignalStrength.java.
        */
        //#if ((PLATFORM_VERSION >= 420) || ((PLATFORM_VERSION < 100) && (PLATFORM_VERSION >= 42)))

        signalStrength.GW_SignalStrength.signalStrength = 31;
        signalStrength.GW_SignalStrength.bitErrorRate = 0;
        signalStrength.CDMA_SignalStrength.dbm = 75;
        signalStrength.CDMA_SignalStrength.ecio = 90;
        signalStrength.EVDO_SignalStrength.dbm = 65;
        signalStrength.EVDO_SignalStrength.ecio = 90;
        signalStrength.EVDO_SignalStrength.signalNoiseRatio = 8;
        RIL_onRequestComplete(t, RIL_E_SUCCESS, &signalStrength, sizeof(RIL_SignalStrength_v6));
        return;
    }

    err = at_send_command_singleline("AT+CSQ", "+CSQ:", &p_response);

    if (err < 0 || p_response->success == 0) {
        RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
        goto error;
    }

    line = p_response->p_intermediates->line;

    err = at_tok_start(&line);
    if (err < 0) goto error;

    err = at_tok_nextint(&line, &(response[0]));
    if (err < 0) goto error;

    err = at_tok_nextint(&line, &(response[1]));
    if (err < 0) goto error;

#if 1 //quectel
{
    /**
     *  In Android Jelly Bean, the invalid value for
     *  LET signalStrength should be 99 depending on the SignalStrength.java.
     */
//#if ((PLATFORM_VERSION >= 420) || ((PLATFORM_VERSION < 100) && (PLATFORM_VERSION >= 42)))

    signalStrength.GW_SignalStrength.signalStrength = response[0];
    signalStrength.GW_SignalStrength.bitErrorRate = response[1];

    /**
     *	Fill the cdma rssi value here. When we fill the actual rssi with 75, the
     *  SignalStrength.java will deal with it as -75
     */
    cdma_rssi = CDMA_RSSI_THRESH - ((response[0] * CDMA_RSSI_SPAN) / 31); // Change it following the modem code.

    signalStrength.CDMA_SignalStrength.dbm = cdma_rssi;
    signalStrength.CDMA_SignalStrength.ecio = cdma_rssi;
    signalStrength.EVDO_SignalStrength.dbm = cdma_rssi;
    signalStrength.EVDO_SignalStrength.ecio = cdma_rssi;

 // these codes used to report LTE SignalStrength, by default, donot not report LTE SignalStrength
    if(s_ril_conf_init.LTE_Is_Report_SignalStrength) {
    quectel_at_cops(cops_response);
    if (cops_response[3] == 7) { //lte mode
        lte_rssi = signalStrength.LTE_SignalStrength.signalStrength = (response[0] <= 31) ? response[0]*63/31 : response[0];
        lte_rsrp = 140 - lte_rssi;
        signalStrength.LTE_SignalStrength.rsrp = lte_rsrp;
        at_response_free(p_response);
        err = at_send_command_multiline("AT+QCSQ", "+QCSQ:", &p_response);
        if ((0 == err) && p_response && (0 != p_response->success)) {
            for (p_cur = p_response->p_intermediates; NULL != p_cur; p_cur = p_cur->p_next) {
                line = p_cur->line;
                err = at_tok_start(&line);
                if (err < 0) {
                    goto cqsq_err;
                }
                tmp = NULL;
                err = at_tok_nextstr(&line, &tmp);
                if (err < 0) {
                    goto cqsq_err;
                }
                if (!strncmp(tmp, "LTE", strlen("LTE"))) {
                    err = at_tok_nextint(&line, &lte_rssi);
                    if (err < 0) {
                        goto cqsq_err;
                    }
                    //signalStrength.LTE_SignalStrength.signalStrength = -lte_rssi;
                    err = at_tok_nextint(&line, &lte_rsrp);
                    if (err < 0) {
                        goto cqsq_err;
                    }
                    signalStrength.LTE_SignalStrength.rsrp = -lte_rsrp;
                    err = at_tok_nextint(&line, &lte_sinr);
                    if (err < 0) {
                        goto cqsq_err;
                    }
                    signalStrength.LTE_SignalStrength.rssnr = lte_sinr;

                    err = at_tok_nextint(&line, &lte_rsrq);
                    if (err < 0) {
                        goto cqsq_err;
                    }
                    signalStrength.LTE_SignalStrength.rsrq = -lte_rsrq;

                    break;
                } else {
                    continue;
                }
            }
        }
        signalStrength.GW_SignalStrength.signalStrength = -1;
        signalStrength.GW_SignalStrength.bitErrorRate = -1;
        signalStrength.CDMA_SignalStrength.dbm = -1;
        signalStrength.CDMA_SignalStrength.ecio = -1;
        signalStrength.EVDO_SignalStrength.dbm = -1;
        signalStrength.EVDO_SignalStrength.ecio = -1;
        signalStrength.EVDO_SignalStrength.signalNoiseRatio = -1;
    }
 }

cqsq_err:

    if (QL_RIL_VERSION < 6) {
        RIL_SignalStrength_v5 response_v5;
        response_v5.GW_SignalStrength = signalStrength.GW_SignalStrength;
        response_v5.CDMA_SignalStrength = signalStrength.CDMA_SignalStrength;
        response_v5.EVDO_SignalStrength = signalStrength.EVDO_SignalStrength;

        if (NULL == t)
            RIL_onUnsolicitedResponse(RIL_UNSOL_SIGNAL_STRENGTH, &response_v5, sizeof(RIL_SignalStrength_v5));
        else
            RIL_onRequestComplete(t, RIL_E_SUCCESS, &response_v5, sizeof(RIL_SignalStrength_v5));
    } else {
        if (NULL == t)
            RIL_onUnsolicitedResponse(RIL_UNSOL_SIGNAL_STRENGTH, &signalStrength, sizeof(RIL_SignalStrength_v6));
        else
            RIL_onRequestComplete(t, RIL_E_SUCCESS, &signalStrength, sizeof(RIL_SignalStrength_v6));
    }
}
#else
    RIL_onRequestComplete(t, RIL_E_SUCCESS, response, sizeof(response));
#endif

#if 1 // some UG95 fireware donot report +CGREG/+CREG when network state change
//9x07 on cdmd/evdo mode also donot report report +CGREG/+CREG when network state change
//    if (ql_is_UG95 || (ql_is_mdm9x07 && ql_is_device_and_sim_both_support_cdma)) {
//    if (ql_is_mdm9215 || ql_is_UG95 || ql_is_mdm9x07) 
    {
        static int old_cgreg_response[4];
        int new_cgreg_response[4];
        quectel_at_cgreg(new_cgreg_response);
        if (g_card_state_change_flg || memcmp(new_cgreg_response, old_cgreg_response, sizeof(old_cgreg_response))) {
            memcpy(old_cgreg_response, new_cgreg_response, sizeof(old_cgreg_response));
            RIL_onUnsolicitedResponse (RIL_UNSOL_RESPONSE_VOICE_NETWORK_STATE_CHANGED,NULL, 0);
            g_card_state_change_flg = 0;
        }
    }
#endif

    at_response_free(p_response);
    return;

error:
    LOGE("requestSignalStrength must never return an error when radio is on");
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    at_response_free(p_response);
}

static int s_lastRegistrationState[4]; //stat, lac, ci, Act
static void requestRegistrationState(int request, void *data,
                                        size_t datalen, RIL_Token t)
{
    int err;
    int response[14] = {0, 0, 0, 0};
    char * responseStr[14];
    ATResponse *p_response = NULL;
    const char *cmd;
    const char *prefix;
    char *line, *p;
    int commas;
    int skip;
    int count = 3;
    int cops_response[4];
    int i;

    if (request == RIL_REQUEST_VOICE_REGISTRATION_STATE) {
        cmd = "AT+CREG?";
        prefix = "+CREG:";
    } else if (request == RIL_REQUEST_DATA_REGISTRATION_STATE) {
        cmd = "AT+CGREG?";
        prefix = "+CGREG:";
    } else {
        assert(0);
        goto error;
    }

#if 1 //quectel
__requestRegistrationState_restart:
    skip = 2; //unsolicited result code with location information +CGREG: 2,<stat>[,<lac>,<ci>[,<Act>]]
#endif
    err = at_send_command_singleline(cmd, prefix, &p_response);

    if (err != 0) goto error;

    line = p_response->p_intermediates->line;

    err = at_tok_start(&line);
    if (err < 0) goto error;

    /* Ok you have to be careful here
     * The solicited version of the CREG response is
     * +CREG: n, stat, [lac, cid]
     * and the unsolicited version is
     * +CREG: stat, [lac, cid]
     * The <n> parameter is basically "is unsolicited creg on?"
     * which it should always be
     *
     * Now we should normally get the solicited version here,
     * but the unsolicited version could have snuck in
     * so we have to handle both
     *
     * Also since the LAC and CID are only reported when registered,
     * we can have 1, 2, 3, or 4 arguments here
     *
     * finally, a +CGREG: answer may have a fifth value that corresponds
     * to the network type, as in;
     *
     *   +CGREG: n, stat [,lac, cid [,networkType]]
     */

    /* count number of commas */
    commas = 0;
    for (p = line ; *p != '\0' ;p++) {
        if (*p == ',') commas++;
    }

    switch (commas) {
        case 0: /* +CREG: <stat> */
            err = at_tok_nextint(&line, &response[0]);
            if (err < 0) goto error;
            response[1] = -1;
            response[2] = -1;
        break;

        case 1: /* +CREG: <n>, <stat> */
            err = at_tok_nextint(&line, &skip);
            if (err < 0) goto error;
            err = at_tok_nextint(&line, &response[0]);
            if (err < 0) goto error;
            response[1] = -1;
            response[2] = -1;
            if (err < 0) goto error;
#if 1 //quectel
            if (/*ql_is_UC20 &&*/ (request == RIL_REQUEST_DATA_REGISTRATION_STATE) && s_lastRegistrationState[3]) {
                quectel_at_cops(cops_response);
                if (cops_response[3]) {
                    response[0] = s_lastRegistrationState[0];
                    response[1] = s_lastRegistrationState[1];
                    response[2] = s_lastRegistrationState[2];
                    response[3] = cops_response[3];
                    count = 4;
                }
            }
#if 1 //????EC21 ��?{g ????ʱ????ʾͼ??
//nwscanmodeex set to gsm|cdma, then first use cdma sim card, then switch to unicom sim card
            else if (ql_is_mdm9x07&& (request == RIL_REQUEST_DATA_REGISTRATION_STATE) && (1 == response[0])) {
                response[1] = 0x2540;
                response[2] = 0x79CD;
                quectel_at_cops(cops_response);
                response[3] = cops_response[3];
                count = 4;
            }
#endif
#endif
        break;

        case 2: /* +CREG: <stat>, <lac>, <cid> */
            err = at_tok_nextint(&line, &response[0]);
            if (err < 0) goto error;
            err = at_tok_nexthexint(&line, &response[1]);
            if (err < 0) goto error;
            err = at_tok_nexthexint(&line, &response[2]);
            if (err < 0) goto error;
        break;
        case 3: /* +CREG: <n>, <stat>, <lac>, <cid> */
            err = at_tok_nextint(&line, &skip);
            if (err < 0) goto error;
            err = at_tok_nextint(&line, &response[0]);
            if (err < 0) goto error;
            err = at_tok_nexthexint(&line, &response[1]);
            if (err < 0) goto error;
            err = at_tok_nexthexint(&line, &response[2]);
            if (err < 0) goto error;
            if (((response[0] == 1) || (response[0] == 5)) && ql_is_GSM) {
                response[3] = 0; //force RADIO_TECH_GPRS
                count = 4;
            }
        break;
        /* special case for CGREG, there is a fourth parameter
         * that is the network type (unknown/gprs/edge/umts)
         */
        case 4: /* +CGREG: <n>, <stat>, <lac>, <cid>, <networkType> */
            err = at_tok_nextint(&line, &skip);
            if (err < 0) goto error;
            err = at_tok_nextint(&line, &response[0]);
            if (err < 0) goto error;
            err = at_tok_nexthexint(&line, &response[1]);
            if (err < 0) goto error;
            err = at_tok_nexthexint(&line, &response[2]);
            if (err < 0) goto error;
            err = at_tok_nexthexint(&line, &response[3]);
            if (err < 0) goto error;
#if 1 //quectel UC15&UC20&UG95 canot get raido tech from AT+CGREG?, what about EC20?
            if ((request == RIL_REQUEST_DATA_REGISTRATION_STATE) && (response[3] == 2)) {
                quectel_at_cops(cops_response);
                if (cops_response[3] > response[3])
                    response[3] = cops_response[3];
            }
#endif
#if 1 //quectel EC21&EC25 SRLTE voice and data network are seperated, use the higher one
            if (ql_is_mdm9x07 && (request == RIL_REQUEST_VOICE_REGISTRATION_STATE)
                && (0x100 == response[3])) {
                memset(cops_response, 0, sizeof(cops_response));
                quectel_at_cops(cops_response);
                if (7 == cops_response[3]) {
                    response[3] = 7;
                }
            }
#endif
            count = 4;
        break;
        default:
            goto error;
    }

#if 1 // using china-telecom 4G sim card, cannot register voice serice
//frameworks\base\packages\SystemUI\src\com\android\systemui\statusbar\policy\NetworkController.java updateTelephonySignalStrength()
    if (ql_is_mdm9215 && (request == RIL_REQUEST_VOICE_REGISTRATION_STATE)
        && currentDataServiceState() && (response[0] != 1) && (response[0] != 5)
        && !strcmp(cmd, "AT+CREG?")) {
        cmd = "AT+CGREG?";
        prefix = "+CGREG:";
        goto __requestRegistrationState_restart;
    }
#endif

#if 1 // For LTE mode, the response from +CREG
#define LTE_MODE_FROM_COPS	7
    if(ql_is_mdm9215 && (request == RIL_REQUEST_VOICE_REGISTRATION_STATE)
        && !currentDataServiceState() && (response[0] == 1 && response[1] == 65534) // 65534 equals 0xfffe, it's not a real lte lac
        && !strcmp(cmd, "AT+CREG?"))
    {
        if(quectel_at_cops(cops_response) == LTE_MODE_FROM_COPS) {
            cmd = "AT+CGREG?";
            prefix = "+CGREG:";
            goto __requestRegistrationState_restart;
        }
    }
#endif

#if 1 //quectel
    if (skip != 2) {
        at_response_free(p_response);
        p_response = NULL;
        at_send_command("AT+CREG=2", NULL);
        at_send_command("AT+CGREG=2", NULL);
        goto __requestRegistrationState_restart;
    }

    if (request == RIL_REQUEST_DATA_REGISTRATION_STATE) {
        setDataServiceState((response[0] == 1) || (response[0] == 5));
        if (currentDataServiceState() && !time_zone_report) {
            ATResponse *p_response = NULL;
            int err = at_send_command_singleline("AT+QLTS", "+QLTS", &p_response);
            if (!(err < 0  || p_response == NULL || p_response->success == 0)) {
                //+QLTS: "13/08/23,06:51:13+32,0"
                char *response;
                char *line = p_response->p_intermediates->line;
                err = at_tok_start(&line);
                if (err == 0)
                    err = at_tok_nextstr(&line, &response);

                if (err != 0) {
                    LOGE("invalid QLTS line %s\n", p_response->p_intermediates->line);
                } else {
                    if (response[4] == '/') //2014 -> 14
                        response += 2;
                    if (response[0] != '\0' && response[0] != '0')
                        RIL_onUnsolicitedResponse (RIL_UNSOL_NITZ_TIME_RECEIVED, response, strlen(response));
                }
            }
            at_response_free(p_response);
            time_zone_report = 1;
        }
    }

    if (count == 3) {
        if ((response[1] == -1) && (response[2] == -1)) {
            //to advoid throw <java.lang.NumberFormatException: Invalid int: "ffffffff"> in GsmServiceStateTracker.java
            count = 1;
        }
    } else if (count > 3) {
        RIL_RadioTechnology radio_tech = RADIO_TECH_UNKNOWN;
        switch (response[3]) {
               case 0: //GSM (Not support on UC20-A)
                    radio_tech = RADIO_TECH_GPRS;
                break;
                case 2: //UTRAN
                    radio_tech = RADIO_TECH_UMTS;
                break;
                case 3: //GSM W/EGPRS
                    radio_tech = RADIO_TECH_EDGE;
                break;
                case 4: //UTRAN W/HSDPA
                    radio_tech = RADIO_TECH_HSDPA;
                break;
                case 5: //UTRAN W/HSUPA
                    radio_tech = RADIO_TECH_HSUPA;
                break;
               case 6: //UTRAN W/HSDPA and HSUPA
                    radio_tech = RADIO_TECH_HSPA;
                break;
               case 7: //LTE
                    radio_tech = RADIO_TECH_LTE;
                break;
                case 0x100:
                    radio_tech= quectel_at_qnwinfo();
                    if (s_lastRegistrationState[3] != 0x100) {
                            char value[PROPERTY_VALUE_MAX];
                            quectel_at_cops(cops_response);
                            sprintf(value, "%d", cops_response[2]);
                            if (cops_response[2] == 46003) //in fact, wo should not set this prop here!
                                property_set("gsm.sim.operator.numeric", value);
                    }
                    //Add by Wythe.WANG for CDMA
                    response[4] = response[2] & 0x0000ffff;
                    response[5] = 0;
                    response[6] = 0;
                    response[7] = 0;
                    response[8] = response[1];
                    response[9] = response[2] & 0xffff0000;
                    response[10] = 0;
                    response[11] = 0;
                    response[12] = 0;
                    response[13] = 0;

                    if (!s_ril_conf_init.support_CDMPhone) {
                        if (radio_tech == RADIO_TECH_1xRTT)
                            radio_tech = RADIO_TECH_GPRS;
                        else
                            radio_tech = RADIO_TECH_UMTS;
                    }
//GsmServiceStateTracker.java CdmaServiceStateTracker.java CdmaLteServiceStateTracker.java
//handle EVENT_POLL_STATE_GPRS
//for cmda, 5.1 always use EVENT_POLL_STATE_GPRS
                    if (ql_is_device_and_sim_both_support_cdma && ql_cdmaphone)
                        count = 14;
                break;
                default:
                    radio_tech = RADIO_TECH_UNKNOWN;
                break;
        }
        response[3] = radio_tech;
    }
#endif

    if (ql_is_UC20 && 5 == response[0] && 46006 == quectel_cimi) {//roaming
        quectel_at_cops(cops_response);
        LOGD("[%s:%d] quectel_cimi = %d, cops_rsp[2] = %d.\n", __func__, __LINE__, quectel_cimi, cops_response[2]);
        if (46001 == cops_response[2]) {
            response[0] = 1;
        }
    }

    if (request == RIL_REQUEST_DATA_REGISTRATION_STATE) {
        s_lastRegistrationState[0] = response[0];
        s_lastRegistrationState[1] = response[1];
        s_lastRegistrationState[2] = response[2];
        s_lastRegistrationState[3] = response[3];
    }

    if (QL_RIL_VERSION < 6) {
        if (response[3] > RADIO_TECH_UMTS)
            response[3] = RADIO_TECH_UMTS;
        if (count > 4)
            count = 4;
    }

    for (i = 0; i < count; i++) {
        if (i == 1 || i == 2)
            asprintf(&responseStr[i], "%x",  response[i]);
        else
            asprintf(&responseStr[i], "%d",  response[i]);
    }
    RIL_onRequestComplete(t, RIL_E_SUCCESS, responseStr, count * sizeof(char *));
    for (i = 0; i < count; i++)
        free(responseStr[i]);
    at_response_free(p_response);

    return;
error:
    LOGE("requestRegistrationState must never return an error when radio is on");
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    at_response_free(p_response);
}

static void requestOperator(void *data, size_t datalen, RIL_Token t)
{
    int err;
    int i;
    int skip;
    ATLine *p_cur;
    char *response[3];

#if 1 //quectel
__requestOperator_restart:
#endif
    memset(response, 0, sizeof(response));

    ATResponse *p_response = NULL;

    err = at_send_command_multiline(
        "AT+COPS=3,0;+COPS?;+COPS=3,1;+COPS?;+COPS=3,2;+COPS?",
        "+COPS:", &p_response);

    /* we expect 3 lines here:
     * +COPS: 0,0,"T - Mobile"
     * +COPS: 0,1,"TMO"
     * +COPS: 0,2,"310170"
     */

    if (err != 0) goto error;

    for (i = 0, p_cur = p_response->p_intermediates
            ; p_cur != NULL
            ; p_cur = p_cur->p_next, i++
    ) {
        char *line = p_cur->line;

        err = at_tok_start(&line);
        if (err < 0) goto error;

        err = at_tok_nextint(&line, &skip);
        if (err < 0) goto error;

        // If we're unregistered, we may just get
        // a "+COPS: 0" response
        if (!at_tok_hasmore(&line)) {
            response[i] = NULL;
            continue;
        }

        err = at_tok_nextint(&line, &skip);
        if (err < 0) goto error;

        // a "+COPS: 0, n" response is also possible
        if (!at_tok_hasmore(&line)) {
            response[i] = NULL;
            continue;
        }

        err = at_tok_nextstr(&line, &(response[i]));
        if (err < 0) goto error;
    }

    if (i != 3) {
        /* expect 3 lines exactly */
        goto error;
    }

#if 1 //quectel
    if (!response[0] && !response[1] && !response[2] && (network_debounce_time > 0)) {
        sleep(1);
        network_debounce_time--;
        at_response_free(p_response);
        goto __requestOperator_restart;
    }
    network_debounce_time = 0;
#endif

    RIL_onRequestComplete(t, RIL_E_SUCCESS, response, sizeof(response));
    at_response_free(p_response);

    return;
error:
    LOGE("requestOperator must not return error when radio is on");
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    at_response_free(p_response);
}

static void requestSendSMS(void *data, size_t datalen, RIL_Token t)
{
    int err;
    const char *smsc;
    const char *pdu;
    int tpLayerLength;
    char *cmd1 = NULL, *cmd2 = NULL;
    RIL_SMS_Response response;
    ATResponse *p_response = NULL;

    smsc = ((const char **)data)[0];
    pdu = ((const char **)data)[1];

    tpLayerLength = strlen(pdu)/2;

    // "NULL for default SMSC"
    if (smsc == NULL) {
        smsc= "00";
    }

    asprintf(&cmd1, "AT+CMGS=%d", tpLayerLength);
    asprintf(&cmd2, "%s%s", smsc, pdu);

    err = at_send_command_sms(cmd1, cmd2, "+CMGS:", &p_response);
    free(cmd1);
    free(cmd2);

    if (err != 0 || p_response->success == 0) goto error;

    memset(&response, 0, sizeof(response));

    /* FIXME fill in messageRef and ackPDU */

    RIL_onRequestComplete(t, RIL_E_SUCCESS, &response, sizeof(response));
    at_response_free(p_response);

    return;
error:
    LOGE("%s error", __func__);
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    at_response_free(p_response);
}

typedef unsigned char uint8;
#define ASSERT(__s) do {} while(0)
#define QL_CDMASMS_TRACE(lvl, fmt,...) LOGD(fmt,##__VA_ARGS__)
#include "ril_cdma_sms.c"

static void requestSendCdmaSMS(void *data, size_t datalen, RIL_Token t)
{
    int err;
    RIL_SMS_Response response;
    ATResponse *p_response = NULL;
    RIL_CDMA_SMS_Message rcsm;
    char tmp[1024];
    int i;
    unsigned char *cdma_pdu;
    unsigned short pdu_len;
    char *cmd1 = NULL, *cmd2 = NULL;

    memcpy(&rcsm, data, datalen);

    LOGD("uTeleserviceID: %d", rcsm.uTeleserviceID);
    LOGD("bIsServicePresent: %d", rcsm.bIsServicePresent);
    LOGD("uServicecategory: %d", rcsm.uServicecategory);

    LOGD("sAddress.digit_mode: %d", rcsm.sAddress.digit_mode);
    LOGD("sAddress.number_mode: %d", rcsm.sAddress.number_mode);
    LOGD("sAddress.number_type: %d", rcsm.sAddress.number_type);
    LOGD("sAddress.number_plan: %d", rcsm.sAddress.number_plan);
    LOGD("sAddress.number_of_digits: %d", rcsm.sAddress.number_of_digits);
    for (i = 0; i < rcsm.sAddress.number_of_digits; i++)
        sprintf(&tmp[i*3], "%02x ", rcsm.sAddress.digits[i]);
    LOGD("sAddress.digits: %s", tmp);

    LOGD("sSubAddress.subaddressType: %d", rcsm.sSubAddress.subaddressType);
    LOGD("sSubAddress.odd: %d", rcsm.sSubAddress.odd);
    LOGD("sSubAddress.number_of_digits: %d", rcsm.sSubAddress.number_of_digits);
    for (i = 0; i < rcsm.sSubAddress.number_of_digits; i++)
        sprintf(&tmp[i*3], "%02x ", rcsm.sSubAddress.digits[i]);
    LOGD("sSubAddress.digits: %s", tmp);

    LOGD("uBearerDataLen: %d", rcsm.uBearerDataLen);
    for (i = 0; i < rcsm.uBearerDataLen; i++)
        sprintf(&tmp[i*3], "%02x ", rcsm.aBearerData[i]);
    LOGD("aBearerData: %s", tmp);

    memset(&response, 0, sizeof(response));

    /* FIXME fill in messageRef and ackPDU */

    cdma_pdu = (unsigned char *)QL_RIL_CDMASMS_sEncodePDUP2P(1, data, &pdu_len);
    if (cdma_pdu == NULL || pdu_len == 0)
        goto error;
    cdma_pdu[pdu_len] = 0;
    for (i = 0; i < pdu_len; i++)
        sprintf(&tmp[i*2], "%02x", cdma_pdu[i]);

    asprintf(&cmd1, "AT+QSMSS=%d", pdu_len);
    //asprintf(&cmd2, "%s", cdma_pdu);

    err = at_send_command_sms(cmd1, tmp, "+QSMSS:", &p_response);
    free(cmd1);
    free(cmd2);

    if (err != 0 || p_response->success == 0) goto error;

    memset(&response, 0, sizeof(response));


    RIL_onRequestComplete(t, RIL_E_SUCCESS, &response, sizeof(response));
    at_response_free(p_response);

    return;
error:
    LOGE("%s error", __func__);
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    at_response_free(p_response);
}

/*
 * RIL_REQUEST_SEND_SMS_EXPECT_MORE
 */
void requestSendSMSExpectMore(void *data, size_t datalen, RIL_Token t)
{
        (void) data; (void) datalen;

    at_send_command("AT+CMMS=1",NULL);

        requestSendSMS(data, datalen, t);
}

int notifyDataCallProcessExit(void) {
#if 1 //(QL_RIL_VERSION > 9) //lollipop
    if (bSetupDataCallCompelete) {
        RIL_requestTimedCallback (onDataCallExit, NULL, NULL);
        bSetupDataCallCompelete = 0;
        return 1;
    } else {
        nSetupDataCallFailTimes++;
        if (nSetupDataCallFailTimes > 3)
            return 1;
    }
#endif
    return 0;
}

//update:Joe.Wang 2014-1-13
static void requestSetupDataCall(void *data, size_t datalen, RIL_Token t)
{
    const char *apn;
    const char *user = NULL;
    const char *pass = NULL;
    const char *auth_type = NULL;
    const char *pdp_type = "IP";
    char ppp_number[20] = {'\0'};
    int cgreg_response[4];
    int cops_response[4];
    char *cmd;
    int err;
    int retry = 0;
    pid_t ql_pppd_pid;
    ATResponse *p_response = NULL;
    ATResponse *p_pwd_response = NULL;
    char *line = NULL;
    char ppp_local_ip[PROPERTY_VALUE_MAX] = {'\0'};
    struct timeval begin_tv, end_tv;
    gettimeofday(&begin_tv, NULL);

#ifdef QUECTEL_RESTART_RIL_WHEN_NO_SETUP_DATA_CALL
    ql_setup_data_call++;
#endif

    apn = ((const char **)data)[2];
    if (datalen > 3 * sizeof(char *))
        user = ((char **)data)[3];
    if (datalen > 4 * sizeof(char *))
        pass = ((char **)data)[4];
    if (datalen > 5 * sizeof(char *))
        auth_type = ((const char **)data)[5]; // 0 ~ NONE, 1 ~ PAP, 1 ~ CHAP, 3 ~ PAP / CHAP
    if (datalen > 6 * sizeof(char *))
        pdp_type = ((const char **)data)[6];

    LOGI("*************************************");
    LOGI("USER:%s",user);
    LOGI("PASS:%s",pass);
    LOGI("auth_type:%s",auth_type);
    LOGI("pdp_type:%s",pdp_type);
    LOGI("*************************************");

#if 1 // quectel Set DTR Function Mode
//ON->OFF on DTR: Disconnect data call, change to command mode. During state DTR = OFF, auto-answer function is disabled
    at_send_command("AT&D2", NULL);
#endif
#ifdef USB_HOST_USE_RESET_RESUME
    if (!ql_mux_enabled)
        at_send_command("AT&D0", NULL);
#endif

    bSetupDataCallCompelete = 0;
    nSetupDataCallFailTimes = 0;
     //Make sure there is no existing connection or pppd instance running
    if (!strncmp(PPP_TTY_PATH, "ppp", 3))
        ql_pppd_stop(SIGTERM);
    else
        ql_ndis_stop(SIGTERM);

    if (isRadioOn() != 1) {
        LOGE("raido is off!");
        goto error;
    }

    quectel_at_cgreg(cgreg_response);
    quectel_at_cops(cops_response);

    if (cgreg_response[0] != 1 && cgreg_response[0] != 5) {
        if (ql_is_mdm9215) {
            if (cops_response[3] == 7) { //lte moe
#if 0
//??Щר????ע???Ϲ???????ֻ??????????ר??APN???????£?????ע?Ϲ?????
//???????ﲻ????????ױ????APN??Ϣ
                at_send_command("AT+CGDCONT=1,\"IPV4V6\",\"\"",  NULL);
                at_send_command("AT+CGATT=0",  NULL);
                at_send_command("AT+CGATT=1",  NULL);
#endif
           }
        }
//????CGREG??ʾ???�����??COPS??ʾע?????ǿ?????ģ????bug?ˣ?UC15/UC20/EC20???����?????????
//???????￪???·???ģʽ??ϣ??ģ???ܹ??ָ????]
        //if (ql_is_UC20)
        {
            if (cops_response[3]/* == 6*/) {
                at_send_command("AT+CFUN=4",  NULL);
                sleep(3);
                at_send_command("AT+CFUN=1",  NULL);
           }
        }
        RIL_onUnsolicitedResponse (RIL_UNSOL_RESPONSE_VOICE_NETWORK_STATE_CHANGED, NULL, 0);
        LOGE("gprsState is out of service!");
        goto error;
    }

//this informations should set by user, it is not a good idea to fix here
    if ((cgreg_response[3] == 0x100) || (cops_response[3] == 100)) { //cdma
#if 0 //if no username nor passwd, use the username and passwd in module preferentially
        err = at_send_command_singleline("AT+QCTPWDCFG", "+QCTPWDCFG:", &p_pwd_response);
        if (err < 0 || !p_pwd_response || p_pwd_response->success == 0) {
            LOGE("%s: send command QCTPWDCFG failed!!!\n", __func__);
        } else if (!(p_pwd_response->p_intermediates) || !(p_pwd_response->p_intermediates->line)) {
            LOGE("%s: null pointer -> p_pwd_response->[p_intermediates|line]!\n", __func__);
        } else {
            line = p_pwd_response->p_intermediates->line;
            err = at_tok_start(&line);
            if (err < 0) {
                LOGE("%s:%d at_tok_start failed!!!\n", __func__, __LINE__);
                line = NULL;
            }
            LOGD("%s: send command QCTPWDCFG successed!\n", __func__);
        }

        if ((!user || !user[0]) && line) {
            err = at_tok_nextstr(&line, (char **)&user);
            if (err < 0) {
                LOGE("%s: get user from at resp failed!!!\n", __func__);
                user = NULL;
            } else {
                LOGD("got user : %s\n", user);
            }
            if (!pass || !pass[0]) {
                err = at_tok_nextstr(&line, (char **)&pass);
                if (err < 0) {
                    LOGE("%s: get password from at resp failed!!!\n", __func__);
                    pass = NULL;
                } else {
                    LOGD("got pass : %s\n", pass);
                }
             }
        }
#endif

        if (!user || !user[0])
            user = "ctnet@mycdma.cn";
        if (!pass || !pass[0])
            pass="vnet.mobi";
        if (!ppp_number[0])
            strcpy(ppp_number, "#777");
        if (!auth_type || auth_type[0] == '0')
            auth_type = "2"; //chap

        at_send_command("at+qcfg=\"cdmaruim\", 1", NULL); //use user&paswd from android, not from uimcard
        asprintf(&cmd, "at+qctpwdcfg=\"%s\",\"%s\"", user, pass);
        err = at_send_command(cmd, NULL);
        free(cmd);

        if (ql_is_mdm9215 && s_lastPreferredNetworkType != PREF_NET_TYPE_CDMA_ONLY) {
            int wait_for_evdo = 5; //ppp will get fail or wrong dns
            while (wait_for_evdo-- > 0 && quectel_at_sysinfo() == RADIO_TECH_1xRTT)
                sleep(1);
        }

    	if (ql_is_mdm9215 && bVoiceCallInCTNw == 1) {
            int wait_for_lte_from_cdma = 15; //wait for UE up back to LTE from fallback voice call
            while (wait_for_lte_from_cdma-- > 0 && quectel_at_qnwinfo() != RADIO_TECH_LTE)
            {
                LOGD("wait for UE back to LTE in %d times", wait_for_lte_from_cdma);
                sleep(1);
            }
            bVoiceCallInCTNw = 0;
        }
    }

    s_default_pdp = 1;
    if (cops_response[3] == 7) //lte moe
        s_default_pdp = CONFIG_DEFAULT_PDP;

    if (ql_is_XX("EC21VF") || ql_is_XX("EC25VF")) {
        LOGD("America Verizon force use cid 3 to setup data call");
        s_default_pdp = 3;
    }

    if (!ppp_number[0])
        snprintf(ppp_number, sizeof(ppp_number), "*99***%d#", s_default_pdp);

    LOGD("requesting data connection to APN '%s'!\n", apn);

    if ((cgreg_response[3] == 0x100) || (cops_response[3] == 100)) { //cdma
    } else {
        //????ר??????Ϣ??ģ???У?ʹ???´?????ģ?飬??ע????ר??
        asprintf(&cmd, "AT+QICSGP=%d,1, \"%s\",\"%s\",\"%s\",%s;+QICSGP=%d",
            s_default_pdp, apn, user?user:"", pass?pass:"",auth_type?auth_type:"0", s_default_pdp);
        at_send_command(cmd, NULL);
        free(cmd);
    }

    asprintf(&cmd, "AT+CGDCONT=%d,\"%s\",\"%s\"", s_default_pdp, pdp_type, apn);

    //FIXME check for error here
    err = at_send_command(cmd, NULL);
    free(cmd);

    PPP_TTY_PATH = "ppp0";
    /* start the gprs pppd */
    if (ql_mux_enabled)
        ql_pppd_pid = ql_pppd_start(CMUX_PPP_PORT, user, pass, auth_type, ppp_number);
    else {
        static char usbnet_adapeter[32];
        if ((ql_is_UC20 || ql_is_mdm9215 || ql_is_mdm9x07) && ql_get_ndisname(usbnet_adapeter, 0)) {
            PPP_TTY_PATH = usbnet_adapeter;
            ql_pppd_pid = ql_ndis_start(apn, user, pass, auth_type, s_default_pdp);
        } else {
#ifdef USB_HOST_USE_RESET_RESUME
            if(ql_is_mdm9215 || ql_is_mdm9x07) {
                if (quectel_at_datamode()) {
                    at_send_command("AT^DATADOWN", NULL);
                }
            }
#endif
            ql_pppd_pid = ql_pppd_start(NULL, user, pass, auth_type, ppp_number);
        }
    }
    if (ql_pppd_pid < 0)
        goto error;

    sleep(1);
    while (!s_closed && (retry++ < 50) && (nSetupDataCallFailTimes <= 3)) {
        if ((waitpid(ql_pppd_pid, NULL, WNOHANG)) == ql_pppd_pid)
            goto error;
        get_local_ip(ppp_local_ip);
        LOGD("[%d] trying to get_local_ip ... %s", retry, ppp_local_ip);
        if(strcmp(ppp_local_ip, "0.0.0.0"))
            break;
        sleep(1);
    }
    gettimeofday(&end_tv, NULL);
    LOGD("get_local_ip: %s, cost %ld sec", ppp_local_ip, (end_tv.tv_sec - begin_tv.tv_sec));

    if (!strcmp(ppp_local_ip, "0.0.0.0")) {
        goto error;
    }

    //Save the current active ip address
    strcpy(sKeepLocalip, ppp_local_ip);
    if (((cgreg_response[3] == 0x100) || (cops_response[3] == 100) && !strcmp(PPP_TTY_PATH, "ppp0"))) {
        RIL_requestTimedCallback(poll_cdma_evdo_ppp_state, NULL, &TIMEVAL_3);
    }
    requestOrSendDataCallList(&t, 1);
    at_response_free(p_response);
    at_response_free(p_pwd_response);

    bSetupDataCallCompelete = 1;
    nSetupDataCallFailTimes = 0;

//frameworks/opt/telephony/src/java/com/android/internal/telephony/dataconnection/DcTracker.java
//boolean isOnlySingleDcAllowed(int rilRadioTech)
//Quectel RIL only support setup one data call at the same time
    if (QL_RIL_VERSION >= 9) { //kitkat
        char prop_value[PROPERTY_VALUE_MAX] = {'\0'};
        property_get("ro.debuggable", prop_value, "0");
        if (prop_value[0] == '1') {
            property_get("persist.telephony.test.singleDc", prop_value, "0");
            if (prop_value[0] == '0') {
                // /data/property/persist.telephony.test.singleDc
                property_set("persist.telephony.test.singleDc", "1");
            }
        }
    }

    return;

error:
    if (!strncmp(PPP_TTY_PATH, "ppp", 3))
        ql_pppd_stop(SIGTERM);
    else
        ql_ndis_stop(SIGTERM);
    LOGE("Unable to setup PDP in %s\n", __func__);
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    at_response_free(p_response);
    at_response_free(p_pwd_response);
#ifdef USB_HOST_USE_RESET_RESUME
    if ((ql_is_mdm9215 || ql_is_mdm9x07) && (!strncmp(PPP_TTY_PATH, "ppp", 3)) &&  (retry < 10) && (nSetupDataCallFailTimes > 3))
    {
        // ppp call fail 4 times in 10 seconds, maybe module is in PPP CALL state
        at_send_command("AT+CFUN=4",  NULL);
        sleep(3);
        at_send_command("AT+CFUN=1",  NULL);
        //it is better check module state before setup ppp call
    }
#endif
}

static void requestSMSAcknowledge(void *data, size_t datalen, RIL_Token t)
{
    int ackSuccess;
    int err;

    ackSuccess = ((int *)data)[0];

    if (ackSuccess == 1) {
        err = at_send_command("AT+CNMA=1", NULL);
    } else if (ackSuccess == 0)  {
        err = at_send_command("AT+CNMA=2", NULL);
    } else {
        LOGE("unsupported arg to RIL_REQUEST_SMS_ACKNOWLEDGE\n");
        goto error;
    }

    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
    return;
error:
    LOGE("%s error", __func__);
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);

}

#if 1 //usim -> sim
typedef struct __TLV {
    unsigned char tag;
    unsigned char len;
    unsigned char data[0];
} TLV;

static int hexCharToInt(char c) {
    if (c >= '0' && c <= '9') return (c - '0');
    if (c >= 'A' && c <= 'F') return (c - 'A' + 10);
    if (c >= 'a' && c <= 'f') return (c - 'a' + 10);
    return 0;
}

static int hexStringToBytes(const char * s, unsigned char *d) {
    int sz, i;

    if (!s || !strlen(s))
        return 0;

    sz = strlen(s) / 2;

    for (i = 0; i < sz ; i++) {
        d[i] = (unsigned char) ((hexCharToInt(s[i*2 + 0]) << 4) | hexCharToInt(s[i*2 + 1]));
    }

    return sz;
}

static TLV * getTLV(const unsigned char *d, unsigned char tag) {
     TLV *tlv = (TLV *)d;
     int sz = tlv->len;

     tlv++; //skip head

     while (sz) {
        if (tlv->tag != tag) {
            tlv = (TLV *)(((char *)tlv) + sizeof(TLV) + tlv->len);
            sz -= sizeof(TLV) + tlv->len;
        } else {
        #if 0
            int i;
            printf("{%02x, %02x, ", tlv->tag, tlv->len);
            for (i = 0; i < tlv->len; i++)
                printf("%02x, ", tlv->data[i]);
            printf("}\n");
        #endif
            return tlv;
        }
    }
    return NULL;
}

//frameworks\base\telephony\java\com\android\internal\telephony\IccFileHandler.java
//from TS 11.11 9.1 or elsewhere
const int EF_ICCID  = 0x2fe2;
const int COMMAND_READ_BINARY = 0xb0;
const int COMMAND_UPDATE_BINARY = 0xd6;
const int COMMAND_READ_RECORD = 0xb2;
const int COMMAND_UPDATE_RECORD = 0xdc;
const int COMMAND_SEEK = 0xa2;
const int COMMAND_GET_RESPONSE = 0xc0;
const int GET_RESPONSE_EF_IMG_SIZE_BYTES = 10;
const int GET_RESPONSE_EF_SIZE_BYTES = 15;

//***** types of files  TS 11.11 9.3
static const int EF_TYPE_TRANSPARENT = 0;
static const int EF_TYPE_LINEAR_FIXED = 1;
static const int EF_TYPE_CYCLIC = 3;

//***** types of files  TS 11.11 9.3
const int TYPE_RFU = 0;
const int TYPE_MF  = 1;
const int TYPE_DF  = 2;
const int TYPE_EF  = 4;

// Byte order received in response to COMMAND_GET_RESPONSE
// Refer TS 51.011 Section 9.2.1
const int RESPONSE_DATA_RFU_1 = 0;
const int RESPONSE_DATA_RFU_2 = 1;

const int RESPONSE_DATA_FILE_SIZE_1 = 2;
const int RESPONSE_DATA_FILE_SIZE_2 = 3;

const int RESPONSE_DATA_FILE_ID_1 = 4;
const int RESPONSE_DATA_FILE_ID_2 = 5;
const int RESPONSE_DATA_FILE_TYPE = 6;
const int RESPONSE_DATA_RFU_3 = 7;
const int RESPONSE_DATA_ACCESS_CONDITION_1 = 8;
const int RESPONSE_DATA_ACCESS_CONDITION_2 = 9;
const int RESPONSE_DATA_ACCESS_CONDITION_3 = 10;
const int RESPONSE_DATA_FILE_STATUS = 11;
const int RESPONSE_DATA_LENGTH = 12;
const int RESPONSE_DATA_STRUCTURE = 13;
const int RESPONSE_DATA_RECORD_LENGTH = 14;

void usim2sim(RIL_SIM_IO_Response *psr) {
    int sz;
    int i;
    unsigned char usim_data[1024];
    unsigned char sim_data[15] = {0};
    static char new_response[31];
    TLV * tlv;
    const char bytesToHexString[] = "0123456789abcdef";

    if (!psr->simResponse)
        return;

    if (!strlen(psr->simResponse)) {
        psr->simResponse = NULL;
        return;
    }

    if (strlen(psr->simResponse) < 4)
        return;

    sz = hexStringToBytes(psr->simResponse, usim_data);

    if (usim_data[0] != 0x62) {
        //LOGD("CRSM: not usim");
        return;
    }

    if (usim_data[1] != (sz - 2)) {
        //LOGD("CRSM: error usim len");
        return;
    }

    tlv = getTLV(usim_data, 0x80);
    if (tlv) {
        //LOGD("CRSM: FILE_SIZE %02X%02X", tlv->data[0], tlv->data[1]);
        sim_data[RESPONSE_DATA_FILE_SIZE_1] = tlv->data[0];
        sim_data[RESPONSE_DATA_FILE_SIZE_2] = tlv->data[1];
    }

    tlv = getTLV(usim_data, 0x83);
    if (tlv) {
        //LOGD("CRSM: FILE_ID %02X%02X", tlv->data[0], tlv->data[1]);
        sim_data[RESPONSE_DATA_FILE_ID_1] = tlv->data[0];
        sim_data[RESPONSE_DATA_FILE_ID_2] = tlv->data[1];
    }

    tlv = getTLV(usim_data, 0x82);
    if (tlv) {
        int filetype = (tlv->data[0] >> 3) & 0x7;
        int efstruct = (tlv->data[0] >> 0) & 0x7;
        //LOGD("CRSM: len: %d, %02x %02x %02x %02x %02x", tlv->len, tlv->data[0], tlv->data[1], tlv->data[2], tlv->data[3], tlv->data[4]);

        //File type:
        if ((filetype == 0) || (filetype == 1)) {
            //LOGD("CRSM: FILE_TYPE_EF");
            sim_data[RESPONSE_DATA_FILE_TYPE] = TYPE_EF;
        } else if ((filetype == 7) && (efstruct == 0)) {
            //LOGD("CRSM: TYPE_DF");
            sim_data[RESPONSE_DATA_FILE_TYPE] = TYPE_DF;
        } else {
            //LOGD("CRSM: TYPE_RFU");
            sim_data[RESPONSE_DATA_FILE_TYPE] = TYPE_RFU;
        }

        //EF struct
        if (efstruct == 1) {
            //LOGD("CRSM: EF_TYPE_TRANSPARENT");
            sim_data[RESPONSE_DATA_STRUCTURE] = EF_TYPE_TRANSPARENT;
        } else if (efstruct == 2) {
            //LOGD("CRSM: EF_TYPE_LINEAR_FIXED");
            sim_data[RESPONSE_DATA_STRUCTURE] = EF_TYPE_LINEAR_FIXED;
        } else if (efstruct == 3) {
            //LOGD("CRSM: EF_TYPE_CYCLIC");
            sim_data[RESPONSE_DATA_STRUCTURE] = EF_TYPE_CYCLIC;
         } else {
            //LOGD("CRSM: EF_TYPE_UNKNOWN");
         }

        if ((efstruct == 2) || (efstruct == 3)) {
            if (tlv->len == 5) {
                sim_data[RESPONSE_DATA_RECORD_LENGTH] = ((tlv->data[2] << 8) + tlv->data[3]) & 0xFF;
               //LOGD("CRSM: RESPONSE_DATA_RECORD_LENGTH %d", sim_data[RESPONSE_DATA_RECORD_LENGTH]);
            } else {
                //LOGD("CRSM: must contain Record length and Number of records");
            }
        }
    }

    for (i = 0; i < 15; i++) {
        new_response[i*2 + 0] =  bytesToHexString[0x0f & (sim_data[i] >> 4)];
        new_response[i*2 + 1] =  bytesToHexString[0x0f & sim_data[i]];
    }
    new_response[30] = '\0';

    psr->simResponse = new_response;

//see telephony\src\java\com\android\internal\telephony\uicc\IccIoResult.java
#if 0
    /**
     * true if this operation was successful
     * See GSM 11.11 Section 9.4
     * (the fun stuff is absent in 51.011)
     */
    public boolean success() {
        return sw1 == 0x90 || sw1 == 0x91 || sw1 == 0x9e || sw1 == 0x9f;
    }
#endif
    if (psr->sw1 == 0x90 || psr->sw1 == 0x91 || psr->sw1 == 0x9e || psr->sw1 == 0x9f)
        ;
    else
        psr->sw1 = 0x90;

    return;
}
#endif

static int settings_get(const char *key, char *prop_value) {
    char *cmd;
    FILE *fp;
    int nreads = 0;

    asprintf(&cmd, "settings get global %s", key);
    fp = popen(cmd, "r");
    free(cmd);

    if (fp) {
        nreads = fread(prop_value, 1, PROPERTY_VALUE_MAX-1, fp);
        if (nreads > 0) {
            prop_value[nreads] = 0;
            LOGD("settings get global %s %s", key, prop_value);
        }
        pclose(fp);
    }

    return nreads;
}

static void  requestSIM_IO(void *data, size_t datalen, RIL_Token t)
{
    ATResponse *p_response = NULL;
    RIL_SIM_IO_Response sr;
    int err;
    char *cmd = NULL;
    RIL_SIM_IO_v6 *p_args;
    char *line;
	unsigned int i = 0;
	ATResponse *qccid_response = NULL;
	char iccid_low = 0;

    memset(&sr, 0, sizeof(sr));

    p_args = (RIL_SIM_IO_v6 *)data;

#ifdef QUECTEL_RESTART_RIL_WHEN_NO_SETUP_DATA_CALL
    if (EF_ICCID == p_args->fileid && p_args->command == COMMAND_READ_BINARY) {
        static struct timeval TIMEVAL_XX = {QUECTEL_RESTART_RIL_WHEN_NO_SETUP_DATA_CALL,0};
        LOGD("%s check SetupDataCallTimeout %d seconds", __func__, (int)TIMEVAL_XX.tv_sec);
        RIL_requestTimedCallback(onSetupDataCallTimeout, NULL, &TIMEVAL_XX);
    }
#endif

    // sim quickened
    if((EF_ICCID != p_args->fileid) && (0 != s_ril_conf_init.Icc_Constants[0])) {
        int flg = 0;
        for(i = 0; i < sizeof(s_ril_conf_init.Icc_Constants) / sizeof(s_ril_conf_init.Icc_Constants[0]); i++) {
            if(p_args->fileid == s_ril_conf_init.Icc_Constants[i]) {
                flg = 1;
                break;
            }
        }

        if(!flg) {
            RIL_onRequestComplete(t, RIL_E_REQUEST_NOT_SUPPORTED, NULL, 0);
    		return;
        }
    }

    /* FIXME handle pin2 */
#if 1 //quectel
    if (p_args->command == COMMAND_GET_RESPONSE) {
        p_args->p3 = 0;
    }

    // This judge just for FIX fresscale's bug.
    // Other case please neglect it.
    if ((EF_ICCID == p_args->fileid) && (COMMAND_READ_BINARY == p_args->command)) {
        if (GET_RESPONSE_EF_SIZE_BYTES == p_args->p3) {
            p_args->p3 = GET_RESPONSE_EF_IMG_SIZE_BYTES; //Change P3 from 15 to 10
        }
    }

#endif

    if (p_args->data == NULL) {
        LOGI("[%s]: p_args->data is NULL", __func__);
        asprintf(&cmd, "AT+CRSM=%d,%d,%d,%d,%d",
                    p_args->command, p_args->fileid,
                    p_args->p1, p_args->p2, p_args->p3);
    } else {
#if 1 //quectel
        asprintf(&cmd, "AT+CRSM=%d,%d,%d,%d,%d,\"%s\"",
#else
        asprintf(&cmd, "AT+CRSM=%d,%d,%d,%d,%d,%s",
#endif
                    p_args->command, p_args->fileid,
                    p_args->p1, p_args->p2, p_args->p3, p_args->data);
    }

    err = at_send_command_singleline(cmd, "+CRSM:", &p_response);

    if (err < 0 || p_response->success == 0) {
        goto error;
    }

    line = p_response->p_intermediates->line;

    err = at_tok_start(&line);
    if (err < 0) goto error;

    err = at_tok_nextint(&line, &(sr.sw1));
    if (err < 0) goto error;

    err = at_tok_nextint(&line, &(sr.sw2));
    if (err < 0) goto error;

    if (at_tok_hasmore(&line)) {
        err = at_tok_nextstr(&line, &(sr.simResponse));
        if (err < 0) goto error;
    }

#if 1 //quectel
//see telephony\src\java\com\android\internal\telephony\uicc\IccFileHandler.java handleMessage() -> case EVENT_GET_BINARY_SIZE_DONE:
    if (p_args->command == COMMAND_GET_RESPONSE)
        usim2sim(&sr);
#endif

    LOGD("[%s]: RIL_SIM_IO_Response Complete sr.sw1=%d, sr.sw2=%d, sr.simResponse=%s",
        __func__, sr.sw1, sr.sw2, sr.simResponse);

#ifdef QUECTEL_REPORT_SIGNAL_STRENGTH
    if ((p_args->fileid == EF_ICCID) && (p_args->command == COMMAND_READ_BINARY)) {
        requestSignalStrength(NULL, 0, NULL);
    }
#endif

    if (p_args->fileid == EF_ICCID && sr.simResponse == NULL)
        goto error;

    RIL_onRequestComplete(t, RIL_E_SUCCESS, &sr, sizeof(sr));
    at_response_free(p_response);
    free(cmd);

    return;
error:
    if (p_args->fileid == EF_ICCID/* && ql_is_mdm9215 && strStartsWith(ql_product_version, "EC20CE")*/) {
        sr.sw1 = 144;
        sr.sw2 = 0;
        sr.simResponse = NULL;
        if (p_args->command == COMMAND_GET_RESPONSE) {
            sr.simResponse = "0000000a2fe2040000000000000000";
        } else if (p_args->command == COMMAND_READ_BINARY) {
#define TELCOMM_DUMMY_ICCID "98681031098310024233"
            err = at_send_command_singleline(cmd, "+QCCID:", &qccid_response);
            if (err < 0 || qccid_response == NULL || qccid_response->success == 0) {
                sr.simResponse = TELCOMM_DUMMY_ICCID;
            } else {
            line = qccid_response->p_intermediates->line;
            if (0 == at_tok_start(&line) && 0 == at_tok_nextstr(&line, &sr.simResponse)) {
                for (i = 0; i < strlen(sr.simResponse); i+=2) {
                    if ((strlen(sr.simResponse) - 1) < (i+1)) {
                        LOGE("[%s:%d] array out of index, invalid iccid length.\n", __func__, __LINE__);
                        sr.simResponse = TELCOMM_DUMMY_ICCID;
                    } else {
                        iccid_low = sr.simResponse[i];
                        sr.simResponse[i] = sr.simResponse[i+1];
                        sr.simResponse[i+1] = iccid_low;
                    }
                }
            } else {
                LOGE("[%s:%d] at_tok_start or at_tok_nextstr failed.\n", __func__, __LINE__);
                sr.simResponse = TELCOMM_DUMMY_ICCID;
            }
            }
        }
        if (sr.simResponse != NULL) {
            RIL_onRequestComplete(t, RIL_E_SUCCESS, &sr, sizeof(sr));
            at_response_free(p_response);
            free(cmd);
        	if (NULL != qccid_response) {
            	at_response_free(qccid_response);
            	qccid_response = NULL;
            }
            return;
        }
    }
    LOGE("%s error\n", __func__);
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    at_response_free(p_response);
    free(cmd);
    if (NULL != qccid_response) {
        at_response_free(qccid_response);
    }
}

static int getRemainingTimes(int request)
{
    ATResponse *p_response = NULL;
    ATLine *p_cur = NULL;
    char *line = NULL;
    int err;
    //int n = 0;
    char *fac = NULL;
    int pin1_remaining_times;
    int puk1_remaining_times;
    int pin2_remaining_times;
    int puk2_remaining_times;

    err = at_send_command_multiline("AT+QPINC?", "+QPINC", &p_response);
    //for (p_cur = p_response->p_intermediates; p_cur != NULL;
    //     p_cur = p_cur->p_next)
    //    n++;

    if(err < 0  || p_response == NULL || p_response->success == 0) goto error;

    for (p_cur = p_response->p_intermediates; p_cur != NULL;p_cur = p_cur->p_next)
    {
        char *line = p_cur->line;

        at_tok_start(&line);

        err = at_tok_nextstr(&line,&fac);
        if(err < 0) goto error;

        if(!strncmp(fac,"SC",2))
        {
            err = at_tok_nextint(&line,&pin1_remaining_times);
            if(err < 0) goto error;
            err = at_tok_nextint(&line,&puk1_remaining_times);
            if(err < 0) goto error;
        }
        else if(!strncmp(fac,"P2",2))
        {
            err = at_tok_nextint(&line,&pin2_remaining_times);
            if(err < 0) goto error;
            err = at_tok_nextint(&line,&puk2_remaining_times);
            if(err < 0) goto error;
        }
        else
            goto error;
    }
    LOGI("PIN1:%d\nPIN2:%d\nPUN1:%d\nPUN2:%d",pin1_remaining_times,pin2_remaining_times,puk1_remaining_times,puk2_remaining_times);

    free(p_response);
    free(p_cur);
#if 0
    if(!strcmp(pinType,"PIN1")) return pin1_remaining_times;
    else  if(!strcmp(pinType,"PIN2")) return pin2_remaining_times;
    else  if(!strcmp(pinType,"PUK1")) return puk1_remaining_times;
    else  if(!strcmp(pinType,"PUK2")) return puk2_remaining_times;
    else return -1;
#else
    switch(request)
    {
        case RIL_REQUEST_SET_FACILITY_LOCK:
        case RIL_REQUEST_ENTER_SIM_PIN:
        case RIL_REQUEST_CHANGE_SIM_PIN:
            return pin1_remaining_times;
        case RIL_REQUEST_ENTER_SIM_PIN2:
        case RIL_REQUEST_CHANGE_SIM_PIN2:
            return pin2_remaining_times;
        case RIL_REQUEST_ENTER_SIM_PUK:
            return puk1_remaining_times;
        case RIL_REQUEST_ENTER_SIM_PUK2:
            return puk2_remaining_times;
        default:
            return -1;
    }
#endif
error:
    return -1;
}

static void  requestQuestFacility(void*  data, size_t  datalen, RIL_Token  t)
{
    ATResponse   *p_response = NULL;
    int           err;
    char*         cmd = NULL;
    int status;
    char *fac = NULL;
    char *mode = "2";
    char *password = NULL;
    char *class = NULL;
    char *line = NULL;

    LOGE("%s Enter\n", __func__);

    fac = ((char **)data)[0];
    password = ((char **)data)[1];
    class = ((char **)data)[2];

    asprintf(&cmd,"AT+CLCK=\"%s\",%s,\"%s\",%s",fac,mode,password,class);
    err = at_send_command_singleline(cmd, "+CLCK:", &p_response);
    if(err < 0  || p_response == NULL || p_response->success == 0) goto error;

    line = p_response->p_intermediates->line;

    err = at_tok_start(&line);
    if (err < 0) goto error;

    err = at_tok_nextint(&line, &status);
    if (err < 0) goto error;

    RIL_onRequestComplete(t, RIL_E_SUCCESS, &status, sizeof(int *));
    goto out;

error:
    LOGE("%s error\n", __func__);
    RIL_onRequestComplete(t, RIL_E_PASSWORD_INCORRECT, NULL, 0);
    //RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
out:

    LOGE("%s leave\n", __func__);

    free(cmd);
    at_response_free(p_response);
}

static void  requestSetFacility(void*  data, size_t  datalen, RIL_Token  t,int request)
{
    ATResponse   *p_response = NULL;
    int           err;
    char*         cmd = NULL;
    int remaining_times = -1;
    char *fac = NULL;
    char *mode = NULL;
    char *password = NULL;
    char *class = NULL;
    RIL_Errno errorril = RIL_E_SUCCESS;

    LOGE("%s Enter\n", __func__);

    fac = ((char **)data)[0];
    mode = ((char **)data)[1];
    password = ((char **)data)[2];
    class = ((char **)data)[3];

    asprintf(&cmd,"AT+CLCK=\"%s\",%s,\"%s\",%s",fac,mode,password,class);
    err = at_send_command(cmd,&p_response);
    free(cmd);
    //if(err < 0 || p_response == NULL || p_response->success == 0) goto error;

    if (err == 0 && p_response != NULL && p_response->success == 0) {
        switch (at_get_cme_error(p_response)) {
        /* CME ERROR 11: "SIM PIN required" happens when PIN is wrong */
        case CME_SIM_PIN_REQUIRED:
            LOGI("Wrong PIN");
            errorril = RIL_E_PASSWORD_INCORRECT;
            break;
        /*
         * CME ERROR 12: "SIM PUK required" happens when wrong PIN is used
         * 3 times in a row
         */
        case CME_SIM_PUK_REQUIRED:
            LOGI("PIN locked, change PIN with PUK");
            //num_retries = 0;/* PUK required */
            errorril = RIL_E_PASSWORD_INCORRECT;
            break;
        /* CME ERROR 16: "Incorrect password" happens when PIN is wrong */
        case CME_INCORRECT_PASSWORD:
            LOGI("Incorrect password, Facility");
            errorril = RIL_E_PASSWORD_INCORRECT;
            break;
        /* CME ERROR 17: "SIM PIN2 required" happens when PIN2 is wrong */
        case CME_SIM_PIN2_REQUIRED:
            LOGI("Wrong PIN2");
            errorril = RIL_E_PASSWORD_INCORRECT;
            break;
        /*
         * CME ERROR 18: "SIM PUK2 required" happens when wrong PIN2 is used
         * 3 times in a row
         */
        case CME_SIM_PUK2_REQUIRED:
            LOGI("PIN2 locked, change PIN2 with PUK2");
            //num_retries = 0;/* PUK2 required */
            errorril = RIL_E_SIM_PUK2;
            break;
        default: /* some other error */
            //num_retries = -1;
            errorril = RIL_E_GENERIC_FAILURE;
            break;
        }
    }

    remaining_times = getRemainingTimes(request);
    LOGI("remaining_times = %d",remaining_times);
    RIL_onRequestComplete(t, errorril, &remaining_times, sizeof(int *));
}

static void  requestChangeSimPin(void*  data, size_t  datalen, char *fac, RIL_Token  t,int request)
{
    ATResponse   *p_response = NULL;
    int           err;
    char*         cmd = NULL;
    int remaining_times = -1;
    char *oldpin = NULL,*newpin = NULL;

    LOGE("%s Enter\n", __func__);

    oldpin = ((char **)data)[0];
    newpin = ((char **)data)[1];

    if(ql_is_device_and_sim_both_support_cdma) {
        asprintf(&cmd,"AT+QCPWD=\"%s\",\"%s\",\"%s\"",fac,oldpin,newpin);
    } else {
        asprintf(&cmd,"AT+CPWD=\"%s\",\"%s\",\"%s\"",fac,oldpin,newpin);
    }

    err = at_send_command(cmd,&p_response);
    free(cmd);
    //if(err < 0 || p_response == NULL || p_response->success == 0) goto error;

    remaining_times = getRemainingTimes(request);
    LOGI("remaining_times = %d",remaining_times);
    //RIL_onRequestComplete(t, RIL_E_SUCCESS, &remaining_times, sizeof(int *));

    if(err < 0 || p_response == NULL || p_response->success == 0) {
        if (at_get_cme_error(p_response) == CME_INCORRECT_PASSWORD) {
            RIL_onRequestComplete(t, RIL_E_PASSWORD_INCORRECT, &remaining_times, sizeof(int *));
        } else {
            RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, &remaining_times, sizeof(int *));
        }
    } else {
    RIL_onRequestComplete(t, RIL_E_SUCCESS, &remaining_times, sizeof(int *));
    }
}

static void  requestEnterSimPin(void*  data, size_t  datalen, RIL_Token  t,int request)
{
    ATResponse   *p_response = NULL;
    int           err;
    char*         cmd = NULL;
    const char**  strings = (const char**)data;
    int remaining_times = -1;

    LOGE("%s Enter\n", __func__);

    if (( datalen == sizeof(char*)) || !strings[1] || !strings[1][0]) {
        asprintf(&cmd, "AT+CPIN=%s", strings[0]);
    } else {
        asprintf(&cmd, "AT+CPIN=\"%s\",\"%s\"", strings[0], strings[1]);
    }

    err = at_send_command(cmd, &p_response);
    free(cmd);
    //if(err < 0 || p_response == NULL || p_response->success == 0) goto error;
    if (!err && p_response && p_response->success == 1) {
        if (currentState() == RADIO_STATE_SIM_LOCKED_OR_ABSENT)
            RIL_requestTimedCallback (onSIMStateChange, NULL, &TIMEVAL_3);
    }

    remaining_times = getRemainingTimes(request);
    LOGI("remaining_times = %d",remaining_times);

    if(err < 0 || p_response == NULL || p_response->success == 0) {
        if (at_get_cme_error(p_response) == CME_INCORRECT_PASSWORD) {
            RIL_onRequestComplete(t, RIL_E_PASSWORD_INCORRECT, &remaining_times, sizeof(int *));
        } else {
            RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, &remaining_times, sizeof(int *));
        }
    } else {
    RIL_onRequestComplete(t, RIL_E_SUCCESS, &remaining_times, sizeof(int *));
    }
}

static void  requestSendUSSD(void *data, size_t datalen, RIL_Token t)
{
    ATResponse   *p_response = NULL;
    int err;
    char *cmd = NULL;
    const char *ussdRequest;
    int mode,dcs;

#ifdef CUSD_USE_UCS2_MODE
	int i = 0;
	char ucs2[100];
	ussdRequest = (char *)(data);
	while (ussdRequest[i]) {
    	sprintf(ucs2+i*4, "%04X", ussdRequest[i]);
    	i++;
    }
	data = ucs2;
#endif
    ussdRequest = (char *)(data);
    mode = 1;
//    dcs = 15;

    //asprintf(&cmd,"AT+CUSD=%d,%s,%d",mode,ussdRequest,dcs);
    asprintf(&cmd,"AT+CUSD=%d,\"%s\"",mode,ussdRequest);
    err = at_send_command(cmd,&p_response);
    if(err != 0 || p_response == NULL || p_response->success == 0)
    {
        goto error;
    }

#if 1 //quectel
    ussd_pending_index++;
    RIL_requestTimedCallback(onUssdTimedCallback, (void *)(&ussd_pending_index), &ussd_timeout_timeval);
#endif
    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
    return;
error:
    LOGE("[Joe]ERROR: %s failed",__func__);
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
// @@@ TODO

}

/**
 *  Wythe:Add on 2013-04-02 for 4.0 ril
 *  Add new function to handle more requests.
 *  Must response to RIL.JAVA.
 */

/*
* Function: requestScreenState
* Purpose : handle the request when screen is on or off.
* Request : RIL_REQUEST_SCREEN_STATE
*/
static void requestScreenState(void *data, size_t datalen, RIL_Token t)
{
	int cops_response[4] = {0};
#ifdef USB_HOST_USE_RESET_RESUME //quectel
    if (ql_is_UC20 || ql_is_mdm9x07) { //if the usb host donot support suspend
        if (((int *)data)[0]) {
            at_send_command("AT+QCFG=\"urc/ri/smsincoming\",\"pulse\",120", NULL);
            at_send_command("AT+QCFG=\"urc/delay\",0", NULL);
        } else {
            at_send_command("AT+QCFG=\"urc/ri/smsincoming\",\"pulse\",2000", NULL);
            at_send_command("AT+QCFG=\"urc/delay\",1", NULL);
        }
    }
#endif

#if 0 //use "AT+CGPADDR=1" to check if ip change on module side.
    if (((int *)data)[0] && bSetupDataCallCompelete && (ql_is_mdm9215 || ql_is_mdm9x07)) {
        char ppp_local_ip[64];
        get_local_ip(ppp_local_ip);
        if (strcmp(ppp_local_ip, "0.0.0.0")) {
            char *cmd;
            ATResponse *p_response = NULL;
            int err;
            int ip_change = 0;
            asprintf(&cmd, "AT+CGPADDR=%d", s_default_pdp);
            err = at_send_command_singleline(cmd, "+CGPADDR: ", &p_response);
            free(cmd);
            if (err < 0 || p_response == NULL || p_response->success == 0) {
                //ip_change = 1;
            } else {
                int pdp;
                char *ppp_remote_ip = "127.0.0.1";
                char *line = p_response->p_intermediates->line;
                err = at_tok_start(&line);
                if (err < 0) goto error;
                err = at_tok_nextint(&line,&pdp);
                if (err < 0) goto error;
                err = at_tok_nextstr(&line,&ppp_remote_ip);
                if (ppp_remote_ip == NULL)
                    ppp_remote_ip = "127.0.0.1";
            error:
                ip_change = strcmp(ppp_local_ip, ppp_remote_ip);
                if (ip_change) {
                    LOGD("localIP: %s VS remoteIP: %s", ppp_local_ip, ppp_remote_ip);
                }
            }
            if (ip_change) {
                if (!strncmp(PPP_TTY_PATH, "ppp", 3))
                    ql_pppd_stop(SIGTERM);
                else
                    ql_ndis_stop(SIGTERM);
            }
            at_response_free(p_response);
        }
    }
#endif

    //Add by Wythe.WANG for XingWang test 2016/1/20
    //Use AT^DATAMODE here to check whether the ppp linker is still alive on modem side
    if((ql_is_mdm9215 || ql_is_mdm9x07) && (((int *)data)[0]) && !strcmp(PPP_TTY_PATH, "ppp0") && !access("/sys/class/net/ppp0", R_OK))
    {
    	if (!quectel_at_datamode()) {
            ql_pppd_stop(SIGTERM);
        }
        
        if (ql_is_device_and_sim_both_support_cdma) {
            /**<- Add by Wythe.WANG to detect the ip change in cdma mode 2016/8/2*/
            if (quectel_is_ip_change_in_cdma_ppp()) {
                ql_pppd_stop(SIGTERM);
            }
        }
    }

    if ((ql_is_mdm9215 || ql_is_mdm9x07) && (((int *)data)[0]) && strcmp(PPP_TTY_PATH, "ppp0"))
        ql_ndis_stop(SIGUSR2);

    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
}

/*
*   Function: requestBaseBandVersion
*   Purpose : return string of BaseBand version
*   Request : RIL_REQUEST_BASEBAND_VERSION
*/
static void requestBaseBandVersion(void *data, size_t datalen, RIL_Token t)
{
    int err;
    ATResponse *atResponse = NULL;
    char *line;

    err = at_send_command_singleline("AT+CGMR", "\0", &atResponse);

    if(err != 0){
        LOGE("[Wythe]%s() Error Reading Base Band Version!",__func__);
        RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
        return;
    }

    line = atResponse->p_intermediates->line;

    RIL_onRequestComplete(t, RIL_E_SUCCESS, line, sizeof(char *));

    at_response_free(atResponse);
}

/*
*   Function: requestGetIMEISV
*   Purpose : return the IMEI SV(software version)
*   Request : RIL_REQUEST_GET_IMEISV
*/
static void requestGetIMEISV(RIL_Token t)
{
    requestBaseBandVersion(NULL, 0, t);
    //RIL_onRequestComplete(t, RIL_E_SUCCESS, (void *)00, sizeof(char *));
}

const char * NetworkTypeStr[] = {
    "PREF_NET_TYPE_GSM_WCDMA",
    "PREF_NET_TYPE_GSM_ONLY",
    "PREF_NET_TYPE_WCDMA_ONLY",
    "PREF_NET_TYPE_GSM_WCDMA_AUTO",
    "PREF_NET_TYPE_CDMA_EVDO_AUTO",
    "PREF_NET_TYPE_CDMA_ONLY",
    "PREF_NET_TYPE_EVDO_ONLY",
    "PREF_NET_TYPE_GSM_WCDMA_CDMA_EVDO_AUTO",
    "PREF_NET_TYPE_LTE_CDMA_EVDO",
    "PREF_NET_TYPE_LTE_GSM_WCDMA",
    "PREF_NET_TYPE_LTE_CMDA_EVDO_GSM_WCDMA",
    "PREF_NET_TYPE_LTE_ONLY",
    "PREF_NET_TYPE_LTE_WCDMA",
    "PREF_NET_TYPE_UNKNOW",
    "PREF_NET_TYPE_UNKNOW",
    "PREF_NET_TYPE_UNKNOW",
    "PREF_NET_TYPE_UNKNOW",
    "PREF_NET_TYPE_UNKNOW",
};

//see [Internal]EC20_ATC_AT+QCFG_NWSCANMODEEX_V1.0_Francis_20160801.docx
#define BIT(num)   (1 << (num))
#define nwscanmodeex_cdma BIT(0) //Bit 0 : cdma2000 1X
#define nwscanmodeex_evdo BIT(1) //Bit 1 : cdma2000 HRPD (1xEV-DO)
#define nwscanmodeex_gsm BIT(2) //Bit 2 : GSM
#define nwscanmodeex_wcdma BIT(3) //Bit 3 : WCDMA
#define nwscanmodeex_lte BIT(4) //Bit 4 : LTE
#define nwscanmodeex_tdscdma BIT(5) //Bit 5 : TDS
#define nwscanmodeex_auto (BIT(0)|BIT(1)|BIT(2)|BIT(3)|BIT(4)|BIT(5))

///android/android-x86/lollipop-x86/packages/services/Telephony/src/com/android/phone/MobileNetworkSettings.java:
//else if (getResources().getBoolean(R.bool.world_phone) == true)
//enable world_phone to get more network menu items
int ql_nwscanmodeex_map[][2] = {
#if 1 //??????????android?????ò˵?????Ĭ?ϵ? 4G / 3G / 2G ѡ??, ???԰????Ƿŵ?ӳ????????ǰ??
    {PREF_NET_TYPE_LTE_GSM_WCDMA,   nwscanmodeex_auto},
#if 1 //??ΪandroidĬ?ϵ???ѡ??????3G, ???????????ﲻ?????İ?ģ?????Ñ?G ģʽ??????û??ʹ??LTE
//pay attention, the default PreferredNetworkType is NETWORK_MODE_WCDMA_PREF. it is not suitable for LTE modules.
//RILConstants.java (frameworks\base\telephony\java\com\android\internal\telephony)
//int PREFERRED_NETWORK_MODE      = NETWORK_MODE_WCDMA_PREF;
//must change to NETWORK_MODE_LTE_CMDA_EVDO_GSM_WCDMA in order to use 4G
    {PREF_NET_TYPE_GSM_WCDMA,   nwscanmodeex_gsm | nwscanmodeex_cdma | nwscanmodeex_wcdma | nwscanmodeex_evdo | nwscanmodeex_tdscdma},
#else
    {PREF_NET_TYPE_GSM_WCDMA,   nwscanmodeex_auto},
#endif
    {PREF_NET_TYPE_GSM_ONLY,    nwscanmodeex_gsm | nwscanmodeex_cdma},
#endif

    {PREF_NET_TYPE_WCDMA,                    nwscanmodeex_wcdma},
    {PREF_NET_TYPE_GSM_WCDMA_AUTO,           nwscanmodeex_gsm | nwscanmodeex_wcdma},
    {PREF_NET_TYPE_CDMA_EVDO_AUTO,           nwscanmodeex_cdma | nwscanmodeex_evdo},
    {PREF_NET_TYPE_CDMA_ONLY,                nwscanmodeex_cdma},
    {PREF_NET_TYPE_EVDO_ONLY,                nwscanmodeex_evdo},
    {PREF_NET_TYPE_GSM_WCDMA_CDMA_EVDO_AUTO, nwscanmodeex_auto /*nwscanmodeex_gsm | nwscanmodeex_wcdma | nwscanmodeex_cdma | nwscanmodeex_evdo*/},
    {PREF_NET_TYPE_LTE_CDMA_EVDO,            nwscanmodeex_auto /*nwscanmodeex_lte | nwscanmodeex_cdma | nwscanmodeex_evdo*/},
    {PREF_NET_TYPE_LTE_GSM_WCDMA,            nwscanmodeex_auto /*nwscanmodeex_lte | nwscanmodeex_gsm | nwscanmodeex_wcdma*/},
    {PREF_NET_TYPE_LTE_CMDA_EVDO_GSM_WCDMA,  nwscanmodeex_auto /*nwscanmodeex_lte | nwscanmodeex_cdma | nwscanmodeex_evdo | nwscanmodeex_gsm | nwscanmodeex_wcdma*/},
    {PREF_NET_TYPE_LTE_ONLY,                 nwscanmodeex_lte},
#if RIL_VERSION >= 9
    {PREF_NET_TYPE_LTE_WCDMA,                nwscanmodeex_auto /*nwscanmodeex_lte | nwscanmodeex_wcdma*/},
#endif
};

static RIL_PreferredNetworkType ql_find_net_type_android(int net_type_module)
{
    unsigned int i = 0;

    for(i = 0; i < sizeof(ql_nwscanmodeex_map) / sizeof(ql_nwscanmodeex_map[0]); i++) {
        if(ql_nwscanmodeex_map[i][1] == net_type_module) {
            return ql_nwscanmodeex_map[i][0];
        }
    }

    return PREF_NET_TYPE_LTE_CMDA_EVDO_GSM_WCDMA;
}

static int ql_find_net_type_module(RIL_PreferredNetworkType net_type_android)
{
    unsigned int i = 0;

    for(i = 0; i < sizeof(ql_nwscanmodeex_map) / sizeof(ql_nwscanmodeex_map[0]); i++) {
        if(ql_nwscanmodeex_map[i][0] == (int)net_type_android) {
            return ql_nwscanmodeex_map[i][1];
        }
    }

    return nwscanmodeex_auto;
}

static RIL_RadioTechnology poll_cdma_evdo_radio_tech = 0;
static int poll_cdma_evdo_state_count = 0;
static void poll_cdma_evdo_state(void *param) {
    if (poll_cdma_evdo_state_count) {
        RIL_RadioTechnology radio_tech = quectel_at_qnwinfo();
        if (radio_tech == poll_cdma_evdo_radio_tech) {
            RIL_onUnsolicitedResponse (RIL_UNSOL_RESPONSE_VOICE_NETWORK_STATE_CHANGED,NULL, 0);
            poll_cdma_evdo_state_count = 0;
        } else {
            RIL_requestTimedCallback (poll_cdma_evdo_state, NULL, &TIMEVAL_3);
            poll_cdma_evdo_state_count--;
        }
    }
}

static int quectel_qcfg_nwscanmodeex(int new_rat) {

    int err;
    char *cmd, *line;
    ATResponse *p_response = NULL;
    int old_nwscanmodeex, old_rat;
    int new_nwscanmodeex;
    int tmp;

    err = at_send_command_singleline("AT+QCFG=\"nwscanmodeex\"", "+QCFG: ", &p_response);
    if (err < 0  ||  p_response == NULL || p_response->success == 0) {
        goto __error;
    }

    line = p_response->p_intermediates->line;

    err = at_tok_start(&line);
    if (err < 0) goto __error;

    err = skipComma(&line);
    if (err < 0) goto __error;

    err = at_tok_nextint(&line, &old_nwscanmodeex);
    if (err < 0) goto __error;

    at_response_free(p_response); p_response = NULL;

    old_rat = ql_find_net_type_android(old_nwscanmodeex);
    LOGD("%s get PreferredNetworkType %d (%s)", __func__, old_rat, NetworkTypeStr[old_rat]);

    if (new_rat < 0)
        return old_rat;

    LOGD("%s set PreferredNetworkType %d (%s)", __func__, new_rat, NetworkTypeStr[new_rat]);

    new_nwscanmodeex = ql_find_net_type_module(new_rat);

    if (new_nwscanmodeex == old_nwscanmodeex)
        return new_rat;

    if((47 == new_nwscanmodeex) && (63 == old_nwscanmodeex)) {
        if (!strncmp(PPP_TTY_PATH, "ppp", 3)) {
            ql_pppd_stop(SIGTERM);
        }
    }

    asprintf(&cmd, "AT+QCFG=\"nwscanmodeex\",%d", new_nwscanmodeex);
    err = at_send_command(cmd, &p_response);
    free(cmd);
#if 1 //9x07 on cdmd/evdo mode also donot report report +CGREG/+CREG when network state change
    if (ql_is_device_and_sim_both_support_cdma && !(new_nwscanmodeex & nwscanmodeex_lte))
    {
        if (new_nwscanmodeex & nwscanmodeex_evdo) {
            poll_cdma_evdo_radio_tech = RADIO_TECH_EVDO_A;
        } else {
            poll_cdma_evdo_radio_tech = RADIO_TECH_1xRTT;
       }

        poll_cdma_evdo_state_count = 15;
        RIL_requestTimedCallback (poll_cdma_evdo_state, NULL, &TIMEVAL_3);
    }
#endif
    if (err < 0  ||  p_response == NULL || p_response->success == 0) {
        goto __error;
    }
    at_response_free(p_response); p_response = NULL;
    return new_rat;

__error:
    return -1;
}

/**
 *  Function: requestSetPreferredNetworkType
 *  Purpose : Requests to set the preferred network type for searching and registering
 *            (CS/PS domain, RAT, and operation mode).
 *  Request : RIL_REQUEST_SET_PREFERRED_NETWORK_TYPE
 */
static void requestSetPreferredNetworkType(void *data, size_t datalen, RIL_Token t)
{
    int rat,err;
    int nwscanmode = 0; /* AUTO */
    char *cmd;
    ATResponse *atResponse = NULL;
    char *line = NULL;
    int cur_scanmode = 0;
    char *default_scanseq = NULL;
    char *cur_scanseq = NULL;
    int scanmode_set = 1;
    int scanseq_set = 1;

    assert (datalen >= sizeof(int *));
    rat = ((int *)data)[0];

    if (QL_RIL_VERSION < 6) {
        if (t != NULL)
            RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
        return;
    }

    if ((ql_is_mdm9215 || ql_is_mdm9x07) && (s_lastPreferredNetworkType == -1 && rat == PREF_NET_TYPE_GSM_WCDMA)) { //4G modules
        char prop_value[PROPERTY_VALUE_MAX] = {'\0'};
        
        if (access("/data/misc/rild/preferred_network_mode", R_OK)) {
//RILConstants.java
//int PREFERRED_NETWORK_MODE      = NETWORK_MODE_WCDMA_PREF;
//must change to NETWORK_MODE_LTE_GSM_WCDMA in order to use 4G
            rat = PREF_NET_TYPE_LTE_GSM_WCDMA;
            asprintf(&cmd, "settings put global preferred_network_mode %d", rat);
            system(cmd);
            free(cmd);
        }
        
        if (settings_get("preferred_network_mode", prop_value) > 0) {
//PhoneFactory.java
//Some SOC get networkMode from RILConstants.java, should from Settings.Secure.getInt(Settings.Secure.PREFERRED_NETWORK_MODE)
            int preferred_network_mode = atoi(prop_value);
            if (preferred_network_mode != PREF_NET_TYPE_GSM_WCDMA && 0 < preferred_network_mode && preferred_network_mode <= 12) {
                rat = preferred_network_mode;
            }
        }   
    }
    asprintf(&cmd, "echo %d > /data/misc/rild/preferred_network_mode", rat);
    if (access("/data/misc/rild", R_OK))
        system("mkdir -p /data/misc/rild");
    system(cmd);
    free(cmd); 
        
    s_lastPreferredNetworkType = rat;
    if ((unsigned)rat < (sizeof(NetworkTypeStr) / sizeof(NetworkTypeStr[0])))
        LOGD("%s %s", __func__, NetworkTypeStr[rat]);

    if (ql_is_GSM) { /* For 2G module, it only work in GSM */
        RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
        return;
    }
	if (ql_is_mdm9215 || ql_is_mdm9x07) { //4G modules
        if (quectel_qcfg_nwscanmodeex(rat) == rat) {
            if (t != NULL)
                RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
            return;
            }
    }

    switch (rat) {
        case PREF_NET_TYPE_GSM_ONLY:    /* GSM ONLY*/
            nwscanmode = 1;
        break;
        case PREF_NET_TYPE_WCDMA: /* WCDMA ONLY*/
            nwscanmode = 2;
        break;
        case PREF_NET_TYPE_GSM_WCDMA: /* GSM/WCDMA (WCDMA preferred) */
        default:
            nwscanmode = 0;
        break;
    }

    if (ql_is_mdm9215 || ql_is_mdm9x07) { //4G modules
//RILConstants.java (frameworks\base\telephony\java\com\android\internal\telephony)
//int PREFERRED_NETWORK_MODE      = NETWORK_MODE_WCDMA_PREF;
//must change to NETWORK_MODE_LTE_CMDA_EVDO_GSM_WCDMA in order to use 4G
        switch (rat) {
            case PREF_NET_TYPE_GSM_ONLY:    /* GSM ONLY*/
                nwscanmode = 1;
            break;
            case PREF_NET_TYPE_WCDMA: /* WCDMA ONLY*/
                nwscanmode = 2; //3G only
            break;
            case PREF_NET_TYPE_CDMA_ONLY: /* CDMA ONLY*/
                nwscanmode = 6;
            break;
            case PREF_NET_TYPE_EVDO_ONLY: /* EvDo ONLY*/
                nwscanmode = 5; //3G only
            break;
            case PREF_NET_TYPE_CDMA_EVDO_AUTO:
                nwscanmode = 8;
            break;
            case PREF_NET_TYPE_LTE_ONLY: /* LTE ONLY*/
                nwscanmode = 3;
            break;
            case PREF_NET_TYPE_LTE_CDMA_EVDO: /* LTE, CDMA and EvDo */
            case PREF_NET_TYPE_LTE_GSM_WCDMA: /* LTE, GSM/WCDMA */
            case PREF_NET_TYPE_LTE_CMDA_EVDO_GSM_WCDMA: /* LTE, CDMA, EvDo, GSM/WCDMA */
            default:
                nwscanmode = 0; //Auto
            break;
        }
    }

    err = at_send_command_multiline("AT+QCFG=\"nwscanmode\";+QCFG=\"nwscanseq\"", "+QCFG: ", &atResponse);
    if ((0 == err) && (NULL != atResponse) && (0 != atResponse->success)) {
        line = atResponse->p_intermediates->line;

        err = at_tok_start(&line);
        if (err < 0) goto send_command;

        err = skipComma(&line);
        if (err < 0) goto send_command;

        err = at_tok_nextint(&line, &cur_scanmode);
        if (err < 0) goto send_command;

        scanmode_set = (cur_scanmode != nwscanmode);

        line = atResponse->p_intermediates->p_next ? atResponse->p_intermediates->p_next->line : NULL;
        err = at_tok_start(&line);
        if (err < 0) goto send_command;

        err = skipComma(&line);
        if (err < 0) goto send_command;

        err = at_tok_nextstr(&line, &cur_scanseq);
        if (err < 0) goto send_command;
        LOGD("[%s:%d] cur_scanseq = %s\n", __func__, __LINE__, cur_scanseq);

#define UC20_NWSCANSEQ "0"
#define EC20_NWSCANSEQ "0405030201"
        if (ql_is_UC20) {
            default_scanseq = UC20_NWSCANSEQ;
        } else if (ql_is_mdm9215 || ql_is_mdm9x07) {
            default_scanseq = EC20_NWSCANSEQ;
        }
        scanseq_set = (strlen(default_scanseq) != strlen(cur_scanseq)) || strncmp(cur_scanseq, default_scanseq, strlen(default_scanseq));
    }
send_command:
#if 1
//ususally, customers set "XXX only mode" only for test, so wo can ignore nwscanseq setting
    if (scanseq_set) {
        if (NULL != default_scanseq) {
            asprintf(&cmd, "AT+QCFG=\"nwscanseq\",%s", default_scanseq);
            err = at_send_command(cmd, NULL);
            free(cmd);
            cmd = NULL;
        } else {
            err = 0;
        }
        if (err != 0)
            goto error;
    }
    if (scanmode_set) {
        asprintf(&cmd,  "AT+QCFG=\"nwscanmode\",%d", nwscanmode);
        err = at_send_command(cmd, NULL);
        free(cmd);
        if (err != 0)
            goto error;
    }
#endif

success:
    if (t != NULL)
        RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
    if (NULL != atResponse) {
        at_response_free(atResponse);
        atResponse = NULL;
    }
    return;

error:
    LOGE("ERROR: requestSetPreferredNetworkType() failed\n");
    if (t != NULL)
        RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    if (NULL != atResponse) {
        at_response_free(atResponse);
        atResponse = NULL;
    }
}

/*
*   Function: requestReportSTKServiceIsRunning
*   Purpose :
*   Request : RIL_REQUEST_REPORT_STK_SERVICE_IS_RUNNING
*/
static void requestReportSTKServiceIsRunning(RIL_Token t)
{
    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
}

/*
 * Function: requestDeactivateDefaultPDP
 * Purpose :
 * Request : RIL_REQUEST_DEACTIVATE_DATA_CALL
 */
static void requestDeactivateDefaultPDP(void *data, size_t datalen, RIL_Token t)
{
    char * cid;
    char *cmd;

    LOGD("requestDeactivateDefaultPDP()");

    bSetupDataCallCompelete = 0;
    if (!strncmp(PPP_TTY_PATH, "ppp", 3))
        ql_pppd_stop(SIGTERM);
    else
        ql_ndis_stop(SIGTERM);
    if (data != NULL)
        cid = ((char **)data)[0];
    else
        cid = "1";
    asprintf(&cmd, "AT+CGACT=0,%s", cid);
#if 1 //it is not need to send this AT to deactive pdp, only to check pdp status
    at_send_command_multiline("AT+CGACT?", "+CGACT:", NULL);
#else
    at_send_command(cmd, NULL);
#endif

    free(cmd);

    if (t != NULL)
        RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
    else
        RIL_onUnsolicitedResponse(RIL_UNSOL_DATA_CALL_LIST_CHANGED, NULL, 0);
    return;

error:
    LOGE("%s error\n",__func__);
    if (t != NULL)
        RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    else
        RIL_onUnsolicitedResponse(RIL_UNSOL_DATA_CALL_LIST_CHANGED, NULL, 0);
}

/**
 * Funtcion:requestLastPDPFailCause
 *
 * Purpose :Requests the failure cause code for the most recently failed PDP
 * context activate.
 *
 * Request : RIL_REQUEST_LAST_CALL_FAIL_CAUSE.
 *
 */
static int s_lastPdpFailCause = PDP_FAIL_ERROR_UNSPECIFIED;
static void requestLastPDPFailCause(RIL_Token t)
{
    RIL_onRequestComplete(t, RIL_E_SUCCESS, &s_lastPdpFailCause, sizeof(int));
}

static void requestGsmGetBroadcastSmsConfig(RIL_Token t) {
	RIL_onRequestComplete(t, RIL_E_REQUEST_NOT_SUPPORTED, NULL, 0);
}

static void requestGsmSetBroadcastSmsConfig(void *data, size_t datalen, RIL_Token t) {
	RIL_onRequestComplete(t, RIL_E_REQUEST_NOT_SUPPORTED, NULL, 0);
}

static void requestGsmSmsBroadcastActivation(void *data, size_t datalen, RIL_Token t) {
	RIL_onRequestComplete(t, RIL_E_REQUEST_NOT_SUPPORTED, NULL, 0);
}

static void requestDtmfStop(RIL_Token t) {
	RIL_onRequestComplete(t, RIL_E_REQUEST_NOT_SUPPORTED, NULL, 0);
}

static void requestQueryCallForwardStatus(void *data, size_t datalen, RIL_Token t)
{
    RIL_CallForwardInfo *callForwardInfo = (RIL_CallForwardInfo *)data;
    RIL_CallForwardInfo *r_callForwardInfo[10];
    char *cmd = NULL;
    int err;
    ATLine *tmp = NULL;
    char *line = NULL;
    ATResponse *atResponse;
    int i = 0;

    if (ql_is_mdm9215 || ql_is_mdm9x07) { //will make modules register 2G/3G from 4G
        i = 0;
        r_callForwardInfo[i] = (RIL_CallForwardInfo *)alloca(sizeof(RIL_CallForwardInfo));
        bzero(r_callForwardInfo[i],sizeof(RIL_CallForwardInfo));
        r_callForwardInfo[i]->number = (char *)alloca(15);
        bzero(r_callForwardInfo[i]->number,15);
        i++;
        RIL_onRequestComplete(t, RIL_E_SUCCESS, r_callForwardInfo, i * sizeof(RIL_CallForwardInfo *));
        return;
    }

    asprintf(&cmd,"AT+CCFC=%d,2",callForwardInfo->reason);
    err = at_send_command_multiline(cmd,"+CCFC:",&atResponse);
    free(cmd);
    if (err < 0 || atResponse->success == 0)
        goto error;

    tmp = atResponse->p_intermediates;
    while (tmp)
    {
        r_callForwardInfo[i] = (RIL_CallForwardInfo *)alloca(sizeof(RIL_CallForwardInfo));
        bzero(r_callForwardInfo[i],sizeof(RIL_CallForwardInfo));
        r_callForwardInfo[i]->number = (char *)alloca(15);
        bzero(r_callForwardInfo[i]->number,15);
        line = tmp->line;
        err = at_tok_start(&line);
        if (err < 0) goto error;
        err = at_tok_nextint(&line,&(r_callForwardInfo[i]->status));
        if (err < 0) goto error;
        if(r_callForwardInfo[i]->status != 0)
        {
            err = at_tok_nextint(&line,&(r_callForwardInfo[i]->serviceClass));
            if (err < 0) goto error;
            err = at_tok_nextstr(&line,&(r_callForwardInfo[i]->number));
            if (err < 0) goto error;
            LOGI("status:%d\nserviceClass:%d\nnumber:%s\n",r_callForwardInfo[i]->status,r_callForwardInfo[i]->serviceClass,r_callForwardInfo[i]->number);
        }
        i++;

        tmp = tmp->p_next;
    }

	RIL_onRequestComplete(t, RIL_E_SUCCESS, r_callForwardInfo, i * sizeof(RIL_CallForwardInfo *));
    return;
error:
	RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
}

static void requestSetCallForward(void *data, size_t datalen, RIL_Token t)
{
    char *cmd = NULL;
    int err;
    RIL_CallForwardInfo *callForwardInfo = (RIL_CallForwardInfo *)data;

    if (ql_is_mdm9215 || ql_is_mdm9x07) { //will make modules register 2G/3G from 4G
        RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
        return;
    }

    if(callForwardInfo->serviceClass == 0)
    {
        asprintf(&cmd,"AT+CCFC=%d,%d,\"%s\",%d",
                 callForwardInfo->reason,callForwardInfo->status,
                 callForwardInfo->number,callForwardInfo->toa);
    }
    else
    {
        asprintf(&cmd,"AT+CCFC=%d,%d,\"%s\",%d,%d",
                 callForwardInfo->reason,callForwardInfo->status,
                 callForwardInfo->number,callForwardInfo->toa,callForwardInfo->serviceClass);
    }

    err = at_send_command(cmd, NULL);
    free(cmd);
    if(err != 0) goto error;

	RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
    return;
error:
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
}

static void requestGetClir(RIL_Token t)
{
    int err;
    int response[2];
    int n;
    int m;
    char *line = NULL;
    ATResponse *atResponse = NULL;

    err = at_send_command_singleline("AT+CLIR?","+CLIR:",&atResponse);
	if (err != 0) goto error;

	line = atResponse->p_intermediates->line;
	err = at_tok_start(&line);
	if (err < 0) goto error;

    err = at_tok_nextint(&line,&n);
	if (err < 0) goto error;

    err = at_tok_nextint(&line,&m);
	if (err < 0) goto error;

    response[0] = n;
    response[1] = m;

	RIL_onRequestComplete(t, RIL_E_SUCCESS, response, sizeof(response));
    return ;

error:
	RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
}

static void requestSetClir(void *data, size_t datalen, RIL_Token t)
{
    int err;
    char *cmd = NULL;

    asprintf(&cmd, "AT+CLIR=%d",((int *)data)[0]);
    err = at_send_command(cmd,NULL);
	if (err != 0) goto error;

	RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
    free(cmd);
    return ;

error:
    free(cmd);
	RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
}

static void requestQueryCallWaiting(void *data, size_t datalen, RIL_Token t)
{
    int err;
    char *cmd = NULL;
    int response[2];
    int status;
    int class;
    char *line = NULL;
    ATResponse *atResponse = NULL;

    asprintf(&cmd, "AT+CCWA=1,2,%d",((int *)data)[0]);
    err = at_send_command_singleline(cmd,"+CCWA:",&atResponse);
	if (err != 0) goto error;

	line = atResponse->p_intermediates->line;
	err = at_tok_start(&line);
	if (err < 0) goto error;

    err = at_tok_nextint(&line,&status);
	if (err < 0) goto error;

    err = at_tok_nextint(&line,&class);
	if (err < 0) goto error;

    response[0] = status;
    response[1] = class;

    RIL_onRequestComplete(t, RIL_E_SUCCESS, response, sizeof(response));
    free(cmd);
    return ;

error:
    free(cmd);
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
}

static void requestSetCallWaiting(void *data, size_t datalen, RIL_Token t)
{
    int err;
    char *cmd = NULL;

    asprintf(&cmd, "AT+CCWA=1,%d,%d",((int *)data)[0],((int *)data)[1]);
    err = at_send_command(cmd,NULL);
	if (err != 0) goto error;

	RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
    free(cmd);
    return ;

error:
    free(cmd);
	RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
}

static void requestDtmfStart(void *data, size_t datalen, RIL_Token t)
{
    int err;
    char *cmd;

    asprintf(&cmd,"AT+VTS=%c",((char *)data)[0]);

    err = at_send_command(cmd,NULL);
	if (err != 0)
    	goto error;

	RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
    return ;
error:
	RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
}

static void requestSetMute(void *data, size_t datalen, RIL_Token t)
{
	int err;
    char *cmd = NULL;

	assert (datalen >= sizeof(int *));

    /* mute */
    asprintf(&cmd, "AT+CMUT=%d", ((int*)data)[0]);
    err = at_send_command(cmd, NULL);

    if (err != 0)
    	goto error;

    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
    return;

error:
	LOGE("ERROR: requestSetMute failed");
	RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
}

static void requestGetMute(RIL_Token t)
{
	int err;
	ATResponse *atResponse = NULL;
	int response[1];
	char *line;

	err = at_send_command_singleline("AT+CMUT?", "+CMUT:", &atResponse);

	if (err != 0)
    	goto error;

	line = atResponse->p_intermediates->line;

	err = at_tok_start(&line);
	if (err < 0) goto error;

	err = at_tok_nextint(&line, &response[0]);
	if (err < 0) goto error;

	RIL_onRequestComplete(t, RIL_E_SUCCESS, response, sizeof(response));
	at_response_free(atResponse);

	return;

error:
	LOGE("ERROR: requestGetMute failed");
	RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
	at_response_free(atResponse);
}

/**
 * Funtcion:requestResetRadio
 *
 * Purpose :reset module for JAVA
 *
 * Request : RIL_REQUEST_RESET_RADIO.
 *
 */
static void requestResetRadio(RIL_Token t)
{
	int err = 0;

    /* Reset MS */
	err = at_send_command("AT+QRST=1,0", NULL);
	if(err != 0)
    	goto error;

    /* sleep 5s to wait for reboot */
    sleep(5);

	RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
	return;

error:
    LOGE("[%s]: error for qrst\n", __func__);
	RIL_onRequestComplete(t, RIL_E_REQUEST_NOT_SUPPORTED, NULL, 0);
	return;
}

//Add by wythe on 2013-9-27 ->start

/**
 * Function:request get preferred network type
 *
 * Purpose :Query the preferred network type (CS/PS domain, RAT, and operation mode)
 * for searching and registering.
 *
 * Request :RIL_REQUEST_GET_PREFERRED_NETWORK_TYPE
 */
static void requestGetPreferredNetworkType(RIL_Token t)
{
    /*
     * AT+QCFG="nwscanmode",0       AUTO,WCDMA preferred
     * AT+QCFG="nwscanmode",1       GSM ONLY
     * AT+QCFG="nwscanmode",2       WCDMA ONLY
     */

    ATResponse *atResponse = NULL;
    int err;
    char *line;
    int ret;
    int nwscanmode = PREF_NET_TYPE_GSM_WCDMA;

    if (QL_RIL_VERSION < 6) {
        nwscanmode = ql_is_GSM ? PREF_NET_TYPE_GSM_ONLY : PREF_NET_TYPE_GSM_WCDMA;
        RIL_onRequestComplete(t, RIL_E_SUCCESS, &nwscanmode, sizeof(int));
        return;
    }

    if (ql_is_GSM) {/* For 2G module, it only work in GSM */
        nwscanmode = PREF_NET_TYPE_GSM_ONLY;
        RIL_onRequestComplete(t, RIL_E_SUCCESS, &nwscanmode, sizeof(int));
        return;
    }

    if (ql_is_mdm9215 || ql_is_mdm9x07) { //4G modules
        int get_rat = quectel_qcfg_nwscanmodeex(-1);
        if (get_rat >= 0) {
            RIL_onRequestComplete(t, RIL_E_SUCCESS, &get_rat, sizeof(int));
        	return;
        }
    }

    err = at_send_command_multiline("AT+QCFG=\"nwscanmode\";+QCFG=\"nwscanseq\"", "+QCFG: ", &atResponse);

    if (err != 0) {
        // assume radio is off
    	goto error;
    }

    line = atResponse->p_intermediates->line;

    err = at_tok_start(&line);
    if (err < 0) goto error;

    err = skipComma(&line);
    if (err < 0) goto error;

    err = at_tok_nextint(&line, &ret);
    if (err < 0) goto error;

    /* Based on reported +QCFG: "nwscanmode" */
    if (ret == 1) {
    	nwscanmode = PREF_NET_TYPE_GSM_ONLY;  /* GSM only */
    } else if (ret == 2) {
    	nwscanmode = PREF_NET_TYPE_WCDMA; /* WCDMA only */
    } else {
    	nwscanmode = PREF_NET_TYPE_GSM_WCDMA; /* for 3G Preferred */
    }

    if (ql_is_mdm9215 || ql_is_mdm9x07) { //4G modules
//RILConstants.java (frameworks\base\telephony\java\com\android\internal\telephony)
//int PREFERRED_NETWORK_MODE      = NETWORK_MODE_WCDMA_PREF;
//must change to NETWORK_MODE_LTE_CMDA_EVDO_GSM_WCDMA in order to use 4G
        switch (ret) {
            case 1:    /* GSM ONLY*/
                nwscanmode = PREF_NET_TYPE_GSM_ONLY;
            break;
            case 2: /* WCDMA ONLY*/
                nwscanmode = PREF_NET_TYPE_WCDMA; //3G only
            break;
            case 6: /* CDMA ONLY*/
                nwscanmode = PREF_NET_TYPE_CDMA_ONLY;
            break;
            case 5: /* EvDo ONLY*/
                nwscanmode = PREF_NET_TYPE_EVDO_ONLY; //3G only
            break;
            case 8:
                nwscanmode = PREF_NET_TYPE_CDMA_EVDO_AUTO;
            break;
            case 3: /* LTE ONLY*/
                nwscanmode = PREF_NET_TYPE_LTE_ONLY;
            break;
            case 0:
            default:
                nwscanmode = PREF_NET_TYPE_LTE_GSM_WCDMA;
                //if (strStartsWith(ql_product_version, "EC20CE")) response = PREF_NET_TYPE_LTE_CMDA_EVDO_GSM_WCDMA;
            break;
        }
    }

    RLOGD("requestGetPreferredNetworkType() mode:%d\n",nwscanmode);
    RIL_onRequestComplete(t, RIL_E_SUCCESS, &nwscanmode, sizeof(int));
    at_response_free(atResponse);
    return;

error:
    LOGE("ERROR: requestGetPreferredNetworkType() failed - modem does not support command\n");
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    at_response_free(atResponse);
}
//Add by wythe on 2013-9-27 ->end

struct operatorPollParams {
    RIL_Token t;
    int loopcount;
};


#define REPOLL_OPERATOR_SELECTED 30
static const struct timeval TIMEVAL_OPERATOR_SELECT_POLL = { 2, 0 };

/**
 * Poll +COPS? and return a success, or if the loop counter reaches
 * REPOLL_OPERATOR_SELECTED, return generic failure.
 */
static void pollOperatorSelected(void *params)
{
    int err = 0;
    int response = 0;
    char *line = NULL;
    ATResponse *atResponse = NULL;
    struct operatorPollParams *poll_params;
    RIL_Token t;

    assert(params != NULL);

    poll_params = (struct operatorPollParams *) params;
    t = poll_params->t;

    if (poll_params->loopcount >= REPOLL_OPERATOR_SELECTED)
        goto error;

    err = at_send_command_singleline("AT+COPS?", "+COPS:", &atResponse);
    if (err != 0)
        goto error;

    line = atResponse->p_intermediates->line;

    err = at_tok_start(&line);
    if (err < 0)
        goto error;

    err = at_tok_nextint(&line, &response);
    if (err < 0)
        goto error;

    /* If we don't get more than the COPS: {0-4} we are not registered.
       Loop and try again. */
    if (!at_tok_hasmore(&line)) {
            poll_params->loopcount++;
            RIL_requestTimedCallback(pollOperatorSelected,
                            poll_params, &TIMEVAL_OPERATOR_SELECT_POLL);
    } else {
        /* We got operator, throw a success! */
        RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
        free(poll_params);
    }

    at_response_free(atResponse);
    return;

error:
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    free(poll_params);
    at_response_free(atResponse);
    return;
}

/**
 *  Add handler for RIL_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC
 *  wythe add on 2014-3-28
 */
void requestSetNetworkSelectionAutomatic(RIL_Token t)
{
    int err = 0;
    ATResponse *atResponse = NULL;
    int mode = 0;
    int skip;
    char *line;
    char *netOperator = NULL;
    struct operatorPollParams *poll_params = NULL;

    poll_params = (struct operatorPollParams*)
                	malloc(sizeof(struct operatorPollParams));
    if (NULL == poll_params)
        goto error;

    /* First check if we are already scanning or in manual mode */
    err = at_send_command_singleline("AT+COPS=3,2;+COPS?", "+COPS:", &atResponse);
    if (err != 0)
        goto error;

    line = atResponse->p_intermediates->line;

    err = at_tok_start(&line);
    if (err < 0)
        goto error;

    /* Read network selection mode */
    err = at_tok_nextint(&line, &mode);
    if (err < 0)
        goto error;

    /* If we're unregistered, we may just get
       a "+COPS: 0" response. */
    if (!at_tok_hasmore(&line)) {
        if (mode == 1) {
            LOGD("%s() Changing manual to automatic network mode", __func__);
            goto do_auto;
        } else
            goto check_reg;
    }

    err = at_tok_nextint(&line, &skip);
    if (err < 0)
        goto error;

    /* A "+COPS: 0, n" response is also possible. */
    if (!at_tok_hasmore(&line)) {
        if (mode == 1) {
            LOGD("%s() Changing manual to automatic network mode", __func__);
            goto do_auto;
        } else
            goto check_reg;
    }

    /* Read numeric operator */
    err = at_tok_nextstr(&line, &netOperator);
    if (err < 0)
        goto error;

    /* If operator is found then do a new scan,
       else let it continue the already pending scan */
    if (netOperator && strlen(netOperator) == 0) {
        if (mode == 1) {
            LOGD("%s() Changing manual to automatic network mode", __func__);
            goto do_auto;
        } else
            goto check_reg;
    }

    /* Operator found */
    if (mode == 1) {
        LOGD("%s() Changing manual to automatic network mode", __func__);
        goto do_auto;
    } else {
        LOGD("%s() Already in automatic mode with known operator, trigger a new network scan", __func__);
#if 1
//?????Ѿ???autoģʽ?ˣ????Ѿ?ע?????????ˡ?Ӧ??û??Ҫ??????????һ??autoģʽ
//?Ͼ????????ܣ????󲿷ֳ????¶??ò?????????AT+COPS=0 ?Ứ?Ѻܳ?ʱ?䣬????????????????τ
        RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
        return;
#endif
        goto do_auto;
    }

    /* Check if module is scanning,
       if not then trigger a rescan */
check_reg:
    at_response_free(atResponse);
    atResponse = NULL;

    /* Check CS domain first */
    err = at_send_command_singleline("AT+CREG?", "+CREG:", &atResponse);
    if (err != 0)
        goto error;

    line = atResponse->p_intermediates->line;

    err = at_tok_start(&line);
    if (err < 0)
        goto error;

    /* Read registration unsolicited mode */
    err = at_tok_nextint(&line, &mode);
    if (err < 0)
        goto error;

    /* Read registration status */
    err = at_tok_nextint(&line, &mode);
    if (err < 0)
        goto error;

    /* If scanning has stopped, then perform a new scan */
    if (mode == 0) {
        LOGD("%s() Already in automatic mode, but not currently scanning on CS,"
         "trigger a new network scan", __func__);
        goto do_auto;
    }

    /* Now check PS domain */
    at_response_free(atResponse);
    atResponse = NULL;
    err = at_send_command_singleline("AT+CGREG?", "+CGREG:", &atResponse);
    if (err != 0)
        goto error;

    line = atResponse->p_intermediates->line;

    err = at_tok_start(&line);
    if (err < 0)
        goto error;

    /* Read registration unsolicited mode */
    err = at_tok_nextint(&line, &mode);
    if (err < 0)
        goto error;

    /* Read registration status */
    err = at_tok_nextint(&line, &mode);
    if (err < 0)
        goto error;

    /* If scanning has stopped, then perform a new scan */
    if (mode == 0) {
        LOGD("%s() Already in automatic mode, but not currently scanning on PS,"
         "trigger a new network scan", __func__);
        goto do_auto;
    }
    else
    {
        LOGD("%s() Already in automatic mode and scanning", __func__);
        goto finish_scan;
    }

do_auto:
    at_response_free(atResponse);
    atResponse = NULL;

    /* This command does two things, one it sets automatic mode,
       two it starts a new network scan! */
    err = at_send_command("AT+COPS=0", NULL);
    if (err != 0)
        goto error;
    if (s_lastPreferredNetworkType != -1) {
        requestSetPreferredNetworkType(&s_lastPreferredNetworkType, sizeof(int *), NULL);
    }

finish_scan:

    at_response_free(atResponse);
    atResponse = NULL;

    poll_params->loopcount = 0;
    poll_params->t = t;

    RIL_requestTimedCallback(pollOperatorSelected,
                    poll_params, &TIMEVAL_OPERATOR_SELECT_POLL);

    return;

error:
    free(poll_params);
    at_response_free(atResponse);
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    return;
}

/**
 *  Add handler for RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL
 *  wythe add on 2014-3-28
 *  Set the network selection manually.
 */
void requestSetNetworkSelectionManual(void *data, size_t datalen,
                                      RIL_Token t)
{
    /*
     * AT+COPS=[<mode>[,<format>[,<oper>[,<AcT>]]]]
     *    <mode>   = 4 = Manual (<oper> field shall be present and AcT optionally) with fallback to automatic if manual fails.
     *    <format> = 2 = Numeric <oper>, the number has structure:
     *                   (country code digit 3)(country code digit 2)(country code digit 1)
     *                   (network code digit 2)(network code digit 1)
     */

    (void) datalen;
    int err = 0;
    	char *cmd = NULL;
    const char *mccMnc = (const char *) data;

    /* Check inparameter. */
    if (mccMnc == NULL)
        goto error;

    /* Build and send command. */
    asprintf(&cmd, "AT+COPS=1,2,\"%s\"", mccMnc);
    err = at_send_command(cmd, NULL);
    if (err != 0) {
        err = at_send_command("AT+COPS=0",NULL);
        if(err != 0)
            goto error;
        if (s_lastPreferredNetworkType != -1) {
            requestSetPreferredNetworkType(&s_lastPreferredNetworkType, sizeof(int *), NULL);
        }
    }

    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
    return;

error:
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
}

/**
 *  Add handler for RIL_REQUEST_QUERY_AVAILABLE_NETWORKS
 *  Wythe 2014-3-28 -> start
 *  Query available network to system. It may takes long
 *  time to scan BaseStation around.
 */
static void requestQueryAvailableNetworks(RIL_Token t)
{
    /**
     * AT+COPS=?
     *   +COPS:[(list of supported<stat>,
     *          long alphanumeric <oper>,
                short alphanumeric <oper>,
                numeric <oper>s[,<Act>]s)]
                [,,(list supported <mode>s),(list of supported <format>s)]
     *
     * <stat>   0   Unknown
     *          1   Operator available
     *          2   Operator current
     *          3   Operator forbidden
     */

    /**
     * long alphanumeric
     * short alphanumeric
     * numeric
     * stat
     */
    #define QUERY_AVAILABLE_NW_PARAMS_NUM   4

    const char *statusTable[]=
    {"unknown", "available", "current", "forbidden"};

    ATResponse *atResponse = NULL;
    int err = 0;
    char *line = NULL;
    int n = 0;
    char **responseArray = NULL;
    int i = 0;

    RIL_RadioTechnology radio_tech = quectel_at_qnwinfo();

    if(ql_is_mdm9x07 && (RADIO_TECH_1xRTT == radio_tech)) {
        at_send_command("at+qcfg=\"nwscanmode\",3", NULL);
    }

    err = at_send_command_singleline("AT+COPS=?", "+COPS", &atResponse);

    if(ql_is_mdm9x07 && (RADIO_TECH_1xRTT == radio_tech)) {
        at_send_command("at+qcfg=\"nwscanmode\",0", NULL);
    }

    if(err != 0 || atResponse->p_intermediates == NULL){
        LOGE("%s:%s It's error when querying the available network\n", __func__, line);
        goto error;
    }

    line = atResponse->p_intermediates->line;

    if(line == NULL)
        goto error;

    LOGD("%s:Read COPS response = %s\n", __func__, line);

    err = at_tok_start(&line);
    if(err < 0) goto error;

    err = at_tok_charcounter(line, "(", &n);
    if(err < 0) goto error;

    //Do not need (list supported <mode>s), (list supported <format>s) here
    if(n < 2) n = 0;
    else n -= 2;

    	LOGD("%s:Get %d operator information from modem", __func__, n);

    responseArray = (char **)alloca(n * QUERY_AVAILABLE_NW_PARAMS_NUM * sizeof(char *));
    	if(NULL == responseArray) goto error;

    for(i = 0; i < n; i++)
    {
        char *s = NULL;
        char *p_line = NULL;
        char *remaining = NULL;
        char *long_alphanumeric = NULL;
        char *short_alphanumeric = NULL;
        char *numeric = NULL;
        int status = 0;
            	int act = 0;

        //get the content in (), like ("CHN-UNICOM")
        s = p_line = at_tok_getElementValue(line, "(", ")", &remaining);
        line = remaining;

        if(p_line == NULL)
        {
            LOGE("ERROR:%s NULL pointer while parsing COPS response\n", __func__);
            break;
        }

        //stat <oper>
        err = at_tok_nextint(&p_line, &status);
        if(err < 0)
        {
error_response:
            free(s);
            goto error;
        }

#if 0
                // If the operator's status is unknown or forbidden, don't pass it to RIL java.
            	if(0 == status || 3 == status)
                {
                    	free(s);
                    	continue;
                }
#endif
        //long alphanumeric <oper>
        err = at_tok_nextstr(&p_line, &long_alphanumeric);
        if(err < 0) goto error_response;

        //short alphanumeric <oper>
        err = at_tok_nextstr(&p_line, &short_alphanumeric);
        if(err < 0) goto error_response;

        //numeric <oper>
        err = at_tok_nextstr(&p_line, &numeric);
        if(err < 0) goto error_response;

                // Act <oper>
            	err = at_tok_nextint(&p_line, &act);
        if(err < 0) {
            if (ql_is_GSM) /* For 2G module, it only work in GSM */
               act = 0;
            else
                goto error_response;
        }

        //add long alphanumeric in array[0]
        responseArray[i*QUERY_AVAILABLE_NW_PARAMS_NUM + 0] = (char *)alloca(strlen(long_alphanumeric)+1);
        strcpy(responseArray[i*QUERY_AVAILABLE_NW_PARAMS_NUM + 0], long_alphanumeric);

        //add short alphanumeric in array[1]
        responseArray[i*QUERY_AVAILABLE_NW_PARAMS_NUM + 1] = (char *)alloca(strlen(short_alphanumeric)+1);
        strcpy(responseArray[i*QUERY_AVAILABLE_NW_PARAMS_NUM + 1], short_alphanumeric);

        //add numeric in array[2]
        responseArray[i*QUERY_AVAILABLE_NW_PARAMS_NUM + 2] = (char *)alloca(strlen(numeric)+1);
        strcpy(responseArray[i*QUERY_AVAILABLE_NW_PARAMS_NUM + 2], numeric);

        free(s);

        /**
         *  If module responds empty sring for long alphanumeric and short alphanumeric,
         *  then we copy the MNC/MMC to them.
         */

        if (responseArray[i * QUERY_AVAILABLE_NW_PARAMS_NUM + 0] && strlen(responseArray[i * QUERY_AVAILABLE_NW_PARAMS_NUM + 0]) == 0) {
            responseArray[i * QUERY_AVAILABLE_NW_PARAMS_NUM + 0] = (char*) alloca(strlen(responseArray[i * QUERY_AVAILABLE_NW_PARAMS_NUM + 2]) + 1);
            strcpy(responseArray[i * QUERY_AVAILABLE_NW_PARAMS_NUM + 0], responseArray[i * QUERY_AVAILABLE_NW_PARAMS_NUM + 2]);
        }

        if (responseArray[i * QUERY_AVAILABLE_NW_PARAMS_NUM + 1] && strlen(responseArray[i * QUERY_AVAILABLE_NW_PARAMS_NUM + 1]) == 0) {
            responseArray[i * QUERY_AVAILABLE_NW_PARAMS_NUM + 1] = (char*) alloca(strlen(responseArray[i * QUERY_AVAILABLE_NW_PARAMS_NUM + 2]) + 1);
            strcpy(responseArray[i * QUERY_AVAILABLE_NW_PARAMS_NUM + 1], responseArray[i * QUERY_AVAILABLE_NW_PARAMS_NUM + 2]);
        }

                //add status in array[3]
        responseArray[i * QUERY_AVAILABLE_NW_PARAMS_NUM + 3] = (char*) alloca(strlen(statusTable[status])+1);
            	strcpy(responseArray[i * QUERY_AVAILABLE_NW_PARAMS_NUM + 3],statusTable[status]);
    }

    RIL_onRequestComplete(t, RIL_E_SUCCESS, responseArray, i * QUERY_AVAILABLE_NW_PARAMS_NUM * sizeof(char *));

finally:
    at_response_free(atResponse);
    return;

error:
    LOGE("ERROR:requestQueryAvailableNetworks() get no response form module\n");
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
	goto finally;
}

/**
 *  Wythe 2014-3-28 -> end
 */

#define MAX_NUM_NEIGHBOR_CELLS 10

void requestGetNeighboringCellIDs(RIL_Token t)
{
    int err = 0;
    char *p = NULL;
    int n = 0;
    ATLine *tmp = NULL;
    ATResponse *atresponse = NULL;
    RIL_NeighboringCell *ptr_cells[MAX_NUM_NEIGHBOR_CELLS];

    err = at_send_command_multiline("AT+QENG=\"neighbourcell\"", "+QENG:", &atresponse);
    if (err < 0 || atresponse->success == 0)
        goto error;

    tmp = atresponse->p_intermediates;
    while (tmp)
    {
        if (n > MAX_NUM_NEIGHBOR_CELLS)
            goto error;
        p = tmp->line;
        {
            char *line = p;
            int lac = 0;
            int cellid = 0;
            int rssi = 0;
            int psc = 0;
            //int pathloss = 0;
            char *net_type;
            char *tmp_str;
            int tmp_int;

            err = at_tok_start(&line);
            if (err < 0)
                goto error;

            /* neighbourcell */
            err = at_tok_nextstr(&line, &tmp_str);
            if (err < 0)
                goto error;

            /* 3G/2G */
            err = at_tok_nextstr(&line, &net_type);
            if (err < 0)
                goto error;

            /* mmc */
            err = at_tok_nextint(&line, &tmp_int);
            if (err < 0)
            {
                LOGE("get mmc error");
                goto next;
            }

            /* mnc */
            err = at_tok_nextint(&line, &tmp_int);
            if (err < 0)
            {
                LOGE("get mnc error");
                goto next;
            }

            /* lac */
            //err = at_tok_nextint(&line, &lac);
            err = at_tok_nexthexint(&line, &lac);
            if (err < 0)
            {
                LOGE("get lac error");
                goto next;
            }

            /* cellid */
            //err = at_tok_nextint(&line, &cellid);
            err = at_tok_nexthexint(&line, &cellid);
            if (err < 0)
            {
                LOGE("get cellid error");
                goto next;
            }

            /* bsic/uarfcn */
            err = at_tok_nextint(&line, &tmp_int);
            if (err < 0)
            {
                LOGE("get bsic/uarfcn error");
                goto next;
            }

            /* psc/arfcn */
            err = at_tok_nextint(&line, &psc);
            if (err < 0)
            {
                LOGE("get psc/arfcn error");
                goto next;
            }

            /* rscp/rxlev */
            err = at_tok_nextint(&line, &rssi);
            if (err < 0)
            {
                LOGE("get rscp/rxlev error");
                goto next;
            }

            /* process data for each cell */
            ptr_cells[n] = alloca(sizeof(RIL_NeighboringCell));
            ptr_cells[n]->rssi = rssi;
            ptr_cells[n]->cid = alloca(9 * sizeof(char));
            if(!strncmp(net_type,"3G",2))
                sprintf(ptr_cells[n]->cid, "%08x", psc);
            else //2G
                sprintf(ptr_cells[n]->cid, "%08x", ((lac << 16) + cellid));
            LOGI("CID:%s  RSSI:%d",ptr_cells[n]->cid,ptr_cells[n]->rssi);
            n++;
        }
next:
        tmp = tmp->p_next;
    }

    RIL_onRequestComplete(t, RIL_E_SUCCESS, ptr_cells,n * sizeof(RIL_NeighboringCell *));

finally:
        at_response_free(atresponse);
    return;

error:
    LOGE("%s error",__func__);
    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
    goto finally;
}

#ifdef RIL_REQUEST_GET_CELL_INFO_LIST
static uint64_t ril_nano_time() {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec * 1000000000LL + now.tv_nsec;
}

static int qeng_rssi_to_csq(int rssi) {
/* Signal level conversion result codes */
#define RSSI_TOOLO_CODE   0
#define RSSI_TOOHI_CODE   31
#define RSSI_UNKNOWN_CODE 99

/* RSSI range conversion */
#define RSSI_MIN        51   /* per 3GPP 27.007  (negative value) */
#define RSSI_MAX        113  /* per 3GPP 27.007  (negative value) */
#define RSSI_NO_SIGNAL  125  /* from CM */
#define RSSI_OFFSET       182.26
#define RSSI_SLOPE        (-100.0/62.0)

  int siglvl;
  int scale = RSSI_TOOHI_CODE;

  /* Note: RSSI dbm is really a negative value        */
  if ((RSSI_MIN < rssi) &&
      (RSSI_MAX > rssi))
  {
    /* Round to nearest integer on scale */
    siglvl = (uint8)floor(((rssi * RSSI_SLOPE + RSSI_OFFSET) *
                            scale)/100 + 0.5);
  }
  else if ((RSSI_MAX <= rssi) && 
           (RSSI_NO_SIGNAL != rssi)) 
  {
    siglvl = RSSI_TOOLO_CODE;
  }
  else if (RSSI_MIN >= rssi)
  {
    siglvl = RSSI_TOOHI_CODE;  /* Capped maximum */
  }
  else
  {
    //DS_AT_MSG1_MED("RSSI outside signal range: %d", rssi);
    siglvl = RSSI_UNKNOWN_CODE;
  }

  return siglvl;
}

#define at_tok_check_err(err) do { if (err < 0) { LOGE("%s %d at_tok_error = %d", __func__, __LINE__, err); goto error;}} while(0)
void requestGetCellInfoList(void *data, size_t datalen, RIL_Token t) {
    int err = 0;
    ATLine *p_cur;
    ATResponse *p_response = NULL;
    unsigned int n = 0;
    RIL_CellInfo ci[6];

    err = at_send_command_multiline("AT+QENG=\"servingcell\"", "+QENG:", &p_response);
    if (err < 0 || p_response == NULL || p_response->success == 0)
        goto error;

    p_cur = p_response->p_intermediates;
    while ((p_cur != NULL) && (n < (sizeof(ci)/sizeof(ci[0])))) {
        char *line = p_cur->line;
        char *celltype;
        char *state;
        char *rat;

        memset(&ci[n], 0x00, sizeof(ci[n]));
        
        err = at_tok_start(&line);
        at_tok_check_err(err);

        err = at_tok_nextstr(&line, &celltype);
        at_tok_check_err(err);

        if (celltype == NULL || strcmp(celltype, "servingcell"))
            goto error;

        err = at_tok_nextstr(&line, &state);
        at_tok_check_err(err);

        if (state == NULL)
            goto error;
        
        ci[n].registered = (!strcmp(state, "NOCONN")) || (!strcmp(state, "CONNECT"));

        err = at_tok_nextstr(&line, &rat);
        at_tok_check_err(err);

        if (rat == NULL)
            goto error;
        
        if (!strcmp(rat, "GSM"))
            ci[n].cellInfoType = RIL_CELL_INFO_TYPE_GSM;
        else if (!strcmp(rat, "WCDMA"))
            ci[n].cellInfoType = RIL_CELL_INFO_TYPE_WCDMA;
        else if (!strcmp(rat, "TDSCDMA"))
            ci[n].cellInfoType = RIL_CELL_INFO_TYPE_TD_SCDMA;
        else if (!strcmp(rat, "CDMA"))
            ci[n].cellInfoType = RIL_CELL_INFO_TYPE_CDMA; 
        else if (!strcmp(rat, "HDR"))
            ci[n].cellInfoType = RIL_CELL_INFO_TYPE_CDMA;        
        else if (!strcmp(rat, "LTE"))
            ci[n].cellInfoType = RIL_CELL_INFO_TYPE_LTE;   
        else
            goto error;

        ci[n].timeStampType = RIL_TIMESTAMP_TYPE_OEM_RIL;
        ci[n].timeStamp = ril_nano_time();
       
//"servingscell",<state>,"GSM",<mcc>,<mnc>,<lac>,<cellid>,<bsic>,<arfcn>,<band>,<rxlev>,<txp>,<rla>,<drx>,<c1>,<c2>,<gprs>,<tch>,<ts>,<ta>,<maio>,<hsn>,<rxlevsub>,<rxlevfull>,<rxqualsub>,<rxqualfull>,<voicecodec>
//"servingcell","NOCONN","GSM",460,01,5504,2B55,52,123,0,-111,5,14,64,0,0,0,-,-,-,-,-,-,-,-,-,
//"servingcell","NOCONN","GSM",460,01,5504,2B55,52,123,-,-48,255,255,0,49,47,1,-,-,-,-,-,-,-,-,-,"-"
        if (ci[n].cellInfoType == RIL_CELL_INFO_TYPE_GSM) {
            int mcc, mnc, lac, cid, bsic, arfcn, band, rxlev;

            err = at_tok_nextint(&line, &mcc);
            at_tok_check_err(err);
            err = at_tok_nextint(&line, &mnc);
            at_tok_check_err(err);
            err = at_tok_nexthexint(&line, &lac);
            at_tok_check_err(err);
            err = at_tok_nexthexint(&line, &cid);
            at_tok_check_err(err);
            err = at_tok_nextint(&line, &bsic);
            at_tok_check_err(err);
            err = at_tok_nextint(&line, &arfcn);
            at_tok_check_err(err);
            err = at_tok_nextint(&line, &band);
            err = at_tok_nextint(&line, &rxlev);
            at_tok_check_err(err);

            ci[n].CellInfo.gsm.cellIdentityGsm.mcc = mcc;
            ci[n].CellInfo.gsm.cellIdentityGsm.mnc = mnc;
            ci[n].CellInfo.gsm.cellIdentityGsm.lac = lac;
            ci[n].CellInfo.gsm.cellIdentityGsm.cid = cid;

            ci[n].CellInfo.gsm.signalStrengthGsm.signalStrength = qeng_rssi_to_csq(-rxlev);            
            ci[n].CellInfo.gsm.signalStrengthGsm.bitErrorRate = 0;            
        }
//"servingcell",<state>,"WCDMA",<mcc>,<mnc>,<lac>,<cellid>,<uarfcn>,<psc>,<trac>,<rscp>,<ecio>,<phych>,<sf>,<slot>,<speech_code>,<comMod> 
//"servingcell","NOCONN","WCDMA",460,01,D5D6,8062AF1,10713,380,11,-67,-2,-,-,-,-,-
        else if (ci[n].cellInfoType == RIL_CELL_INFO_TYPE_WCDMA) {
            int mcc, mnc, lac, cid, uarfcn, psc, trac, rscp;

            err = at_tok_nextint(&line, &mcc);
            at_tok_check_err(err);
            err = at_tok_nextint(&line, &mnc);
            at_tok_check_err(err);
            err = at_tok_nexthexint(&line, &lac);
            at_tok_check_err(err);
            err = at_tok_nexthexint(&line, &cid);
            at_tok_check_err(err);
            err = at_tok_nextint(&line, &uarfcn);
            at_tok_check_err(err);
            err = at_tok_nextint(&line, &psc);
            at_tok_check_err(err);
            err = at_tok_nextint(&line, &trac);
            at_tok_check_err(err);
            err = at_tok_nextint(&line, &rscp);
            at_tok_check_err(err);

            ci[n].CellInfo.wcdma.cellIdentityWcdma.mcc = mcc;
            ci[n].CellInfo.wcdma.cellIdentityWcdma.mnc = mnc;
            ci[n].CellInfo.wcdma.cellIdentityWcdma.lac = lac;
            ci[n].CellInfo.wcdma.cellIdentityWcdma.cid = cid;
            ci[n].CellInfo.wcdma.cellIdentityWcdma.psc = psc;
            
            ci[n].CellInfo.wcdma.signalStrengthWcdma.signalStrength = qeng_rssi_to_csq(-rscp);            
            ci[n].CellInfo.wcdma.signalStrengthWcdma.bitErrorRate = 0;            
        }
//"servingcell",<state>,"TDSCDMA",<mcc>,<mnc>,<lac>,<cellid>,<pfreq>,<rssi>,<rscp>,<ecio>
        else if (ci[n].cellInfoType == RIL_CELL_INFO_TYPE_TD_SCDMA) {
            int mcc, mnc, lac, cellid, pfreq, rssi, rscp;
            
            err = at_tok_nextint(&line, &mcc);
            at_tok_check_err(err);
            err = at_tok_nextint(&line, &mnc);
            at_tok_check_err(err);
            err = at_tok_nexthexint(&line, &lac);
            at_tok_check_err(err);
            err = at_tok_nexthexint(&line, &cellid);
            at_tok_check_err(err);
            err = at_tok_nextint(&line, &pfreq);
            at_tok_check_err(err);
            err = at_tok_nextint(&line, &rssi);
            at_tok_check_err(err);
            err = at_tok_nextint(&line, &rscp);
            at_tok_check_err(err);
            
            ci[n].CellInfo.tdscdma.cellIdentityTdscdma.mcc = mcc;
            ci[n].CellInfo.tdscdma.cellIdentityTdscdma.mnc = mnc;
            ci[n].CellInfo.tdscdma.cellIdentityTdscdma.lac = lac;
            ci[n].CellInfo.tdscdma.cellIdentityTdscdma.cid = cellid;
            ci[n].CellInfo.tdscdma.cellIdentityTdscdma.cpid = INT_MAX;

            ci[n].CellInfo.tdscdma.signalStrengthTdscdma.rscp = rscp;
        }
//"servingscell",<state>,<rat>,<mcc>,<mnc>,<sid>,<nbid>,<channel>,<rxpwr>,<ecio>,<txpwr>
//"servingcell","NOCONN","CDMA",460,03,3747,A221F,283,0,0,0
//"servingcell","NOCONN","HDR",460,03,3747,A221F,37,-65,-45,-30
        else if (ci[n].cellInfoType == RIL_CELL_INFO_TYPE_CDMA) {
            int mcc, mnc, sid, nbid, channel, rxpwr, ecio, txpwr;

            err = at_tok_nextint(&line, &mcc);
            at_tok_check_err(err);
            err = at_tok_nextint(&line, &mnc);
            at_tok_check_err(err);
            err = at_tok_nexthexint(&line, &sid);
            at_tok_check_err(err);
            err = at_tok_nexthexint(&line, &nbid);
            at_tok_check_err(err);
            err = at_tok_nextint(&line, &channel);
            at_tok_check_err(err);
            err = at_tok_nextint(&line, &rxpwr);
            at_tok_check_err(err);
            err = at_tok_nextint(&line, &ecio);
            at_tok_check_err(err);
            err = at_tok_nextint(&line, &txpwr);
            at_tok_check_err(err);
            
            ci[n].CellInfo.cdma.cellIdentityCdma.networkId = nbid >> 16;
            ci[n].CellInfo.cdma.cellIdentityCdma.systemId = sid;
            ci[n].CellInfo.cdma.cellIdentityCdma.basestationId = nbid & 0xffff;
            ci[n].CellInfo.cdma.cellIdentityCdma.longitude = INT_MAX;
            ci[n].CellInfo.cdma.cellIdentityCdma.latitude = INT_MAX;

            ci[n].CellInfo.cdma.signalStrengthCdma.dbm = rxpwr;
            ci[n].CellInfo.cdma.signalStrengthCdma.ecio = ecio;
            ci[n].CellInfo.cdma.signalStrengthEvdo.dbm = rxpwr;
            ci[n].CellInfo.cdma.signalStrengthEvdo.ecio = ecio;
            ci[n].CellInfo.cdma.signalStrengthEvdo.signalNoiseRatio = 0;
        }
//"servingcell",<state>,"LTE",<is_tdd>,<mcc>,<mnc>,<cellid>,<pcid>,<earfcn>,<freq_band_ind>,<ul_bandwidth>,<dl_bandwidth>,<tac>,<rsrp>,<rsrq>,<rssi>,<sinr><srxlev>
//+QENG: "servingcell","NOCONN","LTE","FDD",460,11,6935932,30,1825,3,4,4,6934,-75,-9,-47,25,52
//+QENG: "servingcell","NOCONN","LTE","FDD",460,01,5A8A10B,243,1650,3,5,5,550D,-71,-11,-40,14,56
        else if (ci[n].cellInfoType == RIL_CELL_INFO_TYPE_LTE) {
            char *is_tdd;
            int mcc, mnc, cellid, pcid, earfcn, freq_band_ind, ul_bandwidth, dl_bandwidth, tac, rsrp, rsrq, rssi, sinr;

            err = at_tok_nextstr(&line, &is_tdd);
            at_tok_check_err(err);
            err = at_tok_nextint(&line, &mcc);
            at_tok_check_err(err);
            err = at_tok_nextint(&line, &mnc);
            at_tok_check_err(err);
            err = at_tok_nexthexint(&line, &cellid);
            at_tok_check_err(err);
            err = at_tok_nextint(&line, &pcid);
            at_tok_check_err(err);
            err = at_tok_nextint(&line, &earfcn);
            at_tok_check_err(err);
            err = at_tok_nextint(&line, &freq_band_ind);
            at_tok_check_err(err);
            err = at_tok_nextint(&line, &ul_bandwidth);
            at_tok_check_err(err);
            err = at_tok_nextint(&line, &dl_bandwidth);
            at_tok_check_err(err);
            err = at_tok_nexthexint(&line, &tac);
            at_tok_check_err(err);
            err = at_tok_nextint(&line, &rsrp);
            at_tok_check_err(err);
            err = at_tok_nextint(&line, &rsrq);
            at_tok_check_err(err);
            err = at_tok_nextint(&line, &rssi);
            at_tok_check_err(err);
            err = at_tok_nextint(&line, &sinr);
            at_tok_check_err(err);
            
            ci[n].CellInfo.lte.cellIdentityLte.mcc = mcc;
            ci[n].CellInfo.lte.cellIdentityLte.mnc = mnc;
            ci[n].CellInfo.lte.cellIdentityLte.pci = pcid;
            ci[n].CellInfo.lte.cellIdentityLte.ci = cellid;
            ci[n].CellInfo.lte.cellIdentityLte.tac = tac;
            
            ci[n].CellInfo.lte.signalStrengthLte.signalStrength = qeng_rssi_to_csq(-rsrp);
            ci[n].CellInfo.lte.signalStrengthLte.rsrp = -rsrp;
            ci[n].CellInfo.lte.signalStrengthLte.rsrq = -rsrq;
            ci[n].CellInfo.lte.signalStrengthLte.rssnr = sinr*10; 
            ci[n].CellInfo.lte.signalStrengthLte.cqi = INT_MAX;
            ci[n].CellInfo.lte.signalStrengthLte.timingAdvance = INT_MAX;
        }
        else {
            goto error;;
        }

        n++;
        p_cur = p_cur->p_next;
   }

    at_response_free(p_response);
    
error:
    RIL_onRequestComplete(t, n ? RIL_E_SUCCESS : RIL_E_GENERIC_FAILURE, ci, n*sizeof(RIL_CellInfo));
}
#endif

#ifdef RIL_REQUEST_GET_HARDWARE_CONFIG
static void requestGetHardwareConfig(void *data, size_t datalen, RIL_Token t)
{
#if 1
   RIL_onRequestComplete(t, RIL_E_REQUEST_NOT_SUPPORTED, NULL, 0);
#else
   // TODO - hook this up with real query/info from radio.

   RIL_HardwareConfig hwCfg;

   RIL_UNUSED_PARM(data);
   RIL_UNUSED_PARM(datalen);

   hwCfg.type = -1;

   RIL_onRequestComplete(t, RIL_E_SUCCESS, &hwCfg, sizeof(hwCfg));
#endif
}
#endif

#ifdef RIL_REQUEST_GET_HARDWARE_CONFIG
static void requestShutdown(RIL_Token t)
{
    int onOff;

    int err;
    ATResponse *p_response = NULL;

    if (sState != RADIO_STATE_OFF) {
        err = at_send_command("AT+CFUN=0", &p_response);
        setRadioState(RADIO_STATE_UNAVAILABLE);
    }

    at_response_free(p_response);
    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
    return;
}
#endif

#ifdef RIL_REQUEST_SET_UNSOL_CELL_INFO_LIST_RATE
static void requestSetCellInfoListRate(void *data, size_t datalen, RIL_Token t)
{
    // For now we'll save the rate but no RIL_UNSOL_CELL_INFO_LIST messages
    // will be sent.
    assert (datalen == sizeof(int));
    //s_cell_info_rate_ms = ((int *)data)[0];

    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
}
#endif

#ifdef RIL_REQUEST_DEVICE_IDENTITY
static void requestDeviceIdentity(RIL_Token t)
{
    int err = 0;
    int i = 0;
	int ret = 0;
    ATLine *p_cur = NULL;
	char *line = NULL;
	char *ptmp = NULL;
#define IDENTITY_NUM 4
    char *response[IDENTITY_NUM] = {0};
	char *at_cmd[IDENTITY_NUM] = {"AT+CGSN", "AT+CGMR", "AT+EGMR=0,8", "AT+EGMR=0,8"};
#define MEID_LEN 16
	char meid[MEID_LEN + 1] = {0};

    ATResponse *p_response = NULL;
    ATResponse *cgmr_response = NULL;

    LOGE("%s begin.\n", __func__);
#if 1
    err = at_send_command_numeric("AT+CGSN", &p_response);
    if (err < 0 || p_response == NULL ||  0 == p_response->success) {
        LOGE("%s error for AT+CGSN!\n", __func__);
        ret--;
    } else {
        response[0] = p_response->p_intermediates->line;
        //response[2] = p_response->p_intermediates->line;
        memcpy(meid, p_response->p_intermediates->line, MEID_LEN);
        ptmp = (char *)(&meid[0]);
        while ((NULL != ptmp) && ('\0' != *ptmp)) ptmp++;
        if ((NULL != ptmp) && ('\0' != *(ptmp - 2))) {
            *(ptmp - 2) = 0;
        }
        response[3] = (char *)(&meid[0]);
    }

    err = at_send_command_singleline("AT+CGMR", "\0", &cgmr_response);
    if ((err < 0) || cgmr_response == NULL || 0 == cgmr_response->success) {
        LOGE("%s error for AT+CGMR!\n", __func__);
        ret--;
    } else {
            response[1] = cgmr_response->p_intermediates->line;
    }
#else
	for (i = 0; i < IDENTITY_NUM; i++) {
    	err = at_send_command(at_cmd[i], &p_response);

        if ((err < 0) || (0 == p_response->success)) {
            LOGE("%s error for %s!\n", __func__, at_cmd[i]);
        	if ((2 == i) || (3 == i)) {
            	response[i] = "80C93228";  //dumy esn or meid
            } else {
                ret--;
            }
        } else {
        	if ((2 == i) || (3 == i)) {
            	line = p_response->p_intermediates->line;
            	err = at_tok_start(&line);
            	err = at_tok_nextstr(&line, &ptmp);
            	if ((0 == err) && (NULL != ptmp)) {
                    if (0 == strncmp(ptmp, "0x", strlen("0x"))) {
                        ptmp += 2; //skip "0x"
                    }
                }
            	response[i] = ptmp;
            	while ((NULL != ptmp) && ('"' != *ptmp) && (0 != *ptmp)) {
                    ptmp++;
                }
            	if ('"' == *ptmp) {
                    *ptmp = 0;
                }
            } else {
                response[i] = p_response->p_intermediates->line;
            }
        	LOGE("responese[%d] = %s\n", i, (NULL != response[i]) ? response[i] : "");
        }

    }
    #endif
	for (i = 0; i < IDENTITY_NUM; i++) {
        LOGE("%s : response[%d] = %s\n", __func__, i, (NULL != response[i]) ? response[i] : "");
    }
    if (ret < 0) {
        RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, response, sizeof(response));
    } else {
        RIL_onRequestComplete(t, RIL_E_SUCCESS, response, sizeof(response));
    }
    at_response_free(p_response);
	at_response_free(cgmr_response);
    LOGE("%s end, ret = %d\n", __func__, ret);
    return;

}
#endif

/*** Callback methods from the RIL library to us ***/

/**
 * Call from RIL to us to make a RIL_REQUEST
 *
 * Must be completed with a call to RIL_onRequestComplete()
 *
 * RIL_onRequestComplete() may be called from any thread, before or after
 * this function returns.
 *
 * Will always be called from the same thread, so returning here implies
 * that the radio is ready to process another command (whether or not
 * the previous command has completed).
 */
static void
onRequest (int request, void *data, size_t datalen, RIL_Token t)
{
    ATResponse *p_response;
    int err;
    int cops_response[4] = {0};

#ifdef USE_NDK
    if (requestToString != NULL)
#endif
    LOGD("onRequest: %s", requestToString(request));

#if 1 //quectel  //frameworks\base\telephony\java\com\android\internal\telephony/RIL.java ->processUnsolicited()
    onRequestCount++;
#endif
    switch (request) {

        /**** call ****/
        //--------------------------------------------------------
        case RIL_REQUEST_GET_CURRENT_CALLS:
            if (currentState() <= RADIO_STATE_UNAVAILABLE) {
                RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
                break;
            }
            requestGetCurrentCalls(data, datalen, t);
            break;

        case RIL_REQUEST_DIAL:
            dialing_phone_type = ql_get_phonetype();
            requestDial(data, datalen, t);
            break;

        case RIL_REQUEST_HANGUP:
            if (ql_is_cdma()) {
                if (dialing_phone_type == PHONE_TYPE_GSM && QL_RIL_VERSION == 9) {
                    LOGD("%s RIL_REQUEST_HANGUP: Android 4.4 may send this request while phone type switching"
                        "GSMPhone.java dispose() -> GsmCallTracker.java dispose()-> hangup()\n",__func__);
                 }
                at_send_command("ATH", NULL);
                RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
            } else {
                requestHangup(data, datalen, t);
            }
            break;

        case RIL_REQUEST_HANGUP_WAITING_OR_BACKGROUND:
            if (ql_is_cdma()) {
                at_send_command("ATH", NULL);
            } else if (!access(ql_voice_over_usb_path, X_OK)) { //by now, 9x07 bug, will continue output PCM on /dev/ttyUSB1
                at_send_command("ATH", NULL);
            } else {
                at_send_command("AT+CHLD=0", NULL);
            }

            RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
            break;

        case RIL_REQUEST_HANGUP_FOREGROUND_RESUME_BACKGROUND:
            if (ql_is_cdma()) {
                at_send_command("ATH", NULL);
            } else if (!access(ql_voice_over_usb_path, X_OK)) { //by now, 9x07 bug, will continue output PCM on /dev/ttyUSB1
                at_send_command("ATH", NULL);
            } else {
                at_send_command("AT+CHLD=1", NULL);
            }

            RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
            break;

        case RIL_REQUEST_SWITCH_WAITING_OR_HOLDING_AND_ACTIVE:
            at_send_command("AT+CHLD=2", NULL);

            RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
            break;

        case RIL_REQUEST_ANSWER:
            at_send_command("ATA", NULL);

            RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
            break;

        case RIL_REQUEST_CONFERENCE:
            at_send_command("AT+CHLD=3", NULL);

            RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
            break;

        case RIL_REQUEST_UDUB:
            at_send_command("ATH", NULL);

            RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
            break;

        case RIL_REQUEST_SEPARATE_CONNECTION:
            {
                char  cmd[12];
                int   party = ((int*)data)[0];

                if (party > 0 && party < 10) {
                    sprintf(cmd, "AT+CHLD=2%d", party);
                    at_send_command(cmd, NULL);
                    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
                } else {
                    LOGD("%s error for RIL_REQUEST_SEPARATE_CONNECTION!\n", __func__);
                    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
                }
            }
            break;
        //--------------------------------------------------------

        /**** sms ****/
        //--------------------------------------------------------
        case RIL_REQUEST_SEND_SMS:
            requestSendSMS(data, datalen, t);
            break;

        case RIL_REQUEST_SEND_SMS_EXPECT_MORE:
            requestSendSMSExpectMore(data, datalen, t);
            break;

        case RIL_REQUEST_SMS_ACKNOWLEDGE:
            requestSMSAcknowledge(data, datalen, t);
            break;

        case RIL_REQUEST_WRITE_SMS_TO_SIM:
            requestWriteSmsToSim(data, datalen, t);
            break;

        case RIL_REQUEST_DELETE_SMS_ON_SIM: {
            char * cmd;
            p_response = NULL;
            asprintf(&cmd, "AT+CMGD=%d", ((int *)data)[0] - 1);
            err = at_send_command(cmd, &p_response);
            free(cmd);
            if (err < 0 || p_response->success == 0) {
                LOGE("%s error for RIL_REQUEST_DELETE_SMS_ON_SIM!\n", __func__);
                RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
            } else {
                RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
            }
            at_response_free(p_response);
            break;
        }

        case RIL_REQUEST_CDMA_SEND_SMS:
             requestSendCdmaSMS(data, datalen, t);
            break;

        case RIL_REQUEST_CDMA_SMS_ACKNOWLEDGE:
            RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
        break;

        case RIL_REQUEST_CDMA_SUBSCRIPTION: {
            /**
            * "response" is const char **
            * ((const char **)response)[0] is MDN if CDMA subscription is available
            * ((const char **)response)[1] is a comma separated list of H_SID (Home SID) if
            *                              CDMA subscription is available, in decimal format
            * ((const char **)response)[2] is a comma separated list of H_NID (Home NID) if
            *                              CDMA subscription is available, in decimal format
            * ((const char **)response)[3] is MIN (10 digits, MIN2+MIN1) if CDMA subscription is available
            * ((const char **)response)[4] is PRL version if CDMA subscription is available
            **/
            char * cmd_subscription[] = {"17756067602", "14151", "3734", "0912121001"};
            RIL_onRequestComplete(t, RIL_E_SUCCESS, cmd_subscription, 4 * sizeof(char *));
            break;
        }

        //--------------------------------------------------------

        /**** data connection ****/
        //--------------------------------------------------------
        case RIL_REQUEST_DEACTIVATE_DATA_CALL: {
            requestDeactivateDefaultPDP(data, datalen, t);
            break;
        }

        case RIL_REQUEST_LAST_DATA_CALL_FAIL_CAUSE: {
            requestLastPDPFailCause(t);
            break;
        }

        case RIL_REQUEST_SETUP_DATA_CALL:
            requestSetupDataCall(data, datalen, t);
            break;

        case RIL_REQUEST_DATA_CALL_LIST:
            requestDataCallList(data, datalen, t);
            break;
        //--------------------------------------------------------

        /**** network ****/
        //--------------------------------------------------------
        case RIL_REQUEST_SET_PREFERRED_NETWORK_TYPE:
            if (currentState() <= RADIO_STATE_UNAVAILABLE) {
                RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
                break;
            }
            if (ql_is_UC20 || ql_is_GSM || ql_is_mdm9215 || ql_is_mdm9x07)
                requestSetPreferredNetworkType(data,datalen,t);
            else
                RIL_onRequestComplete(t, RIL_E_REQUEST_NOT_SUPPORTED, NULL, 0);
            break;

        case RIL_REQUEST_SIGNAL_STRENGTH:
            requestSignalStrength(data, datalen, t);
            break;

        case RIL_REQUEST_VOICE_REGISTRATION_STATE:
        case RIL_REQUEST_DATA_REGISTRATION_STATE:
            if (currentState() != RADIO_STATE_SIM_READY && currentState() != RADIO_STATE_RUIM_READY) {
                char * response[] = {"0"}; //Not registered
                RIL_onRequestComplete(t, RIL_E_SUCCESS, response, sizeof(response));
                break;
            }
            requestRegistrationState(request, data, datalen, t);

#ifdef QUECTEL_REPORT_SIGNAL_STRENGTH
            if (RIL_REQUEST_VOICE_REGISTRATION_STATE == request) {
                requestSignalStrength(NULL, 0, NULL);
            }
#endif
            break;

        case RIL_REQUEST_OPERATOR:
            if (currentState() != RADIO_STATE_SIM_READY && currentState() != RADIO_STATE_RUIM_READY) {
                char *response[] = {NULL, NULL, NULL};
                RIL_onRequestComplete(t, RIL_E_SUCCESS, response, sizeof(response));
                break;
            }
            requestOperator(data, datalen, t);
            break;

                /***
                 *	Drop.
                 *	wythe 2014-3-28
                 */
#if 0
        case RIL_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC:
            at_send_command("AT+COPS=0", NULL);
            break;
#endif

        case RIL_REQUEST_QUERY_NETWORK_SELECTION_MODE:
            if (currentState() != RADIO_STATE_SIM_READY && currentState() != RADIO_STATE_RUIM_READY) {
                int response[] = {0};
                RIL_onRequestComplete(t, RIL_E_SUCCESS, response, sizeof(response));
                break;
            }
            requestQueryNetworkSelectionMode(data, datalen, t);
            break;

        /**
         *  Add new request:RIL_REQUEST_GET_PREFERRED_NETWORK_TYPE
         *  Wythe 2013-9-27
         */
        case RIL_REQUEST_GET_PREFERRED_NETWORK_TYPE:
        if (ql_is_UC20 || ql_is_GSM || ql_is_mdm9215 || ql_is_mdm9x07)
            requestGetPreferredNetworkType(t);
        else
            	RIL_onRequestComplete(t, RIL_E_REQUEST_NOT_SUPPORTED, NULL, 0);
         break;

        /**
         *  Add new request:RIL_REQUEST_QUERY_AVAILABLE_NETWORKS
         *  Wythe 2014-3-28
         */
        case RIL_REQUEST_QUERY_AVAILABLE_NETWORKS:
            requestQueryAvailableNetworks(t);
            break;

        /**
         *  Add new request:RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL
         *  Wythe 2014-3-28
         */
        case RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL:
            requestSetNetworkSelectionManual(data, datalen, t);
            break;

        /**
         *  Add new request:RIL_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC
         *  Wythe 2014-3-28
         *
         */
        case RIL_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC:
            requestSetNetworkSelectionAutomatic(t);
            break;

        //--------------------------------------------------------

        /**** sim card ****/
        //--------------------------------------------------------
        case RIL_REQUEST_GET_SIM_STATUS: {
            RIL_CardStatus_v6 *p_card_status;
            char *p_buffer;
            int buffer_size;

            int result = getCardStatus(&p_card_status);
            g_card_state_change_flg = 1;
            if (result == RIL_E_SUCCESS) {
                p_buffer = (char *)p_card_status;
                buffer_size = sizeof(*p_card_status);
            } else {
                p_buffer = NULL;
                buffer_size = 0;
            }

            if (!s_ril_conf_init.support_CDMPhone && result == RIL_E_SUCCESS && p_buffer && buffer_size) {
                if (p_card_status->num_applications == 2) {
                    p_card_status->cdma_subscription_app_index = RIL_CARD_MAX_APPS;
                    p_card_status->num_applications = 1;
                }
            }

            if (QL_RIL_VERSION < 6 && result == RIL_E_SUCCESS && p_buffer && buffer_size) {
                RIL_CardStatus_v5 *p_card_status_v5 = (RIL_CardStatus_v5 *)malloc(sizeof(RIL_CardStatus_v5));

                p_card_status_v5->card_state = p_card_status->card_state;
                p_card_status_v5->universal_pin_state = p_card_status->universal_pin_state;
                p_card_status_v5->gsm_umts_subscription_app_index = p_card_status->gsm_umts_subscription_app_index;
                p_card_status_v5->cdma_subscription_app_index = RIL_CARD_MAX_APPS;
                p_card_status_v5->num_applications = 1;
                p_card_status_v5->applications[0] = p_card_status->applications[0];
                
                RIL_onRequestComplete(t, result, p_card_status_v5, sizeof(RIL_CardStatus_v5));
                free(p_card_status_v5);
            } else {
                RIL_onRequestComplete(t, result, p_buffer, buffer_size);
            }
            
#if 1 //byd, after restart rild, make SIMRecords.java boardcast recordsLoaded event, then make GsmDataConnectionTracker.java to setupDataOnReadyApns
            if (p_card_status->card_state == RIL_CARDSTATE_PRESENT) {
                char prop_value[PROPERTY_VALUE_MAX] = {'\0'};
                static int rild_restart = 1;
                if (rild_restart && property_get("gsm.sim.state", prop_value, NULL) && !strcmp(prop_value, "READY")) {
                    //rild restart
                    int response[3] = {SIM_INIT, 0, 0};
                    RIL_onUnsolicitedResponse (RIL_UNSOL_SIM_REFRESH, response, sizeof(response));
                }
                rild_restart = 0;
            }
#endif

#ifdef QUECTEL_REPORT_SIGNAL_STRENGTH
            requestSignalStrength(NULL, 0, NULL);
#endif

#if 1
        if (!ql_mux_enabled)  //if has sim card, it is better to disable auto-suspend function, because it may cause usb disconnect
            ql_set_autosuspend(p_card_status->card_state != RIL_CARDSTATE_PRESENT);
#endif
            freeCardStatus(p_card_status);
            break;
        }

         case RIL_REQUEST_GET_IMSI:
            p_response = NULL;
            err = at_send_command_numeric("AT+CIMI", &p_response);
            if (err < 0 || p_response == NULL || p_response->success == 0 || p_response->p_intermediates == NULL
			|| p_response->p_intermediates->line == NULL || p_response->p_intermediates->line[0] == '?') {
                //IMSI is used to select APN, must get it. see SIMRecords.java, RuimRecords.java
                int  get_cimi_timeout = 30;
                while (get_cimi_timeout--) {
                    at_response_free(p_response); p_response = NULL;
                    err = at_send_command_numeric("AT+CIMI", &p_response);
                    if (err < 0 || p_response == NULL || p_response->success == 0 || p_response->p_intermediates == NULL
				|| p_response->p_intermediates->line == NULL || p_response->p_intermediates->line[0] == '?')
                        sleep(1);
                    else
                        break;
                };
            }

            if (err < 0 || p_response->success == 0) {
                LOGE("%s error for RIL_REQUEST_GET_IMSI!\n", __func__);
                RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
            } else {
                // it is better to according AT+CPIN? 's result, if it is a cdma RUIM card
                if (ql_is_device_and_sim_both_support_cdma) { //EC20 with China Telecom 3G RUIM card bug.
                    if (strStartsWith(p_response->p_intermediates->line, "20404")) { //Vodafone
                        char value[PROPERTY_VALUE_MAX];
                        property_get("ro.cdma.home.operator.numeric", value, "46003");
                        memcpy(p_response->p_intermediates->line, value, strlen(value));
                    } else if (strStartsWith(p_response->p_intermediates->line, "46099")) { //abnormal imsi
                        memcpy(p_response->p_intermediates->line, "46003", strlen("46003"));
                    }
                }
#define OPERNUM_LEN 5
                char tmpstr[OPERNUM_LEN + 1] = {0};
                memcpy(tmpstr, p_response->p_intermediates->line, OPERNUM_LEN);
                quectel_cimi = atoi(tmpstr);
                LOGD("[%s:%d] quectel_cimi = %d.\n", __func__, __LINE__, quectel_cimi);
                RIL_onRequestComplete(t, RIL_E_SUCCESS,
                    p_response->p_intermediates->line, sizeof(char *));
            }
            at_response_free(p_response);
            break;

         case RIL_REQUEST_SIM_IO:
            requestSIM_IO(data,datalen,t);
            break;

        case RIL_REQUEST_QUERY_FACILITY_LOCK:
            requestQuestFacility(data,datalen,t);
            break;

        case RIL_REQUEST_SET_FACILITY_LOCK:
            requestSetFacility(data,datalen,t,request);
            break;

        case RIL_REQUEST_CHANGE_SIM_PIN:
            requestChangeSimPin(data,datalen,"SC",t,request);
            break;

        case RIL_REQUEST_CHANGE_SIM_PIN2:
            requestChangeSimPin(data,datalen,"P2",t,request);
            break;

        case RIL_REQUEST_ENTER_SIM_PIN:
        case RIL_REQUEST_ENTER_SIM_PUK:
        case RIL_REQUEST_ENTER_SIM_PIN2:
        case RIL_REQUEST_ENTER_SIM_PUK2:
            requestEnterSimPin(data, datalen, t,request);
            break;
        //--------------------------------------------------------

        /**** other,e.g. system, baseband information****/
        case RIL_REQUEST_SCREEN_STATE: {
            if (currentState() != RADIO_STATE_SIM_READY && currentState() != RADIO_STATE_RUIM_READY) {
                RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
                break;
            }
            requestScreenState(data,datalen,t);
            break;
        }

        case RIL_REQUEST_BASEBAND_VERSION: {
            requestBaseBandVersion(data,datalen,t);
            break;
        }

        case RIL_REQUEST_GET_IMEISV: {
            requestGetIMEISV(t);
            break;
        }

        case RIL_REQUEST_REPORT_STK_SERVICE_IS_RUNNING: {
            requestReportSTKServiceIsRunning(t);
            break;
        }

        case RIL_REQUEST_RADIO_POWER:
#if 1 //quectel  //frameworks\base\telephony\java\com\android\internal\telephony/RIL.java ->processUnsolicited()
//case RIL_UNSOL_RIL_CONNECTED : setRadioPower(false, null);
//it it no need to power off radio when RIL.java connect
        if ((onRequestCount < 4) && (((int *)data)[0] == 0)) {
            RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
            break;
        }
#endif
            requestRadioPower(data, datalen, t);
            break;
        case RIL_REQUEST_DTMF: {
            char c = ((char *)data)[0];
            char *cmd;
            asprintf(&cmd, "AT+VTS=%c", (int)c);
            at_send_command(cmd, NULL);
            free(cmd);
            RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
            break;
        }

        case RIL_REQUEST_CDMA_BURST_DTMF: {
            char *c = ((char **)data)[0];
            char *cmd;
            asprintf(&cmd, "AT+VTS=%c", (int)(c[0]));
            at_send_command(cmd, NULL);
            free(cmd);
            RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
            break;
        }

        case RIL_REQUEST_GET_IMEI:
            p_response = NULL;
            err = at_send_command_numeric("AT+CGSN", &p_response);

            if (err < 0 || p_response->success == 0) {
                LOGE("%s error for RIL_REQUEST_GET_IMEI!\n", __func__);
                RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
            } else {
                RIL_onRequestComplete(t, RIL_E_SUCCESS,
                    p_response->p_intermediates->line, sizeof(char *));
            }
            at_response_free(p_response);
            break;

    	case RIL_REQUEST_DEVICE_IDENTITY:
            requestDeviceIdentity(t);
            break;

        case RIL_REQUEST_SEND_USSD:
            requestSendUSSD(data, datalen, t);
            break;

        case RIL_REQUEST_CANCEL_USSD:
#if 1 //quectel
            ussd_pending_index++;
#endif
            at_send_command("AT+CUSD=2", NULL);
            RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
            break;

        case RIL_REQUEST_OEM_HOOK_RAW:
            // echo back data
            RIL_onRequestComplete(t, RIL_E_SUCCESS, data, datalen);
            break;

        case RIL_REQUEST_SET_BAND_MODE:
            if (ql_is_UC20) {
                ATResponse *atResponse = NULL;
                int err;
                int bandval = 0;
                char *cmd;

                switch(((int *)data)[0])
                {
                    case 1:
                        bandval = 3;
                        break;
                    case 2:
                        bandval = 108;
                        break;
                    case 3:
                        bandval = 256;
                        break;
                    case 4:
                    case 5:
                        bandval = 67;
                        break;
                    default:
                        bandval = 512;
                }
                asprintf(&cmd,"AT+QCFG=\"band\",%d",bandval);
                err = at_send_command(cmd,&atResponse);

                if(err < 0 || atResponse == NULL || atResponse->success == 0)
                    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
                else
                    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
                free(atResponse);
                free(cmd);
            } else {
                RIL_onRequestComplete(t, RIL_E_REQUEST_NOT_SUPPORTED, NULL, 0);
            }
            break;

        case RIL_REQUEST_QUERY_AVAILABLE_BAND_MODE:
            RIL_onRequestComplete(t, RIL_E_REQUEST_NOT_SUPPORTED, NULL, 0);
            break;

        case RIL_REQUEST_GET_SMSC_ADDRESS:
            {
                ATResponse *p_response = NULL;
                char *cmd = NULL;
                char *SMSC_address = NULL;
                int err;

                err = at_send_command_singleline("AT+CSCA?","+CSCA:",&p_response);
                if(err < 0 || p_response == NULL || p_response->success == 0)
                    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
                else
                {
                    char *line;
#ifdef CUSD_USE_UCS2_MODE
                    unsigned char char_addr[128] = {0};
                    unsigned char ucs2[64];
#endif

                    line = p_response->p_intermediates->line;
                    err = at_tok_start(&line);
                    err = at_tok_nextstr(&line,&SMSC_address);
#ifdef CUSD_USE_UCS2_MODE
                    ucs2_to_utf8(ucs2, hexStringToBytes(SMSC_address, ucs2) / 2, char_addr);
                    SMSC_address = (char *)char_addr;
#endif

                    RIL_onRequestComplete(t, RIL_E_SUCCESS, SMSC_address, sizeof(char *));
                }
                free(p_response);
                free(cmd);
            }
            break;

        case RIL_REQUEST_SET_SMSC_ADDRESS:
            {
                ATResponse *p_response = NULL;
                char *cmd = NULL;
                char *SMSC_address = NULL;
                int err;
#ifdef CUSD_USE_UCS2_MODE
                char ucs2_addr[128] = {0};
                const char bytesToHexString[] = "0123456789abcdef";
                size_t i;
                for (i = 0; i < strlen((char *)data); i++) {
                    ucs2_addr[i*4 + 0] =  '0';
                    ucs2_addr[i*4 + 1] =  '0';
                    ucs2_addr[i*4 + 2] =  bytesToHexString[((char *)data)[i] >> 4];
                    ucs2_addr[i*4 + 3] =  bytesToHexString[((char *)data)[i] & 0xf];
                }
                data = ucs2_addr;
#endif

                SMSC_address = data;
                asprintf(&cmd,"AT+CSCA=\"%s\"",SMSC_address);

                err = at_send_command(cmd,&p_response);
                if(err < 0 || p_response == NULL || p_response->success == 0)
                    RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
                else
                    RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);

                free(p_response);
                free(cmd);
            }
            break;


        case RIL_REQUEST_OEM_HOOK_STRINGS: {
            int i;
            const char ** cur;

            LOGD("got OEM_HOOK_STRINGS: 0x%8p %lu", data, (long)datalen);


            for (i = (datalen / sizeof (char *)), cur = (const char **)data ;
                    i > 0 ; cur++, i --) {
                LOGD("> '%s'", *cur);
            }

#if 1 //quectel
{
        int err;
        ATResponse *p_response = NULL;
        err = at_send_command_multiline( ((const char **)data)[0], "\0", &p_response);
        if(err < 0 || p_response == NULL || p_response->finalResponse == NULL)
            RIL_onRequestComplete(t, RIL_E_GENERIC_FAILURE, NULL, 0);
        else
        {
            char *lines[100] ;
            size_t i = 0;
            ATLine  *p_intermediates = p_response->p_intermediates;
            while (p_intermediates && (i < ((sizeof(lines) / sizeof(lines[0])) - 1))) {
                if (p_intermediates->line)
                    lines[i++] = p_intermediates->line;
                p_intermediates = p_intermediates->p_next;
            }
            if (p_response->finalResponse)
                lines[i++] = p_response->finalResponse;
            RIL_onRequestComplete(t, RIL_E_SUCCESS, lines, i*sizeof(char *));
        }
}
#else
            // echo back strings
            RIL_onRequestComplete(t, RIL_E_SUCCESS, data, datalen);
#endif
            break;
        }

        case RIL_REQUEST_GSM_SET_BROADCAST_SMS_CONFIG: {
            requestGsmSetBroadcastSmsConfig(data, datalen, t);
            break;
        }

        case RIL_REQUEST_GSM_GET_BROADCAST_SMS_CONFIG: {
            requestGsmGetBroadcastSmsConfig(t);
            break;
        }

        case RIL_REQUEST_GSM_SMS_BROADCAST_ACTIVATION: {
            requestGsmSmsBroadcastActivation(data, datalen, t);
            break;
        }

        case RIL_REQUEST_RESET_RADIO: {
        if (ql_is_UC20)
            requestResetRadio(t);
        else
            	RIL_onRequestComplete(t, RIL_E_REQUEST_NOT_SUPPORTED, NULL, 0);
            break;
        }

        case RIL_REQUEST_GET_MUTE: {
            requestGetMute(t);
            break;
        }

        case RIL_REQUEST_SET_MUTE: {
            requestSetMute(data, datalen, t);
            break;
        }

        case RIL_REQUEST_DTMF_START: {
            requestDtmfStart(data, datalen, t);
            break;
        }

        case RIL_REQUEST_DTMF_STOP: {
            requestDtmfStop(t);
            break;
        }

        case RIL_REQUEST_QUERY_CALL_FORWARD_STATUS: {
            requestQueryCallForwardStatus(data, datalen, t);
            break;
        }

        case RIL_REQUEST_SET_CALL_FORWARD: {
            requestSetCallForward(data, datalen, t);
            break;
        }

        case RIL_REQUEST_GET_CLIR: {
            requestGetClir(t);
            break;
        }

        case RIL_REQUEST_SET_CLIR: {
            requestSetClir(data, datalen, t);
            break;
        }

        case RIL_REQUEST_QUERY_CALL_WAITING: {
            requestQueryCallWaiting(data, datalen, t);
            break;
        }

        case RIL_REQUEST_SET_CALL_WAITING: {
            requestSetCallWaiting(data, datalen, t);
            break;
        }
#if 0
        case RIL_REQUEST_QUERY_AVAILABLE_NETWORKS:
            requestQueryAvailableNetworks(t);
            break;

        case RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL:
            requestSetNetworkSelectionManual(data, datalen, t);
            break;

        case RIL_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC:
            requestSetNetworkSelectionAutomatic(t);
            break;
#endif
        case RIL_REQUEST_GET_NEIGHBORING_CELL_IDS:
        if (ql_is_UC20)
            requestGetNeighboringCellIDs(t);
        else
              RIL_onRequestComplete(t, RIL_E_REQUEST_NOT_SUPPORTED, NULL, 0);
            break;

#ifdef RIL_REQUEST_GET_HARDWARE_CONFIG
        case RIL_REQUEST_GET_HARDWARE_CONFIG:
            requestGetHardwareConfig(data, datalen, t);
            break;
#endif

#ifdef RIL_REQUEST_GET_HARDWARE_CONFIG
        case RIL_REQUEST_SHUTDOWN:
            requestShutdown(t);
            break;
 #endif

#ifdef RIL_REQUEST_SET_UNSOL_CELL_INFO_LIST_RATE
        case RIL_REQUEST_SET_UNSOL_CELL_INFO_LIST_RATE:
            requestSetCellInfoListRate(data, datalen, t);
            break;
 #endif

        case RIL_REQUEST_SET_TTY_MODE:
            RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
            break;

        case RIL_REQUEST_QUERY_TTY_MODE:    {
            int response[] = {0}; // * ((int *)response)[0] is == 0 for TTY off
            RIL_onRequestComplete(t, RIL_E_SUCCESS, response, sizeof(response));
            break;
        }

#ifdef RIL_REQUEST_IMS_REGISTRATION_STATE
        case RIL_REQUEST_IMS_REGISTRATION_STATE: {
            int response[] = {0, 0}; //  0 - Not registered
            RIL_onRequestComplete(t, RIL_E_SUCCESS, response, sizeof(response));
            break;
        }
 #endif

#ifdef RIL_REQUEST_SIM_OPEN_CHANNEL
        case RIL_REQUEST_SIM_OPEN_CHANNEL:
            RIL_onRequestComplete(t, RIL_E_REQUEST_NOT_SUPPORTED, NULL, 0);
            break;
#endif

#ifdef RIL_REQUEST_SIM_CLOSE_CHANNEL
        case RIL_REQUEST_SIM_CLOSE_CHANNEL:
            RIL_onRequestComplete(t, RIL_E_REQUEST_NOT_SUPPORTED, NULL, 0);
            break;
#endif

#ifdef RIL_REQUEST_SET_INITIAL_ATTACH_APN
        case RIL_REQUEST_SET_INITIAL_ATTACH_APN:
            RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
            break;
#endif

#ifdef RIL_REQUEST_GET_CELL_INFO_LIST
        case RIL_REQUEST_GET_CELL_INFO_LIST:
            if (ql_is_mdm9215 || ql_is_mdm9x07)
                requestGetCellInfoList(data, datalen, t);
            else
                RIL_onRequestComplete(t, RIL_E_REQUEST_NOT_SUPPORTED, NULL, 0);
            break;
#endif

        case RIL_REQUEST_CDMA_SET_SUBSCRIPTION_SOURCE:
            RIL_onRequestComplete(t, RIL_E_SUCCESS, NULL, 0);
            break;

#ifdef RIL_REQUEST_GET_RADIO_CAPABILITY
        case RIL_REQUEST_GET_RADIO_CAPABILITY: {
                RIL_RadioCapability RadioCapability = {
                    .version = RIL_RADIO_CAPABILITY_VERSION,            // Version of structure, RIL_RADIO_CAPABILITY_VERSION
                    .session = 0,            // Unique session value defined by framework returned in all "responses/unsol"
                    .phase = RC_PHASE_CONFIGURED,              // CONFIGURED, START, APPLY, FINISH
                    .rat = 0,             // RIL_RadioAccessFamily for the radio
                    .logicalModemUuid = "com.quectel.lm0", // A UUID typically "com.xxxx.lmX where X is the logical modem.
                    .status =RC_STATUS_SUCCESS,             // Return status and an input parameter for RC_PHASE_FINISH
                };
                if (ql_is_GSM) {
                    RadioCapability.rat |= RAF_GSM | RAF_GPRS |RAF_EDGE;
                } else if (ql_is_UC20 || ql_is_UG95) {
                    RadioCapability.rat |= RAF_GSM | RAF_GPRS |RAF_EDGE;
                    RadioCapability.rat |= RAF_UMTS | RAF_HSDPA | RAF_HSUPA |RAF_HSPA | RAF_HSPAP;
                } else {
                    RadioCapability.rat |= RAF_GSM | RAF_GPRS |RAF_EDGE;
                    RadioCapability.rat |= RAF_UMTS | RAF_HSDPA | RAF_HSUPA |RAF_HSPA | RAF_HSPAP; //3G
                    RadioCapability.rat |= RAF_TD_SCDMA;
                    RadioCapability.rat |= RAF_LTE; //4G
                    RadioCapability.rat |= RAF_1xRTT | RAF_EVDO_A | RAF_EVDO_B | RAF_EHRPD; //cdma
                }
                RIL_onRequestComplete(t, RIL_E_SUCCESS, &RadioCapability, sizeof(RIL_RadioCapability));
            }
            break;
#endif

#ifdef RIL_REQUEST_VOICE_RADIO_TECH
        case RIL_REQUEST_VOICE_RADIO_TECH: {
                int voiceRadioTech = RADIO_TECH_GPRS;
                if (ql_is_GSM) {
                    voiceRadioTech = RADIO_TECH_GPRS;
                } else if (ql_is_UC20 || ql_is_UG95) {
                    voiceRadioTech = RADIO_TECH_GPRS;
                } else {
                    voiceRadioTech = RADIO_TECH_GPRS;
                    if (ql_is_device_and_sim_both_support_cdma && ql_cdmaphone)
                        voiceRadioTech = RADIO_TECH_1xRTT;
                }
                RIL_onRequestComplete(t, RIL_E_SUCCESS, &voiceRadioTech, sizeof(int));
            }
            break;
#endif

        default:
            RIL_onRequestComplete(t, RIL_E_REQUEST_NOT_SUPPORTED, NULL, 0);
            break;
    }
}

/**
 * Synchronous call from the RIL to us to return current radio state.
 * RADIO_STATE_UNAVAILABLE should be the initial state.
 */
static RIL_RadioState
currentState()
{
    if (QL_RIL_VERSION >= 12) { //Android7.1.1 ril.cpp->decodeVoiceRadioTechnology()
        if (RADIO_STATE_SIM_READY == sState && ql_is_device_and_sim_both_support_cdma && ql_cdmaphone)
            return RADIO_STATE_RUIM_READY;
    }
    return sState;
}
/**
 * Call from RIL to us to find out whether a specific request code
 * is supported by this implementation.
 *
 * Return 1 for "supported" and 0 for "unsupported"
 */

static int
onSupports (int requestCode)
{
    (void ) requestCode;
    LOGI("Call OnSupports");
    return 1;
}

static void onCancel (RIL_Token t)
{
    (void) t;
    LOGI("Call OnCancel");
}

static const char * getVersion(void)
{
#if 1 //quectel
    onRequestCount = 0; //onNewCommandConnect will call this function, and RIL.java will send RIL_REQUEST_RADIO_POWER
#endif
    if (QL_RIL_VERSION < 6 && currentState() == RADIO_STATE_SIM_READY) {
        RIL_onUnsolicitedResponse (RIL_UNSOL_RESPONSE_RADIO_STATE_CHANGED, NULL, 0);
    }

    return REFERENCE_RIL_VERSION;
}

static void
setRadioState(RIL_RadioState newState)
{
    RIL_RadioState oldState;

    pthread_mutex_lock(&s_state_mutex);

    oldState = sState;

    LOGI("[%s]:oldState=%d, newState=%d", __func__, oldState, newState);

    if (s_closed > 0) {
        // If we're closed, the only reasonable state is
        // RADIO_STATE_UNAVAILABLE
        // This is here because things on the main thread
        // may attempt to change the radio state after the closed
        // event happened in another thread
        newState = RADIO_STATE_UNAVAILABLE;
    }

    if (sState != newState || s_closed > 0) {
        sState = newState;

        pthread_cond_broadcast (&s_state_cond);
    }

    pthread_mutex_unlock(&s_state_mutex);

    /* do these outside of the mutex */
    if (sState != oldState) {
        RIL_onUnsolicitedResponse (RIL_UNSOL_RESPONSE_RADIO_STATE_CHANGED,
                                    NULL, 0);

        /* FIXME onSimReady() and onRadioPowerOn() cannot be called
         * from the AT reader thread
         * Currently, this doesn't happen, but if that changes then these
         * will need to be dispatched on the request thread
         */
        if (sState == RADIO_STATE_SIM_READY) {
            onSIMReady();
        } else if (sState == RADIO_STATE_SIM_NOT_READY) {
            onRadioPowerOn();
        }
    }
}

/** Returns SIM_NOT_READY on error */
static SIM_Status
getSIMStatus()
{
    ATResponse *p_response = NULL;
    int err;
    int ret;
    char *cpinLine;
    char *cpinResult;

    if (sState == RADIO_STATE_OFF || sState == RADIO_STATE_UNAVAILABLE) {
        ret = SIM_NOT_READY;
        goto done;
    }

    err = at_send_command_singleline("AT+CPIN?", "+CPIN:", &p_response);

    if (err != 0) {
        ret = SIM_NOT_READY;
        goto done;
    }

    switch (at_get_cme_error(p_response)) {
        case CME_SUCCESS:
            break;

        case CME_SIM_NOT_INSERTED:
            ret = SIM_ABSENT;
            goto done;

        default:
            ret = SIM_NOT_READY;
            goto done;
    }

    /* CPIN? has succeeded, now look at the result */

    cpinLine = p_response->p_intermediates->line;
    err = at_tok_start (&cpinLine);

    if (err < 0) {
        ret = SIM_NOT_READY;
        goto done;
    }

    err = at_tok_nextstr(&cpinLine, &cpinResult);

    if (err < 0) {
        ret = SIM_NOT_READY;
        goto done;
    }

    if (0 == strcmp (cpinResult, "SIM PIN")) {
        ret = SIM_PIN;
        goto done;
    } else if (0 == strcmp (cpinResult, "SIM PUK")) {
        ret = SIM_PUK;
        goto done;
    } else if (0 == strcmp (cpinResult, "PH-NET PIN")) {
        return SIM_NETWORK_PERSONALIZATION;
    } else if (0 != strcmp (cpinResult, "READY"))  {
        /* we're treating unsupported lock types as "sim absent" */
        ret = SIM_ABSENT;
        goto done;
    }

    at_response_free(p_response);
    p_response = NULL;
    cpinResult = NULL;

    ret = SIM_READY;

done:
    at_response_free(p_response);
    return ret;
}


/**
 * Get the current card status.
 *
 * This must be freed using freeCardStatus.
 * @return: On success returns RIL_E_SUCCESS
 */
static int getCardStatus(RIL_CardStatus_v6 **pp_card_status) {
    static RIL_AppStatus app_status_array[] = {
        // SIM_ABSENT = 0
        { RIL_APPTYPE_UNKNOWN, RIL_APPSTATE_UNKNOWN, RIL_PERSOSUBSTATE_UNKNOWN,
          NULL, NULL, 0, RIL_PINSTATE_UNKNOWN, RIL_PINSTATE_UNKNOWN },
        // SIM_NOT_READY = 1
        { RIL_APPTYPE_SIM, RIL_APPSTATE_DETECTED, RIL_PERSOSUBSTATE_UNKNOWN,
          NULL, NULL, 0, RIL_PINSTATE_UNKNOWN, RIL_PINSTATE_UNKNOWN },
        // SIM_READY = 2
        { RIL_APPTYPE_SIM, RIL_APPSTATE_READY, RIL_PERSOSUBSTATE_READY,
          NULL, NULL, 0, RIL_PINSTATE_UNKNOWN, RIL_PINSTATE_UNKNOWN },
        // SIM_PIN = 3
        { RIL_APPTYPE_SIM, RIL_APPSTATE_PIN, RIL_PERSOSUBSTATE_UNKNOWN,
          NULL, NULL, 0, RIL_PINSTATE_ENABLED_NOT_VERIFIED, RIL_PINSTATE_UNKNOWN },
        // SIM_PUK = 4
        { RIL_APPTYPE_SIM, RIL_APPSTATE_PUK, RIL_PERSOSUBSTATE_UNKNOWN,
          NULL, NULL, 0, RIL_PINSTATE_ENABLED_BLOCKED, RIL_PINSTATE_UNKNOWN },
        // SIM_NETWORK_PERSONALIZATION = 5
        { RIL_APPTYPE_SIM, RIL_APPSTATE_SUBSCRIPTION_PERSO, RIL_PERSOSUBSTATE_SIM_NETWORK,
          NULL, NULL, 0, RIL_PINSTATE_ENABLED_NOT_VERIFIED, RIL_PINSTATE_UNKNOWN }
    };

    RIL_CardState card_state;
    int num_apps;

    int sim_status = getSIMStatus();
    if (sim_status == SIM_ABSENT) {
        card_state = RIL_CARDSTATE_ABSENT;
        num_apps = 0;
    } else {
        card_state = RIL_CARDSTATE_PRESENT;
        num_apps = 1;
    }

    // Allocate and initialize base card status.
    RIL_CardStatus_v6 *p_card_status = malloc(sizeof(RIL_CardStatus_v6));
    p_card_status->card_state = card_state;
    p_card_status->universal_pin_state = RIL_PINSTATE_UNKNOWN;
    p_card_status->gsm_umts_subscription_app_index = RIL_CARD_MAX_APPS;
    p_card_status->cdma_subscription_app_index = RIL_CARD_MAX_APPS;
    p_card_status->ims_subscription_app_index = RIL_CARD_MAX_APPS;
    p_card_status->num_applications = num_apps;

    // Initialize application status
    int i;
    for (i = 0; i < RIL_CARD_MAX_APPS; i++) {
        p_card_status->applications[i] = app_status_array[SIM_ABSENT];
    }

    // Pickup the appropriate application status
    // that reflects sim_status for gsm.
    if (num_apps != 0) {
        // Only support one app, gsm
        p_card_status->num_applications = 1;
        p_card_status->gsm_umts_subscription_app_index = 0;

        // Get the correct app status
        p_card_status->applications[0] = app_status_array[sim_status];

#if 1
        //CE only allow to sale in china, and  only China Telecom support CDMA
        ql_is_device_and_sim_both_support_cdma = 0;
        if ((sim_status == SIM_READY || sim_status == SIM_PUK) && ql_is_device_support_cdma) {
            ATResponse *p_response = NULL;
            int err;
            int  get_cimi_timeout = 0;

            if(sim_status == SIM_READY) {
                get_cimi_timeout = 30;
            }

            while (get_cimi_timeout--) {
                    at_response_free(p_response); p_response = NULL;
                    err = at_send_command_numeric("AT+CIMI", &p_response);
                    if (err < 0 || p_response == NULL || p_response->success == 0 || p_response->p_intermediates == NULL
			|| p_response->p_intermediates->line == NULL || p_response->p_intermediates->line[0] == '?')
                        sleep(1);
                    else
                        break;
            };

            if (err == 0 && p_response != NULL && p_response->success == 1 && p_response->p_intermediates->line) {
                ql_is_device_and_sim_both_support_cdma =
                    strStartsWith(p_response->p_intermediates->line, "46003") ||
                    strStartsWith(p_response->p_intermediates->line, "46011") ||
                    strStartsWith(p_response->p_intermediates->line, "46099") ||
                    strStartsWith(p_response->p_intermediates->line, "20404");
            }

            at_response_free(p_response);
        }

        if (ql_is_device_and_sim_both_support_cdma) {
            //EC20 with China Telecom 3G RUIM card bug support cdma
           static RIL_AppStatus cdma_sim_ready_status = { RIL_APPTYPE_RUIM, RIL_APPSTATE_READY, RIL_PERSOSUBSTATE_READY,
               NULL, NULL, 0, RIL_PINSTATE_UNKNOWN, RIL_PINSTATE_UNKNOWN };
           static RIL_AppStatus cdma_sim_ready_status_puk = { RIL_APPTYPE_RUIM, RIL_APPSTATE_PUK, RIL_PERSOSUBSTATE_UNKNOWN,
               NULL, NULL, 0, RIL_PINSTATE_UNKNOWN, RIL_PINSTATE_UNKNOWN };

           p_card_status->num_applications = 2;
           p_card_status->cdma_subscription_app_index= 1;
           if(sim_status == SIM_READY)
               p_card_status->applications[1] = cdma_sim_ready_status;
           else
               p_card_status->applications[1] = cdma_sim_ready_status_puk;
        }

        if (sim_status == SIM_READY && ql_is_mdm9x07 && ql_is_device_and_sim_both_support_cdma) {
            if (!s_ril_conf_init.support_CDMPhone)
                at_send_command("AT+QCFG=\"cdmasms/cmtformat\",1", NULL); // 0 ~ cdma pdu, 1 ~ gsm pdu
            else if (QL_RIL_VERSION <= 7) //Android4.2, donot support Phone Switch, and GSMPhone donot support cdma pdu
                at_send_command("AT+QCFG=\"cdmasms/cmtformat\",1", NULL); // 0 ~ cdma pdu, 1 ~ gsm pdu
            else
                at_send_command("AT+QCFG=\"cdmasms/cmtformat\",0", NULL); // 0 ~ cdma pdu, 1 ~ gsm pdu
        }
#endif
    }

    *pp_card_status = p_card_status;
    return RIL_E_SUCCESS;
}

/**
 * Free the card status returned by getCardStatus
 */
static void freeCardStatus(RIL_CardStatus_v6 *p_card_status) {
    free(p_card_status);
}

static void onSIMStateChange (void *param)
{
    ATResponse *p_response;

    switch(getSIMStatus()) {
        case SIM_ABSENT:
        case SIM_PIN:
        case SIM_PUK:
        case SIM_NETWORK_PERSONALIZATION:
        default:
            setRadioState(RADIO_STATE_SIM_LOCKED_OR_ABSENT);
        return;

        case SIM_NOT_READY:
            setRadioState(RADIO_STATE_SIM_NOT_READY);
        return;

        case SIM_READY:
            setRadioState(RADIO_STATE_SIM_READY);
        return;
    }
}

static void onSMSReady (void *param) {
    ATResponse *p_response = NULL;
    int err;
    int sms_ready = 1;

//AT+QINISTAT is used to query status of SIM/USIM card initialization.
    err = at_send_command_singleline("AT+QINISTAT", "+QINISTAT:", &p_response);
    if (err < 0 || p_response == NULL || p_response->success == 0) {

    } else {
        int status;
        char *line = p_response->p_intermediates->line;
        if (at_tok_start(&line) == 0) {
            if (at_tok_nextint(&line, &status) == 0) {
                if (ql_is_GSM) {
                    // 0 No initiallization
                    // 1 Ready to execute AT command
                    // 2 Phonebook has finished initialization
                    // 3 SMS has finished initialization
                     sms_ready = ((status & 3) == 3);
                } else if (ql_is_UC20) {
                    // 0 Initial state
                    // 1 CPIN READY. Operation like lock/unlock PIN is allowed
                    // 2 SMS initialization complete
                    // 4 Phonebook initialization complete
                    sms_ready = ((status & 2) == 2);
                } else {
                    sms_ready = (status != 0); // i donot konw by now!
                }
            }
        }
    }
    at_response_free(p_response);

    if (sms_ready) {
        if (ql_is_GSM) {
            /**
            *  Wythe: Modify on 2013-04-02 for 4.0 ril
            *  we support AT+CSMS=128 for GSM module and
            *  AT+CSMS=1 for WCDMA module
            */
            at_send_command_singleline("AT+CSMS=128", "+CSMS:", NULL);

            /*
            * Always send SMS messages directly to the TE
            *
            * mode = 1 // discard when link is reserved (link should never be
            *             reserved)
            * mt = 2   // most messages routed to TE
            * bm = 2   // new cell BM's routed to TE
            * ds = 1   // Status reports routed to TE
            * bfr = 1  // flush buffer
            */
            at_send_command("AT+CNMI=1,2,2,1,1", NULL);
        } else {
            at_send_command_singleline("AT+CSMS=1", "+CSMS:", NULL);
            /*
            * Always send SMS messages directly to the TE
            *
            * mode = 1 // discard when link is reserved (link should never be
            *             reserved)
            * mt = 2   // most messages routed to TE
            * bm = 2   // new cell BM's routed to TE
            * ds = 1   // Status reports routed to TE
            * bfr = 1  // flush buffer
            */
            at_send_command("AT+CNMI=1,2,0,1,0", NULL);
        }

        /*  SMS PDU mode */
        at_send_command("AT+CMGF=0", NULL);
    }else {
        RIL_requestTimedCallback (onSMSReady, NULL, &TIMEVAL_SIMPOLL);
    }
}

/**
 * SIM ready means any commands that access the SIM will work, including:
 *  AT+CPIN, AT+CSMS, AT+CNMI, AT+CRSM
 *  (all SMS-related commands)
 */

static void pollSIMState (void *param)
{
    ATResponse *p_response;
    int ret;

    if (sState != RADIO_STATE_SIM_NOT_READY) {
        // no longer valid to poll
        return;
    }

    switch(getSIMStatus()) {
        case SIM_ABSENT:
        case SIM_PIN:
        case SIM_PUK:
        case SIM_NETWORK_PERSONALIZATION:
        default:
            setRadioState(RADIO_STATE_SIM_LOCKED_OR_ABSENT);
        return;

        case SIM_NOT_READY:
            RIL_requestTimedCallback (pollSIMState, NULL, &TIMEVAL_SIMPOLL);
        return;

        case SIM_READY:
            setRadioState(RADIO_STATE_SIM_READY);
        return;
    }
}

/** returns 1 if on, 0 if off, and -1 on error */
static int isRadioOn()
{
    ATResponse *p_response = NULL;
    int err;
    char *line;
    char ret;

    err = at_send_command_singleline("AT+CFUN?", "+CFUN:", &p_response);

    if (err < 0 || p_response->success == 0) {
        // assume radio is off
        goto error;
    }

    line = p_response->p_intermediates->line;

    err = at_tok_start(&line);
    if (err < 0) goto error;

    err = at_tok_nextbool(&line, &ret);
    if (err < 0) goto error;

    at_response_free(p_response);

    return (int)ret;

error:
    LOGE("[%s]:isRadioOn return error\n", __func__);
    at_response_free(p_response);
    return -1;
}

#ifdef QUECTEL_REPORT_SIGNAL_STRENGTH
static void onReportSignalStrength(void *param)
{
    requestSignalStrength(NULL, 0, NULL);
    RIL_requestTimedCallback(onReportSignalStrength, NULL, &TIMEVAL_20);
}
#endif

static void QuectelCheckAndroidFrameworkSettingCallback(void *param) {
    char prop_value[PROPERTY_VALUE_MAX] = {'\0'};
    FILE *fp;
    char *cmd;
    
    LOGD("%s", __func__);
    //do not recevie RIL request form RIL.java
    if (!onRequestCount) {
        if (access("/data/data/com.android.phone", R_OK)) {
            LOGE("Quectel Check: do you install APK com.android.phone ?");
        }

        prop_value[0] = '\0';
        if (property_get("ro.radio.noril", prop_value, NULL) > 0) {
            LOGD("[ro.radio.noril]: [%s]", prop_value);
            if (!strcmp(prop_value, "yes")) {
                LOGE("Quectel Check:  ro.radio.ril must be no, check files /*.prop or /system/*/prop");
            }
        }
    }

    //check if enable mobile data switch in APK Setting
    if (settings_get("mobile_data", prop_value)) {
        if (prop_value[0] == '0') {
            //system("settings put global mobile_data 1");
        }
    }

    //check if allow setup data call when EC20 is on roaming state
    if (settings_get("data_roaming", prop_value)) {
        if (prop_value[0] == '0') {
            //system("settings put global data_roaming 1");
        }
    }

    //How to check next config ?
    //com.android.internal.R.bool.config_voice_capable allow Android to send/recv voice call
    //com.android.internal.R.bool.config_sms_capable allow Android to send/recv sms
    //com.android.internal.R.array.networkAttributes allow Android to setup mobile data call
    //maybe check config_sms_capable by keyword mSmsCapable from SMSDispatcher.java in radio log
    //maybe check config_voice_capable by keyword mIsVoiceCapable from PhoneBase.java in radio log

    //check APN Settings
    //system("sqlite3 /data/data/com.android.providers.telephony/databases/telephony.db \"select * from carriers\"");
}

static void initializeLaterCallback(void *param)
{
    ATResponse *p_response = NULL;
    int err = 0;
    int need_retry = 0;
    static int AT_QSCLK = 0;
    static int AT_QTONEDET = 0;
    static int max_init_times = 30;

    if (!ql_is_mdm9215) //other modules donot support this feature by now!
        AT_QTONEDET = 1;

    if (!AT_QTONEDET) {
        at_send_command("AT+QTONEDET=1", &p_response); //This command is used to enable or disable DTMF detection.
        if (err == 0 && p_response != NULL && p_response->success == 1) {
            AT_QTONEDET = 1;
            ql_dtmf_init();
        } else {
            need_retry++;
        }
        at_response_free(p_response);
        p_response = NULL;
    }

    if (!AT_QSCLK) {
        err = at_send_command("AT+QSCLK=1", &p_response); //Configure Whether or not to Enter into Sleep Mode
        if (err == 0 && p_response != NULL && p_response->success == 1) {
            AT_QSCLK = 1;
        } else {
            need_retry++;
        }
        at_response_free(p_response);
        p_response = NULL;
    }

    if (need_retry && max_init_times--) {
        RIL_requestTimedCallback(initializeLaterCallback, NULL, &TIMEVAL_1);
    } else {
        AT_QSCLK = AT_QTONEDET;
        max_init_times = 30;
    }
}

static void initializeCustomerCallback(void *param)
{
    ATResponse *p_response = NULL;
    int err;
    static int finish = 0;
    static int max_retry_times = 60;
    static int cmd_cnt = 0;

    //if you want to do something, send your at commands here
/*  const char *at_cmds[] = {"AT+QAUDMOD=2", "AT+QAUDMOD=?", "AT+Qsidet=0", "AT+clvl=4", "AT+Qrxgain=10000",
    "AT+Qmic=2560,2560", "AT+Qeec=0,2048", "AT+Qeec=5,14", "AT+Qeec=10,160", "AT+Qeec=21,28000",
    "AT+Qeec=22,14000", "AT+Qeec=24,768", "AT+Qeec=33,1024", "AT+Qeec?"};
*/
    if(0 == cmd_cnt) {
        while(0 != s_ril_conf_init.At_Cmds_For_Customer[cmd_cnt][0]) {
            cmd_cnt++;
        }

        if (!access(ql_voice_over_usb_path, X_OK)) {
            const char *at_cmds[] = {"AT+QGPSCFG=\"outport\",\"none\"","AT+QGPSCFG=\"outport\"","AT+QPCMV=1,0","AT+QPCMV?"};
            int i = 0;
            for(i = 0; i < sizeof(at_cmds) / sizeof(at_cmds[0]); i++) {
                strcpy(s_ril_conf_init.At_Cmds_For_Customer[cmd_cnt++], at_cmds[i]);
            }
        }
    }

    for (finish = finish; finish < cmd_cnt; finish++) {
        err = at_send_command_multiline(s_ril_conf_init.At_Cmds_For_Customer[finish], "\0", &p_response);
        if (err < 0 || p_response == NULL || p_response->success == 0) {
            at_response_free(p_response);
            break;
        }
        at_response_free(p_response);
    }

    if (finish < cmd_cnt && max_retry_times--) {
        RIL_requestTimedCallback(initializeCustomerCallback, NULL, &TIMEVAL_1);
    } else {
        finish = 0;
        max_retry_times = 60;
    }
}

/**
 * Initialize everything that can be configured while we're still in
 * AT+CFUN=0
 */
static void initializeCallback(void *param)
{
    ATResponse *p_response = NULL;
    int err;

#ifdef USB_HOST_FORCE_OPEN_GPS_PORT
{
    char atdevice[10];
    if (ql_mux_enabled == 0 && ql_get_ttyGPS(atdevice)) {
        char ql_ttyGPS[20] = {'\0'};
        sprintf(ql_ttyGPS, "/dev/%s", atdevice);
        s_gps_fd = open (ql_ttyGPS, O_RDWR);
    }
}
#endif
#ifdef USB_HOST_FORCE_OPEN_PPP_PORT
{
    char atdevice[10];
    //open ppp port here to make KERNEL assign hwep resource. or ppp port will lost data
    //and delete source code: kernel/drivers/usb/serial/option.c     .read_int_callback = option_instat_callback
    //to disable ppp port 's usb interrupt endpoint
    if (ql_mux_enabled == 0 && ql_get_ttyPPP(atdevice)) {
        char ql_ttyPPP[20] = {'\0'};
        sprintf(ql_ttyPPP, "/dev/%s", atdevice);
        //if open moderm port here and pppd exit with 16 (EXIT_HANGUP), will can not send AT on modem port(Try again(11))! (so why?)
        s_ppp_fd = open (ql_ttyPPP, O_RDWR);
        sleep(1);
    }
}
#endif

#if 1 //quectel
    if (s_recovery_mode && (currentState() == RADIO_STATE_SIM_READY || currentState() == RADIO_STATE_RUIM_READY)) { //maybe only usb disconnect
    } else
#endif
    setRadioState (RADIO_STATE_OFF);

    at_handshake();

#if 1 //quectel display module software version
__get_ql_product:
    ql_is_UC20 = ql_is_mdm9215 = ql_is_mdm9x07 = ql_is_UG95 = ql_is_GSM = 0;
    ql_product_version = NULL; //"GSMXX"
    err = at_send_command_multiline("ATI", "\0", &p_response);
    if (!err && p_response && p_response->success) {
        ATLine *p_cur = p_response->p_intermediates;
        while (p_cur) {
             if (strStartsWith(p_cur->line, "Revision: ")) {
                ql_product_version = strdup(p_cur->line + strlen("Revision: "));
                break;
            }
            p_cur = p_cur->p_next;
        }
    }
    at_response_free(p_response);
    p_response = NULL;
    if (!ql_product_version) {
        sleep(1);
        goto __get_ql_product;
    }
    LOGD("Quectel Product Revision: %s", ql_product_version);
    if (ql_is_XX("EC20CEF") || ql_is_XX("EC20CF") || ql_is_XX("EC20EF")) {
        ((char *)ql_product_version)[3] = '5';
    }
    if (ql_is_XX("UC15") || ql_is_XX("UC20")) {
        ql_is_UC20 = 1;
        LOGD("UCXX");
    } else if (ql_is_XX("EC20")) {
        ql_is_mdm9215 = 1;
        LOGD("9X15");
    } else if (ql_is_9X07()) {
        ql_is_mdm9x07 = 1;
        LOGD("9X07");
    } else if (ql_is_XX("UG95") || ql_is_XX("UG96")) {
        ql_is_UG95 = 1;
        LOGD("UGXX");
    } else {
        ql_is_GSM = 1;
        LOGD("GSMXX");
    }

    if (ql_is_XX("EC20CE") || ql_is_XX("EC21CE") || ql_is_XX("EC25CE"))
        ql_is_device_support_cdma = 1;

    if (ql_is_UC20 || ql_is_mdm9215 || ql_is_mdm9x07 || ql_is_XX("MC20") || ql_is_XX("MC60")) {
        if (!access("/system/etc/gps_cfg.inf", R_OK)) //with Quectel GPS HAL
            ql_gps_init(ql_product_version);
    }

    if (ql_is_mdm9215 || ql_is_mdm9x07)
        ql_datafwd_init();

    at_send_command_multiline("AT+CSUB;+CVERSION", "\0", NULL);
#endif

    /* note: we don't check errors here. Everything important will
       be handled in onATTimeout and onATReaderClosed */

    /*  atchannel is tolerant of echo but it must */
    /*  have verbose result codes */
    at_send_command("ATE0Q0V1", NULL);

    /*  No auto-answer */
    at_send_command("ATS0=0", NULL);

//Joe
    if (ql_mux_enabled) {
        at_send_command("AT+QURCCFG=\"URCPORT\",\"UART1\"", NULL);
        at_send_command("AT+QCFG=\"CMUX/URCPORT\",1", NULL);
    } else
        at_send_command("AT+QURCCFG=\"URCPORT\",\"usbat\"", NULL); //urcport",("usbat","usbmodem","uart1"

#if 1 // quectel Set DTR Function Mode
//ON->OFF on DTR: Disconnect data call, change to command mode. During state DTR = OFF, auto-answer function is disabled
    at_send_command("AT&D2", NULL);
#endif

    /*  Extended errors */
    at_send_command("AT+CMEE=1", NULL);

    /*  Network registration events */
    at_send_command("AT+CREG=2", NULL);

//there is a change in the ME network registration status or a change of the network cell:
//+CGREG: <stat>[, <lac>,<ci> ]
    at_send_command("AT+CGREG=2", NULL);

#if 1 //quectel
    if (ql_is_UC20) {
        at_send_command("AT+QINDCFG=\"all\",1", NULL);
        at_send_command("AT+QINDCFG=\"smsfull\",1", NULL);
    }

    if (ql_is_UC20 || ql_is_mdm9215)
        at_send_command("AT+QCFG=\"QMISYNC\",0", NULL);

    if (ql_is_UC20 || ql_is_mdm9215) {
        //at_send_command("AT+QCFG=\"pwrsavedtr\",0", NULL); //Enable/Disable DTR to Control Power Save State
        //at_send_command("AT+QCFG=\"risignaltype\", \"respective\"", NULL);
        //No matter which port URC is presented on, URC only causes the behavior of physical ring line
        //at_send_command("AT+QCFG=\"risignaltype\", \"physical\"", NULL);
    }

    if (ql_is_mdm9215) { //can be used for all LTE modules? i think
        //at_send_command("AT+CGDCONT=1,\"IPV4V6\",\"\"",  NULL);
        at_send_command("at+qcfg=\"pdp/duplicatechk\",1", NULL); //carl allow different pdp use same APN, only for EC20?
    }

#if 0
//????????ר??????APN??Ϣ??һЩר?�����??ע?Ϲ????
//??ֻ????????ר??APN???????£?????ע??ר??
//?????ǵ?һ?????ã????ܻ???Ҫ?ȷ???AT+CFUN=4, Ȼ??AT+CFUN=1
    at_send_command("AT+QICSGP=1,1,\"your_apn\", \"your_username\",\"your_password\",1", NULL);
#endif

#if 0
    at_send_command("AT+QCFG=\"usb/fullspeed\",1", NULL);
#endif
#endif

    /* assume radio is off on error */
    if (isRadioOn() != 1) {
        at_send_command("AT+CFUN=1", NULL);
        sleep(3);
    }
    err = at_send_command_singleline("AT+CPIN?", "+CPIN:", &p_response); ////may be busy, wait a moment
    if ((err != 0) || !p_response || at_get_cme_error(p_response))
        sleep(1);
    at_response_free(p_response);
    p_response = NULL;

    if (isRadioOn() > 0) {
#if 1 //quectel
        if (s_recovery_mode && (currentState() == RADIO_STATE_SIM_READY || currentState() == RADIO_STATE_RUIM_READY)) {//maybe only usb disconnect
            if (getSIMStatus() == SIM_READY)
                onSIMReady();
            else
                setRadioState (RADIO_STATE_SIM_NOT_READY);
            RIL_onUnsolicitedResponse (RIL_UNSOL_RESPONSE_RADIO_STATE_CHANGED, NULL, 0);
            RIL_onUnsolicitedResponse (RIL_UNSOL_RESPONSE_VOICE_NETWORK_STATE_CHANGED, NULL, 0);
            RIL_onUnsolicitedResponse(RIL_UNSOL_DATA_CALL_LIST_CHANGED, NULL, 0);
        } else {
            setRadioState (RADIO_STATE_SIM_NOT_READY);
        }
#else
        setRadioState (RADIO_STATE_SIM_NOT_READY);
#endif
    }
    s_recovery_mode = 0;

    RIL_requestTimedCallback(initializeCustomerCallback, NULL, &TIMEVAL_10);
    RIL_requestTimedCallback(initializeLaterCallback, NULL, &TIMEVAL_1);
#ifdef QUECTEL_REPORT_SIGNAL_STRENGTH
    if (0 == poll_signal_started) {
        RIL_requestTimedCallback(onReportSignalStrength, NULL, &TIMEVAL_0);
        poll_signal_started = 1;
    }
#endif
    if (access(QL_DEBUG_LOG_PATH, W_OK) == 0)
        RIL_requestTimedCallback(QuectelCheckAndroidFrameworkSettingCallback, NULL, &TIMEVAL_40);
}

static void waitForClose()
{
    pthread_mutex_lock(&s_state_mutex);

    while (s_closed == 0) {
        pthread_cond_wait(&s_state_cond, &s_state_mutex);
    }

    pthread_mutex_unlock(&s_state_mutex);
}

/**
 * Called by atchannel when an unsolicited line appears
 * This is called on atchannel's reader thread. AT commands may
 * not be issued here
 */
static void onUnsolicited (const char *s, const char *sms_pdu)
{
    char *line = strdup(s);
    int err;
    char *dup_line = line;
    int cops_response[4] = {0};
    /* Ignore unsolicited responses until we're initialized.
     * This is OK because the RIL library will poll for initial state
     */
    if (sState == RADIO_STATE_UNAVAILABLE) {
        goto __free_line;
    }

    if (strStartsWith(s, "%CTZV:")) {
        /* TI specific -- NITZ time */
        char *response;

        at_tok_start(&line);

        err = at_tok_nextstr(&line, &response);

        if (err != 0) {
            LOGE("invalid NITZ line %s\n", s);
        } else {
            RIL_onUnsolicitedResponse (
                RIL_UNSOL_NITZ_TIME_RECEIVED,
                response, strlen(response));
        }
    } else if (strStartsWith(s,"+CRING:")
                || strStartsWith(s,"RING")
                || strStartsWith(s,"NO CARRIER")
                || strStartsWith(s,"+CCWA")
    ) {
        RIL_onUnsolicitedResponse (
            RIL_UNSOL_RESPONSE_CALL_STATE_CHANGED,
            NULL, 0);
    } else if (strStartsWith(s,"+CREG:")
                || strStartsWith(s,"+CGREG:")
    ) {
#if 1 //quectel
    if (currentDataServiceState() && (network_debounce_time == 0))
        network_debounce_time = NETWORK_DEBOUNCE_TIMEOUT;
#endif
        RIL_onUnsolicitedResponse (
            RIL_UNSOL_RESPONSE_VOICE_NETWORK_STATE_CHANGED,
            NULL, 0);
    } else if (strStartsWith(s, "+CMT:")) {
        RIL_onUnsolicitedResponse (
            RIL_UNSOL_RESPONSE_NEW_SMS,
            sms_pdu, strlen(sms_pdu));
    } else if (strStartsWith(s, "+CDS:")) {
        RIL_onUnsolicitedResponse (
            RIL_UNSOL_RESPONSE_NEW_SMS_STATUS_REPORT,
            sms_pdu, strlen(sms_pdu));
    } else if (strStartsWith(s, "+CGEV:")) {
        /* Really, we can ignore NW CLASS and ME CLASS events here,
         * but right now we don't since extranous
         * RIL_UNSOL_DATA_CALL_LIST_CHANGED calls are tolerated
         */
        /* can't issue AT commands here -- call on main thread */
        RIL_requestTimedCallback (onDataCallListChanged, NULL, NULL);
    }
#if 1 //quectel
    else if(strStartsWith(s,"+CTZE:")) { //NITZ
        //+CTZE: "+32",0,"2014/05/05,05:05:30"
        //yy/mm/dd,hh:mm:ss(+/-)tz,dt
        char *tz = NULL;
        int dt;
        char *n_time = NULL;
        char *response = NULL;

        at_tok_start(&line);
        err = at_tok_nextstr(&line,&tz);
        if (err < 0) goto __free_line;
        err = at_tok_nextint(&line,&dt);
        if (err < 0) goto __free_line;
        err = at_tok_nextstr(&line,&n_time);
        if (err < 0) goto __free_line;

        if (n_time[4] == '/')
            asprintf(&response,"%s%s,%d",n_time+2,tz,dt); //2014 -> 14
        else
            asprintf(&response,"%s%s,%d",n_time,tz,dt);
        time_zone_report = 1;

        RIL_onUnsolicitedResponse (RIL_UNSOL_NITZ_TIME_RECEIVED,response, strlen(response));
    } else if (strStartsWith(s, "+CMTI:")) {
        char *memory = NULL;
        int index;
        at_tok_start(&line);
        err = at_tok_nextstr(&line,&memory);
        if (err < 0) goto __free_line;
        if(!strcmp("SM",memory))
        {
            err = at_tok_nextint(&line,&index);
            if (err < 0) goto __free_line;
             RIL_onUnsolicitedResponse (RIL_UNSOL_RESPONSE_NEW_SMS_ON_SIM,&index, sizeof(int *));
        }
    } else if (strStartsWith(s, "^HCMT:")) {
        unsigned char pdu_buf[512];
        int pdu_len;
        RIL_CDMA_SMS_Message rcsm;

        pdu_len = hexStringToBytes(sms_pdu, pdu_buf);
        LOGD("pdu_len = %d", pdu_len);
        LOGD("sms_pdu = %s", sms_pdu);
        if (strlen(sms_pdu) > 4 && QL_RIL_CDMASMS_sDecodePDUP2P(0, &rcsm, pdu_buf, pdu_len))
        {
            char tmp[1024];
            int i;
            LOGD("uTeleserviceID: %d", rcsm.uTeleserviceID);
            LOGD("bIsServicePresent: %d", rcsm.bIsServicePresent);
            LOGD("uServicecategory: %d", rcsm.uServicecategory);

            LOGD("sAddress.digit_mode: %d", rcsm.sAddress.digit_mode);
            LOGD("sAddress.number_mode: %d", rcsm.sAddress.number_mode);
            LOGD("sAddress.number_type: %d", rcsm.sAddress.number_type);
            LOGD("sAddress.number_plan: %d", rcsm.sAddress.number_plan);
            LOGD("sAddress.number_of_digits: %d", rcsm.sAddress.number_of_digits);
            for (i = 0; i < rcsm.sAddress.number_of_digits; i++)
                sprintf(&tmp[i*3], "%02x ", rcsm.sAddress.digits[i]);
            LOGD("sAddress.digits: %s", tmp);

            LOGD("sSubAddress.subaddressType: %d", rcsm.sSubAddress.subaddressType);
            LOGD("sSubAddress.odd: %d", rcsm.sSubAddress.odd);
            LOGD("sSubAddress.number_of_digits: %d", rcsm.sSubAddress.number_of_digits);
            for (i = 0; i < rcsm.sSubAddress.number_of_digits; i++)
                sprintf(&tmp[i*3], "%02x ", rcsm.sSubAddress.digits[i]);
            LOGD("sSubAddress.digits: %s", tmp);

            LOGD("uBearerDataLen: %d", rcsm.uBearerDataLen);
            for (i = 0; i < rcsm.uBearerDataLen; i++)
                sprintf(&tmp[i*3], "%02x ", rcsm.aBearerData[i]);
            LOGD("aBearerData: %s", tmp);

             RIL_onUnsolicitedResponse (RIL_UNSOL_RESPONSE_CDMA_NEW_SMS, &rcsm, sizeof(rcsm));
        }
    } else if (strstr(s, "POWERED DOWN")) {
        if (wait_for_powerdown == 1)
            wait_for_powerdown = 2;
    } else if (strStartsWith(s, "+QIND: \"smsfull\"")) {
        char *memory = NULL;
        char *tmp = NULL;
        at_tok_start(&line);
        err = at_tok_nextstr(&line,&tmp);
        if (err < 0) goto __free_line;
        err = at_tok_nextstr(&line,&memory);
        if (err < 0) goto __free_line;
        if(!strcmp("SM",memory))
        {
             RIL_onUnsolicitedResponse (RIL_UNSOL_SIM_SMS_STORAGE_FULL,NULL,0);
        }
#if 1 // RIL will get sim state by pollSIMState()
    } else if (strStartsWith(s, "+CPIN: READY") || strStartsWith(s, "+QUSIM:") || strStartsWith(s, "+QIND: SMS DONE") || strStartsWith(s, "+QIND: PB DONE")) {
        if (currentState() == RADIO_STATE_SIM_LOCKED_OR_ABSENT)
            RIL_requestTimedCallback (onSIMStateChange, NULL, NULL); //setRadioState(RADIO_STATE_SIM_READY); //cannot call at_send_command() here, because they are in same thread
#endif
    } else if (strStartsWith(s, "+CPIN: NOT READY")) {
        if (currentState() == RADIO_STATE_SIM_READY || currentState() == RADIO_STATE_RUIM_READY)
            RIL_requestTimedCallback (onSIMStateChange, NULL, NULL); //setRadioState(RADIO_STATE_SIM_READY); //cannot call at_send_command() here, because they are in same thread
    } else if (strStartsWith(s, "+CUSD")) {
        char *response[2];
        char *data;
        char *mode;
        int dcs;
        int outlen = 0;
        int err;

        ussd_pending_index++;
        at_tok_start(&line);
        err = at_tok_nextstr(&line,&mode);
        if(err != 0)
        {
            LOGE("at_tok_nextstr[mode] error");
            goto __free_line;
        }
        err = at_tok_nextstr(&line,&data);
        if(err != 0)
        {
            LOGE("at_tok_nextstr[data] error");
            onUssdResponse(mode);
            goto __free_line;
        }
        err = at_tok_nextint(&line,&dcs);
        if(err != 0)
        {
            LOGE("at_tok_nextstr[dcs] error");
            goto __free_line ;
        }
        dcs &= 0x0c;

        response[0] = (char *)malloc(strlen(mode) + 1);
        memcpy(response[0],mode,strlen(mode) + 1);

        response[1] = (char *)malloc(strlen(data) + 1);

#ifdef CUSD_USE_UCS2_MODE
        if(1)//UCS2
#else
        if(dcs == 0x08)//UCS2
#endif
        {
            //unsigned char *tmp = (unsigned char *)malloc(sizeof(data)/2*3);
            size_t i = 0;
            unsigned char tmp[512];
            while(i < strlen(data))
            {
                tmp[i/2] = (unsigned char)gsm_hex2_to_byte(&data[i]);
                i += 2;
            }
            outlen = ucs2_to_utf8(tmp,i/4,(bytes_t)response[1]);
            response[1][outlen] = 0;
            //free(tmp);
        }
#if 0 // CUSD_USE_UCS2_MODE
        else if(dcs == 0x04)// 8 bit data
        {
            //memcpy(response[1],data,strlen(data) + 1);
            outlen = gsm_hex_to_bytes((bytes_t)data,strlen(data),(bytes_t)response[1]);
        }
        else//GSM 7 bit data, infact module gives us GSM 8 bit data
        {
            // memcpy(response[1],data,strlen(data) + 1);
            outlen = utf8_from_gsm8((cbytes_t)data,strlen(data),(bytes_t)response[1]);
        }
#endif
        LOGI("len:%zd response[0]:%s",strlen(response[0]),response[0]);
        LOGI("len:%zd[%d] response[1]:%s",strlen(response[1]),outlen,response[1]);

        RIL_onUnsolicitedResponse(RIL_UNSOL_ON_USSD, response, sizeof(response[0]) + sizeof(response[1]));

        free(response[0]);
        free(response[1]);
    } else if (strStartsWith(s, "+QDATAFWD: ")) {
        char *hex_string = NULL;

        err = at_tok_start(&line);
        if (err < 0)
            goto __free_line;

        if (line[1] != '\"') { //old style: +QDATAFWD: 313233,6
            char *comma = strchr(line, ',');
            while (*line != '\0' && isspace(*line))
                hex_string = ++line; //skip space
          if ((comma - hex_string - 1) > 0)
                ql_datafwd_send_to_apk((unsigned char *)hex_string, comma - hex_string);
        } else { // //new style: +QDATAFWD: "313233",6
            err = at_tok_nextstr(&line, &hex_string);
            if (err < 0)
                goto __free_line;
            ql_datafwd_send_to_apk((unsigned char *)hex_string, strlen(hex_string));
        }
    } else if (strStartsWith(s, "+QTONEDET: ")) {
    //the customer's apk should connect local socket "rild-dtmf" to receive dtmf code!
    //LocalSocket s = new LocalSocket();
    //LocalSocketAddress l = new LocalSocketAddress("rild-dtmf", LocalSocketAddress.Namespace.ABSTRACT);
    //s.connect(l);
        ql_dtmf_send_to_apk(s[strlen("+QTONEDET: ")]);
    }
#endif
__free_line:
    free(dup_line);
}

/* Called on command or reader thread */
static void onATReaderClosed()
{
    LOGI("AT channel closed\n");
    at_close();
    s_closed = 1;
    s_recovery_mode = 0;
#ifdef USB_HOST_FORCE_OPEN_PPP_PORT
    if (s_ppp_fd != -1) {
        close (s_ppp_fd); s_ppp_fd = -1;
    }
#endif
#ifdef USB_HOST_FORCE_OPEN_GPS_PORT
    if (s_gps_fd != -1) {
        close (s_gps_fd); s_gps_fd = -1;
    }
#endif

    if (s_recovery_mode == 0) {
        RIL_onUnsolicitedResponse (RIL_UNSOL_RESPONSE_SIM_STATUS_CHANGED, NULL, 0); //clean SIMRecords.java info
    }

#if 1 //quectel
    if (wait_for_powerdown) {
        wait_for_powerdown = 3;
        sleep(60);
    }
    setDataServiceState(0);
    if (!strncmp(PPP_TTY_PATH, "ppp", 3))
        ql_pppd_stop(SIGKILL);
    else
        ql_ndis_stop(SIGKILL);
    bSetupDataCallCompelete = 0;
#if 1 //def QUECTEL_DEBUG //quectel //for debug-purpose, record logcat msg to file
    log_dmesg("onATReaderClosed");
#endif
    if (s_recovery_mode && currentState() == RADIO_STATE_SIM_READY) {//maybe only usb disconnect
        pthread_mutex_lock(&s_state_mutex);
        pthread_cond_broadcast (&s_state_cond);
        pthread_mutex_unlock(&s_state_mutex);
        return;
    }
#endif

    setRadioState (RADIO_STATE_UNAVAILABLE);
}

/* Called on command thread */
static void onATTimeout()
{
    LOGI("AT channel timeout; closing\n");
    at_close();
#ifdef USB_HOST_FORCE_OPEN_PPP_PORT
    if (s_ppp_fd != -1) {
        close (s_ppp_fd); s_ppp_fd = -1;
    }
#endif
#ifdef USB_HOST_FORCE_OPEN_GPS_PORT
    if (s_gps_fd != -1) {
        close (s_gps_fd); s_gps_fd = -1;
    }
#endif

    s_closed = 1;
    s_recovery_mode = 0;

    if (s_recovery_mode == 0) {
        RIL_onUnsolicitedResponse (RIL_UNSOL_RESPONSE_SIM_STATUS_CHANGED, NULL, 0); //clean SIMRecords.java info
    }

    setDataServiceState(0);
    if (!strncmp(PPP_TTY_PATH, "ppp", 3))
        ql_pppd_stop(SIGKILL);
    else
        ql_ndis_stop(SIGKILL);
    bSetupDataCallCompelete = 0;
#if 1 //def QUECTEL_DEBUG //quectel //for debug-purpose, record logcat msg to file
    log_dmesg("onATTimeout");
#endif

    /* FIXME cause a radio reset here */

    setRadioState (RADIO_STATE_UNAVAILABLE);
#ifdef QUECTEL_DEBUG //quectel //for debug-purpose, record logcat msg to file
    //log_dmesg("onATTimeout");
#endif
}

static void usage(char *s)
{
#ifdef RIL_SHLIB
    fprintf(stderr, "reference-ril requires: -p <tcp port> or -d /dev/tty_device\n");
#else
    fprintf(stderr, "usage: %s [-p <tcp port>] [-d /dev/tty_device]\n", s);
    exit(-1);
#endif
}

static void clean_up_child_process(int signal_num) {
    /* clean up child process */
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

static void *
mainLoop(void *param)
{
    int fd;
    int ret;

#if 1 //def QUECTEL_DEBUG //quectel //for debug-purpose, record logcat msg to file
//you can fetch logfiles to host-pc by adb tools using command "adb pull /data/quectel_debug_log/"
    if (access(QL_DEBUG_LOG_PATH, W_OK) == 0 && ql_debug_space_available()) {
        char logcat_cmd[256];
        time_t rawtime;
        struct tm *timeinfo;

        time(&rawtime);
        timeinfo=localtime(&rawtime );
        sprintf(logcat_cmd, "/system/bin/logcat -v time -f " QL_DEBUG_LOG_PATH "/%02d%02d_%02d%02d%02d_logcat.txt &",
            timeinfo->tm_mon+1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
        system(logcat_cmd);
        sprintf(logcat_cmd, "/system/bin/logcat -b radio -v time -f " QL_DEBUG_LOG_PATH "/%02d%02d_%02d%02d%02d_radio.txt &",
        	timeinfo->tm_mon+1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
        system(logcat_cmd);
    }
#endif

    signal(SIGCHLD, clean_up_child_process); //becasue use fork() to start and stop pppd
    LOGI("mainLoop Start");
    AT_DUMP("== ", "entering mainLoop()", -1 );
    at_set_on_reader_closed(onATReaderClosed);
    at_set_on_timeout(onATTimeout);

    for (;;) {
            //1. can not get at port for long time, maybe quectel-module is power-off
            //2. when AT timeout occurs, need to reboot quectel-module
                // turn off quectel-module's power-supply here
                //sleep(2); //wait power-supply stable
                //turn on quectel-module's power-supply here
                //sleep(2); //wait power-supply stable
                //use pwr-key to turn on quectel-module here
                //sleep(5); //wait quectel-power boot up
    if (ql_mux_enabled) {
        if (gsm0710muxd(s_device_path, cmux_speed, cmux_ctsrts)) {
            sleep(3);
            continue;
        }
    } else {
        if (s_device_path == ql_ttyAT) {
            char atdevice[10];

            ql_detect_quectel_modules();
            if (!ql_get_ttyAT(atdevice)) {
                sleep(3);
                continue;
            }
            
            sprintf(ql_ttyAT, "/dev/%s", atdevice);
            LOGD("quectel at port is %s", ql_ttyAT);
        }
    }
#if 1//def QUECTEL_DEBUG //quectel //for debug-purpose, record logcat msg to file
        log_dmesg("mainLoop");
#endif

        fd = -1;
        while  (fd < 0) {
            if (s_device_path != NULL) {
                if (ql_mux_enabled) {
                    LOGD("open " CMUX_AT_PORT);
                    fd = open (CMUX_AT_PORT, O_RDWR);
                    LOGD("fd = %d", fd);
                } else {
                fd = open (s_device_path, O_RDWR);
                }
                if ( fd >= 0 ) {
                    /* disable echo on serial ports */
                    struct termios  ios;
                    memset(&ios, 0, sizeof(ios));
                    tcgetattr( fd, &ios );
                    cfmakeraw(&ios);
                    ios.c_lflag = 0;  /* disable ECHO, ICANON, etc... */
                    cfsetispeed(&ios, B115200);
                    cfsetospeed(&ios, B115200);
                    tcsetattr( fd, TCSANOW, &ios );
                    LOGD("open device %s correctly\n", s_device_path);
                    tcflush(fd, TCIOFLUSH);
                }
            }

            if (fd < 0) {
                LOGE("open device %s error for %s\n", s_device_path, strerror(errno));
                perror ("opening AT interface. retrying...");
#if 1 //quectel
                sleep(3);
#else
                sleep(10);
#endif
                /* never returns */
            }
        }

        s_closed = 0;
        ret = at_open(fd, onUnsolicited);

        if (ret < 0) {
            LOGE ("AT error %d on at_open\n", ret);
            return 0;
        }

        RIL_requestTimedCallback(initializeCallback, NULL, &TIMEVAL_0);

        // Give initializeCallback a chance to dispatched, since
        // we don't presently have a cancellation mechanism
        sleep(1);

        waitForClose();
        sleep(1); //wait for other threads clean
        LOGI("Re-opening after close");
    }
}

#ifdef RIL_SHLIB

pthread_t s_tid_mainloop;

void ql_detect_AT_port()
{
    if (s_device_path != NULL)
    {
        LOGD("hardware/rild/rild.c tell me %s is AT port", s_device_path);
        if (access(s_device_path, R_OK))
        {
            LOGD("but it does not exist, access device %s error for %s\n", s_device_path, strerror(errno));
            LOGD("if you access module by UART interface, this error ususally means critical mistake!");
            LOGD("if you access module by USB interface, i will ingore it, for Quectel RIL will auto detect AT and PPP ports");
            s_device_path = NULL;
        }
        else if (ql_detect_quectel_modules() && ql_is_quectel_ttyport(s_device_path))
        {
            s_device_path = NULL;
        }
    }
}

const RIL_RadioFunctions *RIL_Init(const struct RIL_Env *env, int argc, char **argv)
{
    int ret;
    int fd = -1;
    int opt;
    pthread_attr_t attr;
    struct passwd *pwd;

    char prop_value[PROPERTY_VALUE_MAX] = {'\0'};
    int anroid_version = 255;
    int anroid_sub_version = 0;

    LOGD("Quectel RIL Version: " REFERENCE_RIL_VERSION);
#ifdef USE_GIT
	LOGD("\n%s\n", s_ql_buildinfo);
#endif

    pwd = getpwuid(getuid());
    if (strcmp(pwd->pw_name, "root")) {
        LOGE("Error: the user of process rild is %s, it should be root.", pwd->pw_name);
        LOGE("Please follow RIL document to comment rild.c/switchUser()");
        LOGE("Please check service ril-daemon defined in all init*.rc, make sure the user is root");
       // sleep(10);
        pwd = getpwuid(getuid());
    }

    read_conf_of_ril_init();
#ifdef USE_NDK
    ql_ndk_init();
#endif

    if (property_get("ro.build.version.release", prop_value, NULL) > 0) {
        anroid_version = (prop_value[0] - '0') * 10 + (prop_value[2] - '0');
        if (strlen(prop_value) == strlen("2.3.4"))
            anroid_sub_version = (prop_value[4] - '0');
    }
    LOGD("[ro.build.version.release]: [%s]", prop_value);
    if (anroid_version == 23 && anroid_sub_version == 1) QL_RIL_VERSION = 3; //2.3.1
    else if (anroid_version == 23) QL_RIL_VERSION = 4; //2.3.4
    else if (anroid_version <= 40) QL_RIL_VERSION = 6;
    else if (anroid_version == 41) QL_RIL_VERSION = 6;
    else if (anroid_version == 42) QL_RIL_VERSION = 7;
    else if (anroid_version == 43) QL_RIL_VERSION = 8; //maybe should be 7
    else if (anroid_version == 44) QL_RIL_VERSION = 9;
    else if (anroid_version == 50) QL_RIL_VERSION = 10;
    else if (anroid_version == 51) QL_RIL_VERSION = 11;
    else if (anroid_version == 60) QL_RIL_VERSION = 11;
    else if (anroid_version == 71) QL_RIL_VERSION = 12;
    else LOGE("Unsupport Android Version %d by Quectel Now!!!!", anroid_version);
    LOGD("Android Version: %d, RIL_VERSION: %d / %d", anroid_version, QL_RIL_VERSION, RIL_VERSION);
#ifndef USE_NDK
    if (QL_RIL_VERSION != RIL_VERSION) //maybe get error when dynamic detect ril version
        QL_RIL_VERSION = RIL_VERSION;
#endif
    s_callbacks.version = QL_RIL_VERSION;
    if (property_get("ro.build.description", prop_value, NULL) > 0) {
        LOGD("[ro.build.description]: [%s]", prop_value);
    }
    if (property_get("rild.libpath", prop_value, NULL) > 0) {
        LOGD("[rild.libpath]: [%s]", prop_value);
    }
    if (property_get("rild.libargs", prop_value, NULL) > 0) {
        LOGD("[rild.libargs]: [%s]", prop_value);
    }

    if ((QL_RIL_VERSION > 9) && access("/sys/fs/selinux", F_OK) == 0) {
        /* SELinux is not compiled into the kernel, or has been disabled via the kernel command line "selinux=0". */
        LOGD("selinux maybe set Enforcing mode, use command getenforce to check");
        LOGD("selinux maybe set Enforcing mode, use command \"setenforce 0\" to disable");
        /* append next selinux rulers to external/sepolicy/file_contexts
        # Quectel RIL
        /dev/ttyUSB[0-9]       u:object_r:tty_device:s0
        /dev/ttyACM[0-9]       u:object_r:tty_device:s0
        */
         /* append next selinux rulers to external/sepolicy/rild.te
        # Quectel RIL
        allow rild logd:unix_stream_socket connectto;
        allow rild logdr_socket:sock_file write;
        allow rild ppp_device:chr_file { read write ioctl open };
        allow rild ppp_exec:file { read execute open execute_no_trans };
        allow rild self:capability { sys_module setgid dac_override };
        allow rild self:capability2 { syslog };
        allow rild kernel:system { syslog_read syslog_mod };
        allow rild system_data_file:dir {create write add_name};
        allow rild system_data_file:file { write create append };
        allow rild system_data_file:lnk_file create;
        allow rild tmpfs:lnk_file read;
        allow rild devpts:chr_file { write read ioctl open getattr };
        */
    }

    property_set("telephony.lteOnCdmaDevice", "0");  // "1" will make PhoneFactory.java create CDMALTEPhone

    s_rilenv = env;

    while ( -1 != (opt = getopt(argc, argv, "p:d:s:c:C:B:b:"))) {
        switch (opt) {
            case 'p':
                s_port = atoi(optarg);
                if (s_port == 0) {
                    usage(argv[0]);
                    return NULL;
                }
                LOGI("Opening loopback port %d\n", s_port);
            break;

            case 'd':
                s_device_path = optarg;
                if (strStartsWith(optarg, "tty")) { //indians
                    char *devname = NULL;
                    asprintf(&devname, "/dev/%s", optarg);
                    s_device_path = devname;
                }
                LOGI("Opening tty device %s\n", s_device_path);
            break;

            case 's':
                s_device_path   = optarg;
                s_device_socket = 1;
                LOGI("Opening socket %s\n", s_device_path);
            break;

            case 'c':
                LOGI("clientID = %d\n", atoi(optarg));
            break;

            case 'C':
                cmux_ctsrts = !!atoi(optarg);
                LOGI("cmux_ctsrts = %d\n", cmux_ctsrts);
              break;

            case 'b':
            case 'B':
                cmux_speed = atoi(optarg);
                LOGI("cmux_speed = %d\n", cmux_speed);
            break;

            default:
                usage(argv[0]);
                LOGI("unkonw opt = %c\n", opt);
            break;
        }
    }
    ql_detect_AT_port();

    if (s_port < 0 && s_device_path == NULL) {
#if 1 //quectel //get ttyAT dymanic
    s_device_path = ql_ttyAT;
#else
        usage(argv[0]);
        return NULL;
#endif
    }

    if (s_device_path != ql_ttyAT) {
#ifdef USE_MUXD
        ql_mux_enabled = 1;
#endif
    }

    if (ql_mux_enabled == 0 && access(QL_DEBUG_LOG_PATH, W_OK) == 0) {
        //first adb forward tcp:9000 tcp:9000,  second QSPT(127.0.0.1:9000) & QXDM
        ql_tty2tcp("s:9000", "c:/dev/ttyUSB0", 512*1024);
    }

    pthread_attr_init (&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    ret = pthread_create(&s_tid_mainloop, &attr, mainLoop, NULL);

    return &s_callbacks;
}
#else /* RIL_SHLIB */
int main (int argc, char **argv)
{
    int ret;
    int fd = -1;
    int opt;

    while ( -1 != (opt = getopt(argc, argv, "p:d:"))) {
        switch (opt) {
            case 'p':
                s_port = atoi(optarg);
                if (s_port == 0) {
                    usage(argv[0]);
                }
                LOGI("Opening loopback port %d\n", s_port);
            break;

            case 'd':
                s_device_path = optarg;
                LOGI("Opening tty device %s\n", s_device_path);
            break;

            case 's':
                s_device_path   = optarg;
                s_device_socket = 1;
                LOGI("Opening socket %s\n", s_device_path);
            break;

            default:
                usage(argv[0]);
        }
    }

    if (s_port < 0 && s_device_path == NULL) {
        usage(argv[0]);
    }

    RIL_register(&s_callbacks);

    mainLoop(NULL);

    return 0;
}

#endif /* RIL_SHLIB */


