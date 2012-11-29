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
	public partial class ShowGame : UiCommon.AppBasePage
	{
		protected void Page_Load( object sender, EventArgs e )
		{
			string id = Utility.Common.SqlEncode( Utility.Common.GetStringOfUrl( "id" ) );
			if( string.IsNullOrEmpty( id ) || !BCST.Common.CommonManager.String.IsInteger( id ) )
			{
				Response.Write( "����Ƿ����ʣ�" );
				Response.End();
				return;
			}
			else
			{
				//��̬ҳ����ڳ�����KEY
				string Key_Default_Build = "Key_GameRule_Build";
				//��̬ҳ��Url
				string dongPageUrl = "/app/ShowGameHtml.aspx?id=" + id;
				//��̬ҳ��Url
				string jingPageUrl = "/GameInfo_Client_" + id + ".html";
				//��̬�ļ���Ÿ�Ŀ¼
				string RootFilePath = "/Html/GameInfo";

				string returnPageUrl = new UiCommon.BuildStatic().PageBuild( Key_Default_Build, dongPageUrl, jingPageUrl, RootFilePath );
				Response.Redirect( returnPageUrl );
			}


			/*
			if (Request["id"] != null)
			{

				string nameID = Request["id"].ToString();
				if (!BCST.Common.CommonManager.String.IsInteger(nameID))
				{
					Response.Write("�������ó���!");
					Response.End();
				}

				////ת�����ɾ�̬ҳ��
				//Response.Redirect("/Html/GameInfo/" + nameID + ".html");

				Response.Redirect("/app/ShowGameHtml.aspx?id=" + nameID);
			}*/

		}
	}
}
