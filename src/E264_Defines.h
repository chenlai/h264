#ifndef  _D264_DEFINES_H_
#define  _D264_DEFINES_H_
 
#include "E264_BaseType.h"
#include "H264_defines.h"





//#define MAXSPS  1
//#define MAXPPS  1
// should be sufficient for HUGE pictures, need one int per MB in a picture,this value is very large!!!
//#define SIZE_SLICE_GROUP_ID    (sizeof(int32_t) * 60000)
//size of block and mb



// 
// #define MAXRBSPSIZE 64000
// #define MAX_NUM_DEC_REF_PIC_MARKING_BUFFER 10
#define MAX_NUM_SLICE_GROUPS_MINUS1  8
#define MAX_NUM_REF_FRAMES_IN_PIC_ORDER_CNT_CYCLE  255
#define MAX_CODED_PACKET_SIZE (1920*1080*3/2/4)        //!< max bytes for one slice£¬this value is very large
//Start code and Emulation Prevention need this to be defined in identical manner at encoder and decoder
#define ZEROBYTES_SHORTSTARTCODE 2 //indicates the number of zero bytes in the short start-code prefix


//macroblock type
//ÏÂÃæµÄÊýÓ¦´óÓÚ3£¬copy,16x16,16x8,8x18Õ¼¾ÝÁËÇ°ÃæµÄ4¸ö



// #define IS_OLDINTRA(mb_mode) (mb_mode==I4MB)
// #define IS_P8x8(mb_mode)     (mb_mode==INTER8x8_NONE)



#define	END_XPS          0          //!<end of PIC_PARAMETER_SET/SEQ_PARAMETER_SET
#define EOS              1          //!<end of sequence
#define SOP              2          //!<start of picture
#define SOS              3          //!<start of slice
//#define	PARAM_ERR       -1          //!<error
#define NOT_SUPPORTED   -2          //!<can't supported parameter
#define	PARAM_ERR       -3          //!<error

#define PIC_HEADER_LOST  1
#define PIC_WAIST_LOST   2
#define PIC_TAIL_LOST    3 
#define FRAME_LOST       4

#define DECODING_OK     0
#define SEARCH_SYNC     1
#define PICTURE_DECODED 2

#define FREXT_HP        100      //!< YUV 4:2:0/8 "High"
#define FREXT_Hi10P     110      //!< YUV 4:2:0/10 "High 10"
#define FREXT_Hi422     122      //!< YUV 4:2:2/10 "High 4:2:2"
#define FREXT_Hi444     144      //!< YUV 4:4:4/12 "High 4:4:4"


//#define NO_INTRA_PMODE  9        //!< #intra prediction modes
/* 4x4 intra prediction modes */
// #define VERT_PRED             0
// #define HOR_PRED              1
// #define DC_PRED               2
// #define DC_PRED64             0x0002000200020002
// #define DIAG_DOWN_LEFT_PRED   3
// #define DIAG_DOWN_RIGHT_PRED  4
// #define VERT_RIGHT_PRED       5
// #define HOR_DOWN_PRED         6
// #define VERT_LEFT_PRED        7
// #define HOR_UP_PRED           8


//D264_bIsEndOfSlice µÄ·µ»ØÖµ
#define NOENDOFSLICE 0
#define ENDOFSLICE   1
#define ENDOFPIC     2


#define DIV_MBWIDTH_TO_MUL_PRECI 18

#define POW_2_DIV_MBWIDTH_TO_MUL_PRECI 0x40000




#endif

