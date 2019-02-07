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
#include "ql-usb.h"

#define LOG_NDEBUG 0
#define LOG_TAG "RILU"
#include "ql-log.h"

#define USBID_LEN 4
#define MAX_PATH 256

struct ql_module_info {
    const char idVendor[USBID_LEN+1];
    const char idProduct[USBID_LEN+1];
    char dm_inf;
    char gps_inf;
    char at_inf;
    char ppp_inf;
    char ndis_inf;
    const char *driver;
};

struct ql_usb_device_info {
    const struct ql_module_info *module_info;
    char usbdevice_pah[MAX_PATH];
    char ttyDM[16];
    char ttyGPS[16];
    char ttyAT[16];
    char ttyPPP[16];
    char ttyNDIS[16];
};

static const struct ql_module_info ql_module_info_table[] = {
    {"1519", "0331", -1, -1, 6, 0, -1, "cdc-acm"}, //UG95
    {"1519", "0020", -1, -1, 6, 0, -1, "cdc-acm"}, //UG95
    {"05c6", "9003", -1, -1, 2, 3, 4, "option"}, //UC20
    {"05c6", "9090", -1, -1, 2, 3, -1, "option"}, //UC15
    {"05c6", "9215", 0, 1, 2, 3, 4, "option"}, //EC20
    {"2c7c", "0125", 0, 1, 2, 3, 4, "option"}, //EC25
    {"2c7c", "0121", 0, 1, 2, 3, 4, "option"}, //EC21
    {"2c7c", "0191", 0, 1, 2, 3, 4, "option"}, //EG91
    {"2c7c", "0195", 0, 1, 2, 3, 4, "option"}, //EG95
    {"2c7c", "0306", 0, 1, 2, 3, 4, "option"}, //EG06/EP06/EM06
    {"2c7c", "0296", 0, 1, 2, 3, 4, "option"}, //BG96
    {"2c7c", "0435", 0, 1, 2, 3, 4, "option"}, //AG35
};

static struct ql_usb_device_info s_usb_device_info[MAX_CARD_NUM];

static const struct ql_module_info * ql_get_module_info(char idVendor[USBID_LEN+1], char idProduct[USBID_LEN+1]) {
    size_t i;
#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))
    for (i = 0; i < ARRAY_SIZE(ql_module_info_table); i++) {
        if (!strncasecmp(ql_module_info_table[i].idVendor, idVendor, USBID_LEN) && !strncasecmp(ql_module_info_table[i].idProduct, idProduct, USBID_LEN)) 
            return &ql_module_info_table[i];
    }
    return NULL;
}
    
static int ql_find_module(struct ql_usb_device_info *pusb_device_info)
{ 
    DIR *pDir;
    int fd;
    char filename[MAX_PATH];
    int find_usb_device = 0;
    struct stat statbuf;
    struct dirent* ent = NULL;
    struct ql_usb_device_info usb_device_info;
    const char *dir = "/sys/bus/usb/devices";
    
    if ((pDir = opendir(dir)) == NULL)  {
        LOGE("Cannot open directory:%s/", dir);  
        return 0;
    }
    
    while ((ent = readdir(pDir)) != NULL)  {
        memset(&usb_device_info, 0, sizeof(usb_device_info));
        
        sprintf(filename, "%s/%s", dir, ent->d_name);
        lstat(filename, &statbuf);
        if (S_ISLNK(statbuf.st_mode))  {
            char idVendor[USBID_LEN+1] = {0};
            char idProduct[USBID_LEN+1] = {0};

            sprintf(filename, "%s/%s/idVendor", dir, ent->d_name);
            fd = open(filename, O_RDONLY);
            if (fd > 0) {
                read(fd, idVendor, USBID_LEN);
                close(fd);
            }
            
            sprintf(filename, "%s/%s/idProduct", dir, ent->d_name);
            fd = open(filename, O_RDONLY);
            if (fd > 0) {
                read(fd, idProduct, USBID_LEN);
                close(fd);
            }
            
            if (!ql_get_module_info(idVendor, idProduct))
                continue;

            if (find_usb_device > MAX_CARD_NUM){
                LOGE("usb device number is more than max number,please repair it");
                break;
            }

            snprintf(usb_device_info.usbdevice_pah, sizeof(usb_device_info.usbdevice_pah), "%s/%s", dir, ent->d_name);
            usb_device_info.module_info = ql_get_module_info(idVendor, idProduct);
            
            memcpy(&pusb_device_info[find_usb_device], &usb_device_info, sizeof(struct ql_usb_device_info));
            find_usb_device++;
            LOGD("find quectel module %s idVendor=%s idProduct=%s", usb_device_info.usbdevice_pah, idVendor, idProduct);
        }
    }
    
    closedir(pDir);
    return find_usb_device;
}

