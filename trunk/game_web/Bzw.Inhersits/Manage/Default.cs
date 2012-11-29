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

namespace Bzw.Inhersits.Manage
{
    public partial class Manage_Default : UiCommon.ManageBasePage
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
        /// webfooter1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.UserControl webfooter1;
        public string NickName = string.Empty;
        public int Sex = 1;
        public string UserHeadImg = string.Empty;
        public int LogoID = 0;
        public string AllMoney = "0";
        public string BankMoney = "0";
        public string WalletMoney = "0";
        public string Paiming = "0";
        public string Email = string.Empty;
        public string VipTime = "";
        public string OnLineTimeCount = "0";
        public string PlayTimeCount = "0";
        public string AllLoginCount = "0";
        public string RegisterTM = string.Empty;
        public string LastLoginTM = string.Empty;
        public string RealName = string.Empty;
        protected string UserFasci;
        protected string Lotteries;
        protected string Phone = "";
        //protected string ZhiYe = "";
        //protected string Address = "";

        protected void Page_Load(object sender, EventArgs e)
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
             
            
            BankMoney = dr["BankMoney"].ToString();
           
            Email = dr["Email"].ToString();
            //��ȡ���Ǯ����ұ����ж�Ǯ���汾
            if (UiCommon.StringConfig.BankVersion == "2")
            {
                WalletMoney = member.GetWalletMoney(UiCommon.UserLoginInfo.UserID).ToString();
            }
            else
            {
                WalletMoney = dr["WalletMoney"].ToString();
            }
            AllMoney = (Convert.ToInt64(BankMoney) + Convert.ToInt64(WalletMoney)).ToString();
            OnLineTimeCount = member.ConverTimeToDHMS(member.GetOnLineTimeSum(UserLoginInfo.UserID));
            //PlayTimeCount = member.ConverTimeToDHMS(member.GetPlayTimeSum(UserLoginInfo.UserID));
            PlayTimeCount = member.ConverTimeToDHMS(member.GetAllPlayTimeByUserID(UserLoginInfo.UserID));
            AllLoginCount = dr["AllLoginTime"].ToString();
           // string str = dr["RegisterTM"].ToString();
          
            RegisterTM = Convert.ToDateTime(dr["RegisterTM"].ToString()).ToString("yyyy-MM-dd HH:mm:ss");
            LastLoginTM = Convert.ToDateTime(dr["LastLoginTM"]).ToString("yyyy-MM-dd HH:mm:ss");
            UserFasci = dr["Fascination"].ToString();
            Phone = dr["Phone"].ToString();
            RealName = dr["RealName"].ToString();
            Lotteries = dr["Lotteries"].ToString();
            //ZhiYe = dr["OccuPation"].ToString();
            //Address = dr["AdrProvince"].ToString() + dr["AdrCity"].ToString() + dr["AdrZone"].ToString();

            //Paiming

            //VipTime--------------
            VipTime = dr["VipTime2"].ToString();
            DateTime dtNow = DateTime.Now;
            if (Convert.ToDateTime(VipTime) > dtNow)
            {
                VipTime = Convert.ToDateTime(VipTime).ToString("yyyy-MM-dd");
            }
            else
            {
                DateTime dtInit = new DateTime(1970, 01, 01, 08, 00, 00);
                if (Convert.ToDateTime(VipTime) > dtInit)
                {
                    VipTime = "<a href=\"/Manage/VipApply.aspx\" class=\"red\">����VIP�Ѿ����ڣ���������</a>";
                }
                else
                {
                    VipTime = "<a href=\"/Manage/VipApply.aspx\" class=\"red\">��������VIP����������</a>";
                }
            }

            //UserHeadImg-------------
            //if (LogoID >= 1 && LogoID <= 255)//ϵͳĬ��ͷ�� 1<=LogoID<=255
            if (LogoID >= BLL.Member.LogoId_Const1 && LogoID <= BLL.Member.LogoId_Const2)
            {
                if (Sex == 1)
                {
                    UserHeadImg = "/Image/Users/b_" + LogoID + ".png";
                }
                else
                {
                    UserHeadImg = "/Image/Users/g_" + LogoID + ".png";
                }
            }
            else if (LogoID >= BLL.Member.LogoId_Const3 && LogoID <= BLL.Member.LogoId_Const4) //�ϴ����Զ���ͷ�� 256<=LogoID<=512
            {
                int childDirName = Convert.ToInt32(UiCommon.UserLoginInfo.UserID) / 1000;
                UserHeadImg = "/Upload/" + UiCommon.StringConfig.UserCustHeadFolderName + "/" + childDirName + "/" + UiCommon.UserLoginInfo.UserID + ".png";
            }
            else if (LogoID > BLL.Member.LogoId_Const4) //�ϴ����Զ���ͷ�� LogoID>512  ����˻��й���ʱ����Զ���ͷ��
            {
                int childDirName = Convert.ToInt32(UiCommon.UserLoginInfo.UserID) / 1000;
                UserHeadImg = "/Upload/" + UiCommon.StringConfig.UserCustHeadFolderName + "/" + childDirName + "/" + UiCommon.UserLoginInfo.UserID + ".png";
            }
        }

        public string GetMoneyPaiming(string Uid)
        {
            string strSQL = "select (select count(1) from TUserinfo where convert(bigint,BankMoney)+convert(bigint,WalletMoney)>convert(bigint,u.BankMoney)+convert(bigint,u.WalletMoney))+1 as iTop from TUserinfo as u where userid = " + Uid + "";
            return "";
        }
    }
}
