#include "E264_slice.h"
#include "E264_BitStream.h"
#include "E264_decoder.h"
#include "H264_BaseFunc.h"





const unsigned char ZZ_SCAN[16]  =
{  0,  1,  4,  8,  5,  2,  3,  6,  9, 12, 13, 10,  7, 11, 14, 15
};
const unsigned char ZZ_SCAN8[64] =
{  0,  1,  8, 16,  9,  2,  3, 10, 17, 24, 32, 25, 18, 11,  4,  5,
12, 19, 26, 33, 40, 48, 41, 34, 27, 20, 13,  6,  7, 14, 21, 28,
35, 42, 49, 56, 57, 50, 43, 36, 29, 22, 15, 23, 30, 37, 44, 51,
58, 59, 52, 45, 38, 31, 39, 46, 53, 60, 61, 54, 47, 55, 62, 63
};
void Scaling_List(int *scalingList, int sizeOfScalingList, int *UseDefaultScalingMatrix, BITSTREAM *bs)
{
    int j, scanj;
    int delta_scale, lastScale, nextScale;

    lastScale      = 8;
    nextScale      = 8;

    for(j=0; j<sizeOfScalingList; j++)
    {
        scanj = (sizeOfScalingList==16) ? ZZ_SCAN[j]:ZZ_SCAN8[j];

        if(nextScale!=0)
        {
            delta_scale = D264_se_v (bs);
            nextScale = (lastScale + delta_scale + 256) % 256;
            *UseDefaultScalingMatrix = (scanj==0 && nextScale==0);
        }

        scalingList[scanj] = (nextScale==0) ? lastScale:nextScale;
        lastScale = scalingList[scanj];
    }
}


