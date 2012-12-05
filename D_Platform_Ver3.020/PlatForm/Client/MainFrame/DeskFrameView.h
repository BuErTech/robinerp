#pragma once
#ifndef DESKFRAMEVIEW_HEAD_H
#define DESKFRAMEVIEW_HEAD_H
#include "GamePlace.h"
class CGameRoomEx;
//#include "PersonalInfo.h"

#include <string>
using namespace std;

#define		IDR_CLICKDESK					1							//������Ӽ�ʱ��
#define		TIME_QUEUE						2							//������Ӽ�ʱ��

//�궨��
#define		YH_DESK_FACE					1							//Զ������
#define		EX_DESK_FACE					2							//��չ����
#define		BJL_DESK_FACE					3							//�ټ������� add by wlr 20090713
#define		QUEUE_DESK_FACE					4							//�Ŷӻ��������

#define		HUNDRED_PEOPLE_DESK_GAME_NAME   10301800					//�ټ�������Ϸ��� add by ramon
#define		SUOHA_PEOPLE_DESK_GAME_NAME		30501800					//�ٱ��������ID
#define		NIUNIU_PEOPLE_DESK_GAME_NAME	10901800					//�ٱ�ţţID
#define		ERZHANGPAI_PEOPLE_DESK_GAME_NAME	11901800				//�ٱ������ID

#define		WM_LEFT_MOUSE_HIT_DESK			WM_USER+214					//������Ϸ����
#define		WM_RIGHT_MOUSE_HIT_DESK			WM_USER+215					//������Ϸ����
#define		WM_HIT_DESK_BEGIN				WM_USER+216					//����ʼ

#define		WM_HIT_BJL_BEGIN				WM_USER+217					//�ټ��ְ���ʼ add by wlr 20090713

#define		WM_HIT_QUEUE_STOP				WM_USER+218					//�ټ��ְ���ʼ add by wlr 20090713

template	<UINT uPeopleCount, BYTE bDeskType> class CDesk;

typedef		CDesk<1,YH_DESK_FACE>	CDesk1;
typedef		CDesk<2,YH_DESK_FACE>	CDesk2;
typedef		CDesk<3,YH_DESK_FACE>	CDesk3;
typedef		CDesk<4,YH_DESK_FACE>	CDesk4;
typedef		CDesk<5,YH_DESK_FACE>	CDesk5;
typedef		CDesk<6,YH_DESK_FACE>	CDesk6;
typedef		CDesk<7,YH_DESK_FACE>	CDesk7;
typedef		CDesk<8,YH_DESK_FACE>	CDesk8;
typedef		CDesk<16,YH_DESK_FACE>	CDesk16;

typedef		CDesk<2,EX_DESK_FACE>	CDeskEx2;
typedef		CDesk<3,EX_DESK_FACE>	CDeskEx3;
typedef		CDesk<4,EX_DESK_FACE>	CDeskEx4;
typedef		CDesk<5,EX_DESK_FACE>	CDeskEx5;
typedef		CDesk<6,EX_DESK_FACE>	CDeskEx6;
typedef		CDesk<7,EX_DESK_FACE>	CDeskEx7;
typedef		CDesk<8,EX_DESK_FACE>	CDeskEx8;
typedef		CDesk<16,EX_DESK_FACE>	CDeskEx16;

typedef		CDesk<180,BJL_DESK_FACE>	CDeskBJL;//�ټ��� add by wlr 20090713

class CDeskFrameView;

//Avatarϵͳ���
#define BzGetAvatarIndex(obj) \
	GetAvatarIndexInRoom((obj)->GameUserInfo.bLogoID,(obj)->GameUserInfo.i64Bank,(obj)->GameUserInfo.i64Money,\
	(obj)->GameUserInfo.dwPoint,m_uGameNameID,m_uComType)

/*******************************************************************************************************/

//�����û���Ϣ�ṹ
struct MatchUserInfo
{
	long int					dwUserID;								//�û� ID
	TCHAR						szUserName[61];							//�û�����
};

/*******************************************************************************************************/

//��Ϸ��������
class CBaseDeskView
{
	//��Ԫ����
	friend class CDeskFrameView;

	//��������
protected:
	bool						m_bLock;								//�Ƿ���
	bool						m_bPlayGame;							//�Ƿ���Ϸ
	bool						m_bDownMouse;							//�Ƿ������
	bool						m_bFocusFrame;							//������
	bool						m_bBasePoint;							//�Ƿ����ñ���
	int							m_iBasePoint;							//������С
	BYTE						m_bDeskIndex;							//��Ϸ��������
	BYTE						m_bHoverIndex;							//�������
	UserItemStruct				* m_pUserItem[MAX_PEOPLE];				//�û�����

	Font						* m_pFont;								// ������ʾ����

	bool					m_bCanInput;								//�������� 


	//������Ϣ
protected:
	MatchUserInfo				m_MatchInfo[MAX_PEOPLE];				//������Ϣ

public:

	CString						m_deskClass;							//����ͼƬĿ¼
	bool						m_deskFlat;								//����ͷ���Ƿ�����չ��ƽ��ģ���m_deskClass=""ʱ��ǿ��=1

	UINT						m_uComType;								//��Ϸ����
	static UINT					m_uGameNameID;							//��ϷID
	//��������
public:
	//���캯��
	CBaseDeskView();
	//��������
	virtual ~CBaseDeskView();

	//���ܺ���
public:

	//��������
	virtual BYTE MouseHitTest(POINT MousePoint)=0;
	//�滭���溯��
	//virtual bool DrawViewFace(CDC * pDC, CImage * pBackImage, CImage * pLockImage, CImage * pBeginImage, CImage * pAgreeImage, COLORREF crBackColor)=0;
	virtual bool DrawViewFace(CDC * pDC, CImage * pBackImage, CImage * pLockImage, CImage * pBasePointImage, CImage * pBeginImage, CImage * pAgreeImage,
		UINT TMLChairCount,UINT GameRule,UINT GameNameID,COLORREF clrSitDeskFrameLeftTop,COLORREF clrSitDeskFrameRightBottom)=0;
	virtual bool DrawViewFacePng(Graphics &graphics, const CRect &DeskRect, Image *pAgree, Image *pLock, bool bIsVip = false)=0;

