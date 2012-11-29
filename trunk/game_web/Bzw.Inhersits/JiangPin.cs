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
	public partial class JiangPin : UiCommon.BasePage
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
		/// webshortcutkey1 �ؼ���
		/// </summary>
		/// <remarks>
		/// �Զ����ɵ��ֶΡ�
		/// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
		/// </remarks>
		//protected global::Public_WebShortCutKey webshortcutkey1;

		/// <summary>
		/// rpJiangPin �ؼ���
		/// </summary>
		/// <remarks>
		/// �Զ����ɵ��ֶΡ�
		/// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
		/// </remarks>
		protected global::System.Web.UI.WebControls.Repeater rpJiangPin;

		/// <summary>
		/// anpPageIndex �ؼ���
		/// </summary>
		/// <remarks>
		/// �Զ����ɵ��ֶΡ�
		/// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
		/// </remarks>
		protected global::Wuqi.Webdiyer.AspNetPager anpPageIndex;

		/// <summary>
		/// webfooter1 �ؼ���
		/// </summary>
		/// <remarks>
		/// �Զ����ɵ��ֶΡ�
		/// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
		/// </remarks>
		protected global::System.Web.UI.UserControl webfooter1;
		private int PageIndex
		{
			get
			{
				string tmp = CommonManager.Web.Request( "page", "" );
				if( string.IsNullOrEmpty( tmp ) || !CommonManager.String.IsInteger( tmp ) )
					return 1;
				return int.Parse( tmp );
			}
		}

		protected void Page_Load( object sender, EventArgs e )
		{
			SeoSetting( SeoConfig.Config.PrizeSeo );
			BindJiangPin();
		}

		public void BindJiangPin()
		{
			string where = string.Empty;
			int counts = 0;
			DataTable dt = Utility.SqlHelper.ExecuteDataPager( "Web_Award", "Award_ID", "*", "Award_ID", 0, anpPageIndex.PageSize, PageIndex, out counts, where );
			//WebPager1.ItemCount = counts;
			anpPageIndex.RecordCount = counts;
			anpPageIndex.CurrentPageIndex = PageIndex;
			rpJiangPin.DataSource = dt;
			rpJiangPin.DataBind();
		}
	}

}
