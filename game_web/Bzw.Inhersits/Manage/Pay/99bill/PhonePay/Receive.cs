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
using System.Text.RegularExpressions;

namespace Bzw.Inhersits.Manage.Pay._99bill.PhonePay
{
    public partial class Manage_Pay_99bill_PhonePay_Receive : UiCommon.BasePage
    {
        /*
         * @Description: ��Ǯ������֧�����ؽӿڷ���
         * @Copyright (c) �Ϻ���Ǯ��Ϣ�������޹�˾
         * @version 2.0
         */

        //��ʼ���������ַ
        public int rtnOk = 0;
        public String rtnUrl = "";

        protected void Page_Load(object sender, EventArgs e)
        {
            //����֧�������ҳ���ַ
            string showUrl = UiCommon.StringConfig.WebSiteName + "/Manage/Pay/99bill/PhonePay/Show.aspx";





            //��ȡ�����������˻���
            String merchantAcctId = Request["merchantAcctId"].ToString().Trim();

            //����������������Կ
            ///���ִ�Сд
            String key = UiCommon.StringConfig.JJbill_SZX_Key; //"84YB4FXGFKZ85975";


            //��ȡ���ذ汾.�̶�ֵ
            ///������汾�Ź̶�Ϊv2.0
            String version = Request["version"].ToString().Trim();

            //��ȡ��������.�̶�ѡ��ֵ��
            ///ֻ��ѡ��1��2
            ///1�������ģ�2����Ӣ��
            String language = Request["language"].ToString().Trim();

            //��ȡ֧����ʽ
            ///��ѡ��00��41��42��52
            ///00 �����ǮĬ��֧����ʽ��ĿǰΪ�����п���֧���Ϳ�Ǯ�˻�֧����41 �����Ǯ�˻�֧����42 ���������п���֧���Ϳ�Ǯ�˻�֧����52 ���������п���֧��
            String payType = Request["payType"].ToString().Trim();

            //�����п����
            ///���ͨ�������п�ֱ��֧��ʱ����
            String cardNumber = Request["cardNumber"].ToString().Trim();

            //��ȡ�����п�����
            ///���ͨ�������п�ֱ��֧��ʱ����
            String cardPwd = Request["cardPwd"].ToString().Trim();

            //��ȡ�̻�������
            String orderId = Request["orderId"].ToString().Trim();


            //��ȡԭʼ�������
            ///�����ύ����Ǯʱ�Ľ���λΪ�֡�
            ///�ȷ�2 ������0.02Ԫ
            String orderAmount = Request["orderAmount"].ToString().Trim();

            //��ȡ��Ǯ���׺�
            ///��ȡ�ý����ڿ�Ǯ�Ľ��׺�
            String dealId = Request["dealId"].ToString().Trim();


            //��ȡ�̻��ύ����ʱ��ʱ��
            ///14λ���֡���[4λ]��[2λ]��[2λ]ʱ[2λ]��[2λ]��[2λ]
            ///�磺20080101010101
            String orderTime = Request["orderTime"].ToString().Trim();

            //��ȡ��չ�ֶ�1
            ///���̻��ύ����ʱ����չ�ֶ�1����һ��
            String ext1 = Request["ext1"].ToString().Trim();

            //��ȡ��չ�ֶ�2
            ///���̻��ύ����ʱ����չ�ֶ�2����һ��
            String ext2 = Request["ext2"].ToString().Trim();

            //��ȡʵ��֧�����
            ///��λΪ��
            ///�ȷ� 2 ������0.02Ԫ
            String payAmount = Request["payAmount"].ToString().Trim();

            //��ȡ��Ǯ����ʱ��
            String billOrderTime = Request["billOrderTime"].ToString().Trim();

            //��ȡ������
            ///10����֧���ɹ��� 11����֧��ʧ��
            String payResult = Request["payResult"].ToString().Trim();

            //��ȡǩ������
            ///1����MD5ǩ��
            ///��ǰ�汾�̶�Ϊ1
            String signType = Request["signType"].ToString().Trim();

            //��ȡ����ǩ����
            String signMsg = Request["signMsg"].ToString().Trim();




            //���ɼ��ܴ������뱣������˳��
            String merchantSignMsgVal = "";
            merchantSignMsgVal = appendParam(merchantSignMsgVal, "merchantAcctId", merchantAcctId);
            merchantSignMsgVal = appendParam(merchantSignMsgVal, "version", version);
            merchantSignMsgVal = appendParam(merchantSignMsgVal, "language", language);
            merchantSignMsgVal = appendParam(merchantSignMsgVal, "payType", payType);
            merchantSignMsgVal = appendParam(merchantSignMsgVal, "cardNumber", cardNumber);
            merchantSignMsgVal = appendParam(merchantSignMsgVal, "cardPwd", cardPwd);
            merchantSignMsgVal = appendParam(merchantSignMsgVal, "orderId", orderId);
            merchantSignMsgVal = appendParam(merchantSignMsgVal, "orderAmount", orderAmount);
            merchantSignMsgVal = appendParam(merchantSignMsgVal, "dealId", dealId);
            merchantSignMsgVal = appendParam(merchantSignMsgVal, "orderTime", orderTime);
            merchantSignMsgVal = appendParam(merchantSignMsgVal, "ext1", ext1);
            merchantSignMsgVal = appendParam(merchantSignMsgVal, "ext2", ext2);
            merchantSignMsgVal = appendParam(merchantSignMsgVal, "payAmount", payAmount);
            merchantSignMsgVal = appendParam(merchantSignMsgVal, "billOrderTime", billOrderTime);
            merchantSignMsgVal = appendParam(merchantSignMsgVal, "payResult", payResult);
            merchantSignMsgVal = appendParam(merchantSignMsgVal, "signType", signType);
            merchantSignMsgVal = appendParam(merchantSignMsgVal, "key", key);

            String merchantSignMsg = GetMD5(merchantSignMsgVal, "utf-8");



            //�̼ҽ������ݴ�������ת���̼���ʾ֧�������ҳ��
            ///���Ƚ���ǩ���ַ�����֤
            if (signMsg.ToUpper() == merchantSignMsg.ToUpper())
            {

                switch (payResult)
                {

                    case "10":
                        /*  
                         // �̻���վ�߼������ȷ����¶���֧��״̬Ϊ�ɹ�
                        // �ر�ע�⣺ֻ��signMsg.ToUpper() == merchantSignMsg.ToUpper()����payResult=10���ű�ʾ֧���ɹ���
                        */

                        //���¶���Ϊ���״̬
                        BLL.Member member = new BLL.Member();
                        if (!member.IsPaySuccess(orderId))
                        {
                            int bMoney = Convert.ToInt32(payAmount) / 100;
                            member.Update3PayOrder(string.Empty, bMoney, orderId);
                        }

                        //�������Ǯ�����������ṩ��Ҫ�ض���ĵ�ַ��
                        rtnOk = 1;
                        //rtnUrl = "http://www.yoursite.com/show.aspx?msg=success";
                        rtnUrl = showUrl + "?msg=success";
                        break;

                    default:

                        rtnOk = 1;
                        //rtnUrl = "http://www.yoursite.com/show.aspx?msg=false";
                        rtnUrl = showUrl + "?msg=false";
                        break;
                }

            }
            else
            {
                rtnOk = 1;
                rtnUrl = showUrl + "?msg=error";

            }

        }
        //���ܺ�����������ֵ��Ϊ�յĲ�������ַ���
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

        //���±������Ǯ�����������ṩ��Ҫ�ض���ĵ�ַ

    }
}
