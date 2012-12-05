//���ӿ����ͼ
#include "StdAfx.h"
#include "GamePlace.h"
#include "DeskFrameView.h"
#include "GameRoomEx.h"

#define IDC_BJLBEGIN	11//�ټ��� add by wlr 20090713
#define IDC_BJLPLAYING  12//�ټ��� add by wlr 20090713

#define IDC_QUEUEBEGIN	13//�ټ��� add by wlr 20090713
#define IDC_QUEUESTOP  14//�ټ��� add by wlr 20090713

extern void DebugPrintf(const char *p, ...);

/////�¼ӵ����ӣ�������Ϸ
CPoint				CDesk1::m_PtLock;								//��λ��
CPoint				CDesk1::m_PtBasePoint;							//���ӱ���
CPoint				CDesk1::m_PtBegin;								//��ʼλ��//��������Ϸ������
CRect				CDesk1::m_IndexRect;							//��Ϸ������
CPoint				CDesk1::m_PtAgree[1];							//ͬ��λ��
CRect				CDesk1::m_NameRect[1];							//���ֿ��
CRect				CDesk1::m_ChairRect[1];							//����λ��
UINT				CDesk1::m_TextFormat[1];						//��ʽ����
CPoint				CDesk1::m_PtLog[1];								//����λ��
CPoint				CDesk1::m_BtnAgree[1];								//ͬ��λ��
int					CDesk1::m_draw[1];								//��ʾ˳��
/// ��ɫֵ
Color				CDesk1::m_UserNameColor;					///< �����
Color				CDesk1::m_VipNameColor;						///< VIP�����
Color				CDesk1::m_DeskNoColor;						///< ����������ɫ
Color				CDesk1::m_FocusColor;						///< �������ɫ

CPoint				CDesk2::m_PtLock;								//��λ��
CPoint				CDesk2::m_PtBasePoint;							//���ӱ���
CPoint				CDesk2::m_PtBegin;								//��ʼλ��//��������Ϸ������
CRect				CDesk2::m_IndexRect;							//��Ϸ������
CPoint				CDesk2::m_PtAgree[2];							//ͬ��λ��
CRect				CDesk2::m_NameRect[2];							//���ֿ��
CRect				CDesk2::m_ChairRect[2];							//����λ��
UINT				CDesk2::m_TextFormat[2];						//��ʽ����
CPoint				CDesk2::m_PtLog[2];								//����λ��
CPoint				CDesk2::m_BtnAgree[2];								//ͬ��λ��
int					CDesk2::m_draw[2];								//��ʾ˳��
/// ��ɫֵ
Color				CDesk2::m_UserNameColor;					///< �����
Color				CDesk2::m_VipNameColor;						///< VIP�����
Color				CDesk2::m_DeskNoColor;						///< ����������ɫ
Color				CDesk2::m_FocusColor;						///< �������ɫ
 
CPoint				CDesk3::m_PtLock;								//��λ��
CPoint				CDesk3::m_PtBasePoint;							//���ӱ���
CPoint				CDesk3::m_PtBegin;								//��ʼλ��
CRect				CDesk3::m_IndexRect;							//��Ϸ������
CPoint				CDesk3::m_PtAgree[3];							//ͬ��λ��
CRect				CDesk3::m_NameRect[3];							//���ֿ��
CRect				CDesk3::m_ChairRect[3];							//����λ��
UINT				CDesk3::m_TextFormat[3];						//��ʽ����
CPoint				CDesk3::m_PtLog[3];								//����λ��
CPoint				CDesk3::m_BtnAgree[3];								//ͬ��λ��
int					CDesk3::m_draw[3];								//��ʾ˳��
/// ��ɫֵ
Color				CDesk3::m_UserNameColor;					///< �����
Color				CDesk3::m_VipNameColor;						///< VIP�����
Color				CDesk3::m_DeskNoColor;						///< ����������ɫ
Color				CDesk3::m_FocusColor;						///< �������ɫ

CPoint				CDesk4::m_PtLock;								//��λ��
CPoint				CDesk5::m_PtBasePoint;							//���ӱ���
CPoint				CDesk4::m_PtBasePoint;							//���ӱ���
CPoint				CDesk4::m_PtBegin;								//��ʼλ��
CRect				CDesk4::m_IndexRect;							//��Ϸ������
CPoint				CDesk4::m_PtAgree[4];							//ͬ��λ��
CRect				CDesk4::m_NameRect[4];							//���ֿ��
CRect				CDesk4::m_ChairRect[4];							//����λ��
UINT				CDesk4::m_TextFormat[4];						//��ʽ����
CPoint				CDesk4::m_PtLog[4];								//����λ��
CPoint				CDesk4::m_BtnAgree[4];								//ͬ��λ��
int					CDesk4::m_draw[4];								//��ʾ˳��
/// ��ɫֵ
Color				CDesk4::m_UserNameColor;					///< �����
Color				CDesk4::m_VipNameColor;						///< VIP�����
Color				CDesk4::m_DeskNoColor;						///< ����������ɫ
Color				CDesk4::m_FocusColor;						///< �������ɫ

CPoint				CDesk5::m_PtLock;								//��λ��
CPoint				CDesk6::m_PtBasePoint;							//���ӱ���
CPoint				CDesk5::m_PtBegin;								//��ʼλ��
CRect				CDesk5::m_IndexRect;							//��Ϸ������
CPoint				CDesk5::m_PtAgree[5];							//ͬ��λ��
CRect				CDesk5::m_NameRect[5];							//���ֿ��
CRect				CDesk5::m_ChairRect[5];							//����λ��
UINT				CDesk5::m_TextFormat[5];						//��ʽ����
CPoint				CDesk5::m_PtLog[5];								//����λ��
CPoint				CDesk5::m_BtnAgree[5];								//ͬ��λ��
int					CDesk5::m_draw[5];								//��ʾ˳��
/// ��ɫֵ
Color				CDesk5::m_UserNameColor;					///< �����
Color				CDesk5::m_VipNameColor;						///< VIP�����
Color				CDesk5::m_DeskNoColor;						///< ����������ɫ
Color				CDesk5::m_FocusColor;						///< �������ɫ

CPoint				CDesk6::m_PtLock;								//��λ��
CPoint				CDesk6::m_PtBegin;								//��ʼλ��
CRect				CDesk6::m_IndexRect;							//��Ϸ������
CPoint				CDesk6::m_PtAgree[6];							//ͬ��λ��
CRect				CDesk6::m_NameRect[6];							//���ֿ��
CRect				CDesk6::m_ChairRect[6];							//����λ��
UINT				CDesk6::m_TextFormat[6];						//��ʽ����
CPoint				CDesk6::m_PtLog[6];								//����λ��
CPoint				CDesk6::m_BtnAgree[6];								//ͬ��λ��
int					CDesk6::m_draw[6];								//��ʾ˳��
/// ��ɫֵ
Color				CDesk6::m_UserNameColor;					///< �����
Color				CDesk6::m_VipNameColor;						///< VIP�����
Color				CDesk6::m_DeskNoColor;						///< ����������ɫ
Color				CDesk6::m_FocusColor;						///< �������ɫ

CPoint				CDesk8::m_PtLock;								//��λ��							//8����    start
CPoint				CDesk8::m_PtBasePoint;							//���ӱ���
CPoint				CDesk8::m_PtBegin;								//��ʼλ��
CRect				CDesk8::m_IndexRect;							//��Ϸ������
CPoint				CDesk8::m_PtAgree[8];							//ͬ��λ��
CRect				CDesk8::m_NameRect[8];							//���ֿ��
CRect				CDesk8::m_ChairRect[8];							//����λ��
UINT				CDesk8::m_TextFormat[8];						//��ʽ����
CPoint				CDesk8::m_PtLog[8];								//����λ��
CPoint				CDesk8::m_BtnAgree[8];								//ͬ��λ��
int					CDesk8::m_draw[8];								//��ʾ˳��
/// ��ɫֵ
Color				CDesk8::m_UserNameColor;					///< �����
Color				CDesk8::m_VipNameColor;						///< VIP�����
Color				CDesk8::m_DeskNoColor;						///< ����������ɫ
Color				CDesk8::m_FocusColor;						///< �������ɫ							 //end
//CPoint				CDesk7::m_PtLock;								//��λ��
//CPoint				CDesk7::m_PtBegin;								//��ʼλ��
//CRect				CDesk7::m_IndexRect;							//��Ϸ������
//CPoint				CDesk7::m_PtAgree[7];							//ͬ��λ��
//CRect				CDesk7::m_NameRect[7];							//���ֿ��
//CRect				CDesk7::m_ChairRect[7];							//����λ��
//UINT				CDesk7::m_TextFormat[7];						//��ʽ����
//
//CPoint				CDesk8::m_PtLock;								//��λ��
//CPoint				CDesk8::m_PtBegin;								//��ʼλ��
//CRect				CDesk8::m_IndexRect;							//��Ϸ������
//CPoint				CDesk8::m_PtAgree[8];							//ͬ��λ��
//CRect				CDesk8::m_NameRect[8];							//���ֿ��
//CRect				CDesk8::m_ChairRect[8];							//����λ��
//UINT				CDesk8::m_TextFormat[8];						//��ʽ����

CPoint				CDesk16::m_PtLock;								//��λ��
CPoint				CDesk16::m_PtBasePoint;							//���ӱ���
CPoint				CDesk16::m_PtBegin;								//��ʼλ��
CRect				CDesk16::m_IndexRect;							//��Ϸ������
CPoint				CDesk16::m_PtAgree[16];							//ͬ��λ��
CRect				CDesk16::m_NameRect[16];						//���ֿ��
CRect				CDesk16::m_ChairRect[16];						//����λ��
UINT				CDesk16::m_TextFormat[16];						//��ʽ����
CPoint				CDesk16::m_PtLog[16];							//����λ��
CPoint				CDesk16::m_BtnAgree[16];						//ͬ��λ��
int					CDesk16::m_draw[16];							//��ʾ˳��
/// ��ɫֵ
Color				CDesk16::m_UserNameColor;					///< �����
Color				CDesk16::m_VipNameColor;						///< VIP�����
Color				CDesk16::m_DeskNoColor;						///< ����������ɫ
Color				CDesk16::m_FocusColor;						///< �������ɫ

CPoint				CDeskEx2::m_PtLock;								//��λ��
CPoint				CDeskEx2::m_PtBegin;							//��ʼλ��
CRect				CDeskEx2::m_IndexRect;							//��Ϸ������
CPoint				CDeskEx2::m_PtAgree[2];							//ͬ��λ��
CRect				CDeskEx2::m_NameRect[2];						//���ֿ��
CRect				CDeskEx2::m_ChairRect[2];						//����λ��
UINT				CDeskEx2::m_TextFormat[2];						//��ʽ����
CPoint				CDeskEx2::m_PtLog[2];							//����λ��
CPoint				CDeskEx2::m_BtnAgree[2];								//ͬ��λ��
int					CDeskEx2::m_draw[2];							//��ʾ˳��
/// ��ɫֵ
Color				CDeskEx2::m_UserNameColor;					///< �����
Color				CDeskEx2::m_VipNameColor;						///< VIP�����
Color				CDeskEx2::m_DeskNoColor;						///< ����������ɫ
Color				CDeskEx2::m_FocusColor;						///< �������ɫ

CPoint				CDeskEx3::m_PtLock;								//��λ��
CPoint				CDeskEx3::m_PtBegin;							//��ʼλ��
CRect				CDeskEx3::m_IndexRect;							//��Ϸ������
CPoint				CDeskEx3::m_PtAgree[3];							//ͬ��λ��
CRect				CDeskEx3::m_NameRect[3];						//���ֿ��
CRect				CDeskEx3::m_ChairRect[3];						//����λ��
UINT				CDeskEx3::m_TextFormat[3];						//��ʽ����
CPoint				CDeskEx3::m_PtLog[3];							//����λ��
CPoint				CDeskEx3::m_BtnAgree[3];								//ͬ��λ��
int					CDeskEx3::m_draw[3];							//��ʾ˳��
/// ��ɫֵ
Color				CDeskEx3::m_UserNameColor;					///< �����
Color				CDeskEx3::m_VipNameColor;						///< VIP�����
Color				CDeskEx3::m_DeskNoColor;						///< ����������ɫ
Color				CDeskEx3::m_FocusColor;						///< �������ɫ

CPoint				CDeskEx4::m_PtLock;								//��λ��
CPoint				CDeskEx4::m_PtBegin;							//��ʼλ��
CRect				CDeskEx4::m_IndexRect;							//��Ϸ������
CPoint				CDeskEx4::m_PtAgree[4];							//ͬ��λ��
CRect				CDeskEx4::m_NameRect[4];						//���ֿ��
CRect				CDeskEx4::m_ChairRect[4];						//����λ��
UINT				CDeskEx4::m_TextFormat[4];						//��ʽ����
CPoint				CDeskEx4::m_PtLog[4];							//����λ��
CPoint				CDeskEx4::m_BtnAgree[4];								//ͬ��λ��
int					CDeskEx4::m_draw[4];							//��ʾ˳��
/// ��ɫֵ
Color				CDeskEx4::m_UserNameColor;					///< �����
Color				CDeskEx4::m_VipNameColor;						///< VIP�����
Color				CDeskEx4::m_DeskNoColor;						///< ����������ɫ
Color				CDeskEx4::m_FocusColor;						///< �������ɫ

CPoint				CDeskEx5::m_PtLock;								//��λ��
CPoint				CDeskEx5::m_PtBegin;							//��ʼλ��
CRect				CDeskEx5::m_IndexRect;							//��Ϸ������
CPoint				CDeskEx5::m_PtAgree[5];							//ͬ��λ��
CRect				CDeskEx5::m_NameRect[5];						//���ֿ��
CRect				CDeskEx5::m_ChairRect[5];						//����λ��
UINT				CDeskEx5::m_TextFormat[5];						//��ʽ����
CPoint				CDeskEx5::m_PtLog[5];							//����λ��
CPoint				CDeskEx5::m_BtnAgree[5];								//ͬ��λ��
int					CDeskEx5::m_draw[5];							//��ʾ˳��
/// ��ɫֵ
Color				CDeskEx5::m_UserNameColor;					///< �����
Color				CDeskEx5::m_VipNameColor;						///< VIP�����
Color				CDeskEx5::m_DeskNoColor;						///< ����������ɫ
Color				CDeskEx5::m_FocusColor;						///< �������ɫ

CPoint				CDeskEx6::m_PtLock;								//��λ��
CPoint				CDeskEx6::m_PtBegin;							//��ʼλ��
CRect				CDeskEx6::m_IndexRect;							//��Ϸ������
CPoint				CDeskEx6::m_PtAgree[6];							//ͬ��λ��
CRect				CDeskEx6::m_NameRect[6];						//���ֿ��
CRect				CDeskEx6::m_ChairRect[6];						//����λ��
UINT				CDeskEx6::m_TextFormat[6];						//��ʽ����
CPoint				CDeskEx6::m_PtLog[6];							//����λ��
CPoint				CDeskEx6::m_BtnAgree[6];								//ͬ��λ��
int					CDeskEx6::m_draw[6];							//��ʾ˳��
/// ��ɫֵ
Color				CDeskEx6::m_UserNameColor;					///< �����
Color				CDeskEx6::m_VipNameColor;						///< VIP�����
Color				CDeskEx6::m_DeskNoColor;						///< ����������ɫ
Color				CDeskEx6::m_FocusColor;						///< �������ɫ

CPoint				CDeskEx8::m_PtLock;								//��λ��							//8����    start
CPoint				CDeskEx8::m_PtBasePoint;							//���ӱ���
CPoint				CDeskEx8::m_PtBegin;								//��ʼλ��
CRect				CDeskEx8::m_IndexRect;							//��Ϸ������
CPoint				CDeskEx8::m_PtAgree[8];							//ͬ��λ��
CRect				CDeskEx8::m_NameRect[8];							//���ֿ��
CRect				CDeskEx8::m_ChairRect[8];							//����λ��
UINT				CDeskEx8::m_TextFormat[8];						//��ʽ����
CPoint				CDeskEx8::m_PtLog[8];								//����λ��
CPoint				CDeskEx8::m_BtnAgree[8];								//ͬ��λ��
int					CDeskEx8::m_draw[8];								//��ʾ˳��
/// ��ɫֵ
Color				CDeskEx8::m_UserNameColor;					///< �����
Color				CDeskEx8::m_VipNameColor;						///< VIP�����
Color				CDeskEx8::m_DeskNoColor;						///< ����������ɫ
Color				CDeskEx8::m_FocusColor;						///< �������ɫ							 //end
//CPoint				CDeskEx7::m_PtLock;								//��λ��
//CPoint				CDeskEx7::m_PtBegin;							//��ʼλ��
//CRect				CDeskEx7::m_IndexRect;							//��Ϸ������
//CPoint				CDeskEx7::m_PtAgree[7];							//ͬ��λ��
//CRect				CDeskEx7::m_NameRect[7];						//���ֿ��
//CRect				CDeskEx7::m_ChairRect[7];						//����λ��
//UINT				CDeskEx7::m_TextFormat[7];						//��ʽ����
//
//CPoint				CDeskEx8::m_PtLock;								//��λ��
//CPoint				CDeskEx8::m_PtBegin;							//��ʼλ��
//CRect				CDeskEx8::m_IndexRect;							//��Ϸ������
//CPoint				CDeskEx8::m_PtAgree[8];							//ͬ��λ��
//CRect				CDeskEx8::m_NameRect[8];						//���ֿ��
//CRect				CDeskEx8::m_ChairRect[8];						//����λ��
//UINT				CDeskEx8::m_TextFormat[8];						//��ʽ����

CPoint				CDeskEx16::m_PtLock;							//��λ��
CPoint				CDeskEx16::m_PtBegin;							//��ʼλ��
CRect				CDeskEx16::m_IndexRect;							//��Ϸ������
CPoint				CDeskEx16::m_PtAgree[16];						//ͬ��λ��
CRect				CDeskEx16::m_NameRect[16];						//���ֿ��
CRect				CDeskEx16::m_ChairRect[16];						//����λ��
UINT				CDeskEx16::m_TextFormat[16];					//��ʽ����
CPoint				CDeskEx16::m_PtLog[16];							//����λ��
CPoint				CDeskEx16::m_BtnAgree[16];						//ͬ��λ��
int					CDeskEx16::m_draw[16];							//��ʾ˳��
/// ��ɫֵ
Color				CDeskEx16::m_UserNameColor;						///< �����
Color				CDeskEx16::m_VipNameColor;						///< VIP�����
Color				CDeskEx16::m_DeskNoColor;						///< ����������ɫ
Color				CDeskEx16::m_FocusColor;						///< �������ɫ

//�ټ��� add by wlr 20090713
CPoint				CDeskBJL::m_PtLock;								//��λ��
CPoint				CDeskBJL::m_PtBasePoint;						//���ӱ���
CPoint				CDeskBJL::m_PtBegin;							//��ʼλ��
CRect				CDeskBJL::m_IndexRect;							//��Ϸ������
CPoint				CDeskBJL::m_PtAgree[180];						//ͬ��λ��
CRect				CDeskBJL::m_NameRect[180];						//���ֿ��
CRect				CDeskBJL::m_ChairRect[180];						//����λ��
UINT				CDeskBJL::m_TextFormat[180];					//��ʽ����
CPoint				CDeskBJL::m_PtLog[180];							//����λ��
CPoint				CDeskBJL::m_BtnAgree[180];						//ͬ��λ��
int					CDeskBJL::m_draw[180];							//��ʾ˳��
/// ��ɫֵ
Color				CDeskBJL::m_UserNameColor;					///< �����
Color				CDeskBJL::m_VipNameColor;						///< VIP�����
Color				CDeskBJL::m_DeskNoColor;						///< ����������ɫ
Color				CDeskBJL::m_FocusColor;						///< �������ɫ

//end of �ټ��� add by wlr 20090713

UINT CBaseDeskView::m_uGameNameID = 0;							//��ϷID

BEGIN_MESSAGE_MAP(CDeskFrameView, CWnd)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(IDC_DESKPAGEUP, OnHitpageup)
	ON_BN_CLICKED(IDC_DESKPAGEDOWN, OnHitpagedown)
	ON_BN_CLICKED(IDC_BEGINAGREE, OnHitBegin)
	ON_BN_CLICKED(IDC_BJLBEGIN, OnHitBJLBegin)//�ټ��� add by wlr 20090713 IDC_BJLPLAYING
	ON_BN_CLICKED(IDC_BJLPLAYING, OnHitBJLBegin)//�ټ��� add by wlr 20090713 IDC_BJLPLAYING

	ON_BN_CLICKED(IDC_QUEUEBEGIN, OnHitQueueBegin)//�ټ��� add by wlr 20090713 IDC_BJLPLAYING
	ON_BN_CLICKED(IDC_QUEUESTOP, OnHitQueueStop)//�ټ��� add by wlr 20090713 IDC_BJLPLAYING

	ON_WM_TIMER()
	//wushuqun 2009.7.13
	//�޸���Ϸ�������˳���Ϸʱ��������
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

//��̬����
int				CDeskFrameView::m_iWhellTimes=2;							//���ֱ���
int				CDeskFrameView::m_iScrollPos=10;							//��������
/*******************************************************************************************************/

//���캯��
CBaseDeskView::CBaseDeskView()
{
	m_bLock=false;	//�Ƿ���
	m_bDeskIndex=255;//��Ϸ��������
	m_bHoverIndex=255;//�������
	m_bPlayGame=false;//�Ƿ���Ϸ
	m_iBasePoint=0;
	m_bDownMouse=false;//�Ƿ������
	m_bFocusFrame=false;//������
	m_deskClass="";
	m_deskFlat=true;
	memset(m_pUserItem,0,sizeof(m_pUserItem));//�û�����
	memset(m_MatchInfo,0,sizeof(m_MatchInfo));//������Ϣ
	m_pFont = new Font(L"����", 10, FontStyleRegular);
	m_bCanInput = true;
}

//��������
CBaseDeskView::~CBaseDeskView()
{
	delete m_pFont;
	m_pFont = NULL;
}

