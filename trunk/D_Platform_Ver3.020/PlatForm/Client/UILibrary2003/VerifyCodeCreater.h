#pragma once

#ifndef CVERIFYCODECREATER_HEAD_FILE
#define CVERIFYCODECREATER_HEAD_FILE

class EXT_CLASS CVerifyCodeCreater
{
public:
	CVerifyCodeCreater();
	virtual ~CVerifyCodeCreater();

public:
	static CVerifyCodeCreater* GetInstance();
	void SetParent(const HWND& hHwnd);
	void ReCreateBMP(int iWidth,int iHeight,COLORREF crBackGround);
	void Draw(int x,int y);
	bool IsVerify(LPCTSTR lpszInput);

private:
	void AddBackGroundPicture();
	void BackgroundDisturb();
	void FontDisturb(const HDC &dc,const CString & strCode);
	void PixelDisturb(const HDC &dc,const HBITMAP &hBitmap);
	void PixelDisturbOnFont(const HDC &dc,const HBITMAP &hBitmap);

	void BackGroundClear(DWORD color);

	bool CreateTexture(char* FileName,const HWND &m_hWnd);

	//���������֤���㷨
	void RandomCode(BYTE* pChar,int iNums);

private:

	//ͼƬ�ĸ߸���
	int m_iWidth;
	int m_iHeight;

	//ͼƬ�Ŀ�͸���Ϣ
	int m_textrueWidth;
	int m_textrueHeight;

	//ͼƬ����ɫ����Ϣ
	DWORD* m_pTextureData;

	//���һ����Ӧ��С�ı���,��������ɫ���и���
	DWORD* m_backPixel;

	//���ɵ�bitmap�������Ϣ
	BITMAPINFOHEADER bih;

	LOGFONT logFont;  
	HFONT hFont;

	//��������
	CString m_FontArray[23];

	//�����ڵ�DC
	HDC m_dcParentWnd;

	//��֤��ͼƬ����DC
	HDC m_dcCodeMem;

private:

    static CVerifyCodeCreater* m_pInstance;

	//����ַ���
	unsigned char m_szCode[5];
};

#endif // !defined(CVERIFYCODECREATER_HEAD_FILE)
