#ifndef __QL_USB_H__
#define __QL_USB_H__

#define MAX_CARD_NUM 4
char *ql_get_ttyname(int usb_interface, char *out_ttyname);
char *ql_get_ndisname(char *p_usbnet_adapter, int i);
int ql_get_usb_device_info(int usb_interface);
void ql_set_autosuspend(int enable);
int ql_is_quectel_ttyport(const char*in_ttyname);
int ql_detect_quectel_modules(void);

#endif
