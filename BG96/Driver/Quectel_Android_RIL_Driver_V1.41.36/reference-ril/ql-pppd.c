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
#include "ql-usb.h"

#define LOG_NDEBUG 0
#define LOG_TAG "PPPD"
#include "ql-log.h"

int notifyDataCallProcessExit(void);
extern char *g_ppp_number;
extern int ql_mux_enabled;

#define MAX_PATH 256

static int ql_is_digit(const char *pdigit)
{
    const char *pcur = pdigit;
    if (NULL == pcur) {
        return 0;
    }
    for (pcur = pdigit; (NULL != pcur) && (0 != *pcur); pcur++) {
        if ((*pcur < '0') || (*pcur > '9')) {
            return 0;
        }
    }
    return 1;
}

static int is_chat_alive(void)
{
#define BIN_CHAT "/system/bin/chat"
    int ret = 0;
    int nmsz = 0;
    DIR *pDir = NULL;
    struct dirent *ent = NULL;
    char dir[MAX_PATH] = {0};
    char filename[MAX_PATH] = {0};
    char link_path[MAX_PATH] = {0};

    strcat(dir, "/proc");

    // modified by marco
    // On selinux env, this judgement may scan all processes
    // so we close it
    if (access("/sys/fs/selinux", F_OK) == 0) {
        return ret;
    }

    if ((pDir = opendir(dir)) == NULL) {
        LOGE("canot open directory : %s, errno=%d(%s)\n", dir, errno, strerror(errno));
        ret = 0;
        goto out;
    }

    while ((ent = readdir(pDir)) != NULL) {
        if (0 == ql_is_digit((const char *)(ent->d_name))) {
            continue;
        }
        sprintf(filename, "%s/%s/exe", dir, ent->d_name);
        memset(link_path, 0, sizeof(link_path));
        nmsz = readlink(filename, link_path, MAX_PATH);
        if (nmsz <= 0) {
            continue;
        }
        link_path[nmsz] = 0;
        if (!strncmp(link_path, BIN_CHAT, strlen(BIN_CHAT))) {
            ret = 1;
            goto out;
        }
    }
out:
    if(pDir) {
        closedir(pDir);
    }
    return ret;
}

static int chat(int fd, const char *at, const char *expect, int timeout, char **response) {
    int ret;
    static char buf[128];

    if (response)
        *response = NULL;

    tcflush(fd, TCIOFLUSH);
    LOGD("chat --> %s", at);
    do {
        ret = write(fd, at, strlen(at));
    } while (ret < 0 && errno == EINTR);
    
    if (ret <= 0) {
        LOGD("chat write error on stdout: %s(%d) ", strerror(errno), errno);
        return errno ? errno : EINVAL;
    }

    while(timeout > 0) {
        struct pollfd poll_fd = {fd, POLLIN, 0};
        if(poll(&poll_fd, 1, 200) <= 0) {
            if (errno == ETIMEDOUT) {
                timeout -= 200;
                continue;
            } else if(errno != EINTR) {
                LOGE("chat poll error on stdin: %s(%d) ", strerror(errno), errno);
                return errno ? errno : EINVAL;
            }
        }
        
        if(poll_fd.revents && (poll_fd.revents & POLLIN)) {
            memset(buf, 0, sizeof(buf));
            usleep(100*1000);
            if(read(fd, buf, sizeof(buf)-1) <= 0) {
                LOGD("chat read error on stdin: %s(%d) ", strerror(errno), errno);
                return errno ? errno : EINVAL;
            }
            LOGD("chat %zd <-- %s", strlen(buf), buf);
            if(strstr(buf, expect)) {
                if (response)
                    *response = strstr(buf, expect);
                return 0;
            }
        }    
    }

    return errno ? errno : EINVAL;
}

#if 0
static pid_t ql_get_pid(const char *pname) {
    DIR *pDir;  
    struct dirent* ent = NULL;
    pid_t pid = 0;
    char *linkname = (char *) malloc (MAX_PATH + MAX_PATH);
    char *filename = linkname + MAX_PATH;
    int filenamesize;

    if (!linkname)
        return 0;

    pDir = opendir("/proc");
    if (pDir == NULL)  {  
        LOGE("Cannot open directory: /proc, errno: %d (%s)", errno, strerror(errno));  
        return 0;  
    }  

    while ((ent = readdir(pDir)) != NULL)  {
        int i = 0;
        //LOGD("%s", ent->d_name);
        while (ent->d_name[i]) {
            if ((ent->d_name[i] < '0')  || (ent->d_name[i] > '9'))
                break;
            i++;
         }

        if (ent->d_name[i]) {
            //LOGD("%s not digit", ent->d_name);           
            continue;
        }

        sprintf(linkname, "/proc/%s/exe", ent->d_name);  
        filenamesize = readlink(linkname, filename, MAX_PATH-1);
        if (filenamesize > 0) {
            filename[filenamesize] = 0;
            if (!strcmp(filename, pname)) {
                pid = atoi(ent->d_name);
                LOGD("%s -> %s", linkname, filename);
            }
        } else {
            //LOGD("readlink errno: %d (%s)", errno, strerror(errno));
        }
    }
    closedir(pDir);
    free(linkname);

    return pid;
}
#endif

static pid_t ql_pppd_pid = 0;
static int ql_pppd_quit = 0;
static pthread_t ql_pppd_thread;
static int pppd_create_thread(pthread_t * thread_id, void * thread_function, void * thread_function_arg ) {
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
    while (!ql_pppd_quit && (msec > 0)) {
        msec -= 200;
        usleep(200*1000);
    }
}

static char s_ppp_modemport[32];
static char s_ppp_user[128];
static char s_ppp_password[128];
static char s_ppp_auth_type[2];
static char s_ppp_number[32];