	//���ܺ���
public:
	//��ʼ������
	static bool InitDesk(CString deskClass,bool deskFloat,UINT uNameID, bool bIsVip = false);
	//������Ϸ��������
	void SetDeskIndex(BYTE bDeskIndex) { m_bDeskIndex=bDeskIndex; };
	void SetDeskClass(CString deskClass,bool deskFlat){m_deskClass=deskClass;m_deskFlat=deskFlat;};
	//������Ϸ����
	void SetComType(int uComType) { m_uComType=uComType; };
	//����Ĭ����Ϸ��
	static bool LoadDeskImage(BYTE bEveryCount, CGameImage & AFCDeskImage, BYTE bDeskType,CString deskClass,bool deskFlat,UINT uNameID);

	//���ܺ��� 
public:
	//�Ƿ��������
	void  SetCanInput(bool bCanInput) {m_bCanInput = bCanInput;};
	//�Ƿ���Ϸ
	bool IsPlayGame(void) { return m_bPlayGame; };
	//�Ƿ��˶�ͬ��
	virtual bool IsAllAgree(void);
	//ͬ������
	virtual BYTE CountAgree(void);
	//�Ƿ���������
	bool IsSetPass(void) { return m_bLock; }
	//�õ����ӱ���
	int GetBasePoint(void) { return m_iBasePoint;}
	//���ÿ��
	void SetShowFrame(bool bFocusFrame, CDeskFrameView * pDeskFrame);
	//������Ϸ���ӱ���
	void SetBasePoint(bool bSetBasePoint, int iBasePoint, CDeskFrameView * pDeskFrame);
	//��������
	void SetHoverIndex(BYTE bHoverIndex, CDeskFrameView * pDeskFrame);
	//���ð���
	void SetMouseDown(bool bMouseDown, CDeskFrameView * pDeskFrame);
	//��������־
	void SetLock(bool bLock, CDeskFrameView * pDeskFrame);
	//���ÿ�ʼ��־
	void SetPlayGame(bool bPlaying, CDeskFrameView * pDeskFrame);
	//�����û���Ϣ
	bool SetUserItem(BYTE bDeskStation, UserItemStruct * pUserItem, CDeskFrameView * pDeskFrame);
	//���ñ�����Ϣ
	bool SetMatchInfo(BYTE bDeskStation, long int dwUserID, TCHAR * szUserName, CDeskFrameView * pDeskFrame);
	//��ȡ�û���Ϣ
	UserItemStruct * GetUserItem(BYTE bDeskStation);
	//��ȡ������Ϣ
	long int GetMatchInfo(BYTE bDeskStation);
};

/*******************************************************************************************************/

