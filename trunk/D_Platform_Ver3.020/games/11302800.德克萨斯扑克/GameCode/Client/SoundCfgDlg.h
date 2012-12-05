#pragma once

#include "GameLanchSDK/bzwUiInclude/bzwUI.h"
#include "defines.h"
//#include "../../../../sdk/include/GameLanchSdk/LoveSendClassInExe.h"
#include "..\GameMessage\UpGradeMessage.h"
#include "Common\commonuse.h"

class CSoundCfgDlg : public BModalContainer
{
public:

	CSoundCfgDlg(DWORD pId,CWnd* pParent=NULL);

	~CSoundCfgDlg();

	/// ����ǰ�������еĿؼ���Ϣ
	virtual bool OnPostMessage(UINT pId,BBaseParameter* pParam);

	/// ���ضԻ���
	virtual bool LoadConfig(std::string path);

public:

	/// �ط����Ե�ѡ���¼���Ӧ
	void OnSelectDiFang(UINT uiID);

	/// �Ի���ȷ��
	void OnClickedOK(BBaseParameter* pBBPOK);

private:
	CSoundCfgDlg();

private:

	CWnd* m_pWndParent;

	//INI�ļ�����
	CINIFile m_iniFile;

	//�������ֿ���ѡ��ؼ�
	BzRadioButton* m_pChkBgMusic;

	//��Ϸ��Ч����ѡ��ؼ�
	BzRadioButton* m_pChkGameSound;

	//��׼����ѡ��ؼ�
	BzRadioButton* m_pRdoStander;

	//�ط�����ѡ��ؼ�
	BzRadioButton* m_pRdoDiFang;

};