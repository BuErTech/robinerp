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
using System.Collections.Generic;
using Bzw.Data;
using BCST.Common;

namespace Bzw.Inhersits.Manage
{
public partial class Manage_PropClearFasci : UiCommon.ManageBasePage
{
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
    /// Left1 �ؼ���
    /// </summary>
    /// <remarks>
    /// �Զ����ɵ��ֶΡ�
    /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
    /// </remarks>
    protected global::System.Web.UI.UserControl Left1;

    /// <summary>
    /// webuserlogin1 �ؼ���
    /// </summary>
    /// <remarks>
    /// �Զ����ɵ��ֶΡ�
    /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
    /// </remarks>
    protected global::System.Web.UI.UserControl webuserlogin1;

    /// <summary>
    /// form1 �ؼ���
    /// </summary>
    /// <remarks>
    /// �Զ����ɵ��ֶΡ�
    /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
    /// </remarks>
    protected global::System.Web.UI.HtmlControls.HtmlForm form1;

    /// <summary>
    /// ddlType �ؼ���
    /// </summary>
    /// <remarks>
    /// �Զ����ɵ��ֶΡ�
    /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
    /// </remarks>
    protected global::System.Web.UI.WebControls.DropDownList ddlType;

    /// <summary>
    /// lblMsg �ؼ���
    /// </summary>
    /// <remarks>
    /// �Զ����ɵ��ֶΡ�
    /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
    /// </remarks>
    protected global::System.Web.UI.WebControls.Label lblMsg;

    /// <summary>
    /// button5 �ؼ���
    /// </summary>
    /// <remarks>
    /// �Զ����ɵ��ֶΡ�
    /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
    /// </remarks>
    protected global::System.Web.UI.HtmlControls.HtmlInputSubmit button5;

    /// <summary>
    /// rptList �ؼ���
    /// </summary>
    /// <remarks>
    /// �Զ����ɵ��ֶΡ�
    /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
    /// </remarks>
    protected global::System.Web.UI.WebControls.Repeater rptList;

    /// <summary>
    /// webfooter1 �ؼ���
    /// </summary>
    /// <remarks>
    /// �Զ����ɵ��ֶΡ�
    /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
    /// </remarks>
    protected global::System.Web.UI.UserControl webfooter1;
	public int UserFasci
	{
		get
		{
			if( ViewState["UserFascis"] == null )
			{
				return new BLL.Member().GetUserFasci( UiCommon.UserLoginInfo.UserID );
			}
			else
			{
				return Convert.ToInt32( ViewState["UserFascis"] );
			}
		}
		set
		{

			ViewState["UserFascis"] = value;

		}
	}
	protected void Page_Load( object sender, EventArgs e )
	{
		if( !IsPostBack )
		{
			BindPropFasci();
		}
	}
	public void BindPropFasci()
	{
		UserFasci = new BLL.Member().GetUserFasci( UiCommon.UserLoginInfo.UserID );
		DataTable dt = new BLL.DaoJu().GetInfoOfPropFasci();
		if( dt != null )
		{
			this.ddlType.Items.Clear();
			for( int i = 0; i < dt.Rows.Count; i++ )
			{
				this.ddlType.Items.Add( new ListItem( dt.Rows[i]["Title"].ToString(), dt.Rows[i]["ID"].ToString() ) );
			}

			rptList.DataSource = dt;
			rptList.DataBind();
		}
	}

	protected void button5_ServerClick( object sender, EventArgs e )
	{
		if( !CommonManager.Web.CheckPostSource() )
		{
			return;
		}
		int pPrice = 0, pFasci = 0;
		int clearFasci = 0, afterFasci = 0;
		if( UserFasci >= 0 )
		{
			lblMsg.Text = "ֻ������ֵС����ʱ���ſ��Թ���ʹ�ô˵��ߣ�";
			return;
		}
		int iPropID = Convert.ToInt32( ddlType.SelectedValue );//����ID��
		DataRow dr = new BLL.DaoJu().GetPropFasciForID( iPropID );
		if( dr != null )
		{
			pPrice = Convert.ToInt32( dr["Price"].ToString() );// ���߼۸�
			pFasci = Convert.ToInt32( dr["DecFasci"].ToString() );//�������������������
		}

		//�ж����е�Ǯ�Ƿ��㹻
		Int64 bankMoney = new BLL.Member().GetBankMoney( UiCommon.UserLoginInfo.UserID );
		if( bankMoney < pPrice )
		{
            lblMsg.Text = "�������е�" + UiCommon.StringConfig.MoneyName + "���㣬���ֵ���ٹ���";
			return;
		}

		int iLeaveFasci = UserFasci + pFasci;

		if( iLeaveFasci <= 0 )
		{
			clearFasci = pFasci;//�����������ֵ����
			afterFasci = iLeaveFasci;
		}
		else
		{
			clearFasci = pFasci - iLeaveFasci;//�����������ֵ����
			afterFasci = 0;
		}

		//int iRows = new BLL.Member().BuyPropFasci( UiCommon.UserLoginInfo.UserID, iPropID, pPrice, clearFasci, UserFasci, afterFasci );
		Dictionary<string, object> dic = new Dictionary<string, object>();
		DbSession.Default.FromProc( "Web_pClearFasciPropBuyLog" )
			.AddInputParameter( "@UserID", DbType.String, UiCommon.UserLoginInfo.UserID )
			.AddInputParameter( "@FasciPropID", DbType.String, iPropID )
			.AddReturnValueParameter( "@ReturnValue", DbType.Int32 )
			.Execute( out dic );
		if( dic.Count > 0 )
		{
			string mgs = "";
			switch( (int)dic["ReturnValue"] )
			{
				case 1:
					mgs = "ʹ�óɹ������Ѿ�������" + clearFasci + "������ֵ��";
					break;
				case -9:
					mgs = "��Ǹ����������";
					break;
				case -8:
					mgs = "��Ǹ���û������ڣ�";
					break;
				case -7:
					mgs = "��Ǹ����������߲����ڣ�";
					break;
				case -6:
					mgs = "��Ǹ������ֵ������С������ܹ�ִ�����������";
					break;
				default:
					mgs = "��Ǹ������ʧ�ܣ����ܷ�����æ�����Ժ����ԣ�";
					break;				
			}
			CommonManager.Web.RegJs( this, "alert('" + mgs + "');location.href=location.href;", true );
		}
		else
		{
			CommonManager.Web.RegJs( this, "alert('��Ǹ������ʧ�ܣ����ܷ�����æ�����Ժ����ԣ�');location.href=location.href;", true );
		}

	}
}
}
