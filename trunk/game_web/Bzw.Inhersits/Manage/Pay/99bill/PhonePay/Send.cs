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
namespace Bzw.Inhersits.Manage.Pay._99bill.PhonePay
{
    public partial class Manage_Pay_99bill_PhonePay_Send : UiCommon.BasePage
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
        /// Lab_productName �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.Literal Lab_productName;

        /// <summary>
        /// Lab_orderId �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.Literal Lab_orderId;

        /// <summary>
        /// Lab_payerName �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.Literal Lab_payerName;

        /// <summary>
        /// Lab_orderAmount �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.Literal Lab_orderAmount;

        /// <summary>
        /// inputCharset �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlInputHidden inputCharset;

        /// <summary>
        /// bgUrl �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlInputHidden bgUrl;

        /// <summary>
        /// pageUrl �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlInputHidden pageUrl;

        /// <summary>
        /// version �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlInputHidden version;

        /// <summary>
        /// language �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlInputHidden language;

        /// <summary>
        /// signType �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlInputHidden signType;

        /// <summary>
        /// merchantAcctId �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlInputHidden merchantAcctId;

        /// <summary>
        /// payerName �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlInputHidden payerName;

        /// <summary>
        /// payerContactType �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlInputHidden payerContactType;

        /// <summary>
        /// payerContact �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlInputHidden payerContact;

        /// <summary>
        /// orderId �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlInputHidden orderId;

        /// <summary>
        /// orderAmount �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlInputHidden orderAmount;

        /// <summary>
        /// orderTime �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlInputHidden orderTime;

        /// <summary>
        /// productName �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlInputHidden productName;

        /// <summary>
        /// productNum �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlInputHidden productNum;

        /// <summary>
        /// productId �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlInputHidden productId;

        /// <summary>
        /// productDesc �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlInputHidden productDesc;

        /// <summary>
        /// ext1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlInputHidden ext1;

        /// <summary>
        /// ext2 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlInputHidden ext2;

        /// <summary>
        /// signMsg �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlInputHidden signMsg;

        /// <summary>
        /// payType �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlInputHidden payType;

        /// <summary>
        /// fullAmountFlag �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlInputHidden fullAmountFlag;

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
            //�Ƿ�����ʱ�ض����URL��ַ
            string toUrl = "/Manage/Pay/99bill/PhonePay/Default.aspx";

            //���յ�����֧��ƽ̨����֧�������ҳ���ַ��Ϊ���Ե�ַ
            string receivePageUrl = UiCommon.StringConfig.WebSiteName + "/Manage/Pay/99bill/PhonePay/Receive.aspx";

            //��Ʒ���ƣ��̻���
            string companyName = UiCommon.StringConfig.CompanyNameOfPay;

            //֧����
            if (Request.Form["txtUserName"] == null)
            {
                Response.Redirect(toUrl);
            }
            string userName = Request.Form["txtUserName"].ToString();

            //֧�����
            if (Request.Form["PayMoney"] == null)
            {
                Response.Redirect(toUrl);
            }
            string payMoney = Request.Form["PayMoney"].ToString().Trim();
            BLL.Member mem = new BLL.Member();
            int otherPayMoney = mem.UserTodayOtherPayMoney(userName.Trim());
            //�Ƿ񳬳������ֵ���ƶ�
            if (Convert.ToInt32(payMoney) > otherPayMoney)
            {
                Response.Write("<script>alert('�������ܳ�ֵ��������ܴ���" + otherPayMoney + "��');location.href='" + toUrl + "';</script>");
                Response.End();
            }
            //֧������ Web_RMBCost.PayType
            int payTypeLog = 1;

            //������Ϣ Web_RMBCost.Typeinfo
            string typeInfoLog = "��Ǯ�����п���֧��";


            /**
             * @Description: ��Ǯ������֧�����ؽӿڷ���
             * @Copyright (c) �Ϻ���Ǯ��Ϣ�������޹�˾
             * @version 2.0
             */

            //�����������˻���
            ///�����Ǯ������ϵ��ȡ
            merchantAcctId.Value = UiCommon.StringConfig.JJbill_SZX_Account;// "1001622591104";

            //����������������Կ
            ///���ִ�Сд
            String key = UiCommon.StringConfig.JJbill_SZX_Key;// "84YB4FXGFKZ85975";


            //�ַ���.�̶�ѡ��ֵ����Ϊ�ա�
            ///ֻ��ѡ��1��2��3��5
            ///1����UTF-8; 2����GBK; 3����gb2312; 5 ����big5
            ///Ĭ��ֵΪ1
            inputCharset.Value = "1";

            //����������֧������ĺ�̨��ַ.��[pageUrl]����ͬʱΪ�ա������Ǿ��Ե�ַ��
            ///��Ǯͨ�����������ӵķ�ʽ�����׽�����͵�[bgUrl]��Ӧ��ҳ���ַ�����̻�������ɺ������<result>���Ϊ1��ҳ���ת��<redirecturl>��Ӧ�ĵ�ַ��
            ///�����Ǯδ���յ�<redirecturl>��Ӧ�ĵ�ַ����Ǯ����֧�����GET��[pageUrl]��Ӧ��ҳ�档
            bgUrl.Value = receivePageUrl;

