#ifndef _PUBLIC_DEFINE_H_
#define _PUBLIC_DEFINE_H_

// ��EXE��ICOʹ��ʱ��EXT_CLASSΪ����
#ifndef THIS_IS_DLL
#	define EXT_CLASS _declspec(dllimport)
#else
#	define EXT_CLASS _declspec(dllexport)
#endif 

#include <GdiPlus.h>
using namespace Gdiplus;
#pragma comment(lib,"gdiplus.lib")


#endif
