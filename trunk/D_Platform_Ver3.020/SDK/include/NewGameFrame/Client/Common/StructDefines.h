#pragma once
#include <Windows.h>


/// MDM_GP_REQURE_GAME_PARAЭ���õ������ݽṹ
//class CCenterHead
//{
//public:
//	enum enumCenterType
//	{
//		CenterServerPort=65320//6688
//	};
//	class Message
//	{
//	public:
//		char m_strGameSerialNO[20];////�ͻ��˵�ǰ�汾ϵ�к�
//		char m_strMainserverIPAddr[20];			////��������IP��ַ
//		long m_iMainserverPort;					////���������˿ں�
//		int  m_iOnlineUserCount;                ////��ǰ��Ϸ��������
//		char m_strWebRootADDR[50],				////��վ��·�����ڳ������漰���ļ���Ŀ¼���������ַ����չ
//			m_strHomeADDR[50],					////������¼���һ����ʾ��ҳ��
//			m_strGoldADDR[500],				///��Ҳ���ʱ��ʾ����
//			m_strHelpADDR[50],					////����ҳWEB��ַ
//			m_strDownLoadSetupADDR[50],		////�ͻ��˰�װ��������ҳWEB��ַ
//			m_strDownLoadUpdatepADDR[50],		////�ͻ��˸��³�������ҳWEB��ַ
//			m_strRallAddvtisFlashADDR[50],		//�ͻ��˴���FLASH�������ҳWEB��ַ
//			m_strRoomRollADDR[100];			//�ͻ��˴����������
//		int                                gscheck;
//		int	 m_nHallInfoShowClass;
//		Message( )
//		{
//			m_iMainserverPort=0;
//			m_nHallInfoShowClass=0;
//			m_iOnlineUserCount = 0;
//			memset(m_strGameSerialNO,0,20);
//			gscheck=0;
//			memset(m_strMainserverIPAddr,0,20);
//			memset(m_strHomeADDR,0,50);
//			memset(m_strWebRootADDR,0,50);
//			memset(m_strHelpADDR,0,50);
//			memset(m_strDownLoadSetupADDR,0,50);
//			memset(m_strDownLoadUpdatepADDR,0,50);
//			memset(m_strRallAddvtisFlashADDR,0,50);
//			memset(m_strRoomRollADDR,0,100);
//
//		}
//		~Message(){}
//	};
//};
class CCenterHead
{
public:
	enum enumCenterType
	{
		CenterServerPort=65320//6688
	};
	class Message
	{
	public:
		int m_is_haveZhuanZhang;
		char m_strGameSerialNO[20];////�ͻ��˵�ǰ�汾ϵ�к�
		char m_strMainserverIPAddr[20];			////��������IP��ַ
		long m_iMainserverPort;					////���������˿ں�
		char m_strWebRootADDR[50],				////��վ��·�����ڳ������漰���ļ���Ŀ¼���������ַ����չ
			m_strHomeADDR[50],					////������¼���һ����ʾ��ҳ��
			m_strHelpADDR[50],					////����ҳWEB��ַ
			m_strDownLoadSetupADDR[50],		////�ͻ��˰�װ��������ҳWEB��ַ
			m_strDownLoadUpdatepADDR[50],		////�ͻ��˸��³�������ҳWEB��ַ
			m_strRallAddvtisFlashADDR[50],		//�ͻ��˴���FLASH�������ҳWEB��ַ
			//wushuqun 2009.6.5
			m_strRoomRollADDR[200];			//�ͻ��˴����������

		int	 m_nHallInfoShowClass;
		int	 m_nEncryptType;					//ƽ̨�����õļ��ܷ�ʽ��1λMD5��2λSHA1��Ĭ��Ϊ2; 2009-5-30 zxj
		unsigned int m_nFunction;               ///< ���ܿ��� Y.C.YAN
		long m_lNomalIDFrom;                    ///< ���«��ɢ��ID��ʼ
		long m_lNomalIDEnd;						///< ���«��ɢ��ID����
		Message( )
		{
			m_iMainserverPort    = 0;
			m_nHallInfoShowClass = 0;
			m_nEncryptType       = 2;					//ƽ̨�����õļ��ܷ�ʽ��1λMD5��2λSHA1��Ĭ��Ϊ2; 2009-5-30 zxj
			m_nFunction          = 0;
			memset(m_strGameSerialNO,0,20);
			memset(m_strMainserverIPAddr,0,20);
			memset(m_strHomeADDR,0,50);
			memset(m_strWebRootADDR,0,50);
			memset(m_strHelpADDR,0,50);
			memset(m_strDownLoadSetupADDR,0,50);
			memset(m_strDownLoadUpdatepADDR,0,50);
			memset(m_strRallAddvtisFlashADDR,0,50);
			//wushuqun 2009.6.5
			memset(m_strRoomRollADDR,0,200);

		}
		~Message(){}
	};
};
typedef   CCenterHead::Message     CenterServerMsg;