//��ʼ���������������ӵ�λ��
bool CBaseDeskView:: InitDesk(CString deskClass,bool deskFlat,UINT uNameID, bool bIsVip)
{
	bool bInit=false;
	int x1,y1,x2,y2;
	CString s=CBcfFile::GetAppPath ();/////����·��
	CString path;
	// Modified by ZXD ͬʱ֧�����ַ�ʽ��Ϸ�ͻ���
	// Ҫ�жϵ�����ico����exe����û�в������룬ֻ���ñ��취���������ļ�������exe��ʽ������ico��ʽ�ļ�
	bool bIsIco = false;
	//path.Format("image\\client\\alluse\\3d\\deskconfig.bcf");
	path.Format("%d\\config\\%s\\deskconfig.bcf",uNameID,deskClass); ///<add by wxx ;
	if (CBcfFile::IsFileExist(s + path))
	{
		bIsIco = false;
	}
	else
	{
		//path.Format("image\\client\\alluse\\3d\\deskconfig.bcf");
		path.Format("config\\%s\\deskconfig.bcf",deskClass); ///<add by wxx ;
		bIsIco = true;
	}
	/// ״̬�����ļ�
	CBcfFile f( s + path);
	CString key="";

	m_uGameNameID = uNameID;

	/// ��ʼ�����������ɫֵ
	CString strPrivatePath;
	strPrivatePath.Format("%s%d\\DeskBackGround.bcf", s, uNameID);
	if (CBcfFile::IsFileExist(strPrivatePath))
	{
		path = strPrivatePath;
	}
	else
	{
		path.Format("%sconfig\\%s\\deskconfig.bcf", s,deskClass); ///<add by wxx ;
	}
	CBcfFile fbcf(path);
	{
		BYTE r = fbcf.GetKeyVal("DeskColors", "UserName_R", 255);
		BYTE g = fbcf.GetKeyVal("DeskColors", "UserName_G", 255);
		BYTE b = fbcf.GetKeyVal("DeskColors", "UserName_B", 255);
		CDesk1::m_UserNameColor = Color(r,g,b);
		CDesk2::m_UserNameColor = Color(r,g,b);
		CDesk3::m_UserNameColor = Color(r,g,b);
		CDesk4::m_UserNameColor = Color(r,g,b);
		CDesk5::m_UserNameColor = Color(r,g,b);
		CDesk6::m_UserNameColor = Color(r,g,b);
		CDesk8::m_UserNameColor = Color(r,g,b);
		CDesk16::m_UserNameColor = Color(r,g,b);
		r = fbcf.GetKeyVal("DeskColors", "VipName_R", 255);
		g = fbcf.GetKeyVal("DeskColors", "VipName_G", 255);
		b = fbcf.GetKeyVal("DeskColors", "VipName_B", 255);
		CDesk1::m_VipNameColor = Color(r,g,b);
		CDesk2::m_VipNameColor = Color(r,g,b);
		CDesk3::m_VipNameColor = Color(r,g,b);
		CDesk4::m_VipNameColor = Color(r,g,b);
		CDesk5::m_VipNameColor = Color(r,g,b);
		CDesk6::m_VipNameColor = Color(r,g,b);
		CDesk8::m_VipNameColor = Color(r,g,b);
		CDesk16::m_VipNameColor = Color(r,g,b);
		r = fbcf.GetKeyVal("DeskColors", "DeskNo_R", 255);
		g = fbcf.GetKeyVal("DeskColors", "DeskNo_G", 255);
		b = fbcf.GetKeyVal("DeskColors", "DeskNo_B", 255);
		CDesk1::m_DeskNoColor  = Color(r,g,b);
		CDesk2::m_DeskNoColor  = Color(r,g,b);
		CDesk3::m_DeskNoColor  = Color(r,g,b);
		CDesk4::m_DeskNoColor  = Color(r,g,b);
		CDesk5::m_DeskNoColor  = Color(r,g,b);
		CDesk6::m_DeskNoColor  = Color(r,g,b);
		CDesk8::m_DeskNoColor  = Color(r,g,b);
		CDesk16::m_DeskNoColor  = Color(r,g,b);
		r = fbcf.GetKeyVal("DeskColors", "FocusColor_R", 255);
		g = fbcf.GetKeyVal("DeskColors", "FocusColor_G", 255);
		b = fbcf.GetKeyVal("DeskColors", "FocusColor_B", 255);
		CDesk1::m_FocusColor   = Color(r,g,b);
		CDesk2::m_FocusColor   = Color(r,g,b);
		CDesk3::m_FocusColor   = Color(r,g,b);
		CDesk4::m_FocusColor   = Color(r,g,b);
		CDesk5::m_FocusColor   = Color(r,g,b);
		CDesk6::m_FocusColor   = Color(r,g,b);
		CDesk8::m_FocusColor   = Color(r,g,b);
		CDesk16::m_FocusColor   = Color(r,g,b);
	}

	/// ����ͬ��������
	if (bInit==false)
	{
		bInit=true;

		//��ʼ�� 1������
		//CDesk1::m_PtLock.SetPoint(25,20);
		////CDesk1::m_PtBasePoint.SetPoint(25,70);							//���ӱ���
		//CDesk1::m_PtBegin.SetPoint(78,65);//��������Ϸ������
		//CDesk1::m_IndexRect.SetRect(24,152,50,180);
		//CDesk1::m_PtAgree[0].SetPoint(87,60);
		//CDesk1::m_NameRect[0].SetRect(115,135,200,200);
		//CDesk1::m_ChairRect[0].SetRect(65,67,120,150);//���ϣ��ң���
		CDesk1::m_TextFormat[0]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_CENTER;

		key="Desk1";
		x1=f.GetKeyVal(key,"lockx",25);
		y1=f.GetKeyVal(key,"locky",20);
		CDesk1::m_PtLock.SetPoint(x1,y1);
	/*	x1=f.GetKeyVal(key,"basePointx",25);
		y1=f.GetKeyVal(key,"basePointy",140);
		CDesk1::m_PtBasePoint.SetPoint(x1,y1);*/
		CDesk1::m_PtBegin.SetPoint(94, 98);//��������Ϸ������
		x1=f.GetKeyVal(key,"indexx1",25);
		y1=f.GetKeyVal(key,"indexy1",147);
		x2=f.GetKeyVal(key,"indexx2",143);
		y2=f.GetKeyVal(key,"indexy2",165);
		CDesk1::m_IndexRect.SetRect(x1,y1,x2,y2);
		x1=f.GetKeyVal(key,"agree0x",190);
		y1=f.GetKeyVal(key,"agree0y",31);
		CDesk1::m_PtAgree[0].SetPoint(x1, y1);
	   //���ƿ��
		x1=f.GetKeyVal(key,"name0x1",31);
		y1=f.GetKeyVal(key,"name0y1",43);
		x2=f.GetKeyVal(key,"name0x2",x1+80);
		y2=f.GetKeyVal(key,"name0y2",y2+16);
		CDesk1::m_NameRect[0].SetRect(x1,y1,x2,y2);//��
		x1=f.GetKeyVal(key,"chair0x1",181);
		y1=f.GetKeyVal(key,"chair0y1",35);
		x2=f.GetKeyVal(key,"chair0x2",209);
		y2=f.GetKeyVal(key,"chair0y2",110);
		CDesk1::m_ChairRect[0].SetRect(x1, y1,  x2, y2);//��
		
		x1=f.GetKeyVal(key,"log0x",131);
		y1=f.GetKeyVal(key,"log0y",31);
		CDesk1::m_PtLog[0].SetPoint(x1, y1);
		CString str;
		str.Format("Desk:x1=%d y1=%d",x1,y1);
		OutputDebugString(str);

		//��ʼ�� 2 ������
		if (bIsVip)
		{
			key="vipDesk";
		}
		else
		{
			key="Desk2";
		}
		x1=f.GetKeyVal(key,"lockx",25);
		y1=f.GetKeyVal(key,"locky",20);
		CDesk2::m_PtLock.SetPoint(x1,y1);
		x1=f.GetKeyVal(key,"basePointx",25);
		y1=f.GetKeyVal(key,"basePointy",140);
		CDesk2::m_PtBasePoint.SetPoint(x1,y1);
		CDesk2::m_PtBegin.SetPoint(94, 98);//��������Ϸ������
		x1=f.GetKeyVal(key,"indexx1",25);
		y1=f.GetKeyVal(key,"indexy1",147);
		x2=f.GetKeyVal(key,"indexx2",143);
		y2=f.GetKeyVal(key,"indexy2",165);
		CDesk2::m_IndexRect.SetRect(x1,y1,x2,y2);
		x1=f.GetKeyVal(key,"agree0x",190);
		y1=f.GetKeyVal(key,"agree0y",31);
		CDesk2::m_PtAgree[0].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"agree1x",31);
		y1=f.GetKeyVal(key,"agree1y",43);
		CDesk2::m_PtAgree[1].SetPoint(x1, y1);
		//���ƿ��
		x1=f.GetKeyVal(key,"name0x1",31);
		y1=f.GetKeyVal(key,"name0y1",43);
		x2=f.GetKeyVal(key,"name0x2",x1+80);
		y2=f.GetKeyVal(key,"name0y2",y2+16);
		CDesk2::m_NameRect[0].SetRect(x1,y1,x2,y2);//��
		x1=f.GetKeyVal(key,"name1x1",31);
		y1=f.GetKeyVal(key,"name1y1",43);
		x2=f.GetKeyVal(key,"name1x2",x1+80);
		y2=f.GetKeyVal(key,"name1y2",y2+16);
		CDesk2::m_NameRect[1].SetRect(x1,y1,x2,y2);//��		

		x1=f.GetKeyVal(key,"chair0x1",181);
		y1=f.GetKeyVal(key,"chair0y1",35);
		x2=f.GetKeyVal(key,"chair0x2",209);
		y2=f.GetKeyVal(key,"chair0y2",110);
		CDesk2::m_ChairRect[0].SetRect(x1, y1,  x2, y2);//��
		x1=f.GetKeyVal(key,"chair1x1",36);
		y1=f.GetKeyVal(key,"chair1y1",60);
		x2=f.GetKeyVal(key,"chair1x2",71);
		y2=f.GetKeyVal(key,"chair1y2",120);
		CDesk2::m_ChairRect[1].SetRect(x1, y1,  x2, y2);//��
		x1=f.GetKeyVal(key,"log0x",131);
		y1=f.GetKeyVal(key,"log0y",31);
		CDesk2::m_PtLog[0].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"log1x",36);
		y1=f.GetKeyVal(key,"log1y",43);
		CDesk2::m_PtLog[1].SetPoint(x1, y1);
		CDesk2::m_draw[0]=0;
		CDesk2::m_draw[1]=1;

		//��ʼ�� 3 ������012�ֱ�Ϊ���ң���
		key="Desk3";
		x1=f.GetKeyVal(key,"lockx",25);
		y1=f.GetKeyVal(key,"locky",20);
		CDesk3::m_PtLock.SetPoint(x1,y1);
		x1=f.GetKeyVal(key,"basePointx",25);
		y1=f.GetKeyVal(key,"basePointy",140);
		CDesk3::m_PtBasePoint.SetPoint(x1,y1);
		CDesk3::m_PtBegin.SetPoint(94, 98);//��������Ϸ������
		x1=f.GetKeyVal(key,"indexx1",25);
		y1=f.GetKeyVal(key,"indexy1",147);
		x2=f.GetKeyVal(key,"indexx2",143);
		y2=f.GetKeyVal(key,"indexy2",165);
		CDesk3::m_IndexRect.SetRect(x1,y1,x2,y2);
		x1=f.GetKeyVal(key,"agree0x",190);
		y1=f.GetKeyVal(key,"agree0y",31);
		CDesk3::m_PtAgree[0].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"agree1x",97);
		y1=f.GetKeyVal(key,"agree1y",148);
		CDesk3::m_PtAgree[1].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"agree2x",31);
		y1=f.GetKeyVal(key,"agree2y",43);
		CDesk3::m_PtAgree[2].SetPoint(x1, y1);
		//���ֿ��λ��		
		x1=f.GetKeyVal(key,"name0x1",31);
		y1=f.GetKeyVal(key,"name0y1",43);
		x2=f.GetKeyVal(key,"name0x2",x1+80);
		y2=f.GetKeyVal(key,"name0y2",y2+16);
		CDesk3::m_NameRect[0].SetRect(x1,y1,x2,y2);//��
		x1=f.GetKeyVal(key,"name1x1",31);
		y1=f.GetKeyVal(key,"name1y1",43);
		x2=f.GetKeyVal(key,"name1x2",x1+80);
		y2=f.GetKeyVal(key,"name1y2",y2+16);
		CDesk3::m_NameRect[1].SetRect(x1,y1,x2,y2);//��
		x1=f.GetKeyVal(key,"name2x1",31);
		y1=f.GetKeyVal(key,"name2y1",43);
		x2=f.GetKeyVal(key,"name2x2",x1+80);
		y2=f.GetKeyVal(key,"name2y2",y2+16);
		CDesk3::m_NameRect[2].SetRect(x1,y1,x2,y2);//��

		x1=f.GetKeyVal(key,"chair0x1",181);
		y1=f.GetKeyVal(key,"chair0y1",35);
		x2=f.GetKeyVal(key,"chair0x2",209);
		y2=f.GetKeyVal(key,"chair0y2",110);
		CDesk3::m_ChairRect[0].SetRect(x1, y1,  x2, y2);//��
		x1=f.GetKeyVal(key,"chair1x1",112);
		y1=f.GetKeyVal(key,"chair1y1",92);
		x2=f.GetKeyVal(key,"chair1x2",171);
		y2=f.GetKeyVal(key,"chair1y2",180);
		CDesk3::m_ChairRect[1].SetRect(x1, y1,  x2, y2);//���·�
		x1=f.GetKeyVal(key,"chair2x1",36);
		y1=f.GetKeyVal(key,"chair2y1",60);
		x2=f.GetKeyVal(key,"chair2x2",71);
		y2=f.GetKeyVal(key,"chair2y2",120);
		CDesk3::m_ChairRect[2].SetRect(x1, y1,  x2, y2);//��
		x1=f.GetKeyVal(key,"log0x",131);
		y1=f.GetKeyVal(key,"log0y",31);
		CDesk3::m_PtLog[0].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"log1x",98);
		y1=f.GetKeyVal(key,"log1y",52);
		CDesk3::m_PtLog[1].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"log2x",36);
		y1=f.GetKeyVal(key,"log2y",43);
		CDesk3::m_PtLog[2].SetPoint(x1, y1);
		CDesk3::m_draw[0]=0;
		CDesk3::m_draw[1]=2;
		CDesk3::m_draw[2]=1;

		//��ʼ�� 4 ������0123�ֱ�Ϊ�ң���,���ϣ�
		key="Desk4";
		x1=f.GetKeyVal(key,"lockx",25);
		y1=f.GetKeyVal(key,"locky",20);
		CDesk4::m_PtLock.SetPoint(x1,y1);
		x1=f.GetKeyVal(key,"basePointx",25);
		y1=f.GetKeyVal(key,"basePointy",140);
		CDesk4::m_PtBasePoint.SetPoint(x1,y1);
		CDesk4::m_PtBegin.SetPoint(94, 98);//��������Ϸ������
		x1=f.GetKeyVal(key,"indexx1",25);
		y1=f.GetKeyVal(key,"indexy1",147);
		x2=f.GetKeyVal(key,"indexx2",143);
		y2=f.GetKeyVal(key,"indexy2",165);
		CDesk4::m_IndexRect.SetRect(x1,y1,x2,y2);
		x1=f.GetKeyVal(key,"agree0x",140);
		y1=f.GetKeyVal(key,"agree0y",31);
		CDesk4::m_PtAgree[0].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"agree1x",97);
		y1=f.GetKeyVal(key,"agree1y",148);
		CDesk4::m_PtAgree[1].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"agree2x",31);
		y1=f.GetKeyVal(key,"agree2y",43);
		CDesk4::m_PtAgree[2].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"agree3x",71);
		y1=f.GetKeyVal(key,"agree3y",29);
		CDesk4::m_PtAgree[3].SetPoint(x1, y1);
		//���ֿ��λ��		
		x1=f.GetKeyVal(key,"name0x1",31);
		y1=f.GetKeyVal(key,"name0y1",43);
		x2=f.GetKeyVal(key,"name0x2",x1+80);
		y2=f.GetKeyVal(key,"name0y2",y2+16);
		CDesk4::m_NameRect[0].SetRect(x1,y1,x2,y2);//��
		x1=f.GetKeyVal(key,"name1x1",31);
		y1=f.GetKeyVal(key,"name1y1",43);
		x2=f.GetKeyVal(key,"name1x2",x1+80);
		y2=f.GetKeyVal(key,"name1y2",y2+16);
		CDesk4::m_NameRect[1].SetRect(x1,y1,x2,y2);//��
		x1=f.GetKeyVal(key,"name2x1",31);
		y1=f.GetKeyVal(key,"name2y1",43);
		x2=f.GetKeyVal(key,"name2x2",x1+80);
		y2=f.GetKeyVal(key,"name2y2",y2+16);
		CDesk4::m_NameRect[2].SetRect(x1,y1,x2,y2);//��
		x1=f.GetKeyVal(key,"name3x1",31);
		y1=f.GetKeyVal(key,"name3y1",43);
		x2=f.GetKeyVal(key,"name3x2",x1+80);
		y2=f.GetKeyVal(key,"name3y2",y2+16);
		CDesk4::m_NameRect[3].SetRect(x1,y1,x2,y2);//��

		x1=f.GetKeyVal(key,"chair0x1",181);
		y1=f.GetKeyVal(key,"chair0y1",35);
		x2=f.GetKeyVal(key,"chair0x2",209);
		y2=f.GetKeyVal(key,"chair0y2",110);
		CDesk4::m_ChairRect[0].SetRect(x1, y1,  x2, y2);//��
		x1=f.GetKeyVal(key,"chair1x1",112);
		y1=f.GetKeyVal(key,"chair1y1",92);
		x2=f.GetKeyVal(key,"chair1x2",171);
		y2=f.GetKeyVal(key,"chair1y2",180);
		CDesk4::m_ChairRect[1].SetRect(x1, y1,  x2, y2);//���·�
		x1=f.GetKeyVal(key,"chair2x1",36);
		y1=f.GetKeyVal(key,"chair2y1",60);
		x2=f.GetKeyVal(key,"chair2x2",71);
		y2=f.GetKeyVal(key,"chair2y2",120);
		CDesk4::m_ChairRect[2].SetRect(x1, y1,  x2, y2);//��
		x1=f.GetKeyVal(key,"chair3x1",85);
		y1=f.GetKeyVal(key,"chair3y1",35);
		x2=f.GetKeyVal(key,"chair3x2",105);
		y2=f.GetKeyVal(key,"chair3y2",82);
		CDesk4::m_ChairRect[3].SetRect(x1, y1,  x2, y2);//���Ϸ�
		x1=f.GetKeyVal(key,"log0x",131);
		y1=f.GetKeyVal(key,"log0y",31);
		CDesk4::m_PtLog[0].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"log1x",98);
		y1=f.GetKeyVal(key,"log1y",52);
		CDesk4::m_PtLog[1].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"log2x",36);
		y1=f.GetKeyVal(key,"log2y",43);
		CDesk4::m_PtLog[2].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"log3x",71);
		y1=f.GetKeyVal(key,"log3y",29);
		CDesk4::m_PtLog[3].SetPoint(x1, y1);
		CDesk4::m_draw[0]=3;
		CDesk4::m_draw[1]=0;
		CDesk4::m_draw[2]=2;
		CDesk4::m_draw[3]=1;

		CDesk4::m_TextFormat[0]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_RIGHT;//��
		CDesk4::m_TextFormat[1]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_CENTER;//��
		CDesk4::m_TextFormat[2]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_CENTER;//��
		CDesk4::m_TextFormat[3]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_LEFT;//��

		//��ʼ�� 5 ������
		key="Desk5";
		x1=f.GetKeyVal(key,"lockx",25);
		y1=f.GetKeyVal(key,"locky",20);
		CDesk5::m_PtLock.SetPoint(x1,y1);
		x1=f.GetKeyVal(key,"basePointx",25);
		y1=f.GetKeyVal(key,"basePointy",140);
		CDesk5::m_PtBasePoint.SetPoint(x1,y1);
		CDesk5::m_PtBegin.SetPoint(93,103);//��������Ϸ������
		x1=f.GetKeyVal(key,"indexx1",25);
		y1=f.GetKeyVal(key,"indexy1",147);
		x2=f.GetKeyVal(key,"indexx2",143);
		y2=f.GetKeyVal(key,"indexy2",165);
		CDesk5::m_IndexRect.SetRect(x1,y1,x2,y2);
		x1=f.GetKeyVal(key,"agree0x",190);
		y1=f.GetKeyVal(key,"agree0y",31);
		CDesk5::m_PtAgree[0].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"agree1x",97);
		y1=f.GetKeyVal(key,"agree1y",148);
		CDesk5::m_PtAgree[1].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"agree2x",31);
		y1=f.GetKeyVal(key,"agree2y",43);
		CDesk5::m_PtAgree[2].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"agree3x",71);
		y1=f.GetKeyVal(key,"agree3y",29);
		CDesk5::m_PtAgree[3].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"agree4x",91);
		y1=f.GetKeyVal(key,"agree4y",29);
		CDesk5::m_PtAgree[4].SetPoint(x1, y1);

		x1=f.GetKeyVal(key,"name0x1",31);
		y1=f.GetKeyVal(key,"name0y1",43);
		x2=f.GetKeyVal(key,"name0x2",x1+80);
		y2=f.GetKeyVal(key,"name0y2",y2+16);
		CDesk5::m_NameRect[0].SetRect(x1,y1,x2,y2);
		x1=f.GetKeyVal(key,"name1x1",31);
		y1=f.GetKeyVal(key,"name1y1",43);
		x2=f.GetKeyVal(key,"name1x2",x1+80);
		y2=f.GetKeyVal(key,"name1y2",y2+16);
		CDesk5::m_NameRect[1].SetRect(x1,y1,x2,y2);
		x1=f.GetKeyVal(key,"name2x1",31);
		y1=f.GetKeyVal(key,"name2y1",43);
		x2=f.GetKeyVal(key,"name2x2",x1+80);
		y2=f.GetKeyVal(key,"name2y2",y2+16);
		CDesk5::m_NameRect[2].SetRect(x1,y1,x2,y2);
		x1=f.GetKeyVal(key,"name3x1",31);
		y1=f.GetKeyVal(key,"name3y1",43);
		x2=f.GetKeyVal(key,"name3x2",x1+80);
		y2=f.GetKeyVal(key,"name3y2",y2+16);
		CDesk5::m_NameRect[3].SetRect(x1,y1,x2,y2);
		x1=f.GetKeyVal(key,"name4x1",31);
		y1=f.GetKeyVal(key,"name4y1",43);
		x2=f.GetKeyVal(key,"name4x2",x1+80);
		y2=f.GetKeyVal(key,"name4y2",y2+16);
		CDesk5::m_NameRect[4].SetRect(x1,y1,x2,y2);
		x1=f.GetKeyVal(key,"chair0x1",181);
		y1=f.GetKeyVal(key,"chair0y1",35);
		x2=f.GetKeyVal(key,"chair0x2",209);
		y2=f.GetKeyVal(key,"chair0y2",110);
		CDesk5::m_ChairRect[0].SetRect(x1, y1,  x2, y2);//��
		x1=f.GetKeyVal(key,"chair1x1",112);
		y1=f.GetKeyVal(key,"chair1y1",92);
		x2=f.GetKeyVal(key,"chair1x2",171);
		y2=f.GetKeyVal(key,"chair1y2",180);
		CDesk5::m_ChairRect[1].SetRect(x1, y1,  x2, y2);//���·�
		x1=f.GetKeyVal(key,"chair2x1",36);
		y1=f.GetKeyVal(key,"chair2y1",60);
		x2=f.GetKeyVal(key,"chair2x2",71);
		y2=f.GetKeyVal(key,"chair2y2",120);
		CDesk5::m_ChairRect[2].SetRect(x1, y1,  x2, y2);//��
		x1=f.GetKeyVal(key,"chair3x1",85);
		y1=f.GetKeyVal(key,"chair3y1",35);
		x2=f.GetKeyVal(key,"chair3x2",105);
		y2=f.GetKeyVal(key,"chair3y2",82);
		CDesk5::m_ChairRect[3].SetRect(x1, y1,  x2, y2);//���Ϸ�
		x1=f.GetKeyVal(key,"chair4x1",95);
		y1=f.GetKeyVal(key,"chair4y1",35);
		x2=f.GetKeyVal(key,"chair4x2",105);
		y2=f.GetKeyVal(key,"chair4y2",82);
		CDesk5::m_ChairRect[4].SetRect(x1, y1,  x2, y2);//���Ϸ�
		x1=f.GetKeyVal(key,"log0x",131);
		y1=f.GetKeyVal(key,"log0y",31);
		CDesk5::m_PtLog[0].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"log1x",98);
		y1=f.GetKeyVal(key,"log1y",52);
		CDesk5::m_PtLog[1].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"log2x",36);
		y1=f.GetKeyVal(key,"log2y",43);
		CDesk5::m_PtLog[2].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"log3x",71);
		y1=f.GetKeyVal(key,"log3y",29);
		CDesk5::m_PtLog[3].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"log4x",91);
		y1=f.GetKeyVal(key,"log4y",29);
		CDesk5::m_PtLog[4].SetPoint(x1, y1);
		CDesk5::m_draw[0]=0;
		CDesk5::m_draw[1]=1;
		CDesk5::m_draw[2]=2;
		CDesk5::m_draw[3]=3;
		CDesk5::m_draw[4]=4;
		CDesk5::m_TextFormat[0]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_LEFT;
		CDesk5::m_TextFormat[1]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_CENTER;
		CDesk5::m_TextFormat[2]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_RIGHT;
		CDesk5::m_TextFormat[3]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_CENTER;
		CDesk5::m_TextFormat[4]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_CENTER;

		//��ʼ�� 6 ������
		key="Desk6";
		x1=f.GetKeyVal(key,"lockx",25);
		y1=f.GetKeyVal(key,"locky",20);
		CDesk6::m_PtLock.SetPoint(x1,y1);
		x1=f.GetKeyVal(key,"basePointx",25);
		y1=f.GetKeyVal(key,"basePointy",70);
		CDesk6::m_PtBasePoint.SetPoint(x1,y1);
		CDesk6::m_PtBegin.SetPoint(93,103);//��������Ϸ������
		x1=f.GetKeyVal(key,"indexx1",25);
		y1=f.GetKeyVal(key,"indexy1",147);
		x2=f.GetKeyVal(key,"indexx2",143);
		y2=f.GetKeyVal(key,"indexy2",165);
		CDesk6::m_IndexRect.SetRect(x1,y1,x2,y2);
		x1=f.GetKeyVal(key,"agree0x",190);
		y1=f.GetKeyVal(key,"agree0y",31);
		CDesk6::m_PtAgree[0].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"agree1x",97);
		y1=f.GetKeyVal(key,"agree1y",148);
		CDesk6::m_PtAgree[1].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"agree2x",31);
		y1=f.GetKeyVal(key,"agree2y",43);
		CDesk6::m_PtAgree[2].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"agree3x",71);
		y1=f.GetKeyVal(key,"agree3y",29);
		CDesk6::m_PtAgree[3].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"agree4x",91);
		y1=f.GetKeyVal(key,"agree4y",29);
		CDesk6::m_PtAgree[4].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"agree5x",91);
		y1=f.GetKeyVal(key,"agree5y",29);
		CDesk6::m_PtAgree[5].SetPoint(x1, y1);

		x1=f.GetKeyVal(key,"name0x1",31);
		y1=f.GetKeyVal(key,"name0y1",43);
		x2=f.GetKeyVal(key,"name0x2",x1+80);
		y2=f.GetKeyVal(key,"name0y2",y2+16);
		CDesk6::m_NameRect[0].SetRect(x1,y1,x2,y2);
		x1=f.GetKeyVal(key,"name1x1",31);
		y1=f.GetKeyVal(key,"name1y1",43);
		x2=f.GetKeyVal(key,"name1x2",x1+80);
		y2=f.GetKeyVal(key,"name1y2",y2+16);
		CDesk6::m_NameRect[1].SetRect(x1,y1,x2,y2);
		x1=f.GetKeyVal(key,"name2x1",31);
		y1=f.GetKeyVal(key,"name2y1",43);
		x2=f.GetKeyVal(key,"name2x2",x1+80);
		y2=f.GetKeyVal(key,"name2y2",y2+16);
		CDesk6::m_NameRect[2].SetRect(x1,y1,x2,y2);
		x1=f.GetKeyVal(key,"name3x1",31);
		y1=f.GetKeyVal(key,"name3y1",43);
		x2=f.GetKeyVal(key,"name3x2",x1+80);
		y2=f.GetKeyVal(key,"name3y2",y2+16);
		CDesk6::m_NameRect[3].SetRect(x1,y1,x2,y2);
		x1=f.GetKeyVal(key,"name4x1",31);
		y1=f.GetKeyVal(key,"name4y1",43);
		x2=f.GetKeyVal(key,"name4x2",x1+80);
		y2=f.GetKeyVal(key,"name4y2",y2+16);
		CDesk6::m_NameRect[4].SetRect(x1,y1,x2,y2);
		x1=f.GetKeyVal(key,"name5x1",31);
		y1=f.GetKeyVal(key,"name5y1",43);
		x2=f.GetKeyVal(key,"name5x2",x1+80);
		y2=f.GetKeyVal(key,"name5y2",y2+16);
		CDesk6::m_NameRect[5].SetRect(x1,y1,x2,y2);
		x1=f.GetKeyVal(key,"chair0x1",181);
		y1=f.GetKeyVal(key,"chair0y1",35);
		x2=f.GetKeyVal(key,"chair0x2",209);
		y2=f.GetKeyVal(key,"chair0y2",110);
		CDesk6::m_ChairRect[0].SetRect(x1, y1,  x2, y2);//��
		x1=f.GetKeyVal(key,"chair1x1",112);
		y1=f.GetKeyVal(key,"chair1y1",92);
		x2=f.GetKeyVal(key,"chair1x2",171);
		y2=f.GetKeyVal(key,"chair1y2",180);
		CDesk6::m_ChairRect[1].SetRect(x1, y1,  x2, y2);//���·�
		x1=f.GetKeyVal(key,"chair2x1",36);
		y1=f.GetKeyVal(key,"chair2y1",60);
		x2=f.GetKeyVal(key,"chair2x2",71);
		y2=f.GetKeyVal(key,"chair2y2",120);
		CDesk6::m_ChairRect[2].SetRect(x1, y1,  x2, y2);//��
		x1=f.GetKeyVal(key,"chair3x1",85);
		y1=f.GetKeyVal(key,"chair3y1",35);
		x2=f.GetKeyVal(key,"chair3x2",105);
		y2=f.GetKeyVal(key,"chair3y2",82);
		CDesk6::m_ChairRect[3].SetRect(x1, y1,  x2, y2);//���Ϸ�
		x1=f.GetKeyVal(key,"chair4x1",95);
		y1=f.GetKeyVal(key,"chair4y1",35);
		x2=f.GetKeyVal(key,"chair4x2",105);
		y2=f.GetKeyVal(key,"chair4y2",82);
		CDesk6::m_ChairRect[4].SetRect(x1, y1,  x2, y2);//���Ϸ�
		x1=f.GetKeyVal(key,"chair5x1",95);
		y1=f.GetKeyVal(key,"chair5y1",35);
		x2=f.GetKeyVal(key,"chair5x2",105);
		y2=f.GetKeyVal(key,"chair5y2",82);
		CDesk6::m_ChairRect[5].SetRect(x1, y1,  x2, y2);//���Ϸ�
		x1=f.GetKeyVal(key,"log0x",131);
		y1=f.GetKeyVal(key,"log0y",31);
		CDesk6::m_PtLog[0].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"log1x",98);
		y1=f.GetKeyVal(key,"log1y",52);
		CDesk6::m_PtLog[1].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"log2x",36);
		y1=f.GetKeyVal(key,"log2y",43);
		CDesk6::m_PtLog[2].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"log3x",71);
		y1=f.GetKeyVal(key,"log3y",29);
		CDesk6::m_PtLog[3].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"log4x",91);
		y1=f.GetKeyVal(key,"log4y",29);
		CDesk6::m_PtLog[4].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"log5x",91);
		y1=f.GetKeyVal(key,"log5y",29);
		CDesk6::m_PtLog[5].SetPoint(x1, y1);
		CDesk6::m_draw[0]=0;
		CDesk6::m_draw[1]=1;
		CDesk6::m_draw[2]=2;
		CDesk6::m_draw[3]=3;
		CDesk6::m_draw[4]=4;
		CDesk6::m_draw[5]=5;
		CDesk6::m_TextFormat[0]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_CENTER;
		CDesk6::m_TextFormat[1]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_CENTER;
		CDesk6::m_TextFormat[2]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_LEFT;
		CDesk6::m_TextFormat[3]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_CENTER;
		CDesk6::m_TextFormat[4]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_CENTER;
		CDesk6::m_TextFormat[5]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_CENTER;


		//��ʼ��8����

		key="Desk8";
		x1=f.GetKeyVal(key,"lockx",25);
		y1=f.GetKeyVal(key,"locky",20);
		CDesk8::m_PtLock.SetPoint(x1,y1);
		x1=f.GetKeyVal(key,"basePointx",25);
		y1=f.GetKeyVal(key,"basePointy",70);
		CDesk8::m_PtBasePoint.SetPoint(x1,y1);
		CDesk8::m_PtBegin.SetPoint(93,103);//��������Ϸ������
		x1=f.GetKeyVal(key,"indexx1",25);
		y1=f.GetKeyVal(key,"indexy1",147);
		x2=f.GetKeyVal(key,"indexx2",143);
		y2=f.GetKeyVal(key,"indexy2",165);
		CDesk8::m_IndexRect.SetRect(x1,y1,x2,y2);
		x1=f.GetKeyVal(key,"agree0x",190);
		y1=f.GetKeyVal(key,"agree0y",31);
		CDesk8::m_PtAgree[0].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"agree1x",97);
		y1=f.GetKeyVal(key,"agree1y",148);
		CDesk8::m_PtAgree[1].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"agree2x",31);
		y1=f.GetKeyVal(key,"agree2y",43);
		CDesk8::m_PtAgree[2].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"agree3x",71);
		y1=f.GetKeyVal(key,"agree3y",29);
		CDesk8::m_PtAgree[3].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"agree4x",91);
		y1=f.GetKeyVal(key,"agree4y",29);
		CDesk8::m_PtAgree[4].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"agree5x",91);
		y1=f.GetKeyVal(key,"agree5y",29);
		CDesk8::m_PtAgree[5].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"agree6x",91);
		y1=f.GetKeyVal(key,"agree6y",29);
		CDesk8::m_PtAgree[6].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"agree7x",91);
		y1=f.GetKeyVal(key,"agree7y",29);
		CDesk8::m_PtAgree[7].SetPoint(x1, y1);

		x1=f.GetKeyVal(key,"name0x1",31+169);
		y1=f.GetKeyVal(key,"name0y1",43-11);
		x2=f.GetKeyVal(key,"name0x2",x1+80+169);
		y2=f.GetKeyVal(key,"name0y2",y2+16-11);
		CDesk8::m_NameRect[0].SetRect(x1,y1,x2,y2);
		x1=f.GetKeyVal(key,"name1x1",31);
		y1=f.GetKeyVal(key,"name1y1",43);
		x2=f.GetKeyVal(key,"name1x2",x1+80);
		y2=f.GetKeyVal(key,"name1y2",y2+16);
		CDesk8::m_NameRect[1].SetRect(x1,y1,x2,y2);
		x1=f.GetKeyVal(key,"name2x1",31);
		y1=f.GetKeyVal(key,"name2y1",43);
		x2=f.GetKeyVal(key,"name2x2",x1+80);
		y2=f.GetKeyVal(key,"name2y2",y2+16);
		CDesk8::m_NameRect[2].SetRect(x1,y1,x2,y2);
		x1=f.GetKeyVal(key,"name3x1",31);
		y1=f.GetKeyVal(key,"name3y1",43);
		x2=f.GetKeyVal(key,"name3x2",x1+80);
		y2=f.GetKeyVal(key,"name3y2",y2+16);
		CDesk8::m_NameRect[3].SetRect(x1,y1,x2,y2);
		x1=f.GetKeyVal(key,"name4x1",31);
		y1=f.GetKeyVal(key,"name4y1",43);
		x2=f.GetKeyVal(key,"name4x2",x1+80);
		y2=f.GetKeyVal(key,"name4y2",y2+16);
		CDesk8::m_NameRect[4].SetRect(x1,y1,x2,y2);
		x1=f.GetKeyVal(key,"name5x1",31);
		y1=f.GetKeyVal(key,"name5y1",43);
		x2=f.GetKeyVal(key,"name5x2",x1+80);
		y2=f.GetKeyVal(key,"name5y2",y2+16);
		CDesk8::m_NameRect[5].SetRect(x1,y1,x2,y2);
		x1=f.GetKeyVal(key,"name6x1",31);
		y1=f.GetKeyVal(key,"name6y1",43);
		x2=f.GetKeyVal(key,"name6x2",x1+80);
		y2=f.GetKeyVal(key,"name6y2",y2+16);
		CDesk8::m_NameRect[6].SetRect(x1,y1,x2,y2);
		x1=f.GetKeyVal(key,"name7x1",31+210);
		y1=f.GetKeyVal(key,"name7y1",43+40);
		x2=f.GetKeyVal(key,"name7x2",x1+80+210);
		y2=f.GetKeyVal(key,"name7y2",y2+16+40);
		CDesk8::m_NameRect[7].SetRect(x1,y1,x2,y2);

		x1=f.GetKeyVal(key,"chair0x1",181+15);
		y1=f.GetKeyVal(key,"chair0y1",35+58);
		x2=f.GetKeyVal(key,"chair0x2",209+15);
		y2=f.GetKeyVal(key,"chair0y2",110+58);
		CDesk8::m_ChairRect[0].SetRect(x1, y1,  x2, y2);//��
		x1=f.GetKeyVal(key,"chair1x1",181+15-81);
		y1=f.GetKeyVal(key,"chair1y1",35+58);
		x2=f.GetKeyVal(key,"chair1x2",209+15-81);
		y2=f.GetKeyVal(key,"chair1y2",110+58);
		CDesk8::m_ChairRect[1].SetRect(x1, y1,  x2, y2);//���·�
		x1=f.GetKeyVal(key,"chair2x1",181+15-81-81);
		y1=f.GetKeyVal(key,"chair2y1",35+58);
		x2=f.GetKeyVal(key,"chair2x2",209+15-81-81);
		y2=f.GetKeyVal(key,"chair2y2",110+58);
		CDesk8::m_ChairRect[2].SetRect(x1, y1,  x2, y2);//��
		x1=f.GetKeyVal(key,"chair3x1",181+15-81-81-35);
		y1=f.GetKeyVal(key,"chair3y1",35+58-46);
		x2=f.GetKeyVal(key,"chair3x2",209+15-81-81-35);
		y2=f.GetKeyVal(key,"chair3y2",110+58-46);
		CDesk8::m_ChairRect[3].SetRect(x1, y1,  x2, y2);//���Ϸ�
		x1=f.GetKeyVal(key,"chair4x1",181+15-81-81-35+62);
		y1=f.GetKeyVal(key,"chair4y1",35+58-46-39);
		x2=f.GetKeyVal(key,"chair4x2",209+15-81-81-35+62);
		y2=f.GetKeyVal(key,"chair4y2",110+58-46-39);
		CDesk8::m_ChairRect[4].SetRect(x1, y1,  x2, y2);//���Ϸ�
		x1=f.GetKeyVal(key,"chair5x1",181+15-81-81-35+62+81);
		y1=f.GetKeyVal(key,"chair5y1",35+58-46-39);
		x2=f.GetKeyVal(key,"chair5x2",209+15-81-81-35+62+81);
		y2=f.GetKeyVal(key,"chair5y2",110+58-46-39);
		CDesk8::m_ChairRect[5].SetRect(x1, y1,  x2, y2);//���Ϸ�
		x1=f.GetKeyVal(key,"chair6x1",181+15-81-81-35+62+81+81);
		y1=f.GetKeyVal(key,"chair6y1",35+58-46-39);
		x2=f.GetKeyVal(key,"chair6x2",209+15-81-81-35+62+81+81);
		y2=f.GetKeyVal(key,"chair6y2",110+58-46-39);
		CDesk8::m_ChairRect[6].SetRect(x1, y1,  x2, y2);//���Ϸ�
		x1=f.GetKeyVal(key,"chair7x1",181+20+30);
		y1=f.GetKeyVal(key,"chair7y1",35);
		x2=f.GetKeyVal(key,"chair7x2",209+20+30);
		y2=f.GetKeyVal(key,"chair7y2",110);
		CDesk8::m_ChairRect[7].SetRect(x1, y1,  x2, y2);//���Ϸ�

		x1=f.GetKeyVal(key,"log0x",181+15);
		y1=f.GetKeyVal(key,"log0y",35+58);
		CDesk8::m_PtLog[0].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"log1x",181+15-81);
		y1=f.GetKeyVal(key,"log1y",35+58);
		CDesk8::m_PtLog[1].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"log2x",181+15-81-81);
		y1=f.GetKeyVal(key,"log2y",35+58);
		CDesk8::m_PtLog[2].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"log3x",181+15-81-81-35);
		y1=f.GetKeyVal(key,"log3y",35+58-46);
		CDesk8::m_PtLog[3].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"log4x",181+15-81-81-35+62);
		y1=f.GetKeyVal(key,"log4y",35+58-46-39);
		CDesk8::m_PtLog[4].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"log5x",181+15-81-81-35+62+81);
		y1=f.GetKeyVal(key,"log5y",35+58-46-39);
		CDesk8::m_PtLog[5].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"log6x",181+15-81-81-35+62+81+81);
		y1=f.GetKeyVal(key,"log6y",35+58-46-39);
		CDesk8::m_PtLog[6].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"log7x",181+20+30);
		y1=f.GetKeyVal(key,"log7y",35);
		CDesk8::m_PtLog[7].SetPoint(x1, y1);

		CDesk8::m_draw[0]=0;
		CDesk8::m_draw[1]=1;
		CDesk8::m_draw[2]=2;
		CDesk8::m_draw[3]=3;
		CDesk8::m_draw[4]=4;
		CDesk8::m_draw[5]=5;
		CDesk8::m_draw[6]=6;
		CDesk8::m_draw[7]=7;

		CDesk8::m_TextFormat[0]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_CENTER;
		CDesk8::m_TextFormat[1]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_CENTER;
		CDesk8::m_TextFormat[2]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_LEFT;
		CDesk8::m_TextFormat[3]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_CENTER;
		CDesk8::m_TextFormat[4]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_CENTER;
		CDesk8::m_TextFormat[5]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_CENTER;
		CDesk8::m_TextFormat[6]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_CENTER;
		CDesk8::m_TextFormat[7]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_CENTER;
		//DebugPrintf("CDesk6::m_ChairRect[4] = [%d][%d][%d][%d]",
		//	CDesk6::m_ChairRect[4].left,
		//	CDesk6::m_ChairRect[4].top,
		//	CDesk6::m_ChairRect[4].right,
		//	CDesk6::m_ChairRect[4].bottom
		//	);

		//��ʼ�� 4 ������0123�ֱ�Ϊ�ң���,���ϣ�
		key="Desk16";
		x1=f.GetKeyVal(key,"lockx",25);
		y1=f.GetKeyVal(key,"locky",20);
		CDesk16::m_PtLock.SetPoint(x1,y1);
		x1=f.GetKeyVal(key,"basePointx",25);
		y1=f.GetKeyVal(key,"basePointy",140);
		CDesk16::m_PtBasePoint.SetPoint(x1,y1);
		CDesk16::m_PtBegin.SetPoint(94, 98);//��������Ϸ������
		x1=f.GetKeyVal(key,"indexx1",25);
		y1=f.GetKeyVal(key,"indexy1",147);
		x2=f.GetKeyVal(key,"indexx2",143);
		y2=f.GetKeyVal(key,"indexy2",165);
		CDesk16::m_IndexRect.SetRect(x1,y1,x2,y2);
		x1=f.GetKeyVal(key,"agree0x",140);
		y1=f.GetKeyVal(key,"agree0y",31);
		CDesk16::m_PtAgree[0].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"agree1x",97);
		y1=f.GetKeyVal(key,"agree1y",148);
		CDesk16::m_PtAgree[1].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"agree2x",31);
		y1=f.GetKeyVal(key,"agree2y",43);
		CDesk16::m_PtAgree[2].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"agree3x",71);
		y1=f.GetKeyVal(key,"agree3y",29);
		CDesk16::m_PtAgree[3].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"agree4x",71);
		y1=f.GetKeyVal(key,"agree4y",29);
		CDesk16::m_PtAgree[4].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"agree5x",71);
		y1=f.GetKeyVal(key,"agree5y",29);
		CDesk16::m_PtAgree[5].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"agree6x",71);
		y1=f.GetKeyVal(key,"agree6y",29);
		CDesk16::m_PtAgree[6].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"agree7x",71);
		y1=f.GetKeyVal(key,"agree7y",29);
		CDesk16::m_PtAgree[7].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"agree8x",71);
		y1=f.GetKeyVal(key,"agree8y",29);
		CDesk16::m_PtAgree[8].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"agree9x",71);
		y1=f.GetKeyVal(key,"agree9y",29);
		CDesk16::m_PtAgree[9].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"agree10x",71);
		y1=f.GetKeyVal(key,"agree10y",29);
		CDesk16::m_PtAgree[10].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"agree11x",71);
		y1=f.GetKeyVal(key,"agree11y",29);
		CDesk16::m_PtAgree[11].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"agree12x",71);
		y1=f.GetKeyVal(key,"agree12y",29);
		CDesk16::m_PtAgree[12].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"agree13x",71);
		y1=f.GetKeyVal(key,"agree13y",29);
		CDesk16::m_PtAgree[13].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"agree14x",71);
		y1=f.GetKeyVal(key,"agree14y",29);
		CDesk16::m_PtAgree[14].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"agree15x",71);
		y1=f.GetKeyVal(key,"agree15y",29);
		CDesk16::m_PtAgree[15].SetPoint(x1, y1);
		//���ֿ��λ��		
		x1=f.GetKeyVal(key,"name0x1",31);
		y1=f.GetKeyVal(key,"name0y1",43);
		x2=f.GetKeyVal(key,"name0x2",x1+80);
		y2=f.GetKeyVal(key,"name0y2",y2+16);
		CDesk16::m_NameRect[0].SetRect(x1,y1,x2,y2);//��
		x1=f.GetKeyVal(key,"name1x1",31);
		y1=f.GetKeyVal(key,"name1y1",43);
		x2=f.GetKeyVal(key,"name1x2",x1+80);
		y2=f.GetKeyVal(key,"name1y2",y2+16);
		CDesk16::m_NameRect[1].SetRect(x1,y1,x2,y2);//��
		x1=f.GetKeyVal(key,"name2x1",31);
		y1=f.GetKeyVal(key,"name2y1",43);
		x2=f.GetKeyVal(key,"name2x2",x1+80);
		y2=f.GetKeyVal(key,"name2y2",y2+16);
		CDesk16::m_NameRect[2].SetRect(x1,y1,x2,y2);//��
		x1=f.GetKeyVal(key,"name3x1",31);
		y1=f.GetKeyVal(key,"name3y1",43);
		x2=f.GetKeyVal(key,"name3x2",x1+80);
		y2=f.GetKeyVal(key,"name3y2",y2+16);
		CDesk16::m_NameRect[3].SetRect(x1,y1,x2,y2);//��
		x1=f.GetKeyVal(key,"name4x1",31);
		y1=f.GetKeyVal(key,"name4y1",43);
		x2=f.GetKeyVal(key,"name4x2",x1+80);
		y2=f.GetKeyVal(key,"name4y2",y2+16);
		CDesk16::m_NameRect[4].SetRect(x1,y1,x2,y2);//��
		x1=f.GetKeyVal(key,"name5x1",31);
		y1=f.GetKeyVal(key,"name5y1",43);
		x2=f.GetKeyVal(key,"name5x2",x1+80);
		y2=f.GetKeyVal(key,"name5y2",y2+16);
		CDesk16::m_NameRect[5].SetRect(x1,y1,x2,y2);//��
		x1=f.GetKeyVal(key,"name6x1",31);
		y1=f.GetKeyVal(key,"name6y1",43);
		x2=f.GetKeyVal(key,"name6x2",x1+80);
		y2=f.GetKeyVal(key,"name6y2",y2+16);
		CDesk16::m_NameRect[6].SetRect(x1,y1,x2,y2);//��
		x1=f.GetKeyVal(key,"name7x1",31);
		y1=f.GetKeyVal(key,"name7y1",43);
		x2=f.GetKeyVal(key,"name7x2",x1+80);
		y2=f.GetKeyVal(key,"name7y2",y2+16);
		CDesk16::m_NameRect[7].SetRect(x1,y1,x2,y2);//��
		x1=f.GetKeyVal(key,"name8x1",31);
		y1=f.GetKeyVal(key,"name8y1",43);
		x2=f.GetKeyVal(key,"name8x2",x1+80);
		y2=f.GetKeyVal(key,"name8y2",y2+16);
		CDesk16::m_NameRect[8].SetRect(x1,y1,x2,y2);//��
		x1=f.GetKeyVal(key,"name9x1",31);
		y1=f.GetKeyVal(key,"name9y1",43);
		x2=f.GetKeyVal(key,"name9x2",x1+80);
		y2=f.GetKeyVal(key,"name9y2",y2+16);
		CDesk16::m_NameRect[9].SetRect(x1,y1,x2,y2);//��
		x1=f.GetKeyVal(key,"name10x1",31);
		y1=f.GetKeyVal(key,"name10y1",43);
		x2=f.GetKeyVal(key,"name10x2",x1+80);
		y2=f.GetKeyVal(key,"name10y2",y2+16);
		CDesk16::m_NameRect[10].SetRect(x1,y1,x2,y2);//��
		x1=f.GetKeyVal(key,"name11x1",31);
		y1=f.GetKeyVal(key,"name11y1",43);
		x2=f.GetKeyVal(key,"name11x2",x1+80);
		y2=f.GetKeyVal(key,"name11y2",y2+16);
		CDesk16::m_NameRect[11].SetRect(x1,y1,x2,y2);//��
		x1=f.GetKeyVal(key,"name12x1",31);
		y1=f.GetKeyVal(key,"name12y1",43);
		x2=f.GetKeyVal(key,"name12x2",x1+80);
		y2=f.GetKeyVal(key,"name12y2",y2+16);
		CDesk16::m_NameRect[12].SetRect(x1,y1,x2,y2);//��
		x1=f.GetKeyVal(key,"name13x1",31);
		y1=f.GetKeyVal(key,"name13y1",43);
		x2=f.GetKeyVal(key,"name13x2",x1+80);
		y2=f.GetKeyVal(key,"name13y2",y2+16);
		CDesk16::m_NameRect[13].SetRect(x1,y1,x2,y2);//��
		x1=f.GetKeyVal(key,"name14x1",31);
		y1=f.GetKeyVal(key,"name14y1",43);
		x2=f.GetKeyVal(key,"name14x2",x1+80);
		y2=f.GetKeyVal(key,"name14y2",y2+16);
		CDesk16::m_NameRect[14].SetRect(x1,y1,x2,y2);//��
		x1=f.GetKeyVal(key,"name15x1",31);
		y1=f.GetKeyVal(key,"name15y1",43);
		x2=f.GetKeyVal(key,"name15x2",x1+80);
		y2=f.GetKeyVal(key,"name15y2",y2+16);
		CDesk16::m_NameRect[15].SetRect(x1,y1,x2,y2);//��

		x1=f.GetKeyVal(key,"chair0x1",181);
		y1=f.GetKeyVal(key,"chair0y1",35);
		x2=f.GetKeyVal(key,"chair0x2",209);
		y2=f.GetKeyVal(key,"chair0y2",110);
		CDesk16::m_ChairRect[0].SetRect(x1, y1,  x2, y2);//��
		x1=f.GetKeyVal(key,"chair1x1",112);
		y1=f.GetKeyVal(key,"chair1y1",92);
		x2=f.GetKeyVal(key,"chair1x2",171);
		y2=f.GetKeyVal(key,"chair1y2",180);
		CDesk16::m_ChairRect[1].SetRect(x1, y1,  x2, y2);//���·�
		x1=f.GetKeyVal(key,"chair2x1",36);
		y1=f.GetKeyVal(key,"chair2y1",60);
		x2=f.GetKeyVal(key,"chair2x2",71);
		y2=f.GetKeyVal(key,"chair2y2",120);
		CDesk16::m_ChairRect[2].SetRect(x1, y1,  x2, y2);//��
		x1=f.GetKeyVal(key,"chair3x1",85);
		y1=f.GetKeyVal(key,"chair3y1",35);
		x2=f.GetKeyVal(key,"chair3x2",105);
		y2=f.GetKeyVal(key,"chair3y2",82);
		CDesk16::m_ChairRect[3].SetRect(x1, y1,  x2, y2);//���Ϸ�
		x1=f.GetKeyVal(key,"chair4x1",85);
		y1=f.GetKeyVal(key,"chair4y1",35);
		x2=f.GetKeyVal(key,"chair4x2",105);
		y2=f.GetKeyVal(key,"chair4y2",82);
		CDesk16::m_ChairRect[4].SetRect(x1, y1,  x2, y2);//���Ϸ�
		x1=f.GetKeyVal(key,"chair5x1",85);
		y1=f.GetKeyVal(key,"chair5y1",35);
		x2=f.GetKeyVal(key,"chair5x2",105);
		y2=f.GetKeyVal(key,"chair5y2",82);
		CDesk16::m_ChairRect[5].SetRect(x1, y1,  x2, y2);//���Ϸ�
		x1=f.GetKeyVal(key,"chair6x1",85);
		y1=f.GetKeyVal(key,"chair6y1",35);
		x2=f.GetKeyVal(key,"chair6x2",105);
		y2=f.GetKeyVal(key,"chair6y2",82);
		CDesk16::m_ChairRect[6].SetRect(x1, y1,  x2, y2);//���Ϸ�
		x1=f.GetKeyVal(key,"chair7x1",85);
		y1=f.GetKeyVal(key,"chair7y1",35);
		x2=f.GetKeyVal(key,"chair7x2",105);
		y2=f.GetKeyVal(key,"chair7y2",82);
		CDesk16::m_ChairRect[7].SetRect(x1, y1,  x2, y2);//���Ϸ�
		x1=f.GetKeyVal(key,"chair8x1",85);
		y1=f.GetKeyVal(key,"chair8y1",35);
		x2=f.GetKeyVal(key,"chair8x2",105);
		y2=f.GetKeyVal(key,"chair8y2",82);
		CDesk16::m_ChairRect[8].SetRect(x1, y1,  x2, y2);//���Ϸ�
		x1=f.GetKeyVal(key,"chair9x1",85);
		y1=f.GetKeyVal(key,"chair9y1",35);
		x2=f.GetKeyVal(key,"chair9x2",105);
		y2=f.GetKeyVal(key,"chair9y2",82);
		CDesk16::m_ChairRect[9].SetRect(x1, y1,  x2, y2);//���Ϸ�
		x1=f.GetKeyVal(key,"chair10x1",85);
		y1=f.GetKeyVal(key,"chair10y1",35);
		x2=f.GetKeyVal(key,"chair10x2",105);
		y2=f.GetKeyVal(key,"chair10y2",82);
		CDesk16::m_ChairRect[10].SetRect(x1, y1,  x2, y2);//���Ϸ�
		x1=f.GetKeyVal(key,"chair11x1",85);
		y1=f.GetKeyVal(key,"chair11y1",35);
		x2=f.GetKeyVal(key,"chair11x2",105);
		y2=f.GetKeyVal(key,"chair11y2",82);
		CDesk16::m_ChairRect[11].SetRect(x1, y1,  x2, y2);//���Ϸ�
		x1=f.GetKeyVal(key,"chair12x1",85);
		y1=f.GetKeyVal(key,"chair12y1",35);
		x2=f.GetKeyVal(key,"chair12x2",105);
		y2=f.GetKeyVal(key,"chair12y2",82);
		CDesk16::m_ChairRect[12].SetRect(x1, y1,  x2, y2);//���Ϸ�
		x1=f.GetKeyVal(key,"chair13x1",85);
		y1=f.GetKeyVal(key,"chair13y1",35);
		x2=f.GetKeyVal(key,"chair13x2",105);
		y2=f.GetKeyVal(key,"chair13y2",82);
		CDesk16::m_ChairRect[13].SetRect(x1, y1,  x2, y2);//���Ϸ�
		x1=f.GetKeyVal(key,"chair14x1",85);
		y1=f.GetKeyVal(key,"chair14y1",35);
		x2=f.GetKeyVal(key,"chair14x2",105);
		y2=f.GetKeyVal(key,"chair14y2",82);
		CDesk16::m_ChairRect[14].SetRect(x1, y1,  x2, y2);//���Ϸ�
		x1=f.GetKeyVal(key,"chair15x1",85);
		y1=f.GetKeyVal(key,"chair15y1",35);
		x2=f.GetKeyVal(key,"chair15x2",105);
		y2=f.GetKeyVal(key,"chair15y2",82);
		CDesk16::m_ChairRect[15].SetRect(x1, y1,  x2, y2);//���Ϸ�
		x1=f.GetKeyVal(key,"log0x",131);
		y1=f.GetKeyVal(key,"log0y",31);
		CDesk16::m_PtLog[0].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"log1x",98);
		y1=f.GetKeyVal(key,"log1y",52);
		CDesk16::m_PtLog[1].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"log2x",36);
		y1=f.GetKeyVal(key,"log2y",43);
		CDesk16::m_PtLog[2].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"log3x",71);
		y1=f.GetKeyVal(key,"log3y",29);
		CDesk16::m_PtLog[3].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"log4x",71);
		y1=f.GetKeyVal(key,"log4y",29);
		CDesk16::m_PtLog[4].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"log5x",71);
		y1=f.GetKeyVal(key,"log5y",29);
		CDesk16::m_PtLog[5].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"log6x",71);
		y1=f.GetKeyVal(key,"log6y",29);
		CDesk16::m_PtLog[6].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"log7x",71);
		y1=f.GetKeyVal(key,"log7y",29);
		CDesk16::m_PtLog[7].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"log8x",71);
		y1=f.GetKeyVal(key,"log8y",29);
		CDesk16::m_PtLog[8].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"log9x",71);
		y1=f.GetKeyVal(key,"log9y",29);
		CDesk16::m_PtLog[9].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"log10x",71);
		y1=f.GetKeyVal(key,"log10y",29);
		CDesk16::m_PtLog[10].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"log11x",71);
		y1=f.GetKeyVal(key,"log11y",29);
		CDesk16::m_PtLog[11].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"log12x",71);
		y1=f.GetKeyVal(key,"log12y",29);
		CDesk16::m_PtLog[12].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"log13x",71);
		y1=f.GetKeyVal(key,"log13y",29);
		CDesk16::m_PtLog[13].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"log14x",71);
		y1=f.GetKeyVal(key,"log14y",29);
		CDesk16::m_PtLog[14].SetPoint(x1, y1);
		x1=f.GetKeyVal(key,"log15x",71);
		y1=f.GetKeyVal(key,"log15y",29);
		CDesk16::m_PtLog[15].SetPoint(x1, y1);
		CDesk16::m_draw[0]=0;
		CDesk16::m_draw[1]=1;
		CDesk16::m_draw[2]=2;
		CDesk16::m_draw[3]=3;
		CDesk16::m_draw[4]=4;
		CDesk16::m_draw[5]=5;
		CDesk16::m_draw[6]=6;
		CDesk16::m_draw[7]=7;
		CDesk16::m_draw[8]=8;
		CDesk16::m_draw[9]=9;
		CDesk16::m_draw[10]=10;
		CDesk16::m_draw[11]=11;
		CDesk16::m_draw[12]=12;
		CDesk16::m_draw[13]=13;
		CDesk16::m_draw[14]=14;
		CDesk16::m_draw[15]=15;

		CDesk16::m_TextFormat[0]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_CENTER;//��
		CDesk16::m_TextFormat[1]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_CENTER;//��
		CDesk16::m_TextFormat[2]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_CENTER;//��
		CDesk16::m_TextFormat[3]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_CENTER;//��
		CDesk16::m_TextFormat[4]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_CENTER;//��
		CDesk16::m_TextFormat[5]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_CENTER;//��
		CDesk16::m_TextFormat[6]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_CENTER;//��
		CDesk16::m_TextFormat[7]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_CENTER;//��
		CDesk16::m_TextFormat[8]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_CENTER;//��
		CDesk16::m_TextFormat[9]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_CENTER;//��
		CDesk16::m_TextFormat[10]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_CENTER;//��
		CDesk16::m_TextFormat[11]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_CENTER;//��
		CDesk16::m_TextFormat[12]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_CENTER;//��
		CDesk16::m_TextFormat[13]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_CENTER;//��
		CDesk16::m_TextFormat[14]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_CENTER;//��
		CDesk16::m_TextFormat[15]=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS|DT_CENTER;//��
	}
	return bInit;
};

