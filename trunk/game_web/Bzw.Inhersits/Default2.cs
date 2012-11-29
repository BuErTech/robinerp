using System;
using System.Data;
using System.Configuration;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;
using System.Collections;

using System.Text;
using Bzw.Data;
using Bzw.Entities;
using System.Data.SqlClient;
using Utility;
using Bzw.WebLibrary;
using BLL;
using System.IO;


namespace Bzw.Inhersits
{
	
	public partial class Default2:UiCommon.BasePage
	{
		protected HtmlHead Head1;

		/// <summary>
		/// webmeta1 �ؼ���
		/// </summary>
		/// <remarks>
		/// �Զ����ɵ��ֶΡ�
		/// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
		/// </remarks>
		protected UserControl webmeta1;

		/// <summary>
		/// webtop1 �ؼ���
		/// </summary>
		/// <remarks>
		/// �Զ����ɵ��ֶΡ�
		/// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
		/// </remarks>
		protected UserControl webtop1;

		/// <summary>
		/// webuserlogin1 �ؼ���
		/// </summary>
		/// <remarks>
		/// �Զ����ɵ��ֶΡ�
		/// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
		/// </remarks>
		protected UserControl webuserlogin1;

		/// <summary>
		/// webservice1 �ؼ���
		/// </summary>
		/// <remarks>
		/// �Զ����ɵ��ֶΡ�
		/// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
		/// </remarks>
		protected UserControl webservice1; 
	 
		/// <summary>
		/// rpNewHuoDong �ؼ���
		/// </summary>
		/// <remarks>
		/// �Զ����ɵ��ֶΡ�
		/// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
		/// </remarks>
        protected Repeater rptNews;


        protected Repeater rptQuesNews;

		/// <summary>
		/// odjDataHuoDong �ؼ���
		/// </summary>
		/// <remarks>
		/// �Զ����ɵ��ֶΡ�
		/// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
		/// </remarks>
		protected ObjectDataSource odjDataHuoDong;

		/// <summary>
		/// rpNewGame �ؼ���
		/// </summary>
		/// <remarks>
		/// �Զ����ɵ��ֶΡ�
		/// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
		/// </remarks>
		//protected Repeater rpNewGame;

		/// <summary>
		/// odjDataGame �ؼ���
		/// </summary>
		/// <remarks>
		/// �Զ����ɵ��ֶΡ�
		/// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
		/// </remarks>
		protected ObjectDataSource odjDataGame;

	 

		/// <summary>
		/// odjDataSystem �ؼ���
		/// </summary>
		/// <remarks>
		/// �Զ����ɵ��ֶΡ�
		/// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
		/// </remarks>
		protected ObjectDataSource odjDataSystem;

		/// <summary>
		/// rpDaoJu �ؼ���
		/// </summary>
		/// <remarks>
		/// �Զ����ɵ��ֶΡ�
		/// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
		/// </remarks>
		//protected Repeater rpDaoJu;

		/// <summary>
		/// rptUserPicList �ؼ���
		/// </summary>
		/// <remarks>
		/// �Զ����ɵ��ֶΡ�
		/// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
		/// </remarks>
        protected Repeater rptPhotoList;

		/// <summary>
		/// webshortcutkey1 �ؼ���
		/// </summary>
		/// <remarks>
		/// �Զ����ɵ��ֶΡ�
		/// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
		/// </remarks>
		//protected Public_WebShortCutKey webshortcutkey1;

		/// <summary>
		/// rpCaifu �ؼ���
		/// </summary>
		/// <remarks>
		/// �Զ����ɵ��ֶΡ�
		/// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
		/// </remarks>
		 protected Repeater rpCaifu;

         protected Repeater rpMeili;

         protected Repeater rptLotteries;

         protected Repeater rptGameTime;

         protected Repeater rptGameRank;

         protected Repeater rptMatchRank1;
         protected Repeater rptMatchRank2;
         protected Repeater rptMatchRank3;
         protected Repeater rptMatchRank4;
         protected Repeater rptMatchRank5;
		/// <summary>
		/// webfooter1 �ؼ���
		/// </summary>
		/// <remarks>
		/// �Զ����ɵ��ֶΡ�
		/// Ҫ�����޸ģ��뽫�ֶ�������������ļ��Ƶ����������ļ���
		/// </remarks>
		protected UserControl webfooter1;

        //protected Repeater rptGame1;
        //protected Repeater rptGame2;
        //protected Repeater rptGame3;

		 public int CaifuNo = 0;
    protected string fullUrl = "";

    protected DataTable AdsDT;

    protected DataRow UserPhotoNews;

	public int rankid = 0;

