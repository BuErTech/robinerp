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
    public partial class News : UiCommon.BasePage
	{
		protected void Page_Load( object sender, EventArgs e )
		{
			string id = Utility.Common.SqlEncode( Utility.Common.GetStringOfUrl( "id" ) );
			if( string.IsNullOrEmpty( id ) || !BCST.Common.CommonManager.String.IsInteger( id ) )
			{
				Response.Write( "<h2>����Ƿ�������</h2>" );
				Response.End();
				return;
			}
			else
			{
				//��̬ҳ����ڳ�����KEY
				string Key_Default_Build = "Key_NewsInfo_Build";
				//��̬ҳ��Url
				string dongPageUrl = "/News2.aspx?id=" + id;
				//��̬ҳ��Url
				string jingPageUrl = "/News_" + id + ".html";
				//��̬�ļ���Ÿ�Ŀ¼
				string RootFilePath = "/Html/NewsInfo";

				string returnPageUrl = new UiCommon.BuildStatic().PageBuild( Key_Default_Build, dongPageUrl, jingPageUrl, RootFilePath );
				Response.Redirect( returnPageUrl );
			}
		}
	}
}
