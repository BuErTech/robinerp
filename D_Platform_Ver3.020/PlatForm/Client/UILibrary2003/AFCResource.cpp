#include "StdAfx.h"
#include "Resource.h" 
#include "AFCResource.h"
#include "AFCLogoResource.h"

//���ֿռ�
//using namespace AFC;
//using namespace AFC::AFCResource;

//ȫ��ʹ�ñ���
bool					CGameImageLink::m_bAutoLock=true;				//�Ƿ����
CFont					CGameImageLink::m_VFont;						//��ʾ����
CFont					CGameImageLink::m_HFont;						//��ʾ����
HCURSOR					CGameImageLink::m_hHandCursor=NULL;				//�����
CAFCTabResource			CGameImageLink::m_TabResource;					//����ҳ��Դ
CAFCTitleResource		CGameImageLink::m_TitleResource;				//������Դ
CAFCButtonResource		CGameImageLink::m_ButtonResource;				//��ť��Դ
CAFCListResource		CGameImageLink::m_ListResource;					//�б���Դ
CAFCDialogResource		CGameImageLink::m_DialogResource;				//�Ի�����Դ
CAFCEditResource		CGameImageLink::m_EditResource;					//�༭�ؼ���Դ
CAFCSplitResource		CGameImageLink::m_SplitResource;				//�������Դ
CAFCColorResource		CGameImageLink::m_ColorResource;				//��ɫ��Դ

/*******************************************************************************************************/

