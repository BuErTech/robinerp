#pragma once

//////////////////////////////////////////////////////////////////////////
/// ����Э�鶨��
/// ���пͻ�����������˵�����Э���ɱ��ļ��ṩ
/// �����������ܲ�ͬ�ֳ��������֣����ķ���������¼����������Ϸ�����������B��Z��W
/// ���ļ���ZXD��2010��6��2�մ���
//////////////////////////////////////////////////////////////////////////


#include "StructDefines.h"	///< ���¸�Э�����õ������ݽṹ�������ﶨ��

//////////////////////////////////////////////////////////////////////////
/// B������Э��
//////////////////////////////////////////////////////////////////////////
#define MDM_CONNECT						1			//������Ϣ����
#define		ASS_NET_TEST					1			//�������
#define		ASS_CONNECT_SUCCESS 			3			//���ӳɹ�
#define MDM_GP_REQURE_GAME_PARA			102			// ������Ϸȫ�ֲ�������ASS��ʶ
#define	MDM_GP_REQURE_GAME_CHECK		107			// ������Ϸȫ�ֲ�������ASS��ʶ

//////////////////////////////////////////////////////////////////////////
/// Z������Э��
//////////////////////////////////////////////////////////////////////////

#define	MDM_GP_LOGON					100									//������½
#define		ASS_GP_LOGON_BY_NAME			1									//ͨ���û����ֵ�½
#define		ASS_GP_LOGON_BY_ACC				2									//ͨ���û�ACC ��½
#define		ASS_GP_LOGON_BY_MOBILE			3									//ͨ���û��ֻ���½
#define		ASS_GP_LOGON_REG				4									//�û�ע��
#define		ASS_GP_LOGON_SUCCESS			5									//��½�ɹ�
#define		ASS_GP_LOGON_ERROR				6									//��½ʧ��

#define	MDM_GP_LIST						101									//��Ϸ�б�
#define		ASS_GP_LIST_KIND				1									//��ȡ��Ϸ�����б�
#define		ASS_GP_LIST_NAME				2									//��ȡ��Ϸ�����б�
#define		ASS_GP_LIST_ROOM				3									//��ȡ��Ϸ�����б�
#define		ASS_GP_LIST_COUNT				4									//��ȡ��Ϸ�����б�
#define		ASS_GP_ROOM_LIST_PEOCOUNT		5									//��ȡ��Ϸ�����б�
#define		ASS_GP_ROOM_PASSWORD			6									//���ͷ������� Add by zxd����ͼ�������뷿��ʱ���ʹ���Ϣ
#define		ASS_GP_ROOM_CREATE				7									//ʹ�ÿ�������������


#define	MDM_GP_MESSAGE					103									//��Ϣ�ӷ�
#define		ASS_GP_NEWS_SYSMSG				1									//���ź�ϵͳ��Ϣ
#define		ASS_GP_DUDU						2									//С����

#define	MDM_GP_SYSMSG					104									//ϵͳ��Ϣ
#define		ASS_GP_NEWS_SYSMSG				1									//���ź�ϵͳ��Ϣ
#define		ASS_GP_DUDU						2									//С����

#define	MDM_GP_USER_OPT					105									//�������Ӻ���
#define		ASS_UPDATE_FRIEND				1									//���º���
#define		ASS_UPDATE_EMENY				2									//���»���



