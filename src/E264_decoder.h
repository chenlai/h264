#ifndef _D264_DECODER_H_
#define _D264_DECODER_H_

#include "Dec_Struct.h"

typedef  void (*display_frame)(uint8_t *imgy, uint8_t *imgu, uint8_t *imgv,
                               int32_t width, int32_t height, int32_t width_cr, int32_t height_cr);

typedef struct
{
    void *handle;
    display_frame display;
}   DECODERPARAM;

int32_t   D264_decode_one_slice (DECODERPARAM *de_param, uint8_t *nalu_buf, int32_t buf_len);
void  D264_init_decoder(DECODERPARAM *de_param);
void  D264_free_decoder(DECODERPARAM *de_param);



#endif

