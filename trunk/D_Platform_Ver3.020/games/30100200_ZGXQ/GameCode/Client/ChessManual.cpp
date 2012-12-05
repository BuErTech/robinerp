#include "StdAfx.h"
#include ".\chessmanual.h"
#include "ClientGameDlg.h"

CChessManual::CChessManual():CGameFaceGo(CChessManual::IDD)
, m_strQiPu(_T(""))
{
	m_pParnet=NULL;
	m_iBuCount = 1;
	m_iFlag = 0;
	m_szString.Empty();
}

CChessManual::~CChessManual(void)
{
}

BOOL CChessManual::OnInitDialog()
{
	CGameFaceGo::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
BEGIN_MESSAGE_MAP(CChessManual, CGameFaceGo)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CChessManual::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CGameFaceGo::OnPaint()

	///m_strQiPu = m_szString;
	
	LPCTSTR pstr = m_szString.GetBuffer();
	if (pstr != NULL) pstr += TEXT('\0');
	m_szString.ReleaseBuffer();
	GetDlgItem(IDC_EDIT_QIPU)->SetWindowText(pstr);

}


void CChessManual::ClearAll()
{
	m_iBuCount = 1;
	m_iFlag = 0;
	m_szString.Empty();
}

void CChessManual::GetCharSet(int iOrder, CString &str)
{
	str.Empty();
	switch (iOrder)
	{
	case 1:
		str = TEXT("һ");
		break;

	case 2:
		str = TEXT("��");
		break;

	case 3:
		str = TEXT("��");
		break;

	case 4:
		str = TEXT("��");
		break;

	case 5:
		str = TEXT("��");
		break;

	case 6:
		str = TEXT("��");
		break;

	case 7:
		str = TEXT("��");
		break;

	case 8:
		str = TEXT("��");
		break;

	case 9:
		str = TEXT("��");
		break;
	}
}



