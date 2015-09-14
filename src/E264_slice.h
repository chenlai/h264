#ifndef _D264_SLICE_H_
#define _D264_SLICE_H_

//#include <stdio.h>
//#include <stdlib.h>
#include "E264_Defines.h"
#include "E264_EncType.h"
#include "D264.h"
#include "E264_nal.h"
#include "E264_vlc.h"


void D264_FrameInit(H264ENCODER *pEnc);
void D264_first_slice_header(H264ENCODER *pEnc, BITSTREAM *bs);
int32_t  D264_DecSliceHdr(BITSTREAM *bs, NALU *nalu,int *pWidth,int *pHeight, uint8_t *pProfile_idc);
void D264_add_blank_frame (H264ENCODER *pEnc);
void D264_add_tail_slice  (H264ENCODER *pEnc);
void D264_add_header_slice(H264ENCODER *pEnc);
void D264_add_waist_slice (H264ENCODER *pEnc);

int32_t  D264_process_PPS        (BITSTREAM *bs, PIC_PARAMETER_SET *tmp_pps);
int32_t  D264_process_SPS        (BITSTREAM *bs, SEQ_PARAMETER_SET *tmp_sps,int *pWidth,int *pHeight);
int32_t  D264_interpret_PPS      (BITSTREAM *bs, PIC_PARAMETER_SET *pps);
int32_t  D264_interpret_SPS (BITSTREAM *bs, SEQ_PARAMETER_SET *sps,int *pWidth,int *pHeight);
void D264_make_PPS_available (int32_t id, PIC_PARAMETER_SET *m_pps, PIC_PARAMETER_SET *tmp_pps);
void D264_make_SPS_available (int32_t id, SEQ_PARAMETER_SET *m_sps, SEQ_PARAMETER_SET *tmp_sps);

int D264_start_decoder      (H264ENCODER *pEnc);
//void D264_end_decoder        (H264ENCODER *pEnc);
#endif

