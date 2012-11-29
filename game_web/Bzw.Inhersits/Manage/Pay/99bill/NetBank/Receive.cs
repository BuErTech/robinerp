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

namespace Bzw.Inhersits.Manage.Pay._99bill.NetBank
{
    public partial class Manage_Pay_99bill_NetBank_Receive : UiCommon.BasePage
    {
        /**
         * @Description: ��Ǯ�����֧�����ؽӿڷ���
         * @Copyright (c) �Ϻ���Ǯ��Ϣ�������޹�˾
         * @version 2.0
         */

        //��ʼ���������ַ
        public int rtnOk = 0;
        public String rtnUrl = "";
        protected void Page_Load(object sender, EventArgs e)
        {
            //����֧�������ҳ���ַ
            string showUrl = UiCommon.StringConfig.WebSiteName + "/Manage/Pay/99bill/NetBank/Show.aspx";



            //��ȡ����������˻���     
            String merchantAcctId = Request["merchantAcctId"].ToString().Trim();

            //���������������Կ
            ///���ִ�Сд
            String key = UiCommon.StringConfig.JJbill_NetBank_Key; //"K65XTCHC2N72M4MS";

            //��ȡ���ذ汾.�̶�ֵ
            ///��Ǯ����ݰ汾�������ö�Ӧ�Ľӿڴ������
            ///������汾�Ź̶�Ϊv2.0
            String version = Request["version"].ToString().Trim();

            //��ȡ��������.�̶�ѡ��ֵ��
            ///ֻ��ѡ��1��2��3
            ///1�������ģ�2����Ӣ��
            ///Ĭ��ֵΪ1
            String language = Request["language"].ToString().Trim();

            //ǩ������.�̶�ֵ
            ///1����MD5ǩ��
            ///��ǰ�汾�̶�Ϊ1
            String signType = Request["signType"].ToString().Trim();

            //��ȡ֧����ʽ
            ///ֵΪ��10��11��12��13��14
            ///00�����֧��������֧��ҳ����ʾ��Ǯ֧�ֵĸ���֧����ʽ���Ƽ�ʹ�ã�10�����п�֧��������֧��ҳ��ֻ��ʾ���п�֧����.11���绰����֧��������֧��ҳ��ֻ��ʾ�绰֧����.12����Ǯ�˻�֧��������֧��ҳ��ֻ��ʾ��Ǯ�˻�֧����.13������֧��������֧��ҳ��ֻ��ʾ����֧����ʽ��.14��B2B֧��������֧��ҳ��ֻ��ʾB2B֧��������Ҫ���Ǯ���뿪ͨ����ʹ�ã�
            String payType = Request["payType"].ToString().Trim();

            //a
            ///�μ����д����б�
            String bankId = Request["bankId"].ToString().Trim();

            //��ȡ�̻�������
            String orderId = Request["orderId"].ToString().Trim();

            //��ȡ�����ύʱ��
            ///��ȡ�̻��ύ����ʱ��ʱ��.14λ���֡���[4λ]��[2λ]��[2λ]ʱ[2λ]��[2λ]��[2λ]
            ///�磺20080101010101
            String orderTime = Request["orderTime"].ToString().Trim();

            //��ȡԭʼ�������
            ///�����ύ����Ǯʱ�Ľ���λΪ�֡�
            ///�ȷ�2 ������0.02Ԫ
            String orderAmount = Request["orderAmount"].ToString().Trim();

            //��ȡ��Ǯ���׺�
            ///��ȡ�ý����ڿ�Ǯ�Ľ��׺�
            String dealId = Request["dealId"].ToString().Trim();

            //��ȡ���н��׺�
            ///���ʹ�����п�֧��ʱ�������еĽ��׺š��粻��ͨ������֧������Ϊ��
            String bankDealId = Request["bankDealId"].ToString().Trim();

            //��ȡ�ڿ�Ǯ����ʱ��
            ///14λ���֡���[4λ]��[2λ]��[2λ]ʱ[2λ]��[2λ]��[2λ]
            ///�磻20080101010101
            String dealTime = Request["dealTime"].ToString().Trim();

            //��ȡʵ��֧�����
            ///��λΪ��
            ///�ȷ� 2 ������0.02Ԫ
            String payAmount = Request["payAmount"].ToString().Trim();

            //��ȡ����������
            ///��λΪ��
            ///�ȷ� 2 ������0.02Ԫ
            String fee = Request["fee"].ToString().Trim();

            //��ȡ��չ�ֶ�1
            String ext1 = Request["ext1"].ToString().Trim();

            //��ȡ��չ�ֶ�2
            String ext2 = Request["ext2"].ToString().Trim();

            //��ȡ������
            ///10���� �ɹ�; 11���� ʧ��
            ///00���� �¶����ɹ������Ե绰����֧���������أ�;01���� �¶���ʧ�ܣ����Ե绰����֧���������أ�
            String payResult = Request["payResult"].ToString().Trim();

            //��ȡ�������
            ///��ϸ���ĵ���������б�
            String errCode = Request["errCode"].ToString().Trim();

            //��ȡ����ǩ����
            String signMsg = Request["signMsg"].ToString().Trim();



            //���ɼ��ܴ������뱣������˳��
            String merchantSignMsgVal = "";
            merchantSignMsgVal = appendParam(merchantSignMsgVal, "merchantAcctId", merchantAcctId);
            merchantSignMsgVal = appendParam(merchantSignMsgVal, "version", version);
            merchantSignMsgVal = appendParam(merchantSignMsgVal, "language", language);
            merchantSignMsgVal = appendParam(merchantSignMsgVal, "signType", signType);
            merchantSignMsgVal = appendParam(merchantSignMsgVal, "payType", payType);
            merchantSignMsgVal = appendParam(merchantSignMsgVal, "bankId", bankId);
            merchantSignMsgVal = appendParam(merchantSignMsgVal, "orderId", orderId);
            merchantSignMsgVal = appendParam(merchantSignMsgVal, "orderTime", orderTime);
            merchantSignMsgVal = appendParam(merchantSignMsgVal, "orderAmount", orderAmount);
            merchantSignMsgVal = appendParam(merchantSignMsgVal, "dealId", dealId);
            merchantSignMsgVal = appendParam(merchantSignMsgVal, "bankDealId", bankDealId);
            merchantSignMsgVal = appendParam(merchantSignMsgVal, "dealTime", dealTime);
            merchantSignMsgVal = appendParam(merchantSignMsgVal, "payAmount", payAmount);
            merchantSignMsgVal = appendParam(merchantSignMsgVal, "fee", fee);
            merchantSignMsgVal = appendParam(merchantSignMsgVal, "ext1", ext1);
            merchantSignMsgVal = appendParam(merchantSignMsgVal, "ext2", ext2);
            merchantSignMsgVal = appendParam(merchantSignMsgVal, "payResult", payResult);
            merchantSignMsgVal = appendParam(merchantSignMsgVal, "errCode", errCode);
            merchantSignMsgVal = appendParam(merchantSignMsgVal, "key", key);

            String merchantSignMsg = FormsAuthentication.HashPasswordForStoringInConfigFile(merchantSignMsgVal, "MD5");



            //�̼ҽ������ݴ�������ת���̼���ʾ֧�������ҳ��
            ///���Ƚ���ǩ���ַ�����֤
            if (signMsg.ToUpper() == merchantSignMsg.ToUpper())
            {

                switch (payResult)
                {

                    case "10":
                        /*  
                         ' �̻���վ�߼������ȷ����¶���֧��״̬Ϊ�ɹ�
                        ' �ر�ע�⣺ֻ��signMsg.ToUpper() == merchantSignMsg.ToUpper()����payResult=10���ű�ʾ֧���ɹ���
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

        //���±������Ǯ�����������ṩ��Ҫ�ض���ĵ�ַ

    }
}
