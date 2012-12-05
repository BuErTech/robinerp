#pragma once


///�洢�����Ϣ
struct playStrData_t
{
	UINT uUserID[PLAY_COUNT];             //���id��
	UINT uUserMoney[PLAY_COUNT];         //< ���i ʣ��Ǯ��
	char nickName[PLAY_COUNT][61];           //����ǳ�
	ULONG dwUserIP[PLAY_COUNT];             //���ip��ַ   
	UINT  iRoomBasePoint;                   //���䱶��
	playStrData_t()
	{
		memset(uUserID,0,sizeof(uUserID));
		memset(uUserMoney,0,sizeof(uUserMoney));
		memset(nickName,0,sizeof(nickName));
	}
};

///��װ�����Ϣ
struct SaveData_t
{
	WORD wTag;
	UINT uTime;
	UINT uLen;
	playStrData_t buffer;
};

///������
class CAsaveAndReadfile
{
public:
	CAsaveAndReadfile(void);
public:
	~CAsaveAndReadfile(void);
	void  Serialize(CArchive&ar);  
	
	void  SaveFileMessage(NetMessageHead * pNetHead, void * pNetData,UINT uDataSize );

	//�洢�����
	void SavePlayMessage(SaveData_t *pNetPlayMessage,CString strfile);
	int  m_iWeyByte[100];         //ÿһ�����ݰ��Ĵ�С
	int  m_iCountMove;            //��¼һ���ж��ٸ����ݰ�

	//�ӷ���˵õ������ݰ�
	
    //��ʼʱ��
	LONG m_firsttime;
	//����ʱ��
	LONG m_lastlongtime;
	
	CString m_strFile;

};