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
using Bzw.Data;
using UiCommon;
using Bzw.WebLibrary;

namespace Bzw.Inhersits.UserInfo
{
	public partial class UserInfo_Default : UiCommon.BasePage
	{
		/// <summary>
		/// webtop1 �ؼ���
		/// </summary>
		/// <remarks>
		/// �Զ����ɵ��ֶΡ�
		/// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
		/// </remarks>
		protected global::System.Web.UI.UserControl webtop1;

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
		protected DataRow InfoRow;
		protected string UserHeader = "";

		private string un
		{
			get
			{
				return Server.HtmlEncode( CommonManager.Web.RequestUrlDecode( "params", "" ) );
			}
		}

		protected void Page_Load( object sender, EventArgs e )
		{
            SeoSetting(SeoConfig.Config.DefaultSeo);
			if( !string.IsNullOrEmpty( un ) )
			{
				BLL.Member member = new BLL.Member();
				//InfoRow = member.GetInfoByName( un );
				InfoRow = member.GetInfo( un );

				int LogoID = (int)InfoRow["LogoID"];
				int Sex = Convert.ToInt32( InfoRow["Sex"] );
				LogoID = member.GetLogoID( Sex, LogoID );
				int UserId = Convert.ToInt32( InfoRow["UserID"] );
				if( LogoID >= BLL.Member.LogoId_Const1 && LogoID <= BLL.Member.LogoId_Const2 )//ϵͳĬ��ͷ�� 1<=LogoID<=255
				{
					if( int.Parse( InfoRow["Sex"].ToString() ) == 1 )
					{
						UserHeader = "/Image/Users/b_" + LogoID + ".png";
					}
					else
					{
						UserHeader = "/Image/Users/g_" + LogoID + ".png";
					}
				}
				else if( LogoID >= BLL.Member.LogoId_Const3 && LogoID <= BLL.Member.LogoId_Const4 ) //�ϴ����Զ���ͷ�� 256<=LogoID<=512  0x100   0x200
				{
					int childDirName = Convert.ToInt32( UserId ) / 1000;
					UserHeader = "/Upload/" + UiCommon.StringConfig.UserCustHeadFolderName + "/" + childDirName + "/" + UserId + ".png";
				}
				else if( LogoID > BLL.Member.LogoId_Const4 ) //�ϴ����Զ���ͷ�� LogoID>512  ����˻��й���ʱ����Զ���ͷ��
				{
					int childDirName = Convert.ToInt32( UserId ) / 1000;
					UserHeader = "/Upload/" + UiCommon.StringConfig.UserCustHeadFolderName + "/" + childDirName + "/" + UserId + ".png";
				}
			}
		}
	}
}