//��Ϸ����
template <UINT uPeopleCount, BYTE bDeskType> class CDesk : public CBaseDeskView
{
	//��������:
public:
	static CPoint				m_PtLock;								//��λ��
	static CPoint				m_PtBegin;								//��ʼλ��
	static CRect				m_IndexRect;							//��Ϸ������
	static CPoint				m_PtBasePoint;							//���ӱ���λ��
	static CPoint				m_PtAgree[uPeopleCount];				//ͬ��λ��
	static CRect				m_NameRect[uPeopleCount];				//���ֿ��
	static CRect				m_ChairRect[uPeopleCount];				//����λ��
	static UINT					m_TextFormat[uPeopleCount];				//��ʽ����
	static CPoint				m_PtLog[uPeopleCount];					//����λ��
	static CPoint				m_BtnAgree[uPeopleCount];				//ͬ��λ��
	static int					m_draw[uPeopleCount];					//��ʾ˳��
	static Color				m_UserNameColor;					///< �����
	static Color				m_VipNameColor;						///< VIP�����
	static Color				m_DeskNoColor;						///< ����������ɫ
	static Color				m_FocusColor;						///< �������ɫ

	//��������
public:
	//���캯��
	CDesk(void) {};
	//��������
	virtual ~CDesk(void) {};

	//���غ���
public:
	//��������
	virtual BYTE MouseHitTest(POINT MousePoint)
	{
		if (m_deskClass == "queue" && !m_bCanInput) return 255;
		for (UINT i=0;i<uPeopleCount;i++)
		{
			if (m_ChairRect[i].PtInRect(MousePoint)) return i;
		}
		return 255;
	};
	virtual bool IsAllAgree(void)
	{
		for (UINT i=0;i<uPeopleCount;i++)///��ǰ���ӣ��������ӵ��˵�ѭ��
		{
			if (m_pUserItem[i]!=NULL)
			{
				if(m_pUserItem[i]->GameUserInfo.bUserState!=USER_DESK_AGREE&&
					m_pUserItem[i]->GameUserInfo.bUserState!=USER_ARGEE&&
					m_pUserItem[i]->GameUserInfo.bUserState!=USER_CUT_GAME&&
					m_pUserItem[i]->GameUserInfo.bUserState!=USER_PLAY_GAME)
					return false;
			}else return false;
		}
		return true;
	};
	//ͬ������
	virtual BYTE CountAgree(void)
	{
		int count = 0;
		for (UINT i=0;i<uPeopleCount;i++)///��ǰ���ӣ��������ӵ��˵�ѭ��
		{
			if (m_pUserItem[i]!=NULL)
			{
				if(m_pUserItem[i]->GameUserInfo.bUserState == USER_DESK_AGREE)
					count ++;
				/*if(m_pUserItem[i]->GameUserInfo.bUserState!=USER_DESK_AGREE&&
				m_pUserItem[i]->GameUserInfo.bUserState!=USER_ARGEE&&
				m_pUserItem[i]->GameUserInfo.bUserState!=USER_CUT_GAME&&
				m_pUserItem[i]->GameUserInfo.bUserState!=USER_PLAY_GAME)
				return false;*/
			}
		}
		return count;
	};

	/// �������ϵ�����ͬ���־����������ǳƵ��������ӵ�����
	virtual bool DrawViewFacePng(Graphics &graphics, const CRect &DeskRect, Image *pAgree, Image *pLock, bool bIsVip = false)
	{

		//return  DrawViewFacePngQueue(graphics, DeskRect, pAgree, pLock, bIsVip);
		bool isneedpeople=false;
		StringFormat sf;
		SolidBrush brush(Color(255,0,0));
		//Font* font = new Font(L"����", 12, FontStyleRegular);
		RectF rcF;

		if (uPeopleCount>3)
		{
			if (m_pUserItem[3]!=NULL)
			{
				UINT ChairIndex=3; 
				int iSitGrade = 0;
				//iSitGrade = GetMoneyGrade(m_pUserItem[3]->GameUserInfo.i64Money);
				iSitGrade = BzGetAvatarIndex(m_pUserItem[3]);

				TCHAR   szLogoPath[MAX_PATH]={0};//ͼƬ�ļ���

				TCHAR   szLogoBKPath[MAX_PATH]={0};//ͼƬ�ļ���
				//CString strLogoPath;
				BOOL    bFileExist=false;//�ļ��Ƿ���ڣ�ȱʡΪ������
				CFileFind fileFind;	//�����ļ�
				int     uLogoID=m_pUserItem[3]->GameUserInfo.bLogoID;//Ϊ����Ĵ���ʡд

				if (m_deskClass == "2d" || m_deskClass == "queue")
				{
					wsprintf(szLogoPath,"image\\log\\%s\\gamelog%d_small.png",m_pUserItem[3]->GameUserInfo.bBoy?"boy":"girl",iSitGrade);
					wsprintf(szLogoBKPath,"image\\log\\LogoBk.png");
				}
				else
				{
					switch(uPeopleCount )
					{
					case 2:
						{
							if (bIsVip)
							{
								wsprintf(szLogoPath,"image\\log\\%s\\sitelog\\vip\\%s_%d.png",m_deskClass,m_pUserItem[3]->GameUserInfo.bBoy?"boy":"girl",ChairIndex);
							}
							else
							{
								wsprintf(szLogoPath,"image\\log\\%s\\sitelog2\\%s%d\\desk%dsit.png",m_deskClass,m_pUserItem[3]->GameUserInfo.bBoy?"boy":"girl",iSitGrade,ChairIndex);
							}
							break;
						}
					case 5:
						{
							wsprintf(szLogoPath,"image\\log\\%s\\sitelog5\\%s%d\\desk%dsit.png",m_deskClass,m_pUserItem[3]->GameUserInfo.bBoy?"boy":"girl",0,ChairIndex);
							break;
						}
					case 6:
						{
							wsprintf(szLogoPath,"image\\log\\%s\\sitelog6\\%s%d\\desk%dsit.png",m_deskClass,m_pUserItem[3]->GameUserInfo.bBoy?"boy":"girl",0,ChairIndex);
							break;
						}
					case 8:
						{
							wsprintf(szLogoPath,"image\\log\\%s\\sitelog8\\%s%d\\desk%dsit.png",m_deskClass,m_pUserItem[3]->GameUserInfo.bBoy?"boy":"girl",0,ChairIndex);
							break;
						}
					default:
						{
							wsprintf(szLogoPath,"image\\log\\%s\\sitelog\\%s%d\\desk%dsit.png",m_deskClass,m_pUserItem[3]->GameUserInfo.bBoy?"boy":"girl",iSitGrade,ChairIndex);
							break;
						}
					}
				}
				

				
				Image *iLogo;
				Image *iLogoBk;
#ifdef BZ_ZLIB ///add by wxx

				CUnZipRes unZipRes;
				IStream * pStream=NULL;

				//CString strFileName(szLogoPath);
				//int pos=strFileName.Find("image");
				//strFileName=strFileName.Right(strFileName.GetLength()-pos);

				//TCHAR * pchar=strFileName.GetBuffer(strFileName.GetLength() + 1);

				pStream=unZipRes.LoadFileResFromZip(szLogoPath);
				iLogo=new Image(pStream);
				if(pStream!=NULL)
				{
					pStream->Release();
					pStream=NULL;
				}

				pStream=unZipRes.LoadFileResFromZip(szLogoBKPath);
				iLogoBk=new Image(pStream);
				if(pStream!=NULL)
				{
					pStream->Release();
					pStream=NULL;
				}
#else
				iLogo=Image::FromFile(CA2W(szLogoPath));
				iLogoBk=Image::FromFile(CA2W(szLogoBKPath));
#endif
				///iLogo=Image::FromFile(CA2W(szLogoPath));

				int x = m_PtLog[3].x;
				int y = m_PtLog[3].y;

				if (m_deskClass == "queue")
				{
					graphics.DrawImage(iLogoBk, DeskRect.left + x, DeskRect.top + y, iLogoBk->GetWidth(),iLogoBk->GetHeight());
					int _x = (iLogoBk->GetWidth() - iLogo->GetWidth()) / 2;
					int _y = (iLogoBk->GetHeight() - iLogo->GetHeight()) / 2;
					graphics.DrawImage(iLogo, DeskRect.left + x + _x, DeskRect.top + y + _y, iLogo->GetWidth(),iLogo->GetHeight());
				}
				else
				{
					graphics.DrawImage(iLogo, DeskRect.left + x, DeskRect.top + y, iLogo->GetWidth(),iLogo->GetHeight());
				}

				delete iLogo;
				delete iLogoBk;
			}
		}

		int peopleCnt = 0; ///< �鿴һ�������ж��ٸ��ˣ����û�����򲻻ử��������ͼ��

		for (UINT i=0;i<uPeopleCount;i++)///��ǰ���ӣ��������ӵ��˵�ѭ��
		{
			int j=i;//m_draw[i];
			//�滭�û���Ϣ
			if (m_pUserItem[j]!=NULL)
			{
				isneedpeople=true;
				UINT ChairIndex=j; 
				if (ChairIndex==1&&uPeopleCount==2)
				{//2��
					ChairIndex=2;
				}
				
				peopleCnt++;
				//WCHAR	m_wszBuffer[100]={0};
				int iSitGrade = 0;
				//iSitGrade = GetMoneyGrade(m_pUserItem[j]->GameUserInfo.i64Money);
				iSitGrade = BzGetAvatarIndex(m_pUserItem[j]);

				TCHAR   szLogoPath[MAX_PATH]={0};//ͼƬ�ļ���
				TCHAR   szLogoBKPath[MAX_PATH]={0};//ͼƬ�ļ���
				//CString strLogoPath;
				BOOL    bFileExist=false;//�ļ��Ƿ���ڣ�ȱʡΪ������
				CFileFind fileFind;	//�����ļ�
				int     uLogoID=m_pUserItem[j]->GameUserInfo.bLogoID;//Ϊ����Ĵ���ʡд
				if (m_deskClass == "2d" || m_deskClass == "queue")
				{
					wsprintf(szLogoPath,"image\\log\\%s\\gamelog%d_small.png",m_pUserItem[j]->GameUserInfo.bBoy?"boy":"girl", iSitGrade);
										
					wsprintf(szLogoBKPath,"image\\log\\LogoBk.png", "2d");
				}
				else
				{
					switch(uPeopleCount )
					{
					case 2:
						{
							if (bIsVip)
							{
								wsprintf(szLogoPath,"image\\log\\%s\\sitelog\\vip\\%s_%d.png",m_deskClass,m_pUserItem[j]->GameUserInfo.bBoy?"boy":"girl",ChairIndex);
							}
							else
							{
								wsprintf(szLogoPath,"image\\log\\%s\\sitelog2\\%s%d\\desk%dsit.png",m_deskClass,m_pUserItem[j]->GameUserInfo.bBoy?"boy":"girl",iSitGrade,ChairIndex);
							}
							break;
						}
					case 5:
						{
							wsprintf(szLogoPath,"image\\log\\%s\\sitelog5\\%s%d\\desk%dsit.png",m_deskClass,m_pUserItem[j]->GameUserInfo.bBoy?"boy":"girl",0,ChairIndex);
							break;
						}
					case 6:
						{
							wsprintf(szLogoPath,"image\\log\\%s\\sitelog6\\%s%d\\desk%dsit.png",m_deskClass,m_pUserItem[j]->GameUserInfo.bBoy?"boy":"girl",0,ChairIndex);
							break;
						}
					case 8:
						{

							wsprintf(szLogoPath,"image\\log\\%s\\sitelog8\\%s%d\\desk%dsit.png",m_deskClass,m_pUserItem[j]->GameUserInfo.bBoy?"boy":"girl",0,ChairIndex);
							break;
						}
					default:
						{
							wsprintf(szLogoPath,"image\\log\\%s\\sitelog\\%s%d\\desk%dsit.png",m_deskClass,m_pUserItem[j]->GameUserInfo.bBoy?"boy":"girl",iSitGrade,ChairIndex);
							break;
						}
					}
				}


				
				//if(uPeopleCount != 2)
				//{//�Ƿ����������͵�ͷ��ͨ��DESK CLASSĿ¼��Ѱ��
				//	wsprintf(szLogoPath,"image\\log\\%s\\sitelog\\%s%d\\desk%dsit.png",m_deskClass,m_pUserItem[j]->GameUserInfo.bBoy?"boy":"girl",iSitGrade,ChairIndex);
				//}
				//else
				//{
				//	if (bIsVip)
				//	{
				//		wsprintf(szLogoPath,"image\\log\\%s\\sitelog\\vip\\%s_%d.png",m_deskClass,m_pUserItem[j]->GameUserInfo.bBoy?"boy":"girl",ChairIndex);
				//	}
				//	else
				//	{
				//		wsprintf(szLogoPath,"image\\log\\%s\\sitelog\\%s%d\\desk%dsit.png",m_deskClass,m_pUserItem[j]->GameUserInfo.bBoy?"boy":"girl",iSitGrade,ChairIndex);
				//	}
				//}

				if (j != 3 )
				{
					Image *iLogo;
					Image *iLogoBk;

#ifdef BZ_ZLIB ///add by wxx

					CUnZipRes unZipRes;
					IStream * pStream=NULL;

					//CString strFileName(szLogoPath);
					//int pos=strFileName.Find("image");
					//strFileName=strFileName.Right(strFileName.GetLength()-pos);

					//TCHAR * pchar=strFileName.GetBuffer(strFileName.GetLength() + 1);

					pStream=unZipRes.LoadFileResFromZip(szLogoPath);
					iLogo=new Image(pStream);
					if(pStream!=NULL)
					{
						pStream->Release();
						pStream=NULL;
					}

					pStream=unZipRes.LoadFileResFromZip(szLogoBKPath);
					iLogoBk=new Image(pStream);
					if(pStream!=NULL)
					{
						pStream->Release();
						pStream=NULL;
					}
#else
					iLogo=Image::FromFile(CA2W(szLogoPath));
					iLogoBk=Image::FromFile(CA2W(szLogoBKPath));
#endif
					//iLogo=Image::FromFile(CA2W(szLogoPath));

					int x = m_PtLog[j].x;
					int y = m_PtLog[j].y;

					if (m_deskClass == "queue")
					{
						graphics.DrawImage(iLogoBk, DeskRect.left + x, DeskRect.top + y, iLogoBk->GetWidth(),iLogoBk->GetHeight());
						int _x = (iLogoBk->GetWidth() - iLogo->GetWidth()) / 2;
						int _y = (iLogoBk->GetHeight() - iLogo->GetHeight()) / 2;
						graphics.DrawImage(iLogo, DeskRect.left + x + _x, DeskRect.top + y + _y, iLogo->GetWidth(),iLogo->GetHeight());
					}
					else
					{
						graphics.DrawImage(iLogo, DeskRect.left + x, DeskRect.top + y, iLogo->GetWidth(),iLogo->GetHeight());
					}

					delete iLogo;
					delete iLogoBk;
				}

				sf.SetTrimming(StringTrimmingEllipsisWord);

				rcF.X = m_NameRect[j].left + DeskRect.left;
				rcF.Y = m_NameRect[j].top + DeskRect.top;
				rcF.Width = m_NameRect[j].right - m_NameRect[j].left;
				rcF.Height = m_NameRect[j].bottom - m_NameRect[j].top;
				brush.SetColor(GetNameColor(&m_pUserItem[j]->GameUserInfo));
				graphics.DrawString(CA2W(m_pUserItem[j]->GameUserInfo.nickName), wcslen(CA2W(m_pUserItem[j]->GameUserInfo.nickName)),
					m_pFont, rcF, &sf, &brush);

				

			}
		}
		//�滭����,���ӵĺ���m_bDeskIndex
		TCHAR szBuffer[20];
		memset(szBuffer,0,sizeof(szBuffer));
		wsprintf(szBuffer,TEXT("%d"),m_bDeskIndex+1);
		brush.SetColor(m_DeskNoColor);


		//�滭ͬ���־pAgree
		if (bDeskType==YH_DESK_FACE)
		{
			if (m_bPlayGame==false)	// �Ѿ���ʼ�Ͳ��û�ͬ���־��
			{
				for (UINT i=0;i<uPeopleCount;i++)
				{
					if ((m_pUserItem[i]!=NULL)&&(pAgree!=NULL)&&
						(m_pUserItem[i]->GameUserInfo.bUserState==USER_ARGEE||m_pUserItem[i]->GameUserInfo.bUserState==USER_DESK_AGREE))
					{
						//�滭͸��ͼ
						graphics.DrawImage(pAgree, DeskRect.left + m_PtAgree[i].x, DeskRect.top + m_PtAgree[i].y, pAgree->GetWidth(), pAgree->GetHeight());
					}
				}
			}
		}

		// ������Ͼ��ε�λ��

		rcF.X = m_IndexRect.left + DeskRect.left;
		rcF.Y = m_IndexRect.top + DeskRect.top;
		rcF.Width = m_IndexRect.right - m_IndexRect.left;
		rcF.Height = m_IndexRect.bottom - m_IndexRect.top;

		if (m_deskClass != "queue")
		{
			graphics.DrawString(CA2W(szBuffer), lstrlen(szBuffer),
				m_pFont, rcF, &sf, &brush);
		}

		if (0 == peopleCnt)
			m_bLock = false;
		//�滭��m_bLock
		if ((bDeskType==YH_DESK_FACE) && (m_bLock==true) && (pLock!=NULL))
		{
			graphics.DrawImage(pLock,
				DeskRect.left + m_PtLock.x,
				DeskRect.top + m_PtLock.y,
				pLock->GetWidth(),
				pLock->GetHeight());
		}

		//�滭�����
		if (m_bFocusFrame&&m_uComType!=TY_MATCH_GAME)
		{
			brush.SetColor(m_FocusColor);
			Pen pen(&brush, 0.5);
			int nLeft = DeskRect.left+2;
			int nTop = DeskRect.top+2;
			int nRight = DeskRect.right-2;
			int nBottom = DeskRect.bottom-2;
			graphics.DrawLine(&pen, nLeft, nTop, nRight, nTop);
			graphics.DrawLine(&pen, nRight, nTop, nRight, nBottom);
			graphics.DrawLine(&pen, nRight, nBottom, nLeft, nBottom);
			graphics.DrawLine(&pen, nLeft, nTop, nLeft, nBottom);

		}
		return true;
	}

	//�滭���溯��
	virtual bool DrawViewFace(CDC * pDC, CImage * pBackImage, CImage * pLockImage, CImage * pBasePointImage, CImage * pBeginImage, CImage * pAgreeImage, 
		UINT TMLChairCount,UINT GameRule,UINT GameNameID,COLORREF clrSitDeskFrameLeftTop,COLORREF clrSitDeskFrameRightBottom)
	{
		bool isneedpeople=false ;
		//�滭����
		if (m_bPlayGame)
			pBeginImage->BitBlt(pDC->GetSafeHdc(),0,0);
		else
			pBackImage->BitBlt(pDC->GetSafeHdc(),0,0);

		if (bDeskType==BJL_DESK_FACE) return true;//�ټ������� add by wlr 20090713 


		for (UINT i=0;i<uPeopleCount;i++)///��ǰ���ӣ��������ӵ��˵�ѭ��
		{
			int j=i;//m_draw[i];
			//�滭�û���Ϣ
			if (m_pUserItem[j]!=NULL)
			{
				isneedpeople=true;
				UINT ChairIndex=j; 
				if (ChairIndex==1&&uPeopleCount==2)
				{//2��
					ChairIndex=2;
				}

				//WCHAR	m_wszBuffer[100]={0};
				int iSitGrade = 0;
				//iSitGrade = GetMoneyGrade(m_pUserItem[j]->GameUserInfo.i64Money);
				iSitGrade = BzGetAvatarIndex(m_pUserItem[j]);

				TCHAR   szLogoPath[MAX_PATH]={0};//ͼƬ�ļ���
				//CString strLogoPath;
				BOOL    bFileExist=false;//�ļ��Ƿ���ڣ�ȱʡΪ������
				CFileFind fileFind;	//�����ļ�
				int     uLogoID=m_pUserItem[j]->GameUserInfo.bLogoID;//Ϊ����Ĵ���ʡд

				if(m_deskClass!="" && !m_deskFlat)
				{//�Ƿ����������͵�ͷ��ͨ��DESK CLASSĿ¼��Ѱ��
					wsprintf(szLogoPath,"image\\log\\%s\\sitelog\\%s%d\\desk%dsit.png",m_deskClass,m_pUserItem[j]->GameUserInfo.bBoy?"boy":"girl",iSitGrade,ChairIndex);
					bFileExist=fileFind.FindFile(szLogoPath);
					fileFind.Close();
				}
				if(!bFileExist || m_deskFlat)
				{//���û���ҵ�DESK_CLASSĿ¼����ļ�������û������DESK CLASS
					if(uLogoID>=0xFF && uLogoID<0x200)
					{//����Ƿ����ϴ���ͷ�񣬲����ͷ���ļ��Ƿ����
						wsprintf(szLogoPath,"CustomFace\\%d.png",m_pUserItem[j]->GameUserInfo.dwUserID);
						bFileExist=fileFind.FindFile(szLogoPath);
						fileFind.Close();
					}
				}
				if(!bFileExist)
				{//���϶������ڣ�ʹ��Ĭ�ϵ�ͷ��
					//Ϊ�˼��������ƽ̨����ʹ�õ�uLogoID==0xFF�����Ե�LogoID==0xFFʱ��ǿ��=0������ȡLogoID%0x100��Ϊ��ʼ��ID��
					wsprintf(szLogoPath,"image\\log\\stand\\gamelog%d.bmp",uLogoID==0xFF?0:uLogoID%0x100);
				}
				
				Image *iLogo;
#ifdef BZ_ZLIB ///add by wxx

				CUnZipRes unZipRes;
				IStream * pStream=NULL;

				//CString strFileName(szLogoPath);
				//int pos=strFileName.Find("image");
				//strFileName=strFileName.Right(strFileName.GetLength()-pos);

				//TCHAR * pchar=strFileName.GetBuffer(strFileName.GetLength() + 1);

				pStream=unZipRes.LoadFileResFromZip(szLogoPath);
				iLogo=new Image(pStream);
				if(pStream!=NULL)
				{
					pStream->Release();
					pStream=NULL;
				}
				
#else
				iLogo=Image::FromFile(CA2W(szLogoPath));
#endif
				///iLogo=Image::FromFile(CA2W(szLogoPath));
				Graphics graphics(pDC->GetSafeHdc()); // Create a GDI+ graphics object

				int x = m_PtLog[j].x;
				int y = m_PtLog[j].y;

				if(m_deskClass=="" || !bFileExist || m_deskFlat)
				{//��������չ����λ��Ҳû���ҵ���չ����λ��ʹ��ƽ������

					//ֻ��ƽ�����Ӳ���Ҫ�����ӱ���
					TCHAR szChair[MAX_PATH]={0};
					wsprintf(szChair,".\\%d\\image\\desk\\chair.bmp",GameNameID);
					if(fileFind.FindFile(szChair))
					{
						CGameImage img;
						img.SetLoadInfo(szChair,true);
						CGameImageHelper handle(&img);
						handle.BitBlt(pDC->GetSafeHdc(),m_ChairRect[j].left,m_ChairRect[j].top);
					}
					fileFind.Close();

					int logw=iLogo->GetWidth();
					int logh=iLogo->GetHeight();

					switch(uPeopleCount)
					{
					case 2:
						switch(j)
						{
						case 0:
							x=m_PtLog[j].x;
							y=m_PtLog[j].y-logh/2;
							break;
						case 1:
							x=m_PtLog[j].x-logw;
							y=m_PtLog[j].y-logh/2;
							break;
						default:
							break;
						}
						break;
					case 3:
						switch(j)
						{
						case 0:
							x=m_PtLog[j].x;
							y=m_PtLog[j].y-logh/2;
							break;
						case 1:
							x=m_PtLog[j].x-logw;
							y=m_PtLog[j].y-logh/2;
							break;
						case 2:
							x=m_PtLog[j].x-logw/2;
							y=m_PtLog[j].y-logh;
							break;
						default:
							break;
						}
						break;
					case 4:
						switch(j)
						{
						case 0:
							x=m_PtLog[j].x;
							y=m_PtLog[j].y-logh/2;
							break;
						case 1:
							x=m_PtLog[j].x-logw/2;
							y=m_PtLog[j].y;
							break;
						case 2:
							x=m_PtLog[j].x-logw;
							y=m_PtLog[j].y-logh/2;
							break;
						case 3:
							x=m_PtLog[j].x-logw/2;
							y=m_PtLog[j].y-logh;
							break;
						default:
							break;
						}
						break;
					case 5:
						switch(j)
						{
						case 0:
						case 1:
							x=m_PtLog[j].x;
							y=m_PtLog[j].y-logh/2;
							break;
						case 2:
						case 3:
							x=m_PtLog[j].x-logw;
							y=m_PtLog[j].y-logh/2;
							break;
						case 4:
							x=m_PtLog[j].x-logw/2;
							y=m_PtLog[j].y-logh;
							break;
						default:
							break;
						}
						break;
					case 6:
						switch(j)
						{
						case 0:
							x=m_PtLog[j].x;
							y=m_PtLog[j].y-logh;
							break;
						case 1:
							x=m_PtLog[j].x;
							y=m_PtLog[j].y;
							break;
						case 2:
							x=m_PtLog[j].x-logw/2;
							y=m_PtLog[j].y;
							break;
						case 3:
							x=m_PtLog[j].x-logw;
							y=m_PtLog[j].y;
							break;
						case 4:
							x=m_PtLog[j].x-logw;
							y=m_PtLog[j].y-logh;
							break;
						case 5:
							x=m_PtLog[j].x-logw/2;
							y=m_PtLog[j].y-logh;
							break;
						default:
							break;
						}

						break;
					default:
						break;
					}
				}
				graphics.DrawImage(iLogo, x, y, iLogo->GetWidth(),iLogo->GetHeight());

				delete iLogo;
				Color cl = GetNameColor(&m_pUserItem[j]->GameUserInfo);
				pDC->SetTextColor(RGB(cl.GetR(), cl.GetG(), cl.GetB()));//��ɫ
				//if(GameRule & GRR_AUTO_SIT_DESK)
				//	pDC->DrawText("���",lstrlen("���"),&m_NameRect[j],DT_VCENTER|DT_CENTER|DT_NOCLIP|DT_SINGLELINE);
				//else
				pDC->DrawText(m_pUserItem[j]->GameUserInfo.nickName,lstrlen(m_pUserItem[j]->GameUserInfo.nickName),&m_NameRect[j],DT_VCENTER|DT_CENTER|DT_NOCLIP|DT_SINGLELINE);

			}
		}
		//�滭ͬ���־m_PtAgree
		if (bDeskType==YH_DESK_FACE)
		{
			if (m_bPlayGame==false)
			{
				for (UINT i=0;i<uPeopleCount;i++)
				{
					if ((m_pUserItem[i]!=NULL)&&
						(m_pUserItem[i]->GameUserInfo.bUserState==USER_ARGEE||m_pUserItem[i]->GameUserInfo.bUserState==USER_DESK_AGREE))
					{//�滭͸��ͼ
						CDC destdc;
						destdc.CreateCompatibleDC(pDC);
						destdc.SelectObject(*pAgreeImage);
						::TransparentBlt(pDC->GetSafeHdc(),m_PtAgree[i].x,m_PtAgree[i].y,pAgreeImage->GetWidth(),pAgreeImage->GetHeight(),destdc.GetSafeHdc(),0,0,pAgreeImage->GetWidth(),pAgreeImage->GetHeight(),destdc.GetPixel(0,0));
						/// {{Added by zxd 20100709 �ͷ�GDI��Դ
						destdc.DeleteDC();
						/// Added by zxd 20100709 �ͷ�GDI��Դ}}
					}
				}
			}
			else 
			{//�滭��������Ϸ������m_PtBegin͸��ͼ///ȫ��ͬ�����ˣ��Ѿ���ʼ��Ϸ��
			}
		}

		//�滭����,���ӵĺ���m_bDeskIndex
		TCHAR szBuffer[20];
		memset(szBuffer,0,sizeof(szBuffer));
		wsprintf(szBuffer,TEXT("%d"),m_bDeskIndex+1);
		pDC->SetTextColor(RGB(255, 255, 0));//��ɫ
		pDC->DrawText(szBuffer,lstrlen(szBuffer),&m_IndexRect,DT_VCENTER|DT_CENTER|DT_NOCLIP|DT_SINGLELINE);

		//�滭��m_bLock
		if ((bDeskType==YH_DESK_FACE)&&(m_bLock==true)) 
			pLockImage->BitBlt(pDC->GetSafeHdc(),m_PtLock);

		//�滭�����
		if (m_bFocusFrame&&m_uComType!=TY_MATCH_GAME)
		{
			pDC->Draw3dRect(2,2,pBackImage->GetWidth()-2,pBackImage->GetHeight()-2,clrSitDeskFrameLeftTop,clrSitDeskFrameRightBottom);//��������ͼƬ�Ĵ�С���ƽ����

		}
		return true;
	}
	Color GetNameColor(UserInfoStruct *pUserInfo)
	{
		CString stime;
		stime.Format("%d",CTime::GetCurrentTime());        //��ǰ��ʱ�䣬String��ʽ
		int curtime=atoi(stime);                        //ת��Ϊint
		if(pUserInfo->iVipTime>curtime)
			return m_VipNameColor;
		return m_UserNameColor;
	}
};

