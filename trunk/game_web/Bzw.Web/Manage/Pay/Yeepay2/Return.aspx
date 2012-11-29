<%@ Page Language="C#" AutoEventWireup="true" Inherits="Bzw.Inhersits.Manage.Pay.Yeepay2.Manage_Pay_Yeepay2_Return" %>

<%@ Register TagName="Control" TagPrefix="WebTop" Src="~/Public/WebTop.ascx" %>
<%@ Register TagName="Control" TagPrefix="WebFooter" Src="~/Public/WebFooter.ascx" %>
<%@ Register TagName="Control" TagPrefix="WebService" Src="~/Public/WebLeft.ascx" %>
<%@ Register TagName="Control" TagPrefix="WebUserLogin" Src="~/Public/WebUserLogin.ascx" %>

<%@ Register TagName="WebShortCutKey" TagPrefix="BZW" Src="~/Public/WebShortCutKey.ascx" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head id="Head1" runat="server">
    <meta http-equiv="Content-Type" content="text/html; charset=gb2312" />
    <title>
      
    </title>
  
    <link href="/css/layout.css" rel="stylesheet" type="text/css" />

    <script type="text/javascript" src="/Public/Js/common.js"></script>

    <script type="text/javascript" src="/Public/Js/Global.js"></script>

    <script src="/Public/Js/jquery.js" type="text/javascript"></script>

    <script src="/Public/Js/public.js" type="text/javascript"></script>

</head>
<body>
    <div id="container">
        <!--header-->
        <WebTop:Control ID="webTopOne" runat="server" pageType="4" />
        <!--content-->
 
            <div id="left">
                <BZW:WebShortCutKey ID="webshortcutkey1" runat="server" />
                <WebService:Control ID="webservice1" runat="server" />
            </div>
             <div id="content01">
            <div id="title01">
                <h3 class="fl">
                    ����֧�����</h3>
                <p class="dqwz">
                    ��ǰλ�ã���ҳ > �˻���ֵ</p>
            </div>
            <div id="sub_nr">
                <div class="cz">
            
       
                        <form id="form2" runat="server">
                       
                            <div style="text-align: center">
                                <table border="0" cellpadding="2" cellspacing="0">
                                    <tr>
                                        <th align="right">
                                            ֧�������
                                        </th>
                                        <td align="left">
                                            <%=PayResult %>
                                            <asp:Literal ID="Lab_msg" runat="Server" />
                                        </td>
                                    </tr>
                                    <tr>
                                        <th align="right">
                                            �����ţ�
                                        </th>
                                        <td align="left">
                                            <%=PayOrderID %>
                                        </td>
                                    </tr>
                                    <tr>
                                        <th align="right">
                                            ��ֵ��
                                        </th>
                                        <td align="left">
                                            <%=PayMoney %>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td>
                                            &nbsp;
                                        </td>
                                    </tr>
                                    <tr>
                                        <td colspan="2" align="center">
                                            <a href="/Manage/" title="���ػ�Ա����">���ػ�Ա����</a>
                                        </td>
                                    </tr>
                                </table>
                            </div>
                       
                        </form>
                        <div id="warning">
                            ��һ��Ҫ��ȷ����д��ֵ����Ϸ�ʺţ��û���������������ɵĸ�����ʧ������Լ��е����Ρ�</div>
                        <div class="clear">
                        </div>
                    </div>
                    
                </div> <p>
                <img src="/Images/cont_img01.jpg" alt="" /></p>
                
            </div>
            <!--right End-->
        
        <!--content End-->
 
    <!--footer-->
    <WebFooter:Control ID="webFooterone" runat="server" />
</body>
</html>
