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


namespace Bzw.Inhersits.Manage
{
    public partial class Manage_GetPass : UiCommon.BasePage
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
        /// webuserlogin1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.UserControl webuserlogin1;

        /// <summary>
        /// webservice1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.UserControl webservice1;


        /// <summary>
        /// form1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlForm form1;

        /// <summary>
        /// lblMsg �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.Label lblMsg;

        /// <summary>
        /// txtUserName �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.TextBox txtUserName;

        /// <summary>
        /// txtValidCode �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.TextBox txtValidCode;

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
        protected void Page_Load(object sender, EventArgs e)
        {

        }

        protected void button5_ServerClick(object sender, EventArgs e)
        {
            //1.У����֤��
            if (UiCommon.ValidCode.CurrentCode == null)
            {
                lblMsg.Text = "��֤���ѹ��ڣ��������µ���֤�룡";
                txtValidCode.Text = string.Empty;
                return;
            }
            else if (txtValidCode.Text.Trim() != UiCommon.ValidCode.CurrentCode)
            {
                lblMsg.Text = "��֤����������������µ���֤�룡";
                txtValidCode.Text = string.Empty;
                return;
            }

            //2.�ж��û����Ƿ����
            BLL.Member member = new BLL.Member();
            string userName = Utility.Common.SqlEncode(txtUserName.Text.Trim());
            if (!member.ExistName(userName))
            {
                lblMsg.Text = "��Ǹ���������ע���û��������ڣ�";
                txtValidCode.Text = string.Empty;
                return;
            }
            else
            {
                Session["getpwduser"] = txtUserName.Text;
                Response.Redirect("/Manage/GetPassAnswer.aspx");
            }
        }
    }
}