//����Ĭ����Ϸ��������չ����û���ҵ��������
bool CBaseDeskView::LoadDeskImage(BYTE bEveryCount, CGameImage & AFCDeskImage, BYTE bDeskType,CString deskClass,bool deskFlat,UINT uNameID)
{
	TCHAR szPath[MAX_PATH];
	wsprintf(szPath,".\\%d\\image\\Desk\\Desk.bmp",uNameID);
	if (!CBcfFile::IsFileExist(szPath))
	{
		wsprintf(szPath, "image\\client\\alluse\\Desk.bmp");
	}
	return AFCDeskImage.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);

}

//���ÿ�ʼ��־
void CBaseDeskView::SetPlayGame(bool bPlaying, CDeskFrameView * pDeskFrame)
{
	m_bPlayGame=bPlaying;
	pDeskFrame->UpdateDeskView(m_bDeskIndex);//������Ϸ����
	return;
}

//��������
void CBaseDeskView::SetHoverIndex(BYTE bHoverIndex, CDeskFrameView * pDeskFrame) 
{ 
	if(!this)
		return;
	if (m_bHoverIndex!=bHoverIndex)
	{
		m_bHoverIndex=bHoverIndex; 
		pDeskFrame->UpdateDeskView(m_bDeskIndex);//������Ϸ����
	}
	return;
}

