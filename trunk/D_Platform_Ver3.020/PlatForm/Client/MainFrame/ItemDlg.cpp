// ItemDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Resource.h"
#include "ItemDlg.h"

static char g_chClauseInfo[] = 
"\r\n			������ϵ�������Ʒ�û����Э��\r\n \
			 ZHENGYILONG SOFTWARE LICENSE\r\n \
\r\n\
\r\n\
��Ҫ��֪�����������Ķ���\r\n\
\r\n\
        �����ز�ʹ��������ϵ�������Ʒ����Ϊͬ�Ȿ���Э�����ݡ�\r\n\
��	  \r\n\
        ����������ϵ�������Ʒ�����û����Э�顷�����¼�ơ�Э�顷���������������������Ƽ����޹�˾���³�������������֮��������ء���װ��ʹ�á�����������ϵ�������Ʒ�����¼�ơ���������ķ���Э�顣����Э�顷������������������֮����ڡ���������ʹ�ü���ط����Ȩ�����񡣡��û�����������ָͨ�������������ṩ�Ļ�ȡ�����Ȩ��;������������Ȩ��ɺ������Ʒ�ĸ��˻�λ��������ϸ�Ķ�����Э�顷�еĸ����������������������������������ε�����������û���Ȩ�����ơ��������Ĳ����ܻ򲻽��ܱ���Э�顷��δ������Ӧ�ڷ����໤����ͬ�����ģ���\r\n\
        ���������ܱ���Э�顷�����������Ȩ���ء���װ��ʹ�ñ��������������ط������İ�װʹ����Ϊ����Ϊ�Ա���Э�顷�Ľ��ܣ���ͬ����ܱ���Э�顷���������Լ����\r\n\
        δ��������������Ȩ���κ����ۡ�ת�á����⡢�޸ı������������Ϊ������Ϊ����Ȩ��Ϊ��\r\n\
		\r\n\
1     ֪ʶ��Ȩ����\r\n\
1.1 ������������������������������������������һ�а�Ȩ��֪ʶ��Ȩ���Լ��롰�������ص�������Ϣ���ݣ������������ڣ����ֱ���������ϡ��̱ꡢͼ��ͼ�Ρ�������ơ������ܡ��й����ݡ�ӡˢ���ϡ������ĵ��Ⱦ�������Ȩ���͹�������Ȩ��Լ�Լ�����֪ʶ��Ȩ���ɷ���ı����������κ�������������Ȩ���Э�����ṩ����ȷ��������⣬ӵ�б�������������������κ��й���Щר�����̱ꡢ��Ȩ������֪ʶ��Ȩ����ɡ�\r\n\
\r\n\
2     ���������Ȩ��Χ\r\n\
2.1   �û����԰�װ��ʹ�á���ʾ�����б����������\r\n\
2.2   ����Ȩ����δ��ʾ��Ȩ������һ��Ȩ���Թ��������������У��û�ʹ������Ȩ��ʱ������ȡ������������������ͬ�⡣\r\n\
2.3   ������Э�顷����ȷ�涨�⣬����Э�顷��δ�Ա���������������������������������ķ������������涨��������Щ���񣬿����е����ķ���������Թ淶�����û���ʹ���йط���ʱ�����˽���ȷ�ϡ����û�ʹ�ø÷�����Ϊ����ط�������Ľ��ܡ�\r\n\
\r\n\
3     �û�ʹ����֪\r\n\
3.1   �û�Ӧ��֤ע��Ǽ����ʱ�ṩ��������ʵ����\r\n\
3.2   �û������ط��ɼ���Э���ǰ���¿����ݱ���Э�顷ʹ�ñ�����������û���Ȩʵʩ������������������Ϊ��\r\n\
3.2.1 ����ɾ��������������������������й��ڰ�Ȩ����Ϣ�����ݣ�\r\n\
3.2.2 ���öԱ�����������з��򹤳̡������ࡢ�������ȣ�\r\n\
3.2.3 ���ڱ����������ͼ�����ֵ������Ϣ��δ����������������ͬ�⣬�û���������ʵʩ������������������Ϊ��ʹ�á����ơ��޸ġ����ӡ�ת�ء���ࡢ�������棬��������վ�㡢���Խ������������չ��֮�йص�������Ʒ����Ʒ������ȡ�\r\n\
3.2.4 �û��������ñ����������������������Υ�����ҷ��ɡ�Σ�����Ұ�ȫ�����ͳһ������ȶ������ݣ������衢�̰���ɫ�顢�������κ�Υ�����ҷ��ɷ������ߵ����ݡ�\r\n\
3.2.5 �û��������ñ�����������������������ֺ�����֪ʶ��Ȩ����ҵ����Ȩ�ȺϷ�Ȩ�������ݡ�\r\n\
3.3   ʹ�ñ���������������ع����йط��ɺ����ߣ�ά���������棬�������Ұ�ȫ�������û�Υ����Υ����Э���ʹ�ö������һ���������û���ȫ�����Σ�һ���������������޹أ�����������������ʧ�ģ�������������ȨҪ���û��⳥������Ȩ����ֹͣ�ṩ���񲢱�����ؼ�¼�������˾������׷���������ε�Ȩ����\r\n\
3.4   ���������ͬ���������ϵͳһ���������ܵ����ְ�ȫ��������š��磺���ذ�װ��������к��С�������ľ���Ȳ�������в���û��������Ϣ�����ݵİ�ȫ���̶�Ӱ�챾�������������ʹ�õȡ��û�Ӧ��ǿ��Ϣ��ȫ��ʶ��ע���ǿ���뱣������������ʧ��\r\n\
3.5   �û��������ء���װ��ʹ�÷Ǿ�������������������ʽ�����������κ��ɱ����������������������ܵ��²���Ԥ֪�ķ��գ������û���Ҫ�������ء���װ��ʹ�ã��ɴ˲�����һ�з������������һ���������������޹ء�\r\n\
3.6   �����������������κ�ʱ��Ϊ���ṩ������������޸ġ������汾��Ȩ�����Լ�Ϊ�޸Ļ�����������ȡ���õ�Ȩ����\r\n\
\r\n\
4     ��������������\r\n\
4.1   ����������Ϊ�˱��ϲ�Ʒ�ļ������ȣ���Ա���������Ż������������º�Ĳ�Ʒ�û����������ز����ʹ�á�����������������Ѳ�Ʒ�������Կͻ����м���֧�ֺ�ά�����û������ڹ�����̳���ύ�����Ϣ�������������Բ�Ʒ���º������ķ�ʽ����ͳһ�����\r\n\
4.2   �û�Υ����Э�����صķ�������Ĺ涨��������������Ȩ���û�����Ϊ���ʲ�ȡ�������������ж�ʹ����ɡ�ֹͣ�ṩ��������ʹ�á�����׷���ȴ�ʩ��ͬʱ����������������˾�����ŵ�Ҫ��Э�����顣\r\n\
4.3   ʹ�ñ���������漰���������������أ����ܻ��ܵ��������ڲ��ȶ����ص�Ӱ�죬�����򲻿ɿ�����������������ڿ͹�����ϵͳ���ȶ����û�����λ�á��û��ػ��Լ������κ����硢������ͨ����·��ԭ����ɵķ����жϻ��������û�Ҫ��ķ��գ��û������ײ����ге����Ϸ��ա������÷�����������Χ�ڣ��������������κ�����²�����ʹ�û���ʹ�ñ����������Ʒ��������������ġ�����ġ���ֱ�ӻ��ӵ���ʧ�е��⳥���Σ���ʹ�û������ȱ�������������֪���𺦷����Ŀ����ԡ�\r\n\
4.4   �û������������Ų��ŵ�ͨѶ��·���ϡ��������⡢���硢���Թ��ϡ�ϵͳ���ȶ��Լ��������ֲ��ɿ���ԭ������ܵľ�����ʧ���������������е����Ρ�\r\n\
4.5   �������ϵȲ��ɿ��¼�Ӱ�쵽������������еģ�������������ŵ�ڵ�һʱ��������ص�λ��ϼ�ʱ��������޸������û���˶����ܵľ�����ʧ���������������е����Ρ�\r\n\
\r\n\
5     ��������\r\n\
5.1   ��Э�����涨���κ�����Ĳ��ֻ�ȫ����Ч�ߣ���Ӱ�����������Ч����\r\n\
5.2   ��Э��Ľ��͡�Ч�������׵Ľ�����������л����񹲺͹����ɡ����û�������������֮�䷢���κξ��׻����飬����Ӧ�Ѻ�Э�̽����Э�̲��ɵģ��û��ڴ���ȫͬ�⽫���׻������ύ�������������ڵط�Ժ��Ͻ��\r\n\
\r\n\
���������������Ա�Э��Ľ���Ȩ��\r\n\
		  \r\n\
						    �������������Ƽ����޹�˾\r\n\
								2011/1/20 ";

