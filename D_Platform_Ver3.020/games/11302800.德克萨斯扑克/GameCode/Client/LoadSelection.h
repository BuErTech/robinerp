#pragma once

#include "GameLanchSDK/bzwUiInclude/bzwUI.h"
#include "defines.h"
//#include "../../../../sdk/include/GameLanchSdk/LoveSendClassInExe.h"
#include "..\GameMessage\UpGradeMessage.h"

#define  IDT_MODIFY_MONEY	1	//  �ȴ�����޸Ĵ�����

struct SubMoney
{
	int nMinMoney;     // ��С������
	int nMaxMoney;     // ��������
	int nCurrentMoney; // ��ǰҪ����Ľ��
	int nSelfMoney;    // �Լ�����ӵ�еĽ��
	int nMinusMoney ;  // �޸�ע��Ӽ����
	bool bAuto;        // �Զ�����
	SubMoney()
	{
		memset(this, 0, sizeof(SubMoney));
	}
};

class CLoadSelectionDlg : public BModalContainer
{
public:

	CLoadSelectionDlg(DWORD pId,CWnd* pParent=NULL);

	~CLoadSelectionDlg();

	void SetMessage(SubMoney stData);

	/// ����ǰ�������еĿؼ���Ϣ
	virtual bool OnPostMessage(UINT pId,BBaseParameter* pParam);

	/// ���ضԻ���
	virtual bool LoadConfig(std::string path);

	// д����
	void WriteMoney();

public:

	//���ó�ֻ���˳���״̬����Ϊû����ң�ֻ���Լ������Ļ���ֻ���˳�
	void SetNotice(std::string path,bool bShowNextBnt);

	//���óɷ�������Ϸ��Finish��״̬
	void SetGameLoadSelection(bool bLoadSelection);

	// �Զ���ʼ
	void SetAutoStart();
private:
	CLoadSelectionDlg();

private:
	CWnd* m_pWndParent;
	bool m_bGameLoadSelection;
	CString m_strEdit;        // ��ǰ������

public:
	SubMoney m_subMoney;      // ��������
	int      m_nOldSelfMoney; // �Լ�����ӵ�еĽ�ұ���
	
public:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	afx_msg void OnEnChangeEdit();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};