            //����֧�������ҳ���ַ.��[bgUrl]����ͬʱΪ�ա������Ǿ��Ե�ַ��
            ///���[bgUrl]Ϊ�գ���Ǯ��֧�����GET��[pageUrl]��Ӧ�ĵ�ַ��
            ///���[bgUrl]��Ϊ�գ�����[bgUrl]ҳ��ָ����<redirecturl>��ַ��Ϊ�գ���ת��<redirecturl>��Ӧ�ĵ�ַ.
            pageUrl.Value = "";

            //���ذ汾.�̶�ֵ
            ///��Ǯ����ݰ汾�������ö�Ӧ�Ľӿڴ������
            ///������汾�Ź̶�Ϊv2.0
            version.Value = "v2.0";

            //��������.�̶�ѡ��ֵ��
            ///ֻ��ѡ��1��2��3
            ///1�������ģ�2����Ӣ��
            ///Ĭ��ֵΪ1
            language.Value = "1";

            //ǩ������.�̶�ֵ
            ///1����MD5ǩ��
            ///��ǰ�汾�̶�Ϊ1
            signType.Value = "1";


            //֧��������
            ///��Ϊ���Ļ�Ӣ���ַ�
            //payerName.Value="֧����";
            payerName.Value = userName;


            //֧������ϵ��ʽ����.�̶�ѡ��ֵ
            ///ֻ��ѡ��1
            ///1����Email
            payerContactType.Value = "1";

            //֧������ϵ��ʽ
            ///ֻ��ѡ��Email���ֻ���
            payerContact.Value = "";

            //�̻�������
            ///����ĸ�����֡���[-][_]���
            orderId.Value = DateTime.Now.ToString("yyyyMMddHHmmssfffffff"); //DateTime.Now.ToString("yyyyMMddHHmmss");


            //�������
            ///�Է�Ϊ��λ����������������
            ///�ȷ�2������0.02Ԫ
            ///orderAmount.Value="100";
            double dblTemp = Double.Parse(payMoney);
            orderAmount.Value = Convert.ToString(dblTemp * 100);//Ԫת��Ϊ��


            //�����ύʱ��
            ///14λ���֡���[4λ]��[2λ]��[2λ]ʱ[2λ]��[2λ]��[2λ]
            ///�磻20080101010101
            orderTime.Value = DateTime.Now.ToString("yyyyMMddHHmmss");

            //��Ʒ����
            ///��Ϊ���Ļ�Ӣ���ַ�
            //productName.Value="��Ʒ����";
            productName.Value = companyName;

            //��Ʒ����
            ///��Ϊ�գ��ǿ�ʱ����Ϊ����
            productNum.Value = "1";

            //��Ʒ����
            ///��Ϊ�ַ���������
            productId.Value = "";

            //��Ʒ����
            productDesc.Value = "";

            //��չ�ֶ�1
            ///��֧��������ԭ�����ظ��̻�
            ext1.Value = userName;

            //��չ�ֶ�2
            ///��֧��������ԭ�����ظ��̻�
            ext2.Value = "";


            //֧����ʽ.�̶�ѡ��ֵ
            ///��ѡ��00��41��42��52
            ///00 �����ǮĬ��֧����ʽ��ĿǰΪ�����п���֧���Ϳ�Ǯ�˻�֧����41 �����Ǯ�˻�֧����42 ���������п���֧���Ϳ�Ǯ�˻�֧����52 ���������п���֧��
            payType.Value = "52";

            //ȫ��֧����־
            ///ֻ��ѡ������ 0 �� 1
            ///0 �����ȫ��֧����ʽ��֧����ɺ󷵻ض������Ϊ�̻��ύ�Ķ��������Ԥ���ѿ����С�ڶ������ʱ������֧�����Ϊʧ�ܣ�Ԥ���ѿ������ڻ���ڶ������ʱ������֧�����Ϊ�ɹ���
            ///1 ����ȫ��֧����ʽ��֧����ɺ󷵻ض������Ϊ�û�Ԥ���ѿ�����ֻҪԤ���ѿ������ɹ�������֧�������Ϊ�ɹ���
            fullAmountFlag.Value = "0";



