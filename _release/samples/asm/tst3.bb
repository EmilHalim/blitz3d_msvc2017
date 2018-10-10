Global msg$ = "welcome "
Global body$ = " here "
Global xx

Print msg  

AsmInc "tst2asm.txt"

Asm " mov [_vxx],eax   

Print xx 

Asm " mov eax , _vbody 
Asm " add eax, 4       
Asm " push eax         
Asm " call _strlen     
Asm " add esp,4        
Asm " mov [_vxx],eax   

Print xx 

yn$=Input$( "press Return key" )

End

AsmData 
"_vvar   .dd 0   
"        .align	4
AsmEndData

Ccode
"  int strlen(char* mystr) 
"  {
"      char* ch = mystr;
"      while(*ch!=0)ch++;   
"      return ch-mystr;
"  }  
CEndCode