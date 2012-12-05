#pragma once

#include "CommonDefines.h"
#include <Windows.h>
#include <tchar.h>

class X3DEngine;

/// �������࣬���ø�������ڴ���ָ��λ���л���ֱ��
class GRAPH_API XLine
{
	friend class X3DEngine;
public:
	XLine():m_x1(0), m_y1(0), m_x2(100), m_y2(100), m_z(0), m_color1(0xFF000000), m_color2(0xFF000000), m_style(0), m_thick(0){}
	~XLine();

	/// @defgroup group1 �������Ե�����
	/// ����ֱ�ߵĸ�������
	/// @{
	inline void SetPos(int x1, int y1, int x2, int y2){ m_x1 = x1; m_x2 = x2; m_y1 = y1; m_y2=y2; }		///< ����λ��
	inline void SetDeep(short z){m_z = z;}						///< ������ȣ����۾��ľ��룬Խ��ԽԶ
	inline void SetColor(UINT color1, UINT color2){m_color1 = color1; m_color2 = color2;}					///< ������ɫ
	/// @} 

	/// ShowHide()��ʾ������
	void ShowHide(bool bShow = true);

protected:
private:
	int m_x1, m_y1, m_x2, m_y2;		///< ����յ�λ��
	int m_z;					///< ��Ȳ��
	UINT m_color1;			///< �����ɫ
	UINT m_color2;			///< �յ���ɫ
	int m_style;				///< ������߻�ʵ�ߣ���ʵ��20100505
	int m_thick;				///< �ߵĴ�ϸ����ʵ��20100505
};