void CChessManual::SetQiPu(int iStartI, int iStartJ, int iEndI, int iEndJ, int iQiZi, int type,int iMeFlag)
{
	CString str;
	CString s;
	m_iFlag++;
	if (m_iFlag % 3 == 0) 
	{
		m_iBuCount++;
		m_iFlag = 0;
		m_iFlag++;
	}
	
	if (m_iBuCount < 10) s.Format("  %d",m_iBuCount);
	else s.Format("%d",m_iBuCount);

	str = s;
	if (type == XQ_FLAG_RED)
		{	
			if (iMeFlag == XQ_FLAG_BLACK)
			{
				iStartI = 9 - iStartI;
				iEndI = 9 - iEndI;
				iStartJ = 9 - iStartJ -1 ;//+	1;
				iEndJ = 9 - iEndJ - 1;//+ 1;
			}
			str += TEXT("�죺");
			switch (iQiZi)
			{
			case XQ_RED_JU:
				str += TEXT("��");
				
				GetCharSet(9-iStartJ,s);
				str += s;
				if (iStartI == iEndI)
				{
					str += TEXT("ƽ");
					GetCharSet(9-iEndJ,s);
					str += s;
				}
				else
				{
					if (iStartI > iEndI)
					{
						str += TEXT("��");
						GetCharSet(iStartI-iEndI,s);
						str += s;
					}
					else
					{
						str += TEXT("��");
						GetCharSet(iEndI-iStartI,s);
						str += s;
					}
				}
				break;
				
			case XQ_RED_MA:
				str += TEXT("��");
			///	CString s;
				GetCharSet(9-iStartJ,s);
				str += s;
				if (iStartI > iEndI)
				{
					str += TEXT("��");
					GetCharSet(9-iEndJ,s);
					str += s;
				}
				else
				{
					str += TEXT("��");
					GetCharSet(9-iEndJ,s);
					str += s;
				}
				break;

			case XQ_RED_PAO:
				str += TEXT("��");
			///	CString s;
				GetCharSet(9-iStartJ,s);
				str += s;
				if (iStartI == iEndI)
				{
					str += TEXT("ƽ");
					GetCharSet(9-iEndJ,s);
					str += s;
				}
				else
				{
					if (iStartI > iEndI)
					{
						str += TEXT("��");
						GetCharSet(iStartI-iEndI,s);
						str += s;
					}
					else
					{
						str += TEXT("��");
						GetCharSet(iEndI-iStartI,s);
						str += s;
					}
				}
				break;

			case XQ_RED_BIN:
				str += TEXT("��");
		//		CString s;
				GetCharSet(9-iStartJ,s);
				str += s;
				if (iStartI > 4)//δ����
				{
					str += TEXT("��һ");
				}
				else //����
				{
					if (iStartI == iEndI)
					{
						str += TEXT("ƽ");
						GetCharSet(9-iEndJ,s);
						str += s;
					}
					else
					{
						str += TEXT("��һ");
					}
				}
				break;

			case XQ_RED_JIANG:
				str += TEXT("˧");
	//			CString s;
				GetCharSet(9-iStartJ,s);
				str += s;
				if (iStartI == iEndI)
				{
					str += TEXT("ƽ");
					GetCharSet(9-iEndJ,s);
					str += s;
				}
				else
				{
					if (iStartI > iEndI)
					{
						str += TEXT("��һ");
					}
					else
					{
						str += TEXT("��һ");
					}
				}
				break;

			case XQ_RED_SHI:
				str += TEXT("ʿ");
	//			CString s;
				GetCharSet(9-iStartJ,s);
				str += s;
				if (iStartI > iEndI)
				{
					str += TEXT("��");
					GetCharSet(9-iEndJ,s);
					str += s;
				}
				else
				{
					str += TEXT("��");
					GetCharSet(9-iEndJ,s);
					str += s;
				}
				break;

			case XQ_RED_XIANG:
				str += TEXT("��");
	//			CString s;
				GetCharSet(9-iStartJ,s);
				str += s;
				if (iStartI > iEndI)
				{
					str += TEXT("��");
					GetCharSet(9-iEndJ,s);
					str += s;
				}
				else
				{
					str += TEXT("��");
					GetCharSet(9-iEndJ,s);
					str += s;
				}
				break;
			}
		}

	if (type == XQ_FLAG_BLACK)
		{
			if (iMeFlag == XQ_FLAG_RED)
			{
				iStartI = 9 - iStartI;
				iEndI = 9 - iEndI;
				iStartJ = 9 - iStartJ - 1;//+ 1;
				iEndJ = 9 - iEndJ - 1;//+ 1;
			}
			str += TEXT("�ڣ�");
			switch (iQiZi)
			{
			case XQ_BLACK_JU:
				str += TEXT("��");
				//CString s;
				s.Format(" %d ",9-iStartJ);
				str += s;
				if (iStartI == iEndI)
				{
					str += TEXT("ƽ");
					s.Format(" %d ",9-iEndJ);
					str += s;
				}
				else
				{
					if (iStartI > iEndI)
					{
						str += TEXT("��");
						s.Format(" %d ",iStartI-iEndI);
						str += s;
					}
					else
					{
						str += TEXT("��");
						s.Format(" %d ",iEndI-iStartI);
						str += s;
					}
				}
				break;
					
			case XQ_BLACK_MA:
				str += TEXT("��");
	//			CString s;
				s.Format(" %d ",9-iStartJ);
				str += s;
				if (iStartI > iEndI)
				{
					str += TEXT("��");
					s.Format(" %d ",9-iEndJ);
					str += s;
				}
				else
				{
					str += TEXT("��");
					s.Format(" %d ",9-iEndJ);
					str += s;
				}
				break;

			case XQ_BLACK_PAO:
				str += TEXT("��");
	//			CString s;
				s.Format(" %d ",9-iStartJ);
				str += s;
				if (iStartI == iEndI)
				{
					str += TEXT("ƽ");
					s.Format(" %d ",9-iEndJ);
					str += s;
				}
				else
				{
					if (iStartI > iEndI)
					{
						str += TEXT("��");
						s.Format(" %d ",iStartI-iEndI);
						str += s;
					}
					else
					{
						str += TEXT("��");
						s.Format(" %d ",iEndI-iStartI);
						str += s;
					}
				}
				break;

			case XQ_BLACK_BIN:
				str += TEXT("��");
	//			CString s;
				s.Format(" %d ",9-iStartJ);
				str += s;
				if (iStartI > 4)//δ����
				{
					str += TEXT("�� 1");
				}
				else //����
				{
					if (iStartI == iEndI)
					{
						str += TEXT("ƽ");
						s.Format(" %d ",9-iEndJ);
						str += s;
					}
					else
					{
						str += TEXT("�� 1");
					}
				}
				break;

			case XQ_BLACK_JIANG:
				str += TEXT("��");
	//			CString s;
				s.Format(" %d ",9-iStartJ);
				str += s;
				if (iStartI == iEndI)
				{
					str += TEXT("ƽ");
					s.Format(" %d ",9-iEndJ);
					str += s;
				}
				else
				{
					if (iStartI > iEndI)
					{
						str += TEXT("�� 1");
					}
					else
					{
						str += TEXT("�� 1");
					}
				}
				break;

			case XQ_BLACK_SHI:
				str += TEXT("ʿ");
	//			CString s;
				s.Format(" %d ",9-iStartJ);//GetCharSet(9-iStartJ,s);
				str += s;
				if (iStartI > iEndI)
				{
					str += TEXT("��");
					s.Format(" %d ",9-iEndJ);
					str += s;
				}
				else
				{
					str += TEXT("��");
					s.Format(" %d ",9-iEndJ);
					str += s;
				}
				break;

			case XQ_BLACK_XIANG:
				str += TEXT("��");
	//			CString s;
				s.Format(" %d ",9-iStartJ);
				str += s;
				if (iStartI > iEndI)
				{
					str += TEXT("��");
					s.Format(" %d ",9-iEndJ);
					str += s;
				}
				else
				{
					str += TEXT("��");
					s.Format(" %d ",9-iEndJ);
					str += s;
				}
				break;
			}

		}

	str += TEXT("\r\n");
	m_szString += str;
	///m_strQiPu = m_szString;
	///UpdateData();
	return;
}


void CChessManual::DoDataExchange(CDataExchange* pDX)
{
	// TODO: �ڴ����ר�ô����/����û���

	CGameFaceGo::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_QIPU, m_strQiPu);
}
