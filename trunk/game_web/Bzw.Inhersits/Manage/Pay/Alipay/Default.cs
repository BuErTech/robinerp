using System;
using System.Data;
using System.Configuration;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;
using Gateway;
using BCST.Common;
using Bzw.WebLibrary;
using System.Data.SqlClient;
using Utility;

namespace Bzw.Inhersits.Manage.Pay.Alipay
{
    public partial class Manage_Pay_Alipay_Default : UiCommon.BasePage
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
        /// webtop1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.UserControl webtop1;

        /// <summary>
        /// form2 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlForm form2;

        /// <summary>
        /// txtUserName �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.TextBox txtUserName;

        /// <summary>
        /// txtUserName2 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.TextBox txtUserName2;

        /// <summary>
        /// PayMoney �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.TextBox PayMoney;

        /// <summary>
        /// Button1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.Button Button1;

        /// <summary>
        /// Control1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.UserControl Control1;
        protected string  MoneyRate = string.Empty;
        protected string CouponRate = string.Empty;

        protected void Page_Load(object sender, EventArgs e)
        {
            SeoSetting(SeoConfig.Config.PaySeo);
            if (UiCommon.UserLoginInfo.IsLogin)
            {
                txtUserName.Text = UiCommon.UserLoginInfo.UserName;
                txtUserName2.Text = UiCommon.UserLoginInfo.UserName;
                CouponRate = GetCouponRate();
                
            }
            MoneyRate = BLL.Config.GetInfoOfCard()["Con_MoneyChangeRate"].ToString();
        }

        protected void Button1_Click(object sender, EventArgs e)
        {
            /*
            show_url          ="www.xiaohhgame.com"                   '��վ����ַ
            seller_email		= "672561@qq.com"				'�����ó����Լ���֧�����ʻ�
            partner			= "2088102335433231"					'֧�������˻��ĺ��������ID
            key			    = "7a7kaee4ogm8lwywepjgpgkcek9pcnl3"	'֧�����İ�ȫУ����

            notify_url			= "http://www.xiaohhgame.com/alipay/Alipay_Notify.asp"	'�����������֪ͨ��ҳ�� Ҫ�� http://��ʽ������·��
            return_url			= "http://www.xiaohhgame.com/alipay/return_Alipay_Notify.asp"	'��������ת��ҳ�� Ҫ�� http://��ʽ������·��
            */


            //��Ʒ���ƣ��̻���
            string companyName = UiCommon.StringConfig.CompanyNameOfPay;

            //֧����
            string userName = txtUserName.Text.Trim();
            BLL.Member mem = new BLL.Member();
            if (!mem.ExistName(userName))
            {
                CommonManager.Web.RegJs(this, "alert('�Ҳ�����������û��������º˶ԡ�');history.back();", false);
                return;
            }

            //֧�����
            string payMoney = PayMoney.Text.Trim();
            if (string.IsNullOrEmpty(payMoney) || !CommonManager.String.IsInteger(payMoney) || int.Parse(payMoney) <= 0)
            {
                CommonManager.Web.RegJs(this, "alert('���ֻ���������0��������');history.back();", false);
                return;
            }
            int otherPayMoney = mem.UserTodayOtherPayMoney(userName.Trim());
            //�Ƿ񳬳������ֵ���ƶ�
            if (Convert.ToInt32(payMoney) > otherPayMoney)
            {
            
                CommonManager.Web.RegJs(this, "alert(''�������ܳ�ֵ��������ܴ���" + otherPayMoney + "��');history.back();", false);
                return;
            }
            //֧������ Web_RMBCost.PayType
            int payTypeLog = 4;

            //������Ϣ Web_RMBCost.Typeinfo
            string typeInfoLog = "֧��������֧��";

            //*****************ҵ�������ֵ*****************************************************************
            //������(��ʱ����)
            /*System.DateTime currentTime = new System.DateTime();
            currentTime = System.DateTime.Now;
            string out_trade_no = currentTime.ToString("g");
            out_trade_no = out_trade_no.Replace("-", "");
            out_trade_no = out_trade_no.Replace(":", "");
            out_trade_no = out_trade_no.Replace(" ", "");*/
            string out_trade_no = DateTime.Now.ToString("yyyyMMddHHmmssfffffff");

            //֧���ӿڡ�https://www.alipay.com/cooperate/gateway.do?
            string gateway = UiCommon.StringConfig.AlipayNotifyURL;

            //�������
            string service = "create_direct_pay_by_user";

            //partner �������ID(�����ֶ�)
            string partner = UiCommon.StringConfig.AlipayPartnerID;// "2088102335433231"; 

            //��������
            string sign_type = "MD5";

            //subject ��Ʒ����
            string subject = companyName;

            //body ��Ʒ����

            // [modify] jeffery
            //string body = "��Ϸ" + UiCommon.StringConfig.MoneyName;

            double poupon = Convert.ToDouble(CouponRate) * Convert.ToDouble(PayMoney.Text);
            if (poupon < 0)
                poupon = 0;

            string body = "��Ϸ" + UiCommon.StringConfig.MoneyName +
                "!@#" + userName + "!@#" + Convert.ToInt32(poupon).ToString();
            // ---end

            //֧������
            string payment_type = "1";

            //�ܽ�� 0.01��50000.00      	
            string total_fee = payMoney;

            //��վ����ַ
            string show_url = UiCommon.StringConfig.WebSiteName;// ֧����ҳ����ʾ<a href="www.xiaohhgame.com" target="_blank">��Ʒ����</a> ;

            //�����˺�
            string seller_email = UiCommon.StringConfig.AlipayAccount; //"672561@qq.com";

            //partner�˻���֧������ȫУ����   
            string key = UiCommon.StringConfig.AlipayKey; //"7a7kaee4ogm8lwywepjgpgkcek9pcnl3";

            //������֪ͨ���ؽӿڣ����ҳ�����չʾ���ͻ���,ֻ�и���ɹ��Ż���ת�����ҳ��
            string return_url = UiCommon.StringConfig.WebSiteName + "/Manage/Pay/Alipay/Alipay_Return.aspx";

            //������֪ͨ���ؽӿ�
            string notify_url = UiCommon.StringConfig.WebSiteName + "/Manage/Pay/Alipay/Alipay_Notify.aspx";

            string _input_charset = "utf-8";


            //֧���ɹ�ǰ�� ����֧���Ķ�����־
            mem.Add3PayOrder(userName, Convert.ToInt32(payMoney), payTypeLog, typeInfoLog, out_trade_no);


            //��֧�������ύ��֧����������
            AliPay ap = new AliPay();
            string aliay_url = ap.CreatUrl(
                gateway,
                service,
                partner,
                sign_type,
                out_trade_no,
                subject,
                body,
                payment_type,
                total_fee,
                show_url,
                seller_email,
                key,
                return_url,
                _input_charset,
                notify_url
                );

            Response.Redirect(aliay_url);

        }

        private string GetCouponRate()
        {
            string strsql = "select top 1 CouponRate from TRechargeCouponType where Way=@way";
            DataTable table = SqlHelper.ExecuteDataset(
                CommandType.Text,
                strsql,
                new SqlParameter[]{
                    new SqlParameter("@way",4)
                }
                ).Tables[0];
            if (table.Rows.Count > 0)
            {
                return table.Rows[0]["CouponRate"].ToString();
            }
            return "";
        }
    }
}
