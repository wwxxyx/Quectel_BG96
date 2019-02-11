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
#include <linux/sockios.h>
#include "../include/telephony/ril.h"

#define LOG_NDEBUG 0
#define LOG_TAG "DTMF"
#include "ql-log.h"

static int ql_dtmf_control[2];
static pthread_t s_dtmf_thread;
extern int ql_tty2tcp(const char *s, const char *d, int fifo_size);

void ql_dtmf_send_to_apk(unsigned char dtmf) {
    if (ql_dtmf_control[0] > 0) {
        write(ql_dtmf_control[0], &dtmf, 1);
        LOGD("%s( dtmf=%c )", __func__, dtmf);
    }
}

static void * ql_dtmf_recv_from_apk(void *param) {
    if (ql_dtmf_control[0] > 0) {
        unsigned char dtmf;

        while (read(ql_dtmf_control[0], &dtmf, 1) > 0) {
            LOGD("%s( dtmf=%c )", __func__, dtmf);           
        }
    }
    LOGD("%s exit errno: %d (%s)", __func__, errno, strerror(errno));
    return NULL;
}

void ql_dtmf_init(void) {
    if (ql_dtmf_control[0] == 0) {
        char d[32];
        socketpair( AF_LOCAL, SOCK_STREAM, 0, ql_dtmf_control);
        snprintf(d, sizeof(d), "c:%d", ql_dtmf_control[1]);
        ql_tty2tcp("s:rild-dtmf", d, 64);
        pthread_create(&s_dtmf_thread, NULL, ql_dtmf_recv_from_apk, NULL);     
    }
}
