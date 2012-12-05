#pragma once

#include "FastDelegate.h"
using namespace fastdelegate;
typedef FastDelegate2<int, int> CMDCallBack;

struct IUserInterface;
struct IBCInterFace;
/** @brief �������������
 */
class IUICommand
{
public:
    virtual ~IUICommand(){}
    /** @brief ִ������
     */
    virtual int Execute()=0;
    /** @brief ����ִ��
     */
    virtual int Undo()=0;
    /** @brief ��ȡ���ƿؼ���ID��
     */
    virtual int GetRecieverID()=0;
};


/** @brief ִ������ӿ�
 */
class ICmdInterface
{
public:
    /** @brief �������һ��
     */
    virtual int StepRollBack() = 0;
    /** @brief ��ǰ�ظ�һ��
     */
    virtual int StepReplyFront() = 0;
    /** @brief ����ĳ�ؼ���λ��
     */
    virtual IUICommand *SetModeXY(int nID, int nX, int nY) = 0;
    /** @brief ����ĳ�ؼ��Ŀ��
     */
    virtual IUICommand *SetW(int nID, int nW) = 0;
    /** @brief ����ĳ�ؼ��ĸ߶�
     */
    virtual IUICommand *SetH(int nID, int nH) = 0;
    /** @brief ����һ���ؼ�
    */
    virtual IUICommand *AddControl(IUserInterface *pUserInterface, IBCInterFace *pCtrl, IBCInterFace *pParent, CMDCallBack CallBack) = 0;
    /** @brief ɾ��һ���ؼ�
    */
    virtual IUICommand *DelControl(IUserInterface *pUserInterface, IBCInterFace *pCtrl, CMDCallBack CallBack) = 0;
    /** @brief �ƶ�һ���ؼ�
    */
    virtual IUICommand *MoveControl(IUserInterface *pUserInterface, int nToMoveID, int nDstContain, CMDCallBack CallBack) = 0;

};