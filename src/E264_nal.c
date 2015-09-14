#include "E264_nal.h"


#pragma FUNC_CANNOT_INLINE(D264_RBSP_to_SODB);
#pragma CODE_SECTION(D264_RBSP_to_SODB,".H264Dec_SDRAM_Code")
int32_t D264_RBSP_to_SODB(uint8_t *pStreamStart, int32_t last_byte_pos)
{
    int32_t ctr_bit, bitoffset = 0;
    //find trailing 1 bits
    ctr_bit = (pStreamStart[last_byte_pos-1] & (0x01 << bitoffset));
    while (ctr_bit == 0)
    {
        bitoffset++;
        if(bitoffset == 8)
        {
#ifdef _PRINT_DEBUG_INFO
            if(last_byte_pos == 0)
            {
                printf(" Panic: All zero data sequence in RBSP \n");
            }
#endif
            last_byte_pos -= 1;
            bitoffset = 0;
        }
        ctr_bit = pStreamStart[last_byte_pos - 1] & (0x01 << (bitoffset));
    }
    return(last_byte_pos);
}


#ifndef E264_NAL_NEW_003
#pragma FUNC_CANNOT_INLINE(D264_EBSP_to_RBSP);
#pragma CODE_SECTION(D264_EBSP_to_RBSP,".H264Dec_SDRAM_Code")
int32_t D264_EBSP_to_RBSP(uint8_t *nalu_buf, uint8_t *bs_buffer, int32_t end_bytepos, int32_t begin_bytepos)
{
    int32_t i, j, count = 0;
    int32_t tmp;
//  int64_t *pDst64 = (int64_t *)bs_buffer;
    int bFind03 = 0;
    int trid;

    if(end_bytepos < begin_bytepos)
    {
        return end_bytepos;
    }
/*  offset = ((int32_t)(nalu_buf+begin_bytepos) & 7);
    pSrc64 = nalu_buf+begin_bytepos - offset;


#ifdef _ETI_
    for(j = 0, i = begin_bytepos; i < end_bytepos-16; i+=8)
    {
        ii = i>> 3;
        s0 = pSrc64[ii];
        s1 = pSrc64[(ii)+1];
        tmp64 = hmpv_alignr_p8(s1,s0,offset);
        if(!hmpv_ceq_pu8(tmp64,0x0303030303030303))
        {
            pDst64[j>>3] = tmp64;
            j+=8;
        }
        else
        {
            count = 0;
            if(i-3 > begin_bytepos)
            {
                if(nalu_buf[i-3] == 0)
                    count++;
            }
            if(i-2 > begin_bytepos)
            {
                if(nalu_buf[i-2] == 0)
                    count++;
            }
            if(i-1 > begin_bytepos)
            {
                if(nalu_buf[i-1] == 0)
                    count++;
            }

            for(k = 0;k+i < end_bytepos-8;k++)
            {
                tmp = nalu_buf[i+k];
                if(tmp != 0x03)
                {
                    bs_buffer[j++] = tmp;
                    count = tmp == 0 ? count + 1 : 0;
                }
                else if(count != ZEROBYTES_SHORTSTARTCODE)
                {
                    tmp = nalu_buf[i+k];
                    bs_buffer[j++] = tmp;
                    count = tmp == 0 ? count + 1 : 0;
                }
                else
                {
                    count = 0;
                }
                if((j & 7) == 0)
                {
                    int32_t offsetbak = offset;
                    i+=k-7;
                    offset+=k-7;
                    //                      offset = ((int32_t)(nalu_buf+i) & 7);
                    if(offset >= 8)
                    {
                        pSrc64 += offset >> 3;
                        offset = offset & 7;
                    }
                    break;
                }
            }
        }
    }

    for(; i < end_bytepos; i++)
    {
        tmp = nalu_buf[i];
        if(tmp != 0x03)
        {
            bs_buffer[j++] = tmp;
            count = tmp == 0 ? count + 1 : 0;
        }
        else if(count != ZEROBYTES_SHORTSTARTCODE)
        {
            tmp = nalu_buf[i];
            bs_buffer[j++] = tmp;
            count = tmp == 0 ? count + 1 : 0;
        }
        else
        {
            count = 0;
        }
    }
    return j;
#else
    {

        for(j = 0, i = begin_bytepos; i < end_bytepos-16; i+=8)
        {
            s0 = pSrc64[i>>3];
            s1 = pSrc64[(i>>3)+1];
            tmp64 = hmpv_alignr_p8(s1,s0,offset);
            if(!hmpv_ceq_pu8(tmp64,0x0303030303030303))
            {
                pDst64[j>>3] = tmp64;
                j+=8;
            }
            else
            {
                count = 0;
                if(i-3 > begin_bytepos)
                {
                    if(nalu_buf[i-3] == 0)
                        count++;
                }
                if(i-2 > begin_bytepos)
                {
                    if(nalu_buf[i-2] == 0)
                        count++;
                }
                if(i-1 > begin_bytepos)
                {
                    if(nalu_buf[i-1] == 0)
                        count++;
                }

                for(k = 0;k+i < end_bytepos-8;k++)
                {
                    tmp = nalu_buf[i+k];
                    if(tmp != 0x03)
                    {
                        bs_buffer[j++] = tmp;
                        count = tmp == 0 ? count + 1 : 0;
                    }
                    else if(count != ZEROBYTES_SHORTSTARTCODE)
                    {
                        tmp = nalu_buf[i+k];
                        bs_buffer[j++] = tmp;
                        count = tmp == 0 ? count + 1 : 0;
                    }
                    else
                    {
                        count = 0;
                    }
                    if((j & 7) == 0)
                    {
                        int32_t offsetbak = offset;
                        i+=k-7;
                        offset+=k-7;
                        //                      offset = ((int32_t)(nalu_buf+i) & 7);
                        if(offset >= 8)
                        {
                            pSrc64 += offset >> 3;
                            offset = offset & 7;
                        }
                        break;
                    }
                }
            }
        }


        for(; i < end_bytepos; i++)
        {
            tmp = nalu_buf[i];
            if(tmp != 0x03)
            {
                bs_buffer[j++] = tmp;
                count = tmp == 0 ? count + 1 : 0;
            }
            else if(count != ZEROBYTES_SHORTSTARTCODE)
            {
                tmp = nalu_buf[i];
                bs_buffer[j++] = tmp;
                count = tmp == 0 ? count + 1 : 0;
            }
            else
            {
                count = 0;
            }
        }
        {
            for(j = 0, i = begin_bytepos; i < end_bytepos; i++)
            {
                tmp = nalu_buf[i];
                if(tmp != 0x03)
                {
                    if(bs_buffer[j++] != tmp)
                        j = j;
                    count = tmp == 0 ? count + 1 : 0;
                }
                else if(count != ZEROBYTES_SHORTSTARTCODE)
                {
                    tmp = nalu_buf[i];
                    if(bs_buffer[j++] != tmp)
                        j = j;
                    count = tmp == 0 ? count + 1 : 0;
                }
                else
                {
                    count = 0;
                }

            }
        }
        return j;
    }
    for(j = 0, i = begin_bytepos; i < end_bytepos; i++)
    {
        tmp = nalu_buf[i];
        if(tmp != 0x03)
        {
            bs_buffer[j++] = tmp;
            count = tmp == 0 ? count + 1 : 0;
        }
        else if(count != ZEROBYTES_SHORTSTARTCODE)
        {
            tmp = nalu_buf[i];
            bs_buffer[j++] = tmp;
            count = tmp == 0 ? count + 1 : 0;
        }
        else
        {
            count = 0;
        }

    }
    return j;
#endif*/
    for(j = 0, i = begin_bytepos; i < end_bytepos; i++)
    {
        tmp = nalu_buf[i];
        count = (tmp  != 0x03 || count != ZEROBYTES_SHORTSTARTCODE) ? bs_buffer[j++] = tmp ,(!tmp ) * (count+1) : 0;
    }

    return j;
}
#endif

