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
using Bzw.WebLibrary;

namespace Bzw.Inhersits.Manage
{
    public partial class Manage_Login : UiCommon.BasePage
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
        /// form1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlForm form1;

        /// <summary>
        /// webtop1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.UserControl webtop1;

        /// <summary>
        /// webservice1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.UserControl webservice1;



        /// <summary>
        /// lbMsg �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.Label lbMsg;

        /// <summary>
        /// txtAccount �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.TextBox txtAccount;

        /// <summary>
        /// txtPwd �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.TextBox txtPwd;

        /// <summary>
        /// txtValidCode �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.TextBox txtValidCode;

        /// <summary>
        /// imageField �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.Button imageField;

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
            SeoSetting(SeoConfig.Config.DefaultSeo);
            //����ѵ�¼��ص���ҳ
            if (UiCommon.UserLoginInfo.IsLogin)
            {
                Response.Redirect("/Default.aspx");
            }
        }
        protected void btnSubmit_Click(object sender, EventArgs e)
        {
            lbMsg.Text = "�Բ��𣬷��ʷ������������Ժ����ԣ�";

            //1.У����֤��
            if (UiCommon.ValidCode.CurrentCode == null)
            {
                lbMsg.Text = "��֤���ѹ��ڣ��������µ���֤�룡";
                txtValidCode.Text = string.Empty;
                return;
            }
            else if (txtValidCode.Text.Trim() != UiCommon.ValidCode.CurrentCode)
            {
                lbMsg.Text = "��֤����������������µ���֤�룡";
                txtValidCode.Text = string.Empty;
                return;
            }


            string userName = Utility.Common.FilterString(txtAccount.Text);
            string pass = Utility.Common.FilterString(txtPwd.Text); //Utility.Common.FilterString( hidPwd.Value.Trim() );//

            BLL.LoginResults lr = new BLL.Member().Login(userName, pass);
            if (lr == BLL.LoginResults.MemberNameInvalid)
            {
                lbMsg.Text = "�û�������";
                txtValidCode.Text = string.Empty;
            }
            if (lr == BLL.LoginResults.PasswordInvalid)
            {
                lbMsg.Text = "�������";
                txtValidCode.Text = string.Empty;
            }
            else if (lr == BLL.LoginResults.Locked)
            {
                lbMsg.Text = "�����ʺ��Ѿ�������Ա���ƣ����ܵ�¼��";
                txtValidCode.Text = string.Empty;
            } 
            else if (lr == BLL.LoginResults.Succeed)
            {
                new BLL.Member().UpdateLastLogin(userName, Utility.Common.RequestIP);

                string backUrl = Utility.Common.UrlValue;
                if (backUrl.Trim().Length == 0)
                {
                    backUrl = "/Manage/Default.aspx";
                }
                if (backUrl.Trim() == UiCommon.StringConfig.WebSiteName+"/Default2.aspx")
                {
                    backUrl = "/Manage/Default.aspx";
                }
                Response.Redirect(backUrl);
            }

            //lbMsg.Visible = true;
        }
    }
}
