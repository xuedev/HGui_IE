
 function remove(){
	var _mask = document.getElementById('loading');    
    _mask.parentNode.removeChild(_mask);  
 }
 
 function init(){
	 var _html = "<div id='loading' style='position:absolute;left:0;width:100%;height:100%;top:0;background:#E0ECFF;opacity:0.8;filter:alpha(opacity=80);'>\   <div style='position:absolute;  cursor1:wait;left:40%;top:200px;width:auto;height:16px;padding:12px 5px 10px 30px;\    background:#fff;border:2px solid #ccc;color:#000;'>\���ڼ��أ���ȴ�...\</div></div>";    
	 document.write(_html); 

 }
  
 window.onload = function(){    
	
	//��ʽӦ��ʱ���ſ�����ע��
	//remove();

	//����Ч������ʽӦ��ʱ����������
    setTimeout("remove();",2000);
 }        
 
init();