#pragma FUNC_CANNOT_INLINE(D264_find_start_code);
#pragma CODE_SECTION(D264_find_start_code,".H264Dec_SDRAM_Code")
int32_t D264_find_start_code (uint8_t *buf, int32_t zeros_in_startcode)
{
    int32_t i;
    int32_t info;
    info = 1;
    for (i = 0; i < zeros_in_startcode; i++)
    {
        if(buf[i] != 0)
        {
            info = 0;
        }
    }
    if(buf[i] != 1)
    {
        info = 0;
    }
    return info;
}

#pragma FUNC_CANNOT_INLINE(D264_more_rbsp_data);
#pragma CODE_SECTION(D264_more_rbsp_data,".H264Dec_SDRAM_Code")
int32_t D264_more_rbsp_data (uint8_t buffer[],int32_t totbitoffset,int32_t bytecount)
{
    int32_t ctr_bit, byteoffset, bitoffset;
    // there is more until we're in the last byte
    byteoffset = totbitoffset >> 3;
    if (byteoffset < (bytecount-1))
    {
        return TRUE;
    }
    else if (byteoffset >= (bytecount))
    {
        return FALSE;
    }
    else
    {
        // read one bit
        bitoffset  = 7 - (totbitoffset & 7);
        ctr_bit = (buffer[byteoffset] & (0x01 << bitoffset));
        // a stop bit has to be one
        if (ctr_bit == 0)
        {
            return TRUE;
        }
        else
        {
            bitoffset--;
            while (bitoffset >= 0)
            {
                ctr_bit = (buffer[byteoffset] & (0x01 << bitoffset));   // set up control bit
                if (ctr_bit > 0)
                {
                    return TRUE;
                }
                bitoffset--;
            }
            return FALSE;
        }
    }
}

