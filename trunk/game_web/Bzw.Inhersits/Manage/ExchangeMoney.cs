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
using BCST.Common;
using UiCommon;
using Bzw.Entities;
using Bzw.Data;
using Bzw.E;

namespace Bzw.Inhersits.Manage
{
    public partial class Manage_ExchangeMoney : UiCommon.ManageBasePage
    {

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
        /// Form1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlForm Form1;

        /// <summary>
        /// txtPassword �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.TextBox txtPassword;

        /// <summary>
        /// RequiredFieldValidator1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.RequiredFieldValidator RequiredFieldValidator1;

        /// <summary>
        /// CustomValidator1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.CustomValidator CustomValidator1;

        /// <summary>
        /// txtAgent �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.TextBox txtAgent;

        /// <summary>
        /// RequiredFieldValidator2 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.RequiredFieldValidator RequiredFieldValidator2;

        /// <summary>
        /// txtAgent2 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.TextBox txtAgent2;

        /// <summary>
        /// RequiredFieldValidator3 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.RequiredFieldValidator RequiredFieldValidator3;

        /// <summary>
        /// CompareValidator1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.CompareValidator CompareValidator1;

        /// <summary>
        /// CustomValidator2 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.CustomValidator CustomValidator2;

        /// <summary>
        /// txtMoney �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.TextBox txtMoney;

        /// <summary>
        /// RequiredFieldValidator4 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.RequiredFieldValidator RequiredFieldValidator4;

        /// <summary>
        /// CompareValidator2 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.CompareValidator CompareValidator2;

        /// <summary>
        /// btnSubmit �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.Button btnSubmit;

        /// <summary>
        /// webfooter1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.UserControl webfooter1;
        /// <summary>
        /// ���ת�ʽ��
        /// </summary>
        protected string MinMoney = "";
        /// <summary>
        /// ��������ȡ��ʽ
        /// </summary>
        protected string ChangeType = "";
        /// <summary>
        /// �����ѽ����������㹫ʽ
        /// </summary>
        protected string ProcedureFee = "";

        private string _filePath = "/Upload/ProcedureFee.config";

        /// <summary>
        /// �ļ�·��
        /// </summary>
        private string FilePath
        {
            get
            {
                return Server.MapPath(_filePath);
            }
        }


        protected void Page_Load(object sender, EventArgs e)
        {
            //��ȡ�����ļ�����ʼ��ҳ��
            XmlOperator xo = new XmlOperator(_filePath);
            if (!xo.HasDocument)
            {
                CommonManager.Web.RegJs(this, "alert('��������δ��ͨ�����Ժ��ٳ��Խ����ҳ��');history.back();", false);
                return;
            }
            MinMoney = xo.GetNodeValue("MinMoney");
            ChangeType = xo.GetNodeValue("Type") == "0" ? "�̶���" : "�ٷֱ�";
            ProcedureFee = xo.GetNodeValue("Type") == "0" ? xo.GetNodeValue("StaticValue") : "(���׶��" + xo.GetNodeValue("PercentValue") + "%)";
        }

