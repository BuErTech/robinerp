

#include "Stdafx.h"
#include "SkinMgr.h"
#include "MainRoomEx.h"
#include "FindUserDlg.h"
#include "GamePlaceDlg.h"
#include "..\\UILibrary2003\\AFCTabCtrl.h"

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
	DWORD dwCfgFile = ::cfgOpenFile(s + _T("UserSelSkin.bcf"));

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
//��ȡĬ��Ƥ��
CString CSkinMgr::GetDefaultSelSkin()
{
	//1.�ȴ�bcf
	CString s=CBcfFile::GetAppPath();
	DWORD dwCfgFile = ::cfgOpenFile(s + _T("UserSelSkin.bcf"));

	//2.bcf�ļ���ʧ�ܷ���
	if(dwCfgFile < 0x10)
	{
		m_skinIndex = _T("1");
		return m_skinIndex;
	}

	//3.��ȡbcf�ļ��е�ֵ
	m_skinIndex = ::cfgGetValue(dwCfgFile,_T("DefaultSkin"),_T("skin"),_T("1"));

	//4.�ر�bcf�ļ�
	::cfgClose(dwCfgFile);

	return m_skinIndex;
}

//����ʱ������õ��û���
void CSkinMgr::SetLogOnUserName(const CString& strLogonName)
{
	m_strUserName = strLogonName;
}

