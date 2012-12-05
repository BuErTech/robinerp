#pragma once

#ifndef AFCRESOURCE_HEAD_FILE
#define AFCRESOURCE_HEAD_FILE

#include "PublicDefine.h"
#include "AFCImage.h"

//using namespace AFC;
//using namespace AFC::AFCImageGoGoGo;

//�궨��
#define ICON_SIZE				24													//ͼ����
#define FONT_SIZE				12													//������

/*namespace AFC
{
	namespace AFCResource
	{*/
		/*******************************************************************************************************/

		//������ GDI ��Դ�ṹ
		struct CAFCTitleResource
		{
			CGameImage						m_BackPic;						//Ĭ�ϱ���
			COLORREF						m_crTextColor;					//������ɫ
		};

		//����ҳ GDI ��Դ�ṹ
		struct CAFCTabResource
		{
			CGameImage						m_ActivePic;					//�����ǩ
			CGameImage						m_NormalPic;					//�����ǩ
			CGameImage						m_BackPic;						//Ĭ�ϱ���
			CGameImage						m_BackPicNew;
			//�������
			CGameImage						m_BackBottom;				//�������µ�һ��
			CGameImage						m_BackBottomLeft;				//�������µ�һ��
			CGameImage						m_BackBottomRight;				//�������µ�һ��

			COLORREF						m_crFrameHeight;				//�����ɫ
			COLORREF						m_crFrameShadow;				//�����ɫ
			COLORREF						m_crFrameBKColor;				//��ܵ�ɫ
			COLORREF						m_crNormalTXColor;				//������ɫ
			COLORREF						m_crSelectTXColor;				//������ɫ
		};

		//��ť GDI ��Դ�ṹ
		struct CAFCButtonResource
		{
			//������ť��
			bool							m_bSound;						//��������
			HCURSOR							m_hMouneCursor;					//�����

			//��ɫ��ť����
			COLORREF						m_ActiveColor;					//���ɫ
			COLORREF						m_ActiveTextColor;				//�������ɫ
			COLORREF						m_NormalTextColor;				//����������ɫ
			COLORREF						m_SelectTextColor;				//ѡ��������ɫ
			COLORREF						m_FrameColor;					//�����ɫ
			COLORREF						m_FrameHeight;					//�����ɫ
			COLORREF						m_FrameShadow;					//�����ɫ
			COLORREF						m_NormalColorTop;				//������ɫ
			COLORREF						m_NormalColorBottom;			//������ɫ

			//����λͼ��ť����
			CGameImage						m_BackPic;						//��ť��ͼ
		};

		//�б���ͼ�� GDI ��Դ�ṹ
		struct CAFCListResource
		{
			CGameImage						m_TitlePic;						//�б�ͷͼƬ
			COLORREF						m_crListColor;					//�б�������ɫ
			COLORREF						m_crBkColor;					//�б�����ɫ
			COLORREF						m_crFousBkColor;				//�б�����ɫ
			COLORREF						m_crFousTxColor;				//�б�������ɫ
			COLORREF						m_crEmemyTxColor;				//�б�������ɫ
			COLORREF						m_crNormalTxColor;				//�б�������ɫ
			COLORREF						m_crNorMemTxColor;				//�б�������ɫ
			COLORREF						m_crHighMemTxColor;				//�б�������ɫ
			COLORREF						m_crFriendTxColor;				//�б�������ɫ
			COLORREF						m_crMasterTxColor;				//�б�������ɫ
			COLORREF						m_crMeTxColor;					//�б�������ɫ
		};

		//�Ի��� GDI ��Դ�ṹ
		struct CAFCDialogResource
		{
			//��ɫ�Ի��򲿷�
			CBrush							m_BackBrush;					//����ˢ
			COLORREF						m_crTextColor;					//������ɫ
			COLORREF						m_crBackColor;					//������ɫ

			//�Ի��Ի��򲿷�
			CGameImage						m_Title;						//����ͼ
			CGameImage						m_MaxNor;						//��󻯰�ť
			CGameImage						m_MaxFoc;						//��󻯰�ť
			CGameImage						m_MaxDis;						//��󻯰�ť
			CGameImage						m_ResNor;						//��ԭ��ť
			CGameImage						m_ResFoc;						//��ԭ��ť
			CGameImage						m_ResDis;						//��ԭ��ť
			CGameImage						m_MinNor;						//��С����ť
			CGameImage						m_MinFoc;						//��С����ť
			CGameImage						m_MinDis;						//��С����ť
			CGameImage						m_CloseNor;						//�رհ�ť
			CGameImage						m_CloseFoc;						//�رհ�ť
			CGameImage						m_CloseDis;						//�رհ�ť
			CGameImage						m_BackPic;						//���ڵ�ͼ
			COLORREF						m_rcTopFrameColor;				//�����ɫ
			COLORREF						m_rcButFrameColor;				//�����ɫ
			COLORREF						m_rcTitleTextColor;				//������ɫ
		};

		//�༭�ؼ� GDI ��Դ
		struct CAFCEditResource
		{
			CBrush							m_brFouns;						//������ɫ
			CBrush							m_brNoFocus;					//ʧȥ������ɫ
			CBrush							m_brDiable;						//���ɸ�д��ɫ
			COLORREF						m_crFocusBKRGB;					//������ɫ
			COLORREF						m_crNoFocusBKRGB;				//ʧȥ������ɫ
			COLORREF						m_crDisFocusBKRGB;				//���ɸ�д��ɫ
			COLORREF						m_crFocusTXRGB;					//������ɫ
			COLORREF						m_crNoFocusTXRGB;				//ʧȥ������ɫ
			COLORREF						m_crDisFocusTXRGB;				//���ɸ�д��ɫ
		};

		//����� GDI ��Դ
		struct CAFCSplitResource
		{
			HCURSOR							m_hHorCur;						//����
			HCURSOR							m_hVorCur;						//�����
			HCURSOR							m_hDisableCur;					//��ֹʹ��
		};

		//��Ϸ������ɫ
		struct CAFCColorResource
		{
			COLORREF						m_crListTxColor;				//��Ϸ�б�������ɫ
			COLORREF						m_crListBkColor;				//��Ϸ�б�����ɫ
			COLORREF						m_crDeskTxColor;				//��������������ɫ
			COLORREF						m_crSysHeadTxColor;				//ϵͳǰ����ɫ
			COLORREF						m_crSystemTxColor;				//ϵͳ��Ϣ��ɫ
			COLORREF						m_crSystemBdColor;				//ϵͳ������ɫ
			COLORREF						m_crNormalTxColor;				//��ͨ��Ϣ��ɫ
			COLORREF						m_crTalkNameColor;				//�����û�������ɫ
			COLORREF						m_crTalkTxColor;				//������Ϣ������ɫ
			COLORREF						m_crTalkBkColor;				//������Ϣ������ɫ
			COLORREF						m_crNewsTxColor;				//������ɫ
			COLORREF						m_crSentTxColor;
			COLORREF						m_crReceivedTxColor;
		};

		/*******************************************************************************************************/

		//AFC ��Դ��
		class EXT_CLASS CGameImageLink//C-AFCResourceManage
		{
			//ȫ��ʹ�ñ���
		public:
			static CFont					m_VFont;						//��ʾ����
			static CFont					m_HFont;						//��ʾ����
			static HCURSOR					m_hHandCursor;					//�����
			static bool						m_bAutoLock;					//�Ƿ����

			//��Դ����
		public:
			static CAFCTabResource			m_TabResource;					//����ҳ��Դ
			static CAFCTitleResource		m_TitleResource;				//������Դ
			static CAFCButtonResource		m_ButtonResource;				//��ť��Դ
			static CAFCListResource			m_ListResource;					//�б���Դ
			static CAFCDialogResource		m_DialogResource;				//�Ի�����Դ
			static CAFCEditResource			m_EditResource;					//�༭�ؼ���Դ
			static CAFCSplitResource		m_SplitResource;				//�������Դ
			static CAFCColorResource		m_ColorResource;				//��ɫ��Դ

			//���ܺ���
		public:
			//��ʼ����Դ
			static bool InitResource(TCHAR * szSinkFileName=NULL);
		};

#endif