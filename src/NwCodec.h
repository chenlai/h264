#ifndef _NWCODEC_H_
#define _NWCODEC_H_

#ifndef ERR_OK
#define ERR_OK            0L       //�����ɹ�
#define ERR_FAIL          -1L      //����ʧ��
#endif // ERR_OK
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif


typedef struct MEMORY_S
{
	unsigned char *pMem;
	int   nMemLen;
}MEMORY;

#endif

