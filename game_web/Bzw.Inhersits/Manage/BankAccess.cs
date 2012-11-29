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

    public partial class Manage_BankAccess : UiCommon.ManageBasePage
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
        /// Form1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlForm Form1;

        /// <summary>
        /// lblMsg �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.Label lblMsg;

        /// <summary>
        /// lteBank �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.Literal lteBank;

        /// <summary>
        /// lteWallet �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.Literal lteWallet;

        /// <summary>
        /// rdoType �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.RadioButtonList rdoType;

        /// <summary>
        /// txtTwoPassword �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.TextBox txtTwoPassword;

        /// <summary>
        /// txtMoney �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.TextBox txtMoney;
        /// <summary>
        /// txtMoney2 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.TextBox txtMoney2;

        /// <summary>
        /// button5 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlInputSubmit button5;

        /// <summary>
        /// ddlGame �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.DropDownList ddlGame;

        /// <summary>
        /// rpList �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.Repeater rpList;

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

        BLL.Member member = new BLL.Member();
        public int Rowid = 0;

        protected int PageIndex
        {
            get
            {
                string pi = CommonManager.Web.Request("page", "");
                if (string.IsNullOrEmpty(pi) || !CommonManager.String.IsInteger(pi))
                    return 1;
                return int.Parse(pi);
            }
        }	
        protected void Page_Load(object sender, EventArgs e)
        {
            if (member.IsInRoomOrGame(UiCommon.UserLoginInfo.UserName))
            {
                //lblMsg.Text = "��Ǹ�������ʺ��ѽ��뷿���л�������Ϸ�У����ܽ��д�ȡ�������";
                BCST.Common.CommonManager.Web.RegJs(this, "alert('��Ǹ�������ʺ��ѽ��뷿���л�������Ϸ�У����ܽ��д�ȡ�������');history.back();", true);
                return;
            }

            if (!IsPostBack)
            {
                BindUserMoney();
            }
        }

        public void BindUserMoney()
        {
            if (UiCommon.StringConfig.BankVersion == "1")
            {
                lteBank.Text = UiCommon.StringConfig.AddZeros(member.GetBankMoney(UiCommon.UserLoginInfo.UserID));
                lteWallet.Text = UiCommon.StringConfig.AddZeros(member.GetWalletMoney(UiCommon.UserLoginInfo.UserID));
            }
            else
            {
                lteBank.Text = UiCommon.StringConfig.AddZeros(member.GetBankMoney(UiCommon.UserLoginInfo.UserID));
                //����Ϸ�б�
                DataTable ds1 = DbSession.Default.FromSql("SELECT * FROM dbo.TGameNameInfo").ToDataTable();
                if (ds1.Rows.Count > 0)
                {
                    ddlGame.DataSource = ds1;

                    ddlGame.DataTextField = "ComName";
                    ddlGame.DataValueField = "NameID";
                    ddlGame.DataBind();
                }

                DataSet ds = DbSession.Default.FromProc("Web_pGetDataPager")
           .AddInputParameter("@PageSize", DbType.Int32, anpPageIndex.PageSize)
           .AddInputParameter("@CurrentPage", DbType.Int32, PageIndex)
           .AddInputParameter("@Columns", DbType.String, "*")
           .AddInputParameter("@TableName", DbType.String, "Web_VUserGameMoney")
           .AddInputParameter("@Wheres", DbType.String, " and userid="+UiCommon.UserLoginInfo.UserID)
           .AddInputParameter("@KeyName", DbType.String, "GameID")
           .AddInputParameter("@OrderBy", DbType.String, "GameID")
           .ToDataSet();
                if (ds.Tables.Count > 0 && ds.Tables[1].Rows.Count > 0)
                {
                    anpPageIndex.RecordCount = (int)ds.Tables[0].Rows[0][0];
                    anpPageIndex.CurrentPageIndex = PageIndex;

                    rpList.DataSource = ds.Tables[1];
                    rpList.DataBind();
                    rpList.Visible = true;

                }
                else
                {
                    rpList.Visible = false;
                }		
            }
        }

        protected void button5_ServerClick(object sender, EventArgs e)
        {

            if (string.IsNullOrEmpty(txtTwoPassword.Text.Trim()))
            {
                lblMsg.Text = "�������������룡";
                return;
            }
            if (!member.CompareTwoPassword(UiCommon.UserLoginInfo.UserID, txtTwoPassword.Text.Trim()))
            {
                lblMsg.Text = "�Բ�����������������벻��ȷ��";
                return;
            }
            int opType = Convert.ToInt32(rdoType.SelectedValue);

           
            long ioMoney=0;
            int GameNameID=0;
            if (UiCommon.StringConfig.BankVersion == "1")
            {
                if (Convert.ToInt64(txtMoney.Text.Trim()) <= 0)
                {
                    lblMsg.Text = "��ȡ�������Ǵ������������";
                    return;
                }
                ioMoney = Convert.ToInt64(UiCommon.StringConfig.RemoveZeros(txtMoney.Text.Trim()));
            }
            else
            {
                if (Convert.ToInt64(txtMoney2.Text.Trim()) <= 0)
                {
                    lblMsg.Text = "��ȡ�������Ǵ������������";
                    return;
                }
                opType = 0;
                GameNameID=Convert.ToInt32( ddlGame.SelectedValue);
                ioMoney = Convert.ToInt64(UiCommon.StringConfig.RemoveZeros(txtMoney2.Text.Trim()));
            }
            //ioMoney�����㣬��˵������Ľ������õı���С������Ľ������Ҫ�ﵽ�����Ķ�����
            if (ioMoney <= 0)
            {
                lblMsg.Text = "��Ǹ����������ڻ����" + UiCommon.StringConfig.GetMultiple();
                return;
            }

            Dictionary<string, object> dic = new Dictionary<string, object>();
            DbSession.Default.FromProc("Web_pBankMoneyOpera")
                .AddInputParameter("@UserID", DbType.Int32, UiCommon.UserLoginInfo.UserID)
                .AddInputParameter("@MoneyQuantity", DbType.Int64, ioMoney)
                .AddInputParameter("@IsSave", DbType.Int32, opType)
                .AddInputParameter("@NameID", DbType.Int32, GameNameID)
                .AddReturnValueParameter("@ReturnValue", DbType.Int32)
                .Execute(out dic);
            string location = string.Empty;
            if (dic.Count > 0)
            {
                string msg = string.Empty;
                switch ((int)dic["ReturnValue"])
                {
                    case 1:
                        msg = "�����ɹ���";
                        location = "location.href=location.href;";
                        break;

                    case -1:
                    default:
                        msg = "��Ǹ������������δ֪�������Ժ����ԣ�";
                        break;
                    case -2:
                        msg = "��������Ĵ�ȡ���ܴ���Ǯ���е���";
                        break;
                    case -3:
                        msg = "ȡ�������Ĵ�ȡ���ܴ��������еĴ�";
                        break;
                }
                CommonManager.Web.RegJs(this, "alert('" + msg + "');" + location, true);
            }
            else
            {
                CommonManager.Web.RegJs(this, "alert('��Ǹ������������δ֪�������Ժ����ԣ�');history.back();", false);
            }
        }
    }
}
