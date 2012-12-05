#pragma once

#include "CommonDefines.h"
#include <Windows.h>
#include <set>
#pragma warning(disable:4251)

class X2DFont;
class XFont_Greater;

/// ������ʾ���ֵĽӿ���
class GRAPH_API XFont
{
	friend class X2DFont;
	friend class XFont_Greater;
public:
	XFont(void);
	~XFont(void);

	/// Init() ��ʼ������
	/// @param[in] szFontName �������ƣ�����windows����һ�£����Ҳ�������Ĭ��Ϊ����
	/// @param[in] nSize �����С
	/// @param[in] bItalic �Ƿ�б��
	/// @param[in] bBold �Ƿ�Ӵ�
	/// @param[in] bUnderline �Ƿ���»���
	/// @param[in] bIsManyChar �Ƿ���ܻ��кܶ಻ͬ����
	bool Init( TCHAR *szFontName, int nSize, bool bItalic = false, bool bBold = false, bool bUnderline = false, bool bIsManyChar =false );
	/// SetString()�����ַ�������
	bool SetString(TCHAR *szText);
	/// SetXY()�޸��ı��ڴ����е�����
	void SetXY(short x, short y);
	/// SetDeep()�����ı������
	void SetDeep(short z);
	/// SetColor()�����ı�����ɫ
	/// param ǰ�ĸ�������Ӧ���ϣ����ϣ����£�������ɫֵ���м䲿�ְ��Ľǲ�ֵ��
	/// �����������ʾ�ַ���ѡ�����ֳ�����ɫ��ֻ��һ��
	void SetColor(DWORD colorLT, DWORD colorRT,DWORD colorLB,DWORD colorRB, DWORD colorSelect = 0xFFFFFFFF);
	/// ShowHide()��ʾ����������
	void ShowHide(bool bShow = true);
	/// CommitChanges()�ύ�����ĸ���
	void CommitChanges();
	/// �ƶ�λ��
	/// @param[in] dir ������� 0��ʾx����1��ʾy����2��ʾz����
	/// @param[in] offset ƫ��������Ϊ����
	void Move(int dir, int offset)
	{
		switch(dir)
		{
		case 0: m_x += offset; break;
		case 1: m_y += offset; break;
		case 2: m_z += offset; break;
		default: break;
		}
	}
	/// ѡ������
	/// @param st ѡ��Ŀ�ʼλ�ã��ڼ����ַ���0Ϊ��һ���ַ�
	/// @param ed ѡ��Ľ���λ�ã��ڼ����ַ���0Ϊ��һ���ַ�
	/// ����ѡ��ʱ������SelectText(-1, -1);
	void SelectText(short st, short ed){m_selBegin = st; m_selEnd = ed;}
	/// ���òü���Χ
	void SetClipBox(short l, short r, short t, short b){m_sClipBox[0]=l; m_sClipBox[1]=r;m_sClipBox[2]=t; m_sClipBox[3]=b;}

private:
	//Tstring m_str;			///< ���ؼ����ַ���
	TCHAR *m_szText;		///< ���ؼ����ַ���
	int	m_nLen;				///< ���ؼ����ַ����鳤��
	X2DFont *m_p2DFont;		///< ��������ָ��
	short m_x, m_y;			///< ����λ��
	short m_z;				///< ������ȣ�ֵԽ�����۾�ԽԶ���ڴ�����Խ���ڵײ㣬Խ���ױ�����Ԫ���ڵ�
	DWORD m_color[4];		///< ������ɫ��0-3��Ӧ�������������ε����ϣ����ϣ����£����½ǡ�
	DWORD m_colorSelect;	///< ��ѡ��ɫ��ֻ��һ�֣����ṩ��ͬ���㲻ͬ��ɫ
	short m_selBegin;		///< �ӵڼ����ַ���ʼѡ��
	short m_selEnd;			///< ���ڼ����ַ�����ѡ��
	short m_sClipBox[4];		///< �ü���Χ��0,1,2,3��Ӧ���ң��ϣ���
};

/// XFontָ���������
class XFont_Greater : public std::binary_function<XFont *, XFont *, bool>
{
public:
	bool operator()(const XFont *pl, const XFont *pr)
	{
		return (pl->m_z > pr->m_z);
	}
};

/// ʹ�÷���
