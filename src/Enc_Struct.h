#ifndef _ENC_UNION_H_
#define _ENC_UNION_H_



#include "NwCodec.h"

typedef void* HEncoder;

typedef struct
{
	unsigned char * pOutPacket;//�����������ָ��,����ǲ����,��ô����֡��ָ��
	int nLength;               //������
	int last;                  //�Ƿ������һ����
	int bIntraFrame;           //�Ƿ���Intra֡�İ�
	union EncInfo_u
	{
		struct H264EncInfo_s
		{
			unsigned char *pFrameSAD; //��һ֡���һ������ʱ��,
									   //���һ֡���е�SAD���˶����,�����һ��sad�ĺ��������һ֡������ϵ��,���ΪI֡����֡pFrameSAD == NULL
		}H264EncInfo;
		
		struct SpsvEncInfo_s
		{
			int * pFrameSAD; //�����Ҫ�˶����,��pFrameSAD=�������ڲ�����ļ�¼sad�������׵�ַ,
			                 //���ÿ������MinSad;����pFrameSAD=NULL
		}SpsvEncInfo;
		struct Mp4vEncInfo_s
		{
			unsigned char * pFrameSAD; //��һ֡���һ������ʱ��,
									   //���һ֡���е�SAD���˶����,�����һ��sad�ĺ��������һ֡������ϵ��,���ΪI֡����֡pFrameSAD == NULL
		}Mp4vEncInfo;
		struct Vc1EncInfo_s
		{
			unsigned char * pFrameSAD; //��һ֡���һ������ʱ��,
									   //���һ֡���е�SAD���˶����,�����һ��sad�ĺ��������һ֡������ϵ��,���ΪI֡����֡pFrameSAD == NULL
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
 * ������һ���������int width
*/
typedef void(*OutputRecFrame)(unsigned char *Y, unsigned char *U, unsigned char *V,
							  int stride, int height, int width);//�����ؽ�֡����ص�,h263���õ�

/*
 *	add by zhanglp, for h264_enc
 *  Ϊ��ʡ���뿪����ֱ�������ߵ�ͼ�������ͼ�����SNR
 */
//typedef void(*CalculatePSNR)(unsigned char *RecFrame, unsigned char *OrigiFrame,
//							 int nWidth, int nHeight);

enum SRCFORMAT{FORBIDDEN = 0, SUBQCIF, QCIF, CIF, CIF4, CIF16};

//�������
typedef struct ENC_INIT_PARAM_s
{
	int   Max_Key_Interval;    // Intra ֡���.
	OutPacketF OutPacketFunc;  // �ص�����ָ��
	void *hPktProcessor;	   // �ϲ�������Ĵ���handle������ΪNULL
	int   Target_BitRate;	   // ����Ŀ������� bits/second
	int   Src_FrameRate;       // ������ƵԴ�Ĳ���֡��
	MEMORY SDRAM;              // Ƭ����ڴ�ָ��
	MEMORY L2;				   // Ƭ��L2, �ڱ�������ʼ��֮��һֱռ�õ��ڴ档
	MEMORY L2Tmp;			   // Ƭ��L2, �ڱ������ÿһ֡��ʱ����ʱ�õ����ڴ�,����ڶ�·��ʱ���ܸ�������ڴ�.
	int   Max_Quantizer;       // ����ʹ�õ��������ϵ��
	int   Min_Quantizer;       // ����ʹ�õ���С����ϵ��
	int   YStride;             // ����ͼ�����ȷ���Y�Ŀ��           
	int   UStride;			   // ����ͼ��ɫ�ȷ���U�Ŀ��
	int   VStride;			   // ����ͼ��ɫ�ȷ���V�Ŀ��
	int   MaxPacketLen;        // ������󳤶�
	short ImageQuality;		   // ͼ���������ڲ���,0 ͼ��������������,4 ͼ���������,�������
	short   Quality;          //Quality��������:
				//Quality = 0: ʹ�ù̶�������Min_Quantize��Ϊ�̶�QPֵ��
				//Quality = 1: ����ģʽ�£��������ȣ�������ʱ������0.5s��ͬʱ������֤֡�ʣ�����ͼ������û��Ҫ��
				//Quality = 2: ����ģʽ�£�֡�����ȣ�Ҳ���Ǿ�������֡��ͼ��Ƚ�������ͼ�������Ϻã������ʱ2s��
				//Quality = 3: ����ģʽ�£��������ȣ�������ʱ������0.5s����ͼ������Ҫ��ϸߣ������ܱ�֤֡�ʣ�֡�ʸ���20֡��
				//Quality = 4: ����ģʽ�£�ͼ���������ȣ�������ʱ������1s�����ܱ�֤֡�ʣ�ÿ��֡�ʸ���15֡��
				//Quality = 5: ����ģʽ�£�ͼ��������ã�������ʱ������2s�����ܱ�֤֡�ʣ�ÿ��֡�ʸ���7֡��
				//Quality = 7: ����ģʽ�£�ͼ��������ã�������ʱ������0.8s�����ܱ�֤֡��,ר�Ÿ����鶨�ơ�
				//Quality = 8: ��quality=2����,ֻ����С����ֵ��5.���Խ��;�ֹͼ�������,���Ƕ�̬����
				//Quality = 256: ��Quailty = 0����,ֻ�Ǳ�Ϊ��һ֡��һ֡,ʹ�ù̶��������ڲ�����Target_BitRate���õ�ֵ��ͼ���С����һ���̶�QPֵ��
				//Quality = 257: ��Quailty = 1����,ֻ�Ǳ�Ϊ��һ֡��һ֡,����ģʽ�£��������ȣ�������ʱ������0.5s��ͬʱ������֤֡�ʣ�����ͼ������û��Ҫ��
				//Quality = 258: ��Quailty = 2����,ֻ�Ǳ�Ϊ��һ֡��һ֡,����ģʽ�£�֡�����ȣ�Ҳ���Ǿ�������֡��ͼ��Ƚ�������ͼ�������Ϻã������ʱ2s��
 	int   bEncPacket;          // �Ƿ��ѹ�����������1:��, 0:����mp4v��֧�ִ��,�ص���������һ֡
	                           // ����H264, 1:RTP, 0:AnnexB
	unsigned char *pStillImage;// ָ��̬ͼ������,�����Ҫ���뾲̬ͼ��,���ָ����ʹNULL,����뺯�����ش�����
	union EncParam_u
	{
		//h264_enc interface, add by zhanglp, 
		//ע�͵��Ĳ���Ϊ��չ���������ڱ����ڵĸĽ����п����õ�
		struct AVSEncParam_s
		{
			unsigned int     Width;         //���
			unsigned int     Height;        //�߶�
			int loop_filter_disable;		//�Ƿ�����·�˲�

			int SeinitialQP;                //���������ϵ��,��û�����ʿ��Ƶ�ʱ��������

			OutputRecFrame DisplayRecFunc;	//��ʾ�ع�֡�Ļص�
			int   bEnableMotionDetect;      //�Ƿ���Ҫ�˶����
			int   TargetFrameCycle;			//ÿ֡��Ŀ�����cycle
		}AVSEncParam;
		struct H264EncParam_s
		{		 
			int qp0;           //I֡������ϵ��.��û�����ʿ��Ƶ�ʱ��������      
			int qpN;           //P֡������ϵ��.��û�����ʿ��Ƶ�ʱ��������      
			int jumpd;         //������֡����һ֡ (e.g 2 takes frame 0,3,6,9...)
			  
			int img_width;                // ͼ��Ŀ��(176,704,352��)������16�ı���
			int img_height;               //ͼ��߶�(144,576,288��)������16�ı���
	
			int LFSendParameters; //�Ƿ񴫵�������˲�����
			int LFDisableIdc;     //0---��ʾ��Ҫ��loopfilter��1---����loopfilter; 2---��slice�߽粻��loopfilter,������
			int LFAlphaC0Offset;  //�˲�����Alpha & C0 offset div. 2, ȡֵ{-6, -5, ... 0, +1, .. +6}
			int LFBetaOffset;     //�˲�����Beta offset div. 2, ȡֵ{-6, -5, ... 0, +1, .. +6}
			
			//! Rate Control on JVT standard 
			int RCEnable;    //�Ƿ�������ʿ���
			int SeinitialQP; //���г�ʼQPֵ
			
			//!< functions call back 
			OutputRecFrame DisplayRecFunc; //��ʾ�ع�֡�Ļص�
			void*  ComputPSNRFunc;  //[in]����PSNR
			

			int InterSearch16x16;   //δʹ��
			int InterSearch16x8;    //δʹ��
			int InterSearch8x16;    //δʹ��
			int InterSearch8x8;     //δʹ�� 					   
			int search_range;  //������Χ���˲���δʹ�ã��ֱ������ڲ��̶�Ϊ13
			int slice_mode;               //����slice�ķ�ʽ, û�������ã��ڱ������ڲ���Ϊ2�����ֽڳ��Ȼ���slice��
			int UseConstrainedIntraPred;  //�˲���δʹ�ã��ڱ������ڲ���Ϊ0��!< 0: Inter MB pixels are allowed for intra prediction 1: Not allowed
			int basicunit;   //�˲���δʹ��
			int channel_type;//�˲���δʹ�ã���Ϊ0
			int   TargetFrameCycle;//ÿ֡��Ŀ�����cycle
			int   bEnableMotionDetect;//�Ƿ���Ҫ�˶����
			short   bProfileLevel1b; // 1����1b,0���������profile_level��������.
			short   profile_level; //���Ϊ��,���Ǳ������Զ�����������ͼ���С���ú��ʵ�profile level,����ֵprofile_level = profile_level/10 �� 12 ����1.2
			PreProcessF PreProcessFunc; //��ʾ�ع�֡�Ļص�
        }H264EncParam;
		struct SPSEncParam_s
		{
			 unsigned int     Width;                       //���
			 unsigned int     Height;                      //�߶�
             OutputRecFrame   RecFrameFunc;                 // �ع�֡�ص�����
             unsigned char    *InitMemAddr;                //�ⲿ������ڴ��ָ��
             unsigned int     InitMemLength;               //�ⲿ������ڴ���С
             int   bEnableMotionDetect;                    //�Ƿ���Ҫ�˶����
		}SPSEncParam;
		struct Mp4vEncParam_s
		{
			short width;		     // ͼ��Ŀ��
			short height;		     // ͼ��߶�
			OutputRecFrame DisplayRecFunc; //[in]��ʾ�ع�֡�Ļص�
			int   rc_period;         // the intended rate control averaging period
			int   rc_reaction_period;// the reaction period for rate control
			int   rc_reaction_ratio; // the ratio for down/up rate control
			int   use_bidirect;	     // use bidirectional coding
			int   deinterlace;	     // fast deinterlace
			int   flags;		     // flags for internal control of encoder
			char *pWaterMarkData;    // ˮӡ��Ϣ,9*9��С,��Ӧ����˱���Ҳһ��,���Ϊnull,���ǲ���ˮӡ,��ʼ��֮��,�����ͷŵ�
			int   bEnableMotionDetect;//�Ƿ���Ҫ�˶����
			int   cbi;				 // ��������
			int   vbi;				  //��������
			int   TargetFrameCycle;
			int	  SeinitialQP; //�̶�����qpֵ
		}Mp4vEncParam;
		struct Vc1EncParam_s
		{
			short width;		     // ͼ��Ŀ��
			short height;		     // ͼ��߶�
			OutputRecFrame DisplayRecFunc; //[in]��ʾ�ع�֡�Ļص�
			int   rc_period;         // the intended rate control averaging period
			int   rc_reaction_period;// the reaction period for rate control
			int   rc_reaction_ratio; // the ratio for down/up rate control
			int   use_bidirect;	     // use bidirectional coding
			int   deinterlace;	     // fast deinterlace
			int   flags;		     // flags for internal control of encoder
			char *pWaterMarkData;    // ˮӡ��Ϣ,9*9��С,��Ӧ����˱���Ҳһ��,���Ϊnull,���ǲ���ˮӡ,��ʼ��֮��,�����ͷŵ�
			int   bEnableMotionDetect;//�Ƿ���Ҫ�˶����
			int   cbi;				 // ��������
			int   vbi;				  //��������
			int   TargetFrameCycle;
			int	  SeinitialQP; //�̶�����qpֵ
		}Vc1EncParam;
		struct H263EncParam_s
		{
			enum  SRCFORMAT m_SrcFormat;      //����֡�ĸ�ʽ��Ϣ��CIF/QCIF��
			int   bCodeGobHead;               //1:����GOBheader,0:������GOBheader(subQCIF��QCIF��CIF������) 
			OutputRecFrame DisplayRecFunc; //[in]��ʾ�ع�֡�Ļص�			
			int   SeinitialQP; //�̶�����qpֵ
		}H263EncParam;
		struct H261EncParam_s
		{
			enum SRCFORMAT m_SrcFormat;       //����֡�ĸ�ʽ��Ϣ��CIF/QCIF��
		}H261EncParam;
	}EncParamUnion;
} ENC_INIT_PARAM;

typedef struct ENCFRAME_s
{
	unsigned char * pVideoInBuf; //������Ƶ֡��ָ��
	int IntraForced;             //�Ƿ�ǿ�Ʊ���Inra֡
	int CurTime;                 //��ǰʱ��� ��λ�Ǻ���
	int bIs_StillImage;          //�Ƿ���뾲̬ͼ��(����Ǿ�̬ͼ��,��ô����ֱ�Ӵӳ�ʼ���������pStillImageָ����ȡ����.
}ENCFRAME;

#endif

