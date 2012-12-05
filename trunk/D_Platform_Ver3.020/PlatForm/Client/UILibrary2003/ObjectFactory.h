#pragma once
#ifndef OBJECTFACTORY_HEAD_H
#define OBJECTFACTORY_HEAD_H
#include "IGif.h"

// {29B1F315-ECC3-4535-A898-94DAF7B308AA}
const GUID IID_IGif = {0x29b1f315, 0xecc3, 0x4535, {0xa8, 0x98, 0x94, 0xda, 0xf7, 0xb3, 0x8, 0xaa}};

const GUID CLSID_IGif = {0xb568bd34, 0xcc78, 0x4623, {0xb7, 0x98, 0x10, 0xca, 0x7d, 0x1c, 0x82, 0x88}};


class EXT_CLASS CObjectFactory
{
public:
	CObjectFactory(void);

	virtual ~CObjectFactory(void);

public:
	// ����		��	��ʹ��CObjectFactoryǰ,��ʼ������Ҫ��Դ,ֻ��Ҫ����һ��;
	static HRESULT InitRes();

	// ����		��	�ͷ�ռ����Դ,�ڲ�ʹ�øÿ�ʱ����,ע�ⲻ���ڲ�ʹ�ø���ʱ;
	static HRESULT UninitRes();

public:
	// ����		��	����GUIDΪriid����Ķ���
	// ����1	��	Ԥ����
	// ����2	��	���������GUID��ǣ�
	// ����3	��	��������Ķ���GUID��ǣ�
	// ����		��	��IUnknown::CreateInstance
	virtual HRESULT CreateInstance(IUnknown * pUnkOuter, REFIID riid, void ** ppvObject);
};

#endif