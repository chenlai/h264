#ifndef _D264_DECODER_H_
#define _D264_DECODER_H_
//#include "Dec_Struct.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ERR_OK 0L
#define ERR_FAIL -1

	int D264_StartSlice(uint8_t *nalu_buf,int32_t buf_len,int *pWidth,int *pHeight, uint8_t *pProfile_idc);


#ifdef __cplusplus
};
#endif

#endif

