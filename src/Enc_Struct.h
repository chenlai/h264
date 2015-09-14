#ifndef _ENC_UNION_H_
#define _ENC_UNION_H_



#include "NwCodec.h"

typedef void* HEncoder;

typedef struct
{
	unsigned char * pOutPacket;//编码输出包的指针,如果是不打包,那么就是帧的指针
	int nLength;               //包长度
	int last;                  //是否是最后一个包
	int bIntraFrame;           //是否是Intra帧的包
	union EncInfo_u
	{
		struct H264EncInfo_s
		{
			unsigned char *pFrameSAD; //在一帧最后一个包的时候,
									   //输出一帧所有的SAD给运动检测,在最后一个sad的后面输出上一帧的量化系数,如果为I帧或跳帧pFrameSAD == NULL
		}H264EncInfo;
		
		struct SpsvEncInfo_s
		{
			int * pFrameSAD; //如果需要运动检测,则pFrameSAD=编码器内部分配的记录sad的数组首地址,
			                 //输出每个宏块的MinSad;否则pFrameSAD=NULL
		}SpsvEncInfo;
		struct Mp4vEncInfo_s
		{
			unsigned char * pFrameSAD; //在一帧最后一个包的时候,
									   //输出一帧所有的SAD给运动检测,在最后一个sad的后面输出上一帧的量化系数,如果为I帧或跳帧pFrameSAD == NULL
		}Mp4vEncInfo;
		struct Vc1EncInfo_s
		{
			unsigned char * pFrameSAD; //在一帧最后一个包的时候,
									   //输出一帧所有的SAD给运动检测,在最后一个sad的后面输出上一帧的量化系数,如果为I帧或跳帧pFrameSAD == NULL
		}Vc1EncInfo;
		struct H263EncInfo_s
		{
			int reserved;
		}H263EncInfo;
		struct H261EncInfo_s
		{
			int reserved;
		}H261EncInfo;
	}EncInfoUnion;
}PACKET;
typedef	void(*OutPacketF)(void *hPktProcessor, const PACKET *pPacket);
typedef	void(*PreProcessF)(void *hPktProcessor);

/*
 * modified by zhanglp, for h264_enc,
 * 增加了一个输入参数int width
*/
typedef void(*OutputRecFrame)(unsigned char *Y, unsigned char *U, unsigned char *V,
							  int stride, int height, int width);//编码重建帧输出回调,h263里用到

/*
 *	add by zhanglp, for h264_enc
 *  为节省编码开销，直接用扩边的图像和输入图像计算SNR
 */
//typedef void(*CalculatePSNR)(unsigned char *RecFrame, unsigned char *OrigiFrame,
//							 int nWidth, int nHeight);

enum SRCFORMAT{FORBIDDEN = 0, SUBQCIF, QCIF, CIF, CIF4, CIF16};

