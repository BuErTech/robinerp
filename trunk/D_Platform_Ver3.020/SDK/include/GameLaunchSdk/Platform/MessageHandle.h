#ifndef CMESSAGEHANDLE_HEAD_FILE
#define CMESSAGEHANDLE_HEAD_FILE

#include "PublicDefine.h"
#include "AFCEdit.h"
#include "AFCExpressionWnd.h"
#include "SkinMgr.h"
//using namespace AFC::AFCEdit;

class CRightFrame;

//��Ϣ������
class EXT_CLASS CLookingClass//C-MessageHandle
{
	//��������
protected:
	CNewMiniDlg						* m_pRichEdit;
	CRightFrame						* m_pRightFrame;
	
	//��������
public:
	//���캯��
	CLookingClass(CNewMiniDlg * pRichEdit);

	//��������
	virtual ~CLookingClass(void);
	CAFCColorResource					Color_Struct;
	void SetColor(CAFCColorResource	Color_Struct);

	//ʹ��DUI�������Add by Jianguankun 2012.9.17
	void SwitchToDuiVersion(CRightFrame* pRightFrame);

	//���ܺ���
public:
	//����
	void InsertNews(TCHAR * szCharString, UINT uSize=0, TCHAR * szFontName=NULL);
	void InsertNews(CString szCharString, UINT uSize=0, TCHAR * szFontName=NULL);
	//�û�����
	void InsertUserName(TCHAR * szUserName);
	void InsertSuperUserName(TCHAR * szUserName);
	//����Ϣ
	void InsertNormalMessage(TCHAR * szCharString, bool bEnter=true, UINT uSize=0, TCHAR * szFontName=NULL);
	//ϵͳ��Ϣ
	void InsertSystemMessage(TCHAR * szCharString, UINT uSize=0, TCHAR * szFontName=NULL);

	void InsertSystemMessageM(TCHAR * szCharString, UINT uSize=0, TCHAR * szFontName=NULL);
	//ϵͳ����
	void InsertSystemBoard(TCHAR * szCharString, UINT uSize=0, TCHAR * szFontName=NULL);
	//�Զ���ͷ��Ϣ
	void InsertTitleMessage(TCHAR * szMsg, BYTE bNewOrSys=0,UINT uSize=0, TCHAR * szFontName=NULL,TCHAR *Title=NULL);
	//�û�������Ϣ
	void InsertUserTalk(TCHAR * szSendName, TCHAR * szTargetName,TCHAR * szCharString, COLORREF crTextColor, bool bShowTime, UINT uSize=0, TCHAR * szFontName=NULL);
	//��
	void InsertDudu(TCHAR * szSendName, TCHAR * szTargetName,TCHAR * szCharString, COLORREF crTextColor, bool bShowTime, UINT uSize=0, TCHAR * szFontName=NULL,int iDuduType = 0);

	//�ڲ�����
private:
	//���Ͳ���
	bool TranslateInsertChar(TCHAR * szCharString, COLORREF crTextColor, UINT uSize, TCHAR * szFontName);
	//�����ַ�
	CTrueItem * TranslateChar(TCHAR * szCharString, int iCharLength, int & iPassCharCount);

private:
	CSkinMgr	m_skinmgr;
};

#endif