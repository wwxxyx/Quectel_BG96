#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/poll.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <termios.h>
#include <pthread.h>
#include <sys/utsname.h>
#include <linux/kdev_t.h>
#define LOG_NDEBUG 0
#define LOG_TAG "NDIS"
#include "ql-log.h"
#include "ql-usb.h"

int quectel_CM(int argc, char *argv[]);
int notifyDataCallProcessExit(void);
//#define MAX_PATH 256

static pid_t ql_ndis_pid_arr[MAX_CARD_NUM] = {0};

static int ql_ndis_quit = 0;
static pthread_t ql_ndis_thread_arr[MAX_CARD_NUM] = {0};

static int card_cnt = 0;

struct ndis_arg_info {
    int usbnet_adapter_index;
    char usbnet_adapter[16];
};


static int ndis_create_thread(pthread_t * thread_id, void * thread_function, void * thread_function_arg ) {
    static pthread_attr_t thread_attr;
    pthread_attr_init(&thread_attr);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
    if (pthread_create(thread_id, &thread_attr, thread_function, thread_function_arg)!=0) {
        LOGE("%s %s errno: %d (%s)", __FILE__, __func__, errno, strerror(errno));
        return 1;
    }
    pthread_attr_destroy(&thread_attr); /* Not strictly necessary */
    return 0; //thread created successfully
}

static void ql_sleep(int sec) {
    int msec = sec * 1000;
    while (!ql_ndis_quit && (msec > 0)) {
        msec -= 200;
        usleep(200*1000);
    }
}

static char s_ndis_apn[128];
static char s_ndis_user[128];
static char s_ndis_password[128];
static char s_ndis_auth_type[2];
static int s_ndis_default_pdp;
static void* ndis_thread_function(void*  arg) {
    const char *argv[20]={NULL};
    int argc = 0;
    const char *pdpv[10] = {"0", "1", "2", "3", "4", "5", "6", "7", "8"};

    LOGD("%s %s/%s/%s/%s enter", __func__, s_ndis_apn, s_ndis_user, s_ndis_password, s_ndis_auth_type);
        
    //LOGD("apn = %s", apn);
    //LOGD("user = %s", user);
    //LOGD("password = %s", password);
    //LOGD("auth_type = %s", auth_type);

    struct ndis_arg_info* arg_info = (struct ndis_arg_info*)arg;

    argv[argc++] = "quectel-CM";
    //argv[argc++] = "-v";
    argv[argc++] = "-s";
    if (s_ndis_apn[0])
        argv[argc++] = s_ndis_apn;
    if (s_ndis_user[0])
        argv[argc++] = s_ndis_user;
    if (s_ndis_user[0] && s_ndis_password[0])
        argv[argc++] = s_ndis_password;
    if (s_ndis_user[0] && s_ndis_password[0] && s_ndis_auth_type[0])
        argv[argc++] = s_ndis_auth_type;
    argv[argc++] = "-n";
    argv[argc++] = pdpv[s_ndis_default_pdp];
    argv[argc++] = "-i";
    argv[argc++] = arg_info->usbnet_adapter;
    //argv[argc++] = "-v";
    argv[argc] = NULL;
	int index = 0;

    while (!ql_ndis_quit) {
        int child_pid = fork();
        if (child_pid == 0) {
            exit(quectel_CM(argc, (char**) argv));
        } else if (child_pid < 0) {
            LOGE("failed to start ('%s'): %s\n", "quectel-CM", strerror(errno));
            break;
        } else {
            int status, retval = 0;
            ql_ndis_pid_arr[arg_info->usbnet_adapter_index] = child_pid;
            waitpid(child_pid, &status, 0);
            ql_ndis_pid_arr[arg_info->usbnet_adapter_index] = 0;
            if (WIFSIGNALED(status)) {
                retval = WTERMSIG(status);
                LOGD("*** %s: Killed by signal %d retval = %d\n", "quectel-CM", WTERMSIG(status), retval);
            } else if (WIFEXITED(status) && WEXITSTATUS(status) > 0) {
                retval = WEXITSTATUS(status);
                LOGD("*** %s: Exit code %d retval = %d\n", "quectel-CM", WEXITSTATUS(status), retval);
            }
            if (notifyDataCallProcessExit() || ql_ndis_quit)
                break;
            else
                ql_sleep(3);
        }
    }

    ql_ndis_thread_arr[arg_info->usbnet_adapter_index] = 0;

    LOGD("%s exit", __func__);
    pthread_exit(NULL);
    return NULL;     

}