//编码参数
typedef struct ENC_INIT_PARAM_s
{
	int   Max_Key_Interval;    // Intra 帧间隔.
	OutPacketF OutPacketFunc;  // 回调函数指针
	void *hPktProcessor;	   // 上层对码流的处理handle，可以为NULL
	int   Target_BitRate;	   // 编码目标比特率 bits/second
	int   Src_FrameRate;       // 输入视频源的采样帧率
	MEMORY SDRAM;              // 片外的内存指针
	MEMORY L2;				   // 片内L2, 在编码解码初始化之后一直占用的内存。
	MEMORY L2Tmp;			   // 片内L2, 在编码解码每一帧的时候临时用到的内存,因而在多路的时候能复用这个内存.
	int   Max_Quantizer;       // 编码使用的最大量化系数
	int   Min_Quantizer;       // 编码使用的最小量化系数
	int   YStride;             // 输入图像亮度分量Y的跨度           
	int   UStride;			   // 输入图像色度分量U的跨度
	int   VStride;			   // 输入图像色度分量V的跨度
	int   MaxPacketLen;        // 包的最大长度
	short ImageQuality;		   // 图像质量调节参数,0 图像质量最差但最流畅,4 图像质量最好,但最不流畅
	short   Quality;          //Quality的意义是:
				//Quality = 0: 使用固定量化，Min_Quantize作为固定QP值。
				//Quality = 1: 此种模式下，码率优先，网络延时不高于0.5s，同时尽量保证帧率，但对图像质量没有要求。
				//Quality = 2: 此种模式下，帧率优先，也就是尽量不跳帧，图像比较流畅，图像质量较好，最大延时2s。
				//Quality = 3: 此种模式下，码率优先，网络延时不高于0.5s，对图像质量要求较高，但不能保证帧率，帧率高于20帧。
				//Quality = 4: 此种模式下，图像质量优先，网络延时不高于1s，不能保证帧率，每秒帧率高于15帧。
				//Quality = 5: 此种模式下，图像质量最好，网络延时不高于2s，不能保证帧率，每秒帧率高于7帧。
				//Quality = 7: 此种模式下，图像质量最好，网络延时不高于0.8s，不能保证帧率,专门给会议定制。
				//Quality = 8: 和quality=2类似,只是最小量化值是5.可以降低静止图像的码流,就是动态码率
				//Quality = 256: 和Quailty = 0类似,只是变为隔一帧跳一帧,使用固定量化，内部根据Target_BitRate设置的值和图像大小计算一个固定QP值。
				//Quality = 257: 和Quailty = 1类似,只是变为隔一帧跳一帧,此种模式下，码率优先，网络延时不高于0.5s，同时尽量保证帧率，但对图像质量没有要求。
				//Quality = 258: 和Quailty = 2类似,只是变为隔一帧跳一帧,此种模式下，帧率优先，也就是尽量不跳帧，图像比较流畅，图像质量较好，最大延时2s。
 	int   bEncPacket;          // 是否对压缩码流打包（1:打, 0:不打）mp4v不支持打包,回调出来的是一帧
	                           // 对于H264, 1:RTP, 0:AnnexB
	unsigned char *pStillImage;// 指向静态图文数据,如果需要编码静态图文,这个指针又使NULL,则编码函数返回错误码
	union EncParam_u
	{
		//h264_enc interface, add by zhanglp, 
		//注释掉的参数为扩展参数，后期编码期的改进中有可能用到
		struct AVSEncParam_s
		{
			unsigned int     Width;         //宽度
			unsigned int     Height;        //高度
			int loop_filter_disable;		//是否作环路滤波

			int SeinitialQP;                //编码的量化系数,在没有码率控制的时候起作用

			OutputRecFrame DisplayRecFunc;	//显示重构帧的回调
			int   bEnableMotionDetect;      //是否需要运动检测
			int   TargetFrameCycle;			//每帧的目标编码cycle
		}AVSEncParam;
		struct H264EncParam_s
		{		 
			int qp0;           //I帧的量化系数.在没有码率控制的时候起作用      
			int qpN;           //P帧的量化系数.在没有码率控制的时候起作用      
			int jumpd;         //跳多少帧编码一帧 (e.g 2 takes frame 0,3,6,9...)
			  
			int img_width;                // 图像的宽度(176,704,352等)必须是16的倍数
			int img_height;               //图像高度(144,576,288等)必须是16的倍数
	
			int LFSendParameters; //是否传递下面的滤波参数
			int LFDisableIdc;     //0---表示需要做loopfilter；1---不做loopfilter; 2---在slice边界不做loopfilter,其余做
			int LFAlphaC0Offset;  //滤波参数Alpha & C0 offset div. 2, 取值{-6, -5, ... 0, +1, .. +6}
			int LFBetaOffset;     //滤波参数Beta offset div. 2, 取值{-6, -5, ... 0, +1, .. +6}
			
			//! Rate Control on JVT standard 
			int RCEnable;    //是否进行码率控制
			int SeinitialQP; //序列初始QP值
			
			//!< functions call back 
			OutputRecFrame DisplayRecFunc; //显示重构帧的回调
			void*  ComputPSNRFunc;  //[in]计算PSNR
			

			int InterSearch16x16;   //未使用
			int InterSearch16x8;    //未使用
			int InterSearch8x16;    //未使用
			int InterSearch8x8;     //未使用 					   
			int search_range;  //搜索范围，此参数未使用，现编码器内部固定为13
			int slice_mode;               //划分slice的方式, 没有起作用，在编码器内部置为2。按字节长度划分slice。
			int UseConstrainedIntraPred;  //此参数未使用，在编码器内部置为0；!< 0: Inter MB pixels are allowed for intra prediction 1: Not allowed
			int basicunit;   //此参数未使用
			int channel_type;//此参数未使用，置为0
			int   TargetFrameCycle;//每帧的目标编码cycle
			int   bEnableMotionDetect;//是否需要运动检测
			short   bProfileLevel1b; // 1就是1b,0根据下面的profile_level参数来定.
			short   profile_level; //如果为零,就是编码器自动根据码流和图像大小设置合适的profile level,其他值profile_level = profile_level/10 如 12 就是1.2
			PreProcessF PreProcessFunc; //显示重构帧的回调
        }H264EncParam;
		struct SPSEncParam_s
		{
			 unsigned int     Width;                       //宽度
			 unsigned int     Height;                      //高度
             OutputRecFrame   RecFrameFunc;                 // 重构帧回调函数
             unsigned char    *InitMemAddr;                //外部传入的内存块指针
             unsigned int     InitMemLength;               //外部传入的内存块大小
             int   bEnableMotionDetect;                    //是否需要运动检测
		}SPSEncParam;
		struct Mp4vEncParam_s
		{
			short width;		     // 图像的宽度
			short height;		     // 图像高度
			OutputRecFrame DisplayRecFunc; //[in]显示重构帧的回调
			int   rc_period;         // the intended rate control averaging period
			int   rc_reaction_period;// the reaction period for rate control
			int   rc_reaction_ratio; // the ratio for down/up rate control
			int   use_bidirect;	     // use bidirectional coding
			int   deinterlace;	     // fast deinterlace
			int   flags;		     // flags for internal control of encoder
			char *pWaterMarkData;    // 水印信息,9*9大小,相应解码端必须也一致,如果为null,就是不加水印,初始化之后,可以释放掉
			int   bEnableMotionDetect;//是否需要运动检测
			int   cbi;				 // 不起作用
			int   vbi;				  //不起作用
			int   TargetFrameCycle;
			int	  SeinitialQP; //固定量化qp值
		}Mp4vEncParam;
		struct Vc1EncParam_s
		{
			short width;		     // 图像的宽度
			short height;		     // 图像高度
			OutputRecFrame DisplayRecFunc; //[in]显示重构帧的回调
			int   rc_period;         // the intended rate control averaging period
			int   rc_reaction_period;// the reaction period for rate control
			int   rc_reaction_ratio; // the ratio for down/up rate control
			int   use_bidirect;	     // use bidirectional coding
			int   deinterlace;	     // fast deinterlace
			int   flags;		     // flags for internal control of encoder
			char *pWaterMarkData;    // 水印信息,9*9大小,相应解码端必须也一致,如果为null,就是不加水印,初始化之后,可以释放掉
			int   bEnableMotionDetect;//是否需要运动检测
			int   cbi;				 // 不起作用
			int   vbi;				  //不起作用
			int   TargetFrameCycle;
			int	  SeinitialQP; //固定量化qp值
		}Vc1EncParam;
		struct H263EncParam_s
		{
			enum  SRCFORMAT m_SrcFormat;      //编码帧的格式信息（CIF/QCIF）
			int   bCodeGobHead;               //1:编码GOBheader,0:不编码GOBheader(subQCIF、QCIF和CIF起作用) 
			OutputRecFrame DisplayRecFunc; //[in]显示重构帧的回调			
			int   SeinitialQP; //固定量化qp值
		}H263EncParam;
		struct H261EncParam_s
		{
			enum SRCFORMAT m_SrcFormat;       //编码帧的格式信息（CIF/QCIF）
		}H261EncParam;
	}EncParamUnion;
} ENC_INIT_PARAM;

typedef struct ENCFRAME_s
{
	unsigned char * pVideoInBuf; //输入视频帧的指针
	int IntraForced;             //是否强制编码Inra帧
	int CurTime;                 //当前时间戳 单位是毫秒
	int bIs_StillImage;          //是否编码静态图文(如果是静态图文,那么数据直接从初始化参数里的pStillImage指针里取数据.
}ENCFRAME;

#endif

