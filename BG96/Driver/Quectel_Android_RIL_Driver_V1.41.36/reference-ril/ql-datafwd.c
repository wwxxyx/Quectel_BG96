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
#define LOG_TAG "DATAFWD"
#include "ql-log.h"

static int ql_datafwd_control[2];
static pthread_t s_datafwd_thread;
extern int ql_tty2tcp(const char *s, const char *d, int fifo_size);

void ql_datafwd_send_to_apk(unsigned char *hex_data, int length) {
    if (ql_datafwd_control[0] > 0) {
        ssize_t n = write(ql_datafwd_control[0], hex_data, length);
        if (n != length)
            LOGE("%s( datafwd=%zd/%d )", __func__, n, length);
    }
}

static void * ql_datafwd_recv_from_apk(void *param) {
    if (ql_datafwd_control[0] > 0) {
        #define max_hex_data ((1024-64)/2)
        unsigned char *hex_data = (unsigned char *)malloc(max_hex_data);
        unsigned char *hex_string = (unsigned char *)malloc(max_hex_data*2 + 1);
        char *cmd = (char *)malloc(max_hex_data*2 + 128);
        ssize_t n;

        while ((n = read(ql_datafwd_control[0], hex_data, max_hex_data)) > 0) {
            int src_port = 1; //0:uart1 1:usbat 2:usbmodem
            int dest_port = 0; //0:uart1 1:usbat 2:usbmodem 3:WIFI client
            const char hex_map[] = "0123456789ABCDEF";
            ssize_t i;

            for (i = 0; i < n; i++) {
                hex_string[i*2+0] = hex_map[hex_data[i]>>4];
                hex_string[i*2+1] = hex_map[hex_data[i]&0xf];
            }
            hex_string[i*2+0] = 0;

            snprintf(cmd, (max_hex_data*2 + 128), "AT+QDATAFWD=%d,%d,%zd,\"%s\",1", src_port, dest_port, n*2, hex_string);
            at_send_command(cmd, NULL);
        }
    }
    LOGD("%s exit errno: %d (%s)", __func__, errno, strerror(errno));
    return NULL;
}

void ql_datafwd_init(void) {
    if (ql_datafwd_control[0] == 0) {
        char d[32];
        socketpair( AF_LOCAL, SOCK_STREAM, 0, ql_datafwd_control);
        snprintf(d, sizeof(d), "c:%d", ql_datafwd_control[1]);
        ql_tty2tcp("s:rild-fwd", d, 1024*1024);
        pthread_create(&s_datafwd_thread, NULL, ql_datafwd_recv_from_apk, NULL);     
    }
}
