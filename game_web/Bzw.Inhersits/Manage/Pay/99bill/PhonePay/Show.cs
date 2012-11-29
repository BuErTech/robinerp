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

namespace Bzw.Inhersits.Manage.Pay._99bill.PhonePay
{
    public partial class Manage_Pay_99bill_PhonePay_Show : UiCommon.BasePage
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
        /// form2 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlForm form2;

        /// <summary>
        /// Lab_orderId �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.Literal Lab_orderId;

        /// <summary>
        /// Lab_orderAmount �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.Literal Lab_orderAmount;

        /// <summary>
        /// Lab_msg �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.Literal Lab_msg;

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

            if (Request["orderId"] == null)
            {
                Response.Redirect(toUrl);
            }
            string Orderid = Request["orderId"].ToString().Trim();

            if (Request["payAmount"] == null)
            {
                Response.Redirect(toUrl);
            }
            string payAmount = Request["payAmount"].ToString().Trim();

            //�������������ʾ
            //�������
            Lab_orderId.Text = Orderid;
            //�������ѷ�ת����Ԫ
            int bMoney = Convert.ToInt32(Double.Parse(payAmount) / 100);
            Lab_orderAmount.Text = bMoney.ToString();//�õ�ʵ�ʳ�ֵ�Ľ��

            if (Request["msg"] == null)
            {
                Response.Redirect(toUrl);
            }
            string success = Request["msg"].ToString();

            if (success.Trim().ToLower() == "success")
            {
                Lab_msg.Text = "��ϲ������ֵ�ɹ���";

                /*
                BLL.Member member = new BLL.Member();
                if (!member.IsPaySuccess(Orderid)) //��ֹ�ظ�ˢ��ҳ�������������
                {
                    member.Update3PayOrder(string.Empty, bMoney, Orderid);
                }*/
            }
            else if (success.Trim().ToLower() == "false") //success = false
            {
                Lab_msg.Text = "�Բ��𣬳�ֵʧ�ܣ����Ժ��ټ������ԣ��ص�<a href=\"" + toUrl + "\"><b>ȫ���ƶ��ֻ����ų�ֵ</b></a>ҳ��";
            }
            else //success = error
            {
                Lab_msg.Text = "�Բ��𣬷���֧�����󣬵��³�ֵʧ�ܣ������ȷ��Ǯ��֧�����뼰ʱ�����Ƿ��������⣡";
            }

        }
    }
}
