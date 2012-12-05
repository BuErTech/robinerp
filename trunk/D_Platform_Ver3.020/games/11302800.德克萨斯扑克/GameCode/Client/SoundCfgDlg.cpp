#include "stdafx.h"

#include "SoundCfgDlg.h"
#include "MiniGameEngine.h"

extern TCHAR *GlbGetFloatString(TCHAR pRet[], int nValue); 

#define IDT_NOTICES		900

#define IDC_RDO_FIFANG_BASE		201
#define IDC_CHK_SOUND_BASE		211


CSoundCfgDlg::CSoundCfgDlg(DWORD pId,CWnd* pParent)
	: BModalContainer(pId, pParent),m_iniFile(NULL)
{

	//��ȡ������ָ��
	m_pWndParent = pParent;

	//�������ֿ���ѡ��ؼ�
	m_pChkBgMusic = NULL;

	//��Ϸ��Ч����ѡ��ؼ�
	m_pChkGameSound = NULL;

	//��׼����ѡ��ؼ�
	m_pRdoStander = NULL;

	//�ط�����ѡ��ؼ�
	m_pRdoDiFang = NULL;
	
}

CSoundCfgDlg::~CSoundCfgDlg()
{

}

/**
 * ����ǰ�������еĿؼ���Ϣ
 *
 * @param pId Ҫ����Ŀؼ�
 * @param pParam Ҫ������¼���Ϣ
 */
bool CSoundCfgDlg::OnPostMessage(UINT pId, BBaseParameter* pParam)
{
	switch(pId)
	{
	case EVENT_CONTINUE_MAINFRAME: //ȷ����Ϣ��Ӧ
		{
			OnClickedOK(pParam);
		}
		break;
	case EVENT_CLOSE_MAINFRAME:	//ȡ����Ϣ��Ӧ
		{

		}
		break;
	}

	//�ط�����ѡ��
	if (pId >= IDC_RDO_FIFANG_BASE && pId <= IDC_RDO_FIFANG_BASE + 1)
	{
		OnSelectDiFang(pId);
	}


	return __super::OnPostMessage(pId,pParam);
}

/// ���ضԻ���
bool CSoundCfgDlg::LoadConfig(std::string path)
{
	if (__super::LoadConfig(path))
	{
		m_pChkBgMusic	= dynamic_cast<BzRadioButton*>(GetControlById(IDC_CHK_SOUND_BASE));
		m_pChkGameSound = dynamic_cast<BzRadioButton*>(GetControlById(IDC_CHK_SOUND_BASE + 1));

		m_pRdoStander = dynamic_cast<BzRadioButton*>(GetControlById(IDC_RDO_FIFANG_BASE));
		m_pRdoDiFang  = dynamic_cast<BzRadioButton*>(GetControlById(IDC_RDO_FIFANG_BASE + 1));


		CString strFileName;
		strFileName.Format("%sbzGame.ini",CINIFile::GetAppPath());
		m_iniFile.SetINIFileName(strFileName);

		int nChkBgMusic		= m_iniFile.GetKeyVal(_TEXT("SoundCfg"),_TEXT("BK_Music"),1);
		int nChkGameSound	= m_iniFile.GetKeyVal(_TEXT("SoundCfg"),_TEXT("Game_Sound"),1);

		if (m_pChkBgMusic)
		{
			m_pChkBgMusic->SetChecked(nChkBgMusic == 1);
		}

		if (m_pChkGameSound)
		{
			m_pChkGameSound->SetChecked(nChkGameSound == 1);

		}

		int nSelDiFang = m_iniFile.GetKeyVal(_TEXT("SoundCfg"),_TEXT("DiFang_Select"),0);

		if(m_pRdoStander)
		{
			m_pRdoStander->SetChecked(nSelDiFang == 0);
		}

		if (m_pRdoDiFang)
		{
			m_pRdoDiFang->SetChecked(nSelDiFang == 1);
		}

		return true;
	}

	return false;
}

/// �ط����Ե�ѡ���¼���Ӧ
void CSoundCfgDlg::OnSelectDiFang(UINT uiID)
{
	if (m_pRdoStander)
	{
		m_pRdoStander->SetChecked(uiID == IDC_RDO_FIFANG_BASE);
	}

	if (m_pRdoDiFang)
	{
		m_pRdoDiFang->SetChecked(uiID == IDC_RDO_FIFANG_BASE + 1);
	}

	return;
}

/// �Ի���ȷ��
void CSoundCfgDlg::OnClickedOK(BBaseParameter* pBBPOK)
{
	CMiniGameEngine* pGameDlg = (CMiniGameEngine*)m_pWndParent;

	CString strValue = "0";
	BYTE  byCfgType = 0;
	DWORD dwValue = 0;

	//���沢��Ч��Ϸ��������

	if (m_pChkBgMusic)
	{
		if (m_pChkBgMusic->isChecked())
		{
			strValue = "1";
			dwValue = dwValue | 0x0001L;
		}
		else
		{
			strValue = "0";
		}

		m_iniFile.SetKeyValString(_TEXT("SoundCfg"),_TEXT("BK_Music"),strValue);
	}

	if (m_pChkGameSound)
	{
		if (m_pChkGameSound->isChecked())
		{
			strValue = "1";
			dwValue = dwValue | 0x0002L;
		}
		else
		{
			strValue = "0";
		}
		m_iniFile.SetKeyValString(_TEXT("SoundCfg"),_TEXT("Game_Sound"),strValue);
	}

	byCfgType = 0x01;
	pBBPOK->SetValue(IDB_SOUND_CFG_OK,(BYTE)byCfgType);
	pBBPOK->SetValue(IDB_SOUND_CFG_OK,(DWORD)dwValue);
	pGameDlg->OnPostMessage(IDB_SOUND_CFG_OK,pBBPOK);

	byCfgType = 0;
	dwValue = 0;

	////���沢��Ч�ط���������

	//if(m_pRdoStander)
	//{
	//	if (m_pRdoStander->isChecked())
	//	{
	//		strValue = "0";
	//		dwValue = 0;
	//	}
	//}

	//if(m_pRdoDiFang)
	//{
	//	if (m_pRdoDiFang->isChecked())
	//	{
	//		strValue = "1";
	//		dwValue = 1;
	//	}
	//}

	//m_iniFile.SetKeyValString(_TEXT("SoundCfg"),_TEXT("DiFang_Select"),strValue);

	//byCfgType = 0x02;
	//pBBPOK->SetValue(IDB_SOUND_CFG_OK,(BYTE)byCfgType);
	//pBBPOK->SetValue(IDB_SOUND_CFG_OK,(DWORD)dwValue);
	//pGameDlg->OnPostMessage(IDB_SOUND_CFG_OK,pBBPOK);


	return;
}