#pragma once
#include "GameLanchSDK/bzwUiInclude/bzwUI.h"
//#include "defines.h"
#define DLG_MSG_MB_OK     1
#define DLG_MSG_MB_YESNO  2
#define DLG_MSG_MB_OK_NEW  3
#define DLG_MSG_MB_YESNO_NEW  4
// ��Ϣ��ʾ�Ի����� 
#define IDD_MSG_DLG_RES_CONFIRM		"resources\\config\\Confirm.xml"	/**< ȷ�϶Ի���·�� */ 
#define IDD_MSG_DLG_RES_SELECT		"resources\\config\\Select.xml"		/**< ѡ��Ի���·��  */
#define IDD_MSG_DLG_RES_CONFIRM_NEW		"resources\\config\\Confirm_new.xml"	/**< ȷ�϶Ի���·�� */ 
#define IDD_MSG_DLG_RES_SELECT_NEW		"resources\\config\\Select_new.xml"		/**< ѡ��Ի���·��  */
#define IDD_DLG_BTN_OK				10001	/**< ȷ����ť */
#define IDD_DLG_BTN_CANCLE			10005	/**< ȡ����ť */
#define IDD_DLG_TIP_LABLE			12345	/**< ��ǩ�ؼ� */

class CAFCMessageBox : public BModalContainer
{
public:
	CAFCMessageBox(void);

	~CAFCMessageBox(void);

	virtual bool OnPostMessage(UINT pId, BBaseParameter* pParam);

	void SetMessage(CString message);

    void SetMessage_New(CString message);

};

//extern UINT AFCMessageBox(CString strInfo,CString strTitel,CWnd* pParentWnd,UINT style=DLG_MSG_MB_OK);

