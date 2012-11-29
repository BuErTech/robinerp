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
	public partial class ShowGameHtml : UiCommon.AppBasePage
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
		/// form1 �ؼ���
		/// </summary>
		/// <remarks>
		/// �Զ����ɵ��ֶΡ�
		/// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
		/// </remarks>
		protected global::System.Web.UI.HtmlControls.HtmlForm form1;
		public string GameName = string.Empty;
		public string GameInfo = "�ǳ���Ǹ,��δ��Ӹ���Ϸ�Ĺ�������,���ǻᾡ������!";

		protected void Page_Load( object sender, EventArgs e )
		{
			if( !IsPostBack )
			{
				string id = Utility.Common.SqlEncode( Utility.Common.GetStringOfUrl( "id" ) );
				if( string.IsNullOrEmpty( id ) || !BCST.Common.CommonManager.String.IsInteger( id ) )
				{
					Response.Write( "�������ó���!" );
					Response.End();
					return;
				}

				string sql = "select top 1 Game_Rule,Game_Name from Web_GameInfo  where game_id=" + id;
				IDataReader dr = Utility.SqlHelper.ExecuteReader( CommandType.Text, sql, null );
				if( dr.Read() )
				{

					GameInfo = dr.GetString( 0 ).ToString();//��Ϸ�Ĺ�������
					GameName = dr.GetString( 1 ).ToString();
				}
				else
				{
					Response.Write( "�ǳ���Ǹ,��δ��Ӹ���Ϸ�Ĺ�������,���ǻᾡ������!" );
					Response.End();

				}
				dr.Close();
				dr.Dispose();


			}
		}
	}
}
