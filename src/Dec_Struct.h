#ifndef _DECUNION_H_
#define _DECUNION_H_

#include "NwCodec.h"

typedef void* HDecoder;

typedef struct RECFRAME_s
{
	unsigned char *pY; //����Y����
	unsigned char *pU; //ɫ��U����
	unsigned char *pV; //ɫ��V����
	int width;         //����ͼ���
	int height;        //����ͼ���
	int stride;        //����ͼ����
	int bIS_StillImage;//�Ƿ��Ǿ�̬ͼ��
	int stillimage_id; //�о�̬ͼ�Ĵ������id
	union recparam_u
	{
		struct H263_RecParam_s
		{
			/////////////////////////////////////////////////////////////////////////
			int H263_Annex_A:1;  //�Ƿ���Annex A��ѡ��
			int H263_Annex_B:1;  //ͬ��
			int H263_Annex_C:1;  //.............
			int H263_Annex_D:1;
			int H263_Annex_E:1;
			int H263_Annex_F:1;
			int H263_Annex_G:1;
			int H263_Annex_H:1;
			//h263�Ŀ�ѡ��
			
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
			//H263+��ѡ��
			
			//////////////////////////////////////////////////////////////////////////
			int H263_ADD_Annex_U:1;
			int H263_ADD_Annex_V:1;
			int H263_ADD_Annex_W:1;
			//h263++��ѡ��
			int reserved:9;//��������
		}H263_RecParam;
		struct H261_RecParam_s
		{
			int reserved;  //��������
		}H261_RecParam;
		struct Mp4v_RecParam_s
		{
			int bhaveWatermark;//��ǰ֡�Ƿ��ˮӡ
		}Mp4v_RecParam;
		struct SPS_RecParam_s
		{
			int reserved;  //��������
		}SPS_RecParam;
	}RecParamUnion;
}RECFRAME;

typedef void(*OutputRec)(void *m_HandleCallBack, const RECFRAME  *pRecFrame);


typedef struct DEC_INIT_PARAM_s
{
	OutputRec displayImage;   //��ʾ�ص�
	void *m_HandleCallBack;           //�ϲ㴦����ʾ���ֵ�handle,����ΪNULL
	MEMORY SDRAM;             //Ƭ����ڴ�ָ��
	MEMORY L2;				  //Ƭ��L2, �ڱ�������ʼ��֮��һֱռ�õ��ڴ档
	MEMORY L2Tmp;			  //Ƭ��L2, �ڱ������ÿһ֡��ʱ����ʱ�õ����ڴ�,����ڶ�·��ʱ���ܸ�������ڴ�.
	int  bDecPacket;           //�ǽ�֡���ǽⱨ
	int  bEnableDecStillImage; //�Ƿ���뾲̬ͼ��
	union decinit_u
	{
		struct Mp4v_DecInit_s
		{
			char *pWaterMarkData;       //ˮӡ��Ϣ,9*9��С,�������Ӧ�ı����һ��,���ΪNULL�Ͳ����ˮӡ,��ʼ��֮��,�����ͷŵ�
			int   bOutSideAlloc;        //�Ƿ�������������ڴ�,ֻ��tetraƽ̨��mpeg4������������
			void *pMem;	                // ���ⲿ����ô������������ʼ��ַ,ֻ��tetraƽ̨��mpeg4������������
			int   nMemLength;           // ����ĳ���,ֻ��tetraƽ̨��mpeg4������������
		}Mp4v_DecInit;
		struct H263_DecInit_s
		{
			int reserved0;//��������
		}H263_DecInit;
		struct H261_DecInit_s
		{
			int stream_type;//��������
		}H261_DecInit;
		struct SPS_DecInit_s
		{
			unsigned char *pInitMem;
			unsigned int   InitMemLength;      
			int reserved0;//��������
		}SPS_DecInit;
	}DecInitUnion;
	
} DEC_INIT_PARAM;

typedef struct DECFRAME_s
{
	unsigned char *pInPacket; //�������Ƶ��,����ǽ�֡,��������һ֡������.����4�ֽڶ���.
	int nLength;              //������Ƶ���ĳ���
	int bIs_LostPackage;      //�Ƿ񶪰�
	union decparam_u
	{
		struct H264_DecParam_s
		{
			unsigned char *pRecImgY421;//��������
			unsigned char *pRecImgC421;//��������
			int nRec421Stride;
		}H264_DecParam;
		struct Mp4v_DecParam_s
		{
			int   bIs_LAST_Package;     //�Ƿ������һ����
			int   bIs_postprocess;      //�Ƿ�Ҫ�Ӻ���
		}Mp4v_DecParam;
		struct H263_DecParam_s
		{
			int reserved0;//��������
			int reserved1;//��������
		}H263_DecParam;
		struct H261_DecParam_s
		{
			int reserved0;//��������
			int reserved1;//��������
		}H261_DecParam;
		struct SPS_DecParam_s
		{
			int reserved0;//��������
			int reserved1;//��������
		}SPS_DecParam;
	}DecParamUnion;
}DECFRAME;

#endif

