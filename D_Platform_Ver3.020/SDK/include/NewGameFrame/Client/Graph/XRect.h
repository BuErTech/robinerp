#pragma once

#include "CommonDefines.h"
#include <Windows.h>
#include <tchar.h>

class X3DEngine;

/// �������࣬���ø�������ڴ���ָ��λ���л��ƾ���
class GRAPH_API XRect
{
	friend class X3DEngine;
public:
	XRect():m_l(0), m_t(0), m_r(10), m_b(10), m_z(0), m_colorBold(0xFF000000), m_colorFill(0xFFFFFFFF), m_style(0), m_thick(0){}
	~XRect();

	/// @defgroup group1 �������Ե�����
	/// ���þ��εĸ�������
	/// @{
	inline void SetWidth(int w){m_r = m_l + w;}		///< ���ÿ�
	inline void SetHeight(int h){m_b = m_t + h;}	///< ���ø�
	inline void SetX(int x){m_r += x-m_l; m_l = x;}	///< �������ϽǺ�����
	inline void SetY(int y){m_b += y-m_t; m_t = y;}	///< �������Ͻ�������
	inline void SetThick(int thick = 0){m_thick = thick;}		///< ���ú�ȣ��ݲ���
	inline void SetStyle(int style = 0){m_style = style;}		///< ���÷�񣬷�0ֵΪʵ�ľ���
	inline void SetColor(DWORD color){m_colorBold = color;}		///< ���ñ߿���ɫ
	inline void SetFillColor(DWORD color){m_colorFill = color;}	///< ���������ɫ�����ľ��κ��Դ���
	inline void SetDeep(short z){m_z = z;}						///< ������ȣ����۾��ľ��룬Խ��ԽԶ
	/// @} 

	/// ���λ����Ϣ������ǰ���õĴ�С
	inline void GetPosition(WORD &x, WORD &y){x=m_l, y=m_t;}
	inline void GetSize(WORD &w, WORD &h){w=m_r-m_l; h=m_b-m_t;}

	/// ShowHide()��ʾ�����ؾ���
	void ShowHide(bool bShow = true);

protected:
private:
	int m_l, m_t, m_r, m_b;		///< λ�úͳߴ磬�����洰�ڴ�С�����ܳ���655335��������WORD����
	int m_z;					///< ��Ȳ��
	UINT m_colorBold;			///< �߿���ɫ
	UINT m_colorFill;			///< �����ɫ
	int m_style;				///< ������߻�ʵ�ߣ���ʵ��20100505
	int m_thick;				///< �ߵĴ�ϸ����ʵ��20100505
};