static int ql_find_ttyname(int usb_interface, const char usbdevice_pah[MAX_PATH], char out_ttyname[16])
{
    DIR *pDir;
    struct dirent* ent = NULL;
    char dir[MAX_PATH]={0};

    out_ttyname[0] = 0;
    if(usb_interface < 0) {
        return 0;
    }

    snprintf(dir, sizeof(dir), "%s:1.%d", usbdevice_pah, usb_interface);
    if ((pDir = opendir(dir)) == NULL) {
        LOGE("Cannot open directory:%s/", dir);              
        return 0;
    }
    
    while ((ent = readdir(pDir)) != NULL) {
        if (strncmp(ent->d_name, "tty", 3) == 0) {
            LOGD("find %s/%s", dir, ent->d_name);
            strcpy(out_ttyname, ent->d_name);
            break;
        }
    }
    closedir(pDir); 

    if (strcmp(out_ttyname, "tty") == 0) { //find tty not ttyUSBx or ttyACMx
        strcat(dir, "/tty");
        if ((pDir = opendir(dir)) == NULL)  {  
            LOGE("Cannot open directory:%s", dir);      
            return 0;
        }
    
        while ((ent = readdir(pDir)) != NULL)  {
            if (strncmp(ent->d_name, "tty", 3) == 0) {
                LOGD("find %s/%s", dir, ent->d_name);
                strcpy(out_ttyname, ent->d_name);
                break;
            } 
        }
        closedir(pDir); 
    }

    return out_ttyname[0];
}

static int ql_find_cdc_wdm(char *dir, char *usb_device_name)
{
    char subdir[MAX_PATH]={0};
    DIR *pDir, *pSubDir;
    struct dirent* ent = NULL;
    struct dirent* subent = NULL;
#define CDCWDM_UEVENT_LEN 256
#ifndef MKDEV
#define MKDEV(ma,mi) ((ma)<<8 | (mi))
#endif
    int fd_uevent = -1;
    char uevent_path[MAX_PATH] = {0};
    char cdc_nod[MAX_PATH] = {0};
    char uevent_buf[CDCWDM_UEVENT_LEN] = {0};
    char *pmajor = NULL;
    char *pminor = NULL;
    char *pcr = NULL;
    int cdc_major = 0;
    int cdc_minor = 0;
    struct stat st = {0};
    int need_newnod = 0;
    int find_qmichannel = 0;

    strcpy(subdir, dir);
    strncat(subdir, "/", strlen("/"));
    strncat(subdir, usb_device_name, strlen(usb_device_name));
    if ((pSubDir = opendir(subdir)) == NULL)  {  
        LOGE("Cannot open directory:%s/", subdir);
        return -ENODEV;
    }
    while ((subent = readdir(pSubDir)) != NULL) {
        if (strncmp(subent->d_name, "cdc-wdm", strlen("cdc-wdm")) == 0) {
            LOGD("find %s/%s", subdir, subent->d_name);
            LOGD("qmichannel = %s", subent->d_name);
            find_qmichannel = 1;
        #if 1
            snprintf(uevent_path, MAX_PATH, "%s/%s/%s", subdir, subent->d_name, "uevent");
            fd_uevent = open(uevent_path, O_RDONLY);
            if (fd_uevent < 0) {
                LOGE("Cannot open file:%s, errno = %d(%s)", uevent_path, errno, strerror(errno));
            } else {
                snprintf(cdc_nod, MAX_PATH, "/dev/%s", subent->d_name);
                read(fd_uevent, uevent_buf, CDCWDM_UEVENT_LEN);
                close(fd_uevent);
                pmajor = strstr(uevent_buf, "MAJOR");
                pminor = strstr(uevent_buf, "MINOR");
                if (pmajor && pminor) {
                    pmajor += sizeof("MAJOR");
                    pminor += sizeof("MINOR");
                    pcr = pmajor;
                    while (0 != strncmp(pcr++, "\n", 1));
                    *(pcr - 1) = 0;
                    pcr = pminor;
                    while (0 != strncmp(pcr++, "\n", 1));
                    *(pcr - 1) = 0;
                    cdc_major = atoi((const char *)pmajor);
                    cdc_minor = atoi((const char *)pminor);
                    if (0 == stat(cdc_nod, &st)) {
                        if (st.st_rdev != (unsigned)MKDEV(cdc_major, cdc_minor)) {
                            need_newnod = 1;
                            if (0 != remove(cdc_nod)) {
                                LOGE("remove %s failed. errno = %d(%s)", cdc_nod, errno, strerror(errno));
                            }
                        } else {
                            need_newnod = 0;
                        }
                    } else {
                        need_newnod = 1;
                    }
                    if ((1 == need_newnod) && (0 != mknod(cdc_nod, S_IRUSR | S_IWUSR | S_IFCHR, MKDEV(cdc_major, cdc_minor)))) {
                        LOGE("mknod for %s failed, MAJOR = %d, MINOR =%d, errno = %d(%s)", cdc_nod, cdc_major,
                            cdc_minor, errno, strerror(errno));
                    }
                } else {
                    LOGE("major or minor get failed, uevent_buf = %s", uevent_buf);
                }
            }
        #endif
            break;
        }                   
    }
    closedir(pSubDir);
    
    return find_qmichannel;
}

