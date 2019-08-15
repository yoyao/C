#ifndef CRC_16_INCLUDE_H
#define CRC_16_INCLUDE_H

#ifdef __cplusplus
extern "C"
{
#endif
void addfcs16(unsigned char *cp,int len);
unsigned short calcfcs16(unsigned char *cp,int len);
int  isfcs16ok(unsigned char *cp,int len);
#ifdef __cplusplus
}
#endif

#endif
