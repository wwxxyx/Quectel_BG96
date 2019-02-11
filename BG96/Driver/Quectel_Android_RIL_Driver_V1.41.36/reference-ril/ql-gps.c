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
#include "../include/telephony/ril.h"
#include <linux/un.h>
#include <linux/poll.h>

#define LOG_NDEBUG 0
#define LOG_TAG "GPS"
#include "ql-log.h"
extern int ql_tty2tcp(const char *s, const char *d, int fifo_size);

typedef struct _GPS_EVENT {
    RIL_TimedCallback callback;
    void *param;
    long expire_msec;
} GPS_EVENT;

typedef struct _GPS_TLV {
   int type;
   int length;
   unsigned char data[0];
} GPS_TLV;

static int ql_gps_control[2];
static int  ql_gps_recv_cmd_control[2];
static int  ql_gps_send_nmea_control[2];
static int ql_is_mc20_gps = 0;
static int s_gps_state = 0;
static int s_agps_check_times = 0;

static void GPS_requestTimedCallback (RIL_TimedCallback callback, void *param, long expire_msec) {
    if (ql_gps_control[1] > 0) {
        GPS_EVENT gps_event = {callback, param, expire_msec};
        //LOGD("%s callback = %p, expire_msec = %ld\n", __func__, callback, expire_msec);
        if (write(ql_gps_control[1], &gps_event, sizeof(gps_event)) != sizeof(gps_event)) {
            LOGE("%s send errno : %d (%s)", __func__, errno, strerror(errno));
        }
    }
}

static void pollXTRAStateChange (void *param) {
    if (s_gps_state && s_agps_check_times--) {
        int xtradatadurtime = 0;
        ATResponse *p_response = NULL;
        int err = at_send_command_singleline("AT+QGPSXTRADATA?", "+QGPSXTRADATA: ", &p_response);
        if (err == 0 && p_response != NULL && p_response->success == 1) {
            char *line = p_response->p_intermediates->line;
            if (at_tok_start(&line) == 0) {
                at_tok_nextint(&line, &xtradatadurtime);
            }
        }   
        at_response_free(p_response);
        if (xtradatadurtime == 0)
            GPS_requestTimedCallback (pollXTRAStateChange, NULL, 1000);
    }    
}

static void pollNMEAReader (void *param) {
    //LOGD("%s %d %d", __func__, s_gps_state, ql_mc20_nmea_client);
    if (s_gps_state) {
        ATResponse *p_response = NULL;
        
        int err = at_send_command_multiline("AT+QGNSSRD?", "\0", &p_response);
        if (err == 0 && p_response != NULL && p_response->success == 1) {
            ATLine *p_cur = p_response->p_intermediates;
           while (p_cur) {
                const char *line = p_cur->line;
                if (!strncmp(line, "+QGNSSRD: ", strlen("+QGNSSRD: ")))
                    line += strlen("+QGNSSRD: ");

                //LOGD("%s", line);
                {
                    char nmea[128];
                    unsigned int len = strlen(line);
                    if (len < (sizeof(nmea)-1)) {
                        strcpy(nmea, line);
                        nmea[len++] = '\n';
                        if (write(ql_gps_send_nmea_control[0], nmea, len) != len)
                            LOGD("send nmea errno: %d (%s)", errno, strerror(errno));
                    }
                }
            
                p_cur = p_cur->p_next;
            }
        }
        
        at_response_free(p_response);
    } 
    
    if (s_gps_state)
        GPS_requestTimedCallback (pollNMEAReader, NULL, 1000);    
}