static int ql_find_qcqmi(char *dir)
{
    char subdir[MAX_PATH]={0};
    DIR *pSubDir;
    struct dirent* subent = NULL;
    int find_qmichannel = 0;

    strcpy(subdir, dir);
    strcat(subdir, "/GobiQMI");
    
    if ((pSubDir = opendir(subdir)) == NULL)  {    
        LOGE("Cannot open directory:%s/", subdir);
        return -ENODEV;
    }
    
    while ((subent = readdir(pSubDir)) != NULL) {
       if (strncmp(subent->d_name, "qcqmi", strlen("qcqmi")) == 0) {
            LOGD("Find %s/%s", subdir, subent->d_name);
            LOGD("qmichannel = %s", subent->d_name);
            find_qmichannel = 1;
            break;
       }                         
    }
    
    closedir(pSubDir);
    return find_qmichannel;
}

static char* ql_find_usbnet_adapter(char *dir)
{
    char subdir[MAX_PATH]={0};
    DIR *pSubDir;
    struct dirent* subent = NULL;

    strcpy(subdir, dir);
    strcat(subdir, "/net");
    if ((pSubDir = opendir(subdir)) == NULL)  {    
        LOGE("Cannot open directory:%s/", subdir);
        return NULL;
    }
    while ((subent = readdir(pSubDir)) != NULL) {
        if ((strncmp(subent->d_name, "wwan", strlen("wwan")) == 0)
            || (strncmp(subent->d_name, "eth", strlen("eth")) == 0)
            || (strncmp(subent->d_name, "usb", strlen("usb")) == 0)) {
            static char s_pp_usbnet_adapter[32]={0};
            strcpy(s_pp_usbnet_adapter, subent->d_name);
            LOGD("find %s/%s", subdir, subent->d_name);
            LOGD("usbnet_adapter = %s", s_pp_usbnet_adapter);
            closedir(pSubDir);
            return s_pp_usbnet_adapter;
        }
    }
    
    closedir(pSubDir);
    return NULL;
}

static int ql_find_ndis(int usb_interface, const char usbdevice_path[MAX_PATH], char out_ttyname[16])
{
    struct dirent* ent = NULL;  
    DIR *pDir;
    int find_qmichannel = 0;
    char dir[MAX_PATH] = {0};
    char *p_usbnet_adapter = NULL;

    snprintf(dir, sizeof(dir), "%s:1.%d", usbdevice_path, usb_interface);
    
    if ((pDir = opendir(dir)) == NULL) {  
        LOGE("Cannot open directory:%s/", dir);  
        return 0;  
    }
    
    while ((ent = readdir(pDir)) != NULL) {
        if (!strncmp(ent->d_name, "usb", strlen("usb"))) {
            find_qmichannel = ql_find_cdc_wdm(dir, ent->d_name);
        } else if (!strncmp(ent->d_name, "GobiQMI", strlen("GobiQMI"))) {
            find_qmichannel = ql_find_qcqmi(dir);
        } else if (!strncmp(ent->d_name, "net", strlen("net"))) {
            p_usbnet_adapter = ql_find_usbnet_adapter(dir);
        }

        if (find_qmichannel && p_usbnet_adapter)
        {
            strcpy(out_ttyname, p_usbnet_adapter);
            break;
        }
    }
    closedir(pDir);     

    return (find_qmichannel && p_usbnet_adapter) ? 1 : 0;
}

char * ql_get_ttyGPS(char *out_ttyname) {
    if (!s_usb_device_info[0].ttyGPS[0]) {
        LOGE("cannot find ttyname for GPS Port");
        return NULL;
    }

    strcpy(out_ttyname, s_usb_device_info[0].ttyGPS);
    return out_ttyname;
}

char * ql_get_ttyAT(char *out_ttyname) {
    if (!s_usb_device_info[0].ttyAT[0]) {
        LOGE("cannot find ttyname for AT Port");
        return NULL;
    }

    strcpy(out_ttyname, s_usb_device_info[0].ttyAT);
    return out_ttyname;
}

