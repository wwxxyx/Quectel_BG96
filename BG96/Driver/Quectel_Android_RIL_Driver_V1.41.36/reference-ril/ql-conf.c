#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include "ql-conf.h"

#define LOG_NDEBUG 0
#define LOG_TAG "RIL_READ_CONF"
#include "ql-log.h"

char s_commamd_line[128][64] = {{0}};
int  s_commamd_line_cnt = 0;

static void ql_get_commamd_line(const char *line, const char *delim);
static char* ql_rm_pre_space(char *line);
static char* ql_rm_tail_space(char *line);
static void ql_at_cmds_for_customer_initialize(const char *line);
static void ql_icc_constants_init(const char *line);
static void ql_voice_over_usb_mode(char *line);

int read_conf_of_ril_init()
{
#define FORMATS_0X 16
#define FORMATS_D  10

    char *p = NULL;
    char *path = "/system/etc/ql-ril.conf";
    #define BUF_SIZE 1024
    char tmp_buf[BUF_SIZE] = {0};
    char buf[BUF_SIZE] = {0};

    if(access(path, F_OK))
    {
        LOGE("'%s' not exist.", path);
        return -1;
    }

    FILE * fp = fopen(path, "r");
    if(NULL == fp) {
        LOGE("open '%s' failure.", path);
        return -1;
    }

    while(NULL != fgets(tmp_buf, BUF_SIZE - 1, fp))
    {
        if(NULL != (p = strchr(tmp_buf, '#'))) *p = '\0';
        if(NULL != (p = strchr(tmp_buf, '\\'))) {
            *p = '\0';
            strcat(buf, tmp_buf);
            continue;
        }

        strcat(buf, tmp_buf);
        if(NULL != (p = strchr(buf, '\n'))) *p = '\0';
        if(NULL != (p = strchr(buf, '\r'))) *p = '\0';

        if(!(p = ql_rm_pre_space(buf))) goto clear;

        LOGD("original conf line: %s", p);

        if(strstr(p, "LTE_SignalStrength")) {
            s_ril_conf_init.LTE_SignalStrength= (int)strtol(strchr(p, '=') + 1, NULL, FORMATS_D);
            LOGD("LTE_SignalStrength = %d", s_ril_conf_init.LTE_SignalStrength);
        } else if(strstr(p, "LTE_Is_Report_SignalStrength")) {
            s_ril_conf_init.LTE_Is_Report_SignalStrength = (int)strtol(strchr(p, '=') + 1, NULL, FORMATS_D);
            LOGD("LTE_Is_Report_SignalStrength = %d", s_ril_conf_init.LTE_Is_Report_SignalStrength);
        } else if(strstr(p, "At_Cmds_For_Customer_Initialize")) {
            ql_at_cmds_for_customer_initialize(strchr(p, '=') + 1);
        } else if(strstr(p, "Icc_Constants")) {
            ql_icc_constants_init(strchr(p, '=') + 1);
        } else if(strstr(p, "Voice_Over_Usb_Mode")) {
            ql_voice_over_usb_mode(strchr(p, '=') + 1);
        } else if(strstr(p, "support_CDMPhone")) {
            s_ril_conf_init.support_CDMPhone= (int)strtol(strchr(p, '=') + 1, NULL, FORMATS_D);
        }
clear:
        p = NULL;
        memset(buf, 0, BUF_SIZE);
    }
    fclose(fp);

    return 0;
}

typedef struct {
    char *key;
    int value;
}ICC_CONSTANTS_MAP;

