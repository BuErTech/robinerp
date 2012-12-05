#pragma once

#ifndef AFC_BANK_MESSAGE_HEAD_FILE
#define AFC_BANK_MESSAGE_HEAD_FILE

#define MDM_GP_BANK									121

	#define ASS_GP_BANK_OPEN						21      //�����л�ȡǮ����Ϣ
		#define HC_GP_BANK_OPEN_SUC					1		//��Ǯ��ɹ�
		#define HC_GP_BANK_OPEN_ERR					2		//��Ǯ�����
		#define HC_GP_BANK_OPEN_ERR_PSW				3		//��Ǯ�����

	#define ASS_GP_BANK_GETGM_WALLET				22      //��ȡ��ϷǮ��(��������ר��)

	#define ASS_GP_BANK_CHECK						23		//���д�ȡǮ
		#define HC_GP_BANK_CHECK_SUC				0		//�����ɹ�
		#define HC_GP_BANK_CHECK_NO_USER			1		//û���ҵ��û�
		#define HC_GP_BANK_CHECK_ERR_PWD			2		//��������
		#define HC_GP_BANK_CHECK_NOT_MONEY			4		//����

	#define ASS_GP_BANK_TRANSFER					24		//����ת��
		#define HC_GP_BANK_TRANSFER_ERROR			1		//ת�ʴ���
		#define HC_GP_BANK_TRANSFER_SUC				2		//ת�ʳɹ�
		#define HC_GP_BANK_TRANSFER_NO_DEST			3		//ת��Ŀ�겻����
		#define HC_GP_BANK_TRANSFER_PASSWORD_ERROR	4		//ת���������
		#define HC_GP_BANK_TRANSFER_TOTAL_LESS		5		//�����ܶ�̫С�������ʸ�
		#define HC_GP_BANK_TRANSFER_TOO_LESS		6		//����ת����Ŀ̫��
		#define HC_GP_BANK_TRANSFER_MULTIPLE		7		//����ת����Ŀ������ĳ���ı���
		#define HC_GP_BANK_TRANSFER_NOT_ENOUGH		8		//���н�������ת��
		#define HC_GP_BANK_TRANSFER_TOO_MANY_TIME   9		//����ת�˵Ĵ���̫����

	#define ASS_GP_BANK_TRANS_RECORD				25		//����ת�˼�¼
		#define HC_GP_BANK_TRANS_RECORD_SUC			1		//����ת�˼�¼������ȡ

	#define ASS_GP_BANK_DEPOSIT						26		//�浥ҵ��
		#define HC_GP_BANK_DEPOSIT_SUC				0		//�浥ҵ�����ɹ�

	#define ASS_GP_BANK_QUERY_DEPOITS				27		//�浥��ѯ
	#define ASS_GP_BANK_CHPWD						28		//�޸���������
		#define HC_GP_BANK_CHPWD_SUC				1		//����ת�˼�¼������ȡ
		#define HC_GP_BANK_CHPWD_ERR_PWD			2		//����ת�˼�¼������ȡ

	#define ASS_GP_BANK_FINANCE_INFO				29		//�������
		#define HC_GP_BANK_FINANCE_INFO_SUC			1		//��������������ȡ
		#define HC_GP_BANK_FINANCE_INFO_ERR			2		///�������ݻ�ȡ�쳣

//������(ʹ�ö�������)
struct TMSG_GP_BankOpenReq
{
	char szMD5Pass[50]; //��������
};

//Ǯ������
struct TMSG_GP_BankInfo
{
	long int        dwUserID;
	__int64         i64Bank;							///Ǯ��
	char			szTwoPassword[50];					//��������
	int				nVer;								//���а汾
};

//��ϷǮ���ṹ
struct TMSG_GP_BankWallet
{
	int				_game_id;
	char			_game_name[30];
	__int64			_money;
};

//���д�ȡǮͨѶ��
struct TMSG_GP_BankCheck
{
	int				_operate_type;
	int				_user_id;
	int				_game_id;
	TCHAR			_szMD5Pass[50];  //��������
	__int64			_money;
};

//ת��
struct TMSG_GP_BankTransfer
{
	long int		UserID;					//�û�ID
	long int		destUserID;				//ת��˭
	__int64			i64Money;				//��ת�ʽ��
	__int64			i64ActualTransfer;		//ʵ�ʵ��ʽ��
	int				bTranTax;               //�Ƿ��˰����0����˰
	TCHAR			szMD5Pass[50];			//��������
	TCHAR			szNickName[50];			//�û��ǳ�		add by lxl 2010-11-10
	TCHAR			szDestNickName[50];		//��ת���û��ǳ�
	bool			bUseDestID;
	UINT            uHandleCode;			// PengJiLin, 2010-8-23, ���������Ƿ��ǡ�ȫת������

	TMSG_GP_BankTransfer()
	{
		ZeroMemory(this,sizeof(TMSG_GP_BankTransfer));
	}
};

///ת�ʼ�¼��ѯ����
struct TMSG_GP_BankTranRecordReq
{
	long int		dwUserID;	///�û� ID
};

//�û�ת�ʼ�¼���
struct TMSG_GP_BankTranRecord
{
	int				nSeriNo;				///< ���
	UINT			dwUserID;				///< �û� ID
	UINT			destUserID;				///< ת��˭
	__int64			i64Money;				///< ��ת�ʽ��
	__int64			i64ActualTransfer;		///< ʵ�ʵ��ʽ��
	COleDateTime	oleDateTime;			///< ʱ��

	TMSG_GP_BankTranRecord()
	{
		ZeroMemory(this, sizeof(TMSG_GP_BankTranRecord));
	}
};

//�浥,��ֵ
struct TMSG_GP_BankDeposit
{
	int		_operate_type;		//1Ϊ�����浥,2Ϊ��ֵ
	int		_user_id;
	TCHAR	_szNickName[50];	//��ת���û��ǳ�
	TCHAR	_szMD5Pass[50];		//��������
	int		_state;
	int		_csds;
	__int64	_money;
	char	_deposit_id[16];
	char	_deposit_password[16];
	char	_time[32];
};

//�浥��ѯ�ṹ
struct TMSG_GP_BankQueryDeposit
{
	int				_serial_no;	    //���
	char			_deposit_id[16];
	char			_deposit_password[16];
	__int64			_money;
	COleDateTime	_create_time;
	int				_csds;
	int				_user_id;
	int				_state;
	int				_used_user_id;
};

///�޸�����
struct TMSG_GP_BankChPwd
{
	long int		_user_id;								///�û�ID
	TCHAR			_MD5Pass_old[50];					///���������
	TCHAR			_MD5Pass_new[50];					///����������
	TMSG_GP_BankChPwd()
	{
		ZeroMemory(this, sizeof(TMSG_GP_BankChPwd));
	}
};


#endif