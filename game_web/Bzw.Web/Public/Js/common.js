function menu_over(num, nowp) {

    for (i = 1; i < 10; i++) {
        if (document.getElementById('m' + i) != null) {
            document.getElementById('m' + i).src = "/Images/top_menu0" + i + "off.jpg";
        }
    }
    document.getElementById('m' + num).src = "/Images/top_menu0" + num + "on.jpg";
    if (nowp != 0) {
        document.getElementById('m' + nowp).src = "/Images/top_menu0" + nowp + "on.jpg";
    }


}

function menu_over1(num, nowp) {
    for (i = 1; i < 10; i++) {
        if (document.getElementById('l' + i) != null) {
            document.getElementById('l' + i).src = "/Images/sub01/left_btn0" + i + "off.jpg";
        }
    }

    document.getElementById('l' + num).src = "/Images/sub01/left_btn0" + num + "on.jpg";
    if (nowp != 0) {
        document.getElementById('m' + nowp).src = "/Images/top_menu0" + nowp + "on.jpg";
    }

}
function menu_out(now, nowp) {
    for (i = 1; i < 10; i++) {
        if (document.getElementById('m' + i) != null) {
            document.getElementById('m' + i).src = "/Images/top_menu0" + i + "off.jpg";
        }
    }
    if (nowp != 0) {
        document.getElementById('m' + nowp).src = "/Images/top_menu0" + nowp + "on.jpg";
    }
}


//���coolie ��ֵ
function cookie(name) {
    var cookieArray = document.cookie.split("; "); //�õ��ָ��cookie��ֵ��    
    var cookie = new Object();
    for (var i = 0; i < cookieArray.length; i++) {
        var arr = cookieArray[i].split("=");       //������ֵ�ֿ�    
        if (arr[0] == name) return unescape(arr[1]); //�����ָ����cookie���򷵻�����ֵ    
    }
    return "";

}



function delCookie(name)//ɾ��cookie
{
    document.cookie = name + "=;expires=" + (new Date(0)).toGMTString();

}



function getCookie(objName) {//��ȡָ�����Ƶ�cookie��ֵ
    var arrStr = document.cookie.split("; ");
    for (var i = 0; i < arrStr.length; i++) {
        var temp = arrStr[i].split("=");
        if (temp[0] == objName) return unescape(temp[1]);
    }

}



function addCookie(objName, objValue, objHours) {      //���cookie

    var str = objName + "=" + escape(objValue);
    if (objHours > 0) {                               //Ϊʱ���趨����ʱ�䣬������ر�ʱcookie�Զ���ʧ
        var date = new Date();
        var ms = objHours * 3600 * 1000;
        date.setTime(date.getTime() + ms);
        str += "; expires=" + date.toGMTString();
    }
    document.cookie = str;
}



function SetCookie(name, value)//����������һ����cookie�����ӣ�һ����ֵ
{
    var Days = 30; //�� cookie �������� 30 ��
    var exp = new Date();    //new Date("December 31, 9998");
    exp.setTime(exp.getTime() + Days * 24 * 60 * 60 * 1000);

    document.cookie = name + "=" + escape(value) + ";expires=" + exp.toGMTString();

}

function getCookie(name)//ȡcookies����        
{
    var arr = document.cookie.match(new RegExp("(^| )" + name + "=([^;]*)(;|$)"));
    if (arr != null) return unescape(arr[2]); return null;
}

function delCookie(name)//ɾ��cookie
{
    var exp = new Date();
    exp.setTime(exp.getTime() - 1);
    var cval = getCookie(name);
    if (cval != null) document.cookie = name + "=" + cval + ";expires=" + exp.toGMTString();

}

function QQHit(qq) {
    //window.open('http://crm2.qq.com/page/portalpage/wpa.php?uin=4006600669&cref=http://b.qq.com/eim/home.html&ref=&pt=%E4%BC%81%E4%B8%9AQQ%E5%8A%9E%E5%85%AC%E7%89%88_%E4%BC%81%E4%B8%9Aqq&f=1&ty=1&ap=&as=&aty=0&a=&dm=qq.com&sv=4&sp=');
    window.open('http://wpa.qq.com/msgrd?v=3&uin=' + qq + '&site=qq&menu=yes', '_blank', 'height=544, width=644,toolbar=no,scrollbars=no,menubar=no,status=no');
} 

//����չ��������
$(function() {
    $('#qqfr').click(function() {   //չ��

        $('#qqfl').show();
        $('#qqfl').css({ left: '0' });
        $('#qqfr').hide();
        SetCookie("qqDialog", "1");

    });

    $('#qqcls').click(function() {   //�ر�
        $('#qqfl').hide();
        $('#qq').css({ "left": "-167px;" });
        // $('#qqfl').css({left:'-135px'});
        $('#qqfr').show();
        SetCookie("qqDialog", "0");
    });

})

if (getCookie("qqDialog") != null) {
    if (getCookie("qqDialog") == "1") {
        $("#qqfl").show();
        $('#qqfr').hide();
    }
    else {
        $("#qqfl").hide();
        $('#qqfr').show();
    }

}