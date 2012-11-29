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

using Utility;
using UiCommon;
using BCST.Common;
using Bzw.Data;
using System.Collections.Generic;
using Bzw.WebLibrary;

namespace Bzw.Inhersits
{
	public partial class DuiJiang : UiCommon.ManageBasePage
	{
		/// <summary>
		/// Head1 �ؼ���
		/// </summary>
		/// <remarks>
		/// �Զ����ɵ��ֶΡ�s
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
		/// lblMsg �ؼ���
		/// </summary>
		/// <remarks>
		/// �Զ����ɵ��ֶΡ�
		/// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
		/// </remarks>
		protected global::System.Web.UI.WebControls.Label lblMsg;

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

		protected global::System.Web.UI.WebControls.TextBox txtUserRemark;

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

		public string AwardID = "0";
		public string AwardName = string.Empty;
		public string AwardPic = string.Empty;
		public Int64 AwardMoney = 0;
		public int AwardTime = 0;
		public int AwardCount = 0;
		public string AwardInfo = string.Empty;

		BLL.Member member = new BLL.Member();

		protected void Page_Load( object sender, EventArgs e )
		{
            SeoSetting(SeoConfig.Config.PrizeSeo);
			BindJiangPin();
		}

		public void BindJiangPin()
		{
			AwardID = Common.SqlEncode( Common.GetStringOfUrl( "id" ) );
			if( string.IsNullOrEmpty( AwardID ) || !CommonManager.String.IsInteger( AwardID ) )
			{
				Response.Write( "<script>alert('�벻Ҫ���ԷǷ�������');history.back();</script>" );
				Response.End();
				return;
			}

			DataRow dr = new BLL.JiangPin().GetInfo( AwardID );
			if( dr != null )
			{
				AwardName = dr["Award_Name"].ToString();
				AwardPic = dr["Award_Pic"].ToString();
				AwardMoney = Convert.ToInt64( dr["Award_MoneyCost"] );
				AwardTime = Convert.ToInt32( dr["Award_OnLineTime"] );
				AwardCount = Convert.ToInt32( dr["Award_Num"] );
				AwardInfo = dr["Award_Info"].ToString();
			}
			else
			{
				Response.Write( "<script>alert('�����鿴�Ľ�Ʒ�������ڻ��ѱ�ɾ����');history.back();</script>" );
				Response.End();
				return;
			}

			//�����鿴�Ľ�Ʒ�������ڻ��ѱ�ɾ����
		}

		protected void button5_ServerClick( object sender, EventArgs e )
		{
			//1.�˽�Ʒ�Ѷһ����
			if( AwardCount <= 0 )
			{
				/*lblMsg.Text = "��Ǹ���˽�Ʒ�Ѷһ���ϣ�";
				*/
				CommonManager.Web.RegJs( this, "alert('��Ǹ���˽�Ʒ�Ѷһ���ϣ�');location.href=location.href;", true );
				return;
			}

			//2.����Ч������Ϸʱ�䲻���һ��˽�Ʒ��
			//int playTimeSum = Convert.ToInt32(member.GetPlayTimeSum(UserLoginInfo.UserID)); //��
			int playTimeSum = Convert.ToInt32( member.GetAllPlayTimeByUserID( UserLoginInfo.UserID ) ); //��
			int usedPlayTime = member.GetUsedPlayTimeOfJP( UserLoginInfo.UserID ) * 60 * 60;
			int awardTimes = AwardTime * 60 * 60;
			if( ( playTimeSum - usedPlayTime ) < awardTimes )
			{
				/*lblMsg.Text = "��Ǹ������Ч������Ϸʱ�䲻���һ��˽�Ʒ��";
				*/
				CommonManager.Web.RegJs( this, "alert('��Ǹ������Ч������Ϸʱ�䲻���һ��˽�Ʒ��');location.href=location.href;", true );
				return;
			}

			//3.�����еĽ�Ҳ����һ��˽�Ʒ��
            //Int64 userBankMoney = member.GetBankMoney( UserLoginInfo.UserID );
            //if( userBankMoney < AwardMoney )
            //{
            //    /*lblMsg.Text = "��Ǹ�������еĽ�Ҳ����һ��˽�Ʒ��";
            //   */
            //    CommonManager.Web.RegJs( this, "alert('��Ǹ������"+ UiCommon.StringConfig.GoldName +"�������һ��˽�Ʒ��');location.href=location.href;", true );
            //    return;
            //}

			//4.�һ�
			string trueName = Common.FilterString( txtTrueName.Text );
			string phone = Common.FilterString( txtPhone.Text );
			string address = Common.FilterString( txtAddress.Text );
			string remark = Common.FilterString( txtUserRemark.Text );
	        //long summMoney = member.GetAllMoney( UserLoginInfo.UserID );
            //int iRows = member.DuiJiang( AwardID, UserLoginInfo.UserID, AwardMoney, AwardTime, trueName, phone, address, remark);
            //if( iRows > 0 )
            //{
            //    BLL.Member.WriteMoneyExChangeLog( UserLoginInfo.UserID, summMoney, -AwardMoney, (int)Library.MoneyChangeType.�һ���Ʒ, "" );
            //    CommonManager.Web.RegJs( this, "alert('" + ConfigurationManager.AppSettings["DuiJingMgs"] + "');location.href=location.href;", true );
            //    /*lblMsg.Text = "��Ʒ�һ��ɹ���" + DateTime.Now;
            //    BindJiangPin();
            //    txtTrueName.Text = string.Empty;
            //    txtPhone.Text = string.Empty;
            //    txtAddress.Text = string.Empty;*/
            //}
            //else
            //{
            //    //lblMsg.Text = "��Ǹ����Ʒ�һ�ʧ�ܣ������Ƿ�������æ�����Ժ����ԣ�";
            //    CommonManager.Web.RegJs( this, "alert('��Ǹ����Ʒ�һ�ʧ�ܣ������Ƿ�������æ�����Ժ����ԣ�');location.href=location.href;", true );
            //}
            Dictionary<string, object> dic = new Dictionary<string, object>();
            DbSession.Default.FromProc("Web_pAwardBuy").AddInputParameter("@UserID", DbType.Int32, UiCommon.UserLoginInfo.UserID)
                .AddInputParameter("@Award_ID", DbType.Int32, AwardID)
                .AddInputParameter("@TrueName",DbType.String,trueName)
                .AddInputParameter("@Phone",DbType.String,phone)
                .AddInputParameter("@Address",DbType.String,address)
                .AddInputParameter("@UserRemark",DbType.String,remark)
                .AddInputParameter("@Award_OnLineTime",DbType.Int32,AwardTime)
                .AddReturnValueParameter("@ReturnValue", DbType.Int32)
               .Execute(out dic);
            if (dic.Count <= 0)
                CommonManager.Web.RegJs(this, "alert('��Ǹ������Ʒʧ�ܣ������Ƿ�������æ�����Ժ����ԣ�');location.href=location.href;", true);

            if (dic["ReturnValue"].ToString() == "-1")
            {
                CommonManager.Web.RegJs(this, "alert('��Ǹ������Ʒʧ�ܣ�����" + UiCommon.StringConfig.GoldName+ "�����㣡');location.href=location.href;", true);
            }
            CommonManager.Web.RegJs(this, "alert('" + ConfigurationManager.AppSettings["DuiJingMgs"] + "');location.href=location.href;", true);
		}
	}
}
