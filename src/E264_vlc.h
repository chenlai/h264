#ifndef _D264_VLC_H_
#define _D264_VLC_H_

//#include <stdio.h>
//#include <stdlib.h>
//#include <assert.h>
//#include <string.h>
//#include <math.h>
#include "E264_EncType.h"
#include "E264_Defines.h"
//#include "E264_Table.h"
//#include "DMp4v_bitstype.h"
#ifdef CHIP_DM6446
#if 0
#include "c6x.h"
#endif
#endif


uint32_t  D264_BitstreamGetBitsNotInline(BITSTREAM * const bs,
                                    const uint32_t bits);

int32_t D264_se_v (BITSTREAM *bs);

int32_t D264_ue_v (BITSTREAM *bs);

//int32_t D264_u_1  (BITSTREAM *bs);

//int32_t D264_u_v  (int32_t len, BITSTREAM *bs);



int32_t  D264_get_VLC_symbol(BITSTREAM *bs, int32_t *info);

int32_t D264_read_intra4x4_prediction_mode(BITSTREAM *bs);

//int32_t  D264_read_FLC(int32_t sym_len, BITSTREAM *bs);

void D264_read_numcoeff_trailingones (int32_t *numcoeff, int32_t *numtrailingones, BITSTREAM *bs);

void D264_read_numcoeff_trailingones_ChromaDC (int32_t *numcoeff, int32_t *numtrailingones, BITSTREAM *bs);

//int32_t  D264_read_level(int32_t vlcnum, BITSTREAM *bs);

int32_t  D264_read__totalzeros(int32_t vlcnum, BITSTREAM *bs);

int32_t  D264_read_totalzeros_ChromaDC(int32_t vlcnum, BITSTREAM *bs);

int32_t  D264_read_run(int32_t vlcnum, BITSTREAM *bs);

uint32_t D264_show_bits (uint8_t buffer[],uint32_t totbitoffset,uint32_t bytecount, uint32_t numbits);






//  switch (block_type)
//  {
//  case LUMA:
//      max_coeff_num = 16;
//      break;
//  case LUMA_INTRA16x16AC:
//      max_coeff_num = 15;
//      start_scan = 0;
//      break;
//  }
//  block_type>>=1;
//
//  max_coeff_num = 16-block_type;
//  start_scan = block_type - 1;

#endif
