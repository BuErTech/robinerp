#ifndef CAFCEXPRESSIONWND_HEAD_FILE
#define CAFCEXPRESSIONWND_HEAD_FILE

#include "PublicDefine.h"
#include "AFCResource.h"

//���ֿռ�
//using namespace AFC;
//using namespace AFC::AFCResource;

//�궨��
#define MAX_EXPRESS_ITEM			68										//��������Ŀ
#define WM_HIT_EXPMESSTION			WM_USER+11								//���������־
#define	WM_HITDESK					WM_USER+12								//����ʱ�������
#define WM_INVITE_PLAY				WM_USER+113								//����
#define WM_CHATING_WITH					WM_USER+114								//����
#define WM_WATCH_GAME					WM_USER+115								//�Թ�

//��������
class EXT_CLASS CTrueItem//CTrueItem
{
	//��������
public:
	CBitmap							m_ExpPicImage;							//����ͼƬ
	UINT							m_uTranceLength;						//���ͳ���
	TCHAR							m_szTrancelate[11];						//�����ַ�
	TCHAR							m_szChineseName[11];					//���Ľ���

	//��������
public:
	//���캯��
	CTrueItem();
	//��������
	~CTrueItem();
	//�������
	void SetExpressInfo(TCHAR * szChineseName, TCHAR * szTrancelate, UINT uExpPicID);
};

//���鶯��������
class EXT_CLASS CGameTalkText/*CGameTalkText*/ : public CDialog
{
	//��������
protected:
	UINT							m_uSelectIndex;							//ѡ������

	//ȫ�ֱ���
protected:
	static UINT						m_uMaxRow;								//�������Ŀ
	static UINT						m_uMaxLine;								//�������Ŀ

public:
	static CTrueItem			m_ExpreesionItem[MAX_EXPRESS_ITEM];		//��������

	//�����ھ��
	HWND						m_hParentHWND;

	//��������
public:
	//���캯��
	CGameTalkText();
	//��������
	virtual ~CGameTalkText();

	//���ܺ���
public:
	//��������
	bool CreateExpressionWnd(CWnd * pParentWnd, HWND pParentHWND = NULL);
	//��ȡ�ʺϴ�С
	bool GetFixSize(CSize & WndSize);
	//��ȡ�ʺϴ�С
	bool GetFixSize(SIZE & WndSize);
	//��ȡ����
	static CTrueItem * GetExpressItem(UINT uIndex);

	//��Ϣӳ��
protected:
	DECLARE_MESSAGE_MAP()

	//��������
protected:
	//��ʼ����Դ
	static bool InitExpressItem();
	//�滭ͷ��
	void DrawItem(CDC * pDC, UINT iXPos, UINT iYPos, bool bSelect);
	//��������
	UINT HitItemTest(CPoint MousePoint);

	//��Ϣ����
public:
	//�ػ���Ϣ
	afx_msg void OnPaint();
	//ʧȥ������Ϣ
	afx_msg void OnKillFocus(CWnd * pNewWnd);
	//����ƶ���Ϣ
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//��갴����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

#endif