static time_t s_last_inject_time = 0;
static int s_last_inject_uncertainty = 10;
static void *s_last_inject_xtra_data = NULL;
static int s_last_inject_xtra_length = 0;
static void onGPSStateChange (void *param)
{
    char *cmd;
    ATResponse *p_response = NULL;
    int oldState = 0xff;
    GPS_TLV *extra_gps_tlv = (GPS_TLV *)param;
    int err = 0;

    if (ql_is_mc20_gps)
        err = at_send_command_singleline("AT+QGNSSC?", "+QGNSSC: ", &p_response);
    else
        err = at_send_command_singleline("AT+QGPS?", "+QGPS: ", &p_response);

    if (err == 0 && p_response != NULL && p_response->success == 1) {
        char *line = p_response->p_intermediates->line;
        if (at_tok_start(&line) == 0) {
            at_tok_nextint(&line, &oldState);
        }
    }   
    at_response_free(p_response);

    LOGD("onGPSStateChange = {type=%d, length=%d}", extra_gps_tlv->type, extra_gps_tlv->length);
    if (extra_gps_tlv->type == 0)
    {
        s_gps_state = 0;
        if (oldState == 0) {
            free(extra_gps_tlv);
            return;
        }
        if (ql_is_mc20_gps)
            at_send_command("AT+QGNSSC=0", NULL);
        else
            at_send_command("AT+QGPSEND", NULL);
    } 
    else if (extra_gps_tlv->type == 1)
    {
        s_gps_state = 1;
        if (oldState == 1) {
            if (ql_is_mc20_gps) {
                GPS_requestTimedCallback (pollNMEAReader, NULL, 1000);
            }
            free(extra_gps_tlv);
            return;
        }

        if (!ql_is_mc20_gps && s_last_inject_xtra_data != NULL)
        {
            struct tm tm;
            time_t now = time(NULL);

            if (s_last_inject_time > now)
                now = s_last_inject_time;
            gmtime_r(&now, &tm);
  
            at_send_command("AT+QGPSXTRATAUTO=0", NULL);
            at_send_command("AT+QGPSXTRA=1", NULL);
            at_send_command("AT+QFDEL=\"RAM:xtra2.bin\"", NULL);
            
            asprintf(&cmd, "AT+QFUPL=\"RAM:xtra2.bin\",%d,%d", s_last_inject_xtra_length, 60);
            at_send_command_raw(cmd, s_last_inject_xtra_data, s_last_inject_xtra_length, "+QFUPL:", NULL);
            free(cmd);    
            
            asprintf(&cmd, "AT+QGPSXTRATIME=0, \"%d/%d/%d,%d:%d:%d\",1,1,%d",
                tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, s_last_inject_uncertainty);
            at_send_command(cmd, NULL);
            free(cmd);    
            
            at_send_command("AT+QGPSXTRADATA=\"RAM:xtra2.bin\"", NULL);
            at_send_command("AT+QFDEL=\"RAM:xtra2.bin\"", NULL);
            free(s_last_inject_xtra_data);
            s_last_inject_xtra_data = NULL;

            s_agps_check_times = 15;
            GPS_requestTimedCallback (pollXTRAStateChange, NULL, 1000);
        }
        else if (s_last_inject_xtra_data != NULL)
        {       
            free(s_last_inject_xtra_data);
            s_last_inject_xtra_data = NULL;      
        }

        if (ql_is_mc20_gps) {
            p_response = NULL;
            err = at_send_command("AT+QGNSSC=1", &p_response);
            if (err == 0 && p_response != NULL && p_response->success == 1) {
                GPS_requestTimedCallback (pollNMEAReader, NULL, 1000);
            }
            at_response_free(p_response);
        } else {
            at_send_command("AT+QGPS=1", NULL);
         }
    }
    else if (extra_gps_tlv->type == 23)
    { //inject time
        /** Milliseconds since January 1, 1970 */
        typedef int64_t GpsUtcTime;
        GpsUtcTime gpsutctime; int64_t timeReference; int uncertainty;
        struct tm tm;
        
        memcpy(&gpsutctime, extra_gps_tlv->data, sizeof(gpsutctime));
        memcpy(&timeReference, extra_gps_tlv->data + sizeof(gpsutctime), sizeof(timeReference));
        memcpy(&uncertainty, extra_gps_tlv->data + sizeof(gpsutctime) + sizeof(uncertainty), sizeof(uncertainty));
            
        LOGD("%s(time=%lld, timeReference=%lld, uncertainty=%d)",__FUNCTION__,
            (long long int)(*((int64_t *)&gpsutctime)), (long long int)timeReference, uncertainty);
        
        s_last_inject_time = (gpsutctime+999)/1000;
        s_last_inject_uncertainty = uncertainty;

        gmtime_r(&s_last_inject_time, &tm);

        LOGD("%s GpsUtcTime: \"%d/%d/%d,%d:%d:%d\", uncertainty=%d", __func__,
                tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, s_last_inject_uncertainty);
    }
    else if (extra_gps_tlv->type == 34) 
    { //inject xtra
        if (s_last_inject_xtra_data)
            free(s_last_inject_xtra_data);

        s_last_inject_xtra_data = malloc(extra_gps_tlv->length);
        s_last_inject_xtra_length = extra_gps_tlv->length;
        if (s_last_inject_xtra_data != NULL)
            memcpy(s_last_inject_xtra_data, extra_gps_tlv->data, extra_gps_tlv->length);
    }

    free(extra_gps_tlv);
}

