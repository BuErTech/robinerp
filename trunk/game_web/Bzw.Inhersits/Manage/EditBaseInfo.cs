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
using BCST.Common;

namespace Bzw.Inhersits.Manage
{
    public partial class Manage_EditBaseInfo : UiCommon.ManageBasePage
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
    /// frmLogin �ؼ���
    /// </summary>
    /// <remarks>
    /// �Զ����ɵ��ֶΡ�
    /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
    /// </remarks>
    protected global::System.Web.UI.HtmlControls.HtmlForm frmLogin;
    
    /// <summary>
    /// txtPassword �ؼ���
    /// </summary>
    /// <remarks>
    /// �Զ����ɵ��ֶΡ�
    /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
    /// </remarks>
    protected global::System.Web.UI.WebControls.TextBox txtPassword;
    
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

        public string NickName = string.Empty;
        public int Sex = 1;
        public string CustImgDir = string.Empty;
        public int LogoID = 0;
        public string Email = string.Empty;
        public string Phone = string.Empty;
        public string qqNum = string.Empty;
        public string needqq = System.Configuration.ConfigurationSettings.AppSettings["qq"];

        protected string province = "";
        protected string city = "";
        protected string OccuPation = "";
        protected string address = "";

        protected void Page_Load(object sender, EventArgs e)
        {
            if (!IsPostBack)
            {
                BindInfo();
            }
        }

        protected void BindInfo()
        {
            BLL.Member member = new BLL.Member();
            DataRow dr = member.GetInfo(UserLoginInfo.UserID);
            if (dr == null)
            {
                return;
            }

            NickName = dr["NickName"].ToString();
            Sex = Convert.ToInt32(dr["Sex"]);
            LogoID = Convert.ToInt32(dr["LogoID"]);
            LogoID = member.GetLogoID(Sex, LogoID);
            if (LogoID > BLL.Member.LogoId_Const2)
            {
                int childDirName = Convert.ToInt32(UserLoginInfo.UserID) / 1000;
                CustImgDir = "/Upload/" + UiCommon.StringConfig.UserCustHeadFolderName + "/" + childDirName + "/" + UserLoginInfo.UserID + ".png";
            }
            Email = dr["Email"].ToString();

            Phone = dr["Phone"].ToString();

            qqNum = dr["qqnum"].ToString();
            OccuPation = dr["OccuPation"].ToString();
            address = dr["AdrZone"].ToString();
            province = dr["AdrProvince"].ToString();
            city = dr["AdrCity"].ToString();
        }

        protected void button5_ServerClick(object sender, EventArgs e)
        {
            BLL.Member member = new BLL.Member();
            string password = Common.GetStringOfForm("txtPassword");//txtPassword.Text.Trim();
            if (!member.CompareLoginPassword(UserLoginInfo.UserID, password))
            {
                //Label1.Text = "�Բ���������ľ���������";
                CommonManager.Web.RegJs(this, "alert('�Բ���������ľ���������');location.href=location.href;", true);
                BindInfo();
                return;
            }

            //�ǳƿ��������κ��ַ�������Ҫ��������HTML���룬��ֹ����ľ��ű��ַ���������"'"Ϊ"''"������SQLע��ʽ����
            string nickname = Server.HtmlEncode(Common.GetStringOfForm("nickname").Replace("'", "''"));
            string sex = Common.FilterString(Common.GetStringOfForm("sex"));
            string pwd = Common.FilterString(Common.GetStringOfForm("pwd"));
            string email = Common.FilterString(Common.GetStringOfForm("email"));
            string mobile = Common.FilterString(Common.GetStringOfForm("mobile"));
            string logoid = Common.FilterString(Common.GetStringOfForm("hidLogoID"));
            string qqnum = Common.FilterString(Common.GetStringOfForm("qqnum"));
            logoid = member.SetLogoID(Convert.ToInt32(sex.Trim()), Convert.ToInt32(logoid.Trim())).ToString();

            string _address = Server.HtmlEncode( CommonManager.Web.RequestForm( "txtAddress", "" ) );
            string _province = CommonManager.Web.RequestForm( "hidProvince", "" );
            string _city = CommonManager.Web.RequestForm( "hidCity", "" );
            string _occu = CommonManager.Web.RequestForm( "txtOccuPation", "" );

            //int iRows = member.UpdateBaseInfo(UserLoginInfo.UserID, pwd, nickname, int.Parse(logoid), int.Parse(sex), email, mobile, qqnum);
            int iRows = member.UpdateBaseInfo( UserLoginInfo.UserID, pwd, nickname, int.Parse( logoid ), int.Parse( sex ), email, mobile, _occu, _province, _city, _address,qqnum);
            if (iRows > 0)
            {
                //Label1.Text = "�����޸ĳɹ���" + DateTime.Now;
                CommonManager.Web.RegJs(this, "alert('�����޸ĳɹ���');location.href=location.href;", true);
                BindInfo();
                /*if (Convert.ToInt32(logoid.Trim()) <= BLL.Member.LogoId_Const2)
                {
                    //trImgType.Attributes.Add("style", "display:none");
                    //tbCust.Attributes.Add("style", "display:none");
                }*/
            }
            else
            {
                //Label1.Text = "��Ǹ�������޸�ʧ�ܣ������Ƿ�������æ�����Ժ����ԣ�";
                CommonManager.Web.RegJs(this, "alert('��Ǹ�������޸�ʧ�ܣ������Ƿ�������æ�����Ժ����ԣ�');location.href=location.href;", true);
            }
        }
    }
    
}
