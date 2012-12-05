#pragma once

#ifndef AFC_IM_MESSAGE_HEAD_FILE
#define AFC_IM_MESSAGE_HEAD_FILE


/*
�ļ�˵�������ļ���������IM��ص���Ϣ�ͽṹ��
����ƽ̨ʱ����Ҫ��GameRoomLogonDT�а������ļ�
�ļ�������Fred Huang 2008-04-2
��Ϣ��ʽ��MDM_		��ʾ����Ϣ
ASS_IM	��ʾ��IM�������Ϣ
ASS_IMC	��ʾ�ɿͻ�����������˷��͵���Ϣ
ASS_IMS	��ʾ�ɷ���������ͻ��˷��͵���Ϣ
�ṹ��ʽ��MSG_		��ʾ����Ϣ�ṹ��
MSG_IM	��ͷ����ص���Ϣ�ṹ
MSG_IM_C_ �ɿͻ��˷����������˵���Ϣ�ṹ��
MSG_IM_S_ �ɷ������˷����ͻ��˵���Ϣ�ṹ��
*/

//����Ϣ
#define MDM_GP_IM						130					//ͷ�����


//����Ϣ
#define ASS_IMC_GETCOUNT				0x01					//ȡ�÷������Ϻ�������İ�����еĲ�ȡ����Ҫ�����ص���������������
#define ASS_IMC_GETUSERINFO				0x02					//ȡ��ĳ�����ѵ���ϸ����
#define ASS_IMC_ADDREQUEST				0x03					//�����Ϊ���ѣ������Ƿ��ǵ�һ�η��������ڶԷ���Ҫȷ��ʱ����
#define ASS_IMC_ADDREQUESTRESPONSE		0x04					//���������Ϊ���ѣ������Ƿ�ͬ��
#define ASS_IMC_SEARCH					0x05					//��ѯ���ѣ���Ϊ�����벻���ߣ�ͨ��ID�Ż�������������
#define ASS_IMC_MSG						0x0F					//������Ϣ
#define ASS_IMC_SETGROUP				0x06					//���ú�����

#define ASS_IMS_GETCOUNT				0x11					//���غ���������ȡ������
#define ASS_IMS_GETUSERINFO				0x12					//���غ��ѵ���ϸ����,����Ҫ��������
																//ID�ţ��������Ա��Ƿ�����
#define ASS_IMS_GETLIST					0x13					//���غ����б�������ID�ţ��������Ա��Ƿ�����
#define ASS_IMS_USERONLINE				0x14					//�����ߺ�֪ͨ����
#define ASS_IMS_GETONLINELIST			0x15					//���ߺ󣬷������ߵĺ��ѣ������ȡ�������б����ٷ��ش���Ϣ
#define ASS_IMS_ADDREQUEST				0x16					//���������Ϊ���ѣ���Ҫ��ȷ��
#define ASS_IMS_ADDREQUESTRESPONSE		0x17					//ת���Ƿ�ͬ���Ϊ���ѣ������ͬ��ģ����������Ҫ������¼
#define ASS_IMS_USERNOTONLINE			0x1E					//ת��������Ϣʧ�ܣ��û�������
#define ASS_IMS_MSG						0x1F					//ת��������Ϣ

#define ASS_IMS_NOAC					0x23					//ת��������Ϣ


#define ASS_SET_FRIENDTYPE				0x20					///���ø��˺��Ѻ�����Ľ�������


#define MAX_BLOCK_MSG_SIZE				512						//ÿ����Ϣ���������

#define IM_ADDRESPONSE_AGREE			0x0						//ͬ���Ϊ����
#define IM_ADDRESPONSE_REFUSE			0x1						//�ܾ���Ϊ����
#define IM_ADDRESPONSE_IDENT			0x2						//��Ҫ��֤

#define IM_SEARCH_BY_ONLINE				0x0						//��ѯ�����û�
#define IM_SEARCH_BY_NAME				0x1						//ͨ���û�������ѯ
#define IM_SEARCH_BY_SEX				0x2						//ͨ���Ա��ѯ

//=======================���ݽṹ����=====================
/*
�ṹ��ȡ�÷������Ϻ�����/���ط������ϵĺ�����
���ݣ��û���ID����ֹ���󣬿��Բ�Ҫ�ģ������غ�����
	
*/

typedef struct  
{
	long		dwUserID;						//�û�ID
	long		dwRemoteUserID;					//���ѵ�ID
	int			groupID;						//��ID
}MSG_IM_C_SETGROUP;

typedef struct  
{
	long		dwUserID;						//�û�ID
	int			dwUserCount;					//�û��ĺ�����
	int			dwUserCountReturn;				//���ص��û�������
}MSG_IM_C_COUNT,MSG_IM_S_COUNT;

