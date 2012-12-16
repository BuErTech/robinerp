<%@ Page Language="C#" AutoEventWireup="true" Inherits="Bzw.Inhersits.Manage.Manage_FineryID" %>

<%@ Register Assembly="Pager" Namespace="Wuqi.Webdiyer" TagPrefix="webdiyer" %>
<%@ Register TagName="Control" TagPrefix="WebTop" Src="~/Public/WebTop.ascx" %>
<%@ Register TagName="Control" TagPrefix="WebFooter" Src="~/Public/WebFooter.ascx" %>
<%@ Register TagName="Control" TagPrefix="WebService" Src="~/Public/WebLeft.ascx" %>
<%@ Register TagName="Control" TagPrefix="WebUserLogin" Src="~/Public/WebUserLogin.ascx" %>

<%@ Register TagName="WebShortCutKey" TagPrefix="BZW" Src="~/Public/WebShortCutKey.ascx" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<script type="text/javascript" src="/Public/Js/Public.js"></script>

<script type="text/javascript" src="/Public/Js/Global.js"></script>

<script src="/Public/Js/jquery.js" type="text/javascript"></script>

<head runat="server" id="Head1">
    <title> 
    </title>
  
    <link href="/css/layout.css" rel="stylesheet" type="text/css" />
    <link href="/css/lightbox.css" type="text/css" rel="stylesheet" />
    <script type="text/javascript" src="/Public/Js/jquery.js"></script>
    <script type="text/javascript" src="/Public/Js/Global.js"></script>

    <script type="text/javascript" src="/Public/Js/common.js"></script>

    <script type="text/javascript" src="/Public/Js/topbtn.js"></script>

    <script type="text/javascript" src="/Public/Js/submu.js"></script>

