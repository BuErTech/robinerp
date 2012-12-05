#pragma once
#ifndef IGIF_HEAD_H
#define IGIF_HEAD_H
class EXT_CLASS IGif
{
protected:
	IGif(void);

	virtual ~IGif(void);

public:
	// �����Լ���
	virtual ULONG Release() = 0;

public:
	// ����		��	���ļ�װ��Gif��
	// ����1	��	�ļ�����
	// ����		��	�ɹ�����NOERROR,ʧ�ܷ���E_FAIL;
	virtual HRESULT LoadFromFile(LPCTSTR lpstrFileName) = 0;

	// ����		��	���ļ�װ��Gif��
	// ����1	��	Gif����Դ�е�Ŀ¼��.����WAVװ�غ������"WAVE"��,������WAVE��
	// ����2	��	��Դ����
	// ����		��	�ɹ�����NOERROR,ʧ�ܷ���E_FAIL;
	virtual HRESULT LoadFromRes(LPCTSTR lpstrResDirectory, LPCTSTR lpstrResName) = 0;

	// ����		��	�������ػ���gif��ĳһ�壻
	// ����1	��	gif����ID��
	// ����2	��	Ŀ��DCָ�룻
	// ����3	��	Ŀ��λ�����Ͻ�X�����ꣻ
	// ����4	��	Ŀ��λ�����Ͻ�Y�����ꣻ
	// ����		��	�ɹ�����NOERROR,ʧ�ܷ���E_FAIL;
	virtual HRESULT Bitblt(DWORD dwFrameId, CDC *pDestDC, int ixDest, int iyDest) = 0;


	// ����		��	�������ػ���gif��ĳһ�壻
	// ����1	��	�Ѿ����ڵ�ʱ�䣻
	// ����2	��	Ŀ��DCָ�룻
	// ����3	��	Ŀ��λ�����Ͻ�X�����ꣻ
	// ����4	��	Ŀ��λ�����Ͻ�Y�����ꣻ
	// ����		��	�ɹ�����NOERROR,ʧ�ܷ���E_FAIL;
	virtual HRESULT Bitblt(CDC *pDestDC, int ixDest, int iyDest) = 0;

	// ����		��	�����ػ���gif��ĳһ�壻
	// ����1	��	gif����ID��
	// ����2	��	Ŀ��DCָ�룻
	// ����3	��	Ŀ��λ�����Ͻ�X�����ꣻ
	// ����4	��	Ŀ��λ�����Ͻ�Y�����ꣻ
	// ����5	��	������Ŀ�
	// ����6	��	������ĸߣ�
	// ����		��	�ɹ�����NOERROR,ʧ�ܷ���E_FAIL;
	virtual HRESULT StretchBlt(DWORD dwFrameId, CDC *pDestDC, int ixDest, int iyDest, int iWidth, int iHeight) = 0;

	virtual DWORD GetImageWidth() = 0;

	virtual DWORD GetImageHeight() = 0;

	virtual DWORD GetFrameCount() = 0;
	
	// ����		��	gif�������ã�
	virtual void Reset() = 0;
};

#endif
//class IGif
//{
//protected:
//	IGif(void);
//
//	virtual ~IGif(void);
//
//public:
//	virtual ULONG Release() = 0;
//
//public:
//	// ����		��	���ļ�װ��Gif��
//	// ����1	��	�ļ�����
//	// ����		��	�ɹ�����NOERROR,ʧ�ܷ���E_FAIL;
//	virtual HRESULT LoadFromFile(LPCSTR lpstrFileName) = 0;
//
//	// ����		��	���ļ�װ��Gif��
//	// ����1	��	�ļ�����
//	// ����		��	�ɹ�����NOERROR,ʧ�ܷ���E_FAIL;
//	virtual HRESULT LoadFromFile(LPCWSTR lpstrFileName) = 0;
//
//	// ����		��	���ļ�װ��Gif��
//	// ����1	��	Gif����Դ�е�Ŀ¼��.����WAVװ�غ������"WAVE"��,������WAVE��
//	// ����2	��	��Դ����
//	// ����		��	�ɹ�����NOERROR,ʧ�ܷ���E_FAIL;
//	virtual HRESULT LoadFromRes(LPCSTR lpstrResDirectory, LPCTSTR lpstrResName) = 0;
//
//	// ����		��	���ļ�װ��Gif��
//	// ����1	��	Gif����Դ�е�Ŀ¼��.����WAVװ�غ������"WAVE"��,������WAVE��
//	// ����2	��	��Դ����
//	// ����		��	�ɹ�����NOERROR,ʧ�ܷ���E_FAIL;
//	virtual HRESULT LoadFromRes(LPCWSTR lpstrResDirectory, LPCTSTR lpstrResName) = 0;
//
//	// ����		��	�������ػ���gif��ĳһ�壻
//	// ����1	��	gif����ID��
//	// ����2	��	Ŀ��DCָ�룻
//	// ����3	��	Ŀ��λ�����Ͻ�X�����ꣻ
//	// ����4	��	Ŀ��λ�����Ͻ�Y�����ꣻ
//	// ����		��	�ɹ�����NOERROR,ʧ�ܷ���E_FAIL;
//	virtual HRESULT Bitblt(DWORD dwFrameId, CDC *pDestDC, int ixDest, int iyDest) = 0;
//
//	// ����		��	�����ػ���gif��ĳһ�壻
//	// ����1	��	gif����ID��
//	// ����2	��	Ŀ��DCָ�룻
//	// ����3	��	Ŀ��λ�����Ͻ�X�����ꣻ
//	// ����4	��	Ŀ��λ�����Ͻ�Y�����ꣻ
//	// ����5	��	������Ŀ�
//	// ����6	��	������ĸߣ�
//	// ����		��	�ɹ�����NOERROR,ʧ�ܷ���E_FAIL;
//	virtual HRESULT StretchBlt(DWORD dwFrameId, CDC *pDestDC, int ixDest, int iyDest, int iWidth, int iHeight) = 0;
//
//	virtual DWORD GetImageWidth() = 0;
//
//	virtual DWORD GetImageHeight() = 0;
//
//	virtual DWORD GetFrameCount() = 0;
//};
