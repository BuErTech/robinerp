#pragma once
#ifndef CARDSET_HEAD_H
#define CARDSET_HEAD_H

struct SetCardStruct	//�ͻ��˵�������
{
	BYTE pai[8][54];
	int	paicount[8];
};
class EXT_CLASS CCardSet: public CDialog
{
	DECLARE_DYNAMIC(CCardSet)

private:
	int						m_iCardCount;			//�Ƹ���
	int						m_iCurrentDesk;			//��ǰλ��
	int						m_iOldDesk;				//��λ��
	SetCardStruct			m_paistruct;			//�ƽṹ
	int						m_RemainCard[54];		//ʣ������
public:
	CCardSet(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCardSet();

	// Dialog Data
	//enum { IDD = IDD_CARD_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	virtual void OnOK();
	//ѡ����
	void OnChoiceCard(UINT nID);
	//ѡλ�ú�
	void OnChoiceDesk(UINT nID);
	//���õ�ǰ����
	void SetCurrentData(int iDesk);
	//���¿ؼ�
	BOOL UpdateComboBox();
	//��ֵ�õ���
	BYTE GetCardByValue(int Value);
};

#endif