static void * ql_gps_recv_cmd_from_gps_hal(void *param) {
    int fd = ql_gps_recv_cmd_control[0];
    GPS_TLV gps_tlv;
    GPS_TLV *extra_gps_tlv = NULL;
    ssize_t nreads;

    while ((nreads = read(fd, &gps_tlv, sizeof(gps_tlv))) > 0) {
        if (nreads == 1) { //old gps hal only send gps_cmd
            gps_tlv.type = *((unsigned char *)&gps_tlv);
            gps_tlv.length = 0;
        } 

        //gps_tlv->length is the length of data want to read
        //extra_gps_tlv->length is the length of data had read
        extra_gps_tlv = (GPS_TLV *)malloc(sizeof(gps_tlv) + gps_tlv.length);
        extra_gps_tlv->type = gps_tlv.type;
        extra_gps_tlv->length = 0;

        while(gps_tlv.length > 0) {
            nreads = read(fd, extra_gps_tlv->data + extra_gps_tlv->length, gps_tlv.length);
            if (nreads > 0) {
                extra_gps_tlv->length += nreads;
                gps_tlv.length -= nreads;
            }
        }
        
        LOGD("%s = {type=%d, length=%d}", __func__, extra_gps_tlv->type, extra_gps_tlv->length);
        GPS_requestTimedCallback (onGPSStateChange, extra_gps_tlv, 0);            
    }

    LOGD("%s exit errno: %d (%s)", __func__, errno, strerror(errno));
    return NULL;
}

static void * ql_gps_send_nmea_to_gps_hal(void *param) {
    int fd = ql_gps_send_nmea_control[0];
    char tmp[128];
    ssize_t nreads;

    while ((nreads = read(fd, tmp, sizeof(tmp)-1)) > 0) {
        tmp[nreads] = 0;
        LOGD("recevie nema cmd: %s", tmp); 
        if (!strcmp(tmp, "close rild-nema")) {
        }      
    }

    LOGD("%s exit errno: %d (%s)", __func__, errno, strerror(errno));
    return NULL;
}

