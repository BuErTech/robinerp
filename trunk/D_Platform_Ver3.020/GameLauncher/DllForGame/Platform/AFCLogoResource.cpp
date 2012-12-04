#include "../stdafx.h"
#include "AFCFunction.h"
#include "AFCLogoResource.h"
//#include "..\\��Դ.2003\\Resource.h"

BEGIN_MESSAGE_MAP(CGameClientTranStatic, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

//using namespace AFC;

//��̬����
CImage				CGameLogoRes::m_MatchPic;								//����ͷ��
CImage				CGameLogoRes::m_LostPic;								//����ͷ��
CImage				CGameLogoRes::m_LogoPicArray[LOGO_COUNT];				//ͷ��ͼƬ����
CImage				CGameLogoRes::SitBoyArray[ZZ_COUNT];				//��������ͼƬ����
CImage				CGameLogoRes::SitGirlArray[ZZ_COUNT];				//Ů������ͼƬ����
COLORREF			CGameLogoRes::m_crBackColor=RGB(0,0,0);					//͸����ɫ

/*******************************************************************************************************/

//���캯��
CGameLogoRes::CGameLogoRes(void)
{
	Init();
}

//��������
CGameLogoRes::~CGameLogoRes(void)
{
}

//��ȡ����ͼƬ
CImage * CGameLogoRes::GetMatchImage()
{
	return &m_MatchPic;
}

//��ʼ������
bool CGameLogoRes::Init()
{
	//HMODULE hResDllHandle=AfxLoadLibrary(TEXT("BZW_Res.dll"));
	//if (hResDllHandle!=NULL)
	//{
	//	AfxSetResourceHandle(hResDllHandle);
		for (int i=0;i<LOGO_COUNT;i++)	
		{
			TCHAR chLogName[150];
			wsprintf(chLogName,"..\\image\\log\\gamelog%d.png",i);
			m_LogoPicArray[i].Load(chLogName);
		}

		m_LostPic.LoadFromResource(NULL,IDB_LOST_LOGO);
		m_MatchPic.LoadFromResource(NULL,IDB_MATCH_LOGO);
		m_crBackColor=m_LogoPicArray[1].GetPixel(0,0);
		//AfxSetResourceHandle(GetModuleHandle(NULL));
		//FreeLibrary(hResDllHandle);
		return true;
	/*}
	return false;*/

}

//��ȡ LOGO ͼƬ
CImage * CGameLogoRes::GetLogoImage(UINT uIndex, bool bLost)
{
	if (bLost==false) return &m_LogoPicArray[uIndex%LOGO_COUNT];
	return &m_LostPic;
}


//��ȡ���� ͼƬ�����ϵ���Ů��
CImage * CGameLogoRes::GetSitGirl(UINT uIndex, bool bLost)
{
	if (bLost==false) return &SitGirlArray[uIndex%LOGO_COUNT];
	return &m_LostPic;
}
//��ȡ���� ͼƬ�����ϵ����е�
CImage * CGameLogoRes::GetSitBoy(UINT uIndex, bool bLost)
{
	if (bLost==false) return &SitBoyArray[uIndex%LOGO_COUNT];
	return &m_LostPic;
}

/*******************************************************************************************************/

//���캯��
CGameClientTranStatic::CGameClientTranStatic(void)
{
	m_pLogoImage=NULL;
}

//��������
CGameClientTranStatic::~CGameClientTranStatic(void)
{
}

//����ͼ��
bool CGameClientTranStatic::SetLogoImage(CImage * pLogoImage)
{
	m_pLogoImage=pLogoImage;
	Invalidate(FALSE);
	return true;
}

//�ػ�����
void CGameClientTranStatic::OnPaint()
{
	CPaintDC dc(this);

	//���õ�ͼ
	CRect ClientRect;
	GetClientRect(&ClientRect);
	dc.FillSolidRect(&ClientRect,RGB(250,250,250));
	if ((m_pLogoImage!=NULL)&&(m_pLogoImage->IsNull()==false)) 
		AFCStretchImage(&dc,0,0,LOGO_WIDTH,LOGO_HEIGHT,*m_pLogoImage,0,0,LOGO_WIDTH,LOGO_HEIGHT,CGameLogoRes::GetBackColor());
	
	return;
}

//��굥��
void CGameClientTranStatic::OnLButtonDown(UINT nFlags, CPoint point)
{
	GetParent()->PostMessage(WM_HIT_LOGO,0,0);
	__super::OnLButtonDown(nFlags, point);
}

//���˫����
void CGameClientTranStatic::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	GetParent()->PostMessage(WM_HIT_LOGO,0,0);
	__super::OnLButtonDblClk(nFlags, point);
}

/*******************************************************************************************************/

