#include"stdafx.h"

//������������
#define SOUND_OUT_CARD				0x10				//��������
#define SOUND_WARNING				0x20				//����
#define SOUND_SEND_CARD				0x30				//��������
#define SOUND_OUT_CARD_DEF			0x40				//Ĭ�ϳ�������
#define SOUND_OUT_CARD_BOMB			0x50				//��ը��
#define SOUND_OUT_CARD_PASS			0x60				//����
#define SOUND_INVALIDATE			0x70				//��ʾʧ��
#define SOUND_START_DEF				0x80				//�ĸ��˿�ʼ��Ϸ��
#define SOUND_GAME_OVER				0x90				//��Ϸ����
#define SOUND_ADD_DOUBLE			0xA0				//�Ӱ�
#define SOUND_ADD_DOUBLE_NOT		0xA1				//���Ӱ�
#define SOUND_ROB_NT                0xB0				//������
#define SOUND_ROB_NT_NOT            0xB1				//��������
#define SOUND_SHOW_CARD				0xC0				//����

#define SOUND_WIN					0x01				//ʤ��
#define SOUND_LOSE					0x02				//��
#define SOUND_WAITING				0x03				//�ȴ�
#define SOUND_ERROR					0x04				//���Ƴ�����
#define SOUND_READY					0x05				//��Ϸ��ʼ
#define SOUND_START					0x06				//׼������һ����
#define SOUND_START_OUT_CARD		0x07				//����һ����
#define SOUND_CALL_SCORE			0x08				//�з�
#define SOUND_OUT_CARD_FINISH		0x09				//ĳ��ҳ��ƽY��
#define SOUND_GAME_PASS				0x0A				//����
#define SOUND_FRIEND_APPEAR			0x0B				//���ѳ��F
#define SOUND_USER_RUNAWARY			0x0C				//��������
#define SOUND_USER_LEFT				0x0D				//�����뿪
#define SOUND_GIVE_BACK				0x0E				//���

#define SOUND_OUT_CARD_KINGBOMB		0x41				//��ը
#define SOUND_OUT_CARD_LIANDUI		0x42				//����
#define SOUND_OUT_CARD_SHUNZI		0x43				//˳��
#define	SOUND_OUT_CARD_SAN			0x44				//����
#define SOUND_OUT_CARD_SANDAIY		0x45				//����һ
#define SOUND_OUT_CARD_SANDAID		0x46				//����һ��
#define SOUND_OUT_CARD_SIDAIE		0x47				//�Ĵ���
#define SOUND_OUT_CARD_SIDAID		0x48				//�Ĵ�һ��
#define SOUND_OUT_CARD_PLANE		0x49				//�ɻ�
//��ע����(�������)
#define TYPE_GIVE_UP					0x00			//����
#define TYPE_NOTE						0x01			//��ע
#define TYPE_ADD						0x02			//��ע
#define TYPE_FOLLOW						0x03			//��ע
#define TYPE_SHUTTLE					0x04			//���

//������ע����(רΪ���׼��)
BOOL SetNoteSound(BYTE bNoteStyle,BOOL bIsBoy);
//����ͨ������
BOOL SetPlaySound(BYTE bSoundType);
//�������������O��
HWND SetOutCardTypeSound(BYTE iCardList[],int iCardCount,HWND hParent,BOOL IsBoy = true,BOOL IsFirest = TRUE);
//����mp3����
HWND PlayCardMusic(TCHAR FileName[],HWND hParent);
//���볡������
HWND SetSceneSound(BYTE iSceneSound,HWND hParent,BOOL IsBoy = TRUE,int iVal = -1);
//����������mp3���� (090402b1 JJ)
HWND SetDdzSound(BYTE bSoundType, HWND hParent, BOOL IsBoy = TRUE);
//���������ŵ��źͶ������� (090402b1 JJ)
HWND SetDdzCardSound(BYTE cardNum, HWND hParent, BYTE count, BOOL bFirstOut, BOOL IsBoy = TRUE);