#ifndef _BZW_UI_LIBRARY_HEADER_FRED_HUANG_2008_07_ 
#define _BZW_UI_LIBRARY_HEADER_FRED_HUANG_2008_07_ 
// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� BZUI_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// BZUI_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef BZUI_EXPORTS
#define BZUI_API __declspec(dllexport)
#else
#define BZUI_API __declspec(dllimport)
#endif

// �����Ǵ� BZUI.dll ������
class BZUI_API CBZUI {
public:
	CBZUI(void);
	// TODO: �ڴ�������ķ�����
};

extern BZUI_API int nBZUI;

BZUI_API int fnBZUI(void);


/************************************************************************
Function	:PlayMoiveBMP
Notes		:����BMP�������ö������ɶ���BMPƴ��������
Parameter	:
pWnd		:�������ŵĸ�����
szFilename	:BMP�ļ���
bRgn		:�Ƿ�Ҫȥ��������ע�⣬����ɫһ����RGB(255,0,255)�����ٸ�����
nFrames		:�ö����ж���֡
nInterval	:ÿ֮֡��������ʱ�䣬��λΪms
���nFrames=1 ��nInterval=0����ֻ�޶�����ֻ��ʾһ����ͼ���һ֡
nPlayMode	:����ģʽ
0		:������һ��ͽ���
1		:������ͣ�����һ֡��ֱ��ʹ�� BZEngineStopBMP(DWORD)
2		:ѭ�����ţ�ֱ��ʹ�� BZEngineStopBMP(DWORD)
************************************************************************/
BZUI_API DWORD BZUIPlayBMP(CWnd *pWnd,LPCSTR szFilename,bool bRgn,int nFrames,int nInterval,int nPlayMode);

BZUI_API bool BZUIDestoryMoive(DWORD dwHandle);

BZUI_API void BZUIDestoryAll();

BZUI_API void BZUIInitial();
BZUI_API void BZUIShutDown();


#endif