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

using UiCommon;
using Utility;

namespace Bzw.Inhersits.Manage
{
    public partial class Manage_TwoPass : UiCommon.ManageBasePage
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
        /// txtPassword �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.TextBox txtPassword;

        /// <summary>
        /// dd_1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlTableRow dd_1;

        /// <summary>
        /// ddlCardType �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.DropDownList ddlCardType;

        /// <summary>
        /// dd_3 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlTableRow dd_3;

        /// <summary>
        /// txtCardNo �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.TextBox txtCardNo;

        /// <summary>
        /// dd_5 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlTableRow dd_5;

        /// <summary>
        /// txtCardNo2 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.TextBox txtCardNo2;

        /// <summary>
        /// dd_7 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlTableRow dd_7;

        /// <summary>
        /// ddlQuestion �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.DropDownList ddlQuestion;

        /// <summary>
        /// dd_9 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlTableRow dd_9;

        /// <summary>
        /// txtAnwer �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.TextBox txtAnwer;

        /// <summary>
        /// dd_11 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlTableRow dd_11;

        /// <summary>
        /// txtOldTwoPass �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.TextBox txtOldTwoPass;

        /// <summary>
        /// txtTwoPass �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.TextBox txtTwoPass;

        /// <summary>
        /// txtTwoPass2 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.TextBox txtTwoPass2;

        /// <summary>
        /// button5 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlInputSubmit button5;

        /// <summary>
        /// divNotes �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlGenericControl divNotes;

        /// <summary>
        /// divNotes2 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlGenericControl divNotes2;

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
            BindCardType();
            BindQuestion();

            BLL.Member member = new BLL.Member();
            DataRow dr = member.GetInfo(UserLoginInfo.UserID);
            if (dr == null)
            {
                return;
            }
            string dbCardType = dr["ZJ_Type"].ToString();
           txtCardNo.Text = dr["ZJ_Number"].ToString();
            txtCardNo2.Text = txtCardNo.Text;
            string dbQuestion = dr["Pwd_ques"].ToString();
            txtAnwer.Text = dr["Pwd_anw"].ToString();
            string cardNo = dr["ZJ_Number"].ToString().Trim();
            //��Ϊ�����ϵ�֤�����͡����롢��ʾ���⡢�𰸶��ǲ���Ϊ
            //�յģ����ԣ����֤�����������ݣ��������������ݣ��Ͱ��⼸������������
            if (!string.IsNullOrEmpty(dbQuestion))
            {
                Hidden();
            }


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
            string[] ctNames = StringConfig.GetCardTypeNames;
            string[] ctValues = StringConfig.GetCardTypeValues;
            for (int i = 0; i < ctNames.Length; i++)
            {
                ddlCardType.Items.Add(new ListItem(ctNames[i], ctValues[i]));
            }
        }

        public void BindQuestion()
        {
            ddlQuestion.Items.Add(new ListItem("��ѡ��", ""));
            ddlQuestion.Items[0].Selected = true;
            string[] quesNames = StringConfig.GetQuestionNames;
            for (int i = 0; i < quesNames.Length; i++)
            {
                ddlQuestion.Items.Add(new ListItem(quesNames[i], quesNames[i]));
            }
        }

        //����֤�����͡����롢��ʾ���⡢��������
        public void Hidden()
        {
            /*
            dd_1.Attributes.Add("style", "display:none");
            dd_2.Attributes.Add("style", "display:none");
            dd_3.Attributes.Add("style", "display:none");
            dd_4.Attributes.Add("style", "display:none");
            dd_5.Attributes.Add("style", "display:none");
            dd_6.Attributes.Add("style", "display:none");
            dd_7.Attributes.Add("style", "display:none");
            dd_8.Attributes.Add("style", "display:none");
            dd_9.Attributes.Add("style", "display:none");
            dd_10.Attributes.Add("style", "display:none");*/

            /*txtCardNo.Text = string.Empty;
            txtCardNo2.Text = string.Empty;
            txtAnwer.Text = string.Empty;
            //dd_1.Visible = false;
            //dd_2.Visible = false;
            //dd_3.Visible = false;
            //dd_4.Visible = false;
            dd_5.Visible = false;
            //dd_6.Visible = false;
            //dd_7.Visible = false;
            //dd_8.Visible = false;
            //dd_9.Visible = false;
            //dd_10.Visible = false;*/

            dd_1.Visible = false;

            dd_3.Visible = false;

            dd_5.Visible = false;

            dd_7.Visible = false;

            dd_9.Visible = false;

            divNotes.Visible = false;//ͬʱ������ʾ��Ϣ
            divNotes2.Style.Add("display", "block"); //

            //��������������Ŀ�����ʾ�Ѿ����ù��ܱ����ϣ���ʱ�������޸Ķ������룬ӦҪ������ԭ�������룬�Ա�����������
            dd_11.Visible = true;


        }

        protected void button5_ServerClick(object sender, EventArgs e)
        {
            if (!BCST.Common.CommonManager.Web.CheckPostSource())
                return;

            BLL.Member member = new BLL.Member();
            //string password = hidPwd2.Value.Trim();//txtPassword.Text.Trim();
            string password = txtPassword.Text.Trim();
            if (!member.CompareLoginPassword(UserLoginInfo.UserID, password))
            {
                lblMsg.Text = "�Բ���������ĵ�¼��������";
                return;
            }

            if (dd_11.Visible)
            {
                if (string.IsNullOrEmpty(txtOldTwoPass.Text.Trim()))
                {
                    lblMsg.Text = "������ԭ�������룡";
                    return;
                }
                if (!member.CompareTwoPassword(UserLoginInfo.UserID, txtOldTwoPass.Text.Trim()))
                {
                    lblMsg.Text = "�Բ����������ԭ������������";
                    return;
                }
            }

            string cardType = Common.FilterString(ddlCardType.SelectedValue);
            string cardNo = Common.FilterString(txtCardNo.Text.Trim());
            string question = Common.FilterString(ddlQuestion.SelectedValue);
            string answer = Common.FilterString(txtAnwer.Text.Trim());
            string twoPass = Common.FilterString(txtTwoPass.Text.Trim());

            int iRows = 0;
            if (dd_5.Visible)
            {
                iRows = member.UpdateTwoPassInfo(UserLoginInfo.UserID, cardType, cardNo, question, answer, twoPass);
            }
            else if (!string.IsNullOrEmpty(twoPass))
            {
                // iRows = member.UpdateTwoPass(UserLoginInfo.UserID, twoPass);
                iRows = member.UpdateTwoPass(UserLoginInfo.UserID, cardNo, answer, twoPass);
            }
            else
            {
                lblMsg.Text = "�������벻��Ϊ�գ�" + DateTime.Now;
                return;
            }

            if (iRows > 0)
            {
                if (dd_5.Visible)
                {
                    lblMsg.Text = "�������뱣���������óɹ���" + DateTime.Now;
                }
                else
                {
                    lblMsg.Text = "���������޸ĳɹ���" + DateTime.Now;
                }
                Hidden();
                //lblMsg.Text = "�������뱣�������޸ĳɹ���" + DateTime.Now;
                //Hidden();
            }
            else if (iRows == -9)
            {
                lblMsg.Text = "��Ǹ������д��֤�����������𰸲�����δ���޸��������룬���������룡";
                Hidden();
            }
            else
            {
                lblMsg.Text = "��Ǹ�������޸�ʧ�ܣ������Ƿ�������æ�����Ժ����ԣ�";
            }
        }
    }
}
