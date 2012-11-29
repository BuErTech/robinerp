using System;
using System.Data;
using System.Configuration;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;
using System.Text;
using System.Security.Cryptography;
using System.Collections.Specialized;
using System.IO;
using System.Data.SqlClient;
using System.Data.OleDb;
using System.Net;

namespace Bzw.Inhersits.Manage.Pay.Alipay
{
    public partial class Manage_Pay_Alipay_Return : UiCommon.BasePage
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
        /// Lab_msg �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.Literal Lab_msg;

        /// <summary>
        /// Control1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.UserControl Control1;
        /// <summary>
        /// created by sunzhizhi 2006.5.21,sunzhizhi@msn.com��
        /// </summary>
        /// 

        public string PayResult = string.Empty;
        public string PayMoney = string.Empty;
        public string PayOrderID = string.Empty;

        protected void Page_Load(object sender, EventArgs e)
        {
            //***************************************************************     
            string alipayNotifyURL = UiCommon.StringConfig.AlipayNotifyURL;
            //partner �Ķ�Ӧ���װ�ȫУ���루������д��
            string key = UiCommon.StringConfig.AlipayKey;
            string _input_charset = "utf-8";
            //partner�������id��������д��
            string partner = UiCommon.StringConfig.AlipayPartnerID;

            alipayNotifyURL = alipayNotifyURL + "service=notify_verify" + "&partner=" + partner + "&notify_id=" + Request.QueryString["notify_id"];

            //��ȡ֧����ATN���ؽ����true����ȷ�Ķ�����Ϣ��false ����Ч��
            string responseTxt = Get_Http(alipayNotifyURL, 120000);
            int i;
            NameValueCollection coll;
            //Load Form variables into NameValueCollection variable.
            coll = Request.QueryString;

            // Get names of all forms into a string array.
            String[] requestarr = coll.AllKeys;


            //��������
            string[] Sortedstr = BubbleSort(requestarr);

            //for (i = 0; i < Sortedstr.Length; i++)
            //{ 
            //   Response.Write("Form: " + Sortedstr[i] + "=" + Request.QueryString[Sortedstr[i]] + "<br>");
            //}

            //�����md5ժҪ�ַ��� ��

            StringBuilder prestr = new StringBuilder();

            for (i = 0; i < Sortedstr.Length; i++)
            {
                if (Request.Form[Sortedstr[i]] != "" && Sortedstr[i] != "sign" && Sortedstr[i] != "sign_type")
                {
                    if (i == Sortedstr.Length - 1)
                    {
                        prestr.Append(Sortedstr[i] + "=" + Request.QueryString[Sortedstr[i]]);
                    }
                    else
                    {
                        prestr.Append(Sortedstr[i] + "=" + Request.QueryString[Sortedstr[i]] + "&");

                    }
                }
            }

            prestr.Append(key);
            //**********************************************************************
            //����Md5ժҪ��
            // string mysign = GetMD5(prestr.ToString(), _input_charset);
            string mysign = GetMD5(prestr.ToString(), _input_charset);
            string sign = Request.QueryString["sign"];
            string trade_status = Request.QueryString["trade_status"];

            //Response.Write(prestr.ToString());  //��ӡ����֪ͨ��Ϣ�����յ���


            //������ˮ��
            string Orderid = Request.QueryString["out_trade_no"].Trim();
            //֧�����
            string payAmount = Request.QueryString["total_fee"].Trim();

            if (mysign == sign && responseTxt == "true")   //��֤֧������������Ϣ��ǩ���Ƿ���ȷ
            {

                //�����Լ����ݿ�Ķ�����䣬���Լ���дһ��**************************************************


                BLL.Member member = new BLL.Member();
                if (!member.IsPaySuccess(Orderid)) //��ֹ�ظ�ˢ��ҳ�������������
                {
                    //���¶���״̬Ϊ���
                    //member.Update3PayOrder( string.Empty, (int)( float.Parse( payAmount ) ), Orderid );
                    member.Update3PayOrder((int)(float.Parse(payAmount)), Orderid);
                }
                PayResult = "��ϲ����֧���ɹ���";
                PayOrderID = Orderid;
                PayMoney = payAmount + " Ԫ";

                //Lab_msg.Text = "��ϲ������ֵ�ɹ���������Ϊ" + Orderid + "����ֵ���Ϊ" + payAmount + "Ԫ��<br>";

                Response.Write("success");     //���ظ�֧������Ϣ���ɹ������÷��ظ�֧������
                //**************************************************************************
                //��ӡ������֪ͨ��������
                //Response.Write("------------------------------------------");
                //Response.Write("<br>Result:responseTxt=" + responseTxt);
                //Response.Write("<br>Result:mysign=" + mysign);
                //Response.Write("<br>Result:sign=" + sign);

            }
            else
            {
                PayResult = "��Ǹ��֧��ʧ�ܣ�";
                PayOrderID = Orderid;
                PayMoney = payAmount + " Ԫ�������ȷ��Ǯ��֧�����뼰ʱ�����Ƿ��������⣡";
                // Lab_msg.Text = "�Բ��𣬳�ֵʧ�ܣ�������Ϊ" + Orderid + "����ֵ���Ϊ" + payAmount + "Ԫ�������ȷ��Ǯ��֧�����뼰ʱ�����Ƿ��������⣡<br>";

                //**************************************************************************
                //��ӡ������֪ͨ��������
                //Response.Write("------------------------------------------");
                //Response.Write("<br>Result:responseTxt=" + responseTxt);
                //Response.Write("<br>Result:mysign=" + mysign);
                //Response.Write("<br>Result:sign=" + sign);
            }

        }

