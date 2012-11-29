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
namespace Bzw.Inhersits.Manage
{
    public partial class Manage_GetPassAnswer : UiCommon.BasePage
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
        /// form1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlForm form1;

        /// <summary>
        /// lblMsg �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.Label lblMsg;

        /// <summary>
        /// ddlCardType �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.DropDownList ddlCardType;

        /// <summary>
        /// txtCardNo �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.TextBox txtCardNo;

        /// <summary>
        /// ddlQuestion �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.DropDownList ddlQuestion;

        /// <summary>
        /// txtAnwer �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.TextBox txtAnwer;

        /// <summary>
        /// txtTwoPass �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.TextBox txtTwoPass;

        /// <summary>
        /// button5 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlInputSubmit button5;

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
                BindInfo();
            }
        }

        public void BindInfo()
        {
            if (Session["getpwduser"] == null)
            {
                Response.Redirect("/Manage/GetPass.aspx");
            }

            BindCardType();
            BindQuestion();

            BLL.Member member = new BLL.Member();
            DataRow dr = member.GetInfoByName(Session["getpwduser"].ToString());
            if (dr == null)
            {
                return;
            }
            string dbCardType = dr["ZJ_Type"].ToString();
            //txtCardNo.Text = dr["ZJ_Number"].ToString();
            //txtCardNo2.Text = txtCardNo.Text;
            string dbQuestion = dr["Pwd_ques"].ToString();
            //txtAnwer.Text = dr["Pwd_anw"].ToString();

            for (int i = 0; i < ddlCardType.Items.Count; i++)
            {
                if (ddlCardType.Items[i].Value.ToLower() == dbCardType.Trim().ToLower())
                {
                    ddlCardType.SelectedIndex = i;
                    break;
                }
            }

            for (int i = 0; i < ddlQuestion.Items.Count; i++)
            {
                if (ddlQuestion.Items[i].Value.ToLower() == dbQuestion.Trim().ToLower())
                {
                    ddlQuestion.SelectedIndex = i;
                    break;
                }
            }
        }

        public void BindCardType()
        {
            ddlCardType.Items.Add(new ListItem("��ѡ��", "0"));
            ddlCardType.Items[0].Selected = true;
            string[] ctNames = UiCommon.StringConfig.GetCardTypeNames;
            string[] ctValues = UiCommon.StringConfig.GetCardTypeValues;
            for (int i = 0; i < ctNames.Length; i++)
            {
                ddlCardType.Items.Add(new ListItem(ctNames[i], ctValues[i]));
            }
        }

        public void BindQuestion()
        {
            ddlQuestion.Items.Add(new ListItem("��ѡ��", ""));
            ddlQuestion.Items[0].Selected = true;
            string[] quesNames = UiCommon.StringConfig.GetQuestionNames;
            for (int i = 0; i < quesNames.Length; i++)
            {
                ddlQuestion.Items.Add(new ListItem(quesNames[i], quesNames[i]));
            }
        }

        protected void button5_ServerClick(object sender, EventArgs e)
        {
            string backUrl = "/Manage/GetPass.aspx";

            if (Session["getpwduser"] == null)
            {
                Response.Redirect(backUrl);
            }

            string userName = Utility.Common.SqlEncode(Session["getpwduser"].ToString());
            if (txtCardNo.Text.Trim().Length == 0)
            {
                Response.Redirect(backUrl);
            }
            string cardNo = txtCardNo.Text;

            if (txtAnwer.Text.Trim().Length == 0)
            {
                Response.Redirect(backUrl);
            }
            string answer = txtAnwer.Text;

            //if (hidPwd.Value.Trim().Length == 0)
            //{
            //    Response.Redirect(backUrl);
            //}
            string twoPassword = txtTwoPass.Text; //hidPwd.Value.Trim();


            bool flg = new BLL.Member().ValidGetPassAnswer(userName, cardNo, answer, twoPassword);
            if (flg)
            {
                Session["getpwdanswer"] = "pass";
                Response.Redirect("/Manage/GetPassModify.aspx");
            }
            else
            {
                lblMsg.Text = "��Ǹ�����֤�š�����𰸡��������������һ�������ش����";
            }
        }
    }
}
