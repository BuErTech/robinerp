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
using com.yeepay;

namespace Bzw.Inhersits.Manage.Pay.Yeepay
{
    public partial class Manage_Pay_Yeepay_NotePay : UiCommon.BasePage
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
        /// webshortcutkey1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        //protected global::Public_WebShortCutKey webshortcutkey1;

        /// <summary>
        /// Form1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlForm Form1;

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
        protected global::System.Web.UI.WebControls.DropDownList PayMoney;

        /// <summary>
        /// Button1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlInputSubmit Button1;

        /// <summary>
        /// webfooter1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.UserControl webfooter1;
        private string p1_MerId = "";
        private string keyValue = "";
        private string p2_Order = "";
        private string p3_Amt = "";
        private string p4_Cur = "";
        private string p5_Pid = "";
        private string p6_Pcat = "";

        private string p7_Pdesc = "";
        private string p8_Url = "";
        private string p9_SAF = "";
        private string pa_MP = "";
        private string pd_FrpId = "";
        private string pr_NeedResponse = "";

        protected void Page_Load(object sender, EventArgs e)
        {
            if (UiCommon.UserLoginInfo.IsLogin)
            {
                txtUserName.Text = UiCommon.UserLoginInfo.UserName;
                txtUserName2.Text = UiCommon.UserLoginInfo.UserName;
            }
        }
        protected void Button1_Click(object sender, EventArgs e)
        {
            string errorUrl = "/Manage/Pay/Yeepay/NotePay.aspx"; //������ת��ַ
            //��Ʒ���ƣ��̻���
            string companyName = UiCommon.StringConfig.CompanyNameOfPay;

            //֧����
            string userName = txtUserName.Text.Trim();

            //֧�����
            string payMoney = PayMoney.SelectedItem.Value; //PayMoney.Text.Trim();

            //֧������ Web_RMBCost.PayType
            int payTypeLog = 3;

            //������Ϣ Web_RMBCost.Typeinfo
            string typeInfoLog = "�ױ�һ��֧ͨ��";

            //���ױ��Ľ��׶�����
            string out_trade_no = DateTime.Now.ToString("yyyyMMddHHmmssfffffff");

            BLL.Member mem = new BLL.Member();
            int otherPayMoney = mem.UserTodayOtherPayMoney(userName);
            //�Ƿ񳬳������ֵ���ƶ�
            if (Convert.ToInt32(payMoney) > otherPayMoney)
            {
                Response.Write("<script>alert('�������ܳ�ֵ��������ܴ���" + otherPayMoney + "��');location.href='" + errorUrl + "';</script>");
                Response.End();
            }

            //����֧��������־
            new BLL.Member().Add3PayOrder(userName, Convert.ToInt32(payMoney), payTypeLog, typeInfoLog, out_trade_no);


            //��ʼ����֧������

            //���ñ���
            Response.ContentEncoding = System.Text.Encoding.GetEncoding("GB2312");

            //#if !debug
            #region ��ʽ����
            p1_MerId = StringConfig.YeepayMerID;                                     // �̼�ID
            keyValue = StringConfig.YeepayKey;  // �̼���Կ
            Buy.NodeAuthorizationURL = StringConfig.YeepayNodeAuthorizationURL;
            #endregion
            //#endif


            //#region ���Դ���
            //p1_MerId = "10000432521";                                     // �̼�ID
            //keyValue = "8UPp0KE8sq73zVP370vko7C39403rtK1YwX40Td6irH216036H27Eb12792t";  // �̼���Կ
            //// ���������ַ
            //Buy.NodeAuthorizationURL = @"http://tech.yeepay.com:8080/robot/debug.action"; //test 
            //#endregion


            p2_Order = out_trade_no;

            p3_Amt = payMoney;

            //���ױ���,�̶�ֵ"CNY".
            p4_Cur = "CNY";

            //��Ʒ����
            //����֧��ʱ��ʾ���ױ�֧���������Ķ�����Ʒ��Ϣ.
            p5_Pid = "";

            //��Ʒ����
            p6_Pcat = "";

            //��Ʒ����
            p7_Pdesc = "";

            //�̻�����֧���ɹ����ݵĵ�ַ,֧���ɹ����ױ�֧������õ�ַ�������γɹ�֪ͨ.
            //p8_Url = "http://localhost:102/user/yeepay/Callback.aspx";//���ز���URLuser/yeepay/Default.aspx

            //p8_Url = UiCommon.StringConfig.WebSiteName + "/Manage/Pay/Yeepay/Callback.aspx";

            p8_Url = UiCommon.StringConfig.WebSiteName + "/Manage/Pay/Yeepay/Callback.aspx";


            //�ͻ���ַ
            //Ϊ��1��: ��Ҫ�û����ͻ���ַ�����ױ�֧��ϵͳ;Ϊ��0��: ����Ҫ��Ĭ��Ϊ ��0��.
            p9_SAF = "0";

            //�̻���չ��Ϣ
            //�̻�����������д1K ���ַ���,֧���ɹ�ʱ��ԭ������.	
            pa_MP = "";

            //���б���
            //Ĭ��Ϊ""�����ױ�֧������.��������ʾ�ױ�֧����ҳ�棬ֱ����ת�������С�������֧��������һ��ͨ��֧��ҳ�棬���ֶο����ո�¼:�����б����ò���ֵ.
            pd_FrpId = "YPCARD";


            //Ӧ�����
            //Ϊ"1": ��ҪӦ�����;Ϊ"0": ����ҪӦ�����.
            pr_NeedResponse = "1";

            Response.Redirect(Buy.CreateBuyUrl(p1_MerId, keyValue, p2_Order, p3_Amt, p4_Cur, p5_Pid, p6_Pcat, p7_Pdesc, p8_Url, p9_SAF, pa_MP, pd_FrpId, pr_NeedResponse));

        }
    }
}
