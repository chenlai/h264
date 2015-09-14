#ifndef  _D264_BITSTREAM_H_
#define  _D264_BITSTREAM_H_

//#include <stdio.h>
//#include <stdlib.h>
//#include <assert.h>
//#include <string.h>
//#include <limits.h>
#include "E264_vlc.h"
#include "E264_nal.h"
#include "E264_EncType.h"
#include "E264_Defines.h"
#include "E264_interpolation.h"


int D264_BitstreamGetBs(BITSTREAM *bs,uint8_t *nalu_buf, int32_t buf_len,  NALU *nalu);

#endif

