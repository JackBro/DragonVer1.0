#ifndef _TRM_USB_COMM_H_
#define _TRM_USB_COMM_H_

#ifdef __cplusplus
extern "C" {
#endif

int		UsbPipe1WriteBuf(char *pt, int len);
int		UsbPipe1ReadBuf(char *pt, int len);
int		UsbPipe2WriteBuf(char *pt, int len);
int		UsbPipe2ReadBuf(char *pt, int len);
int		UsbPipe3WriteBuf(char *pt, int len);
int		UsbPipe3ReadBuf(char *pt, int len);


#ifdef __cplusplus
}
#endif

#endif


