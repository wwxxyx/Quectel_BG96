/**
 *  Date  :2014-1-26
 *  Author:Wythe
 *  Desc  :This file is used to define the error code for SIM card and SMS. The 
 *  error code can be also found in released ATC document.
 */

#ifndef AT_ERROR_H
#define AT_ERROR_H

#define cme_error \
    aterror(CME, MODULE_FAILURE, 0) \
    aterror(CME, NO_MODULE_CONNECTION, 1) \
    aterror(CME, PHONE_ADAPTER_RESERVED, 2) \
    aterror(CME, OPERATION_NOT_ALLOWED, 3) \
    aterror(CME, OPERATION_NOT_SUPPORTED, 4) \
    aterror(CME, PH_SIM_PIN, 5) \
    aterror(CME, PH_FSIM_PIN, 6) \
    aterror(CME, PH_FSIM_PUK, 7) \
    aterror(CME, SIM_NOT_INSERTED, 10) \
    aterror(CME, SIM_PIN_REQUIRED, 11) \
    aterror(CME, SIM_PUK_REQUIRED, 12) \
    aterror(CME, FAILURE, 13) \
    aterror(CME, SIM_BUSY, 14) \
    aterror(CME, SIM_WRONG, 15) \
    aterror(CME, INCORRECT_PASSWORD, 16) \
    aterror(CME, SIM_PIN2_REQUIRED, 17) \
    aterror(CME, SIM_PUK2_REQUIRED, 18) \
    aterror(CME, MEMORY_FULL, 20) \
    aterror(CME, INVALID_INDEX, 21) \
    aterror(CME, NOT_FOUND, 22) \
    aterror(CME, MEMORY_FAILURE, 23) \
    aterror(CME, STRING_TO_LONG, 24) \
    aterror(CME, INVALID_CHAR, 25) \
    aterror(CME, DIALSTR_TO_LONG, 26) \
    aterror(CME, INVALID_DIALCHAR, 27) \
    aterror(CME, NO_NETWORK_SERVICE, 30) \
    aterror(CME, NETWORK_TIMEOUT, 31) \
    aterror(CME, NETWORK_NOT_ALLOWED, 32) \
    aterror(CME, NETWORK_PERSONALIZATION_PIN_REQUIRED, 40) \
    aterror(CME, NETWORK_PERSONALIZATION_PUK_REQUIRED, 41) \
    aterror(CME, NETWORK_SUBSET_PERSONALIZATION_PIN_REQUIRED, 42) \
    aterror(CME, NETWORK_SUBSET_PERSONALIZATION_PUK_REQUIRED, 43) \
    aterror(CME, SERVICE_PROVIDER_PERSONALIZATION_PIN_REQUIRED, 44) \
    aterror(CME, SERVICE_PROVIDER_PERSONALIZATION_PUK_REQUIRED, 45) \
    aterror(CME, CORPORATE_PERSONALIZATION_PIN_REQUIRED, 46) \
    aterror(CME, CORPORATE_PERSONALIZATION_PUK_REQUIRED, 47) \

#define cms_error \
    aterror(CMS, ME_FAILURE, 300) \
    aterror(CMS, SMS_OF_ME_RESERVED, 301) \
    aterror(CMS, SERVICE_OPERATION_NOT_ALLOWED, 302) \
    aterror(CMS, SERVICE_OPERATION_NOT_SUPPORTED, 303) \
    aterror(CMS, INVALID_PDU_PARAMETER, 304) \
    aterror(CMS, INVALID_TEXT_PARAMETER, 305) \
    aterror(CMS, SERVICE_SIM_NOT_INSERTED, 310) \
    aterror(CMS, SERVICE_SIM_PIN_REQUIRED, 311) \
    aterror(CMS, PH_SIM_PIN_REQUIRED, 312) \
    aterror(CMS, SIM_FAILURE, 313) \
    aterror(CMS, SERVICE_SIM_BUSY, 314) \
    aterror(CMS, SERVICE_SIM_WRONG, 315) \
    aterror(CMS, SIM_PUK_REQUIRED, 316) \
    aterror(CMS, SERVICE_SIM_PIN2_REQUIRED, 317) \
    aterror(CMS, SERVICE_SIM_PUK2_REQUIRED, 318) \
    aterror(CMS, SERVICE_MEMORY_FAILURE, 320) \
    aterror(CMS, INVALID_MEMORY_INDEX, 321) \
    aterror(CMS, SERVICE_MEMORY_FULL, 322) \
    aterror(CMS, SMSC_ADDR_UNKNOWN, 330) \
    aterror(CMS, NO_NETWORK_SERVICE, 331) \
    aterror(CMS, NETWORK_TIMEOUT, 332) \
    aterror(CMS, UNKNOWN_ERROR, 500) \
    aterror(CMS, SIM_NOT_READY, 512) \
    aterror(CMS, MESSAGE_LENGTH_EXCEEDS, 513) \
    aterror(CMS, INVALID_REQUEST_PARAMETERS, 514) \
    aterror(CMS, ME_STORAGE_FAILURE, 515) \
    aterror(CMS, INVALID_SERVICE_MODE, 517) \
    aterror(CMS, MORE_MESSAGE_TO_SEND_STATE_ERROR, 528) \
    aterror(CMS, MO_SMS_IS_NOT_ALLOW, 529) \
    aterror(CMS, GPRS_SUSPENDED, 530) \
    aterror(CMS, ME_STORAGE_FULL, 531)

#define aterror(group, name, num) group(name, num)

typedef enum{
    CME_ERROR_NON_CME = -1,
    CME_SUCCESS = 0,
#define CME(name, num) CME_ ## name = num,
    cme_error
#undef CME            
}AT_CME_Error;

typedef enum{
    CMS_ERROR_NON_CMS = -1,
#define CMS(name, num) CMS_ ## name = num,
    cms_error
#undef CMS            
}AT_CMS_Error;

#define AT_ERROR_BASE          0 /* see also _TOP */
#define CME_ERROR_BASE      1000 /* see also _TOP */
#define CMS_ERROR_BASE      2000 /* see also _TOP */
#define GENERIC_ERROR_BASE  3000 /* see also _TOP */

#define AT_ERROR_TOP       (CME_ERROR_BASE - 1) /* see also _BASE */
#define CME_ERROR_TOP      (CMS_ERROR_BASE - 1) /* see also _BASE */
#define CMS_ERROR_TOP      (GENERIC_ERROR_BASE - 1) /* see also _BASE */
#define GENERIC_ERROR_TOP  (GENERIC_ERROR_BASE + 999) /* see also _BASE */

typedef enum {
    NONE_ERROR,
    AT_ERROR,
    CME_ERROR,
    CMS_ERROR,
    GENERIC_ERROR,
    UNKNOWN_ERROR,
} AT_Error_type;

#endif