ICC_CONSTANTS_MAP Icc_Constants[] = {
    { "EF_ADN", 0x6F3A },
    { "EF_FDN", 0x6F3B },
    { "EF_GID1", 0x6F3E },
    { "EF_SDN", 0x6F49 },
    { "EF_EXT1", 0x6F4A },
    { "EF_EXT2", 0x6F4B },
    { "EF_EXT3", 0x6F4C },
    { "EF_EXT6", 0x6fc8 },
    { "EF_MWIS", 0x6FCA },
    { "EF_MBDN", 0x6fc7 },
    { "EF_PNN", 0x6fc5 },
    { "EF_OPL", 0x6fc6 },
    { "EF_SPN", 0x6F46 },
    { "EF_SMS", 0x6F3C },
    { "EF_ICCID", 0x2fe2 },
    { "EF_AD", 0x6FAD },
    { "EF_MBI", 0x6fc9 },
    { "EF_MSISDN", 0x6f40 },
    { "EF_SPDI", 0x6fcd },
    { "EF_SST", 0x6f38 },
    { "EF_CFIS", 0x6FCB },
    { "EF_IMG", 0x4f20 },
    { "EF_PBR", 0x4F30 },
    { "EF_LI", 0x6F05 },
    { "EF_MAILBOX_CPHS", 0x6F17 },
    { "EF_VOICE_MAIL_INDICATOR_CPHS", 0x6F11 },
    { "EF_CFF_CPHS", 0x6F13 },
    { "EF_SPN_CPHS", 0x6f14 },
    { "EF_SPN_SHORT_CPHS", 0x6f18 },
    { "EF_INFO_CPHS", 0x6f16 },
    { "EF_CSP_CPHS", 0x6f15 },
    { "EF_CST", 0x6f32 },
    { "EF_RUIM_SPN", 0x6F41 },
    { "EF_PL", 0x2F05 },
    { "EF_CSIM_LI", 0x6F3A },
    { "EF_CSIM_SPN", 0x6F41 },
    { "EF_CSIM_MDN", 0x6F44 },
    { "EF_CSIM_IMSIM", 0x6F22 },
    { "EF_CSIM_CDMAHOME", 0x6F28 },
    { "EF_CSIM_EPRL", 0x6F5A },
    { "EF_IMPU", 0x6f04 },
    { "EF_IMPI", 0x6f02 },
    { "EF_DOMAIN", 0x6f03 },
    { "EF_IST", 0x6f07 },
    { "EF_PCSCF", 0x6f09 },
    { "EF_PSI", 0x6fe5 }
};

static int ql_get_icc_constansts_value(const char *key)
{
    unsigned int i = 0;

    if(NULL == key) return 0;
    for(i = 0; i < sizeof(Icc_Constants) / sizeof(Icc_Constants[0]); i++) {
        if(0 == strcmp(key, Icc_Constants[i].key)) {
            return Icc_Constants[i].value;
        }
    }

    return 0;
}

static void ql_icc_constants_init(const char *line)
{
    int i = 0;
    int index = 0;
    int value = 0;

    ql_get_commamd_line(line, "; ");

    for(i = 0; i < s_commamd_line_cnt; i++)    {
        LOGD("Icc_Constants[%d] = %s", i, s_commamd_line[i]);
        if(0 != (value = ql_get_icc_constansts_value(s_commamd_line[i]))) {
            s_ril_conf_init.Icc_Constants[index++] = value;
        }
    }
}

static void ql_at_cmds_for_customer_initialize(const char *line)
{
    int i = 0;
    ql_get_commamd_line(line, ";");
    while(0 != s_commamd_line[i][0]) {
        LOGD("at_cmds_for_customer[%d] = %s", i, s_commamd_line[i]);
        i++;
    }
    memcpy(s_ril_conf_init.At_Cmds_For_Customer, s_commamd_line, sizeof(s_ril_conf_init.At_Cmds_For_Customer));
}

static void ql_get_commamd_line(const char *line, const char *delim)
{
    char *result = NULL;
    int value;

    memset(s_commamd_line, 0, sizeof(s_commamd_line));
    s_commamd_line_cnt = 0;

    if(NULL != line) {
        result = strtok((char *)line, delim);
        while( result != NULL ) {
            strcpy(s_commamd_line[s_commamd_line_cnt++], result);
            result = strtok( NULL, delim);
        }
    }
}

static void ql_voice_over_usb_mode(char *line)
{
    line = ql_rm_pre_space(line);
    line = ql_rm_tail_space(line);
    strcpy(s_ril_conf_init.Voice_Over_Usb_Mode, line);
}

static char* ql_rm_pre_space(char *line)
{
    if((NULL == line) || ( '\0' == *line)) return NULL;
    while(('\t' == *line) || (32 == (int)(*line))) line++;
    return ('\0' == *line) ?  NULL : line;
}

static char* ql_rm_tail_space(char *line)
{
    char *p = NULL;
    if((NULL == line) || ( '\0' == *line)) return NULL;
    p = strchr(line, '\0');

    while('\t' == *(p-1) || '\n' == *(p-1) || ' ' == *(p-1)) p--;
    *p = '\0';

    return ('\0' == *line) ?  NULL : line;
}