</head>
<body>
    <div id="container">
        <!--header-->
        <WebTop:Control ID="webTopOne" runat="server" pageType="3" />
        <!--content-->
        <div id="left">
            <BZW:WebShortCutKey ID="webshortcutkey1" runat="server" isDisplay="0" />
            <WebService:Control ID="webService" runat="server" />
        </div>
        <!-- content start -->
        <div id="content01">
            <div id="title01">
                <h3 class="fl">
                    ��ƷID</h3>
                <p class="dqwz">
                    ��ǰλ�ã���ҳ > ��ƷID</p>
            </div>
            <div id="sub_nr">
                <div id="idss">
                    <label>
                        <img src="/Images/id01.jpg" alt="" /></label>
                    <input type="text" size="50" class="in" name="seachid" id="seachid" />
                    <img src="/Images/cont_btn04.jpg" alt="" onclick="Seach()" />
                </div>
                <div id="tjlh">
                    <ul>
                        <asp:Repeater ID="FineryIDTop" runat="server">
                            <ItemTemplate>
                                <ul>
                                    <li><strong class="text05">ID:<a href='/Manage/FineryIDSale.aspx?id=<%#Eval("FineryID") %>'
                                        class="link02"><%#Eval("FineryID") %></a></strong><br />
                                        �۸�
                                        <%#UiCommon.StringConfig.AddZeros(Eval("Price")) %>��� </li>
                                </ul>
                            </ItemTemplate>
                        </asp:Repeater>
                        <asp:Label runat="server" Text="�����Ƽ�" ID="NoProTop" Visible="false"></asp:Label>
                    </ul>
                </div>
                <div id="gz">
                    <strong class="text05">����</strong>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a
                        href="/Manage/FineryID.aspx?type=all" id="all" class="link03">ȫ��</a> &nbsp;|&nbsp;
                    <a href="/Manage/FineryID.aspx?type=aabb" id="aabb" class="link03">AABB</a> &nbsp;|&nbsp;
                    <a href="/Manage/FineryID.aspx?type=aabbcc" id="aabbcc" class="link03">AABBCC</a>&nbsp;
                    | &nbsp;<a href="/Manage/FineryID.aspx?type=abab" id="abab" class="link03">ABAB</a>&nbsp;
                    |&nbsp; <a href="/Manage/FineryID.aspx?type=ababab" id="ababab" class="link03">ABABAB</a>&nbsp;
                    | &nbsp;<a href="/Manage/FineryID.aspx?type=abcabc" id="abcabc" class="link03">ABCABC</a>&nbsp;
                    | &nbsp;<a href="/Manage/FineryID.aspx?type=abgab" id="abgab" class="link03">ABGAB</a>&nbsp;
                    | &nbsp;<a href="/Manage/FineryID.aspx?type=3a" id="3a" class="link03">3A</a>&nbsp;
                    | &nbsp;<a href="/Manage/FineryID.aspx?type=4a" id="4a" class="link03">4A</a>&nbsp;
                    | &nbsp;<a href="/Manage/FineryID.aspx?type=5a" id="5a" class="link03">5A������</a>&nbsp;
                    | &nbsp;<a href="/Manage/FineryID.aspx?type=4sun" id="4sun" class="link03">4˳������</a>&nbsp;
                    | &nbsp;<a href="/Manage/FineryID.aspx?type=date" id="date" class="link03">����</a>
                </div>
                <div id="jplh">
                    <p>
                        <strong class="text05">��Ʒ����</strong> <strong>��������ҳ���Ϸ�����������������Ҫ�ҵĺ��룬Ҳ���Կ��������Ƽ��ľ�Ʒ���š�</strong></p>
                    <ul>
                        <asp:Repeater ID="FineryIDList" runat="server">
                            <ItemTemplate>
                                <li><strong class="text05">ID&nbsp;&nbsp;&nbsp;&nbsp;��<a href='/Manage/FineryIDSale.aspx?id=<%#Eval("FineryID") %>'
                                    class="link02"><%#Eval("FineryID") %></a></strong><br />
                                    �۸�<%# UiCommon.StringConfig.AddZeros(Eval("Price")) %><%=UiCommon.StringConfig.MoneyName %>
                                </li>
                            </ItemTemplate>
                        </asp:Repeater>
                        <asp:Label runat="server" Text="�ܱ�Ǹ���������������" ID="NoProList" Visible="False" ForeColor="Red"
                            Font-Bold="True"></asp:Label>
                    </ul>
                </div>
            </div>
            <div class="sabrosus">
                <webdiyer:AspNetPager ID="anpPageIndex" runat="server" AlwaysShow="True" CustomInfoHTML="��ǰΪ��%CurrentPageIndex%/%PageCount%ҳ��ÿҳ%PageSize%������%RecordCount%��"
                    FirstPageText="&lt;" LastPageText="&gt;" NextPageText="��һҳ" PrevPageText="��һҳ"
                    ShowPageIndexBox="Never" UrlPaging="True" PageIndexBoxType="DropDownList" SubmitButtonText="Go"
                    TextAfterPageIndexBox="ҳ" TextBeforePageIndexBox="ת��" NumericButtonCount="5"
                    CurrentPageButtonClass="current" ShowPrevNext="False" PageSize="15">
                </webdiyer:AspNetPager>
            </div>
            <p>
                <img src="/Images/cont_img01.jpg" alt="" /></p>
        </div>
    </div>
    <!-- content end -->
    <div class="clear">
    </div>
   
    <!--footer-->
    <WebFooter:Control ID="webFooterone" runat="server" />
</body>
</html>

<script type="text/javascript">
    var seachid = GetRequest("seachid", "");
    $("#seachid").val(seachid);
    var type = GetRequest("type", "");
    if (type == "" || type == null) {
        type = "all";
    }
    //document.getElementById(type).className = "men_numRulvis";
    $("#" + type).addClass("men_numRulvis")
    function Seach() {
        var url = "/Manage/FineryID.aspx";
        var seachid = $("#seachid").val().Trim();
        url = SetUrlParam(url, "seachid=" + seachid);
        LocationTo(url);
    }
</script>