static void * GpsControlLoop(void *param) {
    unsigned int i;
	int ret;
    int fd = ql_gps_control[0];
    GPS_EVENT s_gps_events[5];

    memset(&s_gps_events, 0x00, sizeof(s_gps_events));
    fcntl(fd, F_SETFD, FD_CLOEXEC);
    fcntl(fd, F_SETFL, O_NONBLOCK);
    
    do {
        fd_set rfds;  
        struct timeval tv;

        FD_ZERO(&rfds);
        FD_SET(fd,&rfds);
   
        long expire_msec = 3600*1000L;
            
        for (i = 0; i < sizeof(s_gps_events)/sizeof(s_gps_events[0]); i++) {           
            if (s_gps_events[i].callback == NULL)
                continue;
            //LOGD("%s callback=%p, expire_msec=%ld", __func__, s_gps_events[i].callback, s_gps_events[i].expire_msec);
            if (s_gps_events[i].expire_msec < 10) {
                s_gps_events[i].callback(s_gps_events[i].param);
                memset(&s_gps_events[i], 0x00, sizeof(s_gps_events[i]));
            }

            if (0 < s_gps_events[i].expire_msec && s_gps_events[i].expire_msec < expire_msec)
                expire_msec = s_gps_events[i].expire_msec ;
        }

        //LOGD("%s expire_msec=%ld", __func__, expire_msec);
        
        tv.tv_sec  = expire_msec / 1000;  
        tv.tv_usec = (expire_msec%1000)*1000;

        do {
            ret = select(fd+1, &rfds, NULL, NULL, &tv);
        } while ((ret < 0) && (errno == EINTR));

        if (ret < 0) {
            LOGD("%s select errno: %d (%s)", __func__, errno, strerror(errno));
            break;
        } else {
            long left_mesc;
            
            left_mesc = tv.tv_sec * 1000 + ((tv.tv_usec + 500) / 1000);
            left_mesc = expire_msec - left_mesc;
            for (i = 0; i < sizeof(s_gps_events)/sizeof(s_gps_events[0]); i++) {
                if (s_gps_events[i].callback != NULL) {
                    s_gps_events[i].expire_msec -= left_mesc;
                    //LOGD("%s callback=%p, left_mesc=%ld", __func__, s_gps_events[i].callback, s_gps_events[i].expire_msec);
                }
            }
        }
        
        if (ret > 0 && FD_ISSET(fd, &rfds)) {
            GPS_EVENT gps_event;
            
            if (read(fd, &gps_event, sizeof(gps_event)) == sizeof(gps_event)) {
                for (i = 0; i < sizeof(s_gps_events)/sizeof(s_gps_events[0]); i++) {
                    if (gps_event.callback == s_gps_events[i].callback) {
                        if (gps_event.expire_msec < s_gps_events[i].expire_msec)
                            s_gps_events[i].expire_msec = gps_event.expire_msec;
                        break;
                    }
                }

                if (i == sizeof(s_gps_events)/sizeof(s_gps_events[0])) {
                    for (i = 0; i < sizeof(s_gps_events)/sizeof(s_gps_events[0]); i++) {
                        if (0 == s_gps_events[i].callback) {
                            s_gps_events[i] = gps_event;
                            break;
                        }
                    }
                }
            }
        }
    } while (1);
        
    LOGD("%s exit", __func__);
    return NULL;
}

void ql_gps_init(const char *module_version) {
    pthread_t pthread_id;

    if (ql_gps_control[0] == 0) {
        socketpair( AF_LOCAL, SOCK_STREAM, 0, ql_gps_control);
        pthread_create(&pthread_id, NULL, GpsControlLoop, NULL);     
    }
    
    ql_is_mc20_gps = !strncmp(module_version, "MC", 2);

    if (ql_gps_recv_cmd_control[0] == 0) {
        char d[32];
        socketpair( AF_LOCAL, SOCK_STREAM, 0, ql_gps_recv_cmd_control);
        snprintf(d, sizeof(d), "c:%d", ql_gps_recv_cmd_control[1]);
        ql_tty2tcp("s:rild-gps", d, 512*1024);
        pthread_create(&pthread_id, NULL, ql_gps_recv_cmd_from_gps_hal, NULL);     
    }
    
    if (ql_is_mc20_gps && ql_gps_send_nmea_control[0] == 0) {
        char d[32];
        socketpair( AF_LOCAL, SOCK_STREAM, 0, ql_gps_send_nmea_control);
        snprintf(d, sizeof(d), "c:%d", ql_gps_send_nmea_control[1]);
        ql_tty2tcp("s:rild-nmea", d, 4*1024);
        pthread_create(&pthread_id, NULL, ql_gps_send_nmea_to_gps_hal, NULL);     
    }
    
    if (s_gps_state) {
        if (ql_is_mc20_gps) {
            at_send_command("AT+QGNSSC=1", NULL);
            GPS_requestTimedCallback (pollNMEAReader, NULL, 1000);
        } else {
            at_send_command("AT+QGPS=1", NULL);
        }
    }
}
