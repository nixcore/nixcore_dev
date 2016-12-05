// http://www.onlineaspect.com/2009/06/10/reading-get-variables-with-javascript/
function getQueryVariable(variable) { 
var query = window.location.search.substring(1); 
var mainvars = query.split("&"); 
for (var x = 0; x< mainvars.length; x++)
{ 
var vars = mainvars[x].split("?");
for (var i = 0; i < vars.length; i++)
{ 
    var pair = vars[i].split("="); 
    if (pair[0] == variable)
    { 
        return unescape(pair[1]); 
    } 
} 
}
return false; 
}
// http://www.w3schools.com/ajax/ajax_xmlhttprequest_send.asp
function ajaxcgi_single(P,N,V)
{
var x;
if (window.XMLHttpRequest)
{
    x = new XMLHttpRequest();
} else {
    x = new ActiveXObject("Microsoft.XMLHTTP");
}
x.open("POST", "cgi-bin/"+P, true);
x.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
x.send(N+"="+V);
return; 
}
function ajaxcgi_multi(P,Q)
{
var x;
if (window.XMLHttpRequest)
{
    x = new XMLHttpRequest();
} else {
    x = new ActiveXObject("Microsoft.XMLHTTP");
}
x.open("POST", "cgi-bin/"+P, true);
x.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
x.send(Q);
return; 
}