/*
�ṹ������ȡĳ���û�������
���ݣ������ߵ�ID�ͱ������ߵ�ID���Լ��û���
*/
typedef struct 
{
	long		dwUserID;						//������ID
	long		dwRequestedUserID;				//��������ID
}MSG_IM_C_GETUSERINFO;

/*
�ṹ���û���������Ϣ
���ݣ��û���������Ϣ
ʹ����������Ϣ�У�
*/
typedef struct  
{
	long		dwUserID;						//�û�ID
	long		dwRemoteID;						//���ͽ����ߵ�ID
	char		sUserName[30];					//����
	bool		nSex;							//�Ա�
	long		nOnline;						//�Ƿ�����
	long		GroupID;						//����ID���ݲ�ʹ��
}MSG_IM_S_GETUSERINFO;

/*
�ṹ�������Ϊ����
���ݣ������ߵ�ID���������ߵ�ID������Է���Ҫ��֤���򷢵ڶ������󣬸��ô˽ṹ�壬��ˣ�Ҫ�����Ƿ��ǵ�һ�η��ͣ��Լ���������
ע	������ֻ֧��������ߵ��û�
*/
typedef struct 
{
	long		dwUserID;						//�������û�ID
	
	char		sUserName[61];					//����������  	//change by yjj 090323
	long		dwRequestedUserID;				//�������û�ID
	char		sRequestedUserName[61];					//����������  	//change by yjj 090323
	int			nMsgLength;						//������Ϣ�ĳ���
	int			cbSize;							//������Ϣ�ĳ���
	char		sRequestNotes[128];				//��������ݣ�ʵ�ʳ�����nMsgLength���
}MSG_IM_C_ADDREQUEST,MSG_IM_S_ADDREQUEST;

/*
�ṹ���Ƿ�ͬ���Ϊ����
*/
typedef struct 
{
	long		dwUserID;						//���������ߵ��û�ID
	long		dwRequestedUserID;				//�������ߣ�����Ӧ�û�����ID
	long		dwRefusedUserID;				//����󱻾ܾ����û�id
	char		sRequestedUserName[61];			//�������ߣ�����Ӧ�û���������//change by yjj 090323
	char        sRequirUsrName[61]  ;           //�����ߵ�����  add by yjj 090323
	int			nResponse;						//��Ӧ��ʽ��IDYES-ͬ��,IDNO-��ͬ��
	int			nSex;							//��Ӧ�ߵ��Ա�
}MSG_IM_C_ADDRESPONSE,MSG_IM_S_ADDRESPONSE;

/*
�ṹ����ѯ�û�
���ݣ������ǲ�ѯ�����û���Ҳ������ͨ���û�������ѯ
ע�⣺����ͨ��ID�Ų�ѯ����ΪID�Ŷ��û������ǲ�͸����
	  ���ؽ����MSG_IM_S_GETUSERINFO
*/
typedef struct  
{
	long		dwUserID;						//�����ѯ���û�ID
	int			nSearchType;					//��ѯ�ķ�ʽ����ֻ֧�ֵ����ѯ����֧����ϲ�ѯ
	int			nSex;							//ֻ����nSearchType==IM_SEARCH_BY_SEXʱ��Ч
	char		sSearchName[30];				//ֻ����nSearchType==IM_SEZRCH_BY_NAMEʱ��Ч
}MSG_IM_C_SEARCHUSER;

/*
�ṹ�������û��б�������û���Ϣ
*/
typedef struct  
{
	int			nListCount;						//����Ϣ���а������û��б���
	int			nBodyLength;					//����Ϣ�����ܹ������ݳ���
	char		sBody[MAX_BLOCK_MSG_SIZE];		//��Ϣ�壬���MSG_IM_S_GETUSERINFO
}MSG_IM_S_USERLIST;

/*
�ṹ����ʱ��Ϣ
���ݣ�Ҫ����������Ϣ
*/
typedef struct  
{
	long		dwUserID;						//�û���ID��
	char		szUserName[30];					//�û�������
	long		dwRemoteUserID;					//�Է���ID��
	CHARFORMAT	cf;								//��Ϣ��ʽ
	char		szFontname[50];					//�������ƣ�CHARFORMAT�п��ܻ᲻���������Ϣ����Ϊ����һ��ָ��
	long		szMsgLength;					//��Ϣ����
	int			cbSize;							//������Ϣ�ܳ���
	char		szMsg[MAX_BLOCK_MSG_SIZE];		//��Ϣ��
}MSG_IM_C_MSG,MSG_IM_S_MSG;
#endif