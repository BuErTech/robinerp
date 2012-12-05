#pragma once

#ifndef AFC_BANK4Z_DATAMESSAGE_HEAD_FILE_
#define AFC_BANK4Z_DATAMESSAGE_HEAD_FILE_

#include "AFCDataBase.h"
#include "MainManage.h"

//���ݿ���Ϣ
#define DTK_GP_BANK_OPEN			80       //��ѯ���
#define DTR_GP_BANK_OPEN_SUC		81		 //��Ǯ��ɹ�
#define DTR_GP_BANK_OPEN_ERROR		82		 //��Ǯ�����
#define DTK_GP_BANK_GETGM_WALLET	83		 //��Ǯ��
#define DTK_GP_BANK_CHECK			84		 //��ȡ
#define DTK_GP_BANK_TRANSFER		85		 //ת��
#define DTK_GP_BANK_TRANS_RECORD	86		 //ת�˼�¼
#define DTK_GP_BANK_DEPOSIT			87		 //�浥ҵ��
#define DTK_GP_BANK_QUERY_DEPOSIT	88		 //��ѯ�浥
#define DTK_GP_BANK_CHPWD			95		 //�޸���������
#define DTK_GP_BANK_FINANCE_INFO	96		 //��ȡ���˲������

//������
struct DL_GP_I_BankOpen
{
	DataBaseLineHead		DataBaseHead;
	long int				dwUserID;				//�û� ID
};

struct DL_GP_O_BankOpen
{
	DataBaseResultLine		ResultHead;
	TMSG_GP_BankInfo		_data;
};

//��ѯǮ��
struct DL_GP_I_BankGetWallet
{
	DataBaseLineHead		DataBaseHead;
	int						_UserID;
};

struct DL_GP_O_BankGetWallet
{
	DataBaseResultLine		ResultHead;
	TMSG_GP_BankWallet		_data;
};

//��ȡǮ
struct DL_GP_I_BankCheck
{
	DataBaseLineHead		DataBaseHead;
	TMSG_GP_BankCheck		_data;
};

struct DL_GP_O_BankCheck
{
	DataBaseResultLine		ResultHead;
	TMSG_GP_BankCheck		_data;
};

//ת��
struct DL_GP_I_BankTransfer
{
	DataBaseLineHead		DataBaseHead;
	TMSG_GP_BankTransfer    _data;
};

struct DL_GP_O_BankTransfer
{
	DataBaseResultLine		ResultHead;
	TMSG_GP_BankTransfer    _data;
};

///ת�ʼ�¼
struct DL_GP_I_BankTransRecord
{
	DataBaseLineHead			DataBaseHead;
	TMSG_GP_BankTranRecordReq	_data;
};

struct DL_GP_O_BankTransRecord
{
	DataBaseResultLine			ResultHead;	
	TMSG_GP_BankTranRecord		_data;
};

//�浥����
struct DL_GP_I_BankDeposit
{
	DataBaseLineHead			DataBaseHead;
	TMSG_GP_BankDeposit			_data;
};

struct DL_GP_O_BankDeposit
{
	DataBaseResultLine			ResultHead;
	TMSG_GP_BankDeposit			_data;
	__int64						_Money;
};

//�浥��ѯ
struct DL_GP_I_BankQueryDeposit
{
	DataBaseLineHead			DataBaseHead;
	int							_UserID;
};

struct DL_GP_O_BankQueryDeposit
{
	DataBaseResultLine			ResultHead;
	TMSG_GP_BankQueryDeposit	_data;
};

//�޸���������
struct DL_GP_I_BankChPwd
{
	DataBaseLineHead			DataBaseHead;
	TMSG_GP_BankChPwd			_data;
};

struct DL_GP_O_BankChPwd
{
	DataBaseResultLine			ResultHead;
};


#endif