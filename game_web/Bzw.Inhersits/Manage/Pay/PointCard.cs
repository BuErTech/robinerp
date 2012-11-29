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
using Bzw.Data;
using Bzw.WebLibrary;

namespace Bzw.Inhersits.Manage.Pay
{
    public partial class Manage_Pay_PointCard :UiCommon.BasePage
    {
          /// <summary>
    /// Manage_Pay_PointCard �ࡣ
    /// </summary>
    /// <remarks>
    /// �Զ����ɵ��ࡣ
    /// </remarks>

        
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
        /// 
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
        /// Form1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlForm Form1;
        
        /// <summary>
        /// lblMsg �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.Label lblMsg;
        
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
        /// txtCardNo �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.TextBox txtCardNo;
        
        /// <summary>
        /// txtCardPwd �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.TextBox txtCardPwd;
        
        /// <summary>
        /// txtValidCode �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.WebControls.TextBox txtValidCode;
        
        /// <summary>
        /// imageField2 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.HtmlControls.HtmlInputSubmit imageField2;
        
        /// <summary>
        /// webfooter1 �ؼ���
        /// </summary>
        /// <remarks>
        /// �Զ����ɵ��ֶΡ�
        /// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
        /// </remarks>
        protected global::System.Web.UI.UserControl webfooter1;
        /// <summary>
        /// ddlGameList �ؼ�
        /// </summary>
        protected global::System.Web.UI.WebControls.DropDownList ddlGameList;

        protected global::System.Web.UI.WebControls.RadioButtonList rblPayType;

        protected void Page_Load(object sender, EventArgs e)
        {
            SeoSetting(SeoConfig.Config.DefaultSeo);
            if (!IsPostBack)
            {
                if (UiCommon.UserLoginInfo.IsLogin)
                {
                    txtUserName.Text = UiCommon.UserLoginInfo.UserName;
                    txtUserName2.Text = UiCommon.UserLoginInfo.UserName;
                }

                DataTable dt = DbSession.Default.FromSql("SELECT NameID,ComName FROM dbo.TGameNameInfo WHERE NameID IN (SELECT GameNameID FROM dbo.TGameRoomInfo WHERE GameTypeID=1 GROUP BY GameNameID) ORDER BY dbo.TGameNameInfo.IDSort").ToDataTable();

                
             
                ddlGameList.DataSource = dt;
                ddlGameList.DataTextField = "ComName";
                ddlGameList.DataValueField = "NameID";
                ddlGameList.DataBind();
                
            }

        }
        
        protected void button5_ServerClick(object sender, EventArgs e)
        {
            if (!BCST.Common.CommonManager.Web.CheckPostSource())
                return;
            //1.У����֤��
            if (UiCommon.ValidCode.CurrentCode == null)
            {
                lblMsg.Text = "��֤���ѹ��ڣ��������µ���֤�룡";
                txtValidCode.Text = string.Empty;
                return;
            }
            else if (txtValidCode.Text.Trim() != UiCommon.ValidCode.CurrentCode)
            {
                lblMsg.Text = "��֤����������������µ���֤�룡";
                txtValidCode.Text = string.Empty;
                return;
            }

            //2.�ж��û����Ƿ����
            BLL.Member member = new BLL.Member();
            string userName = Utility.Common.SqlEncode(txtUserName.Text.Trim());
            if (!member.ExistName(userName))
            {
                lblMsg.Text = "���������Ϸ�ʺ�(�û���)�����ڣ�";
                txtValidCode.Text = string.Empty;
                return;
            }
            //�ж���ҳ�ֵʱ����������Ϸ��
            if (member.IsInRoomOrGame(userName))
            {
                lblMsg.Text = "Ҫ��ֵ���ʺ�������Ϸ�У������˳���Ϸ�ٽ��г�ֵ��";
                txtValidCode.Text = string.Empty;
                return;
               
            }
            //3.�жϳ�ֵ���Ƿ��ѳ�ֵ���򿨺ż��������
            string cardNo = Utility.Common.SqlEncode(txtCardNo.Text.Trim());
            string cardPwd = Utility.Common.SqlEncode(txtCardPwd.Text.Trim());
            int cardState = new BLL.Card().PointCardState(cardNo, cardPwd);
            if (cardState == -1)
            {
                lblMsg.Text = "�Բ���������Ŀ��Ż���������";
                txtValidCode.Text = string.Empty;
                return;
            }
            else if (cardState == 1)
            {
                lblMsg.Text = "�Բ���������Ŀ����Ѿ�����ֵ��";
                txtValidCode.Text = string.Empty;
                return;
            }

            #region ��ѿ�
            
		// ��ѿ�������
        int freeCardForUser = 0;
        DataRow dr = member.GetFreeCardCount();
        if (dr != null)
        {
            freeCardForUser = Convert.ToInt32(dr["FreeCardCount"]);
        }
        
        
        //������ѿ������ж��û��Ƿ��Ѿ���ֵ��XX�š��磺��ѿ�ÿ���û�ֻ�ܳ�ֵ2�š�
        if (member.IsFreeCard(cardNo))
        {
            int cardCount = member.FreeCardCount(userName);
            if (cardCount >= freeCardForUser)
            {
                lblMsg.Text = "���Ѿ�ʹ��" + freeCardForUser + "����ѿ��ˣ�";
                txtValidCode.Text = string.Empty;
                return;
            }
		} 
            #endregion

            //4.����ֵ���ת���ɽ�Ҵ�������
            //����ֵ�û�������Ƽ��ˣ���Web_Config�и��ݲ���ֵ�����Ƽ���
            //���µ㿨״̬
            //by YMH 2012-9-4  ��ֵ����
         //���ֳ�ֵ����
        if (ConfigurationManager.AppSettings["IsPayToPoint"] == "1" && rblPayType.SelectedValue=="point" && ddlGameList.SelectedValue!="")
        { 
            //��ֵ����
            int result = member.PointCardFull2(userName, cardNo, cardPwd, Utility.Common.RequestIP,ddlGameList.SelectedValue);
            if (result == 0)
            {
                lblMsg.Text = "��ֵ��[" + cardNo + "]��ֵ�ɹ�����ֵ����Ϸ�ʺ�(�û���)��" + userName + "��" + DateTime.Now;
                txtUserName.Text = string.Empty;
                txtUserName2.Text = string.Empty;
                txtCardNo.Text = string.Empty;
                txtValidCode.Text = string.Empty;
            }
            else if (result == -1)
            {
                lblMsg.Text = "��Ǹ���㿨�����ڻ�㿨�š����벻��ȷ��";
                txtValidCode.Text = string.Empty;
            }
            else
            {
                lblMsg.Text = "��Ǹ���㿨��ֵʧ�ܣ������Ƿ�������æ�����Ժ����ԣ�";
                txtValidCode.Text = string.Empty;
            }
        }
        else//��ֵ���
        {
            if (member.PointCardFull(userName, cardNo, cardPwd, Utility.Common.RequestIP))
            {
                lblMsg.Text = "��ֵ��[" + cardNo + "]��ֵ�ɹ�����ֵ����Ϸ�ʺ�(�û���)��" + userName + "��" + DateTime.Now;
                txtUserName.Text = string.Empty;
                txtUserName2.Text = string.Empty;
                txtCardNo.Text = string.Empty;
                txtValidCode.Text = string.Empty;
            }
            else
            {
                lblMsg.Text = "��Ǹ���㿨��ֵʧ�ܣ������Ƿ�������æ�����Ժ����ԣ�";
                txtValidCode.Text = string.Empty;
            }
        }
        }
    }
}
