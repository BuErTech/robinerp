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
using Utility;
using Bzw.Data;
using System.Collections.Generic;
using BCST.Common;
namespace Bzw.Inhersits.Manage
{
    public partial class Manage_FasciExchange : UiCommon.ManageBasePage
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
        /// webfooter1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.UserControl webfooter1;

        /// <summary>
        /// anpPageIndex �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::Wuqi.Webdiyer.AspNetPager anpPageIndex;
        /// <summary>
        /// rpList �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.Repeater rpList;
    

        public int Fascination = 0;
      
        //���
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

            if (!IsPostBack)
            {
                if (Request.QueryString["pid"] != null && Request.QueryString[""] != "pid")
                {
                    Exchange();
                }
                else
                {
                    GetUserFascination();
                    DataSet ds = DbSession.Default.FromProc("Web_pGetDataPager")
               .AddInputParameter("@PageSize", DbType.Int32, anpPageIndex.PageSize)
               .AddInputParameter("@CurrentPage", DbType.Int32, PageIndex)
               .AddInputParameter("@Columns", DbType.String, "*")
               .AddInputParameter("@TableName", DbType.String, "Web_VCharmExchange")
               .AddInputParameter("@Wheres", DbType.String, " and CharmValue<=(SELECT  Fascination FROM dbo.TUserInfo WHERE UserID=" + UiCommon.UserLoginInfo.UserID + ")")
               .AddInputParameter("@KeyName", DbType.String, "ID")
               .AddInputParameter("@OrderBy", DbType.String, "ID desc")
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
           
        }
        //����������ֵ��
        public void GetUserFascination()
        {
            DataRow dr = BLL.Member.GetInfoOfFascination(UiCommon.UserLoginInfo.UserID);
            if (dr != null)
            {
                Fascination = Convert.ToInt32(dr["Fascination"]);
            }
        }
       //����ֵ�һ�����

        public void Exchange()
        {
            string pid = CommonManager.Web.Request("pid", "");
            Dictionary<string, object> dic = new Dictionary<string, object>();
            DbSession.Default.FromProc("Web_CharmExchangeAdd")
                 .AddInputParameter("@UserID", DbType.Int32, UiCommon.UserLoginInfo.UserID)
                 .AddInputParameter("@id", DbType.Int32, pid)
                 .AddReturnValueParameter("@ReturnValue", DbType.Int32)
                 .Execute(out dic);
            string result = dic["ReturnValue"].ToString();
            string msg = "";
            switch (result)
            {
                case "1": msg = "��Ǹ���˶һ����Ͳ����ڣ�"; break;
                case "2": msg = "��Ǹ���˶һ����Ͳ����ڣ�"; break;
                case "3": msg = "��Ǹ����������ֵ���㣡"; break;
                case "4": msg = "��Ǹ����������ֵ���㣡"; break;
                case "5": msg = (Convert.ToInt32(pid) < 10) ? "�һ��ɹ���" : "�һ������ѳɹ��ύ��"; break;
                default: msg = "��Ǹ�������һ�ʧ�ܣ������Ƿ�������æ�����Ժ����ԣ�"; break;
            }
            CommonManager.Web.RegJs(this, "alert('" + msg + "');location.href='FasciExchange.aspx';", false);
        }

         
    }
}