//��ʼ����Դ
bool CGameImageLink::InitResource(TCHAR * szSinkFileName)
{
	//��Դ���
	HINSTANCE hDllHinstance=::GetModuleHandle(FACE_DLL_NAME);
	//��ʼ��ͷ����Դ
	CGameLogoRes::Init();

	//��ʼ��ȫ����Դ
	m_VFont.CreateFont(-12,0,900,900,400,0,0,0,134,0,0,0,0,TEXT("������"));
	m_HFont.CreateFont(-12,0,0,0,400,0,0,0,134,3,2,1,2,TEXT("������"));
	m_hHandCursor=::LoadCursor(hDllHinstance,MAKEINTRESOURCE(IDC_HAND));
	
	//��ʼ������ҳ��Դ
	m_TabResource.m_crFrameHeight=RGB(255,255,255);
	m_TabResource.m_crFrameShadow=RGB(0,0,0);
	m_TabResource.m_crFrameBKColor=RGB(125,125,125);
	m_TabResource.m_crNormalTXColor=RGB(0,0,0);
	//m_TabResource.m_crSelectTXColor=RGB(255,0,0);//��ɫ
	m_TabResource.m_crSelectTXColor=RGB(186,218,254);//����ɫ
	//m_TabResource.m_ActivePic.SetLoadInfo(hDllHinstance,IDB_TAB_ACTIVE,m_bAutoLock);//��ѡ�����
	//m_TabResource.m_NormalPic.SetLoadInfo(hDllHinstance,IDB_TAB_NORMAL,m_bAutoLock);//��ѡ�ͻ��
	//m_TabResource.m_BackPic.SetLoadInfo(hDllHinstance,IDB_TAB_BACK,m_bAutoLock);///���²�ı���ͼƬ
	//m_TabResource.m_ActivePic.SetLoadInfo("tab_sel.bmp",m_bAutoLock);//��ѡ�����
	//m_TabResource.m_NormalPic.SetLoadInfo("tab_normal.bmp",m_bAutoLock);//��ѡ�ͻ��
	//m_TabResource.m_BackPic.SetLoadInfo("tab_bottom.bmp",m_bAutoLock);///�����ı���ͼƬ

	////�������
	//m_TabResource.m_BackBottom.SetLoadInfo(hDllHinstance,IDB_ROOM_BOTTOM,m_bAutoLock);//����ܵײ��м�ƽ��ͼƬ
	//m_TabResource.m_BackBottomLeft.SetLoadInfo(hDllHinstance,IDB_ROOM_BOTTOM_LEFT,m_bAutoLock);//////////����ܵײ����ͼƬ
	//m_TabResource.m_BackBottomRight.SetLoadInfo(hDllHinstance,IDB_ROOM_BOTTOM_RIGHT,m_bAutoLock);/////����ܵײ��Ҳ�ͼƬ
	//m_TabResource.m_BackBottom.SetLoadInfo("tab_bottom.bmp",m_bAutoLock);//����ܵײ��м�ƽ��ͼƬ
	//m_TabResource.m_BackBottomLeft.SetLoadInfo("bottom1.jpg",m_bAutoLock);//////////����ܵײ����ͼƬ
	//m_TabResource.m_BackBottomRight.SetLoadInfo("bottom3.jpg",m_bAutoLock);/////����ܵײ��Ҳ�ͼƬ
//	m_TabResource.m_BackPicNew.SetLoadInfo(hDllHinstance,IDB_TAB_BACK_NEW,m_bAutoLock);

	//��ʼ�� TAB �ؼ���Դ
	m_TitleResource.m_crTextColor=RGB(255,255,255);
//	m_TitleResource.m_BackPic.SetLoadInfo(hDllHinstance,IDB_TITLE_NOR,m_bAutoLock);

	//��ʼ����ť��Դ
	m_ButtonResource.m_bSound=true;
	m_ButtonResource.m_hMouneCursor=NULL;

	m_ButtonResource.m_ActiveColor=RGB(255,120,80);		
	m_ButtonResource.m_ActiveTextColor=RGB(0,0,255);	
	m_ButtonResource.m_NormalTextColor=RGB(0,0,0);		
	m_ButtonResource.m_SelectTextColor=RGB(0,0,255);	
	m_ButtonResource.m_FrameColor=RGB(0,64,128);		
	m_ButtonResource.m_FrameHeight=RGB(230,230,230);	
	m_ButtonResource.m_FrameShadow=RGB(128,128,128);	
	m_ButtonResource.m_NormalColorTop=RGB(255,255,255);
	m_ButtonResource.m_NormalColorBottom=RGB(0,128,255);
//	m_ButtonResource.m_BackPic.SetLoadInfo(hDllHinstance,IDB_BUTTON_BACK,m_bAutoLock);

	//��ʼ���б���Դ
//	m_ListResource.m_crBkColor=RGB(107,142,198);
	//��������
	m_ListResource.m_crBkColor=RGB(233,233,233);
	m_ListResource.m_crFousBkColor=RGB(125,125,125);
	m_ListResource.m_crFousTxColor=RGB(0,0,255);
	m_ListResource.m_crEmemyTxColor=RGB(0,0,255);
	m_ListResource.m_crNormalTxColor=RGB(0,0,0);
	m_ListResource.m_crNorMemTxColor=RGB(0,0,255);
	m_ListResource.m_crHighMemTxColor=RGB(0,0,255);
	m_ListResource.m_crFriendTxColor=RGB(0,0,255);
	m_ListResource.m_crMasterTxColor=RGB(0,0,255);
	m_ListResource.m_crMeTxColor=RGB(0,0,255);
	m_ListResource.m_crListColor=RGB(255,255,255);
//	m_ListResource.m_TitlePic.SetLoadInfo(hDllHinstance,IDB_LIST_TITLE,m_bAutoLock);

	//��ʼ���Ի�����Դ
	m_DialogResource.m_rcTopFrameColor=RGB(0,80,255);	
	m_DialogResource.m_rcButFrameColor=RGB(0,52,214);	
	m_DialogResource.m_rcTitleTextColor=RGB(255,255,255);
	m_DialogResource.m_crTextColor=RGB(0,0,0);
	m_DialogResource.m_crBackColor=RGB(240,240,240);
	m_DialogResource.m_BackBrush.CreateSolidBrush(RGB(240,240,240));		
//	m_DialogResource.m_Title.SetLoadInfo(hDllHinstance,IDB_TITLE_NOR,m_bAutoLock);
//	m_DialogResource.m_CloseNor.SetLoadInfo(hDllHinstance,IDB_CLOSE_NOR,m_bAutoLock);
//	m_DialogResource.m_CloseFoc.SetLoadInfo(hDllHinstance,IDB_CLOSE_FOC,m_bAutoLock);
//	m_DialogResource.m_CloseDis.SetLoadInfo(hDllHinstance,IDB_CLOSE_DIS,m_bAutoLock);
//	m_DialogResource.m_MaxNor.SetLoadInfo(hDllHinstance,IDB_MAX_NOR,m_bAutoLock);
//	m_DialogResource.m_MaxFoc.SetLoadInfo(hDllHinstance,IDB_MAX_FOC,m_bAutoLock);
//	m_DialogResource.m_MaxDis.SetLoadInfo(hDllHinstance,IDB_MAX_DIS,m_bAutoLock);
//	m_DialogResource.m_ResNor.SetLoadInfo(hDllHinstance,IDB_RES_NOR,m_bAutoLock);
//	m_DialogResource.m_ResFoc.SetLoadInfo(hDllHinstance,IDB_RES_FOC,m_bAutoLock);
//	m_DialogResource.m_ResDis.SetLoadInfo(hDllHinstance,IDB_RES_DIS,m_bAutoLock);
//	m_DialogResource.m_MinNor.SetLoadInfo(hDllHinstance,IDB_MIN_NOR,m_bAutoLock);
//	m_DialogResource.m_MinFoc.SetLoadInfo(hDllHinstance,IDB_MIN_FOC,m_bAutoLock);
//	m_DialogResource.m_MinDis.SetLoadInfo(hDllHinstance,IDB_MIN_DIS,m_bAutoLock);
//	m_DialogResource.m_BackPic.SetLoadInfo(hDllHinstance,IDB_DIALOG_BACK,m_bAutoLock);

	//��ʼ���༭����Դ
	m_EditResource.m_crFocusTXRGB=RGB(200,0,0);
	m_EditResource.m_crNoFocusTXRGB=RGB(0,0,0);
	m_EditResource.m_crDisFocusTXRGB=RGB(255,255,255);
	m_EditResource.m_crFocusBKRGB=RGB(255,255,0);
	m_EditResource.m_crNoFocusBKRGB=RGB(255,255,255);
	m_EditResource.m_crDisFocusBKRGB=RGB(125,125,125);
	m_EditResource.m_brFouns.CreateSolidBrush(RGB(255,255,0));
	m_EditResource.m_brNoFocus.CreateSolidBrush(RGB(255,255,255));
	m_EditResource.m_brDiable.CreateSolidBrush(RGB(125,125,125));

	//��ʼ���������Դ
	m_SplitResource.m_hHorCur=LoadCursor(hDllHinstance,MAKEINTRESOURCE(IDC_HCUR));
	m_SplitResource.m_hVorCur=LoadCursor(hDllHinstance,MAKEINTRESOURCE(IDC_VCUR));
	m_SplitResource.m_hDisableCur=LoadCursor(hDllHinstance,MAKEINTRESOURCE(IDC_DIS));

	//��ʼ����ɫ��Դ
	m_ColorResource.m_crListTxColor=RGB(0,0,0);		
	m_ColorResource.m_crListBkColor=RGB(240,240,250);//�б�����ɫ����ǰ��RGB(242,235,219);���ڸĳ���ɫ
	m_ColorResource.m_crDeskTxColor=RGB(240,240,240);		
	m_ColorResource.m_crSysHeadTxColor=RGB(255,0,0);
	m_ColorResource.m_crSystemTxColor=RGB(255,0,0);
	m_ColorResource.m_crNewsTxColor=RGB(255,0,0);
	m_ColorResource.m_crNormalTxColor=RGB(0,0,200);
//	m_ColorResource.m_crTalkNameColor=RGB(255,255,255);
	m_ColorResource.m_crTalkNameColor=RGB(0,0,0);
	m_ColorResource.m_crTalkTxColor=RGB(0,0,0);
//	m_ColorResource.m_crTalkBkColor=RGB(107,142,198);
	m_ColorResource.m_crTalkBkColor=RGB(233,233,233);

	return true;
}
/*******************************************************************************************************/