    public int helpid = 0;
    public BLL.Member member = new BLL.Member();
    protected void Page_Load(object sender, EventArgs e)
    {
        //Seo seo = SeoConfig.Config.IndexSeo;
        //Response.Write(string.IsNullOrEmpty(seo.Keyword) ? "a" : seo.Keyword);
        //Response.End();
        SeoSetting(SeoConfig.Config.IndexSeo);
        
		if (!IsPostBack)
		{
            //���Ŷ�̬
            BindNews();
            //��������
           BindQuesNews();
            //�Ƹ�����
           BindCaifu();
            //����ֵ����
           BindMeiliList();
            //��ȯ����
           BindLotteriesList();
            //��Ϸʱ������
           BindGameTimeList();
            //�����峡������
           BindTopFiveMatch();
            //�����������
           InitUserAlbum();
           //����Ϸ�б�
        //    BindGameList();
		}
    }
        //���Ŷ�̬
        public void BindNews()
        {
            rptNews.DataSource = (new BLL.News()).ListByTypeName("���Ŷ�̬", 8);
            rptNews.DataBind();
        }
         //��������
        public void BindQuesNews()
        {
            rptQuesNews.DataSource = (new BLL.News()).ListByTypeName("��������", 8);
            rptQuesNews.DataBind();
        }
        //����ֵ����
        public void BindMeiliList()
        {

            string where = " and meili>0";
            if (ConfigurationManager.AppSettings["RobotRank"] != null && ConfigurationManager.AppSettings["RobotRank"] == "0")
            {
                where += " and userid in (select userid from tusers where isrobot=0)";
            }
            int counts;
            DataTable dt = new BLL.Member().PagerListFascination(10, 1, out counts, where);

            rpMeili.DataSource = dt.DefaultView;
            rpMeili.DataBind();
        }
        //��ȯ����
        public void BindLotteriesList()
        {
            DataTable dt = new BLL.Member().ListLotteries(10);
            rptLotteries .DataSource = dt.DefaultView;
            rptLotteries.DataBind();
        }
        //��Ϸʱ������
        public void BindGameTimeList()
        {
            DataTable dt = new BLL.Member().ListPlayTimeCount(10);
            rptGameTime.DataSource = dt.DefaultView;
            rptGameTime.DataBind();
        }
        /// <summary>
        /// ��ʼ��������ҵ�������Ƭ
        /// </summary>
        private void InitUserAlbum()
        {
            //DataTable dt = DbSession.Default.FromSql("select top 20 * from Bzw_GetUserPic()").ToDataTable();
            DataTable dt = DbSession.Default.From<VUsersAlbum>().Where(VUsersAlbum._.Enable == Library.EnableState.ͨ��.ToString() && VUsersAlbum._.IsPublic == Library.PublicState.����.ToString()).OrderBy(VUsersAlbum._.InputTime.Desc).ToDataTable(20);
            if (dt.Rows.Count > 0)
            {
                rptPhotoList.DataSource = dt;
                rptPhotoList.DataBind();
            }
        }
        /// <summary>
        /// ����5�������� 
        /// </summary>
        private void BindTopFiveMatch()
        {
            
            DataTable dt = DbSession.Default.FromSql("select top 5 Name,ContestID from Web_VTContestRoom where  EndTime<getdate() and comName is not null order by startTime desc").ToDataTable();
            rptGameRank.DataSource = dt.DefaultView;
            rptGameRank.DataBind();
            if (dt.Rows.Count > 0)
            {
                int i = 0;
                Game gm = new Game();
                foreach (DataRow row in dt.Rows)
                {
                    switch (i) {
                        case 0: rptMatchRank1.DataSource = gm.GetMatchRankDetail(row["ContestID"].ToString()); rptMatchRank1.DataBind(); break;
                        case 1: rptMatchRank2.DataSource = gm.GetMatchRankDetail(row["ContestID"].ToString()); rptMatchRank2.DataBind(); break;
                        case 2: rptMatchRank3.DataSource = gm.GetMatchRankDetail(row["ContestID"].ToString()); rptMatchRank3.DataBind(); break;
                        case 3: rptMatchRank4.DataSource = gm.GetMatchRankDetail(row["ContestID"].ToString()); rptMatchRank4.DataBind(); break;
                        case 4: rptMatchRank5.DataSource = gm.GetMatchRankDetail(row["ContestID"].ToString()); rptMatchRank5.DataBind(); break;
                        default: break;
                    }
                    i++;
                }

            }
            
        }
        //
        //public void BindGameList()
        //{
        //    Game game = new Game();
        //    rptGame1.DataSource = game.GetGameInfoList(0, 1);
        //    rptGame1.DataBind();
        //    rptGame2.DataSource = game.GetGameInfoList(0, 2);
        //    rptGame2.DataBind();
        //    rptGame3.DataSource = game.GetGameInfoList(0, 3);
        //    rptGame3.DataBind();
        //}
 
 

	 

	 
    //��������Ϸ�б�
    //public void BindHotGame()
    //{
    //    rpHotGameList.DataSource = new BLL.Game().List(5, 1).DefaultView;
    //    rpHotGameList.DataBind();
    //}