//���ð���
void CBaseDeskView::SetMouseDown(bool bMouseDown, CDeskFrameView * pDeskFrame) 
{ 
	if(!this)
		return;
	if (m_bDownMouse!=bMouseDown)
	{
		m_bDownMouse=bMouseDown; 
		pDeskFrame->UpdateDeskView(m_bDeskIndex);//������Ϸ����
	}
	return;
}

//���ÿ��
void CBaseDeskView::SetShowFrame(bool bFocusFrame, CDeskFrameView * pDeskFrame)
{ 
	if(!this)
		return;
	if (m_bFocusFrame!=bFocusFrame)
	{
		m_bFocusFrame=bFocusFrame; 
		pDeskFrame->UpdateDeskView(m_bDeskIndex);//������Ϸ����
	} 
	return;
}

//��������־
void CBaseDeskView::SetLock(bool bLock, CDeskFrameView * pDeskFrame)
{
	if (m_bLock!=bLock)
	{
		m_bLock=bLock;
		pDeskFrame->UpdateDeskView(m_bDeskIndex);//������Ϸ����
	}
	return;
}
//�Ƿ�ȫ��ͬ��
bool CBaseDeskView::IsAllAgree(void)
{
	return false;
}
//ͬ������
BYTE CBaseDeskView::CountAgree(void)
{
	return 0;
}
void CBaseDeskView::SetBasePoint(bool bSetBasePoint, int iBasePoint, CDeskFrameView * pDeskFrame)
{
	m_bBasePoint=bSetBasePoint;
	m_iBasePoint=iBasePoint;
	pDeskFrame->UpdateDeskView(m_bDeskIndex);//������Ϸ����
	return;
}
//�����û���Ϣ
bool CBaseDeskView::SetUserItem(BYTE bDeskStation, UserItemStruct * pUserItem, CDeskFrameView * pDeskFrame)
{
	if (bDeskStation<MAX_PEOPLE)
	{
		m_pUserItem[bDeskStation]=pUserItem;
		pDeskFrame->UpdateDeskView(m_bDeskIndex);//������Ϸ����
		return true;
	}
	return false;
}

//���ñ�����Ϣ
bool CBaseDeskView::SetMatchInfo(BYTE bDeskStation, long int dwUserID, TCHAR * szUserName, CDeskFrameView * pDeskFrame)
{
	if (bDeskStation<MAX_PEOPLE)
	{
		m_MatchInfo[bDeskStation].dwUserID=dwUserID;
		if (szUserName!=NULL) lstrcpy(m_MatchInfo[bDeskStation].szUserName,szUserName);
		else m_MatchInfo[bDeskStation].szUserName[0]=0;
		pDeskFrame->UpdateDeskView(m_bDeskIndex);
		return true;
	}
	return false;
}

//��ȡ�û���Ϣ
UserItemStruct * CBaseDeskView::GetUserItem(BYTE bDeskStation)
{
	if (bDeskStation<MAX_PEOPLE) 
		return m_pUserItem[bDeskStation];
	return NULL;
}

//��ȡ������Ϣ
long int CBaseDeskView::GetMatchInfo(BYTE bDeskStation)
{
	if (bDeskStation<MAX_PEOPLE) return m_MatchInfo[bDeskStation].dwUserID;
	return 0L;
}

/*******************************************************************************************************/

//���캯��
CDeskFrameView::CDeskFrameView()
{
	m_iPos=0;				//��ǰλ��		
	m_iMaxPos=0;			//���λ��		
	m_iRowCount=0;			//��������Ŀ
	m_iPicWidth=0;			//ͼ����
	m_iPicHight=0;			//ͼ��߶�
	m_iXBasePos=0;			//X ������
	m_iYBasePos=0;			//Y ������
	m_iAllLineCount=0;		//����������
	m_iScreenLineCount=0;	//����������
	m_uClassSize=0;			//���С
	m_bDeskCount=0;			//��Ϸ����Ŀ
	m_bEveryCount=0;		//ÿ����Ϸ����
	m_pDeskArray=NULL;		//��Ϸ������
	m_bDownDeskIndex=255;	//������������
	m_bDownDeskStation=255;	//����λ������
	m_bUserFrameIndex=255;	//�û��߿�����
	m_bFindFrameIndex=255;	//���ұ߿�����
	m_hMouseCursor=NULL;	//�����
	m_bLeftMouseDown=false;	//�Ƿ�����������
	m_bRightMouseDown=false;//�Ƿ�����Ҽ�����
	pOrderName=NULL;		//�ȼ�����

	m_byDeskType = YH_DESK_FACE;
	m_pImgDesk = NULL;
	m_pImgLock= NULL;
	m_pImgTableBasePoint = NULL;
	m_pImgBegin= NULL;
	m_pImgAgree= NULL;
	m_pBackBmp = NULL;
	m_pGraph = NULL;
	m_bShowPlaying = false;
	m_pImageQueueTip = NULL;
	m_pImageQueueTip1 = NULL;
	m_pImageQueueTip2 = NULL;
	m_pImageQueueTime = NULL;
	m_pImageQueueplan = NULL;
	m_SumTime = 0;
	m_CurTime = 0;

	m_pRoom = NULL;

	m_bmp = new HBITMAP;
}
//��������
CDeskFrameView::~CDeskFrameView()
{
	m_BJLBegin.DestroyWindow();
	m_BJLPlaying.DestroyWindow();
	m_BeginAgree.DestroyWindow();

	m_QueueBegin.DestroyWindow();
	m_QueueStop.DestroyWindow();

	delete m_pImgDesk;
	m_pImgDesk = NULL;
	delete m_pImgLock;
	m_pImgLock= NULL;
	delete m_pImgTableBasePoint;
	m_pImgTableBasePoint = NULL;
	delete m_pImgBegin;
	m_pImgBegin= NULL;
	delete m_pImgAgree;
	m_pImgAgree= NULL;
	UnInitDeskFrame();
	if (m_hMouseCursor!=NULL) DestroyCursor(m_hMouseCursor);//С����״�������
	delete m_pBackBmp;
	m_pBackBmp = NULL;
	delete m_pGraph;
	m_pGraph = NULL;
	delete m_pImageQueueTip;
	m_pImageQueueTip = NULL;
	delete m_pImageQueueTip1;
	m_pImageQueueTip1 = NULL;
	delete m_pImageQueueTip2;
	m_pImageQueueTip2 = NULL;
	delete m_pImageQueueTime;
	m_pImageQueueTime = NULL;
	delete m_pImageQueueplan;
	m_pImageQueueplan = NULL;
	
	delete m_bmp;
}

//��������
bool CDeskFrameView::CreateDeskFrame(CWnd * pParentWnd, UINT uWndID)
{
	if (Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,
		CRect(0,0,0,0),pParentWnd,uWndID)==FALSE) return false;
	m_hMouseCursor=LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_HAND));//��꾭��ʱ���С����״������꣬IDC_HAND�Ǹ���Դ��ID
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);
	m_BeginAgree.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|BS_OWNERDRAW,CRect(0,0,75,25),this,IDC_BEGINAGREE);
	TCHAR szPath[MAX_PATH];
	///ʹ�ù���ͼƬ
	wsprintf(szPath, "image\\client\\alluse\\start_bt.bmp",this->pGameInfo->uNameID);
	m_BeginAgree.LoadButtonBitmap(szPath,false);

	CGameImage img;
	img.SetLoadInfo(szPath,true);

	CGameImageHelper ImageHandle(&img);
	HRGN hRgn=AFCBmpToRgn(ImageHandle,img.GetPixel(0,0),RGB(1,1,1));
	if (hRgn!=NULL)
	{
		m_BeginAgree.SetWindowRgn(hRgn,TRUE);
		///{{ Added by zxd 20100709 �ͷ��������
		DeleteObject(hRgn);
		///Added by zxd 20100709 �ͷ��������}}
	}
	
	m_BeginAgree.ShowWindow(0);
	LoadBtnAgree(m_deskClass);

	//�ټ��ֿ�ʼ��ťadd by wlr 20090713
	m_BJLBegin.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|BS_OWNERDRAW,CRect(0,0,75,25),this,IDC_BJLBEGIN);
	wsprintf(szPath,".\\%d\\image\\Desk\\EnterGame.bmp",this->pGameInfo->uNameID);
	m_BJLBegin.LoadButtonBitmap(szPath,false);
	m_BJLBegin.ShowWindow(SW_HIDE);

	m_BJLPlaying.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|BS_OWNERDRAW,CRect(0,0,75,25),this,IDC_BJLPLAYING);
	wsprintf(szPath,".\\%d\\image\\Desk\\playingGameBt.bmp",this->pGameInfo->uNameID);
	m_BJLPlaying.LoadButtonBitmap(szPath,false);
	//m_BJLPlaying.EnableWindow(FALSE);
	m_BJLPlaying.ShowWindow(SW_HIDE);
	//end of �ټ��ֿ�ʼ��ťadd by wlr 20090713

	m_QueueBegin.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|BS_OWNERDRAW,CRect(0,0,136,51),this,IDC_QUEUEBEGIN);
	wsprintf(szPath,"image\\client\\alluse\\queue\\QueueBegin.bmp");
	m_QueueBegin.LoadButtonBitmap(szPath,false);
	m_QueueBegin.ShowWindow(SW_HIDE);
	m_QueueStop.Create(TEXT(""),WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|BS_OWNERDRAW,CRect(0,0,136,51),this,IDC_QUEUESTOP);
	wsprintf(szPath,"image\\client\\alluse\\queue\\QueueStop.bmp");
	m_QueueStop.LoadButtonBitmap(szPath,false);
	m_QueueStop.ShowWindow(SW_HIDE);

	return true;
}


//desc:��ʾ�ټ��ֿ�ʼ��ť add by wlr 20090713
void CDeskFrameView::ShowBJLBeginBtn()
{
	if (pGameInfo)
	{
		//if (HUNDRED_PEOPLE_DESK_GAME_NAME == pGameInfo->uNameID		//�ټ���
		//	||SUOHA_PEOPLE_DESK_GAME_NAME == pGameInfo->uNameID			//�ٱ����
		//	||NIUNIU_PEOPLE_DESK_GAME_NAME == pGameInfo->uNameID		//�ٱ�ţţ
		//	||ERZHANGPAI_PEOPLE_DESK_GAME_NAME == pGameInfo->uNameID )	//�ٱ������)// add by ramon 09.05 �ټ���)
		char cKey[10];
		CString sPath=CBcfFile::GetAppPath (false);
		CBcfFile fsr( sPath + "SpecialRule.bcf");
		sprintf(cKey, "%d", pGameInfo->uNameID);
		int iResult = fsr.GetKeyVal(_T("BJLType"), cKey, 0);

		if (iResult)
		{
			if(m_BJLBegin.GetSafeHwnd()!=NULL && !m_bShowPlaying)
			{
				m_BJLPlaying.ShowWindow(SW_HIDE);
				m_BJLBegin.ShowWindow(SW_SHOW);
			}
			if (m_BJLPlaying.GetSafeHwnd()!=NULL && m_bShowPlaying)//��Ϸ�����а�ť
			{
				m_BJLBegin.ShowWindow(SW_HIDE);
				m_BJLPlaying.EnableWindow(FALSE);
				m_BJLPlaying.ShowWindow(SW_SHOW);				
			}
		}
	}
}

//�ټ��� add by wlr 20090717
//desc:����ʾ������Ϸ��ť������ʾ��Ϸ�����а�ť
void CDeskFrameView::showBJLBeginOrPlayingBtn(bool bShowPlaying)
{
	m_BJLPlaying.EnableWindow(TRUE);
	m_BJLBegin.EnableWindow(TRUE);
	if (bShowPlaying)
	{
		m_BJLBegin.ShowWindow(SW_HIDE);
		m_BJLPlaying.EnableWindow(FALSE);
		m_BJLPlaying.ShowWindow(SW_SHOW);
		m_bShowPlaying = true;
	}
	else
	{
		m_BJLPlaying.ShowWindow(SW_HIDE);
		m_BJLBegin.ShowWindow(SW_SHOW);
		m_bShowPlaying = false;
	}
	//m_BJLBegin.UpdateWindow();
	//m_BJLPlaying.UpdateWindow();

	return;
}
//end of �ټ��� add by wlr 20090717


