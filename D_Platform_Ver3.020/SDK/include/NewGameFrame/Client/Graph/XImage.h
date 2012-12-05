#pragma once

/***********************************************************************
* Module:  XImage.h
* Author:  Administrator
* Modified: 2010��4��28�� 14:24:10
* Purpose: Declaration of the class XImage
***********************************************************************/

#if !defined(__Hall_XImage_h)
#define __Hall_XImage_h

#include "CommonDefines.h"
#include <Windows.h>
#include <tchar.h>
#include <set>

class X3DEngine;
class XTexture;
class XImageContainer;
class ImageChange_t_Greater;

/// XImage�࣬�����ṩ��UIģ��ʹ�õĽӿ�
class GRAPH_API XImage
{
	friend class ImageChange_t_Greater;
	friend class XImageContainer;
	friend class X3DEngine;
public:
	/// ���캯��
	XImage();
	/// ��������
	virtual ~XImage();
	/// �������캯��
	XImage(const XImage& oldXImage);
	/// ���ظ�ֵ������
	XImage &operator= (const XImage& oldXImage);
	/// ����������ʱ��ʼ���������������
	int LoadFromFile(TCHAR *szFileName);
	/// ����������ʱ��ʼ���������������
	/// @param pData ָ���ļ����ݵ�ָ��
	/// @param nSize pDataָ����ָ�ڴ��У����ڱ��ļ����ݵĴ�С����λΪBYTE
	/// @param szIdentifiedName ����ʶ�����������Ƿ���ͬ������ַ���
	int LoadFromFileInMemory(const void *pData, int nSize, const TCHAR *szIdentifiedName);
    /// ��������������ɾ�����ؽ�
    int LoadTextureFrom32Bitmap(const void *pData, int nWidth, int nHeight, const TCHAR *szIdentifiedName);
	/// ���ø�ͼƬ�������е����꣬�������������ⲿ����
	int SetPosInTexture(int tx, int ty, int tw, int th);
	/// ���ø�ͼƬ�ڴ�������ʾ������
	int SetPosition(int x, int y, int w, int h, int z);
	/// ���ø�ͼƬ�ķ���
	int SetDir(int direction);
	/// �������ĳߴ�
	int GetTextureSize(int &w, int &h);
	/// ����ͼƬ�ĸ��������ɫ�������ϣ����ϣ����¡����µĴ���
	int SetColor(DWORD c1, DWORD c2, DWORD c3, DWORD c4);
	/// ��ʾ������ͼƬ
	void ShowHide(bool bShow = true);
	/// �ύ֮ǰ�����ĸ���
	void CommitChanges();
	/// ���òü���Χ
	void SetClipBox(short l, short r, short t, short b){m_sClipBox[0]=l; m_sClipBox[1]=r;m_sClipBox[2]=t; m_sClipBox[3]=b;}
    /// ���þ���
    void SetCrossMirror(bool bMirror){m_bCorssMirror = bMirror;}
    void SetVerticalMirror(bool bMirror){m_bVerticalMirror = bMirror;}
protected:
private:
	int m_x, m_y, m_w, m_h;		///< ͼƬ�ڴ����е�λ��
	int m_z;					///< ͼƬ�Ĳ�Σ�ǰ���ϵ��ԽСԽ���ϲ�
	int m_tx, m_ty, m_tw, m_th;	///< ͼƬ�������е�λ��
	int m_direction;			///< ͼƬ�ķ��򣬵�λΪ��
	DWORD m_color[4];			///< ͼƬ��ʾʱ�õ��ĸ��������ɫ
	short m_sClipBox[4];		///< �ü���Χ��0,1,2,3��Ӧ���ң��ϣ���
    
    bool m_bCorssMirror;             /// �Ƿ���
    bool m_bVerticalMirror;          /// �Ƿ���

	XTexture* m_pTexture;		///< ��ʹ�õ�����

	static XImageContainer* sm_pImageContainer;	///< ͼƬ����
};

/// Ԫ���޸�״����¼���������仯ʱ���豸������Ҫ�����Ѿ��������ı仯
/// ��ĳͼƬ��������״̬ʱ������Ҫд���Դ�
struct ImageChange_t
{
	XImage * _pImage;		///< �޸ĵ�Ԫ��
	bool	_bHide;			///< �޸��Ƿ�Ϊ����

	ImageChange_t(){_pImage = NULL; _bHide = false;}
	ImageChange_t(XImage *pImage){_pImage = pImage; _bHide = false;}
	bool operator==(const ImageChange_t rhs){return this->_pImage == rhs._pImage;}
};
/// �������������ʹ�õĽڵ�������������
class ImageChange_t_Greater : public std::binary_function<const ImageChange_t &, const ImageChange_t &, bool>
{
public:
	bool operator()(const ImageChange_t &lhs, const ImageChange_t &rhs)
	{
		if (lhs._pImage->m_z == rhs._pImage->m_z)
		{
			if (lhs._pImage->m_y == rhs._pImage->m_y)
			{
				if (lhs._pImage->m_x == rhs._pImage->m_x)
				{
					if (lhs._pImage->m_pTexture == rhs._pImage->m_pTexture)
					{
						if (lhs._pImage->m_tx == rhs._pImage->m_tx)
						{
							if (lhs._pImage->m_ty == rhs._pImage->m_ty)
							{
								if (lhs._pImage->m_tw == rhs._pImage->m_tw)
								{
									return lhs._pImage->m_pTexture >rhs._pImage->m_pTexture;
								}
								return (lhs._pImage->m_tw > rhs._pImage->m_tw);
							}
							return (lhs._pImage->m_ty > rhs._pImage->m_ty);
						}
						return lhs._pImage->m_tx > rhs._pImage->m_tx;
					}
					return (lhs._pImage->m_pTexture >rhs._pImage->m_pTexture);
				}
				return (lhs._pImage->m_x > rhs._pImage->m_x);
			}
			return (lhs._pImage->m_y > rhs._pImage->m_y);
		}
		else
		{
			return (lhs._pImage->m_z > rhs._pImage->m_z);
		}
	}
};
#endif
