#ifndef _DECUNION_H_
#define _DECUNION_H_

#include "NwCodec.h"

typedef void* HDecoder;

typedef struct RECFRAME_s
{
	unsigned char *pY; //亮度Y分量
	unsigned char *pU; //色度U分量
	unsigned char *pV; //色度V分量
	int width;         //解码图像宽
	int height;        //解码图像高
	int stride;        //解码图像跨度
	int bIS_StillImage;//是否是静态图文
	int stillimage_id; //中静态图文传出后的id
	union recparam_u
	{
		struct H263_RecParam_s
		{
			/////////////////////////////////////////////////////////////////////////
			int H263_Annex_A:1;  //是否有Annex A可选项
			int H263_Annex_B:1;  //同上
			int H263_Annex_C:1;  //.............
			int H263_Annex_D:1;
			int H263_Annex_E:1;
			int H263_Annex_F:1;
			int H263_Annex_G:1;
			int H263_Annex_H:1;
			//h263的可选项
			
			//////////////////////////////////////////////////////////////////////////
			int H263_ADD_Annex_I:1;
			int H263_ADD_Annex_J:1;
			int H263_ADD_Annex_K:1;
			int H263_ADD_Annex_L:1;
			int H263_ADD_Annex_M:1;
			int H263_ADD_Annex_N:1;
			int H263_ADD_Annex_O:1;
			int H263_ADD_Annex_P:1;
			int H263_ADD_Annex_Q:1;
			int H263_ADD_Annex_R:1;
			int H263_ADD_Annex_S:1;
			int H263_ADD_Annex_T:1;
			//H263+可选项
			
			//////////////////////////////////////////////////////////////////////////
			int H263_ADD_Annex_U:1;
			int H263_ADD_Annex_V:1;
			int H263_ADD_Annex_W:1;
			//h263++可选项
			int reserved:9;//保留参数
		}H263_RecParam;
		struct H261_RecParam_s
		{
			int reserved;  //保留参数
		}H261_RecParam;
		struct Mp4v_RecParam_s
		{
			int bhaveWatermark;//当前帧是否带水印
		}Mp4v_RecParam;
		struct SPS_RecParam_s
		{
			int reserved;  //保留参数
		}SPS_RecParam;
	}RecParamUnion;
}RECFRAME;

typedef void(*OutputRec)(void *m_HandleCallBack, const RECFRAME  *pRecFrame);


typedef struct DEC_INIT_PARAM_s
{
	OutputRec displayImage;   //显示回调
	void *m_HandleCallBack;           //上层处理显示部分的handle,可以为NULL
	MEMORY SDRAM;             //片外的内存指针
	MEMORY L2;				  //片内L2, 在编码解码初始化之后一直占用的内存。
	MEMORY L2Tmp;			  //片内L2, 在编码解码每一帧的时候临时用到的内存,因而在多路的时候能复用这个内存.
	int  bDecPacket;           //是解帧还是解报
	int  bEnableDecStillImage; //是否解码静态图文
	union decinit_u
	{
		struct Mp4v_DecInit_s
		{
			char *pWaterMarkData;       //水印信息,9*9大小,必须和相应的编码端一致,如果为NULL就不检测水印,初始化之后,可以释放掉
			int   bOutSideAlloc;        //是否在在外面分配内存,只对tetra平台的mpeg4解码器起作用
			void *pMem;	                // 由外部分配好传给编码起的起始地址,只对tetra平台的mpeg4解码器起作用
			int   nMemLength;           // 分配的长度,只对tetra平台的mpeg4解码器起作用
		}Mp4v_DecInit;
		struct H263_DecInit_s
		{
			int reserved0;//保留参数
		}H263_DecInit;
		struct H261_DecInit_s
		{
			int stream_type;//保留参数
		}H261_DecInit;
		struct SPS_DecInit_s
		{
			unsigned char *pInitMem;
			unsigned int   InitMemLength;      
			int reserved0;//保留参数
		}SPS_DecInit;
	}DecInitUnion;
	
} DEC_INIT_PARAM;

typedef struct DECFRAME_s
{
	unsigned char *pInPacket; //输入的视频包,如果是解帧,就是输入一帧的数据.必须4字节对齐.
	int nLength;              //输入视频包的长度
	int bIs_LostPackage;      //是否丢包
	union decparam_u
	{
		struct H264_DecParam_s
		{
			unsigned char *pRecImgY421;//保留参数
			unsigned char *pRecImgC421;//保留参数
			int nRec421Stride;
		}H264_DecParam;
		struct Mp4v_DecParam_s
		{
			int   bIs_LAST_Package;     //是否是最后一个包
			int   bIs_postprocess;      //是否要加后处理
		}Mp4v_DecParam;
		struct H263_DecParam_s
		{
			int reserved0;//保留参数
			int reserved1;//保留参数
		}H263_DecParam;
		struct H261_DecParam_s
		{
			int reserved0;//保留参数
			int reserved1;//保留参数
		}H261_DecParam;
		struct SPS_DecParam_s
		{
			int reserved0;//保留参数
			int reserved1;//保留参数
		}SPS_DecParam;
	}DecParamUnion;
}DECFRAME;

#endif

