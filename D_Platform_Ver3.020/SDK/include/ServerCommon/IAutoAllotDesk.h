#pragma once
#include <Windows.h>


class CGameUserInfo;
struct MSG_GM_QueueUserList;

interface IAutoAllotDesk
{
public:
	virtual void Add(CGameUserInfo* pGameUserInfo) = 0;
	virtual void Delete(CGameUserInfo* pGameUserInfo) = 0;
	virtual void DeleteAll() = 0;
	virtual void ReQueue(CGameUserInfo* pGameUserInfo) = 0;
	virtual void AllotDesk(int& iDeskCount, int* &arrDesk) = 0;
	virtual void GetQueueUserList(MSG_GM_QueueUserList * pList, int iCount) = 0;
	virtual size_t GetQueueUserCount() = 0;
	virtual void SetUserEnterTime(int iUserID, __int64 iTime) = 0;
	virtual bool Find(int iUserID) = 0;
	virtual bool IsAllUserStand() = 0;
};

/// �����Զ���������
/// @param iDeskPeopleCount[in] ÿ������
/// @param iDeskCount[in] �ܹ��е�������Ŀ
/// @return �����Ľӿ�ָ��
extern IAutoAllotDesk *CreateAutoAllotDesk(int iDeskPeopleCount, int iDeskCount);

/// ɾ���Զ���������
/// @param ppIAutoAllotDesk[in, out] ��CreateAutoAllotDesk���������Ľӿ�ָ�룬ɾ������NULL
extern void DestroyAutoAllotDesk(IAutoAllotDesk *&rpIAutoAllotDesk);

//extern IAutoAllotDesk *CreateAutoAllotDeskNew(int iDeskPeopleCount, int iDeskCount);
/// ɾ���Զ���������
extern void DestroyAutoAllotDeskNew(IAutoAllotDesk *&rpIAutoAllotDesk);