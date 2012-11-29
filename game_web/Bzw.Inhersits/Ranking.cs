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


namespace Bzw.Inhersits
{
	public partial class Ranking : UiCommon.BasePage
	{
		/// <summary>
		/// Head1 �ؼ���
		/// </summary>
		/// <remarks>
		/// �Զ����ɵ��ֶΡ�
		/// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
		/// </remarks>
		protected global::System.Web.UI.HtmlControls.HtmlHead Head1;

		protected void Page_Load( object sender, EventArgs e )
		{
			string id = Utility.Common.SqlEncode( Utility.Common.GetStringOfUrl( "id" ) );
			if( string.IsNullOrEmpty( id ) || !BCST.Common.CommonManager.String.IsInteger( id ) )
			{
				Response.Redirect( "/RankMoney.aspx" );
			}
			else
			{
				//��̬ҳ����ڳ�����KEY
				string Key_Default_Build = "Key_RankGamePoint_Build";
				//��̬ҳ��Url
				string dongPageUrl = "/Ranking2.aspx?id=" + id + "&name=" + Server.UrlEncode( Utility.Common.GetStringOfUrl( "name" ) );
				//��̬ҳ��Url
				string jingPageUrl = "/RankGamePoint_" + id + ".html";
				//��̬�ļ���Ÿ�Ŀ¼
				string RootFilePath = "/Html/Ranking";

				string returnPageUrl = new UiCommon.BuildStatic().PageBuild( Key_Default_Build, dongPageUrl, jingPageUrl, RootFilePath );
				Response.Redirect( returnPageUrl );
			}
		}

	}

}