char * ql_get_ttyPPP(char *out_ttyname) {
    if (!s_usb_device_info[0].ttyPPP[0]) {
        LOGE("cannot find ttyname for PPP Port");
        return NULL;
    }
  
    strcpy(out_ttyname, s_usb_device_info[0].ttyPPP);
    return out_ttyname;
}

char * ql_get_ndisname(char *out_ttyname, int i) {
    if (!s_usb_device_info[i].ttyNDIS[0]) {
        //LOGE("cannot find ttyname for NDIS Port");
        return NULL;
    }
  
    strcpy(out_ttyname, s_usb_device_info[0].ttyNDIS);
    return out_ttyname;
}

int ql_is_quectel_ttyport(const char*in_ttyname) {
    if (s_usb_device_info[0].ttyDM[0] && strstr(in_ttyname, s_usb_device_info[0].ttyDM)) {
        return 1;
    }
    if (s_usb_device_info[0].ttyGPS[0] && strstr(in_ttyname, s_usb_device_info[0].ttyGPS)) {
        return 1;
    }    
    if (s_usb_device_info[0].ttyAT[0] && strstr(in_ttyname, s_usb_device_info[0].ttyAT)) {
        return 1;
    }
    if (s_usb_device_info[0].ttyPPP[0] && strstr(in_ttyname, s_usb_device_info[0].ttyPPP)) {
        return 1;
    }
    return 0;
}

int ql_detect_quectel_modules(void) 
{
    int find_usb_device = 0;
    int i = 0;    
    struct ql_usb_device_info usb_device_info[MAX_CARD_NUM];
    
    memset(usb_device_info, 0, sizeof(usb_device_info));

    find_usb_device = ql_find_module(usb_device_info);
    
    if(!find_usb_device) {
        memcpy(s_usb_device_info, usb_device_info, sizeof(usb_device_info));
        return 0;
    }

    sleep(1); //wait usb driver load

    for(i = 0; i < find_usb_device; i++) {
        struct ql_module_info *info = (struct ql_module_info *)usb_device_info[i].module_info;
        
        if (ql_find_ttyname(info->at_inf, usb_device_info[i].usbdevice_pah, usb_device_info[i].ttyAT)) {
            LOGD("ttyAT = %s", usb_device_info[i].ttyAT);
        } else {
            if (!strcmp(info->driver, "option")) {
                if (access("/sys/bus/usb-serial/drivers/option1/new_id", W_OK) == 0) {
                    char *cmd;
                    LOGE("find usb serial option driver, but donot cantain quectel vid&pid");
                    asprintf(&cmd, "echo %s %s > /sys/bus/usb-serial/drivers/option1/new_id", info->idVendor, info->idProduct);
                    system(cmd);
                    free(cmd);
                    sleep(1); //wait usb driver load
                    return 0;
                } else {
                    LOGE("can not find usb serial option driver");
                }
            }
        }

        if (info->ppp_inf > 0 && ql_find_ttyname(info->ppp_inf, usb_device_info[i].usbdevice_pah, usb_device_info[i].ttyPPP)) {
            LOGD("ttyPPP = %s", usb_device_info[i].ttyPPP);
        }
        
        if (info->dm_inf >= 0 && ql_find_ttyname(info->dm_inf, usb_device_info[i].usbdevice_pah, usb_device_info[i].ttyDM)) {
            LOGD("ttyDM = %s", usb_device_info[i].ttyDM);
        }

        if (info->gps_inf > 0 && ql_find_ttyname(info->gps_inf, usb_device_info[i].usbdevice_pah, usb_device_info[i].ttyGPS)) {
            LOGD("ttyGPS = %s", usb_device_info[i].ttyGPS);
        }

        if (info->ndis_inf > 0 && ql_find_ndis(info->ndis_inf, usb_device_info[i].usbdevice_pah, usb_device_info[i].ttyNDIS)) {
            LOGD("ttyNDIS = %s", usb_device_info[i].ttyNDIS);
        }
    }

    memcpy(s_usb_device_info, usb_device_info, sizeof(usb_device_info));

    return find_usb_device;
}

void ql_set_autosuspend(int enable) {
    int i = 0;

    do {
        if (s_usb_device_info[i].usbdevice_pah[0]) {
            char shell_command[MAX_PATH+32];
            snprintf(shell_command, sizeof(shell_command), "echo %s > %s/power/control", enable ? "auto" : "on", s_usb_device_info[i].usbdevice_pah);
            system(shell_command);
            LOGD("%s", shell_command);
            LOGD("%s %s", __func__, enable ? "auto" : "off");
        }

        i++;
    } while(i < MAX_CARD_NUM);
}