//�Ŷӻ���ʾ�ŶӺ��뿪
void CDeskFrameView::showQueueBtn(bool bShowPlaying)
{
	m_QueueBegin.EnableWindow(TRUE);
	m_QueueStop.EnableWindow(TRUE);
	if (bShowPlaying)
	{
		m_QueueBegin.ShowWindow(SW_HIDE);
		m_QueueBegin.EnableWindow(FALSE);
		m_QueueStop.ShowWindow(SW_SHOW);
		m_bQueueBegin = true;
	}
	else
	{
		m_QueueBegin.ShowWindow(SW_HIDE);
		m_QueueBegin.EnableWindow(FALSE);
		m_QueueStop.ShowWindow(SW_HIDE);
		m_QueueStop.EnableWindow(FALSE);
		m_bQueueBegin = false;
	}
	//m_BJLBegin.UpdateWindow();
	//m_BJLPlaying.UpdateWindow();

	return;
}

void CDeskFrameView::OnHitQueueBegin()
{
	//AFCMessageBox("OnHitQueueBegin");
	DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"OnHitQueueBegin");
}

void CDeskFrameView::OnHitQueueStop()
{
	CWnd* wnd=GetParent();
	if(wnd!=NULL)
		::PostMessage(wnd->GetSafeHwnd(),WM_HIT_QUEUE_STOP,0,0);
}


void CDeskFrameView::ShowQueueTime(int x, int y, int inumber)
{
	int w = m_pImageQueueTime->GetWidth() / 10;
	int h = m_pImageQueueTime->GetHeight();

	char stnumber[128];
	ZeroMemory(stnumber, sizeof(stnumber));
	sprintf(stnumber, "%d", inumber);
	string _time(stnumber);

	for (int i = 0; i < _time.size(); i++)
	{
		m_pGraph->DrawImage(m_pImageQueueTime, x + i * (w + 5), y, (_time[i] - '0') * w, 0, w, h, UnitPixel);
	}
}

void CDeskFrameView::ShowQueuePlan(int x, int y, int iPlan)
{
	int w = m_pImageQueueplan->GetWidth() / 11;
	int h = m_pImageQueueplan->GetHeight();

	int _p = iPlan / 10;

	for (int i = 0; i < 10; i++)
		m_pGraph->DrawImage(m_pImageQueueplan, x + i * (w + 6), y, 10 * w, 0, w, h, UnitPixel);

	//m_pGraph->DrawImage(m_pImageQueueplan, x, y, 0, 0, w * 10, h, UnitPixel);
	for (int i = 0; i < _p; i++)
	{
		m_pGraph->DrawImage(m_pImageQueueplan, x + i * (w + 6), y, i * w, 0, w, h, UnitPixel);
	}
}

void CDeskFrameView::SetQueueTime(int iTime, bool setType)
{
	if (setType)
		m_SumTime = iTime;
	m_CurTime = iTime;



	//CRect rect,ClientRect;
	//GetClientRect(&ClientRect);
	//int iWidth = m_pImageQueueTip1->GetWidth();
	//int iHeight = m_pImageQueueTip1->GetHeight();
	//m_YTip = 20;
	//m_YTip1 = m_YTip + iHeight + 20;
	//int x = (ClientRect.Width() - iWidth) / 2;

	//ShowQueueTime(x + iWidth / 2 + 20, m_YTip1 + 10, m_CurTime);
	//ShowQueuePlan(x + iWidth / 2 + 20, m_YTip1 + 48, ((m_SumTime-m_CurTime) * 1.0) / (1.0 * m_SumTime) * 100);

	//CRect DeskRect(x, m_YTip1, iWidth, iHeight);
	//InvalidateRect(&DeskRect,TRUE);
	Invalidate();

	KillTimer(TIME_QUEUE);
	SetTimer(TIME_QUEUE, 1000, NULL);

}

//��������
void CDeskFrameView::LoadBtnAgree(CString deskClass)
{
	CString s=CBcfFile::GetAppPath ();/////����·��
	CString path;
	// Modified by ZXD ͬʱ֧�����ַ�ʽ��Ϸ�ͻ���
	// Ҫ�жϵ�����ico����exe��
	bool bIsIco = (NULL == strstr(pGameInfo->szProcessName, ".exe"));
	if (bIsIco)
	{
		path.Format("config\\%s\\deskconfig.bcf",deskClass);  ///<add by wxx ;
	}
	else
	{
		path.Format(".\\%d\\config\\%s\\deskconfig.bcf",pGameInfo->uNameID,deskClass); ///<add by wxx ;
	}

	if (!CBcfFile::IsFileExist(s+path))
	{
		return;
	}
	CBcfFile f( s + path);
	CString key="BtnAgree";
	m_BtnAgree0X=f.GetKeyVal(key,"BtnAgree0X",75);
	m_BtnAgree0Y=f.GetKeyVal(key,"BtnAgree0Y",75);
	m_BtnAgree1X=f.GetKeyVal(key,"BtnAgree1X",75);
	m_BtnAgree1Y=f.GetKeyVal(key,"BtnAgree1Y",75);
	m_BtnAgree2X=f.GetKeyVal(key,"BtnAgree2X",75);
	m_BtnAgree2Y=f.GetKeyVal(key,"BtnAgree2Y",75);
	m_BtnAgree3X=f.GetKeyVal(key,"BtnAgree3X",75);
	m_BtnAgree3Y=f.GetKeyVal(key,"BtnAgree3Y",75);
	return ;
}
/// ��ʼ�����������ڴ棬�����ͼƬ��PNG��ʽ
bool CDeskFrameView::InitDeskFramePng(BYTE bDeskCount, BYTE bEveryCount,
								   const wchar_t * pDeskImage,
								   const wchar_t * pLockImage,
								   const wchar_t * pBasePoint,
								   const wchar_t * pBeginImage,
								   const wchar_t * pAgreeImage,
								   BYTE bDeskType,int uComType,
								   GameInfoStruct *GameInfo,
								   CGameRoomEx* pRoom,
								   CString deskClass,
								   bool deskFlat,
								   COLORREF iclrSitDeskFrameLeftTop,
								   COLORREF iclrSitDeskFrameRightBottom,
								   bool bIsVip)
{
	m_bIsVip = bIsVip;
	// {{Added by zxd 20091125 �������ļ��ж�ȡ������ɫֵ
	/// �ȴ���Ϸ�Լ���Ŀ¼�¶�ȡ
	CString strPrivatePath;
	CString s = CBcfFile::GetAppPath();

	strPrivatePath.Format("%s%d\\DeskBackGround.bcf", s, GameInfo->uNameID);
	CString path;
	if (CBcfFile::IsFileExist(strPrivatePath))
	{
		path = strPrivatePath;
	}
	else
	{
		path.Format("%s\\config\\%s\\deskconfig.bcf",s,deskClass); ///<add by wxx ;
	}

	m_pRoom = pRoom;
	m_personalInfo.m_roomWnd = pRoom;

#ifndef BZ_ZLIB
	if (CBcfFile::IsFileExist(path))
	{
		CBcfFile fbcf(path);
		m_r1 = fbcf.GetKeyVal("BackGround", "Color1_R", 255);
		m_g1 = fbcf.GetKeyVal("BackGround", "Color1_G", 255);
		m_b1 = fbcf.GetKeyVal("BackGround", "Color1_B", 204);
		m_r2 = fbcf.GetKeyVal("BackGround", "Color2_R", 204);
		m_g2 = fbcf.GetKeyVal("BackGround", "Color2_G", 153);
		m_b2 = fbcf.GetKeyVal("BackGround", "Color2_B", 51);
		// Added by zxd 20091125 �������ļ��ж�ȡ������ɫֵ}}
	}
#else
	CBcfFile fbcf(path);
	m_r1 = fbcf.GetKeyVal("BackGround", "Color1_R", 255);
	m_g1 = fbcf.GetKeyVal("BackGround", "Color1_G", 255);
	m_b1 = fbcf.GetKeyVal("BackGround", "Color1_B", 204);
	m_r2 = fbcf.GetKeyVal("BackGround", "Color2_R", 204);
	m_g2 = fbcf.GetKeyVal("BackGround", "Color2_G", 153);
	m_b2 = fbcf.GetKeyVal("BackGround", "Color2_B", 51);
#endif
	


	// Added by zxd ����ԭ��.icoģʽ�����ж��Ƿ�ΪICOģʽ
	bool bIsIco = (NULL == strstr(GameInfo->szProcessName, ".exe"));

	m_UserID=0;
	m_uComType=uComType;
	pGameInfo=GameInfo;
	m_bIsClick=false;
	m_deskClass=deskClass;
	m_deskFlat=deskFlat;
	m_Tooltip.Create(this);
	m_Tooltip.SetDelayTime(50);
	clrSitDeskFrameLeftTop=iclrSitDeskFrameLeftTop;
	clrSitDeskFrameRightBottom=iclrSitDeskFrameRightBottom;

	//��������
	UnInitDeskFrame();
	//��������
	CBaseDeskView * pDeskArray=NULL;

	//Ϊ���������ڴ�
	switch (bEveryCount)
	{
	case 1:///////һ�����ӵ�����
		{
			if (bDeskType==YH_DESK_FACE) { pDeskArray=new CDesk1 [bDeskCount]; m_uClassSize=sizeof(CDesk1); }
			//if (bDeskType==EX_DESK_FACE) { pDeskArray=new CDeskEx1 [bDeskCount]; m_uClassSize=sizeof(CDeskEx2); }
			break;
		}
	case 2:///////�������ӵ�����
		{
			if (bDeskType==YH_DESK_FACE) { pDeskArray=new CDesk2 [bDeskCount]; m_uClassSize=sizeof(CDesk2); }
			if (bDeskType==EX_DESK_FACE) { pDeskArray=new CDeskEx2 [bDeskCount]; m_uClassSize=sizeof(CDeskEx2); }

			break;
		}
	case 3:///////�������ӵ�����
		{
			if (bDeskType==YH_DESK_FACE) { pDeskArray=new CDesk3 [bDeskCount]; m_uClassSize=sizeof(CDesk3); }
			if (bDeskType==EX_DESK_FACE) { pDeskArray=new CDeskEx3 [bDeskCount]; m_uClassSize=sizeof(CDeskEx3); }
			break;
		}
	case 4:///////�İ����ӵ�����
		{
			if (bDeskType==YH_DESK_FACE) { pDeskArray=new CDesk4 [bDeskCount]; m_uClassSize=sizeof(CDesk4); }
			if (bDeskType==EX_DESK_FACE) { pDeskArray=new CDeskEx4 [bDeskCount]; m_uClassSize=sizeof(CDeskEx4); }
			break;
		}
	case 5:
		{
			if (bDeskType==YH_DESK_FACE) { pDeskArray=new CDesk5 [bDeskCount]; m_uClassSize=sizeof(CDesk5); }
			if (bDeskType==EX_DESK_FACE) { pDeskArray=new CDeskEx5 [bDeskCount]; m_uClassSize=sizeof(CDeskEx5); }
			break;
		}
	case 8:
		{
			if (bDeskType==YH_DESK_FACE) { pDeskArray=new CDesk8 [bDeskCount]; m_uClassSize=sizeof(CDesk8); }
			if (bDeskType==EX_DESK_FACE) { pDeskArray=new CDeskEx8 [bDeskCount]; m_uClassSize=sizeof(CDeskEx8); }
			break;
		}
	case 16:
		{
			if (bDeskType==YH_DESK_FACE) { pDeskArray=new CDesk16 [bDeskCount]; m_uClassSize=sizeof(CDesk16); }
			if (bDeskType==EX_DESK_FACE) { pDeskArray=new CDeskEx16 [bDeskCount]; m_uClassSize=sizeof(CDeskEx16); }
			break;
		}
	default:
		{
			if (bDeskType==YH_DESK_FACE) { pDeskArray=new CDesk6 [bDeskCount]; m_uClassSize=sizeof(CDesk6); }
			if (bDeskType==EX_DESK_FACE) { pDeskArray=new CDeskEx6 [bDeskCount]; m_uClassSize=sizeof(CDeskEx6); }
			break;
		}
	}
	//�ټ��� add by wlr 20090713
	if (pDeskArray==NULL)
	{
		if (bDeskType==BJL_DESK_FACE) 
		{ 
			pDeskArray	=new CDeskBJL [bDeskCount]; 
			m_uClassSize=sizeof(CDeskBJL);
		}

	}
	//end of �ټ��� add by wlr 20090713

	if (pDeskArray==NULL)
	{
		UnInitDeskFrame();
		//AFCMessageBox("��Ϸ��ʼ��ʧ�ܣ������www.bzw.cn��ϵ�����ߣ�");
	    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"��Ϸ��ʼ��ʧ�ܣ������www.bzw.cn��ϵ�����ߣ�");
		return false;
	}
	//��������
	m_pDeskArray=pDeskArray;//��Ϸ������
	m_bDeskCount=bDeskCount;
	m_bEveryCount=bEveryCount;

	//��ʼ������ͼƬ
	if (pDeskImage!=NULL)
	{
#ifdef BZ_ZLIB   //add by wxx 
		CUnZipRes unZipRes;
		IStream * pStream=NULL;
		
		int len = WideCharToMultiByte(CP_ACP, 0, pDeskImage, wcslen(pDeskImage), NULL, 0,NULL,NULL);
		TCHAR * tchar=new char[len+1];
		//���ֽڱ���ת���ɶ��ֽڱ���   
		WideCharToMultiByte(CP_ACP, 0, pDeskImage, wcslen(pDeskImage), tchar, len, NULL, NULL); 
		tchar[len]='\0';
		//CString strFileName(tchar);
		//int pos=strFileName.Find("image");
		//strFileName=strFileName.Right(strFileName.GetLength()-pos);
		//TCHAR * pchar=strFileName.GetBuffer(strFileName.GetLength() + 1);

		pStream=unZipRes.LoadFileResFromZip(tchar);
		m_pImgDesk=new Image(pStream);
		if(pStream!=NULL)
		{
			pStream->Release();
			pStream=NULL;
		}
		
		if(tchar!=NULL)
		{
			delete []tchar;
			tchar=NULL;
		}

#else
		m_pImgDesk = new Image(pDeskImage, FALSE);
#endif

		//m_DeskImage.Attach(pDeskImage,CGameImageLink::m_bAutoLock);
	}
	else
	{
		UnInitDeskFrame();
		//AFCMessageBox("��������ͼƬʧ��");
	    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"��������ͼƬʧ��");
		return false;
	}
	if (m_pImgDesk == NULL) 
	{
		UnInitDeskFrame();
		return false;
	}

	TCHAR szPath[MAX_PATH]={0};
	//��ʼ����ͼ
	if (pLockImage!=NULL)
	{
#ifdef BZ_ZLIB   //add by wxx 
		CUnZipRes unZipRes;
		IStream * pStream=NULL;
		
		int len = WideCharToMultiByte(CP_ACP, 0, pLockImage, wcslen(pLockImage), NULL, 0,NULL,NULL);
		TCHAR * tchar=new char[len+1];
		//���ֽڱ���ת���ɶ��ֽڱ���   
		WideCharToMultiByte(CP_ACP, 0, pLockImage, wcslen(pLockImage), tchar, len, NULL, NULL); 
		tchar[len]='\0';
		//CString strFileName(tchar);
		//int pos=strFileName.Find("image");
		//strFileName=strFileName.Right(strFileName.GetLength()-pos);

		//TCHAR * pchar=strFileName.GetBuffer(strFileName.GetLength() + 1);

		pStream=unZipRes.LoadFileResFromZip(tchar);
		m_pImgLock=new Image(pStream);
		if(pStream!=NULL)
		{
			pStream->Release();
			pStream=NULL;
		}
		
		if(tchar!=NULL)
		{
			delete []tchar;
			tchar=NULL;
		}

#else
		m_pImgLock = new Image(pLockImage, FALSE);
#endif
		//m_pImgLock = new Image(pLockImage, FALSE);
		//m_LockImage.Attach(pLockImage,CGameImageLink::m_bAutoLock);
	}
	else 
	{
		if (bDeskType!=BJL_DESK_FACE)
		{
			UnInitDeskFrame();
			//AFCMessageBox("��������ͼƬʧ��");
	        DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"��������ͼƬʧ��");
			return false;
		}
	}

	//��ʼ�����ӱ���ͼ
	if (pBasePoint!=NULL)
	{
#ifdef BZ_ZLIB   //add by wxx 
		CUnZipRes unZipRes;
		IStream * pStream=NULL;
		
		int len = WideCharToMultiByte(CP_ACP, 0, pBasePoint, wcslen(pBasePoint), NULL, 0,NULL,NULL); 
		TCHAR * tchar=new char[len+1];
		//���ֽڱ���ת���ɶ��ֽڱ���   
		WideCharToMultiByte(CP_ACP, 0, pBasePoint, wcslen(pBasePoint), tchar, len, NULL, NULL); 
		tchar[len]='\0';
		//CString strFileName(tchar);
		//int pos=strFileName.Find("image");
		//strFileName=strFileName.Right(strFileName.GetLength()-pos);

		//TCHAR * pchar=strFileName.GetBuffer(strFileName.GetLength() + 1);

		pStream=unZipRes.LoadFileResFromZip(tchar);
		m_pImgTableBasePoint=new Image(pStream);
		if(pStream!=NULL)
		{
			pStream->Release();
			pStream=NULL;
		}
		
		if(tchar!=NULL)
		{
			delete []tchar;
			tchar=NULL;
		}

#else
		m_pImgTableBasePoint = new Image(pBasePoint, FALSE);
#endif
		//m_pImgTableBasePoint = new Image(pBasePoint, FALSE);
		//m_TableBasePoint.Attach(pBasePoint,CGameImageLink::m_bAutoLock);
	}
	else 
	{
		m_pImgTableBasePoint = NULL;
		//UnInitDeskFrame();
		//AFCMessageBox("�������ӱ���ͼʧ��");
		//return false;
	}

	//��ʼ����ʼ��Ϸ��ͼ
	if (pBeginImage!=NULL) 
	{
#ifdef BZ_ZLIB   //add by wxx 
		CUnZipRes unZipRes;
		IStream * pStream=NULL;
		
		int len = WideCharToMultiByte(CP_ACP, 0, pBeginImage, wcslen(pBeginImage), NULL, 0,NULL,NULL);
		TCHAR * tchar=new char[len+1];
		//���ֽڱ���ת���ɶ��ֽڱ���   
		WideCharToMultiByte(CP_ACP, 0, pBeginImage, wcslen(pBeginImage), tchar, len, NULL, NULL); 
		tchar[len]='\0';
		//CString strFileName(tchar);
		//int pos=strFileName.Find("image");
		//strFileName=strFileName.Right(strFileName.GetLength()-pos);

		//TCHAR * pchar=strFileName.GetBuffer(strFileName.GetLength() + 1);

		pStream=unZipRes.LoadFileResFromZip(tchar);
		m_pImgBegin=new Image(pStream);
		if(pStream!=NULL)
		{
			pStream->Release();
			pStream=NULL;
		}
		
		if(tchar!=NULL)
		{
			delete []tchar;
			tchar=NULL;
		}

#else
		m_pImgBegin = new Image(pBeginImage, FALSE);
#endif
		//m_pImgBegin = new Image(pBeginImage, FALSE);
		//m_BeginImage.Attach(pBeginImage,CGameImageLink::m_bAutoLock);//pBeginImage����������Ϸ������m_PtBegin͸��ͼ
	}
	else 
	{
		if (bDeskType!=BJL_DESK_FACE)
		{
			UnInitDeskFrame();
			//AFCMessageBox("���뿪ʼ��Ϸ��ͼʧ��");
	        DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"���뿪ʼ��Ϸ��ͼʧ��");
			return false;
		}
	}

	
	//��ʼ��ͬ��ͼ
	if (pAgreeImage!=NULL)
	{
#ifdef BZ_ZLIB   //add by wxx 
		CUnZipRes unZipRes;
		IStream * pStream=NULL;
		
		int len = WideCharToMultiByte(CP_ACP, 0, pAgreeImage, wcslen(pAgreeImage), NULL, 0,NULL,NULL); 
		TCHAR * tchar=new char[len+1];
		//���ֽڱ���ת���ɶ��ֽڱ���   
		WideCharToMultiByte(CP_ACP, 0, pAgreeImage, wcslen(pAgreeImage), tchar, len, NULL, NULL); 
		tchar[len]='\0';
		//CString strFileName(tchar);
		//int pos=strFileName.Find("image");
		//strFileName=strFileName.Right(strFileName.GetLength()-pos);

		//TCHAR * pchar=strFileName.GetBuffer(strFileName.GetLength() + 1);

		pStream=unZipRes.LoadFileResFromZip(tchar);
		m_pImgAgree=new Image(pStream);
		if(pStream!=NULL)
		{
			pStream->Release();
			pStream=NULL;
		}
		if(tchar!=NULL)
		{
			delete []tchar;
			tchar=NULL;
		}

#else
		m_pImgAgree = new Image(pAgreeImage, FALSE);
#endif
		//m_pImgAgree = new Image(pAgreeImage, FALSE);
		//m_AgreeImage.Attach(pAgreeImage,CGameImageLink::m_bAutoLock);
	}
	else 
	{
		if (bDeskType!=BJL_DESK_FACE)
		{
			UnInitDeskFrame();
			//AFCMessageBox("����ͬ��ͼʧ��");
	        DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"����ͬ��ͼʧ��");
			return false;
		}
	}

	

	wchar_t wcsQueueTip[MAX_PATH];
	wsprintfW(wcsQueueTip, L".\\image\\client\\alluse\\queue\\Queue_BK_Tip.png");
	if (GameInfo->dwRoomRule & GRR_CONTEST)
	{
		wsprintfW(wcsQueueTip, L".\\image\\client\\alluse\\queue\\Contest_BK_Tip.png");
	}
	
	//��ʼ��ͬ��ͼ
	//if (m_pImageQueueTip!=NULL)
	{
#ifdef BZ_ZLIB   //add by wxx 
		CUnZipRes unZipRes;
		IStream * pStream=NULL;
		
		int len = WideCharToMultiByte(CP_ACP, 0, wcsQueueTip, wcslen(wcsQueueTip), NULL, 0,NULL,NULL); 
		TCHAR * tchar=new char[len+1];
		//���ֽڱ���ת���ɶ��ֽڱ���   
		WideCharToMultiByte(CP_ACP, 0, wcsQueueTip, wcslen(wcsQueueTip), tchar, len, NULL, NULL); 
		tchar[len]='\0';
		//CString strFileName(tchar);
		//int pos=strFileName.Find("image");
		//strFileName=strFileName.Right(strFileName.GetLength()-pos);

		//TCHAR * pchar=strFileName.GetBuffer(strFileName.GetLength() + 1);

		pStream=unZipRes.LoadFileResFromZip(tchar);
		m_pImageQueueTip=new Image(pStream);
		if(pStream!=NULL)
		{
			pStream->Release();
			pStream=NULL;
		}
		if(tchar!=NULL)
		{
			delete []tchar;
			tchar=NULL;
		}

#else
		m_pImageQueueTip = new Image(wcsQueueTip, FALSE);
#endif
		//m_pImgAgree = new Image(pAgreeImage, FALSE);
		//m_AgreeImage.Attach(pAgreeImage,CGameImageLink::m_bAutoLock);
	}



	wchar_t wcsQueueTip1[MAX_PATH];
	wsprintfW(wcsQueueTip1, L".\\image\\client\\alluse\\queue\\Queue_BK_Tip1.png");

	wchar_t wcsQueueTip2[MAX_PATH];
	wsprintfW(wcsQueueTip2, L".\\image\\client\\alluse\\queue\\seat_tip.png");
	//��ʼ��ͬ��ͼ
	//if (m_pImageQueueTip!=NULL)
	{
#ifdef BZ_ZLIB   //add by wxx 
		CUnZipRes unZipRes;
		IStream * pStream=NULL;
		
		int len = WideCharToMultiByte(CP_ACP, 0, wcsQueueTip1, wcslen(wcsQueueTip1), NULL, 0,NULL,NULL); 
		TCHAR * tchar=new char[len+1];
		//���ֽڱ���ת���ɶ��ֽڱ���   
		WideCharToMultiByte(CP_ACP, 0, wcsQueueTip1, wcslen(wcsQueueTip1), tchar, len, NULL, NULL); 
		tchar[len]='\0';
		//CString strFileName(tchar);
		//int pos=strFileName.Find("image");
		//strFileName=strFileName.Right(strFileName.GetLength()-pos);

		//TCHAR * pchar=strFileName.GetBuffer(strFileName.GetLength() + 1);

		pStream=unZipRes.LoadFileResFromZip(tchar);
		m_pImageQueueTip1=new Image(pStream);
		if(pStream!=NULL)
		{
			pStream->Release();
			pStream=NULL;
		}
		if(tchar!=NULL)
		{
			delete []tchar;
			tchar=NULL;
		}

		len = WideCharToMultiByte(CP_ACP, 0, wcsQueueTip2, wcslen(wcsQueueTip2), NULL, 0,NULL,NULL); 
		TCHAR * tchar=new char[len+1];
		//���ֽڱ���ת���ɶ��ֽڱ���   
		WideCharToMultiByte(CP_ACP, 0, wcsQueueTip2, wcslen(wcsQueueTip2), tchar, len, NULL, NULL); 
		tchar[len]='\0';

		pStream=unZipRes.LoadFileResFromZip(tchar);
		m_pImageQueueTip2=new Image(pStream);
		if(pStream!=NULL)
		{
			pStream->Release();
			pStream=NULL;
		}
		if(tchar!=NULL)
		{
			delete []tchar;
			tchar=NULL;
		}

#else
		m_pImageQueueTip1 = new Image(wcsQueueTip1, FALSE);
		m_pImageQueueTip2 = new Image(wcsQueueTip2, FALSE);
#endif
		//m_pImgAgree = new Image(pAgreeImage, FALSE);
		//m_AgreeImage.Attach(pAgreeImage,CGameImageLink::m_bAutoLock);
	}



	wchar_t wcsQueueTime[MAX_PATH];
	wsprintfW(wcsQueueTime, L".\\image\\client\\alluse\\queue\\queueTime.png");
	//��ʼ��ͬ��ͼ
	//if (m_pImageQueueTip!=NULL)
	{
#ifdef BZ_ZLIB   //add by wxx 
		CUnZipRes unZipRes;
		IStream * pStream=NULL;
		
		int len = WideCharToMultiByte(CP_ACP, 0, wcsQueueTime, wcslen(wcsQueueTime), NULL, 0,NULL,NULL); 
		TCHAR * tchar=new char[len+1];
		//���ֽڱ���ת���ɶ��ֽڱ���   
		WideCharToMultiByte(CP_ACP, 0, wcsQueueTime, wcslen(wcsQueueTime), tchar, len, NULL, NULL); 
		tchar[len]='\0';
		//CString strFileName(tchar);
		//int pos=strFileName.Find("image");
		//strFileName=strFileName.Right(strFileName.GetLength()-pos);

		//TCHAR * pchar=strFileName.GetBuffer(strFileName.GetLength() + 1);

		pStream=unZipRes.LoadFileResFromZip(tchar);
		m_pImageQueueTime=new Image(pStream);
		if(pStream!=NULL)
		{
			pStream->Release();
			pStream=NULL;
		}
		if(tchar!=NULL)
		{
			delete []tchar;
			tchar=NULL;
		}

#else
		m_pImageQueueTime = new Image(wcsQueueTime, FALSE);
#endif
		//m_pImgAgree = new Image(pAgreeImage, FALSE);
		//m_AgreeImage.Attach(pAgreeImage,CGameImageLink::m_bAutoLock);
	}


	wchar_t wcsQueuePlan[MAX_PATH];
	wsprintfW(wcsQueuePlan, L".\\image\\client\\alluse\\queue\\queuePlan.png");
	//��ʼ��ͬ��ͼ
	//if (m_pImageQueueTip!=NULL)
	{
#ifdef BZ_ZLIB   //add by wxx 
		CUnZipRes unZipRes;
		IStream * pStream=NULL;
		
		int len = WideCharToMultiByte(CP_ACP, 0, wcsQueuePlan, wcslen(wcsQueuePlan), NULL, 0,NULL,NULL); 
		TCHAR * tchar=new char[len+1];
		//���ֽڱ���ת���ɶ��ֽڱ���   
		WideCharToMultiByte(CP_ACP, 0, wcsQueuePlan, wcslen(wcsQueuePlan), tchar, len, NULL, NULL); 
		tchar[len]='\0';
		//CString strFileName(tchar);
		//int pos=strFileName.Find("image");
		//strFileName=strFileName.Right(strFileName.GetLength()-pos);

		//TCHAR * pchar=strFileName.GetBuffer(strFileName.GetLength() + 1);

		pStream=unZipRes.LoadFileResFromZip(tchar);
		m_pImageQueueplan=new Image(pStream);
		if(pStream!=NULL)
		{
			pStream->Release();
			pStream=NULL;
		}
		if(tchar!=NULL)
		{
			delete []tchar;
			tchar=NULL;
		}

#else
		m_pImageQueueplan = new Image(wcsQueuePlan, FALSE);
#endif
		//m_pImgAgree = new Image(pAgreeImage, FALSE);
		//m_AgreeImage.Attach(pAgreeImage,CGameImageLink::m_bAutoLock);
	}


	m_pImgDesk->GetHeight();
	m_iPicWidth= m_pImgDesk->GetWidth();
	m_iPicHight=m_pImgDesk->GetHeight();
	for (UINT i=0;i<bDeskCount;i++)
	{
		GetDeskArrayPtr(i)->SetDeskClass(deskClass,deskFlat);
		GetDeskArrayPtr(i)->SetDeskIndex(i);
		GetDeskArrayPtr(i)->SetComType(uComType);
	}

	if (GameInfo->dwRoomRule & GRR_QUEUE_GAME)
		SetTimer(TIME_QUEUE, 1000, NULL);



	return true;
};

//��ʼ������
/*
	��ʼ�������ڴ棻��ʼ�������ͼƬ
*/
bool CDeskFrameView::InitDeskFrame(BYTE bDeskCount, BYTE bEveryCount, CGameImage * pDeskImage, CGameImage * pLockImage, CGameImage* pBasePoint, CGameImage * pBeginImage, CGameImage * pAgreeImage,
								   BYTE bDeskType,int uComType,GameInfoStruct *GameInfo,CString deskClass,bool deskFlat,COLORREF iclrSitDeskFrameLeftTop,COLORREF iclrSitDeskFrameRightBottom)
{
	// Added by zxd ����ԭ��.icoģʽ�����ж��Ƿ�ΪICOģʽ
	bool bIsIco = (NULL == strstr(GameInfo->szProcessName, ".exe"));

	m_UserID=0;
	m_uComType=uComType;
	m_byDeskType = bDeskType;
	pGameInfo=GameInfo;
	m_bIsClick=false;
	m_deskClass=deskClass;
	m_deskFlat=deskFlat;
	m_Tooltip.Create(this);
	m_Tooltip.SetDelayTime(50);
	clrSitDeskFrameLeftTop=iclrSitDeskFrameLeftTop;
	clrSitDeskFrameRightBottom=iclrSitDeskFrameRightBottom;

	//��������
	UnInitDeskFrame();
	//��������
	CBaseDeskView * pDeskArray=NULL;

	//Ϊ���������ڴ�
	switch (bEveryCount)
	{
	case 1:///////һ�����ӵ�����
		{
			if (bDeskType==YH_DESK_FACE) { pDeskArray=new CDesk1 [bDeskCount]; m_uClassSize=sizeof(CDesk1); }
			//if (bDeskType==EX_DESK_FACE) { pDeskArray=new CDeskEx1 [bDeskCount]; m_uClassSize=sizeof(CDeskEx2); }
			break;
		}
	case 2:///////�������ӵ�����
		{
			if (bDeskType==YH_DESK_FACE) { pDeskArray=new CDesk2 [bDeskCount]; m_uClassSize=sizeof(CDesk2); }
			if (bDeskType==EX_DESK_FACE) { pDeskArray=new CDeskEx2 [bDeskCount]; m_uClassSize=sizeof(CDeskEx2); }
			break;
		}
	case 3:///////�������ӵ�����
		{
			if (bDeskType==YH_DESK_FACE) { pDeskArray=new CDesk3 [bDeskCount]; m_uClassSize=sizeof(CDesk3); }
			if (bDeskType==EX_DESK_FACE) { pDeskArray=new CDeskEx3 [bDeskCount]; m_uClassSize=sizeof(CDeskEx3); }
			break;
		}
	case 4:///////�İ����ӵ�����
		{
			if (bDeskType==YH_DESK_FACE) { pDeskArray=new CDesk4 [bDeskCount]; m_uClassSize=sizeof(CDesk4); }
			if (bDeskType==EX_DESK_FACE) { pDeskArray=new CDeskEx4 [bDeskCount]; m_uClassSize=sizeof(CDeskEx4); }
			break;
		}
	case 5:
		{
			if (bDeskType==YH_DESK_FACE) { pDeskArray=new CDesk5 [bDeskCount]; m_uClassSize=sizeof(CDesk5); }
			if (bDeskType==EX_DESK_FACE) { pDeskArray=new CDeskEx5 [bDeskCount]; m_uClassSize=sizeof(CDeskEx5); }
			break;
		}
	case 16:///////�İ����ӵ�����
		{
			if (bDeskType==YH_DESK_FACE) { pDeskArray=new CDesk16 [bDeskCount]; m_uClassSize=sizeof(CDesk16); }
			if (bDeskType==EX_DESK_FACE) { pDeskArray=new CDeskEx16 [bDeskCount]; m_uClassSize=sizeof(CDeskEx16); }
			break;
		}
	default:
		{
			if (bDeskType==YH_DESK_FACE) { pDeskArray=new CDesk6 [bDeskCount]; m_uClassSize=sizeof(CDesk6); }
			if (bDeskType==EX_DESK_FACE) { pDeskArray=new CDeskEx6 [bDeskCount]; m_uClassSize=sizeof(CDeskEx6); }
			break;
		}
	//case 7:
	//	{
	//		if (bDeskType==YH_DESK_FACE) { pDeskArray=new CDesk7 [bDeskCount]; m_uClassSize=sizeof(CDesk7); }
	//		if (bDeskType==EX_DESK_FACE) { pDeskArray=new CDeskEx7 [bDeskCount]; m_uClassSize=sizeof(CDeskEx7); }
	//		break;
	//	}
	//case 8:
	//	{
	//		if (bDeskType==YH_DESK_FACE) { pDeskArray=new CDesk8 [bDeskCount]; m_uClassSize=sizeof(CDesk8); }
	//		if (bDeskType==EX_DESK_FACE) { pDeskArray=new CDeskEx8 [bDeskCount]; m_uClassSize=sizeof(CDeskEx8); }
	//		break;
	//	}

	}
	//�ټ��� add by wlr 20090713
	if (pDeskArray==NULL)
	{
		if (bDeskType==BJL_DESK_FACE) 
		{ 
			pDeskArray	=new CDeskBJL [bDeskCount]; 
			m_uClassSize=sizeof(CDeskBJL);
		}
	}
	//end of �ټ��� add by wlr 20090713

	if (pDeskArray==NULL)
	{
		UnInitDeskFrame();
		//AFCMessageBox("��Ϸ��ʼ��ʧ�ܣ������www.bzw.cn��ϵ�����ߣ�");
	    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"��Ϸ��ʼ��ʧ�ܣ������www.bzw.cn��ϵ�����ߣ�");
		return false;
	}
	//��������
	m_pDeskArray=pDeskArray;//��Ϸ������
	m_bDeskCount=bDeskCount;
	m_bEveryCount=bEveryCount;
	
	//��ʼ������ͼƬ
	if (pDeskImage!=NULL)
	{
		m_DeskImage.Attach(pDeskImage,CGameImageLink::m_bAutoLock);
	}
	else
	{
		// {{Modified by zxd ����ԭ��.icoģʽ����������ͼƬ
		if (bIsIco)
		{
			switch (bEveryCount)
			{
			case 1:	{m_DeskImage.SetLoadInfo(".\\image\\client\\alluse\\desk1.bmp",CGameImageLink::m_bAutoLock);break;}
			case 2:	{m_DeskImage.SetLoadInfo(".\\image\\client\\alluse\\desk2.bmp",CGameImageLink::m_bAutoLock);break;}
			case 3:	{m_DeskImage.SetLoadInfo(".\\image\\client\\alluse\\desk3.bmp",CGameImageLink::m_bAutoLock);break;}
			case 4:	{m_DeskImage.SetLoadInfo(".\\image\\client\\alluse\\desk4.bmp",CGameImageLink::m_bAutoLock);break;}
			case 5:	{m_DeskImage.SetLoadInfo(".\\image\\client\\alluse\\desk5.bmp",CGameImageLink::m_bAutoLock);break;}
			case 6:	{m_DeskImage.SetLoadInfo(".\\image\\client\\alluse\\desk6.bmp",CGameImageLink::m_bAutoLock);break;}
			}
		}
		else
		{
			CBaseDeskView::LoadDeskImage(bEveryCount,m_DeskImage,bDeskType,deskClass,deskFlat,GameInfo->uNameID);
		}
	}
	if (m_DeskImage.IsNull()) 
	{
		UnInitDeskFrame();
		return false;
	}

	TCHAR szPath[MAX_PATH]={0};
	//��ʼ����ͼ
	if (pLockImage!=NULL)
		m_LockImage.Attach(pLockImage,CGameImageLink::m_bAutoLock);
	else 
	{
		if (bIsIco)
		{
			wsprintf(szPath,".\\image\\client\\alluse\\lock.bmp");
		}
		else
		{
			wsprintf(szPath,".\\%d\\image\\desk\\lock.bmp",GameInfo->uNameID);
		}
		m_LockImage.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);
	}
	if (m_LockImage.IsNull())
	{
		UnInitDeskFrame();
		return false;
	}

	//��ʼ�����ӱ���ͼ
	if (pBasePoint!=NULL)
		m_TableBasePoint.Attach(pBasePoint,CGameImageLink::m_bAutoLock);
	else 
	{
		if (bIsIco)
		{
			wsprintf(szPath,".\\image\\client\\alluse\\basepoint.bmp");
		}
		else
		{
			wsprintf(szPath,".\\%d\\image\\desk\\basepoint.bmp",GameInfo->uNameID);
		}
		m_TableBasePoint.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);
	}
	if (m_TableBasePoint.IsNull())
	{
		UnInitDeskFrame();
		return false;
	}
	
	//��ʼ����ʼ��Ϸ��ͼ
	if (pBeginImage!=NULL) 
		m_BeginImage.Attach(pBeginImage,CGameImageLink::m_bAutoLock);//pBeginImage����������Ϸ������m_PtBegin͸��ͼ
	else 
	{
		if (bIsIco)
		{
			switch (bEveryCount)
			{
			case 1:	{wsprintf(szPath,".\\image\\client\\alluse\\Desk1playing.bmp");break;}
			case 2:	{wsprintf(szPath,".\\image\\client\\alluse\\Desk2playing.bmp");break;}
			case 3:	{wsprintf(szPath,".\\image\\client\\alluse\\Desk3playing.bmp");break;}
			case 4:	{wsprintf(szPath,".\\image\\client\\alluse\\Desk4playing.bmp");break;}
			case 5:	{wsprintf(szPath,".\\image\\client\\alluse\\Desk5playing.bmp");break;}
			case 6:	{wsprintf(szPath,".\\image\\client\\alluse\\Desk6playing.bmp");break;}
			case 8:	{wsprintf(szPath,".\\image\\client\\alluse\\Desk8playing.bmp");break;}
			default: break;
			}
		}
		else
		{
			wsprintf(szPath,".\\%d\\image\\desk\\playing.bmp",GameInfo->uNameID);
		}
		m_BeginImage.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);
	}
	if (m_BeginImage.IsNull())
	{
		UnInitDeskFrame();
		return false;
	}

	//��ʼ��ͬ��ͼ
	if (pAgreeImage!=NULL)
		m_AgreeImage.Attach(pAgreeImage,CGameImageLink::m_bAutoLock);
	else 
	{
		if (bIsIco)
		{
			wsprintf(szPath,".\\image\\client\\alluse\\agree.png");
		}
		else
		{
			wsprintf(szPath,".\\%d\\image\\agree.png",GameInfo->uNameID);
		}
		m_AgreeImage.SetLoadInfo(szPath,CGameImageLink::m_bAutoLock);

	}
	if (m_AgreeImage.IsNull())
	{
		UnInitDeskFrame();
		return false;
	}
	

	CGameImageHelper ImageHandle(&m_DeskImage);
	m_iPicWidth=ImageHandle.GetWidth();
	m_iPicHight=ImageHandle.GetHeight();
	for (UINT i=0;i<bDeskCount;i++)
	{
		GetDeskArrayPtr(i)->SetDeskClass(deskClass,deskFlat);
		GetDeskArrayPtr(i)->SetDeskIndex(i);
		GetDeskArrayPtr(i)->SetComType(uComType);
	}
	return true;
}
//ȡ����ʼ��
bool CDeskFrameView::UnInitDeskFrame()
{
	m_iPos=0;				
	m_iMaxPos=0;			
	m_iRowCount=0;		
	m_iPicWidth=0;
	m_iPicHight=0;
	m_iAllLineCount=0;	
	m_iScreenLineCount=0;	
	m_uClassSize=0;
	m_bDeskCount=0;
	m_bEveryCount=0;
	delete [] m_pDeskArray;
	m_pDeskArray=NULL;
	m_DeskImage.Detach();


	return true;
}

void CDeskFrameView::SetBasePoint(BYTE bDeskIndex,int iBasePoint,bool bSet)
{
	CBaseDeskView * pDesk=GetDeskArrayPtr(bDeskIndex);
	if(pDesk!=NULL) pDesk->SetBasePoint(bSet,iBasePoint,this);
}