        public static string GetMD5(string s, string _input_charset)
        {

            /// <summary>
            /// ��ASP���ݵ�MD5�����㷨
            /// </summary>

            MD5 md5 = new MD5CryptoServiceProvider();
            byte[] t = md5.ComputeHash(Encoding.GetEncoding(_input_charset).GetBytes(s));
            StringBuilder sb = new StringBuilder(32);
            for (int i = 0; i < t.Length; i++)
            {
                sb.Append(t[i].ToString("x").PadLeft(2, '0'));
            }
            return sb.ToString();
        }

        public static string[] BubbleSort(string[] r)
        {
            /// <summary>
            /// ð������
            /// </summary>

            int i, j; //������־ 
            string temp;

            bool exchange;

            for (i = 0; i < r.Length; i++) //�����R.Length-1������ 
            {
                exchange = false; //��������ʼǰ��������־ӦΪ��

                for (j = r.Length - 2; j >= i; j--)
                {
                    if (System.String.CompareOrdinal(r[j + 1], r[j]) < 0)��//��������
                    {
                        temp = r[j + 1];
                        r[j + 1] = r[j];
                        r[j] = temp;

                        exchange = true; //�����˽������ʽ�������־��Ϊ�� 
                    }
                }

                if (!exchange) //��������δ������������ǰ��ֹ�㷨 
                {
                    break;
                }

            }
            return r;
        }


        //��ȡԶ�̷�����ATN���
        public String Get_Http(String a_strUrl, int timeout)
        {
            string strResult;
            try
            {

                HttpWebRequest myReq = (HttpWebRequest)HttpWebRequest.Create(a_strUrl);
                myReq.Timeout = timeout;
                HttpWebResponse HttpWResp = (HttpWebResponse)myReq.GetResponse();
                Stream myStream = HttpWResp.GetResponseStream();
                StreamReader sr = new StreamReader(myStream, Encoding.Default);
                StringBuilder strBuilder = new StringBuilder();
                while (-1 != sr.Peek())
                {
                    strBuilder.Append(sr.ReadLine());
                }

                strResult = strBuilder.ToString();
            }
            catch (Exception exp)
            {

                strResult = "����" + exp.Message;
            }

            return strResult;
        }
    }
}
