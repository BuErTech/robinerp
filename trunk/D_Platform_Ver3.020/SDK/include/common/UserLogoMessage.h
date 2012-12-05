#pragma once

#ifndef AFC_USER_LOGO_MESSAGE_HEAD_FILE
#define AFC_USER_LOGO_MESSAGE_HEAD_FILE

/*
�ļ�˵�������ļ����������û�ͷ���ϴ�������ص���Ϣ�ͽṹ��
		  ����ƽ̨ʱ����Ҫ��GameRoomLogonDT�а������ļ�
�ļ�������Fred Huang 2008-03-17
��Ϣ��ʽ��MDM_		��ʾ����Ϣ
		  ASS_UL	��ʾ���û�ͷ���������Ϣ
		  ASS_ULC	��ʾ�ɿͻ�����������˷��͵���Ϣ
		  ASS_ULS	��ʾ�ɷ���������ͻ��˷��͵���Ϣ
�ṹ��ʽ��MSG_		��ʾ����Ϣ�ṹ��
		  MSG_UL	��ͷ����ص���Ϣ�ṹ
		  MSG_UL_C_ �ɿͻ��˷����������˵���Ϣ�ṹ��
		  MSG_UL_S_ �ɷ������˷����ͻ��˵���Ϣ�ṹ��
*/

//����Ϣ
#define MDM_GR_USER_LOGO				120					//ͷ�����


//����Ϣ
#define ASS_ULC_UPLOAD					0x01					//�ϴ�ͷ������
#define ASS_ULC_INFOREQUEST				0x02					//����ͷ���ļ���Ϣ
#define ASS_ULC_DOWNREQUEST				0x03					//��������ͷ���ļ�
#define ASS_ULC_DOWNRESULT				0x04					//�������صĽ����ʵ�������ڷְ�����ʱ������һ���ݰ�

#define ASS_ULS_UPLOADSUCCEED			0x11					//�ϴ���ͷ��ɹ�
#define ASS_ULS_UPLOADFAILED			0x12					//�ϴ�ͷ��ʧ��
#define ASS_ULS_LOGOINFORMATION			0x13					//�û�ͷ���ļ���Ϣ
#define ASS_ULS_DOWN					0x14					//����ͷ������


#define MAX_BLOCK_SIZE					512						//ÿ����Ϣ�����ɴ�ͷ�����ݴ�С
/*
�ṹ���ϴ�ͷ�����ݽṹ
���ݣ��ļ������ļ���С���۰���С���۰������۰���š�����ͼƬ������Ч��С��ͼƬ����
*/
typedef struct 
{
	long		dwUserID;										//�û�ID
	int			dwUserSourceLogo;								//ԭʼ��LOGOid��
	int 		nFileSize;										//�ļ���С��һ�㲻����20K����20480Byte
	int 		nBlockSize;										//�۰���С
	int			nBlockCount;									//�۰���
	int			nBlockIndex;									//�۰���ţ���0-nPackageCount
	int			nPackageSize;									//������Ϣ�����������ݴ�С
	char		szData[MAX_BLOCK_SIZE];							//ͼ������
}MSG_UL_C_UPLOAD,MSG_UL_S_DOWN;//����ͷ�����ݰ�

//�ϴ����ؽ��
typedef struct 
{
	long		dwUserID;										//�ϴ������ص��û�ID
	int			nBlockCount;									//�ϴ������ص����ݿ�����
	int			nBlockIndex;									//�ϴ������ص����ݿ�����
	bool		bNeedCheck;
}MSG_UL_C_DOWNRESULT,MSG_UL_S_UPLOADRESULT;

//�����û���ͷ������
typedef struct 
{
	long		dwUserID;										//�����ߵ��û�ID
	long		dwRequestUserID;								//�������ߵ��û�ID
}MSG_UL_C_INFORREQUEST,MSG_UL_C_DOWNREQUEST;//��������ͷ��

//���ص��û�ͷ����Ϣ
typedef struct 
{
	long		dwUserID;										//�û�ID��
	long		dwUserLogoID;									//ͷ��ID�ţ����=0xFF���ű�ʾ���Զ���ͷ�񣬴˴����������һ�٣���Ϊ�˹淶�ͷ�ֹ�����������
	char		szFileMD5[33];									//ͷ���ļ���MD5ֵ
}MSG_UL_S_INFORMATION;

#endif