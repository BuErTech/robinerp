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
using com.yeepay;
using UiCommon;
namespace Bzw.Inhersits.Manage.Pay.Yeepay
{
    public partial class Manage_Pay_Yeepay_Callback : UiCommon.BasePage
    {
        protected string p1_MerId;
        protected string keyValue;

        protected void Page_Load(object sender, EventArgs e)
        {
            if (!IsPostBack)
            {
                //p1_MerId = "10000432521";                                     // �̼�ID
                //keyValue = "8UPp0KE8sq73zVP370vko7C39403rtK1YwX40Td6irH216036H27Eb12792t";  // �̼���Կ

                p1_MerId = StringConfig.YeepayMerID;// �̼�ID
                keyValue = StringConfig.YeepayKey;  // �̼���Կ

                // У�鷵�����ݰ�
                BuyCallbackResult result = Buy.VerifyCallback(p1_MerId, keyValue, Buy.GetQueryString("r0_Cmd"), Buy.GetQueryString("r1_Code"), Buy.GetQueryString("r2_TrxId"),
                    Buy.GetQueryString("r3_Amt"), Buy.GetQueryString("r4_Cur"), Buy.GetQueryString("r5_Pid"), Buy.GetQueryString("r6_Order"), Buy.GetQueryString("r7_Uid"),
                    Buy.GetQueryString("r8_MP"), Buy.GetQueryString("r9_BType"), Buy.GetQueryString("rp_PayDate"), Buy.GetQueryString("hmac"));

                if (string.IsNullOrEmpty(result.ErrMsg))
                {
                    if (result.R1_Code == "1")
                    {
                        if (result.R9_BType == "1")
                        {
                            //  callback��ʽ:������ض���
                            UpdateDB(result);
                            Response.Write("֧���ɹ���<br />��ֵ��" + result.R5_Pid + "<br />֧����" + result.R3_Amt + "<br><a href='/Manage/' title='���ػ�Ա����'>���ػ�Ա����</a>");
                        }
                        else if (result.R9_BType == "2")
                        {
                            UpdateDB(result);
                            // * ����Ƿ��������ػ��ߵ绰֧������(result.R9_BType==2 or result.R9_BType==3)����Ҫ��Ӧһ���ض��ַ���'SUCCESS',����'SUCCESS'֮ǰ���������κ������ַ����,��֤�����������'SUCCESS'�ַ���
                            Response.Write("SUCCESS");
                        }
                        else if (result.R9_BType == "3")
                        {
                            UpdateDB(result);
                            // * ����Ƿ��������ػ��ߵ绰֧������(result.R9_BType==2 or result.R9_BType==3)����Ҫ��Ӧһ���ض��ַ���'SUCCESS',����'SUCCESS'֮ǰ���������κ������ַ����,��֤�����������'SUCCESS'�ַ���
                            Response.Write("SUCCESS");
                        }
                    }
                    else
                    {
                        Response.Write("֧��ʧ��!");
                    }
                }
                else
                {
                    Response.Write("����ǩ����Ч!");
                }
            }
        }

        private void UpdateDB(BuyCallbackResult result)
        {
            if (result == null)
                return;

            //������ˮ��
            string Orderid = result.R6_Order;
            //֧�����
            string payAmount = result.R3_Amt;

            BLL.Member member = new BLL.Member();
            if (!member.IsPaySuccess(Orderid)) //��ֹ�ظ�ˢ��ҳ�������������
            {
                //���¶�����ϢΪ���״̬
                member.Update3PayOrder((int)(float.Parse(payAmount)), Orderid);
            }
        }
    }
}
