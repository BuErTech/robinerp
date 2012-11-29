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
using BCST.Common;
using Bzw.Data;
using Bzw.Entities;
using System.IO;
using UiCommon;
using System.Collections.Generic;


namespace Bzw.Inhersits.Manage
{
    public partial class Manage_UsersForm : UiCommon.BasePage
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            if (!CommonManager.Web.CheckPostSource())
                CommonManager.Web.ResponseEnd("/", true);
            string param = CommonManager.Web.Request("params", "");
            if (string.IsNullOrEmpty(param))
                return;

            switch (param)
            {
                case "addpic":
                    AddPic();
                    break;
                case "delalbumcover":
                    DelAlbumCover();
                    break;
                case "setalbumpublic":
                    SetAlbumPublic();
                    break;
                case "delpic":
                    DelPic();
                    break;
                case "delalbumcolumn":
                    DelAlbumColumn();
                    break;
                case "addalbumcolumn":
                    AddAlbumColumn();
                    break;
                case "setalbumtop":
                    SetAlbumTop();
                    break;
                case "settocover":
                    SetToCover();
                    break;
                default:
                    CommonManager.Web.ResponseEnd("/", true);
                    break;
            }
        }

        #region ����ĳ����ƬΪ������
        /// <summary>
        /// ����ĳ����ƬΪ������
        /// </summary>
        private void SetToCover()
        {
            if (!UserLoginInfo.IsLogin)
            {
                Response.Write("���ȵ�¼��");
                return;
            }
            TUsers user = DbSession.Default.Get<TUsers>(TUsers._.UserID == UserLoginInfo.UserID);
            if (user == null)
            {
                Response.Write("���ȵ�¼��");
                return;
            }

            string aid = CommonManager.Web.Request("id", "");

            if (string.IsNullOrEmpty(aid) || !CommonManager.String.IsInteger(aid))
            {
                Response.Write("�벻Ҫ���ԷǷ�������");
                return;
            }
            Web_UserAlbum pic = DbSession.Default.Get<Web_UserAlbum>(Web_UserAlbum._.ID == aid && Web_UserAlbum._.UserID == user.UserID);
            if (pic == null)
            {
                Response.Write("�벻Ҫ���ԷǷ�������");
                return;
            }

            Web_UserAlbumColumn column = DbSession.Default.Get<Web_UserAlbumColumn>(Web_UserAlbumColumn._.ID == pic.ColumnID);
            column.Cover = pic.PicPath;
            column.Attach();
            DbSession.Default.Save<Web_UserAlbumColumn>(column);
            Response.Write("success");
        }
        #endregion

        #region ����ĳ����������������
        /// <summary>
        /// ����ĳ����������������
        /// </summary>
        private void SetAlbumTop()
        {
            if (!UserLoginInfo.IsLogin)
            {
                Response.Write("���ȵ�¼��");
                return;
            }
            TUsers user = DbSession.Default.Get<TUsers>(TUsers._.UserID == UserLoginInfo.UserID);
            if (user == null)
            {
                Response.Write("���ȵ�¼��");
                return;
            }

            string aid = CommonManager.Web.Request("id", "");

            if (string.IsNullOrEmpty(aid) || !CommonManager.String.IsInteger(aid))
            {
                Response.Write("�벻Ҫ���ԷǷ�������");
                return;
            }
            Web_UserAlbumColumn column = DbSession.Default.Get<Web_UserAlbumColumn>(Web_UserAlbumColumn._.ID == aid && Web_UserAlbumColumn._.UserID == user.UserID);
            if (column == null)
            {
                Response.Write("�벻Ҫ���ԷǷ�������");
                return;
            }

            int max = (int)DbSession.Default.Max<Web_UserAlbumColumn>(Web_UserAlbumColumn._.OrderID, Web_UserAlbumColumn._.UserID == UserLoginInfo.UserID);
            column.OrderID = max + 1;
            column.Attach();
            DbSession.Default.Save<Web_UserAlbumColumn>(column);
            Response.Write("success");
        }
        #endregion

        #region ��ӻ��޸�һ����Ƭ
        /// <summary>
        /// ��ӻ��޸�һ����Ƭ
        /// </summary>
        private void AddPic()
        {
            if (!UserLoginInfo.IsLogin)
            {
                Response.Write("����δ��¼��<a href='javascript:history.back();'>��˷�����һҳ</a>");
                return;
            }

            TUsers model = DbSession.Default.Get<TUsers>(TUsers._.UserName == UserLoginInfo.UserName);

            if (model == null)
            {
                Response.Write("����δ��¼��<a href='javascript:history.back();'>��˷�����һҳ</a>");
                return;
            }
            //��ȡ��ֵ
            string lcover = "";

            //�ж��Ƿ������
            string columnid = CommonManager.Web.RequestForm("selColumnId", "");
            if (string.IsNullOrEmpty(columnid) || !CommonManager.String.IsInteger(columnid))
            {
                Response.Write("û����ᣬ���ȴ�����ᡣ<a href='/Manage/AlbumWrite.aspx'>����������</a>");
                return;
            }

            if (Request.Files.Count > 0)
            {
                try
                {
                    if (!CommonManager.File.UploadFile(Request.Files["filePicPath"], 100 * 1024, new string[] { "jpg", "jpeg", "gif", "bmp", "png" }, "/Upload/Photo/" + model.UserID.ToString(), FileOperator.RenameType.��ˮ��, out lcover))
                    {
                        lcover = "";
                    }
                }
                catch (Exception ex)
                {
                    Response.Write("�ļ��ϴ�����" + ex.Message + "<a href='javascript:history.back();'>��˷�����һҳ</a>");
                    return;
                }
            }
            string pname = Server.HtmlEncode(CommonManager.Web.RequestForm("txtPicName", ""));
            string pdes = Server.HtmlEncode(CommonManager.Web.RequestForm("txtPicDescription", ""));
            string shortnum = CommonManager.Web.RequestForm("txtShortNum", "1");
            string id = CommonManager.Web.Request("id", "");

            //У��
            if (string.IsNullOrEmpty(id) || !CommonManager.String.IsInteger(id))
            {
                if (string.IsNullOrEmpty(lcover))
                {
                    Response.Write("���ϴ�������Ƭ��<a href='javascript:history.back();'>��˷�����һҳ</a>");
                    return;
                }
            }
            if (string.IsNullOrEmpty(pname) && !string.IsNullOrEmpty(lcover))
            {
                pname = lcover.Substring(lcover.LastIndexOf('/') + 1);
            }
            //�������ݿ�
            Web_UserAlbum pic;
            if (string.IsNullOrEmpty(id) || !CommonManager.String.IsInteger(id))
            {
                pic = new Web_UserAlbum();
                pic.InputTime = DateTime.Now;
                pic.UserID = model.UserID;
                pic.Enable = Library.EnableState.δ����.ToString();
            }
            else
            {
                pic = DbSession.Default.Get<Web_UserAlbum>(Web_UserAlbum._.ID == id);
                pic.Attach();
            }
            pic.ColumnID = int.Parse(columnid);
            pic.PicDescription = pdes;
            pic.PicName = pname;
            pic.PicPath = string.IsNullOrEmpty(pic.PicPath) ? lcover : pic.PicPath;
            pic.ShortNum = (string.IsNullOrEmpty(shortnum) || !CommonManager.String.IsInteger(shortnum)) ? 1 : int.Parse(shortnum);

            DbSession.Default.Save<Web_UserAlbum>(pic);
            //����
            Response.Write("<script>location.href='PicList.aspx';</script>");
        }
        #endregion

        #region ɾ��һ��������
        /// <summary>
        /// ɾ��һ��������
        /// </summary>
        private void DelAlbumCover()
        {

            if (!UserLoginInfo.IsLogin)
            {
                Response.Write("����δ��¼��");
                return;
            }

            TUsers model = DbSession.Default.Get<TUsers>(TUsers._.UserName == UserLoginInfo.UserName);

            if (model == null)
            {
                Response.Write("����δ��¼��");
                return;
            }

            string tmp = CommonManager.Web.Request("id", "");
            if (string.IsNullOrEmpty(tmp) || !CommonManager.String.IsInteger(tmp) || !DbSession.Default.Exists<Web_UserAlbumColumn>(Web_UserAlbumColumn._.ID == tmp))
            {
                Response.Write("�Ƿ�������");
                return;
            }

            Web_UserAlbumColumn album = DbSession.Default.Get<Web_UserAlbumColumn>(Web_UserAlbumColumn._.ID == tmp);
            if (album == null)
            {
                Response.Write("�Ƿ�������");
                return;
            }

            CommonManager.File.DeleteFile(album.Cover);

            DbSession.Default.Update<Web_UserAlbumColumn>(Web_UserAlbumColumn._.Cover, "", Web_UserAlbumColumn._.ID == tmp);

            Response.Write("success");
        }
        #endregion

        #region ��ӻ��޸�һ�����
        /// <summary>
        /// ��ӻ��޸�һ�����
        /// </summary>
        private void AddAlbumColumn()
        {

            if (!UserLoginInfo.IsLogin)
            {
                Response.Write("����δ��¼��<a href='javascript:history.back();'>��˷�����һҳ</a>");
                return;
            }

            TUsers model = DbSession.Default.Get<TUsers>(TUsers._.UserName == UserLoginInfo.UserName);

            if (model == null)
            {
                Response.Write("����δ��¼��<a href='javascript:history.back();'>��˷�����һҳ</a>");
                return;
            }

            //��ȡ������
            string name = CommonManager.Web.RequestForm("txtAlbumName", "");
            string ucover = CommonManager.Web.RequestForm("txtCover", "");

            string lcover = "";
            if (Request.Files.Count > 0)
            {
                try
                {
                    if (!CommonManager.File.UploadFile(Request.Files["fileCover"], 100 * 1024, new string[] { "jpg", "jpeg", "gif", "bmp", "png" }, "/Upload/UserAlbum", FileOperator.RenameType.��ˮ��, out lcover))
                    {
                        lcover = "";
                    }
                }
                catch (Exception ex)
                {
                    Response.Write("�ļ��ϴ�����" + ex.Message + "<a href='javascript:history.back();'>��˷�����һҳ</a>");
                    return;
                }
            }

            string ispublic = CommonManager.Web.RequestForm("rdoIsPublic", Library.PublicState.����.ToString());
            //У��
            if (string.IsNullOrEmpty(name))
            {
                Response.Write("����д������ơ�<a href='javascript:history.back();'>��˷�����һҳ</a>");
                return;
            }
            //������ݿ�
            string cur = CommonManager.Web.Request("cur", "");
            Web_UserAlbumColumn album;
            if (string.IsNullOrEmpty(cur))
            {
                album = new Web_UserAlbumColumn();
                album.InputTime = DateTime.Now;
                album.UserID = model.UserID;
            }
            else
            {
                album = DbSession.Default.Get<Web_UserAlbumColumn>(Web_UserAlbumColumn._.ID == cur);
                album.Attach();
            }
            album.AlbumName = name;
            album.Cover = string.IsNullOrEmpty(album.Cover) ? (string.IsNullOrEmpty(lcover) ? ucover : lcover) : album.Cover;
            album.IsPublic = ispublic;
            DbSession.Default.Save<Web_UserAlbumColumn>(album);
            //������Ϣ
            Response.Write("<script>location.href='Album.aspx';</script>");
        }
        #endregion

        #region ɾ��һ�����
        /// <summary>
        /// ɾ��һ�����
        /// </summary>
        private void DelAlbumColumn()
        {
            string aid = CommonManager.Web.Request("id", "");
            #region У��

            if (!UserLoginInfo.IsLogin)
            {
                Response.Write("����δ��¼��");
                return;
            }

            TUsers model = DbSession.Default.Get<TUsers>(TUsers._.UserName == UserLoginInfo.UserName);

            if (model == null)
            {
                Response.Write("����δ��¼��");
                return;
            }

            if (string.IsNullOrEmpty(aid) || !CommonManager.String.IsInteger(aid))
            {
                Response.Write("�벻Ҫ���ԷǷ�������");
                return;
            }

            if (!DbSession.Default.Exists<Web_UserAlbumColumn>(Web_UserAlbumColumn._.ID == aid))
            {
                Response.Write("�벻Ҫ���ԷǷ�������");
                return;
            }
            #endregion

            //ת��
            //Web_UserAlbumColumn album = DbSession.Default.Get<Web_UserAlbumColumn>( Web_UserAlbumColumn._.ID == ( DbSession.Default.Max<Web_UserAlbumColumn>( Web_UserAlbumColumn._.ID, Web_UserAlbumColumn._.UserID == model.UserID ) ) );
            DataTable dt = DbSession.Default.From<Web_UserAlbumColumn>().Where(Web_UserAlbumColumn._.UserID == UserLoginInfo.UserID).ToDataTable();
            if (dt.Rows.Count > 1)
            {
                //����������ᣬ�򽫵�ǰ����µ���Ƭת��
                //DataRow[] rows = dt.Select( "ID=" + aid );
                //if( rows.Length <= 0 )
                //    return;
                //dt.Rows.Remove(rows[0]);
                string maxid = dt.Compute("max(ID)", "ID<>" + aid).ToString();
                DbSession.Default.Update<Web_UserAlbum>(Web_UserAlbum._.ColumnID, maxid, Web_UserAlbum._.ColumnID == aid && Web_UserAlbum._.UserID == UserLoginInfo.UserID);
            }
            else
            {
                //����������ˣ���ɾ��������������
                DbSession.Default.Update<Web_UserAlbumColumn>(Web_UserAlbumColumn._.AlbumName, "Ĭ�����", Web_UserAlbumColumn._.ID == aid);
                Response.Write("success");
                return;
            }

            DbSession.Default.Delete<Web_UserAlbumColumn>(Web_UserAlbumColumn._.ID == aid);

            Response.Write("success");
        }
        #endregion

        #region ɾ��һ����Ƭ
        /// <summary>
        /// ɾ��һ����Ƭ
        /// </summary>
        private void DelPic()
        {
            string aid = CommonManager.Web.Request("id", "");
            string type = CommonManager.Web.Request("type", "0");

            #region У��
            if (string.IsNullOrEmpty(aid) || !CommonManager.String.IsInteger(aid))
            {
                Response.Write("�벻Ҫ���ԷǷ�������");
                return;
            }

            if (!DbSession.Default.Exists<Web_UserAlbum>(Web_UserAlbum._.ID == aid))
            {
                Response.Write("�벻Ҫ���ԷǷ�������");
                return;
            }

            if (!UserLoginInfo.IsLogin)
            {
                Response.Write("����δ��¼��");
                return;
            }

            TUsers model = DbSession.Default.Get<TUsers>(TUsers._.UserName == UserLoginInfo.UserName);

            if (model == null)
            {
                Response.Write("����δ��¼��");
                return;
            }
            #endregion

            Web_UserAlbum album = DbSession.Default.Get<Web_UserAlbum>(Web_UserAlbum._.ID == aid);
            CommonManager.File.DeleteFile(album.PicPath);
            if (type == "1")
                DbSession.Default.Delete<Web_UserAlbum>(album);
            else if (type == "0")
                DbSession.Default.Update<Web_UserAlbum>(Web_UserAlbum._.PicPath, "", Web_UserAlbum._.ID == aid);

            Response.Write("success");
        }
        #endregion

        #region �����û����Ĺ���״̬
        /// <summary>
        /// �����û����Ĺ���״̬
        /// </summary>
        private void SetAlbumPublic()
        {
            string state = CommonManager.Web.Request("public", "");
            string aid = CommonManager.Web.Request("id", "");

            #region У��
            if (string.IsNullOrEmpty(state) || string.IsNullOrEmpty(aid) || !CommonManager.String.IsInteger(state) || !CommonManager.String.IsInteger(aid))
            {
                Response.Write("�벻Ҫ���ԷǷ�������");
                return;
            }

            if (!DbSession.Default.Exists<Web_UserAlbumColumn>(Web_UserAlbumColumn._.ID == aid))
            {
                Response.Write("�벻Ҫ���ԷǷ�������");
                return;
            }

            //string UserName = uc.GetUserName.ToString();
            if (!UserLoginInfo.IsLogin)
            {
                Response.Write("����δ��¼��");
                return;
            }

            TUsers model = DbSession.Default.Get<TUsers>(TUsers._.UserName == UserLoginInfo.UserName);

            if (model == null)
            {
                Response.Write("����δ��¼��");
                return;
            }

            if (!DbSession.Default.Exists<Web_UserAlbumColumn>(Web_UserAlbumColumn._.ID == aid && Web_UserAlbumColumn._.UserID == model.UserID))
            {
                Response.Write("�����Ƿ���");
                return;
            }
            #endregion

            DbSession.Default.Update<Web_UserAlbumColumn>(Web_UserAlbumColumn._.IsPublic, ((Library.PublicState)int.Parse(state)).ToString(), Web_UserAlbumColumn._.UserID == model.UserID && Web_UserAlbumColumn._.ID == aid);
            Response.Write("success");
        }
        #endregion
    }
}
