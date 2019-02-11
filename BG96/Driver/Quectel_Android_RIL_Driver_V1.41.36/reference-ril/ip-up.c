#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

static int property_set(const char *key, const char *value) {
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "/system/bin/setprop %s %s", key, (value && value[0]) ? value : "\"\"");
    return system(cmd);
}

int main(int argc, char *argv[]) {
    argc = argc;
    argv = argv;    
    char *dns1 = getenv("DNS1");
    char *dns2 = getenv("DNS2");
    char *iplcocal = getenv("IPLOCAL");
    char *ipremote = getenv("IPREMOTE");

    property_set("net.ppp0.dns1", dns1 ? dns1 : "");
    property_set("net.ppp0.dns2", dns2 ? dns2 : "");
    property_set("net.ppp0.local-ip", iplcocal ? iplcocal : "");
    property_set("net.ppp0.remote-ip", ipremote ? ipremote : "");
    property_set("net.ppp0.gw", ipremote ? ipremote : "");
    return 0;
}
