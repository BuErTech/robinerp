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
using Bzw.WebLibrary;

namespace Bzw.Inhersits
{
	public partial class DaoJuGM : UiCommon.ManageBasePage
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
		/// webfooter1 �ؼ���
		/// </summary>
		/// <remarks>
		/// �Զ����ɵ��ֶΡ�
		/// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
		/// </remarks>
		protected global::System.Web.UI.UserControl webfooter1;

		public string PropID = "0";
		public string PropName = string.Empty;
		public string VipPrice = string.Empty;
		public string Price = string.Empty;
		public string PropDesc = string.Empty;

		protected void Page_Load( object sender, EventArgs e )
		{
			if( !IsPostBack )
			{
                SeoSetting(SeoConfig.Config.DefaultSeo);
				BindInfo();
			}
		}

		public void BindInfo()
		{
           
			PropID = Utility.Common.GetStringOfUrl( "id" ).ToString();
			DataRow dr = new BLL.DaoJu().GetInfo( Utility.Common.SqlEncode( PropID ) );
			if( dr != null )
			{
				PropName = dr["PropName"].ToString();
				VipPrice = dr["VipPrice"].ToString();
				Price = dr["Price"].ToString();
				PropDesc = dr["Descript"].ToString();

			}
		}
	}
}