/*******************************************************************************************************/

//��Ϸ���ӿ����
/*

*/
class CDeskFrameView : public CWnd
{
	Graphics						*m_pGraph;
	Bitmap							*m_pBackBmp;

	//��������
protected:
	//{{Added by zxd 20091124 ͸����ͼ
	Image	*m_pImgDesk;
	Image	*m_pImgLock;
	Image	*m_pImgTableBasePoint;
	Image	*m_pImgBegin;
	Image	*m_pImgAgree;
	BYTE	m_byDeskType;	/// �������ͣ��ǲ��ǰټ�������
	int		m_r1, m_g1, m_b1;	// ����ɫ�ĵ�һ��
	int		m_r2, m_g2, m_b2;	// ����ɫ�ĵڶ���
	//Added by zxd 20091124}}

	CToolTipCtrl				m_Tooltip;								//��ʾ������
	UINT						m_uClassSize;							//���С
	BYTE						m_bDeskCount;							//��Ϸ����Ŀ
	BYTE						m_bEveryCount;							//ÿ����Ϸ����
	HCURSOR						m_hMouseCursor;							//�����
	CGameImage					m_DeskImage;							//��Ϸ����ͼ
	CGameImage					m_LockImage;							//��Ϸ����ͼ
	CGameImage					m_TableBasePoint;						//��Ϸ��������ͼ
	CGameImage					m_BeginImage;							//��Ϸ��ʼͼ
	CGameImage					m_AgreeImage;							//��Ϸͬ��ͼ
	CBaseDeskView				* m_pDeskArray;							//��Ϸ������ָ��
	CNormalBitmapButtonEx		m_BeginAgree;							//ͬ����Ϸ��ť
	CNormalBitmapButtonEx		m_BJLBegin;								//�ټ��ֿ�ʼ��Ϸ��ť add by wlr 20090713
	CNormalBitmapButtonEx		m_BJLPlaying;							//�ټ�����Ϸ�����а�ť add by wlr 20090713

//�Ŷӻ�ר�� 
	CNormalBitmapButtonEx		m_QueueBegin;								//�Ŷӻ��Ŷ�
	CNormalBitmapButtonEx		m_QueueStop;								//�Ŷӻ��뿪
	bool						m_bQueueBegin;