//�������ӽ�������
void CDeskFrameView::SetCanInput(BYTE bDeskIndex, bool bCanInput)
{
	CBaseDeskView * pDesk=GetDeskArrayPtr(bDeskIndex);
	if(pDesk!=NULL) pDesk->SetCanInput(bCanInput);
}
//��ȡ��Ϸ��ָ��
CBaseDeskView * CDeskFrameView::GetDeskArrayPtr(UINT uIndex)
{
	if ((m_pDeskArray!=NULL)&&(uIndex<m_bDeskCount))
	{
		return ((CBaseDeskView *)((BYTE *)m_pDeskArray+uIndex*m_uClassSize));
	}
	return NULL;
}
//������ͼ����
void CDeskFrameView::SetViewDesk(BYTE bDeskIndex)
{
	if (bDeskIndex<m_bDeskCount)
	{
		//��������
		int iLastPos=m_iPos;
		int iXPos=(bDeskIndex%m_iRowCount)*m_iPicWidth;
		int iYPos=(bDeskIndex/m_iRowCount)*m_iPicHight-m_iPos+m_iYBasePos;
		CRect DeskRect,ClientRect;
		GetClientRect(&ClientRect);
		DeskRect.SetRect(iXPos,iYPos,iXPos+m_iPicWidth,iYPos+m_iPicHight);
		if ((DeskRect.bottom>ClientRect.bottom)||(DeskRect.top<0)) m_iPos+=DeskRect.bottom-ClientRect.bottom;
		m_iPos=__max(0,__min(m_iPos,m_iMaxPos-m_iScreenLineCount*m_iPicHight));
		//����λ��
		if (iLastPos!=m_iPos)
		{
			SetScrollPos(SB_VERT,m_iPos);
			ScrollWindow(0,iLastPos-m_iPos,NULL,NULL);
		}
	}

	return;
}
void CDeskFrameView::OnPaint()
{
     // PengJiLin, 2011-6-23, ������Ϣ//zht 2012-4-23 GDI+�޸Ŀ�������
   /* if(FALSE == m_bActive) 
    {
        CPaintDC dc(this);
        return;
    }*/

	if (NULL == GetSafeHwnd())
	{
		return;
	}
	

	//if (m_byDeskType==BJL_DESK_FACE)
	//{
	//	OnPaintBmp();
	//}
	//else
	{
		OnPaintPng();
	}
}
/// ����PNGͼƬ���ػ�����
void CDeskFrameView::OnPaintPng()
{
	//�滭����
	CPaintDC dc(this);
	//Sleep(1000);

	CRect ClientRect,ClipRect;
	dc.GetClipBox(&ClipRect);
	GetClientRect(&ClientRect);
	//DebugPrintf("CDeskFrameView::OnpaintPng()---ClientRect[x=%d, y=%d, w=%d, h=%d] ClipRect[x=%d, y=%d, w=%d, h=%d]",
	//	ClientRect.left, ClientRect.top, ClientRect.Width(), ClientRect.Height(),
	//	ClipRect.left, ClipRect.top, ClipRect.Width(), ClipRect.Height());
	Gdiplus::RectF rcClipF(ClipRect.left, ClipRect.top, ClipRect.Width(), ClipRect.Height());

	int nWidth = ClientRect.Width(); 
	int nHeight = ClientRect.Height();
	//Bitmap *bmp = new Bitmap(nWidth, nHeight);
	if (m_pBackBmp==NULL)
	{
		return;
	}
	// ����д�Կ��Ĳ���ֻ��һ��
	Graphics graphThis(dc.GetSafeHdc());
	//graphThis.SetClip(rcClipF);
	Rect rcBG(0, -m_iPos, ClientRect.Width(), ClientRect.Height() + m_iMaxPos);
	m_pGraph->SetClip(rcClipF);

	if ((m_r1 == m_r2) 
		||(m_g1 == m_g2)
		||(m_b1 == m_b2)
		)
	{
		SolidBrush solidBrush(Color(255, m_r1, m_g1, m_b1));  //   ��һ����ɫ
		//SolidBrush solidBrush(Color(255, 255, 255, 255));  //   ��һ����ɫ
		m_pGraph->FillRectangle(&solidBrush, Rect(0, 0, ClientRect.Width(), ClientRect.Height()));
	}
	else
	{
		//����ɫ����
		LinearGradientBrush linGrBrush(rcBG,  //   ��������
			Color(255, m_r1, m_g1, m_b1),  //   ��һ����ɫ
			Color(255, m_r2, m_g2, m_b2),  //   �ڶ�����ɫ
			90);  //   ����ɫ�ĽǶ�
		m_pGraph->FillRectangle(&linGrBrush, Rect(0, 0, ClientRect.Width(), ClientRect.Height()));
	}

    ImageAttributes  imAtt;
    imAtt.SetWrapMode(WrapModeClamp);    //ƽ��ͼƬ


	    /////////////ƽ�̱���ͼ
	if (!m_pRoom->IsBJLRoom())
	{
		Bitmap  backgroundtop(L".\\image\\client\\alluse\\queue\\bk.bmp");   //����PNGͼƬ
		TextureBrush brush(&backgroundtop, WrapModeTile/*FlipXY*/ );
		m_pGraph->FillRectangle(&brush, RectF(0.0f, 0.0f,ClientRect.Width(),ClientRect.Height()));
	}
    
    //ƽ��ͼƬ
   // Bitmap  backgroundtop(L".\\image\\client\\alluse\\queue\\bk.bmp");   //����PNGͼƬ
   // m_pGraph->DrawImage(&backgroundtop,Rect(0, 0, ClientRect.Width(), ClientRect.Height()),
   //     0, 0,backgroundtop.GetWidth(),backgroundtop.GetHeight(),UnitPixel,&imAtt);

	CBaseDeskView * pDesk=NULL;
	CRect IntersectRect,DeskRect;



	//ֻ��һ�����ӣ�������ʾ add by ramon
	if ((m_byDeskType==BJL_DESK_FACE) || (1 == m_bDeskCount && m_deskClass != "queue" ))//)
	{
		//dc.FillSolidRect(&ClientRect,crBackColor);
		pDesk=GetDeskArrayPtr(0);
		if (pDesk)
		{
			DeskRect.SetRect((ClientRect.Width() - m_iPicWidth)/2,
				(ClientRect.Height() - m_iPicHight)/2,
				(ClientRect.Width() - m_iPicWidth)/2+m_iPicWidth,
				(ClientRect.Height() - m_iPicHight)/2+m_iPicHight);
			/// �������Ϸ�У�����ʾ��Ϸ�е�����ͼƬ
			if (pDesk->IsPlayGame())
			{
				m_pGraph->DrawImage(m_pImgBegin, DeskRect.left, DeskRect.top, DeskRect.Width(), DeskRect.Height());
			}
			else /// ������Ϸ�У�����ʾƽ������ͼƬ 
			{
				m_pGraph->DrawImage(m_pImgDesk, DeskRect.left, DeskRect.top, DeskRect.Width(), DeskRect.Height());
			}

			if(m_BJLBegin.GetSafeHwnd()!=NULL && m_BJLPlaying.GetSafeHwnd() != NULL)
			{					
				showBJLBeginOrPlayingBtn(m_bShowPlaying);//�ټ��� add by wlr 20090717
			}				
		}
	}//end of �ټ��� add by wlr 20090713
	else if (m_deskClass == "queue" && 1 == m_bDeskCount)
	{

		m_YTip = 20;
		m_YTip1 = m_YTip + m_pImageQueueTip->GetHeight() + 20;
		m_YDesk = (ClientRect.Height() - m_YTip1 - m_iPicHight) / 2 + m_YTip1;

		//Queue_BK_Tip.png
		int x = (ClientRect.Width() - m_pImageQueueTip->GetWidth()) / 2;
		m_pGraph->DrawImage(m_pImageQueueTip, x, m_YTip, m_pImageQueueTip->GetWidth(), m_pImageQueueTip->GetHeight());

		
		m_pGraph->DrawImage(m_pImageQueueTip1, x, m_YTip1, m_pImageQueueTip1->GetWidth(), m_pImageQueueTip1->GetHeight());

		ShowQueueTime(x + m_pImageQueueTip1->GetWidth() / 2 + 14, m_YTip1 + 3, m_CurTime);
		ShowQueuePlan(x + m_pImageQueueTip1->GetWidth() / 2 + 14, m_YTip1 + 45, ((m_SumTime-m_CurTime) * 1.0) / (1.0 * m_SumTime) * 100);

		//y = y + m_pImageQueueTip1->GetHeight();
		//dc.FillSolidRect(&ClientRect,crBackColor);
		pDesk=GetDeskArrayPtr(0);
		if (pDesk)
		{
			m_iXBasePos = (ClientRect.Width() - m_iPicWidth)/2;
			m_iYBasePos = m_YDesk;
			DeskRect.SetRect(
				m_iXBasePos,
				m_iYBasePos,
				m_iXBasePos+m_iPicWidth,
				m_iYBasePos+m_iPicHight);
			/// �������Ϸ�У�����ʾ��Ϸ�е�����ͼƬ

			// �����Ƿ�����Ϸ״̬����ͬ����ͼƬ
			if (pDesk->IsPlayGame())
			{
				m_pGraph->DrawImage(m_pImgBegin, DeskRect.left, DeskRect.top, DeskRect.Width(), DeskRect.Height());
			}
			else
			{
				m_pGraph->DrawImage(m_pImgDesk, DeskRect.left, DeskRect.top, DeskRect.Width(), DeskRect.Height());
			}

			// �����ͬ���־�����Ӻ��룬��
			pDesk->DrawViewFacePng(*m_pGraph, DeskRect, m_pImgAgree, m_pImgLock, m_bIsVip);

			//showQueueBtn(true);
		}

		if (m_QueueStop.GetSafeHwnd())
		{
			if (!m_QueueStop.IsWindowVisible())
			{
				CRect rect,ClientRect;
				GetClientRect(&ClientRect);
				m_QueueStop.GetClientRect(&rect);
				int iWidth = rect.Width();
				int iHeight = rect.Height();

				m_YTip = 20;
				m_YTip1 = m_YTip + m_pImageQueueTip->GetHeight() + 20;
				m_YDesk = (ClientRect.Height() - m_YTip1 - m_iPicHight) / 2 + m_YTip1;

				int y = (ClientRect.Height() - m_YDesk - m_iPicHight - iHeight)/2 + m_YDesk + m_iPicHight - 60;

				m_pGraph->DrawImage(m_pImageQueueTip2, x + 10, y, m_pImageQueueTip2->GetWidth(), m_pImageQueueTip2->GetHeight());
			}
		}
		
		
	}
	else
	{
		//�滭��Ϸ��
		int iBeginRow=m_iPos/m_iPicHight-1;
		iBeginRow=__max(0,iBeginRow);

		for (int i=iBeginRow;i<m_iScreenLineCount+iBeginRow+3;i++)
		{
			for (int j=0;j<m_iRowCount+1;j++)
			{
				DeskRect.SetRect(m_iXBasePos+j*m_iPicWidth,m_iYBasePos+i*m_iPicHight-m_iPos,
					m_iXBasePos+(j+1)*m_iPicWidth,m_iYBasePos+(i+1)*m_iPicHight-m_iPos);
				if ((j<m_iRowCount)&&((pDesk=GetDeskArrayPtr(i*m_iRowCount+j))!=NULL))
				{
					//IntersectRect=DeskRect&ClipRect;
					//if (IntersectRect.IsRectEmpty())
					//{
					//	continue;
					//}

					// �����Ƿ�����Ϸ״̬����ͬ����ͼƬ
					if (pDesk->IsPlayGame())
					{
						m_pGraph->DrawImage(m_pImgBegin, DeskRect.left, DeskRect.top, DeskRect.Width(), DeskRect.Height());
					}
					else
					{
						m_pGraph->DrawImage(m_pImgDesk, DeskRect.left, DeskRect.top, DeskRect.Width(), DeskRect.Height());
					}

					// �����ͬ���־�����Ӻ��룬��
					pDesk->DrawViewFacePng(*m_pGraph, DeskRect, m_pImgAgree, m_pImgLock, m_bIsVip);
				}
			}
		}
	}

	///////////////////////////rqf zht 2012-4-23 GDI+���ֵ����޸Ŀ�������
	Color c;
	m_pBackBmp->GetHBITMAP(c, m_bmp);
	CBitmap* p = CBitmap::FromHandle(*m_bmp);
	m_dcMem.SelectObject(p);
	dc.BitBlt(0,0,ClientRect.Width(),ClientRect.Height(), 
		&m_dcMem,0,0,SRCCOPY);

	p->DeleteObject();
	////////////////////////////////////////////////


	//graphThis.DrawImage(m_pBackBmp, 0, 0);

	if (pGameInfo->dwRoomRule & GRR_CONTEST)
	{
		CFont font;
		font.CreateFont(-16,0,0,0,700,NULL,NULL,NULL,134,3,2,1,2,TEXT("����"));
		CFont font1;
		font1.CreateFont(-12,0,0,0,400,NULL,NULL,NULL,134,3,2,1,2,TEXT("����"));
		dc.SelectObject(&font);
		dc.SetBkMode(TRANSPARENT);
		dc.SetTextColor(RGB(255,255,255));
		CRect rect[7];

		rect[0].SetRect(40, 10, 400, 40);
		for (int i = 1; i < 7; i++)
		{
			rect[i].SetRect(40, 40 + (i-1) * 30, 400, 40 + (i-1) * 30 + 30);
		}

		CString szInfo;
		if (pGameInfo->pMeUserInfo != NULL)
		{
			szInfo.Format("�� %d ��", pGameInfo->pMeUserInfo->GameUserInfo.iContestCount);
		}
		else
		{
			szInfo = "�� 1 ��";
		}
		dc.DrawText(szInfo,szInfo.GetLength(),rect[0],DT_TOP|DT_LEFT|DT_END_ELLIPSIS);
		szInfo = "���ַ�ʽ: ���� * ����";
		dc.SelectObject(&font1);
		dc.DrawText(szInfo,szInfo.GetLength(),rect[1],DT_TOP|DT_LEFT|DT_END_ELLIPSIS);
		szInfo.Format("��Ϸ����: %d", pGameInfo->iBasePoint);
		dc.DrawText(szInfo,szInfo.GetLength(),rect[2],DT_TOP|DT_LEFT|DT_END_ELLIPSIS);
		szInfo.Format("�� %d �����Ͽ��Բ�������", pGameInfo->iLowCount);
		dc.DrawText(szInfo,szInfo.GetLength(),rect[3],DT_TOP|DT_LEFT|DT_END_ELLIPSIS);
		szInfo.Format("���� %I64d �ֽ�����̭", pGameInfo->i64LowChip);
		dc.DrawText(szInfo,szInfo.GetLength(),rect[4],DT_TOP|DT_LEFT|DT_END_ELLIPSIS);	
		if (pGameInfo->pMeUserInfo != NULL)
		{
			szInfo.Format("����: %I64d", pGameInfo->pMeUserInfo->GameUserInfo.i64ContestScore);
		}
		else
		{
			szInfo.Format("����: %I64d", 0);
		}
		dc.DrawText(szInfo,szInfo.GetLength(),rect[5],DT_TOP|DT_LEFT|DT_END_ELLIPSIS);

		if (pGameInfo->pMeUserInfo != NULL)
		{
			if (pGameInfo->pMeUserInfo->GameUserInfo.iRankNum > 0)
			{
				szInfo.Format("��ǰ����������%d",pGameInfo->pMeUserInfo->GameUserInfo.iRankNum);
			}
			else
			{
				szInfo.Format("��ǰ����������");
			}
			
		}
		else
		{
			szInfo.Format("��ǰ����������");
		}
		dc.DrawText(szInfo,szInfo.GetLength(),rect[6],DT_TOP|DT_LEFT|DT_END_ELLIPSIS);	
		
	}
	
	
	//CachedBitmap cb(m_pBackBmp, &graphThis);
	//graphThis.DrawCachedBitmap(&cb,0,0);

	


	//graphThis.DrawImage(m_pBackBmp, 0, 0);
}


void CDeskFrameView::PaintContestInfo()
{
	CPaintDC dc(this);
	CFont font;
	font.CreateFont(-16,0,0,0,700,NULL,NULL,NULL,134,3,2,1,2,TEXT("����"));
	CFont font1;
	font1.CreateFont(-12,0,0,0,400,NULL,NULL,NULL,134,3,2,1,2,TEXT("����"));
	dc.SelectObject(&font);
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(255,255,255));
	CRect rect;
	CString szInfo;
	szInfo = "��һ��";
	rect.SetRect(40, 10, 400, 60);
	dc.DrawText(szInfo,szInfo.GetLength(),rect,DT_TOP|DT_LEFT|DT_END_ELLIPSIS);
	szInfo = "���ַ�ʽ: ���� * ����";
	dc.SelectObject(&font1);
	rect.SetRect(40, 60, 400, 90);
	dc.DrawText(szInfo,szInfo.GetLength(),rect,DT_TOP|DT_LEFT|DT_END_ELLIPSIS);
	szInfo = "��Ϸ����: 10";
	rect.SetRect(40, 90, 400, 120);
	dc.DrawText(szInfo,szInfo.GetLength(),rect,DT_TOP|DT_LEFT|DT_END_ELLIPSIS);
	szInfo = "�� 20 �����Ͽ��Բ�������";
	rect.SetRect(40, 120, 400, 150);
	dc.DrawText(szInfo,szInfo.GetLength(),rect,DT_TOP|DT_LEFT|DT_END_ELLIPSIS);
	szInfo = "���� 500 �ֽ�����̭";
	rect.SetRect(40, 150, 400, 180);
	dc.DrawText(szInfo,szInfo.GetLength(),rect,DT_TOP|DT_LEFT|DT_END_ELLIPSIS);	
}

//�ػ�����
void CDeskFrameView::OnPaintBmp()
{
	//--------------------------------------------
	/// ˫�����沿�ִ���
	CPaintDC realdc(this);
	CRect ClientRect, clipRect;
	GetClipBox( realdc.GetSafeHdc(), &clipRect );
	GetClientRect(&ClientRect);
	int iWidth = ClientRect.Width();
	int iHeight = ClientRect.Height();
	CDC dc;
	CRect rc;
	GetClientRect(&rc);
	dc.CreateCompatibleDC(&realdc);
	CBitmap BufBmp;
	BufBmp.CreateCompatibleBitmap(&realdc, rc.Width(), rc.Height());
	CBitmap * pOldBmp=dc.SelectObject(&BufBmp);
	//--------------------------------------------

	////�滭����
	//CPaintDC dc(this);
	//CRect ClientRect,ClipRect;
	//dc.GetClipBox(&ClipRect);
	//GetClientRect(&ClientRect);

	// {{Added by zxd 20091124 ����ɫ����
	int nWidth = ClientRect.Width(); 
	int nHeight = ClientRect.Height();
	CRect rectangle;
	for (int i=0; i<nHeight; ++i)
	{
		rectangle.SetRect(0, i, nWidth, i+1);
		dc.FillSolidRect(&rectangle, RGB(255-MulDiv(i, 255-204, nHeight),255-MulDiv(i, 255-153, nHeight),204-MulDiv(i, 204-51, nHeight)));
		// 41 102 153
	}
	// Added by zxd 20091124 ����ɫ����}}
	//�滭��Ϸ��
	
	COLORREF crBackColor=m_DeskImage.GetPixel(0,0);
	if (m_pDeskArray!=NULL)
	{
		//����������
		//CDC BufferDC;//������
		//CBitmap BufferBmp;
		//BufferDC.CreateCompatibleDC(&dc);
		//BufferDC.SetBkMode(TRANSPARENT);//͸����, ��Ȼ��, �����ı�����ɫ
		//BufferDC.SelectObject(&CGameImageLink::m_HFont);//��ʾ����
		//BufferDC.SetTextColor(CGameImageLink::m_ColorResource.m_crDeskTxColor);//ѡ����ı���ɫ
		//BufferBmp.CreateCompatibleBitmap(&dc,m_iPicWidth,m_iPicHight);//��������λͼ
		//CBitmap * pOldBmp=BufferDC.SelectObject(&BufferBmp);


		CGameImageHelper DeskImageHandle(&m_DeskImage);//����ͼƬ
		CGameImageHelper SetBasePointImageHandle(&m_TableBasePoint);//���ӱ���ͼƬ
		CGameImageHelper LockImageHandle(&m_LockImage);//��ͼƬ
		CGameImageHelper ArgeeImageHandle(&m_AgreeImage);//ͬ��ͼƬ
		CGameImageHelper BeginImageHandle(&m_BeginImage);
		crBackColor=DeskImageHandle.GetPixel(0,0);
		//BufferDC.FillSolidRect(ClientRect,crBackColor);
		//�滭����
		//�ټ��� add by wlr 20090713
		CBaseDeskView * pDesk=NULL;
		if (1 == m_bDeskCount)//ֻ��һ�����ӣ�������ʾ add by ramon
		{
			//dc.FillSolidRect(&ClientRect,crBackColor);
			pDesk=GetDeskArrayPtr(0);
			if (pDesk)
			{
				pDesk->DrawViewFace(&dc,DeskImageHandle,LockImageHandle,SetBasePointImageHandle,BeginImageHandle,ArgeeImageHandle,
					m_bEveryCount,pGameInfo->dwRoomRule,pGameInfo->uNameID,clrSitDeskFrameLeftTop,clrSitDeskFrameRightBottom);

				//dc.BitBlt((ClientRect.Width() - m_iPicWidth)/2,(ClientRect.Height() - m_iPicHight)/2,m_iPicWidth,m_iPicHight,&BufferDC,0,0,SRCCOPY);			
				
				if(m_BJLBegin.GetSafeHwnd()!=NULL && m_BJLPlaying.GetSafeHwnd() != NULL)
				{					
					showBJLBeginOrPlayingBtn(m_bShowPlaying);//�ټ��� add by wlr 20090717
				}				
			}
		}//end of �ټ��� add by wlr 20090713
		else
		{	
			CRect IntersectRect,DeskRect;
			int iBeginRow=m_iPos/m_iPicHight-1;
			iBeginRow=__max(0,iBeginRow);
			for (int i=iBeginRow;i<m_iScreenLineCount+iBeginRow+3;i++)
			{
				for (int j=0;j<m_iRowCount+1;j++)
				{
					CBaseDeskView * pDesk=NULL;
					DeskRect.SetRect(m_iXBasePos+j*m_iPicWidth,m_iYBasePos+i*m_iPicHight-m_iPos,
									 m_iXBasePos+(j+1)*m_iPicWidth,m_iYBasePos+(i+1)*m_iPicHight-m_iPos);
					if ((j<m_iRowCount)&&((pDesk=GetDeskArrayPtr(i*m_iRowCount+j))!=NULL))
					{
						IntersectRect=DeskRect&clipRect;
						if (IntersectRect.IsRectEmpty()==FALSE)
						{
							pDesk->DrawViewFace(&dc,DeskImageHandle,LockImageHandle,SetBasePointImageHandle,BeginImageHandle,ArgeeImageHandle,
								m_bEveryCount,pGameInfo->dwRoomRule,pGameInfo->uNameID,clrSitDeskFrameLeftTop,clrSitDeskFrameRightBottom);
							//dc.BitBlt(DeskRect.left,DeskRect.top,m_iPicWidth,m_iPicHight,&BufferDC,0,0,SRCCOPY);
						}
					}
					//else
					//{
					//	dc.FillSolidRect(&DeskRect,crBackColor);
					//}
				}
			}
		}

		////������Դ
		//BufferDC.SelectObject(pOldBmp);
		//BufferBmp.DeleteObject();
		//BufferDC.DeleteDC();
	}
	//else dc.FillSolidRect(&ClipRect,crBackColor);

	//--------------------------------------------
	/// ˫������沿�ִ���
	realdc.BitBlt(
		clipRect.left,
		clipRect.top,
		clipRect.Width(), 
		clipRect.Height(),
		&dc,
		clipRect.left,
		clipRect.top,
		SRCCOPY);
	dc.SelectObject(pOldBmp);
	BufBmp.DeleteObject();
	dc.DeleteDC();
	/// ˫������沿�ִ���
	//--------------------------------------------
}

//���̰�����Ϣ
void CDeskFrameView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_UP://�����Ϸ����
		{
			SendMessage(WM_VSCROLL,MAKELONG(SB_LINEUP,m_iPos),NULL);//����
			return;
		}
	case VK_DOWN://�����·����
		{
			SendMessage(WM_VSCROLL,MAKELONG(SB_LINEDOWN,m_iPos),NULL);//�ߵ�
			return;
		}
	case VK_PRIOR://ǰ��ҳ
		{
			SendMessage(WM_VSCROLL,MAKELONG(SB_PAGEUP,m_iPos),NULL);//ǰ��ҳ
			return;
		}
	case VK_NEXT://��ҳ 
	case VK_SPACE://�ո�
		{
			SendMessage(WM_VSCROLL,MAKELONG(SB_PAGEDOWN,m_iPos),NULL);
			return;
		}
	case VK_HOME://HOME
		{
			SendMessage(WM_VSCROLL,MAKELONG(SB_TOP,m_iPos),NULL);
			return;
		}
	case VK_END://END
		{
			SendMessage(WM_VSCROLL,MAKELONG(SB_BOTTOM,m_iPos),NULL);
			return;
		}
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);//����������
}
//��������Ϣ
void CDeskFrameView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar)
{
	int iLastPos=m_iPos;
	switch (nSBCode)
	{
	case SB_TOP:
		{
			m_iPos=0;
			break;
		}
	case SB_BOTTOM:
		{
			m_iPos=m_iMaxPos-m_iScreenLineCount*m_iPicHight;
			break;
		}
	case SB_LINEUP:
		{
			m_iPos-=m_iScrollPos;
			break;
		}
	case SB_PAGEUP:
		{
			m_iPos-=m_iScreenLineCount*m_iPicHight;
			break;
		}
	case SB_LINEDOWN:
		{
			m_iPos+=m_iScrollPos;
			break;
		}
	case SB_PAGEDOWN:
		{
			m_iPos+=m_iScreenLineCount*m_iPicHight;
			break;
		}
	case SB_THUMBTRACK:
		{
			m_iPos=nPos;
			break;
		}
	}

	//����λ��
	SetFocus();
	m_iPos=__max(0,__min(m_iPos,m_iMaxPos-m_iScreenLineCount*m_iPicHight));
	if (iLastPos!=m_iPos)
	{
		SetScrollPos(SB_VERT,m_iPos);
		ScrollWindow(0,iLastPos-m_iPos,NULL,NULL);
	}
	//���ð�ťλ��
/*	if(m_pageup.GetSafeHwnd()!=NULL)
	{
		CRect rect;
		this->GetWindowRect(&rect);
		int cx,cy;
		cx=rect.Width();
		cy=rect.Height();
		m_pageup.MoveWindow(cx-30,cy/3,26,34);
		m_pagedown.MoveWindow(cx-30,cy*2/3,26,34);
	}*/
	return;
}

//��������Ϣ
BOOL CDeskFrameView::OnMouseWheel(UINT nFlags, short zDelta, CPoint MousePoint)
{
	//����λ��
	int iLastPos=m_iPos;
	m_iPos=__max(0,__min(m_iPos-zDelta/m_iWhellTimes,m_iMaxPos-m_iScreenLineCount*m_iPicHight));
	if (iLastPos != m_iPos)
	{
		SetScrollPos(SB_VERT,m_iPos);
		ScrollWindow(0,iLastPos-m_iPos,NULL,NULL);
		//Invalidate();
	}
/*	if(m_pageup.GetSafeHwnd()!=NULL)
	{
		CRect rect;
		this->GetWindowRect(&rect);
		int cx,cy;
		cx=rect.Width();
		cy=rect.Height();
		m_pageup.MoveWindow(cx-30,cy/3,26,34);
		m_pagedown.MoveWindow(cx-30,cy*2/3,26,34);
	}*/
	return TRUE;
}

//λ����Ϣ
void CDeskFrameView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	if(cx==0 || cy==0)
		return;
	//���Ļ���ͼ
	if (SIZE_MINIMIZED!=nType)
	{
		int w=0, h=0;
		if (m_pBackBmp != NULL)
		{
			w = m_pBackBmp->GetWidth();
			h = m_pBackBmp->GetHeight();
		}
		if ((m_pBackBmp==NULL)||(cx>w) ||(cy>h))
		{
			delete m_pBackBmp;
			m_pBackBmp = new Bitmap(cx, cy);
			delete m_pGraph;
			m_pGraph = new Graphics(m_pBackBmp);
			m_dcMem.DeleteDC();
			m_dcMem.CreateCompatibleDC(GetDC());
		}
	}

	//����λ��
	if(m_iPicWidth==0 || m_iPicHight==0)
	{
		//AFCMessageBox("System Failed ");
	    DUIMessageBox(m_hWnd,MB_ICONINFORMATION|MB_OK,"ϵͳ��ʾ",false,"System Failed ");
		return;
	}
	if (m_bDeskCount!=0)
	{
		int iWidth=__max(m_iPicWidth,cx);
		int iHeight=__max(m_iPicHight,cy);
		m_iRowCount=iWidth/m_iPicWidth;
		m_iScreenLineCount=iHeight/m_iPicHight;
		m_iAllLineCount=(m_bDeskCount+m_iRowCount-1)/m_iRowCount;
		m_iXBasePos=(iWidth-m_iRowCount*m_iPicWidth)/2;
	}

	/// �����ټ������͵Ŀ�ʼ��ť��λ��
	char cKey[10];
	CString sPath=CBcfFile::GetAppPath (false);
	CBcfFile fsr( sPath + "SpecialRule.bcf");
	sprintf(cKey, "%d", pGameInfo->uNameID);
	int iResult = fsr.GetKeyVal(_T("BJLType"), cKey, 0);

	if (iResult)	/// ����ǰټ�������
	{
		CRect rect,ClientRect;
		GetClientRect(&ClientRect);
		int iClientWidth = ClientRect.Width();
		int iClientHeight = ClientRect.Height();
		m_BJLBegin.GetClientRect(&rect);
		int iWidth = rect.Width();
		int iHeight = rect.Height();
		//int x = (ClientRect.Width()-m_iPicWidth)/2;
		//if (x<0)
		//{
		//	x=0;
		//}
		//x += m_iPicWidth-iWidth-20;
		//int y = (ClientRect.Height()-m_iPicHight)/2;
		//if (y<0)
		//{
		//	y=0;
		//}
		//y+=m_iPicHight-iHeight-20;
	//	m_BJLBegin.MoveWindow(x,y,iWidth,iHeight);
		m_BJLBegin.MoveWindow(ClientRect.Width()-iWidth-20,ClientRect.Height()-iHeight-20,iWidth,iHeight);
	//	m_BJLPlaying.MoveWindow(x,y,iWidth,iHeight);
		m_BJLPlaying.MoveWindow(ClientRect.Width()-iWidth-20,ClientRect.Height()-iHeight-20,iWidth,iHeight);




		//ShowBJLBeginBtn();// �ټ��� add by wlr 20090713
	}

	if (m_deskClass == "queue")
	{
		CRect rect,ClientRect;
		GetClientRect(&ClientRect);
		m_QueueStop.GetClientRect(&rect);
		int iWidth = rect.Width();
		int iHeight = rect.Height();

	m_YTip = 20;
	m_YTip1 = m_YTip + m_pImageQueueTip->GetHeight() + 20;
	m_YDesk = (ClientRect.Height() - m_YTip1 - m_iPicHight) / 2 + m_YTip1;

		int x = (ClientRect.Width() - iWidth)/2;
		int y = (ClientRect.Height() - m_YDesk - m_iPicHight - iHeight)/2 + m_YDesk + m_iPicHight - 60;

		m_QueueBegin.MoveWindow(x , y,iWidth,iHeight);
		m_QueueStop.MoveWindow(x, y,iWidth,iHeight);
	}
