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
using Bzw.WebLibrary;
using UiCommon;

namespace Bzw.Inhersits
{
	public partial class Service : UiCommon.BasePage
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
		/// webshortcutkey1 �ؼ���
		/// </summary>
		/// <remarks>
		/// �Զ����ɵ��ֶΡ�
		/// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
		/// </remarks>
		//protected global::Public_WebShortCutKey webshortcutkey1;

		/// <summary>
		/// Form1 �ؼ���
		/// </summary>
		/// <remarks>
		/// �Զ����ɵ��ֶΡ�
		/// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
		/// </remarks>
		protected global::System.Web.UI.HtmlControls.HtmlForm Form1;

		/// <summary>
		/// txtName �ؼ���
		/// </summary>
		/// <remarks>
		/// �Զ����ɵ��ֶΡ�
		/// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
		/// </remarks>
		protected global::System.Web.UI.WebControls.TextBox txtName;

		/// <summary>
		/// txtEmail �ؼ���
		/// </summary>
		/// <remarks>
		/// �Զ����ɵ��ֶΡ�
		/// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
		/// </remarks>
		protected global::System.Web.UI.WebControls.TextBox txtEmail;

		/// <summary>
		/// txtTitle �ؼ���
		/// </summary>
		/// <remarks>
		/// �Զ����ɵ��ֶΡ�
		/// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
		/// </remarks>
		protected global::System.Web.UI.WebControls.TextBox txtTitle;

		/// <summary>
		/// txtContent �ؼ���
		/// </summary>
		/// <remarks>
		/// �Զ����ɵ��ֶΡ�
		/// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
		/// </remarks>
		protected global::System.Web.UI.WebControls.TextBox txtContent;

		/// <summary>
		/// imageField2 �ؼ���
		/// </summary>
		/// <remarks>
		/// �Զ����ɵ��ֶΡ�
		/// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
		/// </remarks>
		protected global::System.Web.UI.HtmlControls.HtmlInputSubmit imageField2;

		/// <summary>
		/// webfooter1 �ؼ���
		/// </summary>
		/// <remarks>
		/// �Զ����ɵ��ֶΡ�
		/// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
		/// </remarks>
		protected global::System.Web.UI.UserControl webfooter1;
        protected global::System.Web.UI.WebControls.TextBox VerifyCode;



		protected void Page_Load( object sender, EventArgs e )
		{
			SeoSetting( SeoConfig.Config.FAQSeo );
			if( !IsPostBack )
			{
				BindUserInfo();
			}
		}

		public void BindUserInfo()
		{
			if( UiCommon.UserLoginInfo.IsLogin )
			{
				DataRow dr = new BLL.Member().GetInfo( UiCommon.UserLoginInfo.UserID );
				if( dr != null )
				{
					txtName.Text = dr["NickName"].ToString();
					//UiCommon.UserLoginInfo.UserName;
					//txtName.ReadOnly = true;
					txtEmail.Text = dr["Email"].ToString();
				}
			}
		}
		protected void button5_ServerClick( object sender, EventArgs e )
		{
			/*
			string name = Utility.Common.FilterString(txtName.Text);
			string email = Utility.Common.FilterString(txtEmail.Text);
			string title = Utility.Common.FilterString(txtTitle.Text);
			string content = Utility.Common.FilterString(txtContent.Text);*/

			string name = Server.HtmlEncode( Utility.Common.SqlEncode( txtName.Text ) );
			string email = Utility.Common.FilterString( txtEmail.Text );
			string title = Server.HtmlEncode( Utility.Common.SqlEncode( txtTitle.Text ) );
			string content = Server.HtmlEncode( Utility.Common.SqlEncode( txtContent.Text ) );

            string verify = VerifyCode.Text;
            if (ValidCode.CurrentCode != verify)
            {

                CommonManager.Web.RegJs(this, "alert('��ܰ��ʾ����֤������������������룡');", true);
                return;
            }



			int iRows = new BLL.Feedback().Add( name, email, title, content, Utility.Common.RequestIP );
			if( iRows > 0 )
			{
				if( UiCommon.UserLoginInfo.IsLogin )
					Response.Redirect( "/Manage/Feedbacks.aspx" );
				else
					CommonManager.Web.RegJs( this, "location.href='Service.aspx?success';", false );
			}
			else
				CommonManager.Web.RegJs( this, "location.href='Service.aspx?fail';", false );

			//if (iRows > 0)
			//{
			//    lblMsg.Text = "�����ύ�ɹ�!"+DateTime.Now;
			//    if (UiCommon.UserLoginInfo.IsLogin)
			//    {
			//        Response.Redirect("/Manage/Feedbacks.aspx");
			//    }
			//    else
			//    {
			//        txtName.Text = string.Empty;
			//        txtEmail.Text = string.Empty;
			//        txtTitle.Text = string.Empty;
			//        txtContent.Text = string.Empty;
			//    }
			//}
			//else
			//{
			//    lblMsg.Text = "��Ǹ�������ύʧ�ܣ������Ƿ�������æ�����Ժ����ԣ�";
			//}
		}
	}

}
