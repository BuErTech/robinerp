#pragma once

#ifndef CAFCFUNCTION_HEAD_FILE
#define CAFCFUNCTION_HEAD_FILE

#include "PublicDefine.h"

//AFC ����
	//��ʾ��Ϣ��
	//EXT_CLASS UINT AFCMessage(UINT uResourceID, HINSTANCE hInstance, const TCHAR * const szTitle, UINT uType, CWnd * pParentWnd);
	//��ʾ��Ϣ��
	//EXT_CLASS UINT AFCMessage(const TCHAR * const szText, const TCHAR * const szTitle, UINT uType, CWnd * pParentWnd);
	//��������
	EXT_CLASS bool AFCPlaySound(HINSTANCE hInstance, TCHAR * szResourceName);
	//λͼת��������
	EXT_CLASS HRGN AFCBmpToRgn(HBITMAP hBmp, COLORREF cTransparentColor, COLORREF cTolerance);
	//�滭͸��ͼ
	EXT_CLASS bool AFCStretchBlt(CDC * pDesDC, int iXDes, int iYDes, int iDesWidth, int iDesHeight, CDC * pScrDC, int iXScr, int iYScr, int iScrWidth, int iScrHeight, COLORREF crColor);
	//�滭͸��ͼ
	EXT_CLASS bool AFCStretchImage(CDC * pDesDC, int iXDes, int iYDes, int iDesWidth, int iDesHeight,HBITMAP hBitBmp, int iXScr, int iYScr, int iScrWidth, int iScrHeight, COLORREF crColor);
	//bmpת��������
	EXT_CLASS HRGN CreateRegionFromBitmap(HBITMAP hBitmap, COLORREF transparentColor);
	//��ʾ��͸��������ͼƬ
	EXT_CLASS bool AFCAlphaBlend(HDC destdc, int dx, int dy, int cx, int cy, HDC srcdc, int sx, int sy, COLORREF crTransparent, int SourceConstantAlpha);
	//��ͼ
	EXT_CLASS void SaveScreen();
	//������
	EXT_CLASS HBITMAP CopyScreenToBitmap() ;
	//�����ļ�
	EXT_CLASS int SaveBitmapToFile(HBITMAP   hBitmap  ,LPTSTR   lpFileName);
	//�����ַ�����������ȡ�ַ�
	EXT_CLASS bool GetChar(TCHAR * src, TCHAR * dest, int count);
	//д��error��ͷ�Ĵ�����־
	EXT_CLASS void WriteError(TCHAR *pMessage,int iRoomID = 0,int iSmallRoomID = 0);
	//д�����ִ�
	EXT_CLASS void WriteStr(TCHAR *str,int iRoomID = 0,int iSmallRoomID = 0);
	EXT_CLASS void WriteStr(int iValue);
	EXT_CLASS void WriteStr(long int iValue);
	EXT_CLASS void WriteStr(DWORD iValue);
	//ɾ����־
	EXT_CLASS void Dellog(int iRoomID = 0,int iSmallRoomID = 0);
	//���غ���
	EXT_CLASS int InternetGetFile(TCHAR szUrl[], TCHAR szFileName[],CString &filename);
	//�쳣����
	EXT_CLASS LONG WINAPI ExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo);

	//wushuqun 2008.9.24
	//�Զ�����Ϣ��
	EXT_CLASS UINT AFCMessageBox(CString strInfo,CString strTitel = "ϵͳ��ʾ",UINT style = 500);
	EXT_CLASS UINT AFCTimerMsgBox(int iTimer, CString strInfo,CString strTitel = "ϵͳ��ʾ",UINT style = 500);

	//��Ϣ��,DirectUI��

	/// ��ʾ
	/// @param pToCtrl ��ĳһ�ؼ�����ʾ
	/// @param hParentWnd �����ھ��
	/// @param style ��ʽ����WinAPI��MessageBox�ṩ����ʽһ��
	/// @param lpTitle ��Ϣ��ı���
	/// @param bFormat �Ƿ�ͨ�������ָ�ʽ����������ʽ������ʱ�����ֿ��Ծ���
	/// @param lpMainText ��Ϣ�����ı���֧���ı��ĸ�ʽ��
	/// @return UINT ����ѯ�ʽ������WinAPI��MessageBox�ṩ�ķ���ֵһ��
	/// @author JianGuankun,2012.7.25
	///
	EXT_CLASS UINT DUIMessageBox(HWND hParentWnd,UINT style,LPCTSTR lpTitle,bool bFormat,LPCTSTR lpMainText,...);

	//ֻ��һ��OK��ť�����ҹ̶�����
	EXT_CLASS UINT DUIOkeyMsgBox(HWND hParentWnd,bool bFormat,LPCTSTR lpMainText,...);

#endif