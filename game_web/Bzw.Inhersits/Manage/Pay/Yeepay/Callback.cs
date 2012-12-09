using System;
using System.Data;
using System.Data.SqlClient;
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
using Utility;
namespace Bzw.Inhersits.Manage.Pay.Yeepay
{
    public partial class Manage_Pay_Yeepay_Callback : UiCommon.BasePage
    {
        protected string p1_MerId;
        protected string keyValue;

        protected void Page_Load(object sender, EventArgs e)
        {
            bool res = false;

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
                            res = true;
                            UpdateDB(result);

                            Response.Write("֧���ɹ���<br />��ֵ��" + result.R5_Pid + "<br />֧����" + result.R3_Amt + "<br><a href='/Manage/' title='���ػ�Ա����'>���ػ�Ա����</a>");
                        }
                        else if (result.R9_BType == "2")
                        {
                            res = true;
                            UpdateDB(result);
                            // * ����Ƿ��������ػ��ߵ绰֧������(result.R9_BType==2 or result.R9_BType==3)����Ҫ��Ӧһ���ض��ַ���'SUCCESS',����'SUCCESS'֮ǰ���������κ������ַ����,��֤�����������'SUCCESS'�ַ���
                            Response.Write("SUCCESS");
                        }
                        else if (result.R9_BType == "3")
                        {
                            res = true;
                            UpdateDB(result);
                            // * ����Ƿ��������ػ��ߵ绰֧������(result.R9_BType==2 or result.R9_BType==3)����Ҫ��Ӧһ���ض��ַ���'SUCCESS',����'SUCCESS'֮ǰ���������κ������ַ����,��֤�����������'SUCCESS'�ַ���
                            Response.Write("SUCCESS");
                        }

                        if(res)
                        {
                            string r8mp = result.R8_MP;
                            int pos = r8mp.IndexOf("$");
                            string username = r8mp.Substring(0, pos);
                            int couponNum = Convert.ToInt32(
                                r8mp.Substring(pos + 1)
                                );

                            string strsql =
                                "select UserID from TUsers where UserName=@username";
                            DataTable dt=
                                SqlHelper.ExecuteDataset(CommandType.Text,
                                strsql,
                                new SqlParameter[]
                                {
                                    new SqlParameter("@username",username)
                                }
                                ).Tables[0];
                            int userid = Convert.ToInt32( dt.Rows[0]["UserID"] );

                            strsql =
                                "insert into TCoupon(UserID,CouponNum)values(@userid,@coupon)";
                            int num =
                                SqlHelper.ExecuteNonQuery
                                (CommandType.Text,
                                strsql,
                                new SqlParameter[]
                                {
                                    new SqlParameter("@userid",userid),
                                    new SqlParameter("@coupon",couponNum)
                                });
                            if(num!=1)
                            {
                                Response.Write("<script>alert('��ֵ�ɹ��������ͽ���ʧ�ܣ�')</script>");
                            }
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
