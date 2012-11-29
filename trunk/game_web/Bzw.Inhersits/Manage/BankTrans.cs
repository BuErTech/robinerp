using System;
using System.Data;
using System.Configuration;
using System.Collections;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;
using Bzw.Data;
using BCST.Common;
using System.Collections.Generic;


namespace Bzw.Inhersits.Manage
{

    public partial class Manage_BankTrans : UiCommon.ManageBasePage
    {
        /// <summary>
        /// Head1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlHead Head1;

        /// <summary>
        /// webmeta1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.UserControl webmeta1;

        /// <summary>
        /// webtop1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.UserControl webtop1;

        /// <summary>
        /// Left1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.UserControl Left1;

        /// <summary>
        /// webuserlogin1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.UserControl webuserlogin1;

        /// <summary>
        /// Form1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlForm Form1;

        /// <summary>
        /// lblMsg �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.Label lblMsg;

        /// <summary>
        /// txtPassword �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.TextBox txtPassword;

        /// <summary>
        /// txtInputUserName �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.TextBox txtInputUserName;

        /// <summary>
        /// txtInputUserName2 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.TextBox txtInputUserName2;

        /// <summary>
        /// txtMoney �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.TextBox txtMoney;

        /// <summary>
        /// button5 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlInputSubmit button5;

        /// <summary>
        /// webfooter1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.UserControl webfooter1;
        /// <summary>
        /// �˻�������������������ת��
        /// </summary>
        public Int64 TransferLessAllMoney = 0;
        /// <summary>
        /// ����ת�˵���С�����
        /// </summary>
        public Int64 TransferLessMoney = 0;
        /// <summary>
        /// ת�˵Ľ��ֵ������������ı���
        /// </summary>
        public int TransferMultiple = 0;
        /// <summary>
        /// ת��Ҫ�۳����������� 0.01��ʾ1%
        /// </summary>
        public int TransferTax = 0;
        /// <summary>
        /// ����ת����߿۳�����������
        /// </summary>
        public int MaxTransTax = 0;
        /// <summary>
        /// VIP�û�ת��Ҫ�۳���������
        /// </summary>
        public int VipTransferTax = 0;

        BLL.Member member = new BLL.Member();

        protected void Page_Load(object sender, EventArgs e)
        {
            if (member.IsInRoomOrGame(UiCommon.UserLoginInfo.UserName))
            {
                //lblMsg.Text = "��Ǹ�������ʺ��ѽ��뷿���л�������Ϸ�У����ܽ��д�ȡ�������";
                BCST.Common.CommonManager.Web.RegJs(this, "alert('��Ǹ�������ʺ��ѽ��뷿���л�������Ϸ�У����ܽ��д�ȡ�������');location.href='/Manage/';", false);
                return;
            }

            //1.��ȡ��web_config�е�ת�˲�����Ϣ
            DataRow dr = BLL.Config.GetInfoOfTrans();
            if (dr != null)
            {
                if (dr["TransferLessAllMoney"] != null)
                {
                    TransferLessAllMoney = Convert.ToInt64(dr["TransferLessAllMoney"]);
                }

                if (dr["TransferLessMoney"] != null)
                {
                    TransferLessMoney = Convert.ToInt32(dr["TransferLessMoney"]);
                }

                if (dr["TransferMultiple"] != null)
                {
                    TransferMultiple = Convert.ToInt32(dr["TransferMultiple"]);
                }

                if (dr["TransferTax"] != null)
                {
                    TransferTax = Convert.ToInt32(dr["TransferTax"]);
                }

                if (dr["MaxTransTax"] != null)
                {
                    MaxTransTax = Convert.ToInt32(dr["MaxTransTax"]);
                }

                if (dr["VipTransferTax"] != null)
                {
                    VipTransferTax = Convert.ToInt32(dr["VipTransferTax"]);
                }
            }
        }

