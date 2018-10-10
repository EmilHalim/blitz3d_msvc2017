

Global msg$ = "welcome "

Global body$ = " here "

Print msg  

Asm " sub esp,8 
Asm " mov eax,_vbody 
Asm " mov [esp],eax 
Asm " call __bbStrLoad  
Asm " mov [esp],eax 
Asm " call _fprint 


yn$=Input$( "press Return key" )


End