        protected void btnSubmit_Click(object sender, EventArgs e)
        {
            if (!CommonManager.Web.CheckPostSource())
                return;
            if (!IsValid)
                return;

            //0�������У��
            #region �����У��
            //��ȡ�����ļ�
            XmlOperator xo = new XmlOperator(_filePath);
            if (!xo.HasDocument)
            {
                CommonManager.Web.RegJs(this, "alert('��������δ��ͨ�����Ժ��ٳ��Խ����ҳ��');history.back();", false);
                return;
            }
            if (string.IsNullOrEmpty(txtPassword.Text.Trim()))
            {
                CommonManager.Web.RegJs(this, "alert('���������ĵ�ǰ���룡');", false);
                return;
            }
            TUsers user = DbSession.Default.Get<TUsers>(TUsers._.UserID == UserLoginInfo.UserID);
            if (user.Pass != CommonManager.String.EncryptMD5SystemDefaultMethod(txtPassword.Text.Trim(), false, true))
            {
                CommonManager.Web.RegJs(this, "alert('����������벻��ȷ�����������롣');", false);
                return;
            }
            if (string.IsNullOrEmpty(txtAgent.Text.Trim()) || string.IsNullOrEmpty(txtAgent2.Text.Trim()) || txtAgent.Text.Trim() != txtAgent2.Text.Trim())
            {
                CommonManager.Web.RegJs(this, "alert('����ȷ����������ʺ�');", false);
                return;
            }
            else if (!DbSession.Default.Exists<Web_Agent>(Web_Agent._.LoginName == txtAgent.Text.Trim()))
            {
                CommonManager.Web.RegJs(this, "alert('�������ʺŲ����ڣ������º˶ԣ�');", false);
                return;
            }
            if (string.IsNullOrEmpty(txtMoney.Text.Trim()) || !CommonManager.String.IsInteger(txtMoney.Text.Trim()))
            {
                CommonManager.Web.RegJs(this, "alert('����ȷ����Ҫת�ʵĽ�ң�');", false);
                return;
            }
            #endregion
            //1������û�����Ľ��
            #region ����û�����Ľ��
            int money = int.Parse(txtMoney.Text.Trim());
            BLL.Member m = new BLL.Member();
            Int64 bmoney = m.GetBankMoney(user.UserID.ToString());
            if (money > bmoney)
            {
                CommonManager.Web.RegJs(this, "alert('�������н�����ֻ��" + bmoney.ToString() == "0" ? "0" : bmoney.ToString() + UiCommon.StringConfig.AddZero() + "�����������롣');", false);
                return;
            }
            int minmoney = int.Parse(xo.GetNodeValue("MinMoney"));
            if (money <= minmoney)
            {
                CommonManager.Web.RegJs(this, "alert('ÿ�ζһ���������������" + minmoney.ToString() + "00��');", false);
                return;
            }
            #endregion
            //2��ȡ�ô�������Ϸ�ʻ�
            #region ȡ�ô�������Ϸ�ʻ�
            Web_VAgentUserInfo agent = DbSession.Default.Get<Web_VAgentUserInfo>(Web_VAgentUserInfo._.LoginName == txtAgent.Text.Trim());
            if (agent == null)
            {
                CommonManager.Web.RegJs(this, "alert('�������ʺŲ����ڣ������º˶ԣ�');", false);
                return;
            }
            #endregion
            //3��ת��
            //3.1���������ʺ��Ƿ�����Ϸ��
            #region �������ʺ��Ƿ�����Ϸ��
            if (m.IsInRoomOrGame(UserLoginInfo.UserName))
            {
                CommonManager.Web.RegJs(this, "alert('�����ʺ�������Ϸ�У������˳���Ϸ�ٽ��в�����');", false);
                return;
            }
            #endregion
            //3.2�����������ʺ��Ƿ�����Ϸ��
            #region ���������ʺ��Ƿ�����Ϸ��
            if (m.IsInRoomOrGame(agent.UserName))
            {
                CommonManager.Web.RegJs(this, "alert('�������ʺ�������Ϸ�У����ζһ����ܽ��У����Ժ��ԡ�');", false);
                return;
            }
            #endregion
            //3.3����ʼת��
            #region ��ʼת��
            //3.3.1������ʵ�ʵ��˽�ң���ȡ�����ļ��е������ѷ�ʽ
            string type = xo.GetNodeValue("Type");
            //3.3.2������������
            int profee = type == "0" ? int.Parse(xo.GetNodeValue("StaticValue")) : (int)((float)money * (float.Parse(xo.GetNodeValue("PercentValue")) / 100));
            //3.3.3������ʵ�ʵ��˽��
            int amoney = money - profee;


            //3.3.4��ת��
            //if( m.BankTransfer( UserLoginInfo.UserName, agent.UserName, amoney, CommonManager.Web.GetIP() ) )
            //{
            //    //3.4����¼ת����־
            //    Web_AgentMoneyLog log = new Web_AgentMoneyLog();
            //    log.ActuallyMoney = amoney;
            //    log.AgentID = agent.ID;
            //    log.InputDate = DateTime.Now;
            //    log.Money = money;
            //    log.ProcedureFee = profee;
            //    log.UserID = int.Parse( UserLoginInfo.UserID );
            //    DbSession.Default.Save<Web_AgentMoneyLog>( log );
            //    //4����ʾ��Ϣ��ת����־��¼ҳ��
            //    CommonManager.Web.RegJs( this, "alert('�һ��ɹ���');location.href='ExchangeMoneyLog.aspx';", false );
            //}
            //else
            //{
            //    //4����ʾ��Ϣ
            //    CommonManager.Web.RegJs( this, "alert('�һ�ʧ�ܣ�����������ԭ�����Ժ��ԡ�');", false );
            //}
            #endregion
        }
    }
}
