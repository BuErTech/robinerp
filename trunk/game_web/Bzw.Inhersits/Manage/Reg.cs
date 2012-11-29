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
using UiCommon;
using BCST.Common;
using Bzw.Data;
using System.Collections.Generic;
using BCST.Cache;
using BLL;
using System.Text;
using System.Management;
namespace Bzw.Inhersits.Manage
{
    public partial class Manage_Reg : UiCommon.BasePage
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
        /// webservice1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.UserControl webservice1;

        /// <summary>
        /// frmLogin �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlForm frmLogin;

        /// <summary>
        /// btnSubmit �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlInputSubmit btnSubmit;

        /// <summary>
        /// webfooter1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.UserControl webfooter1;

        public string needqq;
        public string needrealname;
        public string needIDCard;
        public string needProcted;
        protected void Page_Load(object sender, EventArgs e)
        {
            //��ȡע���ʱ��QQ,��ʵ����,���֤�����Ƿ�Ϊ����
            needqq = System.Configuration.ConfigurationSettings.AppSettings["QQ"];
            needrealname = System.Configuration.ConfigurationSettings.AppSettings["RealName"];
            needIDCard = System.Configuration.ConfigurationSettings.AppSettings["IDCard"];
            needProcted = System.Configuration.ConfigurationSettings.AppSettings["IsCardProtected"];//�Ƿ���Ҫ��֤�Ƿ����
            //ip�����жϺ�ʱ�����жϣ������Ϲ�����ҳ�淵�ء�
            DataTable dt = DbSession.Default.FromSql("select top 1 * from Web_Config").ToDataTable();
            if (dt.Rows.Count <= 0)
                return;

            //ip��ֹ����
            string ipset = dt.Rows[0]["IPDisabled"].ToString();
            //�ظ�ע����ʱ��
            int timespan = (int)dt.Rows[0]["ReRegTimeSpan"];
         
            string userip = CommonManager.Web.GetIP();
           
            if (!string.IsNullOrEmpty(ipset))
            {
                List<string> iplist = new List<string>(ipset.Split(','));

                if (iplist.Contains("*.*.*.*"))//ȫ������
                {
                    //CommonManager.Web.RegJs( this, "alert('��ǰ��ͣע�ᣬ���Ժ��������ԡ�');history.back;", false );
                    //return;
                    Response.Write("��ǰ��ͣע�ᣬ���Ժ��������ԡ�");
                    Response.End();
                }


                if (
                    iplist.Contains(userip) ||
                    iplist.Contains(CommonManager.String.CutIP(userip, 1)) ||
                    iplist.Contains(CommonManager.String.CutIP(userip, 2)) ||
                    iplist.Contains(CommonManager.String.CutIP(userip, 3))
                )
                {
                    //CommonManager.Web.RegJs( this, "alert('����IP���ڱ����Ƶķ�Χ�ڣ����ܽ���ע�ᣬ����ϵ���ǵĿͷ�������ѯ��');history.back;", false );
                    //return;
                    Response.Write("����IP���ڱ����Ƶķ�Χ�ڣ����ܽ���ע�ᣬ����ϵ���ǵĿͷ�������ѯ��");
                    Response.End();
                }
            }
           
            object lastRegDate = BCSTCache.Default.Get<CookiesCache>("RegDateTime");
            if (lastRegDate != null && CommonManager.String.IsDateTime(lastRegDate.ToString()))
            {
                if ((DateTime.Now - DateTime.Parse(lastRegDate.ToString())).TotalMinutes < timespan)
                {
                    //CommonManager.Web.RegJs( this, "alert('�벻Ҫ�ظ�ע�ᡣ');history.back;", false );
                    //return;
                    Response.Write("�벻Ҫ�ظ�ע�ᡣ");
                    Response.End();
                }
            }
            //ͬIPÿ��ע����û�������
            int MaxReg = (int)dt.Rows[0]["SameIPRegCount"];
            if (MaxReg > 0)
            { 
                //�������
                int UserCount =(int) DbSession.Default.FromSql("select COUNT(1) FROM dbo.Web_Users WHERE RegisterIP='"+userip+"'").ToScalar();
                if (UserCount >= MaxReg)
                {
                    //Response.Write("����IP����ע�������Ѵﵽ��������");
                    //Response.End();
                    CommonManager.Web.RegJs(this, "alert('����IP����ע�������Ѵﵽ��������');location.href='/Default.aspx';", false);
                    return;
                }
            }

            //�Ȳ�Ҫ�ӱ��и���ip��ȡʱ��������ơ�
            /*if( lastRegDate == null || !CommonManager.String.IsDateTime( lastRegDate.ToString() ) )
            {
                //��ѯͬһip������ʱ�����Ƿ����û�ע���¼���Լ�ע��ʱ��
                DateTime? d = DbSession.Default.FromSql( "select RegisterTM from Web_Users where RegisterIP=@RegIP" ).AddInputParameter( "@RegIP", DbType.String, userip ).ToScalar<DateTime?>();
                if( d.HasValue && ( ( DateTime.Now - d.Value ).TotalMinutes < timespan * 60 ) )
                {
                    CommonManager.Web.RegJs( this, "alert('�벻Ҫ�ظ�ע�ᡣ');history.back;", false );
                    return;
                }
            }
            else
            {
                if( ( DateTime.Now - DateTime.Parse( lastRegDate.ToString() ) ).TotalMinutes < timespan * 60 )
                {
                    CommonManager.Web.RegJs( this, "alert('�벻Ҫ�ظ�ע�ᡣ');history.back;", false );
                    return;
                }
            }*/
        }
        protected void btnSubmit_ServerClick(object sender, EventArgs e)
        {
            string validcode = Common.GetStringOfForm("validcode");
            if (ValidCode.CurrentCode != validcode)
            {
                /*Label1.Text = "��ܰ��ʾ����֤������������������룡";
                Label1.Visible = true;*/
                CommonManager.Web.RegJs(this, "alert('��ܰ��ʾ����֤������������������룡');location.href=location.href;", true);
                return;
            }

            string username = Common.FilterString(Common.GetStringOfForm("username"));
            //�ǳƿ��������κ��ַ�������Ҫ��������HTML���룬��ֹ����ľ��ű��ַ���������"'"Ϊ"''"������SQLע��ʽ����
            string nickname = Server.HtmlEncode(Common.GetStringOfForm("nickname").Replace("'", "''"));
            string sex = Common.FilterString(Common.GetStringOfForm("sex"));
            string pwd = Common.FilterString(Common.GetStringOfForm("pwd"));//Common.FilterString(Common.GetStringOfForm("pwd"));
            string ejpwd = Common.FilterString(Common.GetStringOfForm("ejpwd"));//Common.FilterString(Common.GetStringOfForm("ejpwd"));
            string email = Common.FilterString(Common.GetStringOfForm("email"));
            string mobile = Common.FilterString(Common.GetStringOfForm("mobile"));//�绰���浽TUserInfo���PhoneNum��
            string promoter = Common.FilterString(Common.GetStringOfForm("promoter"));
            string logoid = Common.FilterString(Common.GetStringOfForm("hidLogoID"));
            string idcard = Common.FilterString(Common.GetStringOfForm("idcard"));
            string qqNum = Common.FilterString(Common.GetStringOfForm("qqnum"));
            string realname = Common.FilterString(Common.GetStringOfForm("realname"));
            if (string.IsNullOrEmpty(sex) || !CommonManager.String.IsInteger(sex))
            {
                sex = "1";
            }
            if (string.IsNullOrEmpty(logoid) || !CommonManager.String.IsInteger(logoid))
            {
                logoid = "1";
            }

            //������ע��ʱѡ��ְҵ�͵���
            //string occu = CommonManager.Web.RequestForm( "txtOccuPation", "" );
            //string prov = CommonManager.Web.RequestForm( "hidProvince", "" );
            //string city = CommonManager.Web.RequestForm( "hidCity", "" );
            //string address = Server.HtmlEncode( CommonManager.Web.RequestForm( "txtAddress", "" ) );
            //�����������Ϊ�գ��Ͱ�����Ϊ��¼����
            if (ejpwd.Trim().Length == 0)
            {
                ejpwd = pwd;
            }
            BLL.Member member = new BLL.Member();
            int dbLogoId = member.SetLogoID(int.Parse(sex), int.Parse(logoid));

            #region �����ж��Ƿ��������������ڼ�
            /*
        bool isaction = false;
        if (RookiePresentConfig.Config.IsEnable)
        {
            if (RookiePresentConfig.Config.BeginDate <= DateTime.Now && RookiePresentConfig.Config.EndDate >= DateTime.Now)
            {
                isaction = true;
                //walletMoney += RookiePresentConfig.Config.Money;
                //���͵���
                if (RookiePresentConfig.Config.PropPresent.Rows.Count > 0)
                {
                    string select = "select Count(PropID) from TPropDefine where PropID=@PropID;";
                    string sql = "insert into TUserProp (UserID,PropID,HoldCount) values ({0},{1},{2});";
                    StringBuilder sqlCollection = new StringBuilder();
                    foreach (DataRow row in RookiePresentConfig.Config.PropPresent.Rows)
                    {
                        int tmpcount = DbSession.Default.FromSql(select).AddInputParameter("@PropID", DbType.Int32, row["PropID"]).ToScalar<int>();
                        if (tmpcount <= 0)
                            continue;
                        sqlCollection.AppendFormat(sql, userId, row["PropID"].ToString(), row["Amount"].ToString());
                    }
                    if (sqlCollection.ToString().Length > 0)
                    {
                        DbSession.Default.FromSql(sqlCollection.ToString()).Execute();
                    }
                }
            }
            else
            {
                isaction = false;
                RookiePresentConfig.Config.IsEnable = false;
                RookiePresentConfig.Save(RookiePresentConfig.Config);
            }
        }*/
            #endregion


          

            //2012-12-16 ������������ȡ������ID�����������ڴ����������޸���
            string agencyName = UiCommon.StringConfig.GetAgencyNameByTwoDname(); //��ȡ������ȡ�������˺�



            int userId = Convert.ToInt32(member.Add_UserInfo(username, nickname, promoter, int.Parse(sex), dbLogoId, pwd, ejpwd, idcard, mobile, email, Common.RequestIP, qqNum, realname, agencyName)["UserID"]);
            if (userId > 0)
            {
                //5.�����û���Ϣ
                UiCommon.UserLoginInfo.SetInfo(userId.ToString(), username);

                //�����û���½��Ϣ
                new BLL.Member().UpdateLastLogin(username, Utility.Common.RequestIP);

                //6.��¼��ǰ�ͻ������ϵ�ע��ʱ�䵽cookies
                BCSTCache.Default.Save<CookiesCache>("RegDateTime", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));
                Response.Redirect("/Manage/Default.aspx");
            }
            //else if (userId == -3)
            //{
            //    CommonManager.Web.RegJs(this, "alert('��Ǹ��ע��ʧ�ܣ���IP����ע������������');location.href=location.href;", true);
            //}
            //else if (userId == -4)
            //{
            //    CommonManager.Web.RegJs(this, "alert('��Ǹ��ע��ʧ�ܣ������֤���Ѵﵽע��������ƣ�');location.href=location.href;", true);
            //}
            else
            {
                CommonManager.Web.RegJs(this, "alert('��Ǹ��ע��ʧ�ܣ������Ƿ�������æ�����Ժ����ԣ�');location.href=location.href;", true);
            }


        }
      
    }
}
