#include "E264_BitStream.h"
//#include "E264_Table.h"
//#include "DMp4v_bitstype.h"
#ifdef CHIP_DM6446
#include "csl_idma.h"
#if 0
#include "c6x.h"
#endif
#endif

//#include "time_counter.h"






#pragma CODE_SECTION(ByteSwapU32,".H264Dec_L2_Code_GetMBData")
static _INLINE_ uint32_t ByteSwapU32(uint32_t data)
{
#ifdef CHIP_DM6446
    data = _packlh2(data,data);
    data = _swap4(data);
    return data;
#elif defined WIN32
    uint32_t ret_value;
    //°´byte½»»»Ë³Ðò£¬Èç£º1234->4321
    __asm{
        mov eax,data
            bswap eax
            mov ret_value,eax;
    }
    return ret_value;
#else
    uint32_t a0, a1, a2, a3;
    a0 = (uint8_t)(data>>24);
    a1 = (uint8_t)(data>>16);
    a2 = (uint8_t)(data>>8);
    a3 = (uint8_t)(data);
    return a0 | (a1<<8) | (a2<<16) | (a3<<24);
#endif
}


#pragma FUNC_CANNOT_INLINE(D264_DecBitstreamInit);
#pragma CODE_SECTION(D264_DecBitstreamInit,".H264Dec_L2_Code")
void D264_DecBitstreamInit( BITSTREAM * bs, uint8_t * bitstream)
{

    uint32_t tmp;
    bs->iPos = 0;

    //  bs->offset = (uint32_t)bitstream % 4;
    //  bitstream -= bs->offset;
    //  bs->iPos = bs->offset*8;
    bs->pStart = bs->pCurrent = bitstream;

    //tmp = *(int*)(bs->pCurrent);
    memcpy(&tmp, bs->pCurrent, sizeof(uint32_t));
    bs->pCurrent += 4;
    bs->DataA = ByteSwapU32(tmp);
    //tmp = *(int*)(bs->pCurrent);
    memcpy(&tmp, bs->pCurrent, sizeof(uint32_t));
    bs->pCurrent += 4;
    bs->DataB = ByteSwapU32(tmp);

}

#pragma FUNC_CANNOT_INLINE(D264_BitstreamGetUseByte);
#pragma CODE_SECTION(D264_BitstreamGetUseByte,".H264Dec_L2_Code_GetMBData")
uint32_t  D264_BitstreamGetUseByte(BITSTREAM * bs)
{
    uint32_t len = (uint32_t) bs->pCurrent  - (uint32_t) bs->pStart;

    len-=8;
    len *= 8;
    len += (bs->iPos);

    return len;
}

#pragma CODE_SECTION(GetNal_Dec,".H264Dec_L2_Code")
int GetNal_Dec(uint8_t* naluBuffer,uint8_t *pStart,int32_t byte_pos)
{
    int nBsLength = 0;
    int i;
    int count = 0;
    int data;
//#pragma MUST_ITERATE(4,,);
    for(i = 0; i < byte_pos; i++)
    {
        data = pStart[i];
        (count == ZEROBYTES_SHORTSTARTCODE && !(data & 0xFC)) ? naluBuffer[nBsLength++] = 0x03 : 0;
        count = count == ZEROBYTES_SHORTSTARTCODE ? 0:count;
        naluBuffer[nBsLength++] = data;
        count = data == 0 ? count+1:0;

    }

    return nBsLength;
}

#pragma FUNC_CANNOT_INLINE(D264_BitstreamGetBs);
#pragma CODE_SECTION(D264_BitstreamGetBs,".H264Dec_L2_Code")
int D264_BitstreamGetBs(BITSTREAM *bs,uint8_t *nalu_buf, int32_t buf_len, NALU *nalu)
{
    int32_t start_code_len;
    int32_t nalu_len;

    if(nalu_buf[0] == 0 && nalu_buf[1] == 0 && nalu_buf[2] == 0 && nalu_buf[3] == 1)
    {
        start_code_len = 4;
    }
    else if(nalu_buf[0] == 0 && nalu_buf[1] == 0 && nalu_buf[2] == 1)
    {
        start_code_len = 3;
    }
    else
    {
        //´íÎóÃ»ÓÐ´¦Àí
        start_code_len = 0;
    }

    //naluÀàÐÍºÍsliceÀàÐÍ
    nalu->nal_reference_idc    = (nalu_buf[start_code_len]>>5) & 3;
    nalu->nal_unit_type        = (nalu_buf[start_code_len]) & 0x1f;

#ifndef E264_NAL_NEW_003
    if(start_code_len != 0)
    {//nal packet
        nalu_len = D264_EBSP_to_RBSP(nalu_buf, (uint8_t *)bs->pStart, buf_len, 1 + start_code_len);
    }
    else
#endif
    {//rtp packet
        nalu_len = buf_len-1-start_code_len;
//      memcpy(bs->pStart,nalu_buf+1+start_code_len,nalu_len);
#ifndef E264_NAL_NEW_003
        bs->pStart = nalu_buf+1+start_code_len;
#else
        if(start_code_len != 0)
            bs->pStart = nalu_buf+1+start_code_len;
        else
        {
            nalu_len = GetNal_Dec(bs->pStart,nalu_buf+1+start_code_len,nalu_len);
        }
#endif
    }

    bs->nBsLength   = D264_RBSP_to_SODB((uint8_t *)bs->pStart, nalu_len);

    //  bs->pCurrent = bs->pStart;
    //  bs->nEBit = (8-(bs->nBsLength & 7)) & 7;

    //  bs->iPos    = 0;

    D264_DecBitstreamInit(bs,(uint8_t *)bs->pStart);


    bs->m_CodCounter = -1;
    return ERR_OK;
}



#define ZERO_P8x8     (mb_type == 5)
#define MODE_IS_P8x8  (mb_type == 4 || mb_type == 5)
#define MODE_IS_I4x4  (mb_type == 6)
#define I16OFFSET     (mb_type-7)

