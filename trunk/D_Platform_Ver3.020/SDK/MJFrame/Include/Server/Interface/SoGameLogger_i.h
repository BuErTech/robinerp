#ifndef _SOLOGGER_H
#define _SOLOGGER_H

#include "Svrdef.h"
#include "gameobject_i.h"
#include "gameevent_i.h"


// SO����־ϵͳ����Ϊһ��ģ�飬ʹ�õ�ʱ������Query֮��Ȼ�������ӿڽ�����־�������֧�ֶ�̬ˢ��log�������ļ�
// log�������ļ��������з�����Ч���Ǹ�ȫ�ֵ����á�
// [setting]
// log = evtf1s	//log�ĺ��壺 e(error���أ�  v��verbose���أ�t��trace���أ�f1��������ļ���s���������Ļ��
// logPath = "../log/DaGunZi"

namespace MyGame
{
									// ->8765 4321<-
									//  "0000,0000"
	enum 
	{
		configed = 0x80,		//  8bit:�Ƿ��Ѿ���ȡ����
		excludelog = 0x40,		//  7bit:�ų� exclude
        //tomultifile = 0x10,	//  5bit:�Ƿ�д������ļ�
		toscreen = 0x10,		//  5bit:�Ƿ�printf���նˣ������ļ��У�s��
		tosinglefile = 0x8,		//  4bit:�Ƿ�д��ͳһ�ļ��������ļ��У�f1��
		logerror = 0x4,			//  3bit:�Ƿ�Log���� 
		logverbose = 0x2,		//  2bit:�Ƿ�Log��ϸ
		logtrace = 0x1			//  1bit:�Ƿ�Log��ͨ 
	};
	class ISoGameLogger : public IGameObject
	{
	public:
		virtual void  OutputTraceInfo(const char* msg, ...) = 0;     
		virtual void  OutputVerboseInfo(const char* msg, ...) = 0;     
		virtual void  OutputErrorInfo(const char* msg, ...) = 0; 
// 		virtual BYTE GetGlobalLoggerFlag() = 0;
// 		virtual void OutputLog(const char* lpszLogBuf) = 0;
	};

}

#endif //_SOLOGGER_H