	Image*						m_pImageQueueTip;
	Image*						m_pImageQueueTip1;			
	Image*						m_pImageQueueTip2;
	int							m_XTip, m_YTip;
	int							m_XTip1, m_YTip1;
	int							m_XDesk, m_YDesk;
	Image*						m_pImageQueueTime;
	Image*						m_pImageQueueplan;
	int							m_SumTime;
	int							m_CurTime;



	COLORREF					clrSitDeskFrameLeftTop;					//����������ʱ�����ߺ��ϱ���ɫ
	COLORREF					clrSitDeskFrameRightBottom;				//����������ʱ����ұߺ��±���ɫ

	int							m_BtnAgree0X,m_BtnAgree0Y;				//ͬ�ⰴťλ��
	int							m_BtnAgree1X,m_BtnAgree1Y;				//ͬ�ⰴťλ��
	int							m_BtnAgree2X,m_BtnAgree2Y;				//ͬ�ⰴťλ��
	int							m_BtnAgree3X,m_BtnAgree3Y;				//ͬ�ⰴťλ��

	//״̬��Ϣ
protected:
	bool						m_bLeftMouseDown;						//�Ƿ�����������
	bool						m_bRightMouseDown;						//�Ƿ�����Ҽ�����
	BYTE						m_bDownDeskIndex;						//������������
	BYTE						m_bDownDeskStation;						//����λ������

	BYTE                        m_bCurDownDesk;
	BYTE                        m_bCurDownStation;

	BYTE						m_bUserFrameIndex;						//�û��߿�����
	BYTE						m_bFindFrameIndex;						//���ұ߿�����
	int							m_uComType;								//��Ϸ����
	bool						m_bIsClick;								//���Ƶ�����ӵ�Ƶ��
	CString						m_deskClass;							//��������
	bool						m_deskFlat;
	long						m_UserID;								//�Լ����û�ID
	CPersonalInfo				m_personalInfo;							//��ʾ�����û���Ϣ�Ի���
public:
	GetOrderName				* pOrderName;							//��Ӧ����ȼ�����ָ��

	bool						m_bShowPlaying;							//�Ƿ���ʾ��Ϸ�����а�ť �ټ��� add by wlr 20090717

    // PengJiLin, 2011-6-23, ������Ϣ
    BOOL        m_bActive;

	//������Ϣ
protected:
	bool				m_bIsVip;									///< �Ƿ�VIP����
	int							m_iPos;									//��ǰλ��
	int							m_iMaxPos;								//���λ��
	int							m_iXBasePos;							//X ������
	int							m_iYBasePos;							//Y ������
	int							m_iPicWidth;							//ͼ����
	int							m_iPicHight;							//ͼ��߶�
	int							m_iRowCount;							//��������Ŀ
	int							m_iAllLineCount;						//����������
	int							m_iScreenLineCount;						//����������
	static int					m_iWhellTimes;							//���ֱ���
	static int					m_iScrollPos;							//��������

