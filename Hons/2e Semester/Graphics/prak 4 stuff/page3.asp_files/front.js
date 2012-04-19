var itxturl='http://gamedev.us.intellitxt.com/v3/door.jsp?ts='+(new Date()).getTime()+'&ipid=1966&fg=66AA77&mk=4&refurl='+document.location.href.replace(/\&/g,'%26').replace(/\'/g, '%27').replace(/\"/g, '%22');
try {
document.write('<s'+'cript language="javascript" src="'+itxturl+'"></s'+'cript>');
}catch(e){}