    //public void BindAllGame()
    //{
    //    rpAllGameList.DataSource = new BLL.Game().List(16, 0).DefaultView;
    //    rpAllGameList.DataBind();
    //}
    /*
    /// <summary>
    /// ������ʻ�����
    /// </summary>
    private void BindTopFollows()
    {
        DataTable dt = DbSession.Default.FromSql("select TOP 7 * from Web_vFaciSum order by fascisum desc").ToDataTable();
        if (dt.Rows.Count > 0)
        {
            rptFollowsTop.Visible = true;
            ltNonFollows.Visible = false;
            rptFollowsTop.DataSource = dt;
            rptFollowsTop.DataBind();
        }
        else
        {
            rptFollowsTop.Visible = false;
            ltNonFollows.Visible = true;
        }
    }

    /// <summary>
    /// ��������Ŀ�б�
    /// </summary>
    private void BindNewsColumn()
    {
        //DataTable dt = ( new BLL.News() ).TypeList(4);
        //if( dt.Rows.Count > 0 )
        //{
        //    ltNonNewsColumn.Visible = false;
        //    rptNewsColumnList.Visible = true;
        //    rptNewsColumnList.DataSource = dt;
        //    rptNewsColumnList.DataBind();
        //}
        //else
        //{
        //    ltNonNewsColumn.Visible = true;
        //    rptNewsColumnList.Visible = false;
        //}
    }

 

    //�ҽ�����
    public void BindDuiJiangLog()
    {
        //rpDuiJingLog.DataSource = new BLL.Member().ListDuiJiangLog(6);
        //rpDuiJingLog.DataBind();
    }

  

    //�󶨽�Ʒ���� ���ʤ 2009-4-14
    public void BindJiangPin()
    {
        
        <asp:Repeater ID="rpJiangPin" runat="server" Visible="false">
            <ItemTemplate>
                <tr>
                    <td>
                        <a href="/DuiJiang.aspx?id=<%#Eval("Award_ID") %>">
                            <img src="<%#Eval("Award_Pic") %>" alt="<%#Eval("Award_Name") %>" onerror="this.src='/Images/NoPicture/no_pic.gif'" height="100" width="265" /></a></td>
                </tr>
            </ItemTemplate>
            <SeparatorTemplate>
                <tr>
                    <td>
                        &nbsp;</td>
                </tr>
            </SeparatorTemplate>
        </asp:Repeater>
         

        //DataTable dt = new BLL.JiangPin().List(3);
        //rpJiangPin.DataSource = dt.DefaultView;
        //rpJiangPin.DataBind();
    }

    //�󶨵������� ���ʤ 2009-4-13
    public void BindDaoJu()
    {
        //DataTable dt = new BLL.DaoJu().List( 4 );
        //rpDaoJu.DataSource = dt.DefaultView;
        //rpDaoJu.DataBind();
    }

    //���������� ���ʤ 2009-4-23
    public void BindFriend()
    {
        //rpFriend.DataSource = new BLL.FriendLink().List(0);
        //rpFriend.DataBind();
    }
        */
        //��ҲƸ�����
        public void BindCaifu()
        {
            DataTable dt = new BLL.Member().ListCaifu(10);
            rpCaifu.DataSource = dt.DefaultView;
            rpCaifu.DataBind();
        }
    //����Flash����JS���� ���ʤ 2009-4-12
    public string GetFlashJS(int width, int height, int adCount)
    {
        //:<<<<=�߶�ע��=>>>>:
        //ֻ֧��JPG��ʽ��ͼƬ�������GIF��������ʽ��ͼƬ�ļ�����չ��ֱ�Ӹĳ�JPGҲ����
        //�����վ���õĶ˿��г�ͻ��ʱ����FLASH����������ʾ��ֻ�л���
        string xmlPath = Server.MapPath("/Upload/xml/flash.xml");
        if (!File.Exists(xmlPath))
        {
            return "�뵽��վ��̨Flash��������Ӽ�¼";
        }

        DataSet ds = new DataSet();
        ds.ReadXml(xmlPath);
        DataTable dt =ds.Tables[0];
        if (dt.Rows.Count > 0)
        {
            //JS: 
            //<script>
            //var flash = new flashPlayer(311, 263, 0);
            //flash.add('ͼƬ·��', '���ӵ�ַ', 'Title'); 
            //flash.play();
            //</script>
            StringBuilder sb = new StringBuilder();
            sb.Append("<script type=\"text/javascript\">");
            sb.Append("\n<!--");
            sb.Append("\nvar flash = new flashPlayer(" + width + ", " + height + ", 0);");
            for (int i = 0; i < dt.Rows.Count; i++)
            {
                sb.Append("\nflash.add('" + dt.Rows[i]["item_url"] + "', '" + dt.Rows[i]["link"] + "', '" + dt.Rows[i]["itemtitle"] + "');");
            }

            sb.Append("\nflash.play();");
            sb.Append("\n//-->");
            sb.Append("\n</script>");

            return sb.ToString();
        }

        return "�뵽��վ��̨Flash��������Ӽ�¼";
    }
	}
}