// CItemDlg �Ի���

IMPLEMENT_DYNAMIC(CItemDlg, CDialog)

//CItemDlg::CItemDlg(CWnd* pParent /*=NULL*/)
//	: CDialog(CItemDlg::IDD_DIG_ITEM, pParent)
CItemDlg::CItemDlg() : CDialog(IDD_DIG_ITEM)
{

}

CItemDlg::~CItemDlg()
{
}

void CItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCLOSE,m_Bt1);	
	DDX_Control(pDX, IDOK,m_Bt2);	
}


BEGIN_MESSAGE_MAP(CItemDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDCLOSE,OnClickedClose)
	ON_MESSAGE(WM_EXCHANGE_SKIN,OnExchangeSkin)
END_MESSAGE_MAP()


// CItemDlg ��Ϣ�������
BOOL CItemDlg::OnInitDialog()
{
	__super::OnInitDialog();

	CString s=CBcfFile::GetAppPath ();/////����·��
	m_strText = g_chClauseInfo;

	//CBcfFile bcf( s + "bzgame.bcf");
	//m_strText = bcf.GetKeyVal("BZW", "ItemText", "aa");

	CString strSkin = m_skinmgr.GetSkinBcfFileName();
	CBcfFile f( s + strSkin);
	TCHAR path[MAX_PATH];
	CString skinfolder;
	if ("skin0.bcf" == strSkin)
		skinfolder=f.GetKeyVal("skin0","skinfolder",m_skinmgr.GetSkinPath());
	else
		skinfolder=f.GetKeyVal("skin1","skinfolder",m_skinmgr.GetSkinPath());

	// 2012.12.17 yyf ����
	//wsprintf(path,"%sdialog\\item.bmp",skinfolder);
	//m_bk.SetLoadInfo(path,TRUE);
	////m_bk.SetLoadInfo(AfxGetInstanceHandle(),IDB_BITMAP9,false);
	//HRGN hRgn2;
	//MoveWindow(0,0,m_bk.GetWidth(),m_bk.GetHeight(),0);
	//CGameImageHelper help(&m_bk);
	//HRGN hRgn;
	//hRgn = AFCBmpToRgn(help,m_bk.GetPixel(0,0),RGB(1,1,1));
	//if(hRgn)
	//	SetWindowRgn(hRgn,TRUE);

	
	//*-------��ʼ�� m_configDlg--------- 2012.10.15 yyf
	TCHAR pathConfig[MAX_PATH];
	TCHAR PathDefault[MAX_PATH];
	
	wsprintf(pathConfig,"%s%sdialog\\configDlg\\CItemDlgConfig\\configDlg.bcf",s,skinfolder);// 
	wsprintf(PathDefault,"%s%s",s,skinfolder);
	m_configDlg.LoadConfig(pathConfig,PathDefault);	

	int cx,cy; 
	cx=m_configDlg.GetDlgWide();
	cy=m_configDlg.GetDlgHigh(); 
	MoveWindow(0,0,cx,cy,0);
	//-----------------------------------------------------*/
	CenterWindow();

	HRGN hRgn2;
	CGameImage					m_bt;
	wsprintf(path,"%slogon\\log_exit_bt.bmp",skinfolder);
	m_bt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	m_Bt1.LoadButtonBitmap(path,false);
	hRgn2=AFCBmpToRgn(m_bt,RGB(255,0,255),RGB(1,1,1));
	if(hRgn2)
		m_Bt1.SetWindowRgn(hRgn2,true);
	//m_Bt1.MoveWindow(m_bk.GetWidth()-m_bt.GetWidth()/4-5,0,m_bt.GetWidth()/4,m_bk.GetHeight(),0);
	m_Bt1.MoveWindow(cx-m_bt.GetWidth()/4-5,0,m_bt.GetWidth()/4,cy,0);//2012.10.17 yyf

	wsprintf(path,"%sdialog\\AboutOk.bmp",skinfolder);
	m_bt.SetLoadInfo(path,CGameImageLink::m_bAutoLock);
	m_Bt2.LoadButtonBitmap(path,false);
	hRgn2=AFCBmpToRgn(m_bt,RGB(255,0,255),RGB(1,1,1));
	if(hRgn2)
		m_Bt2.SetWindowRgn(hRgn2,true);

	
	SetWndRgn();// ���ô���Բ��2012.10.17 yyf
	return TRUE;  
}
void CItemDlg::OnPaint()
{
	CPaintDC dc(this); 

	/*// 2012.12.17 yyf ����
	CGameImageHelper	help(&m_bk);
	help.BitBlt(dc.GetSafeHdc(),0,0);*/
	
	//�滭��ܵ�ͼ 2012.10.17 yyf
	m_configDlg.DrawViewFrame(&dc);

	SetDlgItemText( IDC_EDITTEXT , m_strText ) ;
}

//CString  CItemDlg::ReadFile(CString filename)
//{
//	CFile file(filename,CFile::modeRead);
//	char *buf;
//	int len=file.GetLength();
//	buf=new char[len+1];
//	file.Read(buf,len);
//	buf[len]=0;
//	CString resStr=buf;
//	delete buf;
//	return resStr;
//}


void CItemDlg::OnClickedClose()
{
	CDialog::OnOK();
}

LRESULT CItemDlg::OnExchangeSkin(WPARAM wpara,LPARAM lpara)
{
	//OnPaint(); 
	Invalidate();
	return LRESULT(0);
}

// ���ô���Բ�� 2012.10.17 yyf
void CItemDlg::SetWndRgn(void)
{
	POINT m_arPoints[8];
	CRect winRect;
	GetWindowRect(&winRect);
	ScreenToClient(&winRect);
 
    //���Ͻ�
	m_arPoints[0].x = winRect.left;
	m_arPoints[0].y = winRect.top+3;
	m_arPoints[1].x = winRect.left+3;
	m_arPoints[1].y = winRect.top;

	//���Ͻ�
	m_arPoints[2].x = winRect.right-3;
	m_arPoints[2].y = winRect.top;
	m_arPoints[3].x = winRect.right;
	m_arPoints[3].y = winRect.top+3;

	//���½�
	m_arPoints[4].x = winRect.right;
	m_arPoints[4].y = winRect.bottom-4;
	m_arPoints[5].x = winRect.right-4;
	m_arPoints[5].y = winRect.bottom;

	//���½�
	m_arPoints[6].x = winRect.left+4;
	m_arPoints[6].y = winRect.bottom;
	m_arPoints[7].x = winRect.left;
	m_arPoints[7].y = winRect.bottom-4;

	HRGN hRgn = CreatePolygonRgn(m_arPoints, 8, WINDING);
	if (hRgn!=NULL)
	{
		SetWindowRgn(hRgn,TRUE);
		DeleteObject(hRgn);
	}
}