static void* pppd_thread_function(void*  arg) {
    char **argvv = (char **)arg;

    LOGD("%s %s/%s/%s/%s/%s enter", __func__, s_ppp_modemport, s_ppp_user, s_ppp_password, s_ppp_auth_type, s_ppp_number);
        
    //LOGD("modemport = %s", modemport);
    //LOGD("user = %s", user);
    //LOGD("password = %s", password);
    //LOGD("auth_type = %s", auth_type);
      
    while (!ql_pppd_quit) {
        char ttyname[10];
        char serialdevname[32];
        pid_t child_pid;
        int modem_fd, fdflags;
        struct termios  ios;
        int modembits = TIOCM_DTR;
        char *response;
        
        if (!s_ppp_modemport[0] && !ql_get_ttyPPP(ttyname)) {
            ql_sleep(3);
            continue;
        }
    
        if (!s_ppp_modemport[0]) {
            strcpy(serialdevname, "/dev/");
            strcat(serialdevname, ttyname);
        } else {
            strcpy(serialdevname, s_ppp_modemport);
        }

        //make sure modem is not in data mode!
        modem_fd = open (serialdevname, O_RDWR | O_NONBLOCK);
        if (modem_fd == -1) {
            LOGE("failed to open %s  errno: %d (%s)\n",  serialdevname, errno, strerror(errno));
            ql_sleep(3);
            continue;
        }
        
        fdflags = fcntl(modem_fd, F_GETFL);
        if (fdflags != -1)
            fcntl(modem_fd, F_SETFL, fdflags | O_NONBLOCK);
        /* disable echo on serial ports */
        tcgetattr( modem_fd, &ios );
        cfmakeraw(&ios);
        ios.c_lflag = 0;  /* disable ECHO, ICANON, etc... */
        cfsetispeed(&ios, B115200);
        cfsetospeed(&ios, B115200);
        tcsetattr( modem_fd, TCSANOW, &ios );

        ioctl(modem_fd, (0 ? TIOCMBIS: TIOCMBIC), &modembits); //clear DTR
        if (chat(modem_fd, "AT\r", "OK", 1000, NULL)) {
            if (ql_mux_enabled) {
                close(modem_fd);
                ql_sleep(3);
            } else {
                ioctl(modem_fd, (1 ? TIOCMBIS: TIOCMBIC), &modembits);
                ql_sleep(1);
                ioctl(modem_fd, (0 ? TIOCMBIS: TIOCMBIC), &modembits);
                ql_sleep(1);
                close(modem_fd);
            }
            if (notifyDataCallProcessExit() || ql_pppd_quit)
                break;
            else
                continue;
        }  

        chat(modem_fd, "AT+CGREG?\r", "+CGREG: ", 1000, &response);
        if (response)
            response = strstr(response, ",");
        if (!response || ((response[1] != '1') && (response[1] != '5'))) {
            close(modem_fd);
            ql_sleep(3);
            continue;
        }
        
        if (ql_mux_enabled) {
            //close(modem_fd);
            //sleep(1);
        } else {
            close(modem_fd);  
        }

        child_pid = fork();
        if (0 == child_pid) { //this is the child_process
            int argc = 0;
            const char *argv[40] = {"pppd", "115200", "nodetach", "nolock", "debug", "dump", "nocrtscts", "modem", "hide-password", 
                "usepeerdns", "noipdefault", "novj", "novjccomp", "noccp", "defaultroute", "ipcp-accept-local", "ipcp-accept-remote", "ipcp-max-failure", "30",
                //"connect", "/etc/ppp/init.quectel-pppd chat connect",
                //"disconnect","/etc/ppp/init.quectel-pppd chat disconnect",
                NULL
            };
            char *ppp_dial_number = NULL;
    
            while (argv[argc]) argc++;
            argv[argc++] = serialdevname;
            if (s_ppp_user[0]) {
                argv[argc++] = "user";
                argv[argc++] = s_ppp_user;
            }
            if (s_ppp_user[0] && s_ppp_password[0]) {
                argv[argc++] = "password";
                argv[argc++] = s_ppp_password;   
            }
            if (s_ppp_user[0] && s_ppp_password[0] && s_ppp_auth_type[0]) {        
                if (s_ppp_auth_type[0] == '0') { //  0 => PAP and CHAP is never performed.
                    argv[argc++] = "refuse-pap";
                    argv[argc++] = "refuse-chap";
                } else if (s_ppp_auth_type[0] == '1') { //  1 => PAP may be performed; CHAP is never performed.
                    argv[argc++] = "refuse-chap";
                } else if (s_ppp_auth_type[0] == '2') { //  2 => CHAP may be performed; PAP is never performed.
                    argv[argc++] = "refuse-pap";
                } else if (s_ppp_auth_type[0] == '3') { //  3 => PAP / CHAP may be performed - baseband dependent.
                }
                argv[argc++] = "refuse-eap";
                argv[argc++] = "refuse-mschap";
                argv[argc++] = "refuse-mschap-v2";
            }

            asprintf(&ppp_dial_number,
                "''/system/bin/chat -s -v ABORT BUSY ABORT \"NO CARRIER\" ABORT \"NO DIALTONE\" ABORT ERROR ABORT \"NO ANSWER\" TIMEOUT 40 \"\" ATD%s CONNECT''", s_ppp_number);
            argv[argc++] = "connect";
            argv[argc++] = ppp_dial_number;
            argv[argc] = NULL;   

            if (execv("/system/bin/pppd", (char**) argv)) {
                LOGE("cannot execve('%s'): %s\n", argv[0], strerror(errno));
                free(ppp_dial_number);
                exit(errno);
            }
            free(ppp_dial_number);
            exit(0);
        } else if (child_pid < 0) {
            LOGE("failed to start ('%s'): %s\n", "pppd", strerror(errno));
            break;
        } else {
            int status, retval = 0;
            ql_pppd_pid = child_pid;
            waitpid(child_pid, &status, 0);
            ql_pppd_pid = 0;
            if (ql_mux_enabled)
                close(modem_fd);
            if (WIFSIGNALED(status)) {
                retval = WTERMSIG(status);
                LOGD("*** %s: Killed by signal %d retval = %d\n", "pppd", WTERMSIG(status), retval);
            } else if (WIFEXITED(status) && WEXITSTATUS(status) > 0) {
                //see external/ppp/pppd/pppd.h
                const char *exitCodes[] = {
                    "EXIT_OK", //            0
                    "EXIT_FATAL_ERROR", //    1
                    "EXIT_OPTION_ERROR", //    2
                    "EXIT_NOT_ROOT", //        3
                    "EXIT_NO_KERNEL_SUPPORT", //    4
                    "EXIT_USER_REQUEST", //    5
                    "EXIT_LOCK_FAILED", //    6
                    "EXIT_OPEN_FAILED", //    7
                    "EXIT_CONNECT_FAILED", //    8
                    "EXIT_PTYCMD_FAILED    ", //9
                    "EXIT_NEGOTIATION_FAILED", //    10
                    "EXIT_PEER_AUTH_FAILED", //    11
                    "EXIT_IDLE_TIMEOUT", //    12
                    "EXIT_CONNECT_TIME", //    13
                    "EXIT_CALLBACK", //        14
                    "EXIT_PEER_DEAD", //        15
                    "EXIT_HANGUP", //        16
                    "EXIT_LOOPBACK", //        17
                    "EXIT_INIT_FAILED", //    18
                    "EXIT_AUTH_TOPEER_FAILED", //    19
                    "EXIT_TRAFFIC_LIMIT", //    20
                    "EXIT_CNID_AUTH_FAILED", //    21
                };
                retval = WEXITSTATUS(status);
                LOGD("*** %s: Exit code %d (%s) retval = %d\n", "pppd", WEXITSTATUS(status),
                    ((unsigned int)retval < (sizeof(exitCodes)/sizeof(exitCodes[0]))) ? exitCodes[retval] : "EXIT_UNKNOW_REASON", retval);
            }
            if (notifyDataCallProcessExit() || ql_pppd_quit)
                break;
            else
                ql_sleep(3);
        }
    }

    ql_pppd_thread = 0;
    LOGD("%s exit", __func__);
    pthread_exit(NULL);
    return NULL;         
}

