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

namespace Bzw.Inhersits.Public
{
	public partial class Public_NewsTypeList : System.Web.UI.UserControl
	{

		/// <summary>
		/// rpNewType �ؼ���
		/// </summary>
		/// <remarks>
		/// �Զ����ɵ��ֶΡ�
		/// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
		/// </remarks>
		protected global::System.Web.UI.WebControls.Repeater rpNewType;

		protected void Page_Load( object sender, EventArgs e )
		{
			BLL.News news = new BLL.News();
			rpNewType.DataSource = news.TypeList().DefaultView;
			rpNewType.DataBind();
		}
	}
}
