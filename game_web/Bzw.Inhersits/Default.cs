using System;
using System.Data;
using System.Configuration;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;

namespace Bzw.Inhersits
{
	public partial class Default : UiCommon.BasePage
	{

		protected void Page_Load( object sender, EventArgs e )
		{
			//��̬ҳ����ڳ�����KEY
			string Key_Default_Build = "Key_Default_Build";
			//��̬ҳ��Url
			string dongPageUrl = "/Default2.aspx";
			//��̬ҳ��Url
			string jingPageUrl = "/Default.html";
			//��̬�ļ���Ÿ�Ŀ¼
			string RootFilePath = "/Html";

			string returnPageUrl = new UiCommon.BuildStatic().PageBuild( Key_Default_Build, dongPageUrl, jingPageUrl, RootFilePath );
			Response.Redirect( returnPageUrl );


			/*
			//��̬ҳ����ڳ�����KEY
			string Key_Default_Build = "Key_Default_Build";
			//��̬ҳ���ļ���
			string dongPageUrl = "/Default.aspx";
			//��̬ҳ���ļ���
			string jingPageUrl = "/Default.html";
			//��̬�ļ���Ÿ�Ŀ¼
			string RootFilePath = "/Html";

			//1.��ȡ���ڳ���
			int timeOutCost = 0;
			if (ConfigurationManager.AppSettings[Key_Default_Build] != null)
			{
				string temp = ConfigurationManager.AppSettings[Key_Default_Build];
				if (BCST.Common.CommonManager.String.IsInteger(temp))
				{
					timeOutCost = Convert.ToInt32(temp);
				}
			}

			//2.�������С�ڻ����0��ֱ�ӷ��ʶ�̬ҳ��
			if (timeOutCost <= 0)
			{
				Response.Redirect(dongPageUrl);
			}

			//3.---------
			string path = HttpContext.Current.Server.MapPath(RootFilePath + jingPageUrl);
			DateTime fileLastWriteTime = new DateTime(1970, 1, 1, 8, 0, 0);
			if (File.Exists(path))//�ж��ļ��Ƿ���ڣ��������ȡ�ļ�����޸�ʱ��
			{
				fileLastWriteTime = File.GetLastWriteTime(path);
			}
			//�ļ�����޸�ʱ����Ϲ��ڳ�������С�ڵ�ǰʱ�䣬�������ɾ�̬�ļ�
			if (fileLastWriteTime.AddMinutes(timeOutCost) < DateTime.Now)
			{
				//���ɾ�̬ҳ��
				System.Net.WebRequest rq = System.Net.WebRequest.Create("http://" + Request.ServerVariables["Http_Host"] + dongPageUrl);
				System.Net.HttpWebResponse rp = (System.Net.HttpWebResponse)rq.GetResponse();
				System.IO.Stream pageStream = rp.GetResponseStream();
				using (System.IO.StreamReader sr = new System.IO.StreamReader(pageStream))
				{
					System.IO.File.WriteAllText(path, sr.ReadToEnd(), System.Text.Encoding.UTF8);
				}
				rq.Abort();
				rp.Close();
				pageStream.Close();
			}

			//4.ҳ����ת
			if (File.Exists(path))
			{
				Response.Redirect(RootFilePath + jingPageUrl);
			}
			else
			{
				Response.Redirect(dongPageUrl);
			}
			*/
		}
	}
}
