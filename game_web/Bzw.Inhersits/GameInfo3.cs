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

namespace Bzw.Inhersits
{
	public partial class GameInfo3 : UiCommon.BasePage
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
		/// rpGameType �ؼ���
		/// </summary>
		/// <remarks>
		/// �Զ����ɵ��ֶΡ�
		/// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
		/// </remarks>
		protected global::System.Web.UI.WebControls.Repeater rpGameType;

		/// <summary>
		/// webservice1 �ؼ���
		/// </summary>
		/// <remarks>
		/// �Զ����ɵ��ֶΡ�
		/// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
		/// </remarks>
		protected global::System.Web.UI.UserControl webservice1;

		/// <summary>
		/// webfooter1 �ؼ���
		/// </summary>
		/// <remarks>
		/// �Զ����ɵ��ֶΡ�
		/// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
		/// </remarks>
		protected global::System.Web.UI.UserControl webfooter1;

		public string GameName = string.Empty;
		public string GameDownAddress = string.Empty;
		public string GameInfo2 = string.Empty;
		public string GameRule = string.Empty;

		protected void Page_Load( object sender, EventArgs e )
		{

			SeoSetting( SeoConfig.Config.GameInfoSeo );
			if( !IsPostBack )
			{
				BindGameType();
				BindGameInfo();
			}
		}

		public void BindGameType()
		{
			rpGameType.DataSource = new BLL.Game().List( 0 );
			rpGameType.DataBind();
		}

		public void BindGameInfo()
		{
			string id = Utility.Common.SqlEncode( Utility.Common.GetStringOfUrl( "id" ) );
			if( string.IsNullOrEmpty( id ) || !Utility.Common.IsNumber( id ) )
			{
				Response.Write( "<script>history.back();</script>" );
				Response.End();
				return;
			}
			DataRow dr = new BLL.Game().GetInfo( id );
			if( dr != null )
			{
				GameName = dr["Game_Name"].ToString();
				GameDownAddress = dr["Down_Address"].ToString();
				GameInfo2 = dr["Game_Info"].ToString();
				GameRule = dr["Game_Rule"].ToString();
			}
		}
	}
}
