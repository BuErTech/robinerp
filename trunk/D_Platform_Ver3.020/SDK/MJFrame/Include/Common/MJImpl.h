// �齫��ܵײ��ļ�
#ifndef WINDOWS_LOGIC
	// ������ļ�
	#include "SoDeskLogicImpl.cpp"
	#include "SoGameFactory.cpp"
	#include "SoGameLogerImpl.cpp"
	#include "SoGameProcessor.cpp"
	#include "SoGameSettingImpl.cpp"
	#include "SoUserManager.cpp"
	#include "SoBaseLogic.cpp"
	#include "CLog.cpp"
	#include "LoadMoudelHelp.cpp"

	#include "SoMJTimerMgr.cpp"
	#include "SoMJFrameLogic.cpp"
	#include "SoMJFrameSetting.cpp"
#else
    // �ͻ����ļ�
	#include "MJFrameLogic.cpp"
#endif

// �齫����߼����ÿ��ļ�
#include "IMJStateEvent.cpp"
#include "MJ.cpp"
#include "MJRule.cpp"
#include "MJFanCounter.cpp"
#include "ComConfig.cpp"