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
    public partial class Down : UiCommon.BasePage
	{
		protected void Page_Load( object sender, EventArgs e )
		{
			//��̬ҳ����ڳ�����KEY
			string Key_Default_Build = "Key_Down_Build";
			//��̬ҳ��Url
			string dongPageUrl = "/Down2.aspx";
			//��̬ҳ��Url
			string jingPageUrl = "/Down.html";
			//��̬�ļ���Ÿ�Ŀ¼
			string RootFilePath = "/Html";

			string returnPageUrl = new UiCommon.BuildStatic().PageBuild( Key_Default_Build, dongPageUrl, jingPageUrl, RootFilePath );
			Response.Redirect( returnPageUrl );
		}
	}
}