#pragma FUNC_CANNOT_INLINE(D264_interpret_SPS);
#pragma CODE_SECTION(D264_interpret_SPS,".H264Dec_SDRAM_Code")
int32_t  D264_interpret_SPS (BITSTREAM *bs, SEQ_PARAMETER_SET *sps,int *pWidth,int *pHeight)
{

    int32_t i;
    int32_t reserved_zero;
    int tmp;

    int crop_left, crop_right;
    int crop_top, crop_bottom;

    static const int SubWidthC  [4]= { 1, 2, 2, 1};
    static const int SubHeightC [4]= { 1, 2, 1, 1};

//  assert (bs->pStart != NULL);
//  assert (sps              != NULL);

    if(bs->pStart == NULL)
        return 1;
    if(sps == NULL)
        return 1;

    sps->profile_idc                               = D264_BitstreamGetBitsNotInline  (bs,8);
//  if(sps->profile_idc != 66)
//  {
//      printf("sorry! only profile of baseline is supported!\n");
//      return 1;
//  }

    sps->constrained_set0_flag                     = D264_BitstreamGetBitsNotInline(bs,1);
    sps->constrained_set1_flag                     = D264_BitstreamGetBitsNotInline(bs,1);
    sps->constrained_set2_flag                     = D264_BitstreamGetBitsNotInline(bs,1);
    reserved_zero                                  = D264_BitstreamGetBitsNotInline(bs,5);
//  assert (reserved_zero == 0);
    if(reserved_zero != 0)
        return 1;
    sps->level_idc                                 = D264_BitstreamGetBitsNotInline(bs,8);
//  if(sps->level_idc != 30)
//  {
//      printf("sorry! only level 2.0 is supported!\n");
//      return 1;
//  }

    sps->seq_parameter_set_id                      = D264_ue_v (bs);

     sps->chroma_format_idc = 1;
    if((sps->profile_idc==FREXT_HP   ) ||
        (sps->profile_idc==FREXT_Hi10P) ||
        (sps->profile_idc==FREXT_Hi422) ||
        (sps->profile_idc==FREXT_Hi444))
    {
        sps->chroma_format_idc                      = D264_ue_v (bs);

        // Residue Color Transform
        if(sps->chroma_format_idc == 3)
            sps->residue_transform_flag = D264_BitstreamGetBitsNotInline(bs,1);

        sps->bit_depth_luma_minus8                  = D264_ue_v (bs);
        sps->bit_depth_chroma_minus8                = D264_ue_v (bs);
        sps->lossless_qpprime_flag                  = D264_BitstreamGetBitsNotInline(bs,1);

        sps->seq_scaling_matrix_present_flag        = D264_BitstreamGetBitsNotInline(bs,1);

        if(sps->seq_scaling_matrix_present_flag)
        {
            for(i=0; i<8; i++)
            {
                sps->seq_scaling_list_present_flag[i]   = D264_BitstreamGetBitsNotInline(bs,1);
                if(sps->seq_scaling_list_present_flag[i])
                {
                    if(i<6)
                        Scaling_List(sps->ScalingList4x4[i], 16, &sps->UseDefaultScalingMatrix4x4Flag[i], bs);
                    else
                        Scaling_List(sps->ScalingList8x8[i-6], 64, &sps->UseDefaultScalingMatrix8x8Flag[i-6], bs);
                }
            }
        }
  }



    sps->log2_max_frame_num_minus4                 = D264_ue_v (bs);
    sps->pic_order_cnt_type                        = D264_ue_v (bs);
    if (sps->pic_order_cnt_type == 0)
    {
        sps->log2_max_pic_order_cnt_lsb_minus4     = D264_ue_v (bs);
    }
    else if (sps->pic_order_cnt_type == 1)
    {
        sps->delta_pic_order_always_zero_flag      = D264_BitstreamGetBitsNotInline  (bs,1);
        sps->offset_for_non_ref_pic                = D264_se_v (bs);
        sps->offset_for_top_to_bottom_field        = D264_se_v (bs);
        sps->num_ref_frames_in_pic_order_cnt_cycle = D264_ue_v (bs);

        for(i = 0; i < sps->num_ref_frames_in_pic_order_cnt_cycle; i++)
        {
            sps->offset_for_ref_frame[i]           = D264_se_v (bs);
        }
    }

    tmp  = D264_ue_v (bs);
    if(sps->num_ref_frames!= tmp)
        sps->param_changed_flag = 1;
    sps->num_ref_frames = tmp;

    sps->gaps_in_frame_num_value_allowed_flag      = D264_BitstreamGetBitsNotInline  (bs,1);

    tmp                   = D264_ue_v (bs);
    if(sps->pic_width_in_mbs_minus1!= tmp)
        sps->param_changed_flag = 1;
    sps->pic_width_in_mbs_minus1 = tmp;

    tmp                   = D264_ue_v (bs);
    if(sps->pic_height_in_map_units_minus1!= tmp)
        sps->param_changed_flag = 1;
    sps->pic_height_in_map_units_minus1 = tmp;

    #if 0
    *pWidth = sps->pic_width_in_mbs_minus1*16+16;
    *pHeight = sps->pic_height_in_map_units_minus1*16+16;
    #endif


    #if 0
    if(*pHeight == 1088)
        *pHeight = 1080;

    #endif


    sps->frame_mbs_only_flag                       = D264_BitstreamGetBitsNotInline  (bs,1);
    if (!sps->frame_mbs_only_flag)
    {
        sps->mb_adaptive_frame_field_flag          = D264_BitstreamGetBitsNotInline  (bs,1);
    }
    sps->direct_8x8_inference_flag                 = D264_BitstreamGetBitsNotInline  (bs,1);
    sps->frame_cropping_flag                       = D264_BitstreamGetBitsNotInline  (bs,1);

    if (sps->frame_cropping_flag)
    {
        sps->frame_cropping_rect_left_offset       = D264_ue_v (bs);
        sps->frame_cropping_rect_right_offset      = D264_ue_v (bs);
        sps->frame_cropping_rect_top_offset        = D264_ue_v (bs);
        sps->frame_cropping_rect_bottom_offset     = D264_ue_v (bs);

//        printf("chroma_format_idc = %d\n", sps->chroma_format_idc);
        crop_left   = SubWidthC [sps->chroma_format_idc] * sps->frame_cropping_rect_left_offset;
        crop_right  = SubWidthC [sps->chroma_format_idc] * sps->frame_cropping_rect_right_offset;
        crop_top    = SubHeightC[sps->chroma_format_idc] * ( 2 - sps->frame_mbs_only_flag ) *  sps->frame_cropping_rect_top_offset;
        crop_bottom = SubHeightC[sps->chroma_format_idc] * ( 2 - sps->frame_mbs_only_flag ) *  sps->frame_cropping_rect_bottom_offset;
    }
    else
    {
        crop_left = crop_right = crop_top = crop_bottom = 0;
    }

    sps->vui_parameters_present_flag               = D264_BitstreamGetBitsNotInline  (bs,1);
#ifdef _PRINT_DEBUG_INFO
    if (sps->vui_parameters_present_flag)
    {
//        printf ("VUI sequence parameters present but not supported, ignored\n");
    }
#endif

    *pWidth  = sps->pic_width_in_mbs_minus1 * 16 + 16 - crop_left - crop_right;
    *pHeight = sps->pic_height_in_map_units_minus1 * 16 + 16 - crop_top - crop_bottom;

//    printf("width = %d, height = %d\n",*pWidth, *pHeight);
    sps->Valid  = TRUE;
    return 0;
}
/**/



#pragma FUNC_CANNOT_INLINE(D264_process_SPS);
#pragma CODE_SECTION(D264_process_SPS,".H264Dec_SDRAM_Code")
int32_t  D264_process_SPS (BITSTREAM *bs,SEQ_PARAMETER_SET *tmp_sps,int *pWidth,int *pHeight)
{
    if ( D264_interpret_SPS(bs, tmp_sps,pWidth,pHeight) != 0 )
    {
        return 1;
    }
    return 0;
}




#pragma FUNC_CANNOT_INLINE(D264_DecSliceHdr);
#pragma CODE_SECTION(D264_DecSliceHdr,".H264Dec_L2_Code")
int32_t  D264_DecSliceHdr(BITSTREAM *bs, NALU *nalu,int *pWidth,int *pHeight, uint8_t *pProfile_idc)
{
    int32_t old_max_dpb;
    int32_t          re_status;
    int32_t          current_header = 0;
    int32_t          param_changed_flag = 0;
    int32_t          old_width_in_mb;
    int32_t          old_height_in_mb;
    SEQ_PARAMETER_SET m_sps[1];

    if(nalu->nal_unit_type == NALU_TYPE_SPS)
    {
        re_status = D264_process_SPS(bs, m_sps,pWidth,pHeight);
        if ( 0 != re_status )
        {
            return PARAM_ERR;
        }
        else
        {
            *pProfile_idc = m_sps[0].profile_idc;
            return END_XPS;
        }
    }
    else
    {
        return PARAM_ERR;
    }
}


