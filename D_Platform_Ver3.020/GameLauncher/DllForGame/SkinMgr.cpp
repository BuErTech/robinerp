

#include "Stdafx.h"
#include "SkinMgr.h"
#include "LoveSendClassInExe.h"

//���õ�Ƥ��ѡ������
CString CSkinMgr::m_skinIndex;

//�û���
CString CSkinMgr::m_strUserName;

CSkinMgr::CSkinMgr()
{
}
CSkinMgr::~CSkinMgr()
{
}

bool CSkinMgr::IsUserCountFirstLogOn(const CString& strLogonUserID)
{
	bool ret = false;

	//2.�ȴ�bcf
	CString s=CBcfFile::GetAppPath();
	DWORD dwCfgFile = ::cfgOpenFile(s + _T("UserSelSkin.bcf"));

	//3.bcf�ļ���ʧ�ܷ���
	if(dwCfgFile < 0x10)
		return true;

	//4.��ȡbcf�ļ��е�ֵ
	m_skinIndex = ::cfgGetValue(dwCfgFile,strLogonUserID,_T("skin"),_T(""));
	if ("" == m_skinIndex)
		ret = true;

	//5.�ر�bcf�ļ�
	::cfgClose(dwCfgFile);

	return ret;
}

CString CSkinMgr::GetValOfSkinBcf(const CString& strLogonUserID)
{
	//1.��ȡ��½��
	CString key = _T("");
	if (0 != strLogonUserID.Compare(_T("")))
		key = strLogonUserID;
	else if (0 != m_strUserName.Compare(_T("")))
		key = m_strUserName;

	if (0 == key.Compare(_T("")))
	{
		m_skinIndex = _T("1");
		return m_skinIndex;
	}

	//2.�ȴ�bcf
	CString s=CBcfFile::GetAppPath();
	DWORD dwCfgFile = ::cfgOpenFile(s + _T("..\\UserSelSkin.bcf"));

	//3.bcf�ļ���ʧ�ܷ���
	if(dwCfgFile < 0x10)
	{
		m_skinIndex = _T("1");
		return m_skinIndex;
	}

	//4.��ȡbcf�ļ��е�ֵ
	m_skinIndex = ::cfgGetValue(dwCfgFile,key,_T("skin"),_T(""));

	//5.�ر�bcf�ļ�
	::cfgClose(dwCfgFile);

	return m_skinIndex;
}

//����ʱ������õ��û���
void CSkinMgr::SetLogOnUserName(const CString& strLogonName)
{
	m_strUserName = strLogonName;
}
//��ȡ��ǰ���õ�Ƥ��
CString CSkinMgr::GetUserSelSkinIndex() const
{
	return m_skinIndex;
}

//��ȡ�û�ѡ���Ƥ��·��
CString CSkinMgr::GetSkinPath(const CString& strLogonUserID)
{
	CString strUserID = GetSkinBcfFileName();
	if (_T("skin0.bcf") == strUserID)
	{
		return _T("image\\client\\skin0\\");
	}
	else if (_T("skin1.bcf") == strUserID)
	{
		return _T("image\\client\\skin1\\");
	}
	else
		return _T("image\\client\\skin0\\");
}
CString CSkinMgr::GetGameSkinPath()
{
	CString strUserID = GetSkinBcfFileName();
	if (_T("skin0.bcf") == strUserID)
	{
		return _T("image\\client\\skin0\\game\\");
	}
	else if (_T("skin1.bcf") == strUserID)
	{
		return _T("image\\client\\skin1\\game\\");
	}
	else
		return _T("image\\client\\skin0\\game\\");
}
//��ȡƤ��·���µĶ�Ӧ�ļ���
CString CSkinMgr::GetSkinBcfFileName(const CString& strLogonUserID)
{
	GetValOfSkinBcf(strLogonUserID);
	if (m_skinIndex == _T("1"))
		return _T("skin0.bcf");
	else if (m_skinIndex == _T("2"))
		return _T("skin1.bcf");
	else
		return _T("skin0.bcf");
}
//��ȡ��Ϸ����·����Ӧ���ļ���
CString CSkinMgr::GetGameSkinBcfFileName(const CString& strLogonUserID)
{
	GetValOfSkinBcf(strLogonUserID);
	if (m_skinIndex == _T("1"))
		return _T("gameskin0.bcf");
	else if (m_skinIndex == _T("2"))
		return _T("gameskin1.bcf");
	else
		return _T("gameskin0.bcf");
}
//��ȡkeyֵ
CString CSkinMgr::GetKeyVal(const CString& strSkinBcfFileName)
{
	if (_T("skin0.bcf") == strSkinBcfFileName)
		return _T("skin0");
	else if (_T("skin1.bcf") == strSkinBcfFileName)
		return _T("skin1");

	return "skin0.bcf";
}

//���õ�ǰ�û�ѡ���Ƥ��
void CSkinMgr::SetCurUserSelSkin(const CString& strindex)
{
	//1.����Ƥ��������bcf,�ȴ�bcf
	CString s=CBcfFile::GetAppPath();
	DWORD dwCfgFile = ::cfgOpenFile(s + _T("UserSelSkin.bcf"));

	//2.bcf�ļ���ʧ�ܷ���
	if(dwCfgFile < 0x10 || 0 == m_strUserName.Compare(_T(""))) 
		return;

	//3.����ֵ��bcf�ļ��е�ֵ��ȣ���������
	if (0 != strindex.Compare(_T("")) && strindex == ::cfgGetValue(dwCfgFile,m_strUserName,_T("skin"),_T(""))) 
		return;

	//4.�û���ѡƤ��ѡ��ֵ����
	if (0 == strindex.Compare(_T("")))
		m_skinIndex = _T("1");
	else
		m_skinIndex = strindex;

	::cfgSetValue(dwCfgFile,m_strUserName,_T("skin"),m_skinIndex);
	::cfgSetKeyMemo(dwCfgFile,m_strUserName,_T("skin"),_T("�û�ѡ���Ƥ��"));

	//5.�ر�bcf�ļ�
	::cfgClose(dwCfgFile);
}
//֪ͨ����ˢ��Ƥ��
void CSkinMgr::Notify()
{
	CString s = CBcfFile::GetAppPath ();/////����·��
	CString strSkin = GetSkinBcfFileName();
	CBcfFile f(s + "skin.bcf");
	TCHAR szUIPath[MAX_PATH];
	CString skinfolder;

	skinfolder = f.GetKeyVal(GetKeyVal(strSkin),"skinfolder","..\\" + GetSkinPath());
	wsprintf(szUIPath,"%s",skinfolder);

	BzDui::CPaintManagerUI::SetResourcePath(szUIPath);
	BzDui::CPaintManagerUI::ReloadSkin();
}