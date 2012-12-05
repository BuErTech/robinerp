#pragma once

#include "GameLanchSDK/bzwUiInclude/bzwUI.h"
#include "defines.h"
//#include "../../../../sdk/include/GameLanchSdk/LoveSendClassInExe.h"
#include "..\GameMessage\UpGradeMessage.h"

struct TResults
{
	int iBetPools[8];				//������ע��
	int iUserBet[PLAY_COUNT][8];	//ÿ�������ע����
	int iScore[PLAY_COUNT];			//��Ӯ����
	int iMoney[PLAY_COUNT];			//��Ӯ���
	int iSelfMoney;					//�Լ��Ľ�Ҹ���: �ͻ��˻�ȡ�Ľ���п��ܻ�û��ˢ��, ���������﷢��ȥ
	int nUserID[PLAY_COUNT];		//ÿ����ҵ�ID

	TResults()
	{
		memset(this, 0, sizeof(TResults));
	}
};

class CFinishDlg : public BModalContainer
{
public:

	CFinishDlg(DWORD pId,CWnd* pParent=NULL);

	~CFinishDlg();

	void SetMessage(const TResult& Result);

	/// ����ǰ�������еĿؼ���Ϣ
	virtual bool OnPostMessage(UINT pId,BBaseParameter* pParam);

	/// ���ضԻ���
	virtual bool LoadConfig(std::string path);

public:

	//���ó�ֻ���˳���״̬����Ϊû����ң�ֻ���Լ������Ļ���ֻ���˳�
	void SetNotice(std::string path,bool bShowNextBnt);

	//���óɷ�������Ϸ��Finish��״̬
	void SetGameFinish(bool bFinish);

private:
	CFinishDlg();

private:

	CWnd* m_pWndParent;

	bool m_bGameFinished;

};