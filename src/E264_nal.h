#ifndef _E264_NAL_H_
#define _E264_NAL_H_

//#include <stdio.h>
//#include <stdlib.h>
//#include <assert.h>
//#include <string.h>
#include "E264_Defines.h"
#include "E264_EncType.h"

int32_t E264_RBSP_to_SODB(uint8_t *buf, int32_t last_byte_pos);

int32_t E264_find_start_code(uint8_t *buf, int32_t zeros_in_startcode);

int32_t E264_more_rbsp_data(uint8_t *buf, int32_t totbitoffset, int32_t bytecount);

int32_t E264_EBSP_to_RBSP(uint8_t *nalu_buf, uint8_t *bs_buffer, int32_t end_bytepos, int32_t begin_bytepos);

#endif
