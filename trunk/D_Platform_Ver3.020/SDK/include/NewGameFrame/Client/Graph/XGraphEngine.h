#ifndef __XGraphEngine_H
#define __XGraphEngine_H

#include "CommonDefines.h"
#include <windows.h>
#include <tchar.h>

/// �ṩ����ܵ��õĽӿڣ�ֻ������ʼ���������ʵ�ֲ���Ҫ����

/// ͼ������ӿ�
class XGraphEngine
{
public:
	/// ���������������Ա��ͷ���Դ
	virtual ~XGraphEngine(){}
	/// ��ʼ�����ṩ���ھ����D3D�豸
	virtual HRESULT Initial(HWND hwnd, HANDLE hKeyBoard, HANDLE hMouse) = 0;
	/// ��ʼ����
	virtual HRESULT Start2Draw() = 0;
	/// ��������
	virtual HRESULT FinishDraw() = 0;
	/// ���ڻָ��״̬
	virtual bool OnActivate(bool bIsActived)=0;
	/// �豸��ʧ
	virtual void OnDeviceLost()=0;
	/// �豸�ָ�
	virtual HRESULT OnResetDevice()=0;
protected:
	/// ��ȡִ���ļ�����Ŀ¼����·��
	const TCHAR *GetAppPath()
	{
		static TCHAR s_Path[MAX_PATH];
		static bool s_bIsReady = false;
		if (!s_bIsReady)
		{
			ZeroMemory(s_Path,sizeof(s_Path));
			DWORD dwLength=GetModuleFileName(GetModuleHandle(NULL), s_Path, sizeof(s_Path));
			TCHAR *p = _tcsrchr(s_Path, TEXT('\\'));
			++p;
			*p = TEXT('\0');
			s_bIsReady = true;
		}
		return s_Path;
	}
};
extern GRAPH_API XGraphEngine *GlbGraphEngine();
#endif//__XGraphEngine_H