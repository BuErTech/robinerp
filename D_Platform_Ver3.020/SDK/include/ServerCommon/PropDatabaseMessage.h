#pragma once

#ifndef AFC_PROP_DATAMESSAGE_HEAD_FILE_
#define AFC_PROP_DATAMESSAGE_HEAD_FILE_
#include "AFCDataBase.h"
#include "MainManage.h"


//���ݿ���Ϣ
#define DTK_GP_PROP_GETPROP				300						//����ͷ��ID
#define DTK_GR_PROP_USE					310						//ʹ�õ���
#define DTK_GP_PROP_BUY					320						//�������
#define DTK_GP_PROP_GIVE				330						//���͵���
#define DTK_GPR_PROP_BIG_BRD			340						//������
#define DTK_GPR_PROP_BIG_BRD_BUYADNUSE			341						//������
#define DTK_GPR_PROP_SMALL_BRD			350						//С����

// PengJiLin, 2010-10-13, �̵���ߵļ�ʱ������
#define DTK_GP_PROP_BUY_NEW				360
//�������VIP����  add by wyl  11-5-21
#define DTK_GP_PROP_BUY_VIP				370



#define DTR_GR_PROP_USE_ERR_PARAM_INVALID	10
#define DTR_GR_PROP_USE_ERR_UNKNOW			20
#define DTR_GR_PROP_USE_ERR_NOPROP			30
#define DTK_GR_PROP_USE_SUCCEED				80

#define DTK_GP_PROP_BUY_ERROR				10
#define DTK_GP_PROP_BUY_NOMONEY				20
#define DTK_GP_PROP_BUY_SUCCEED				80
#define DTK_GP_PROP_BUYANDUSE_SUCCEED				81
//�����û�ͷ�����ݿ�����ṹ
typedef struct
{
	DataBaseLineHead					DataBaseHead;						//���ݰ�ͷ
	long								dwUserID;							//�û�ID��
} PROP_GP_I_GETPROP;

typedef struct 
{
	DataBaseResultLine					ResultHead;
	MSG_PROP_S_GETPROP					msgPropGet;
}PROP_GP_O_GETPROP;


typedef struct  
{
	DataBaseLineHead					DataBaseHead;
	_TAG_USINGPROP						usingProp;
}DL_GR_I_USEPROP;

typedef struct  
{
	DataBaseResultLine					ResultHead;
	_TAG_USINGPROP						usingProp;
}DL_GR_O_USEPROP;

typedef struct  
{
	DataBaseLineHead					DataBaseHead;
	_TAG_PROP_BUY						propBuy;
}PROP_GP_I_BUY;


typedef struct  
{
	DataBaseLineHead					DataBaseHead;
	_TAG_PROP_BUY_VIP					propBuyVIP;
}PROP_GP_I_BUY_VIP;

typedef struct  
{
	DataBaseResultLine					ResultHead;
	MSG_PROP_S_BUYVIP					msgPropGet;
}PROP_GP_O_BUY_VIP;

typedef struct  
{
	DataBaseResultLine					ResultHead;
	MSG_PROP_S_GETPROP					msgPropGet;
}PROP_GP_O_BUY;

typedef struct  
{
	DataBaseLineHead					DataBaseHead;
	_TAG_PROP_GIVE						propGive;
}PROP_GP_I_GIVE;

typedef struct  
{
	DataBaseResultLine					ResultHead;
	_TAG_PROP_GIVE						propGive;
}PROP_GP_O_GIVE;

typedef struct  
{
	DataBaseResultLine					ResultHead;
	_TAG_PROP_GIVE_FOR					propBeGive;
}PROP_GP_O_GIVE_FOR;

typedef struct  
{
	DataBaseLineHead					DataBaseHead;
	_TAG_BOARDCAST						boardCast;
}PROP_GPR_I_BOARDCAST;

typedef struct  
{
	DataBaseResultLine					ResultHead;
	_TAG_BOARDCAST						boardCast;
}PROP_GPR_O_BOARDCAST;

#endif