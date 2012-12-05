#pragma once
#ifndef MJSET_HEAD_H
#define MJSET_HEAD_H
// CMjSet dialog
struct SetPaiStruct	//�ͻ��˵�������
{
	int pai[8][20];
	int	paicount[8];
};

class EXT_CLASS CMjSet : public CDialog
{
	DECLARE_DYNAMIC(CMjSet)
private:
	int						m_iCurrentDesk;			//��ǰλ��
	int						m_iOldDesk;				//��λ��
	SetPaiStruct			m_paistruct;			//�ƽṹ
	int						m_RemainCard[42];		//ʣ������

public:
	CMjSet(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMjSet();

// Dialog Data
	//enum { IDD = IDD_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	//int	m_id[42];
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
	//ѡλ�ú�
	void OnChoiceDesk(UINT nID);
	//���õ�ǰ����
	void SetCurrentData(int iDesk);
	//���¿ؼ�
	BOOL UpdateComboBox();
	//��ֵ�õ���
	int GetCardByValue(int Value);

};

#endif