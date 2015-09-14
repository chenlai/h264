#include "E264_vlc.h"
//#include "E264_Table.h"
//#include "DMp4v_bitstype.h"

uint32_t bitc(uint8_t src)
{
    int i;
    int nBit = 0;
    for(i=0;i<8;i++)
    {
        nBit += (((1<<i)&src) != 0);
    }
    return nBit;
}
uint32_t _bitc4(uint32_t src)
{
    int result;

    int i;
    for(i=0;i<4;i++)
    {
        ((uint8_t *)(&result))[i] = bitc(((uint8_t *)(&src))[i]);
    }
    return result;
}
uint32_t _swap4(uint32_t src)
{
    int result = 0;
    int i;

    result = 0;



    ((uint8_t *)(&result))[0] =  ((uint8_t *)(&src))[1];
    ((uint8_t *)(&result))[1] =  ((uint8_t *)(&src))[0];
    ((uint8_t *)(&result))[2] =  ((uint8_t *)(&src))[3];
    ((uint8_t *)(&result))[3] =  ((uint8_t *)(&src))[2];

    return result;

}
uint32_t _packlh2   (uint32_t src1, uint32_t src2)
{
    uint32_t result;
    ((uint16_t *)(&result))[0] =  ((uint16_t *)(&src2))[1];

    ((uint16_t *)(&result))[1] =  ((uint16_t *)(&src1))[0];
    return result;
}
#ifdef E264_NAL_NEW_003
#pragma FUNC_CANNOT_INLINE(D264_GetNal)
#pragma CODE_SECTION(D264_GetNal,".H264Dec_L2_Code_GetMBData")
_INLINE_ void D264_GetNal(BITSTREAM *bs,uint32_t iPrevData,uint32_t iData)
{
    uint8_t* naluBuffer = bs->pCurrent-4;
    int nBsLength = 0;
    int i;
    int count;
    uint8_t *pStart = (uint8_t *)(&iData);
    uint8_t data;

    count = (!(iPrevData&0x000000ff)+!(iPrevData&0x0000ffff));
    //  count = (iPrevData&0xff000000) ? 0:count;

    for(i=0; i < 4; i++)
    {
        data = naluBuffer[nBsLength++];
        (count == ZEROBYTES_SHORTSTARTCODE && (data == 0x03)) ? nBsLength++ : 0;
        count = count == ZEROBYTES_SHORTSTARTCODE ? 0:count;
        pStart[i] = naluBuffer[nBsLength-1];
        count = data == 0 ? count+1:0;
    }
    iData = _packlh2(iData,iData);
    iData = _swap4(iData);

    bs->DataB = iData;


    bs->pCurrent+=nBsLength-4;
}
#endif

int _cmpgtu4(int src1,int src2)
{
    int result;
    int i;

    result = 0;
    for(i=0;i<4;i++)
    {
        result |=  (((uint8_t *)(&src1))[i]>((uint8_t *)(&src2))[i])<<i;
    }
    return result;
}
int _norm(uint32_t val)
{
    int size = 0;
    while(val)
    {
        val>>=1;
        size++;
    }

    return 31-size;
}

#pragma CODE_SECTION(BitstreamShowBits,".H264Dec_L2_Code_GetMBData")
uint32_t  BitstreamShowBits(BITSTREAM * const bs,
                                            const uint32_t bits)
{
    int iPos = bs->iPos;
    uint32_t DataA = bs->DataA;
    uint32_t DataB = bs->DataB;
    int rs = 32 - bits;
    int nbit = (bits + iPos) - 32;
    uint32_t l;


    l = nbit <= 0?(DataA << iPos) >> rs
        :((DataA << iPos) >> rs) | (DataB >> (32 - nbit));

    return l;
}

#pragma CODE_SECTION(D264_BitstreamGetBits,".H264Dec_L2_Code_GetMBData")//Get(0)
static _INLINE_ uint32_t  D264_BitstreamGetBits(BITSTREAM * const bs,
                                                const uint32_t bits)
{
    int iPos = bs->iPos;
    uint32_t DataA = bs->DataA;
    uint32_t DataB = bs->DataB;
    int nbit = (bits + iPos) - 32;
#if 1
    uint32_t l = nbit <= 0?(DataA << iPos) >> (32 - bits)
        :((DataA << iPos)>>(32-bits)) | (DataB >> (32 - nbit));
#else
    uint32_t l = nbit <= 0?(uint64_t)(DataA << iPos) >> (32 - bits)
        :((uint64_t)(DataA << iPos)>>(32-bits)) | (uint64_t)(DataB >> (32 - nbit));

#endif
    iPos += bits;
    if (iPos >= 32)
    {
        uint32_t DataA = bs->DataA;
        uint32_t DataB = bs->DataB;
        int data;// = *(int*)(bs->pCurrent);
        memcpy(&data, bs->pCurrent, sizeof(int));
        iPos -= 32;
        DataA = DataB;

        data = _packlh2(data,data);
        DataB = _swap4(data);

        bs->pCurrent+=sizeof(int);
        bs->DataA = DataA;
        bs->DataB = DataB;

#ifdef E264_NAL_NEW_003
        {
            int c0,c1;

            c0=_cmpgtu4(0x00000101,DataA);
            c1=_cmpgtu4(0x01010100,DataB);

            if(_bitc4(c0)+_bitc4(c1)>=2)
            {
                D264_GetNal(bs,DataA,DataB);
            }
        }
#endif
    }
    bs->iPos = iPos;
    return l;
}

#pragma CODE_SECTION(D264_BitstreamGetBitsNotInline,".H264Dec_L2_Code")
uint32_t  D264_BitstreamGetBitsNotInline(BITSTREAM * const bs,
                                         const uint32_t bits)
{

    return D264_BitstreamGetBits(bs,bits);
}

#pragma CODE_SECTION(D264_linfo_ue,".H264Dec_L2_Code_GetMBData")
_INLINE_ int32_t D264_linfo_ue(int32_t len, int32_t info)
{
    return ((1 << (len >> 1)) + info - 1);
}
#pragma CODE_SECTION(D264_linfo_se,".H264Dec_L2_Code_GetMBData")
_INLINE_ int32_t D264_linfo_se(int32_t len,  int32_t info)
{
    int32_t n;
    int32_t value;
    n = (1 << (len >> 1)) + info - 1;
    value = (n + 1) >> 1;
    return ( (n & 0x01) == 0 ? -value : value);// lsb is signed bit
}

#pragma CODE_SECTION(D264_get_VLC_symbol,".H264Dec_L2_Code_GetMBData")
int32_t D264_get_VLC_symbol (BITSTREAM *bs, int32_t *info)
{
    int32_t ctr_bit;
    uint32_t tmp;
    int len = 0;
    int a;

    //  int lennew = 0;
    int code = BitstreamShowBits(bs,16);

    len = _norm(code) + 1 - (32-16);

#if 0
    BitstreamSkip(bs,len+1);


    *info = D264_BitstreamGetBitsLenZero(bs,len);
#else
    *info = D264_BitstreamGetBits(bs,len*2+1) & ((1<<len)-1);

#endif

    return ((len<<1) + 1);

}

#pragma CODE_SECTION(D264_ue_v,".H264Dec_L2_Code")
int32_t D264_ue_v (BITSTREAM *bs)
{
    int32_t info;
    int32_t sym_len;
    int32_t val;

    sym_len =  D264_get_VLC_symbol (bs, &info);

    val = D264_linfo_ue(sym_len, info);

    return val;
}

#pragma CODE_SECTION(D264_se_v,".H264Dec_L2_Code")
int32_t D264_se_v (BITSTREAM *bs)
{
    int32_t info;
    int32_t sym_len;
    int32_t val;

    sym_len =  D264_get_VLC_symbol (bs, &info);
    val = D264_linfo_se(sym_len, info);
    return val;
}

