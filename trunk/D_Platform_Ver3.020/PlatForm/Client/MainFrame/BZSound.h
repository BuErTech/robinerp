#ifndef _BZSOUND_INCLUDE_FILE_2008_07_FRED_HUANG_
#define _BZSOUND_INCLUDE_FILE_2008_07_FRED_HUANG_

// ������Ϊ�Ǳ������ġ�
#ifdef BZSOUND_EXPORTS
#define BZSOUND_API __declspec(dllexport)
#else
#define BZSOUND_API __declspec(dllimport)
#endif

#define BZSOUND_STOP		0
#define BZSOUND_PLAYING		1
#define BZSOUND_PAUSE		2

BZSOUND_API int fnBZSound(void);

/*************************************************************************
Function	:BZSoundInitial()
Note		:ʹ��ǰҪ��ʼ��
Author		:Fred Huang 2008-07-22
*************************************************************************/
BZSOUND_API void BZSoundInitial();

/*************************************************************************
Function	:BZSoundPlay
Note		:��������
Author		:Fred Huang 2008-07-22
Parameter	:
CWnd 		*:��Ҫһ�����ھ������������
szFilename	:�����ļ���
nLoop		:�Ƿ�ѭ������

Return		:
int			:����һ��ID�ţ������Ĳ������ݸ�ID��������
*************************************************************************/
BZSOUND_API int	BZSoundPlay(CWnd *pWnd,char *szFilename,DWORD dwPlayEnd,int nLoop=1);

/*************************************************************************
Function	:BZSoundStop
Note		:ֹͣ����
Author		:Fred Huang 2008-07-22
Parameter	:
nSoundID			:��BZSoundPlay()���ص�ID��
*************************************************************************/
BZSOUND_API void BZSoundStop(int nSoundID);
/*************************************************************************
Function	:BZSoundStopAll
Note		:ֹͣ������������
Author		:Fred Huang 2008-07-22
*************************************************************************/
BZSOUND_API void BZSoundStopAll();

/*************************************************************************
Function	:BZSoundGetVolume
Note		:ȡ��������
Author		:Fred Huang 2008-07-22
Parameter	:
nSoundID	:��BZSoundPlay()���ص�ID��
*************************************************************************/
BZSOUND_API long BZSoundGetVolume(int nSoundID);

/*************************************************************************
Function	:SetVolume
Note		:ȡ��������
Author		:Fred Huang 2008-07-22
Parameter	:
nSoundID	:��BZSoundPlay()���ص�ID��
nVolume		:������С
*************************************************************************/
BZSOUND_API void BZSoundSetVolume(int nSoundID,long nVolume);

/*************************************************************************
Function	:BZSoundPause
Note		:��ͣ��������
Author		:Fred Huang 2008-07-23
Parameter	:
nSoundID	:��BZSoundPlay()���ص�ID��
*************************************************************************/
BZSOUND_API void BZSoundPause(int nSoundID);

/*************************************************************************
Function	:BZSoundContinue
Note		:�������ű���ͣ������
Author		:Fred Huang 2008-07-23
Parameter	:
nSoundID	:��BZSoundPlay()���ص�ID��
*************************************************************************/
BZSOUND_API void BZSoundContinue(int nSoundID);



/*************************************************************************
Function	:BZSoundCreateList
Note		:����һ���б�ʽ����ͨ��
Author		:Fred Huang 2008-07-23
Parameter	:
CWnd *		:��Ҫһ�����ھ������������
dwPlayEnd	:�������˺�������ڵ���Ϣ
Return		:
int			:����һ��ID�ţ������Ĳ������ݸ�ID��������
*************************************************************************/
BZSOUND_API int BZSoundCreateList(CWnd *pWnd,DWORD dwPlayEndMsg);


/*************************************************************************
Function	:BZSoundAddList
Note		:�ڲ����б��У���������ļ�
Author		:Fred Huang 2008-07-23
Parameter	:
nSoundID	:
szFilename	:�ļ���
Return		:
bool		:�Ƿ���ӳɹ�
*************************************************************************/
BZSOUND_API bool BZSoundAddList(int nSoundID,char *szFilename);

/*************************************************************************
Function	:BZSoundDestoryList
Note		:��������б����ͷ���Դ
Author		:Fred Huang 2008-07-23
Parameter	:
nSoundID	:
*************************************************************************/
BZSOUND_API void BZSoundDestoryList(int nSoundID);


/*************************************************************************
Function	:BZSoundPlayList
Note		:�����б��е�����
Author		:Fred Huang 2008-07-23
Parameter	:
nSoundID	:
*************************************************************************/
BZSOUND_API void BZSoundPlayList(int nSoundID);

#endif