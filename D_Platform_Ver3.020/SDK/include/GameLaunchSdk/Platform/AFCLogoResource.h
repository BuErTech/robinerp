#pragma once

#ifndef CAFCLOGORESOURCE_HEAD_FILE
#define CAFCLOGORESOURCE_HEAD_FILE

#include "PublicDefine.h"
#include "AFCResource.h"

//�궨��
#define LOGO_HEIGHT				108										//ͷ��߶�
#define LOGO_WIDTH				65										//ͷ����
#define LOGO_COUNT				12										//ͷ����Ŀ
#define ZZ_COUNT				10										//������Ŀ

//��Ϣ����
#define WM_HIT_LOGO				WM_USER+30								//����ͷ��

/*******************************************************************************************************/

//ͷ����Դ��
class EXT_CLASS CGameLogoRes//CGameLogoRes
{
public:
	//��������
protected:
	static CImage				m_MatchPic;								//����ͷ��
	static CImage				m_LostPic;								//����ͷ��
	static CImage				m_LogoPicArray[LOGO_COUNT];				//ͷ������
	static CImage				SitBoyArray[ZZ_COUNT];				//������������
	static CImage				SitGirlArray[ZZ_COUNT];				//Ů����������
	static COLORREF				m_crBackColor;							//͸����ɫ

	//��������
public:
	//���캯��
	CGameLogoRes(void);
	//��������
	virtual ~CGameLogoRes(void);

	//���ܺ���
public:
	//��ʼ������
	static bool Init();
	//��ȡ͸��ɫ
	static COLORREF GetBackColor() { return  m_crBackColor; };
	//��ȡ����ͼƬ
	static CImage * GetMatchImage();
	//��ȡ LOGO ͼƬ
	static CImage * GetLogoImage(UINT uIndex, bool bLost);
//��ȡ���� ͼƬ�����ϵ���Ů��
static CImage *GetSitGirl(UINT uIndex, bool bLost);
//��ȡ���� ͼƬ�����ϵ�������
static CImage *GetSitBoy(UINT uIndex, bool bLost);
};

/*******************************************************************************************************/

//ͷ����ʾ��
class EXT_CLASS CGameClientTranStatic/*CGameClientTranStatic*/ : public CStatic
{
	//��������
protected:
	CImage							* m_pLogoImage;						//ͷ��ͼƬ

	//��������
public:
	//���캯��
	CGameClientTranStatic();
	//��������
	virtual ~CGameClientTranStatic();
	//����ͼƬ
	bool SetLogoImage(CImage * pLogoImage);

	DECLARE_MESSAGE_MAP()

	//��Ϣ����
public:
	//��굥��
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//���˫����
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//�ػ�����
	afx_msg void OnPaint();
};

/*******************************************************************************************************/

#endif