int ql_pppd_stop(int signo);
int ql_pppd_start(const char *modemport, const char *user, const char *password, const char *auth_type, const char *ppp_number) {    
    ql_pppd_stop(SIGKILL);

    //LOGD("apn = %s", modemport);
    //LOGD("user = %s", user);
    //LOGD("password = %s", password);
    //LOGD("auth_type = %s", auth_type);

    s_ppp_modemport[0] = s_ppp_user[0] = s_ppp_password[0] = s_ppp_auth_type[0] = s_ppp_number[0] = '\0';
    if (modemport != NULL) strncpy(s_ppp_modemport, modemport, sizeof(s_ppp_modemport) - 1);
    if (user != NULL) strncpy(s_ppp_user, user, sizeof(s_ppp_user) - 1);
    if (password != NULL) strncpy(s_ppp_password, password, sizeof(s_ppp_password) - 1);
    if (auth_type != NULL) strncpy(s_ppp_auth_type, auth_type, sizeof(s_ppp_auth_type) - 1);
    if (ppp_number != NULL) strncpy(s_ppp_number, ppp_number, sizeof(s_ppp_number) - 1);

    if (access("/system/bin/pppd", X_OK)) {
        LOGE("/system/bin/pppd do not exist or is not Execute!");
        return (-ENOENT);
    }        
    if (access("/system/bin/chat", X_OK)) {
        LOGE("/system/bin/chat do not exist or is not Execute!");
        return (-ENOENT);
    }
    if (access("/etc/ppp/ip-up", X_OK)) {
        LOGE("/etc/ppp/ip-up do not exist or is not Execute!");
        return (-ENOENT);
    }
    
    ql_pppd_quit = 0;
    if (!pppd_create_thread(&ql_pppd_thread, pppd_thread_function, NULL))
        return getpid();
    else
        return -1;
}

int ql_pppd_stop(int signo) 
{
    unsigned int kill_time = 15000;
    int retry = 0;
    ql_pppd_quit = 1;

    if (ql_pppd_pid == 0 && ql_pppd_thread == 0)
        return 0;
#if 1 //wait for chat over, or pppd will kill all progresses in our progresses group
    for (retry = 0; retry < 50; retry++) {
        if (0 == is_chat_alive()) {
            break;
        }
        LOGD("wait for chat over!!!\n");
        sleep(1);
    }
#endif

    if (ql_pppd_pid != 0) {
        if (fork() == 0) {//kill may take long time, so do it in child process
            int kill_time = 10;
            kill(ql_pppd_pid, signo);
            while(kill_time-- && !kill(ql_pppd_pid, 0)) //wait pppd quit
                sleep(1);
            if (signo != SIGKILL && !kill(ql_pppd_pid, 0))
                kill(ql_pppd_pid, SIGKILL);
            exit(0);
        } 
    }

    do {
        usleep(100*1000);
        kill_time -= 100;
    } while ((kill_time > 0) && (ql_pppd_pid != 0 || ql_pppd_thread != 0));

    LOGD("%s cost %d msec", __func__, (15000 - kill_time));
    return 0;
}

