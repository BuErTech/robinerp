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
using System.Collections.Generic;

namespace Bzw.Inhersits.Manage
{
    public partial class Manage_PromotionCheckout : UiCommon.ManageBasePage
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
        /// lteTrueName �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.Literal lteTrueName;

        /// <summary>
        /// lteTgMoney �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.Literal lteTgMoney;

        /// <summary>
        /// txtMoney �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.TextBox txtMoney;

        /// <summary>
        /// button5 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlInputSubmit button5;

        /// <summary>
        /// lteCon_MinClearingMoney �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.Literal lteCon_MinClearingMoney;

        /// <summary>
        /// webfooter1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.UserControl webfooter1;
        protected void Page_Load(object sender, EventArgs e)
        {
            if (!IsPostBack)
            {
                BindTgInfo();
            }
        }

        public void BindTgInfo()
        {
            DataRow dr = new BLL.Promotion().GetInfo(UiCommon.UserLoginInfo.UserID);
            if (dr != null)
            {
                lteTrueName.Text = dr["TrueName"].ToString();
                lteTgMoney.Text = UiCommon.StringConfig.AddZeros(dr["MoneyCount"]);
            }

            DataRow dr2 = BLL.Config.GetInfoOfPromotion();
            if (dr2 != null)
            {
                //�ƹ�����������Ҫ�ﵽ�Ľ����
                lteCon_MinClearingMoney.Text = UiCommon.StringConfig.AddZeros(dr2["Con_MinClearingMoney"]);

            }
        }

        protected void button5_ServerClick(object sender, EventArgs e)
        {
            if (!BCST.Common.CommonManager.Web.CheckPostSource())
            {
                return;
            }
            string mgs = string.Empty;
            //Con_MinClearingMoney �ƹ��˻��������Ҫ�ﵽXX��ң����ܹ�����
            int Con_MinClearingMoney = Convert.ToInt32(lteCon_MinClearingMoney.Text.Trim());
            int tgMoney = Convert.ToInt32(lteTgMoney.Text.Trim());
            if (tgMoney < Con_MinClearingMoney)
            {
                mgs = "��Ǹ���ƹ��������Ҫ�ﵽ" + Con_MinClearingMoney + UiCommon.StringConfig.MoneyName+"���ſ��Խ��н��ˣ�";
                CommonManager.Web.RegJs(this, "alert('" + mgs + "');location.href=location.href;", true);
                return;
            }

            int jzMoney = Convert.ToInt32(txtMoney.Text.Trim());


            if (jzMoney > tgMoney)
            {
                mgs = "��Ǹ��������Ľ���" + UiCommon.StringConfig.MoneyName + "�����ܴ��������ƹ����룡";
                CommonManager.Web.RegJs(this, "alert('" + mgs + "');location.href=location.href;", true);
                return;
            }
            //jzMoney�����㣬��˵������Ľ������õı���С������Ľ������Ҫ�ﵽ�����Ķ�����2010-01-26
            jzMoney = Convert.ToInt32(UiCommon.StringConfig.RemoveZeros(jzMoney));
            if (jzMoney <= 0)
            {
                lblMsg.Text = "��Ǹ�����˽�������ڻ����" + UiCommon.StringConfig.GetMultiple();
                return;
            }

            //��������TUserInfo�������ƹ������־Web_ClearingLog
            //int iRows = new BLL.Member().TuiguangCheckout(UiCommon.UserLoginInfo.UserID, jzMoney, Utility.Common.RequestIP);
            Dictionary<string, object> dic = new Dictionary<string, object>();
            DbSession.Default.FromProc("Web_pTuiguangCheckout")
                .AddInputParameter("@UserID", DbType.Int32, UiCommon.UserLoginInfo.UserID)
                .AddInputParameter("@OPerateMoney", DbType.Int32, jzMoney)
                .AddInputParameter("@OperateIP", DbType.String, Utility.Common.RequestIP)
                .AddOutputParameter("@Result", DbType.Int32, 4)
                .Execute(out dic);
            if (dic.Count > 0 && (int)dic["Result"] == 0)
            {
                mgs = "���˳ɹ�������" + UiCommon.StringConfig.MoneyName + "��Ϊ" + UiCommon.StringConfig.AddZeros(jzMoney) +UiCommon.StringConfig.MoneyName+ "��" + DateTime.Now;
                CommonManager.Web.RegJs(this, "alert('" + mgs + "');location.href=location.href;", true);
            }
            else if (dic.Count > 0 && (int)dic["Result"] == 1)
            {
                mgs = "��Ǹ�������ƹ�����" + UiCommon.StringConfig.MoneyName + "���㣡";
                CommonManager.Web.RegJs(this, "alert('" + mgs + "');location.href=location.href;", true);
            }
            else
            {
                mgs = "��Ǹ�����˲���ʧ�ܣ������Ƿ�������æ�����Ժ����ԣ�";
                CommonManager.Web.RegJs(this, "alert('" + mgs + "');location.href=location.href;", true);
            }
        }
    }
}
