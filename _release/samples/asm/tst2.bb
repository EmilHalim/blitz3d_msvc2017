Global msg$ = "welcome "

Global body$ = " here "

Print msg  

AsmInc "tst2asm.txt"

yn$=Input$( "press Return key" )

End

AsmData 
"_vVar   .db 0"
"   .align	4"
AsmEndData