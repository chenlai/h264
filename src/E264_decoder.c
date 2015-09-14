#include "E264_BaseType.h"
#include "E264_slice.h"
#include "E264_Defines.h"
//#include "E264_decmb.h"
#include "E264_EncType.h"
//#include "H264_loopfilter.h"
#include "D264.h"
#include "E264_BitStream.h"
#include "E264_decoder.h"
//#include "E264_Table.h"
#include "H264_BaseFunc.h"
#ifdef ENABLE_EDMA3
#include "edma3_dm6467.h"
#include "channel.h"
#include "csl_cache.h"
#include "csl_idma.h"
#endif

//#include "time_counter.h"

#define MAX_SPS_LEN 256
#define BS_ALIGN_BYTE 8
unsigned char pBs[MAX_SPS_LEN+8];

#pragma CODE_SECTION(D264_StartSlice,".H264Dec_SDRAM_Code")
int D264_StartSlice(uint8_t *nalu_buf,int32_t buf_len,int *pWidth,int *pHeight, uint8_t *pProfile_idc)
{
    int ret;
    BITSTREAM   bs[1];//     = &pEnc->m_bs;
    NALU nalu;
    int CurHdr;

    unsigned char *pAlignBs = pBs+((BS_ALIGN_BYTE-((int)pBs)%8)&(BS_ALIGN_BYTE-1));

    int bslen = buf_len > 256 ? 256 : buf_len;
    memcpy(pAlignBs,nalu_buf,bslen);

    if(bslen <= 0)
        return ERR_FAIL;
    ret = D264_BitstreamGetBs(bs, pAlignBs, bslen, &nalu);
    if(ret != ERR_OK)
        return ERR_FAIL;
    CurHdr = D264_DecSliceHdr(bs, &nalu,pWidth,pHeight,pProfile_idc);
    if(CurHdr != PARAM_ERR && CurHdr != NOT_SUPPORTED && CurHdr != ERR_FAIL)
    {
        if(CurHdr == END_XPS)
            return ERR_OK;
    }
    else
    {
        return ERR_FAIL;
    }

    return ERR_OK;
}




