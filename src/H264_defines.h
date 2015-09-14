

#ifndef H264_DEFINE_HEADFILE
#define H264_DEFINE_HEADFILE

#define MBNUM_ROW 8 //解码器可以不对齐,编码器现在必须要对齐注意测试
#define   STATUS_NUM  8
#define FLT_EDGE 8

#define FLTBUF_WIDTH ((STATUS_NUM)*16)
#define FLTBUF_WIDTH_CR ((STATUS_NUM)*8)
#define FLTBUF_STRIDE (FLTBUF_WIDTH+FLT_EDGE)
#define FLTBUF_STRIDE_CR (FLTBUF_WIDTH/2+FLT_EDGE)
#define FLTBUF_STRIDE_UV (FLTBUF_STRIDE_CR*2)

#define I4PREDMODE_EDGE 8
#define I4PREDMODE_WIDTH (MBNUM_ROW*4)
#define I4PREDMODE_STRIDE (I4PREDMODE_EDGE+I4PREDMODE_WIDTH)


#define REFMBINTRA_LEN_INT 24


#define MBWIDTH 16
#define MBHEIGHT 16

#define MBWIDHT_CR 8
#define MBHEIGHT_CR 8

#define REFMB_XDIM 80
#define REF_XDIM       (REFMB_XDIM+(MBNUM_ROW-1)*16)//56
#define REF_YDIM 64//必须
#define REF_XDIM_CR     REF_XDIM/2
#define REF_YDIM_CR     REF_YDIM/2
#define REF_EDGE_H     (REF_XDIM-CUR_XDIM)/2//19
//#define REF_EDGE_H_SEQ     (REF_XDIM_SEQ-CUR_XDIM_SEQ)/2//19
#define REF_EDGE_V     (REF_YDIM-MBHEIGHT)/2//19 4

#define REF_EDGE_H_CR     REF_EDGE_H/2//19 4
#define REF_EDGE_V_CR     REF_EDGE_V/2//19 4

#define CUR_XDIM (MBWIDTH*MBNUM_ROW)
#define CUR_XDIM_CR MBWIDTH/2*MBNUM_ROW
#define CUR_YDIM MBHEIGHT
#define CUR_YDIM_CR MBHEIGHT/2

#define MBROWSIZE       CUR_XDIM * CUR_YDIM
#define MBROWSIZE_CR CUR_XDIM_CR*CUR_YDIM_CR

#define SEARCH_ROWMB_WINDOW_SIZE    REF_XDIM * REF_YDIM



// 16x16 intra prediction modes
#define VERT_PRED_16    0
#define HOR_PRED_16     1
#define DC_PRED_16      2
#define PLANE_16        3

/* 4x4 intra prediction modes */
#define VERT_PRED             0
#define HOR_PRED              1
#define DC_PRED               2
#define DIAG_DOWN_LEFT_PRED   3
#define DIAG_DOWN_RIGHT_PRED  4
#define VERT_RIGHT_PRED       5
#define HOR_DOWN_PRED         6
#define VERT_LEFT_PRED        7
#define HOR_UP_PRED           8

// 8x8 chroma intra prediction modes
#define DC_PRED_8       0
#define HOR_PRED_8      1
#define VERT_PRED_8     2
#define PLANE_8         3

//used in mv predictor
#define MVPRED_MEDIAN   0
#define MVPRED_L        1
#define MVPRED_U        2
#define MVPRED_UR       3


//used in iquant
#define MIN_QP                          0
#define MAX_QP                          51
#define MAX_QP64                        0x0033003300330033
#define QP_SIZE                         52
#define DQ_BITS                         6
#define DQ_ROUND                        (1<<(DQ_BITS-1))
#define DQ_ROUND2                       ((DQ_ROUND<<16) + DQ_ROUND)


#define MAX_VALUE       999999  //!< used for start value for some variables

#define NBYTEALIGN 32
#define NBYTEALIGN8 8
#define NBYTEALIGN16 16



typedef enum{
    PSKIP = 0,
        INTER16x16,  //1
        INTER16x8,   //2
        INTER8x16,   //3
        INTER8x8_NONE,    //4
        INTER8X4_NONE,    //5
        INTER4x8_NONE,    //6
        INTER4x4_NONE,    //7
        P8x8_NONE,       //8
        I4MB,       //9
        I16MB,      //10
        IPCM_NONE       //11
}MB_MODE;

