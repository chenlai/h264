#ifndef _D264_ENCTYPE_H_
#define _D264_ENCTYPE_H_

#include "E264_Defines.h"
#include "E264_BaseType.h"
#include "D264.h"
#include "NwCodec.h"
//#include "H264_type.h"





//sps
typedef struct
{
	uint8_t  Valid;                                            // indicates the parameter set is Valid
    uint8_t  profile_idc;                                      // u(8)
	uint8_t  constrained_set0_flag;                            // u(1)
	uint8_t  constrained_set1_flag;                            // u(1)
	uint8_t  constrained_set2_flag;                            // u(1)
	uint8_t  level_idc;                                        // u(8)
	uint8_t  seq_parameter_set_id;                             // ue(v)
	uint8_t  log2_max_frame_num_minus4;                        // ue(v)
	uint8_t  pic_order_cnt_type;
	// if( pic_order_cnt_type == 0 )
	uint8_t  log2_max_pic_order_cnt_lsb_minus4;                 // ue(v)
	// else if( pic_order_cnt_type == 1 )
	uint8_t       num_ref_frames;                                   // ue(v)
	uint8_t  gaps_in_frame_num_value_allowed_flag;             // u(1)
	uint8_t       pic_width_in_mbs_minus1;                          // ue(v)
	uint8_t       pic_height_in_map_units_minus1;                   // ue(v)
	uint8_t  frame_mbs_only_flag;                              // u(1)
	// if( !frame_mbs_only_flag )
	uint8_t  direct_8x8_inference_flag;                        // u(1)
	uint8_t  frame_cropping_flag;                              // u(1)
	uint8_t   vui_parameters_present_flag;
                 // u(1)
	//  vui_seq_parameters_t vui_seq_parameters;            // vui_seq_parameters_t
	uint8_t  delta_pic_order_always_zero_flag;               // u(1)
	uint8_t       offset_for_non_ref_pic;                         // se(v)
	uint8_t       offset_for_top_to_bottom_field;                 // se(v)
	uint8_t       num_ref_frames_in_pic_order_cnt_cycle;          // ue(v)
	// for( i = 0; i < num_ref_frames_in_pic_order_cnt_cycle; i++ )
	uint8_t   offset_for_ref_frame[MAX_NUM_REF_FRAMES_IN_PIC_ORDER_CNT_CYCLE];   // se(v)
	uint8_t  mb_adaptive_frame_field_flag;                 // u(1)
	uint8_t  frame_cropping_rect_left_offset;              // ue(v)
	uint8_t  frame_cropping_rect_right_offset;             // ue(v)
	uint8_t  frame_cropping_rect_top_offset;               // ue(v)
	uint8_t  frame_cropping_rect_bottom_offset;            // ue(v)
	uint8_t  param_changed_flag;

	unsigned  chroma_format_idc;                                // ue(v)

	int  seq_scaling_matrix_present_flag;                   // u(1)
	int      seq_scaling_list_present_flag[8];                  // u(1)
	int      ScalingList4x4[6][16];                             // se(v)
	int      ScalingList8x8[2][64];                             // se(v)
	int  UseDefaultScalingMatrix4x4Flag[6];
	int  UseDefaultScalingMatrix8x8Flag[2];
	int residue_transform_flag;
	unsigned  bit_depth_luma_minus8;                            // ue(v)
	unsigned  bit_depth_chroma_minus8;                          // ue(v)
	int lossless_qpprime_flag;
}SEQ_PARAMETER_SET;

//pps
typedef struct
{
	uint8_t  Valid;                                            // indicates the parameter set is Valid
	uint8_t  pic_parameter_set_id;                             // ue(v)
	uint8_t  seq_parameter_set_id;                             // ue(v)
	uint8_t  entropy_coding_mode_flag;                         // u(1)
	uint8_t  pic_order_present_flag;                           // u(1)
	uint8_t  num_slice_groups_minus1;                          // ue(v)
	// if( slice_group_map_type = = 0 )
	// else if( slice_group_map_type = = 2 )
	// else if( slice_group_map_type = = 3 || 4 || 5
	// else if( slice_group_map_type = = 6 )
	uint8_t  num_ref_idx_l0_active_minus1;                     // ue(v)
	uint8_t  num_ref_idx_l1_active_minus1;                     // ue(v)
	uint8_t  weighted_pred_flag;                               // u(1)
	uint8_t  weighted_bipred_idc;                              // u(2)
	int8_t       pic_init_qp_minus26;                              // se(v)
	int8_t       pic_init_qs_minus26;                              // se(v)
	int8_t       chroma_qp_index_offset;                           // se(v)
	uint8_t  deblocking_filter_control_present_flag;           // u(1)
	uint8_t  constrained_intra_pred_flag;                      // u(1)
	uint8_t  redundant_pic_cnt_present_flag;                   // u(1)

	uint8_t  slice_group_map_type;                         // ue(v)
	uint8_t  run_length_minus1[MAX_NUM_SLICE_GROUPS_MINUS1]; // ue(v)
	uint8_t  top_left    [MAX_NUM_SLICE_GROUPS_MINUS1];     // ue(v)
	uint8_t  bottom_right[MAX_NUM_SLICE_GROUPS_MINUS1];     // ue(v)
	uint8_t  slice_group_change_direction_flag;            // u(1)
	uint8_t  slice_group_change_rate_minus1;               // ue(v)
	uint8_t  num_slice_group_map_units_minus1;             // ue(v)
	uint8_t  *slice_group_id;                              // complete MBAmap u(v)
}   PIC_PARAMETER_SET;








typedef struct
{
	uint8_t *pStart;
	uint8_t *pCurrent;
	uint32_t DataA;//两个32位的缓冲可能没有利用起来,到时候确认一下
	uint32_t DataB;
	uint32_t iPos;

	int32_t  nBsLength;
	int32_t m_CodCounter;//<! Current count of number of skipped macroblocks in a row

	int32_t m_SliceType;


	int16_t *runarr;
	int16_t *levarr;
//	NNZ_BLOCK *m_NnzBlkRow[3];

	int32_t WidthInMbs;
//	uint32_t WidthInMbs_mul;
#ifdef ENABLE_SMALL_NAL
	int32_t DecSliceNo;
#endif
	int32_t MbNumOfPic;
//	int32_t NumDecoderMB;
	///////////////, 用来计算宏块可用性
//	int32_t ChromaQpOffset;
}   BITSTREAM;

typedef struct
{
//	int UseAnnexbLongStartcode;
	int nal_unit_type;            //! NALU_TYPE_xxxx
	int nal_reference_idc;        //! NALU_PRIORITY_xxxx
} NALU;



typedef struct
{
	uint8_t   m_SliceType;       //I_slice/P_slice
#ifdef ENABLE_SMALL_NAL
	uint8_t   m_SliceIdx;  //index of slice in current frame
#endif
	int32_t  m_StartMbIdx;           //slice start macroblock, must be set by NAL even in case of ei_flag == 1

	uint8_t   m_fgIDR;     //IDR flag
	//////////////////////////////////////////////////////////////////////////

	SEQ_PARAMETER_SET *m_sps;
	PIC_PARAMETER_SET *m_pps;


	BITSTREAM            m_bs;

	MEMORY SDRAM;             //片外的内存指针
	MEMORY L2Tmp;			  //片内L2, 在编码解码每一帧的时候临时用到的内存,因而在多路的时候能复用这个内存.
    MEMORY L2Filter;
	int32_t  nal_reference_idc;//NALU_PRIORITY_xxxx
	int32_t  m_PicMbNum;
	int bSkipFrame;
}   H264ENCODER;

#endif

