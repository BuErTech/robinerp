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
using System.Text;

namespace Bzw.Inhersits.Manage
{
    public partial class Manage_BankTransLog : UiCommon.ManageBasePage
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
        /// form1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlForm form1;

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
        /// PanelTransOut �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.Panel PanelTransOut;

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
        /// PanelTransIn �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.Panel PanelTransIn;

        /// <summary>
        /// rpListIn �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.Repeater rpListIn;

        /// <summary>
        /// AspNetPager1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::Wuqi.Webdiyer.AspNetPager AspNetPager1;

        /// <summary>
        /// webfooter1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.UserControl webfooter1;
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
        protected int type
        {
            get
            {
                string temp = CommonManager.Web.Request("type", "");
                if (!CommonManager.String.IsInteger(temp))
                    return 1;
                return int.Parse(temp);

            }
        }

        protected void Page_Load(object sender, EventArgs e)
        {
            if (type == 0)
            {
                this.PanelTransIn.Visible = false;
                this.PanelTransOut.Visible = true;
            }
            else
            {
                this.PanelTransIn.Visible = true;
                this.PanelTransOut.Visible = false;
            }

            if (!IsPostBack)
            {
                BindLogList();
                BindLogListIn();
            }
        }

        public void BindLogList()
        {
            StringBuilder where = new StringBuilder();
            where.AppendFormat(" and UserName='{0}' ", UiCommon.UserLoginInfo.UserName);
            string bd = CommonManager.Web.RequestUrlDecode("bd", "");
            string ed = CommonManager.Web.RequestUrlDecode("ed", "");
            if (!string.IsNullOrEmpty(bd) && CommonManager.String.IsDateTime(bd))
            {
                where.AppendFormat(" and TransTime>='{0} 00:00:00'", bd);
            }
            if (!string.IsNullOrEmpty(ed) && CommonManager.String.IsDateTime(ed))
            {
                where.AppendFormat(" and TransTime<='{0} 23:59:59'", ed);
            }

            int counts;
            DataTable dt = new BLL.Member().TransLogPagerList(anpPageIndex.PageSize, PageIndex, out counts, where.ToString());
            anpPageIndex.RecordCount = counts;
            anpPageIndex.CurrentPageIndex = PageIndex;

            rpList.DataSource = dt.DefaultView;
            rpList.DataBind();
        }
        public void BindLogListIn()
        {
            //string where = " and UserNameZZ='" + UiCommon.UserLoginInfo.UserName + "'";
            StringBuilder where = new StringBuilder();
            where.AppendFormat(" and UserNameZZ='{0}' ", UiCommon.UserLoginInfo.UserName);
            string bd = CommonManager.Web.RequestUrlDecode("bd", "");
            string ed = CommonManager.Web.RequestUrlDecode("ed", "");
            if (!string.IsNullOrEmpty(bd) && CommonManager.String.IsDateTime(bd))
            {
                where.AppendFormat(" and TransTime>='{0} 00:00:00'", bd);
            }
            if (!string.IsNullOrEmpty(ed) && CommonManager.String.IsDateTime(ed))
            {
                where.AppendFormat(" and TransTime<='{0} 23:59:59'", ed);
            }

            int counts;
            DataTable dt = new BLL.Member().TransLogPagerList(AspNetPager1.PageSize, PageIndex, out counts, where.ToString());
            AspNetPager1.RecordCount = counts;
            AspNetPager1.CurrentPageIndex = PageIndex;
            //WebPager1.ItemCount = counts;
            //Rowid = WebPager1.CurrentPageIndex * WebPager1.PageSize - WebPager1.PageSize;
            rpListIn.DataSource = dt.DefaultView;
            rpListIn.DataBind();
        }

    }
}