//! image type
typedef enum
{
    P_SLICE = 5,
        B_SLICE,
        I_SLICE,
        SP_SLICE,
        SI_SLICE,
        FORCED_SKIP
}   SLICE_TYPE;


//nalu type
#define NALU_TYPE_SLICE    1
#define NALU_TYPE_DPA      2
#define NALU_TYPE_DPB      3
#define NALU_TYPE_DPC      4
#define NALU_TYPE_IDR      5
#define NALU_TYPE_SEI      6
#define NALU_TYPE_SPS      7
#define NALU_TYPE_PPS      8
#define NALU_TYPE_PD       9
#define NALU_TYPE_EOSEQ    10
#define NALU_TYPE_EOSTREAM 11
#define NALU_TYPE_FILL     12

#define NALU_PRIORITY_HIGHEST     3
#define NALU_PRIORITY_HIGH        2
#define NALU_PRIRITY_LOW          1
#define NALU_PRIORITY_DISPOSABLE  0

//--------------------------
// define for vlc
//---------------------------
#define LUMA              0
#define LUMA_INTRA16x16DC 1
#define LUMA_INTRA16x16AC 2
#define CHROMA_DC         6
#define CHROMA_AC         7



static _INLINE_ int median3(int a0, int a1, int a2)
{
    int t1, t2;

    t1 = (a1 > a2)?a1:((a0 > a2)?a2:a0);
    t2 = (a0 > a2)?a0:((a1 > a2)?a2:a1);
    return (a0 > a1)?t1:t2;
}

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define CLIP1(x)  (MAX(0, MIN(x, 255)))

#ifndef CHIP_DM6446
#define ABS(x)    (((x)<0)? (-(x)) : (x))
#else
#define ABS _abs
#endif
// #endif


#define SWAP(a,b) {void *tmp = (void*)(a);a=(void*)(b);b=tmp;}

#define SWAP3(a,b,c) {void *tmp = (void*)(a);a=(void*)(b);b=(void*)(c);c=tmp;}

// #define IS_NEWINTRA(m_MbMode) (m_MbMode==I16MB)
// #define IS_INTRA(m_MbMode)    (m_MbMode!=I4MB || m_MbMode==I16MB)
// #define IS_INTER(m_MbMode)    (m_MbMode!=I4MB && m_MbMode!=I16MB)
// #define IS_INTERMV(m_MbMode)  (m_MbMode!=I4MB && m_MbMode!=I16MB  && m_MbMode!=0)
// #define NEW_ANY_INTRA        (mb_p_type==I16MB ||mb_q_type==I16MB)

#if defined(NANGWANG_H264_DECODER)
#define IS_NEWINTRA(mb_mode) (mb_mode==I16MB)
#define IS_INTRA(mb_mode)    (mb_mode==I16MB)
#define IS_INTER(mb_mode)    (mb_mode!=I16MB)
#define IS_INTERMV(mb_mode)  (mb_mode!=I16MB  && mb_mode!=0)
#define NEW_ANY_INTRA        (mb_p_type==I16MB ||mb_q_type==I16MB)
#else
#define IS_NEWINTRA(mb_mode) (mb_mode==I16MB)
#define IS_INTRA(mb_mode)    (mb_mode >= I4MB)//(mb_mode==I4MB   || mb_mode==I16MB)
#define IS_INTER(mb_mode)    (mb_mode < I4MB)//(mb_mode!=I4MB  && mb_mode!=I16MB)
#define IS_INTERMV(mb_mode)  (IS_INTER(mb_mode) && mb_mode!=0)
#define NEW_ANY_INTRA        (mb_p_type==I4MB ||mb_p_type==I16MB ||mb_q_type==I4MB ||mb_q_type==I16MB)
#endif

#ifdef CHIP_DM6446
#define  ICLIP(minval, maxval, val) (short)(_min2(_max2(minval,val),maxval))
#else
#define  ICLIP(minval, maxval, val) (((val)<(minval))? (minval):(((val)>(maxval))? (maxval):(val)))
#endif


#define ALYGNN(val,n) (((val)+n-1)/n*n)


#define INTRAREFBUFWIDTH 24
#define INTRAREFBUFUVWIDTH 16
#define INTRAREFBUFYLEN (INTRAREFBUFWIDTH*17)
#define INTRAREFBUFUVLEN (INTRAREFBUFUVWIDTH*16)

#endif //H264_DEFINE_HEADFILE