            //�����к��������ַ��Ĳ������б���urlencode���룬���������Ľ��ȫ��ת���ɴ�д��ĸ
            ///��ע�⣺�������µ���������������������ַ��Ĳ���������ת���ɴ�д��ĸ����������á�.ToUpper()������
            ///        �����ڷ���ҳ����յ��ķ��ز�����ȫ����Ϊ��д��ĸ���⽫���ܵ������ݿⶩ�����ݸ���ʧ�ܣ���
            payerName.Value = System.Web.HttpUtility.UrlEncode(payerName.Value, Encoding.GetEncoding("UTF-8")).ToUpper();
            productName.Value = System.Web.HttpUtility.UrlEncode(productName.Value, Encoding.GetEncoding("UTF-8")).ToUpper();
            productDesc.Value = System.Web.HttpUtility.UrlEncode(productDesc.Value, Encoding.GetEncoding("UTF-8")).ToUpper();
            ext1.Value = System.Web.HttpUtility.UrlEncode(ext1.Value, Encoding.GetEncoding("UTF-8")).ToUpper();
            ext2.Value = System.Web.HttpUtility.UrlEncode(ext2.Value, Encoding.GetEncoding("UTF-8")).ToUpper();



            //���ɼ���ǩ����
            ///����ذ�������˳��͹�����ɼ��ܴ���
            String signMsgVal = "";
            signMsgVal = appendParam(signMsgVal, "inputCharset", inputCharset.Value);
            signMsgVal = appendParam(signMsgVal, "bgUrl", bgUrl.Value);
            signMsgVal = appendParam(signMsgVal, "pageUrl", pageUrl.Value);
            signMsgVal = appendParam(signMsgVal, "version", version.Value);
            signMsgVal = appendParam(signMsgVal, "language", language.Value);
            signMsgVal = appendParam(signMsgVal, "signType", signType.Value);
            signMsgVal = appendParam(signMsgVal, "merchantAcctId", merchantAcctId.Value);
            signMsgVal = appendParam(signMsgVal, "payerName", payerName.Value);
            signMsgVal = appendParam(signMsgVal, "payerContactType", payerContactType.Value);
            signMsgVal = appendParam(signMsgVal, "payerContact", payerContact.Value);
            signMsgVal = appendParam(signMsgVal, "orderId", orderId.Value);
            signMsgVal = appendParam(signMsgVal, "orderAmount", orderAmount.Value);
            signMsgVal = appendParam(signMsgVal, "payType", payType.Value);
            signMsgVal = appendParam(signMsgVal, "fullAmountFlag", fullAmountFlag.Value);
            signMsgVal = appendParam(signMsgVal, "orderTime", orderTime.Value);
            signMsgVal = appendParam(signMsgVal, "productName", productName.Value);
            signMsgVal = appendParam(signMsgVal, "productNum", productNum.Value);
            signMsgVal = appendParam(signMsgVal, "productId", productId.Value);
            signMsgVal = appendParam(signMsgVal, "productDesc", productDesc.Value);
            signMsgVal = appendParam(signMsgVal, "ext1", ext1.Value);
            signMsgVal = appendParam(signMsgVal, "ext2", ext2.Value);
            signMsgVal = appendParam(signMsgVal, "key", key);

            signMsg.Value = GetMD5(signMsgVal, "utf-8").ToUpper();


            //��ӡ������Ϣ
            Lab_orderId.Text = orderId.Value;
            Lab_orderAmount.Text = payMoney;
            Lab_payerName.Text = userName;
            Lab_productName.Text = companyName;

            /*
            Response.Write("userName:" + userName + "<br>");
            Response.Write("payMoney:" + payMoney + "<br>");
            Response.Write("payTypeLog:" + payTypeLog + "<br>");
            Response.Write("typeInfoLog:" + typeInfoLog + "<br>");
            Response.Write("orderId:" + orderId.Value + "<br>");
            Response.End();
            */

            //֧���ɹ�ǰ�� ����֧���Ķ�����־
            new BLL.Member().Add3PayOrder(userName, Convert.ToInt32(payMoney), payTypeLog, typeInfoLog, orderId.Value);
        }

        //���ܺ�����������ֵ��Ϊ�յĲ�������ַ�������ʼ
        String appendParam(String returnStr, String paramId, String paramValue)
        {
            if (returnStr != "")
            {

                if (paramValue != "")
                {

                    returnStr += "&" + paramId + "=" + paramValue;
                }

            }
            else
            {

                if (paramValue != "")
                {
                    returnStr = paramId + "=" + paramValue;
                }
            }

            return returnStr;
        }
        //���ܺ�����������ֵ��Ϊ�յĲ�������ַ���������



        //���ܺ��������ַ������б����ʽת����������MD5���ܣ�Ȼ�󷵻ء���ʼ
        private static string GetMD5(string dataStr, string codeType)
        {
            System.Security.Cryptography.MD5 md5 = new System.Security.Cryptography.MD5CryptoServiceProvider();
            byte[] t = md5.ComputeHash(System.Text.Encoding.GetEncoding(codeType).GetBytes(dataStr));
            System.Text.StringBuilder sb = new System.Text.StringBuilder(32);
            for (int i = 0; i < t.Length; i++)
            {
                sb.Append(t[i].ToString("x").PadLeft(2, '0'));
            }
            return sb.ToString();
        }
        //���ܺ��������ַ������б����ʽת����������MD5���ܣ�Ȼ�󷵻ء�����
    }

}