//��ȡ�û�ѡ���Ƥ��·��
CString CSkinMgr::GetSkinPath(const CString& strLogonUserID)
{
	CString strUserID = GetSkinBcfFileName(strLogonUserID);
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
//��ȡkeyֵ
CString CSkinMgr::GetKeyVal(const CString& strSkinBcfFileName)
{
	if (_T("skin0.bcf") == strSkinBcfFileName)
		return _T("skin0");
	else if (_T("skin1.bcf") == strSkinBcfFileName)
		return _T("skin1");
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
	int skinIndex = atoi(m_skinIndex);
	CString strSkinFileName = GetSkinBcfFileName();
	CMainRoomEx *pMainRoom = ::GetMainRoom();

	//1.������ 
	if (pMainRoom == NULL) return;
		pMainRoom->PostMessage(WM_EXCHANGE_SKIN,skinIndex,0);//��ҳ����

	//3.��Ϸ�б� 
	if (pMainRoom->m_pGameListWnd != NULL)
		pMainRoom->m_pGameListWnd->PostMessage(WM_EXCHANGE_SKIN,skinIndex,0);

	//4.�·����� 
	if (pMainRoom->m_pActWnd != NULL)
		pMainRoom->m_pActWnd->PostMessage(WM_EXCHANGE_SKIN,skinIndex,0);

	//5.�Ϸ����� 
	if (pMainRoom->m_TopWnd != NULL)
		pMainRoom->m_TopWnd->PostMessage(WM_EXCHANGE_SKIN,skinIndex,0);

	if (((CGamePlaceDlg *)AfxGetMainWnd()) != NULL)
	{
		((CGamePlaceDlg*)AfxGetMainWnd())->PostMessage(WM_EXCHANGE_SKIN,skinIndex,0);
		((CGamePlaceDlg*)AfxGetMainWnd())->m_topDuWnd.PostMessage(WM_EXCHANGE_SKIN,skinIndex,0);
		((CGamePlaceDlg*)AfxGetMainWnd())->m_RoomTabWnd.OnExchangeSkin(GetSkinBcfFileName(),GetKeyVal(strSkinFileName),GetSkinPath());
	}

	//9.
	if (pMainRoom->m_pAppealDlg != NULL)
		pMainRoom->m_pAppealDlg->PostMessage(WM_EXCHANGE_SKIN,skinIndex,0);

	//10.
	if (pMainRoom->hBulletin != NULL)
		pMainRoom->hBulletin->PostMessage(WM_EXCHANGE_SKIN,skinIndex,0);

	//11.
	if (pMainRoom->CharmPannel != NULL)
		pMainRoom->CharmPannel->PostMessage(WM_EXCHANGE_SKIN,skinIndex,0);

	//12.����GM�Ի��� 
	if (pMainRoom->m_pGMListDlg != NULL)
		pMainRoom->m_pGMListDlg->PostMessage(WM_EXCHANGE_SKIN,skinIndex,0);

	//13.ͷ���ϴ����� 
	if (pMainRoom->logoUpload != NULL)
		pMainRoom->logoUpload->PostMessage(WM_EXCHANGE_SKIN,skinIndex,0);

	//14.��Ϸ�б� 
	if (pMainRoom->m_pGameListWnd != NULL)
	{
		if (pMainRoom->m_pGameListWnd->m_GameList.IsWindowVisible())
			pMainRoom->m_pGameListWnd->m_GameList.PostMessage(WM_EXCHANGE_SKIN,skinIndex,0);
		if (pMainRoom->m_pGameListWnd->m_HelpList.IsWindowVisible())
			pMainRoom->m_pGameListWnd->m_HelpList.PostMessage(WM_EXCHANGE_SKIN,skinIndex,0);
	}
	pMainRoom->m_Connect.PostMessage(WM_EXCHANGE_SKIN,skinIndex,0);

	//15
	if (pMainRoom->m_pGameListWnd != NULL && pMainRoom->m_pGameListWnd->m_pIMWnd != NULL)
	{
		pMainRoom->m_pGameListWnd->m_pIMWnd->PostMessage(WM_EXCHANGE_SKIN,skinIndex,0);
		if (pMainRoom->m_pGameListWnd->m_pIMWnd->addFriendDlg.IsWindowVisible())
			pMainRoom->m_pGameListWnd->m_pIMWnd->addFriendDlg.PostMessage(WM_EXCHANGE_SKIN,skinIndex,0);
		if (pMainRoom->m_pGameListWnd->m_pIMWnd->setFriendDlg.IsWindowVisible())
			pMainRoom->m_pGameListWnd->m_pIMWnd->setFriendDlg.PostMessage(WM_EXCHANGE_SKIN,skinIndex,0);
	}
	if (pMainRoom->m_pGameListWnd != NULL && pMainRoom->m_pGameListWnd->m_IMList.IsWindowVisible())
		pMainRoom->m_pGameListWnd->m_IMList.PostMessage(WM_EXCHANGE_SKIN,skinIndex,0);

	//17.
	if (pMainRoom->pdDlg.IsWindowVisible())
		pMainRoom->pdDlg.PostMessage(WM_EXCHANGE_SKIN,skinIndex,0);
	
	//18.�˳��������
	if (pMainRoom->m_TopDuWnd != NULL && pMainRoom->m_TopDuWnd->m_quitAskdlg.IsWindowVisible())
		pMainRoom->m_TopDuWnd->m_quitAskdlg.PostMessage(WM_EXCHANGE_SKIN,skinIndex,0);

	//19.�˳��������
	if (pMainRoom->m_TopWnd != NULL && pMainRoom->m_TopWnd->m_quitAskdlg.IsWindowVisible())
		pMainRoom->m_TopWnd->m_quitAskdlg.PostMessage(WM_EXCHANGE_SKIN,skinIndex,0);

	//20.��Ϸ����
	for(int i = 0; i < MAX_GAME_ROOM; i ++)
	{
		if(pMainRoom->m_RoomInfo[i].bAccess)
		{
			// �ر���Ϸ����
			CGameRoomEx* pGameRoomEx = (CGameRoomEx*)pMainRoom->m_RoomInfo[i].pGameRoomWnd;
			if (NULL != pGameRoomEx)
			{
				pGameRoomEx->PostMessage(WM_EXCHANGE_SKIN,skinIndex,0);
				pGameRoomEx->m_vipDlg.PostMessage(WM_EXCHANGE_SKIN,skinIndex,0);
 				if (pGameRoomEx->boardCast != NULL)
 					pGameRoomEx->boardCast->PostMessage(WM_EXCHANGE_SKIN,skinIndex,0);
				if (pGameRoomEx->m_pCallGMDlg.IsWindowVisible())
					pGameRoomEx->m_pCallGMDlg.PostMessage(WM_EXCHANGE_SKIN,skinIndex,0);
				if (pGameRoomEx->dlg.IsWindowVisible())
					pGameRoomEx->dlg.PostMessage(WM_EXCHANGE_SKIN,skinIndex,0);
				if (pGameRoomEx->m_pGetHelpMoneyDlg != NULL)
					pGameRoomEx->m_pGetHelpMoneyDlg->PostMessage(WM_EXCHANGE_SKIN,skinIndex,0);
				if (pGameRoomEx->m_pPersonBank != NULL)
					pGameRoomEx->m_pPersonBank->PostMessage(WM_EXCHANGE_SKIN,skinIndex,0);
				if (pGameRoomEx->m_pManageDlg != NULL)
					pGameRoomEx->m_pManageDlg->PostMessage(WM_EXCHANGE_SKIN,skinIndex,0);
				if (pGameRoomEx->m_pQueueInRoomDlg != NULL)
					pGameRoomEx->m_pQueueInRoomDlg->PostMessage(WM_EXCHANGE_SKIN,skinIndex,0);
				if (pGameRoomEx->m_pUserList != NULL)
					pGameRoomEx->m_pUserList->PostMessage(WM_EXCHANGE_SKIN,skinIndex,0);
				if (pGameRoomEx->m_pUserDetailsDlg != NULL)
					pGameRoomEx->m_pUserDetailsDlg->PostMessage(WM_EXCHANGE_SKIN,skinIndex,0);
				if (pGameRoomEx->m_pFindUser != NULL)
				{
					pGameRoomEx->m_pFindUser->PostMessage(WM_EXCHANGE_SKIN,skinIndex,0);
					pGameRoomEx->m_pFindUser->m_UserList.PostMessage(WM_EXCHANGE_SKIN,skinIndex,0);
				}
				if (pGameRoomEx->m_IGameFrame != NULL)
					pGameRoomEx->m_IGameFrame->OnControlMessage(CM_EX_SKIN,&(::GetMainRoom()->m_PlaceUserInfo),sizeof(MSG_GP_R_LogonResult));
			}

			//�����ұߴ���
			CRoomRightWnd* pRightWnd = pMainRoom->m_RoomInfo[i].pRoomInfoWnd;
			if (NULL != pRightWnd)
			{
				pRightWnd->PostMessage(WM_EXCHANGE_SKIN,skinIndex,0);
				pRightWnd->m_ChatControl.PostMessage(WM_EXCHANGE_SKIN,skinIndex,0);
				pRightWnd->m_UserList.PostMessage(WM_EXCHANGE_SKIN,skinIndex,0);
				if (pRightWnd->m_TalkMessage.m_DiyChat.IsWindowVisible())
					pRightWnd->m_TalkMessage.m_DiyChat.OnExchangeSkin(strSkinFileName,GetKeyVal(strSkinFileName),GetSkinPath());
			}
		}
	}
}