	GameInfoStruct				*pGameInfo;
	CGameRoomEx					*m_pRoom;
	
	CDC m_dcMem;
	HBITMAP* m_bmp;

	//��������
public:
	//���캯��
	CDeskFrameView();
	//��������
	virtual ~CDeskFrameView();

	//��Ϣӳ��
protected:
	DECLARE_MESSAGE_MAP()

	//��������
protected:
	//��������
	BYTE MouseHitTest(POINT point);
	//ȡ����ʼ��
	bool UnInitDeskFrame();

	// �ֱ������¾����ͣ�BmpͼƬ��PngͼƬ
	void OnPaintPng();
	void OnPaintBmp();


	void PaintContestInfo();

	//���ܺ���
public:
	//�õ��Լ��û� ID
	bool GetMeUserID(long MeUserID);
	//��ʼ������
	bool InitDeskFramePng(BYTE bDeskCount, BYTE bEveryCount,
		const wchar_t * pDeskImage, const wchar_t * pLockImage, const wchar_t* pBasePoint, const wchar_t * pBeginImage, const wchar_t * pAgreeImage,
		BYTE bDeskType,int uComType,GameInfoStruct *GameInfo,CGameRoomEx* pRoom,CString deskClass,bool deskFlat,COLORREF clrSitDeskFrameLeftTop,COLORREF clrSitDeskFrameRightBottom
		, bool bIsVip = false);
	//��ʼ������
	bool InitDeskFrame(BYTE bDeskCount, BYTE bEveryCount, CGameImage * pDeskImage, CGameImage * pLockImage, CGameImage* pBasePoint, CGameImage * pBeginImage, CGameImage * pAgreeImage,
		BYTE bDeskType,int uComType,GameInfoStruct *GameInfo,CString deskClass,bool deskFlat,COLORREF clrSitDeskFrameLeftTop,COLORREF clrSitDeskFrameRightBottom);
	//��ȡ��Ϸ����
	BYTE GetEveryDeskCount() { return m_bEveryCount; }
	//�Ƿ���������
	bool IsSetPass(BYTE bDeskIndex);
	//��ȡ��Ϸ��ָ��
	CBaseDeskView * GetDeskArrayPtr(UINT uIndex);	
	//��ȡ������Ϣ
	long int GetMatchInfo(BYTE bDeskIndex, BYTE bDeskStation);
	//������ͼ����
	void SetViewDesk(BYTE bDeskIndex);
	//������Ϸ���ӱ���ͼƬ
	void SetBasePoint(BYTE bDeskIndex,int iBasePoint,bool bSet);
	//�������ӽ�������
	void SetCanInput(BYTE bDeskIndex, bool bCanInput);
	//��������
	bool CreateDeskFrame(CWnd * pParentWnd, UINT uWndID);
	//��ȡ��Ϸ����Ŀ
	BYTE GetDeskCount() { return m_bDeskCount; }
	//���ñ�����Ϣ
	bool SetMatchInfo(BYTE bDeskIndex, BYTE bDeskStation, long int dwUserID, TCHAR * szUserName);
	//���ÿ�ʼ��־
	void SetPlayGame(BYTE bDeskIndex, bool bPlaying);
	//������Ϸ��					
	void UpdateDeskView(BYTE bDeskIndex);
	void	OnHitpagedown();//�Ϸ�
	void	OnHitpageup();	//�·�
	//������ʼ��ť
	void    OnHitBegin();	//
	//��ʾ��ʼ��Ϸ��ť
	void ShowBeginBtn(BYTE bDeskIndex,BYTE bDeskStation,bool bShow);

