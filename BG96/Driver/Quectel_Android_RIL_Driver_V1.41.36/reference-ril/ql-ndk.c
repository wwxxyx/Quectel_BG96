#ifdef USE_NDK
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <dlfcn.h>

#define LOG_NDEBUG 0
#define LOG_TAG "NDK"
#include "ql-log.h"

const char *(*requestToString)(int request);
int (*ifc_init)(void);
void (*ifc_close)(void);
int (*do_dhcp)(const char *iname);
void (*get_dhcp_info)(uint32_t *ipaddr, uint32_t *gateway, uint32_t *prefixLength,
                   uint32_t *dns1, uint32_t *dns2, uint32_t *server,
                   uint32_t *lease);  
int (*property_set)(const char *key, const char *value);
int (*property_get)(const char *key, char *value, const char *default_value);

#define dlsym_get(__func) do { \
        __func = (typeof(__func))dlsym(dlHandle, #__func); \
        if (__func == NULL) { \
            LOGE("%s not defined or exported in %s\n", #__func, libpath); \
            sleep(60); \
        }    \
} while(0)
 
void ql_ndk_init(void) {
    void *dlHandle;
    char *libpath;

    libpath = "/system/lib/libnetutils.so";
    if (sizeof(libpath) == 8) //64bit
        libpath = "/system/lib64/libnetutils.so";
    
    dlHandle = dlopen(libpath, RTLD_NOW);
    if (dlHandle == NULL) {
        LOGE("dlopen %s failed: %s\n", libpath, dlerror());
        goto __dlopen_done;
    }
        
    dlsym_get(ifc_init);
    dlsym_get(ifc_close);
    dlsym_get(do_dhcp);
    dlsym_get(get_dhcp_info);

    libpath = "/system/lib/libril.so";
    if (sizeof(libpath) == 8) //64bit
        libpath = "/system/lib64/libril.so";
    
    dlHandle = dlopen(libpath, RTLD_NOW);
    if (dlHandle == NULL) {
        LOGE("dlopen %s failed: %s\n", libpath, dlerror());
        goto __dlopen_done;
    }
        
    dlsym_get(requestToString);

    libpath = "/system/lib/libcutils.so";
    if (sizeof(libpath) == 8) //64bit
        libpath = "/system/lib64/libcutils.so";
    
    dlHandle = dlopen(libpath, RTLD_NOW);
    if (dlHandle == NULL) {
        LOGE("dlopen %s failed: %s\n", libpath, dlerror());
        goto __dlopen_done;
    }
        
    dlsym_get(property_set);
    dlsym_get(property_get);
    
__dlopen_done:
    return;
}
#endif
