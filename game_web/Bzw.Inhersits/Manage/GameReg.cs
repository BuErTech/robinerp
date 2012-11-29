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
using BCST.Cache;
using System.Collections.Generic;
using System.Text;
using BLL;

namespace Bzw.Inhersits.Manage
{


    public partial class Manage_GameReg : UiCommon.BasePage
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
        /// Label1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.Label Label1;

        /// <summary>
        /// btnSubmit �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlInputSubmit btnSubmit;
        //����ע��ҳ���Ƿ���Ҫ��ʾQQ�ֶ�
        public string needqq = System.Configuration.ConfigurationSettings.AppSettings["qq"];
		public string needrealname = System.Configuration.ConfigurationSettings.AppSettings["RealName"];
		public string needIDCard = System.Configuration.ConfigurationSettings.AppSettings["IDCard"];
        public string needProcted = System.Configuration.ConfigurationSettings.AppSettings["IsCardProtected"];
        protected void Page_Load(object sender, EventArgs e)
        {
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
                    Response.Write("����IP���ڱ����Ƶķ�Χ�ڣ����ܽ���ע�ᣬ����ϵ���ǵĿͷ�������ѯ��");
                    Response.End();
                    //CommonManager.Web.RegJs( this, "alert('����IP���ڱ����Ƶķ�Χ�ڣ����ܽ���ע�ᣬ����ϵ���ǵĿͷ�������ѯ��');history.back;", false );
                    //return;
                }
            }

            object lastRegDate = BCSTCache.Default.Get<CookiesCache>("RegDateTime");
            if (lastRegDate != null && CommonManager.String.IsDateTime(lastRegDate.ToString()))
            {
                if ((DateTime.Now - DateTime.Parse(lastRegDate.ToString())).TotalMinutes < timespan)
                {
                    Response.Write("�벻Ҫ�ظ�ע�ᡣ");
                    Response.End();
                    //CommonManager.Web.RegJs( this, "alert('�벻Ҫ�ظ�ע�ᡣ');history.back;", false );
                    //return;
                }
            }
            
        }
        protected void btnSubmit_ServerClick(object sender, EventArgs e)
        {
            string validcode = Common.GetStringOfForm("validcode");
            if (ValidCode.CurrentCode != validcode)
            {
                Label1.Text = "��ܰ��ʾ����֤������������������룡";
                Label1.Visible = true;
                return;
            }

            string username = Common.FilterString(Common.GetStringOfForm("username"));
            //�ǳƿ��������κ��ַ�������Ҫ��������HTML���룬��ֹ����ľ��ű��ַ���������"'"Ϊ"''"������SQLע��ʽ����
            string nickname = Server.HtmlEncode(Common.GetStringOfForm("nickname").Replace("'", "''"));
            string sex = Common.FilterString(Common.GetStringOfForm("sex"));
            string pwd = Common.FilterString(Common.GetStringOfForm("pwd"));//Common.FilterString( Common.GetStringOfForm( "hidPwd" ) );//
            string ejpwd ="";//Common.FilterString( Common.GetStringOfForm( "hidPwd2" ) );//
            string email ="";
            string mobile = Common.FilterString(Common.GetStringOfForm("mobile"));//�绰���浽TUserInfo���PhoneNum��
            string promoter = Common.FilterString(Common.GetStringOfForm("promoter"));
            string logoid = Common.FilterString(Common.GetStringOfForm("logoid"));
            string idcard = Common.FilterString(Common.GetStringOfForm("idcard"));
            string signDsc = Server.HtmlEncode(Common.GetStringOfForm("sign").Replace("'", "''"));
            string qqnum = "";
			string realname ="";
            if (string.IsNullOrEmpty(sex) || !CommonManager.String.IsInteger(sex))
                sex = "1";

            if (string.IsNullOrEmpty(logoid) || !CommonManager.String.IsInteger(logoid))
                logoid = "1";

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
            /*
            string userId = Utility.SqlHelper.GetMaxID("UserID", "TUsers").ToString();

            //1.����TUsers��
	
            member.Add2TUsers( userId, username, nickname, pwd, ejpwd, int.Parse( sex ), dbLogoId );

            //2.����TUserInfo��
            int walletMoney = BLL.Config.GoldCountOfReg();
            BLL.Promotion promo = new BLL.Promotion();
            bool isTg = false;
            if( promoter.Trim().Length > 0 )
            {
                if( member.ExistName( promoter ) && promo.ExistTGUser( promoter ) )
                {
                    isTg = true;
                    walletMoney += BLL.Config.GoldCountOfRegTG();
                }
            }*/

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

            /*member.Add2TUserInfo(userId, walletMoney, Common.RequestIP);
		//�޸�Ϊ���·�����
		//member.Add2TUserInfo( userId, walletMoney, Common.RequestIP, occu, prov, city, address, mobile );

		//3.����Web_Users��
		member.Add2Web_Users(userId, email, mobile, Common.RequestIP, idcard);
		//member.Add2Web_Users( userId, email, "", Common.RequestIP );

		//4.�����ƹ���־��Ϣ
		if( isTg )
		{
			promo.AddLog( promoter, userId );
		}
		*/
			string agencyName = UiCommon.StringConfig.GetAgencyNameByTwoDname(); //��ȡ������ȡ�������˺�
            int userId =Convert.ToInt32(member.Add_UserInfo(username, nickname, promoter, int.Parse(sex), dbLogoId, pwd, ejpwd, idcard, mobile, email, Common.RequestIP, qqnum, realname,agencyName,signDsc)["UserID"]);
            if (userId > 0)
            {
                //5.�����û���Ϣ
                UiCommon.UserLoginInfo.SetInfo(userId.ToString(), username);


                //�����û���½��Ϣ
                new BLL.Member().UpdateLastLogin(username, Utility.Common.RequestIP);

                //6.��¼��ǰ�ͻ������ϵ�ע��ʱ�䵽cookies
                BCSTCache.Default.Save<CookiesCache>("RegDateTime", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss"));


                //Limit.outMsgBox("��ܰ��ʾ��\\n\\n��Աע��ɹ���", "bzwp://regsucceed?username=" + SqlUsername + "", true);


                //Response.Write("<script language=javascript>window.alert('" + Message + "');" + str + "</script>");

                string ieEvent = "bzwp://regsucceed?username=" + username + "&userid=" + userId;
                Response.Write("<script language=javascript>window.location.href='" + ieEvent + "';</script>");
            }
            else
            {
                Label1.Text = "��Ǹ��ע��ʧ�ܣ������Ƿ�������æ�����Ժ����ԣ�";
                return;
            }
            /*
            if (isaction)
            {
                Response.Write("<script language=javascript>alert('�����ǻ�ڼ䣬�����ϵͳ���͸����ĵ��ߣ�\\r\\n�쵽��Ϸ�ĵ�������ȥ�������ǵĵ��߰ɣ�');window.location.href='" + ieEvent + "';</script>");
            }
            else
            {
                Response.Write("<script language=javascript>window.location.href='" + ieEvent + "';</script>");
            }
            */


        }
    }
}