/*	if(m_pageup.GetSafeHwnd()!=NULL)
	{
		m_pageup.MoveWindow(cx-30,cy/3,26,34);
		m_pagedown.MoveWindow(cx-30,cy*2/3,26,34);
	}*/
	//���ù�������
	m_iMaxPos=m_iAllLineCount*m_iPicHight;//+m_iYBasePos;
	m_iPos=__max(0,__min(m_iPos,m_iMaxPos-m_iScreenLineCount*m_iPicHight));


	//���ù�����
	SCROLLINFO ScrollInfo;
	ScrollInfo.cbSize=sizeof(SCROLLINFO);
	ScrollInfo.fMask=SIF_RANGE|SIF_PAGE|SIF_POS;
	ScrollInfo.nMin=0;
	ScrollInfo.nMax=m_iMaxPos;
	ScrollInfo.nPage=m_iScreenLineCount*m_iPicHight;
	ScrollInfo.nPos=m_iPos;
	SetScrollInfo(SB_VERT,&ScrollInfo,TRUE);

	return;
}

//����ƶ���Ϣ
void CDeskFrameView::OnMouseMove(UINT nFlags, CPoint point)
{
	//CGameRoomEx* pGameRoom = GetGameRoom();
	CGameRoomEx* pGameRoom = GetGameRoom(pGameInfo->uRoomID);

	if (pGameRoom != NULL)
	{
		//if (pGameRoom->IsQueueGameRoom())//Ϊ�Ŷӻ����
		//{
		//	return;
		//}
	}

	if ((m_bRightMouseDown==false)&&(m_bLeftMouseDown==false))
	{
		//���԰���
		BYTE bNewDeskIndex=255,bNewDeskStation=255;
		bNewDeskIndex=MouseHitTest(point);
		if (bNewDeskIndex!=255)
		{
			CBaseDeskView * pDeskPtr=GetDeskArrayPtr(bNewDeskIndex);
			CPoint DeskPoint((point.x-m_iXBasePos)%m_iPicWidth,(point.y+m_iPos-m_iYBasePos)%m_iPicHight);
			bNewDeskStation=pDeskPtr->MouseHitTest(DeskPoint);
		}

		//���½���
		if (((bNewDeskIndex!=m_bDownDeskIndex)||(bNewDeskStation!=m_bDownDeskStation))
			&&((bNewDeskStation!=255)||(m_bDownDeskStation!=255)))
		{
			if (m_bDownDeskIndex!=255) GetDeskArrayPtr(m_bDownDeskIndex)->SetHoverIndex(255,this);
			if (bNewDeskIndex!=255) GetDeskArrayPtr(bNewDeskIndex)->SetHoverIndex(bNewDeskStation,this);
		}

		//�����Ҽ��������û���Ϣ modify by lxl 2010-11-8
		if(bNewDeskIndex == 255|| bNewDeskStation == 255)
		{
			m_Tooltip.Activate(false);
			if(m_personalInfo.GetSafeHwnd()!=NULL)
			{
				m_personalInfo.ShowWindow(SW_HIDE);
				Invalidate(FALSE);
				m_personalInfo.close();//DestroyWindow();
				//InvalidateRect(NULL,true);
			}
		}

		//��������
		m_bDownDeskIndex=bNewDeskIndex;
		m_bDownDeskStation=bNewDeskStation;
	}
	CWnd::OnMouseMove(nFlags, point);
}
//����Ҽ�������Ϣ
void CDeskFrameView::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	if (m_bRightMouseDown==false)
	{
		//���԰���
		BYTE bDeskIndex=MouseHitTest(point);
		if (bDeskIndex!=255)
		{
			CBaseDeskView * pDeskPtr=GetDeskArrayPtr(bDeskIndex);
			CPoint DeskPoint((point.x-m_iXBasePos)%m_iPicWidth,(point.y+m_iPos-m_iYBasePos)%m_iPicHight);
			BYTE bDeskStation=pDeskPtr->MouseHitTest(DeskPoint);
			if (bDeskStation!=255)
			{
				//��������
				m_bLeftMouseDown=true;
				pDeskPtr->SetMouseDown(true,this);
				SetCapture();
			}
		}
	}
	CWnd::OnLButtonDown(nFlags, point);
}

//����Ҽ�������Ϣ
void CDeskFrameView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if ((m_bLeftMouseDown==true)&&(m_bRightMouseDown==false))
	{
		//���԰���
		ReleaseCapture();
		m_bLeftMouseDown=false;
		BYTE bDeskIndex=MouseHitTest(point);
		GetDeskArrayPtr(bDeskIndex)->SetMouseDown(false,this);
		if ((bDeskIndex==m_bDownDeskIndex)&&(bDeskIndex!=255))
		{
			if(m_bIsClick)
				return ;
			m_bIsClick=true;
			CBaseDeskView * pDeskPtr=GetDeskArrayPtr(bDeskIndex);
			CPoint DeskPoint((point.x-m_iXBasePos)%m_iPicWidth,(point.y+m_iPos-m_iYBasePos)%m_iPicHight);
			BYTE bDeskStation=pDeskPtr->MouseHitTest(DeskPoint);
			if(m_uComType!=TY_MATCH_GAME && !(pGameInfo->dwRoomRule & GRR_AUTO_SIT_DESK))//���з����׳�
			{
				if ((bDeskStation==m_bDownDeskStation)&&(bDeskStation!=255))
					GetParent()->SendMessage(WM_LEFT_MOUSE_HIT_DESK,m_bDownDeskIndex,m_bDownDeskStation);
			}
			//if(m_uComType==TY_MATCH_GAME)
			//{
			//	GetParent()->SendMessage(WM_HITDESK,m_bDownDeskIndex,m_bDownDeskStation);
			//}
			SetTimer(IDR_CLICKDESK,100,NULL);
		}
	}
	CWnd::OnLButtonUp(nFlags, point);
}

//�Ҽ�������Ϣ
void CDeskFrameView::OnRButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	if (m_bLeftMouseDown==false)
	{
		//���԰���
		BYTE bDeskIndex=MouseHitTest(point);
		if (bDeskIndex!=255)
		{
			CBaseDeskView * pDeskPtr=GetDeskArrayPtr(bDeskIndex);
			CPoint DeskPoint((point.x-m_iXBasePos)%m_iPicWidth,(point.y+m_iPos-m_iYBasePos)%m_iPicHight);
			BYTE bDeskStation=pDeskPtr->MouseHitTest(DeskPoint);
			if (bDeskStation!=255)
			{
				//��������
				m_bRightMouseDown=true;
				pDeskPtr->SetMouseDown(m_bRightMouseDown,this);
				SetCapture();
			}
		}
	}
	CWnd::OnRButtonDown(nFlags, point);
}
//�Ҽ�������Ϣ
void CDeskFrameView::OnRButtonUp(UINT nFlags, CPoint point)
{
	if ((m_bRightMouseDown==true)&&(m_bLeftMouseDown==false))	
	{
		//���԰���
		ReleaseCapture();
		m_bRightMouseDown=false;
		BYTE bDeskIndex=MouseHitTest(point);
		GetDeskArrayPtr(bDeskIndex)->SetMouseDown(m_bRightMouseDown,this);
		if ((bDeskIndex==m_bDownDeskIndex)&&(bDeskIndex!=255))
		{
			CBaseDeskView * pDeskPtr=GetDeskArrayPtr(bDeskIndex);
			CPoint DeskPoint((point.x-m_iXBasePos)%m_iPicWidth,(point.y+m_iPos-m_iYBasePos)%m_iPicHight);
			BYTE bDeskStation=pDeskPtr->MouseHitTest(DeskPoint);

			if ((bDeskStation==m_bDownDeskStation)&&(bDeskStation!=255))
				GetParent()->SendMessage(WM_RIGHT_MOUSE_HIT_DESK,m_bDownDeskIndex,m_bDownDeskStation);

			//�����Ҽ������û���Ϣ add by lxl 2010-11-8
			UserItemStruct *user;
			user =this->GetUserItem(bDeskIndex,bDeskStation);
			if(user!=NULL && user->GameUserInfo.dwUserID!=m_UserID && m_UserID!=0)
			{
				if(m_uComType!=TY_MATCH_GAME && !(pGameInfo->dwRoomRule&GRR_AUTO_SIT_DESK))
				{
					
// 					if(m_personalInfo.GetSafeHwnd()==NULL)
// 					{	
// 						m_personalInfo.Create(IDD_PERSONINFO,this);
// 						m_personalInfo.getRoomPtr(GetParent());
// 
// 						//������ת������Ļ����
// 						CPoint p = point;
// 						/// ������λ��̫�ͣ�����Ҫ���λ�ã����������Ϣ�����
// 						/// ��ػ��ܵ���Ļ����
// 						int w= m_personalInfo.GetWidth();
// 						int h= m_personalInfo.GetHeight();
// 						RECT rcClient;
// 						GetClientRect(&rcClient);
// 						if (p.y + h > rcClient.bottom)
// 						{
// 							p.y = rcClient.bottom - h;
// 						}
// 						ClientToScreen(&p);
// 						m_personalInfo.MoveWindow(p.x, p.y, w, h);//JMod-185,255);-20090506
// 						
// 						m_personalInfo.UpdateUserItem(user->GameUserInfo);
// 						if(pOrderName!=NULL)
// 							m_personalInfo.pOrderName=pOrderName;
// 
// 						m_personalInfo.ShowWindow(SW_NORMAL);
// 						m_personalInfo.BringWindowToTop();
// 					}else 
// 					{
// 						m_personalInfo.UpdateUserItem(user->GameUserInfo);
// 						m_personalInfo.ShowWindow(SW_NORMAL);
// 						m_personalInfo.BringWindowToTop();
// 					}	

					//JianGuankun 2011.11.19 //�Ҽ����Logo�����˵�
					CGameRoomEx* pGameRoomEx = (CGameRoomEx*)GetParent();
					if (pGameRoomEx)
					{
						pGameRoomEx->OpenUserMenuEx(user,true);
					}
					//end add
				}
				m_Tooltip.Update();
				m_Tooltip.Activate(true);
			}
		}
	}
	CWnd::OnRButtonUp(nFlags, point);
}
//��������
BYTE CDeskFrameView::MouseHitTest(POINT point)
{
	point.y+=m_iPos;
	if ((m_iPicHight<=0) || (m_iPicWidth<=0))
	{
		return 255;
	}
	int iDeskRow=(point.x-m_iXBasePos)/m_iPicWidth;
	int iDeskLine=(point.y-m_iYBasePos)/m_iPicHight;
	if ((iDeskRow>=0)&&(iDeskRow<m_iRowCount))
	{
		BYTE bDeskIndex=iDeskLine*m_iRowCount+iDeskRow;
		return (bDeskIndex<m_bDeskCount)?bDeskIndex:255;
	}
	return 255;
}
//������Ϸ����
void CDeskFrameView::UpdateDeskView(BYTE bDeskIndex)
{
	CBaseDeskView * pDeskPtr=GetDeskArrayPtr(bDeskIndex);
	if (m_iRowCount<=0)
	{
		return;
	}
	if (pDeskPtr!=NULL && this->m_hWnd != NULL) 
	{
		int iXPos=(bDeskIndex%m_iRowCount)*m_iPicWidth;
		int iYPos=(bDeskIndex/m_iRowCount)*m_iPicHight-m_iPos;
		CRect DeskRect(iXPos+m_iXBasePos,iYPos+m_iYBasePos,m_iXBasePos+iXPos+m_iPicWidth,m_iYBasePos+iYPos+m_iPicHight);
		InvalidateRect(&DeskRect,FALSE);
	}
	return;
}

//���ù��
BOOL CDeskFrameView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if ((m_bRightMouseDown==false)&&(m_bLeftMouseDown==false))
	{
		CPoint MousePoint;
		GetCursorPos(&MousePoint);
		ScreenToClient(&MousePoint);
		BYTE bDeskIndex=MouseHitTest(MousePoint);
		if (bDeskIndex!=255)
		{
			CBaseDeskView * pDeskPtr=GetDeskArrayPtr(bDeskIndex);
			CPoint DeskPoint((MousePoint.x-m_iXBasePos)%m_iPicWidth,(MousePoint.y+m_iPos-m_iYBasePos)%m_iPicHight);
			BYTE bDeskStation=pDeskPtr->MouseHitTest(DeskPoint);
			if (bDeskStation!=255)
			{
				SetCursor(m_hMouseCursor);
				return TRUE;
			}
		}
	}
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

//�Ƿ���Ϸ
bool CDeskFrameView::IsPlayGame(BYTE bDeskIndex)
{
	CBaseDeskView * pDesk=GetDeskArrayPtr(bDeskIndex);
	if (pDesk!=NULL) return pDesk->IsPlayGame();
	return false;
}
//�Ƿ�ȫ����ʼͬ��
bool CDeskFrameView::IsAllAgree(BYTE bDeskIndex)
{
	CBaseDeskView * pDeskPtr=GetDeskArrayPtr(bDeskIndex);
	if (pDeskPtr!=NULL) 
	{
		return (pDeskPtr->IsAllAgree());
	}
	return false;
}
//����ͬ����Ա����
BYTE CDeskFrameView::CountAgreePeople(BYTE bDeskIndex)
{
	CBaseDeskView * pDeskPtr=GetDeskArrayPtr(bDeskIndex);
	if (pDeskPtr!=NULL) 
	{
		return (pDeskPtr->CountAgree());
	}
	return 0;
}
//�Ƿ���������
bool CDeskFrameView::IsSetPass(BYTE bDeskIndex)
{
	CBaseDeskView * pDesk=GetDeskArrayPtr(bDeskIndex);
	if (pDesk!=NULL) return pDesk->IsSetPass();
	return false;
}

//��������־
void CDeskFrameView::SetLock(BYTE bDeskIndex, bool bLock)
{
	CBaseDeskView * pDesk=GetDeskArrayPtr(bDeskIndex);
	if (pDesk!=NULL) pDesk->SetLock(bLock,this);
	return;
}

//���ÿ�ʼ��־
void CDeskFrameView::SetPlayGame(BYTE bDeskIndex, bool bPlaying)
{
	CBaseDeskView * pDesk=GetDeskArrayPtr(bDeskIndex);
	if (pDesk!=NULL)
	{	
		pDesk->SetPlayGame(bPlaying,this);
		//showBJLBeginOrPlayingBtn(bPlaying);//�ټ��� add by wlr 20090717
	}
	return;
}

//�����û���Ϣ
bool CDeskFrameView::SetUserItem(BYTE bDeskIndex, BYTE bDeskStation, UserItemStruct * pUserItem)
{
	CBaseDeskView * pDesk=GetDeskArrayPtr(bDeskIndex);
	if (pDesk!=NULL) return pDesk->SetUserItem(bDeskStation,pUserItem,this);
	return false;
}

//��ȡ�û���Ϣ
UserItemStruct * CDeskFrameView::GetUserItem(BYTE bDeskIndex, BYTE bDeskStation)
{
	CBaseDeskView * pDesk=GetDeskArrayPtr(bDeskIndex);
	if (pDesk!=NULL) return pDesk->GetUserItem(bDeskStation);
	return NULL;
}

//��ȡ������Ϣ
long int CDeskFrameView::GetMatchInfo(BYTE bDeskIndex, BYTE bDeskStation)
{
	CBaseDeskView * pDesk=GetDeskArrayPtr(bDeskIndex);
	if (pDesk!=NULL) return pDesk->GetMatchInfo(bDeskStation);
	return 0L;
}

//���ñ�����Ϣ
bool CDeskFrameView::SetMatchInfo(BYTE bDeskIndex, BYTE bDeskStation, long int dwUserID, TCHAR * szUserName)
{
	CBaseDeskView * pDesk=GetDeskArrayPtr(bDeskIndex);
	if (pDesk!=NULL) return pDesk->SetMatchInfo(bDeskStation,dwUserID,szUserName,this);
	return false;
}

//�����û��߿�
void CDeskFrameView::SetUserFrame(BYTE bDeskIndex)
{
	//����ɱ߿�
	if (m_bUserFrameIndex!=m_bFindFrameIndex)
	{
		CBaseDeskView * pDesk=GetDeskArrayPtr(m_bUserFrameIndex);
		if (pDesk!=NULL) pDesk->SetShowFrame(false,this);
	}
	m_bUserFrameIndex=255;

	//�����±߿�
	m_bUserFrameIndex=bDeskIndex;
	CBaseDeskView * pDesk=GetDeskArrayPtr(m_bUserFrameIndex);
	if (pDesk!=NULL) pDesk->SetShowFrame(true,this);

	return;
}

//���ò��ұ߿�
void CDeskFrameView::SetFindFrame(BYTE bDeskIndex)
{
	//����ɱ߿�
	if (m_bFindFrameIndex!=m_bUserFrameIndex)
	{
		CBaseDeskView * pDesk=GetDeskArrayPtr(m_bFindFrameIndex);
		if (pDesk!=NULL) pDesk->SetShowFrame(false,this);
	}
	m_bFindFrameIndex=255;

	//�����±߿�
	m_bFindFrameIndex=bDeskIndex;
	CBaseDeskView * pDesk=GetDeskArrayPtr(m_bFindFrameIndex);
	if (pDesk!=NULL) pDesk->SetShowFrame(true,this);

	return;
}

//����λ��
BYTE CDeskFrameView::FindGameDesk(BYTE bLastFind, bool bOneNull, bool bTwoNull, bool bNoAllNull, bool bAllNull)
{
	BYTE bCheckDesk=bLastFind,bNullCount=0;
	for (BYTE i=0;i<m_bDeskCount;i++)
	{
		bCheckDesk++;
		if (bCheckDesk>=m_bDeskCount) bCheckDesk=0;
		if ((IsPlayGame(bCheckDesk)==false)&&((bNullCount=GetNullCount(bCheckDesk))!=0))
		{
			if (((bOneNull==true)&&(bNullCount==1))||((bTwoNull==true)&&(bNullCount==2))||
				((bNoAllNull==true)&&(bNullCount!=m_bEveryCount))||((bAllNull==true)&&(bNullCount==m_bEveryCount)))
			{
				//�ҵ�λ��
				if(m_uComType!=TY_MATCH_GAME)
				{
					SetViewDesk(bCheckDesk);
					SetFindFrame(bCheckDesk);
				}
				return bCheckDesk;
			}
		}
	}
	return 255;
}

//��ȡ��λ��
BYTE CDeskFrameView::GetNullStation(BYTE bDeskIndex)
{
	CBaseDeskView * pDeskView=GetDeskArrayPtr(bDeskIndex);
	if (pDeskView!=NULL)
	{
		for (BYTE i=0;i<m_bEveryCount;i++)
		{
			if (pDeskView->m_pUserItem[i]==NULL) return i;
		}
	}
	return 255;
}

//��ȡ��λ����Ŀ
BYTE CDeskFrameView::GetNullCount(BYTE bDeskIndex)
{
	CBaseDeskView * pDeskView=GetDeskArrayPtr(bDeskIndex);
	if (pDeskView!=NULL)
	{
		BYTE bNullCount=0;
		for (BYTE i=0;i<m_bEveryCount;i++)
		{
			if (pDeskView->m_pUserItem[i]==NULL) bNullCount++;
		}
		return bNullCount;
	}
	return 0;
}

//����Լ��û�ID
bool CDeskFrameView::GetMeUserID(long MeUserID) 
{
	if(MeUserID>=0)
	{
		m_UserID=MeUserID;
		return true;
	}else return false;
}
/*******************************************************************************************************/

BOOL CDeskFrameView::PreTranslateMessage(MSG* pMsg)
{
	m_Tooltip.RelayEvent(pMsg);
	return CWnd::PreTranslateMessage(pMsg);
}

//�ټ��� add by wlr 20090713
void CDeskFrameView::OnHitBJLBegin()
{
	CWnd* wnd=GetParent();
	if(wnd!=NULL)
		::PostMessage(wnd->GetSafeHwnd(),WM_HIT_BJL_BEGIN,0,0);
	
	////m_bShowPlaying = true;
	//m_BJLPlaying.EnableWindow(FALSE);
	//m_BJLBegin.EnableWindow(FALSE);
	//ShowBJLBeginBtn();
}
//end of add by wlr 20090713

//���Ϸ�ҳ��ť
void CDeskFrameView::OnHitpageup()
{
	::SendMessage(this->m_hWnd,WM_VSCROLL,SB_PAGEUP,0);
	return;
}
//��ʾ��ʼ��ť
void CDeskFrameView::ShowBeginBtn(BYTE bDeskIndex,BYTE bDeskStation,bool bShow)
{
	if(m_BeginAgree.GetSafeHwnd()!=NULL)
	{
		int iXPos=(bDeskIndex%m_iRowCount)*m_iPicWidth;
		int iYPos=(bDeskIndex/m_iRowCount)*m_iPicHight-m_iPos+m_iYBasePos;
		CRect rect;
		m_BeginAgree.GetClientRect(&rect);
		int iWidth = rect.Width();
		int iHeight = rect.Height();

		int startBtX,startBtY;
		/// X����
		startBtX = iXPos + (m_iPicWidth - iWidth)/2 + m_iXBasePos;
		/// Y����
		startBtY = iYPos + m_iPicHight- iHeight;

		m_BeginAgree.MoveWindow(startBtX,startBtY,iWidth,iHeight);

		m_BeginAgree.ShowWindow(bShow?1:0);
#ifdef AI_MACHINE
		if(bShow)
			OnHitBegin();
#endif
	}
}
void CDeskFrameView::OnHitBegin()
{
	CWnd* wnd=GetParent();
	if(wnd!=NULL)
		::PostMessage(wnd->GetSafeHwnd(),WM_HIT_DESK_BEGIN,0,0);
}
//���Ϸ�ҳ��ť
void CDeskFrameView::OnHitpagedown()
{
	::SendMessage(this->GetSafeHwnd(),WM_VSCROLL,SB_PAGEDOWN,0);
	return;
}

void CDeskFrameView::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==IDR_CLICKDESK)
	{
		KillTimer(IDR_CLICKDESK);
		m_bIsClick=false;
	}
	if (nIDEvent == TIME_QUEUE)
	{
		if (m_CurTime > 0)
			m_CurTime--;
		else
			m_CurTime = m_SumTime;

		//CRect rect,ClientRect;
		//GetClientRect(&ClientRect);
		//int iWidth = m_pImageQueueTip1->GetWidth();
		//int iHeight = m_pImageQueueTip->GetHeight();
		//m_YTip = 20;
		//m_YTip1 = m_YTip + iHeight + 20;
		//int x = (ClientRect.Width() - iWidth) / 2;

		//ShowQueueTime(x + iWidth / 2 + 20, m_YTip1 + 10, m_CurTime);
		//ShowQueuePlan(x + iWidth / 2 + 20, m_YTip1 + 48, ((m_SumTime-m_CurTime) * 1.0) / (1.0 * m_SumTime) * 100);

		////CRect DeskRect(x, m_YTip1, iWidth, iHeight);
		//CRect DeskRect(0, m_YTip1, 800, iHeight);
		////Invalidate();
		//InvalidateRect(&DeskRect, TRUE);
		Invalidate();
	}
	CWnd::OnTimer(nIDEvent);
}
