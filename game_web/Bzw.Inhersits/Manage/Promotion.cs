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
using System.IO;
using System.Text;

namespace Bzw.Inhersits.Manage
{

    public partial class Manage_Promotion : UiCommon.ManageBasePage
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
        /// Form1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlForm Form1;

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
        /// h3Add �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlGenericControl h3Add;

        /// <summary>
        /// h3Edit �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlGenericControl h3Edit;

        /// <summary>
        /// h3Pass �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlGenericControl h3Pass;

        /// <summary>
        /// dltgAddress �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlTable dltgAddress;


        /// <summary>
        /// DownPromotionSet �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlTable DownPromotionSet;

        /// <summary>
        /// DownIN �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlInputButton DownIN;

        /// <summary>
        /// txtTrueName �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.TextBox txtTrueName;

        /// <summary>
        /// txtPhone �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.TextBox txtPhone;

        /// <summary>
        /// RequiredFieldValidator2 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.RequiredFieldValidator RequiredFieldValidator2;

        /// <summary>
        /// RegularExpressionValidator1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.RegularExpressionValidator RegularExpressionValidator1;

        /// <summary>
        /// txtAddress �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.TextBox txtAddress;

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
        BLL.Promotion promo = new BLL.Promotion();

        protected void Page_Load(object sender, EventArgs e)
        {
            if (!IsPostBack)
            {
                IsPass();
                BindInfo();
            }
        }

        public void IsPass()
        {
            int passState = promo.GetPassState(UiCommon.UserLoginInfo.UserID); //Response.Write(passState);
            if (passState == -1)
            {
                h3Add.Visible = true;
                h3Edit.Visible = false;
                h3Pass.Visible = false;
                dltgAddress.Visible = false;
                DownPromotionSet.Visible = false;

            }
            else if (passState == 0)
            {
                h3Add.Visible = false;
                h3Edit.Visible = true;
                h3Pass.Visible = false;
                dltgAddress.Visible = false;
                DownPromotionSet.Visible = false;
            }
            else if (passState == 1)
            {
                h3Add.Visible = false;
                h3Edit.Visible = false;
                h3Pass.Visible = true;
                dltgAddress.Visible = true;
                DownPromotionSet.Visible = true;
            }
        }

        public void BindInfo()
        {
            DataRow dr = promo.GetInfo(UiCommon.UserLoginInfo.UserID);
            if (dr != null)
            {
                ViewState["id"] = dr["ID"].ToString();
                txtTrueName.Text = dr["TrueName"].ToString();
                txtPhone.Text = dr["Phone"].ToString();
                txtAddress.Text = dr["Address"].ToString();

                button5.Value = "�޸�����";
            }
            else
            {
                button5.Value = "�ύ����";
            }
        }

        protected void button5_ServerClick(object sender, EventArgs e)
        {
            string trueName = Utility.Common.FilterString(txtTrueName.Text);
            string phone = Utility.Common.FilterString(txtPhone.Text);
            string address = Utility.Common.FilterString(txtAddress.Text);

            int iRows = 0;
            string strMsg = "�ύ";
            if (ViewState["id"] == null || Convert.ToInt32(ViewState["id"]) <= 0)
            {
                iRows = promo.Add(UiCommon.UserLoginInfo.UserID, trueName, phone, address);
            }
            else
            {
                iRows = promo.Update(UiCommon.UserLoginInfo.UserID, trueName, phone, address);
                strMsg = "�޸�";
            }
            string js = string.Empty;
            if (iRows > 0)
            {
                js = "alert('����" + strMsg + "�ɹ���');location.href=location.href;";
                CommonManager.Web.RegJs(this, js, false);
                IsPass();
                BindInfo();
            }
            else
            {
                js = "alert('��Ǹ������" + strMsg + "ʧ�ܣ������Ƿ�������æ�����Ժ����ԣ�');location.href=location.href;";
                CommonManager.Web.RegJs(this, js, false);
            }
        }
        protected void DownIN_ServerClick(object sender, EventArgs e)
        {


            StringBuilder sb = new StringBuilder();

            sb.Append("[SalesmanID]\r\n");
            sb.AppendFormat("ID={0}", UiCommon.UserLoginInfo.UserID);


            try
            {


                byte[] buffer = System.Text.Encoding.Default.GetBytes(sb.ToString());  //�õ��ַ������ֽ���

                Response.Clear();

                Response.AddHeader("Content-Disposition", "attachment;filename=key.ini");
                Response.AddHeader("Content-Length", buffer.Length.ToString());
                Response.ContentType = CommonManager.File.GetFileContentType("ini");

                Response.BinaryWrite(buffer);
                Response.End();
            }
            catch (Exception ex)
            {
                Response.Write("<script>alert(\"" + ex.Message + "\");</script>");
            }

        }
    }
}