        protected void button5_ServerClick(object sender, EventArgs e)
        {
            if (!BCST.Common.CommonManager.Web.CheckPostSource())
                return;

            string userId = UiCommon.UserLoginInfo.UserID;
            string outUserName = UiCommon.UserLoginInfo.UserName;
            string password = txtPassword.Text.Trim();//hidPwd.Value.Trim(); //
            if (!member.CompareTwoPassword(userId, password))
            {
                lblMsg.Text = "�Բ����������������������";
                return;
            }

            Int64 outUserBankMoney = member.GetBankMoney(userId);

            string inputUserName = Utility.Common.FilterString(txtInputUserName.Text.Trim());



            //2.�ж�ת�����˺��Ƿ�����Ϸ�����������Ϸ�У������ܽ���ת�˲�����
            if (member.IsInRoomOrGame(outUserName))
            {
                lblMsg.Text = "��Ǹ�������ʺ��ѽ��뷿���л�������Ϸ�У����ܽ���ת�˲�����";
                return;
            }

            //2.1ת�����˻���������������TransferLessAllMoney����ת��
            long LeftMoney = member.GetAllMoney(userId);       //��ȡ�˻���� 

            if (LeftMoney <= Convert.ToInt64(TransferLessAllMoney))
            {
                lblMsg.Text = "��Ǹ�������˻���������ڹ涨��" + UiCommon.StringConfig.AddZeros(TransferLessAllMoney) +UiCommon.StringConfig.MoneyName+ "����ת�ˣ�";
                return;

            }

            //3.���ת�뷽���˺Ų����ڣ����ܽ���ת�˲���
            if (!member.ExistName(inputUserName))
            {
                lblMsg.Text = "��Ǹ������д��ת���ʺŲ����ڣ�";
                return;
            }

            //4.���ת�뷽���˺�������Ϸ�У����ܽ���ת�˲������������ƿ�ѡ��
            if (member.IsInRoomOrGame(inputUserName))
            {
                lblMsg.Text = "��Ǹ��ת�뷽���ʺ��ѽ��뷿���л�������Ϸ�У����ܽ���ת�˲�����";
                return;
            }

            long transMoney = Convert.ToInt64(txtMoney.Text.Trim());
            //5.1ת�˽�������������ı���TransferMultiple
            if (transMoney % TransferMultiple != 0)
            {
                lblMsg.Text = "��Ǹ���������ת�˽�������" + TransferMultiple + "�ı�����";
                return;
            }

            transMoney = Convert.ToInt64(UiCommon.StringConfig.RemoveZeros(transMoney));
            //5.2ת�˽���С�ڵ���ת�˹涨ֵTransferLessMoney
            if (transMoney < TransferLessMoney)
            {
                lblMsg.Text = "��Ǹ���������ת�˽��С�ڵ���ת�˹涨ֵ" + UiCommon.StringConfig.AddZeros(TransferLessMoney) +UiCommon.StringConfig.MoneyName+ "��";
                return;
            }

            //6.ת���������ʻ����㣬������ڽ���ת�ʣ�
            if (outUserBankMoney < transMoney)
            {
                lblMsg.Text = "��Ǹ�������������㣬������ֵ���ٽ���ת�ˣ�";
                return;
            }

            //transMoney�����㣬��˵������Ľ������õı���С������Ľ������Ҫ�ﵽ�����Ķ�����2010-01-26
            if (transMoney <= 0)
            {
                lblMsg.Text = "��Ǹ��ת�˽�������ڻ����" + UiCommon.StringConfig.GetMultiple();
                return;
            }

            //7.����ת�������д�����ת�뷽���д�д��ת�� Web_TransLog ��¼��
            Dictionary<string, object> dic = new Dictionary<string, object>();
            DbSession.Default.FromProc("Web_pBankTrans")
                .AddInputParameter("@outUserName", DbType.String, outUserName)
                .AddInputParameter("@inUserName", DbType.String, inputUserName)
                .AddInputParameter("@transMoney", DbType.Int64, transMoney)
                .AddInputParameter("@transTime", DbType.DateTime, DateTime.Now)
                .AddInputParameter("@transIP", DbType.String, CommonManager.Web.GetIP())
                .AddOutputParameter("@Result", DbType.Int32, 4)
                .Execute(out dic);

            //if( member.BankTransfer( outUserName, inputUserName, transMoney, Utility.Common.RequestIP ) )
            if (dic.Count > 0 && (int)dic["Result"] == 0)
            {
                DateTime tmp_viptime = DbSession.Default.FromSql("select DATEADD([second],viptime,'1970-01-01 08:00:00') from Web_vUserList where UserName=@UserName").AddInputParameter("@UserName", DbType.String, outUserName).ToScalar<DateTime>();

                int taxMoney = 0;// = transMoney;
                if (tmp_viptime > DateTime.Now)
                {
                    //VIP������   ������Ŀʵʩ��Ҫ��ת�������Ѹĳ�ǧ�ֱ�  ��ʥ�� 2010-05-14
                    taxMoney = (int)((float)transMoney * (((float)VipTransferTax / 10000))); //*= (VipTransferTax / 100);
                }
                else
                {
                    //��ͨ��Ա������
                    taxMoney = (int)((float)transMoney * (((float)TransferTax / 10000))); //*= (TransferTax / 100);
                }

                if (taxMoney > MaxTransTax)
                {
                    taxMoney = MaxTransTax;
                }
                int sjTransMoney = (int)transMoney - taxMoney; //ʵ��ת�˽�� 

                CommonManager.Web.RegJs(this, "alert('ת�˳ɹ�����ת��" + inputUserName + "��" + UiCommon.StringConfig.AddZeros(transMoney) +UiCommon.StringConfig.MoneyName+ "���۳�������" + UiCommon.StringConfig.AddZeros(taxMoney) +UiCommon.StringConfig.MoneyName+ "��" + UiCommon.StringConfig.AddZeros(sjTransMoney) + UiCommon.StringConfig.MoneyName+"��ת����/���������˻���');location.href=location.href;", false);
            }
            else if (dic.Count > 0 && (int)dic["Result"] == 1)
            {
                CommonManager.Web.RegJs(this, "alert('��������" + UiCommon.StringConfig.MoneyName + "���㣬����ȥ��ֵ����ת�ʡ�');location.href=location.href;", true);
            }
            else if (dic.Count > 0 && (int)dic["Result"] == 3)
            {
                CommonManager.Web.RegJs(this, "alert('��Ǹ��ת�˲����Լ�ת���Լ�������������ת���ʺţ�');location.href=location.href;", true);
            }
            else
            {
                //lblMsg.Text = "��Ǹ��ת�˲���ʧ�ܣ������Ƿ�������æ�����Ժ����ԣ�";
                CommonManager.Web.RegJs(this, "alert('��Ǹ��ת�˲���ʧ�ܣ������Ƿ�������æ�����Ժ����ԣ�');location.href=location.href;", true);
            }
        }
    }
}
