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
using System.Text;
using BCST.Common;
using Bzw.Data;

namespace Bzw.Inhersits
{
    public partial class PropInfo : UiCommon.ManageBasePage
    {
        /// <summary>
        /// rpList �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.Repeater rpList;

        public int PageSize = 4;
    //    public int CurrentNum = 1;
        public int MaxPage = 999;

        public string PropID = string.Empty;
        public string PropName = string.Empty;
        public string PropPrice = string.Empty;
        public string PropVipPrice = string.Empty;
        public string PropDescript = string.Empty;
        public string PropImg = "1";
        public string bgColor = string.Empty;
        public bool IsVip = false;
        public string UserPropCount = string.Empty;
        protected void Page_Load(object sender, EventArgs e)
        {
            PropID = CommonManager.Web.Request("propid", "");
            bgColor = CommonManager.Web.Request("bgcolor", "cccccc");
            //ȡ��ǰҳ��
            //if (Request["num"] != null)
            //{
            //    CurrentNum = Convert.ToInt32(Request["num"]);
            //}
            if (!IsPostBack)
            {
                string sql = "select * from TPropDefine where PropID=@PropID";
                DataTable dt = DbSession.Default.FromSql(sql)
                    .AddInputParameter("@PropID", DbType.Int32, PropID)
                    .ToDataTable();
                if (dt.Rows.Count > 0)
                {
                    //ȡ��һ������
                    PropImg = dt.Rows[0]["PropID"].ToString();
                    PropName = dt.Rows[0]["PropName"].ToString();
                    PropPrice = dt.Rows[0]["Price"].ToString();
                    PropVipPrice = dt.Rows[0]["VipPrice"].ToString();
                    PropDescript = dt.Rows[0]["Descript"].ToString();
                    PropImg = PropID;//

                    //������
                    /*rpList.DataSource = dt.DefaultView;
                    rpList.DataBind();*/
                }
                try
                {
                    UserPropCount = DbSession.Default.FromSql("select ISNULL(holdcount,0) from TUserProp where userid=" + UiCommon.UserLoginInfo.UserID + " and propid=" + PropID).ToScalar().ToString();
                }
                catch { UserPropCount = "0"; }
                
         //   BindVip();
            }
        }
        public string GetBgColor()
        {
            if (Request["bgcolor"] != null && Request["bgcolor"].ToString().Trim().Length > 0)
            {
                return Request["bgcolor"].ToString();
            }

            return "ffeccd";
        }

        //�ж��û��Ƿ���vip
        public void BindVip()
        {
            string VipTime2;
            DataRow dr = new BLL.Member().GetInfoInTUserInfo(UiCommon.UserLoginInfo.UserID);
            if (dr != null)
            {
                VipTime2 = dr["VipTime2"].ToString();
                DateTime dtInit = new DateTime(1970, 01, 01, 08, 00, 00);
                DateTime dtNow = DateTime.Now;

                if (Convert.ToDateTime(VipTime2) > dtInit)
                {
                    if (dtNow <= Convert.ToDateTime(VipTime2))
                    {
                        IsVip = true;
                    }
                }
            }
        }
        public void DaojuList()
        {
            #region �ɽ����е��߶������Ĵ���
            /*//��¼����
			string sql = "select count(PropID) from TPropDefine";
			int rsCount = Convert.ToInt32( Utility.SqlHelper.ExecuteScalar( CommandType.Text, sql, null ) );
			//ȡ��ǰҳ��
			if( Request["page"] != null )
			{
				CurrentPage = Convert.ToInt32( Request["page"] );
			}
			//���ҳ��
			if( rsCount % PageSize == 0 )
			{
				MaxPage = rsCount / PageSize;
			}
			else
			{
				MaxPage = Convert.ToInt32( Math.Floor( Convert.ToDouble( rsCount / PageSize ) ) ) + 1;
			}
			//Response.Write("<br>" + MaxPage);

			//ȡ��ǰҳ����
			if( CurrentPage == 1 )
			{
				sql = "select top " + PageSize + " * from TPropDefine order by PropID";
			}
			else if( CurrentPage > 1 && CurrentPage < MaxPage )
			{
				sql = "select top " + PageSize + " * from TPropDefine where PropID not in(select top " + ( PageSize * ( CurrentPage - 1 ) ) + " PropID from TPropDefine order by PropID) order by PropID";
			}
			else if( CurrentPage >= MaxPage )
			{
				CurrentPage = MaxPage;
				sql = "select * from TPropDefine where PropID not in(select top " + ( PageSize * ( CurrentPage - 1 ) ) + " PropID from TPropDefine order by PropID) order by PropID";
			}
			//Response.Write("<br>"+sql);*/
            #endregion



        }

    }
}