int ql_ndis_stop(int signo);
int ql_ndis_start(const char *apn, const char *user, const char *password, const char *auth_type, int default_pdp) {    
    static char *argv[4] = {NULL, NULL, NULL, NULL};
    ql_ndis_stop(SIGKILL);
    int pid = -1;
    //LOGD("apn = %s", apn);
    //LOGD("user = %s", user);
    //LOGD("password = %s", password);
    //LOGD("auth_type = %s", auth_type);

    s_ndis_apn[0] = s_ndis_user[0] = s_ndis_password[0] = s_ndis_auth_type[0] = '\0';
    if (apn != NULL) strncpy(s_ndis_apn, apn, sizeof(s_ndis_apn) - 1);
    if (user != NULL) strncpy(s_ndis_user, user, sizeof(s_ndis_user) - 1);
    if (password != NULL) strncpy(s_ndis_password, password, sizeof(s_ndis_password) - 1);
    if (auth_type != NULL) strncpy(s_ndis_auth_type, auth_type, sizeof(s_ndis_auth_type) - 1);
    s_ndis_default_pdp = default_pdp;

    ql_ndis_quit = 0;

    static struct ndis_arg_info ndis_arg[MAX_CARD_NUM];
    memset(ndis_arg, 0, sizeof(ndis_arg));

    for(card_cnt = 0; card_cnt < MAX_CARD_NUM; card_cnt++) {   

        if (!ql_get_ndisname(ndis_arg[card_cnt].usbnet_adapter, card_cnt))
            break;
        
        ndis_arg[card_cnt].usbnet_adapter_index = card_cnt;
        
        if (!ndis_create_thread(&ql_ndis_thread_arr[card_cnt], ndis_thread_function, (void*)(&ndis_arg[card_cnt]))) {
            pid = getpid();
        }
    }

    return pid;
}

int ql_ndis_stop(int signo) {
    unsigned int kill_time = 15000;
    int i = 0;
    int cnt = 0;
    if (signo == SIGUSR2) {
        //used to check data call state
        for (i = 0; i < card_cnt; i++) {
            if (ql_ndis_pid_arr[i] != 0) {
                kill(ql_ndis_pid_arr[i], signo);
            }
        }
        return 0;
    }

    ql_ndis_quit = 1;

    for (i = 0; i < card_cnt; i++) {
        if (ql_ndis_pid_arr[i] == 0 && ql_ndis_thread_arr[i] == 0){
            cnt++;
        }
    }

    if(cnt == card_cnt) {
        return 0;
    }
    
    for (i = 0; i < card_cnt; i++) {
        if (ql_ndis_pid_arr[i] != 0) {
            if (fork() == 0) {//kill may take long time, so do it in child process
                int kill_time = 10;
                kill(ql_ndis_pid_arr[i], signo);
                while(kill_time-- && !kill(ql_ndis_pid_arr[i], 0)) //wait pppd quit
                sleep(1);
                if (signo != SIGKILL && !kill(ql_ndis_pid_arr[i], 0))
                kill(ql_ndis_pid_arr[i], signo);
                exit(0);
            }
        }
        cnt++;
    }

    for (i = 0; i < card_cnt; i++) {
        do {
            usleep(100*1000);
            kill_time -= 100;
        } while ((kill_time > 0) && (ql_ndis_pid_arr[i] != 0 || ql_ndis_thread_arr[i] != 0));
    }
    LOGD("%s cost %d msec", __func__, (15000 - kill_time));
    return 0;
}