	//add by wlr 20090713
	// �����ټ��ֿ�ʼ��ť
	void	OnHitBJLBegin();
	//��ʾ�ټ��ֿ�ʼ��ť 
	void ShowBJLBeginBtn();
	//��ʾ�ټ��ֿ�ʼ��ť������Ϸ�����а�ť
	void showBJLBeginOrPlayingBtn(bool bShowPlaying = false); 
	//end of add by wlr 20090713
		
	//�Ŷӻ���ʾ�ŶӺ��뿪
	void showQueueBtn(bool bShowPlaying = false); 

	//�Ŷӻ���ʼ
	void OnHitQueueBegin();
	//�Ŷӻ�ֹͣ
	void OnHitQueueStop();
	//�ų�����ʾʱ��
	void ShowQueueTime(int x, int y,  int inumber);
	//�ų�����ʾ����
	void ShowQueuePlan(int x, int y, int iPlan);
	//�ų�������ʱ��
	void SetQueueTime(int iTime, bool setType = true);



	//���ܺ���
public:
	//�Ƿ���Ϸ
	bool IsPlayGame(BYTE bDeskIndex);
	//�Ƿ�ȫ����ʼͬ��
	bool IsAllAgree(BYTE bDeskIndex);
	//����ͬ����Ա����
	BYTE CountAgreePeople(BYTE bDeskIndex);
	//��������־
	void SetLock(BYTE bDeskIndex, bool bLock);
	//��ȡ�û���Ϣ
	UserItemStruct * GetUserItem(BYTE bDeskIndex, BYTE bDeskStation);
	//�����û��߿�
	void SetUserFrame(BYTE bDeskIndex);
	//���ò��ұ߿�
	void SetFindFrame(BYTE bDeskIndex);
	//�����û���Ϣ
	bool SetUserItem(BYTE bDeskIndex, BYTE bDeskStation, UserItemStruct * pUserItem);

	//��Ϣ����
public:
	//�ػ�����
	afx_msg void OnPaint();
	//λ����Ϣ
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//��������Ϣ
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar);
	//��������Ϣ
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint MousePoint);
	//����Ҽ�������Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//���̰���
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//����Ҽ�������Ϣ
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//����ƶ���Ϣ
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//���ù��
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message);
	//�Ҽ�������Ϣ
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//�Ҽ�������Ϣ
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//���Һ���
public:
	//��ȡ��λ����Ŀ
	BYTE GetNullCount(BYTE bDeskIndex);
	//����λ��
	BYTE FindGameDesk(BYTE bLastFind, bool bOneNull, bool bTwoNull, bool bNoAllNull, bool bAllNull);
	//��ȡ��λ��
	BYTE GetNullStation(BYTE bDeskIndex);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//����
	virtual void LoadBtnAgree(CString deskClass);
	//wushuqun 2009.7.13
	//�޸���Ϸ�������˳���Ϸʱ��������
	afx_msg BOOL OnEraseBkgnd(CDC* pDC)
	{
		return true;
		//
		return CWnd::OnEraseBkgnd(pDC);
	}
};

#endif
/*******************************************************************************************************/