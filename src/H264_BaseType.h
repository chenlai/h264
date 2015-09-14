

#ifndef H264_BASETYPE_HEADFILE
#define H264_BASETYPE_HEADFILE


/*
 * basic types: according to C99 <stdint.h>, <stdbool.h>
 */
#ifndef NULL
  #ifdef  __cplusplus
    #define NULL    0
  #else
    #define NULL    ((void *)0)
  #endif
#endif

typedef char int8_t; 
typedef unsigned char uint8_t;
typedef short int16_t; 
typedef unsigned short uint16_t; 
typedef int int32_t;
typedef unsigned int uint32_t;


/*
 * Definitions for different environments
 */
#ifdef WIN32
  #define _INLINE_ __inline 
#else
  #define _INLINE_ inline
#endif


#ifndef CHIP_DM6446
#define far
#define near
#endif


  
#ifdef CHIP_DM6446
#define ENABLE_EDMA3
#define ENABLE_EDMA3_SEQ
#define ENABLE_EDMA3_PICEXT
#define ENABLE_IDMA

#ifndef CHIP_DM6446
#define CHIP_DM6446
#define CHIP_DM6467
#endif
  
#endif

//#define CHROMA_ONLY_DC_PRED

//#define INTRAMB_ONLY_UPDC_PRED //仅仅用上边块作dc预测



//把编码解码专用的宏区分开来
//////////////////////////////////////////////////////////////////////////
//编码控制宏
#define E264_NUM_MB_ME_SEQ 1
  
#define E264_ENABLE_IMX
  
//#define E264_SIMU_IMX
#ifdef SIMU
#define E264_SIMU_IMX
#endif
  
//#define E264_SEQ_DEBUG
  
//#define E264_ENABLE_CHECKSKIP


#define E264_ENABLE_NAL_003

//#define ENABLE_SMALL_NAL
  
#define E264_PFRAME_INTRA_ENABLE

//#define E264_DEC_GET_STRENGTH
 
#define E264_DSP_QUARTER_PIXEL


#define ENABLE_QUAL_PIXEL
//#define ENABLE_HALF_HV_MV


#define E264_DSP_HALF_PIXEL

//半像素用一半的sad
#define HALFPIXEL_HALF_SAD

//#define SEQ_CYCLE_PRINT


//////////////////////////////////////////////////////////////////////////
//解码控制宏
#define E264_NAL_NEW_003

//////////////////////////////////////////////////////////////////////////


#ifdef CHIP_DM648
#define CHIP_DM648_IO
#endif
//调试用
extern int nFrame;
// #ifdef CHIP_DM648_IO
// #include "ti_cci_dsp.h"
// #include "DM642_cycle.h"
// #else
// #include "DavinIo.h"
// #endif
// #include "dsp_comm.h"

#endif //BASETYPE_HEADFILE

