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
	public partial class News2 : UiCommon.BasePage
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
		/// rpNewType �ؼ���
		/// </summary>
		/// <remarks>
		/// �Զ����ɵ��ֶΡ�
		/// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
		/// </remarks>
		protected global::System.Web.UI.WebControls.Repeater rpNewType;

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
		public string NewTypeId = "0";
		public string NewTypeName = string.Empty;

		public string NewTitle = string.Empty;
		public string NewContent = string.Empty;
		public string NewAuthor = string.Empty;
		public string NewFrom = string.Empty;
		public string Hits = string.Empty;
		public string AddTime = string.Empty;
		public string TitleColor = string.Empty;

		BLL.News news = new BLL.News();

		protected void Page_Load( object sender, EventArgs e )
		{


			if( !IsPostBack )
			{
			//	BindNewType();
				BindNew();
			}
		}

		public void BindNewType()
		{
			rpNewType.DataSource = news.TypeList().DefaultView;
			rpNewType.DataBind();
		}

		public void BindNew()
		{
			string id = Utility.Common.SqlEncode( Utility.Common.GetStringOfUrl( "id" ) );

			if( string.IsNullOrEmpty( id ) || !BCST.Common.CommonManager.String.IsInteger( id ) )
				return;

			news.UpdateHits( id );

			DataRow dr = news.GetInfo( id );
			if( dr != null )
			{
				NewTypeId = dr["Typeid"].ToString();
				NewTypeName = dr["TypeName"].ToString();
				TitleColor = dr["TitleColor"].ToString();
				NewTitle = dr["Title"].ToString();
				NewContent = dr["Content"].ToString();
				NewAuthor = dr["Author"].ToString();
				NewFrom = dr["News_From"].ToString();
				Hits = dr["Hits"].ToString();
				AddTime = ( (DateTime)dr["Issue_Time"] ).ToString( "yyyy��MM��dd��" );

				#region ���⼰SEO����

				string seoDesc = Utility.Common.ClearHtml( Utility.Common.CutStringUnicode( NewContent, 300, "..." ) );

				SeoNewsSetting( NewTitle, dr["Keywords"].ToString(), seoDesc );


				#endregion
			}


		}
	}

}
