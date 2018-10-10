// *************************************************
//            User Defined Constants
// *************************************************

 #define false 0
 #define true 1
 //#define NULL 0
// #define MAX_PATH 255


 #define MaxStk 4096
 #define c_DblQt 34
 int tmpLen,nLine,p,k,inAsm,inMasmProc=0,inData=0;
 char CurMasmProcName[255];
 char AsmBuf[255];
 int Asm;
 long fpos;
int nxtlab=0;   /* next avail label # */
int oprnd1=0;
extern int Assignment(int tok);

// *************************************************
//               Standard Prototypes
// *************************************************

int ProcessAsm(int mtok);
void ParseOp(int tok);

char buf[100];

void InitAsm()
{
//    PushSyntax2(tokEax, AddIdent("EAX"));

}

void doAsmOption()
{
            int tok = GetToken();
            if(tok != tokIdent && strcmp("PROLOGUE",TokenIdentName) && strcmp("EPILOGUE",TokenIdentName) )
                error("expecting , PROLOGUE/EPILOGUE");
            int prlg = TokenIdentName[0] == 'P';
            if(GetToken()!=':')
                error("expecting , ':'");
            tok = GetToken();
            if(strcmp("NONE",TokenIdentName)==0)
             {
               if(prlg)
                  PrologFlg = 0;
               else
                  EpilogFlg = 0;
             }
            else if(strcmp("PrologueDef",TokenIdentName)==0)
             {
               if(prlg)
                  PrologFlg = 1;
               else
                  EpilogFlg = 1;
             }
}

unsigned int dofrom() // returns number of bytes read from FROM file
{
    FILE* filehandle;
	int k,ch,cnt=0;
    SkipSpace(0);
    if (*CharQueue != '"')
    {
        error(" file name string expected");
    }
	GetString('"', '#');
    if ((filehandle = fopen(TokenValueString, "rb")) == NULL)
	{
		error("Cannot open output file \"%s\"\n", TokenValueString);
	}

    while ((ch = fgetc(filehandle)) != EOF)
    {
       printf2("\n  DB 0x%x",ch);
       cnt++;
       k=15;
       while (k>0 && (ch = fgetc(filehandle)) != EOF)
       {
           printf2(",0x%x",ch);
           cnt++;
           k--;
       }
    }
    fclose(filehandle);
    return cnt;
}

unsigned int doExtract() // returns number of bytes read from FROM file
{
    FILE* filehandle;
	int tok,k,ch,cnt=0;
	int startPos,Size;
    SkipSpace(0);
    if (*CharQueue != '"')
    {
        error(" file name string expected");
    }
	GetString('"', '#');
    if(GetToken() != ',' ) error(" ',' expected");
    tok = GetToken();
    if(tok != tokNumInt) error(" int value expected");
    startPos = GetNumber();
    if(GetToken() == ',' )
    {
      tok = GetToken();
      if(tok != tokNumInt) error(" int value expected");
      Size = GetNumber();
    }
    else Size = 0;
    if ((filehandle = fopen(TokenValueString, "rb")) == NULL)
	{
		error("Cannot open output file \"%s\"\n", TokenValueString);
	}
    fseek( filehandle, Size, 0);
    while (cnt < Size  && (ch = fgetc(filehandle)) != EOF)
    {
       printf2("\n  DB 0x%x",ch);
       cnt++;
       k=15;
       while (cnt < Size  && k > 0 && (ch = fgetc(filehandle)) != EOF)
       {
           printf2(",0x%x",ch);
           cnt++;
           k--;
       }
    }
    fclose(filehandle);
    return cnt;
}

int SkipAsmComment()
{
   SkipSpace(0);
   if( CharQueue[0] == ';') { SkipLine(); return 1;}
   if( CharQueue[0] == '/' && CharQueue[1] == '/') { SkipLine(); return 1;}
   else if(CharQueue[0]==10)  return 1;
   return 0;
}

void doIndirect( int tok )
{
   while(tok != ']')
   {
	   if(tok == tokIdent) printf2("%s",TokenIdentName);
     else if(tok>=tokEax && tok<=tokEbp ) printf2(" %s ",TokenIdentName);
     else if(tok == '+') printf2(" + ");
     else if(tok == '-') printf2(" - ");
     else if(tok == '*') printf2(" * ");
     else if(tok == tokNumInt || tok == tokNumUint) printf2(" %d ",TokenValueInt);
     else error("bad asm token\n");
     tok = GetToken();
   }
  printf2("]");
}

void doIndirectExpr(int tok)
{
  char tmp[100];
  strcat(buf,"[");
  while(tok != ']')
   {
     if(tok == tokIdent) {sprintf(tmp,"_%s",TokenIdentName);strcat(buf,tmp);}
     else if(tok>=tokEax && tok<=tokEbp ) {sprintf(tmp," %s ",TokenIdentName);strcat(buf,tmp);}
     else if(tok == '+') strcat(buf," + ");
     else if(tok == '-') strcat(buf," - ");
     else if(tok == '*') strcat(buf," * ");
     else if(tok == tokNumInt || tok == tokNumUint) {sprintf(tmp," %d ",TokenValueInt);strcat(buf,tmp);}
     else error("bad asm token\n");
     tok = GetToken();
   }
  strcat(buf,"]");
  ParseOp(GetToken());
}


void doPtrIndirct(int flg)
{
   int mtok = GetToken();
   if(tokExt == tokAsmPTR) mtok = GetToken();
   if(mtok == '[')
       if(flg)
         doIndirectExpr(GetToken());
       else
         doIndirect(GetToken());
   else error(" Expecting PTR or [ here");
}

void doDwordExpr(int flg)
{
   if (flg)
       strcpy(buf,"dword ");
   else
       printf2("dword ptr [");
   doPtrIndirct(flg);
}

void doWordExpr(int flg)
{
   if (flg)
       strcpy(buf,"word ");
   else
       printf2("word ptr [");
   doPtrIndirct(flg);
}

void doByteExpr(int flg)
{
   if (flg)
       strcpy(buf,"byte ");
   else
       printf2("byte ptr [");
   doPtrIndirct(flg);
}

void doEqual(int tok)
{
   printf2(" mov  %s , ", buf);
   if(tok =='[')
    {
             printf2(" dword ptr [");
             doIndirect(GetToken());
    }
    else if(tok>=tokEax && tok <=tokEbp)
             printf2("%s",TokenIdentName);
    else if(tok>=tokAx && tok<=tokDi)
             printf2("%s",TokenIdentName);
    else if(tok>=tokAL && tok<=tokDH)
             printf2("%s",TokenIdentName);
    else if(tok==tokAsmKeyword)
      {
          switch( tokExt )
          {
              case  tokAsmDWORD:
                      doDwordExpr(false);
                      return;
              case  tokAsmWORD:
                      doWordExpr(false);
                      return;
              case  tokAsmBYTE:
                      doByteExpr(false);
                      return;
          }
      }

}

void doLShift(int tok)
{
   printf2(" shl %s , ", buf);
   if(tok == tokNumInt || tok == tokNumUint) printf2(" %d ",TokenValueInt & 0xFF);
   else if(tok == tokCL) printf2("CL");
   else error("bad asm token\n");
}

void doRShift(int tok)
{
   printf2(" shr %s , ", buf);
   if(tok == tokNumInt || tok == tokNumUint) printf2(" %d ",TokenValueInt & 0xFF);
   else if(tok == tokCL) printf2("CL");
   else error("bad asm token\n");
}

void doXchg(int tok)
{
    printf2(" xchg %s , %s\n", buf,TokenIdentName);
}

void ParseOp(int tok)
{
    switch (tok)
          {
             case '=':
                  doEqual(GetToken());
                  break;
             case tokLShift:
                  doLShift(GetToken());
                  break;
             case tokRShift:
                  doRShift(GetToken());
                  break;
             case tokXChng:
                  doXchg(GetToken());
                  break;
             case '(':
                  printf("function call\n");
                  break;
             case tokInc:
             case tokPostInc:
                  printf2(" inc %s \n", buf);
                  break;
             case tokDec:
             case tokPostSub:
                  printf2(" dec %s \n", buf);
                  break;
             case tokAssignAdd:
                  tok = GetToken();
				  if(tok == tokIdent)
                     printf2(" add %s , %s\n", buf,TokenIdentName);
				  else if(tok == tokNumInt || tok == tokNumUint)
					  printf2(" add %s , %d\n", buf,TokenValueInt);
                  break;
             case tokAssignSub:
                  tok = GetToken();
                  if(tok == tokIdent)
                     printf2(" sub %s , %s\n", buf,TokenIdentName);
				  else if(tok == tokNumInt || tok == tokNumUint)
					  printf2(" sub %s , %d\n", buf,TokenValueInt);
                  break;
             case tokAssignMul:
                  tok = GetToken();
                  printf2(" Mul %s , %s\n", buf,TokenIdentName);
                  break;
             case tokAssignDiv:
                  tok = GetToken();
                  printf2(" Div %s , %s\n", buf,TokenIdentName);
                  break;
             case tokAssignOr:
                  tok = GetToken();
                  printf2(" or %s , %s\n", buf,TokenIdentName);
                  break;
             case tokAssignAnd:
                  tok = GetToken();
                  printf2(" and %s , %s\n", buf,TokenIdentName);
                  break;
             case tokAssignXor:
                  tok = GetToken();
                  printf2(" Xor %s , %s\n", buf,TokenIdentName);
                  break;
              case '~': // Not
                  tok = GetToken();
                  printf2(" not %s , %s\n", buf,TokenIdentName);
                  break;
              case '-':  // Nigative
                  tok = GetToken();
                  printf2(" neg %s , %s\n", buf,TokenIdentName);
                  break;
              case '.':
                  tok = GetToken();
                  if (tok != tokStruct && tok != tokUnion && tok != tokTag) error("Pointer to or structure or union expected\n");
                  puts("using reg32 as a structre");
                  break;
          }

}

void doReg32Expr(int mtok)
{
    strcpy(buf,TokenIdentName);
	int tok = GetToken();
//	if (tok == '=') Assignment(tok);
//	else 
		ParseOp(tok);
}


void doOprnd1(int mtok,int mtokExt)
{
   oprnd1=mtok;
   // printf("%x %d %s\n",mtok,mtok,TokenIdentName);
   if(mtok>=tokEax && mtok<=tokEbp)
        printf2("%s",TokenIdentName);
   else if(mtok>=tokAx && mtok<=tokDi)
        printf2("%s",TokenIdentName);
   else if(mtok>=tokAL && mtok<=tokDH)
        printf2("%s",TokenIdentName);
/*
   else if( mtok == '[' )
   {
          printf2("dword [");
          doIndirect(GetToken());
   }
*/
  else if(mtok==tokAsmKeyword)
   {
      // printf("%x %d  %d %s\n",mtok,mtok,mtokExt,TokenIdentName);
       switch( tokExt )
        {
           case  tokAsmDWORD:
                      doDwordExpr(false);
                      return;
             case  tokAsmWORD:
                      doWordExpr(false);
                      return;
             case  tokAsmBYTE:
                      doByteExpr(false);
                      return;
        }
   }
   else if(mtok == tokIdent)
    {
		 printf2("dword ptr [%s]",TokenIdentName);
    }
    else if(mtok == '*' &&  mtokExt == tokAsmMOV)
    {
		 mtok = GetToken();
         if(mtok != tokIdent) error("Identfire expecting here");
		 printf2("dword ptr [dword ptr[%s]]",TokenIdentName);
    }
   else
    {
       //printf("%x %d %s\n",mtok,mtok,TokenIdentName);
       //error("bad first operand");
	   printf2("%s",GetTokenName(mtok));
	   while(*CharQueue != ',')
        {
          fputc(*CharQueue, OutFile);
          ShiftCharN(1);
        }
    }
}

void doOprnd2(int mtok,int mtokExt)
{
   int gotUnary, synPtr,  constExpr, exprVal;
   if(mtok>=tokEax && mtok<=tokEbp)
        printf2("%s",TokenIdentName);
   else if(mtok>=tokAx && mtok<=tokDi)
        printf2("%s",TokenIdentName);
   else if(mtok>=tokAL && mtok<=tokDH)
        printf2("%s",TokenIdentName);
/*
   else if( mtok == '[' )
   {
          printf2("DWORD [");
          doIndirect(GetToken());
   }
*/
   else if(mtok==tokAsmKeyword)
   {
      // printf("%x %d  %d %s\n",mtok,mtok,mtokExt,TokenIdentName);
       switch( tokExt )
        {
            case  tokAsmDWORD:
                      doDwordExpr(false);
                      return;
             case  tokAsmWORD:
                      doWordExpr(false);
                      return;
             case  tokAsmBYTE:
                      doByteExpr(false);
                      return;
             case  tokAsmOFFSET:
             case  tokAsmADDR:
                   mtok = GetToken();
                   if(mtok != tokIdent) error("Identfire expecting here");
                   //doOprnd2(mtok,mtokExt);
				   printf2(" offset %s",TokenIdentName);
                   return;
        }
   }
   else if(mtok == tokIdent)
   {
          if(mtokExt == tokAsmLEA)
			  printf2("[%s]",TokenIdentName);
          else  if(mtokExt == tokAsmCALL)
			   printf2("%s",TokenIdentName);
		  else
			  printf2("dword ptr [%s]",TokenIdentName);
   }
   else if(mtok == '*' &&  mtokExt == tokAsmPUSH)
    {
		mtok = GetToken();
       if(mtok != tokIdent) error("Identfire expecting here");
	   fseek(OutFile,-5,SEEK_CUR);
	   printf2("MOV EAX,dword ptr [%s]\n",TokenIdentName);
	   printf2("PUSH dword ptr [EAX]");
	   //printf2("dword[dword[%s]]",TokenIdentName);
	   return;
    }
   else if(mtok == '&' && (mtokExt == tokAsmMOV || mtokExt == tokAsmPUSH))
    {
       mtok = GetToken();
       if(mtok != tokIdent) error("Identfire expecting here");
       if(mtokExt == tokAsmPUSH)
	        printf2("%s",TokenIdentName);
	   else
	    {
		   printf2("%s",TokenIdentName);
	    }
	    return;
    }
   else if(mtok == tokNumInt || mtok == tokNumUint) printf2(" %d ",TokenValueInt);
   else if (mtok == tokLitStr)
   {
      int lb = LabelCnt++;
	  printf2("offset  ml%d",lb);
      char** oldHeaderFooter = CurHeaderFooter;
      if (oldHeaderFooter)  puts2(oldHeaderFooter[1]);
	  CurHeaderFooter =  DataHeaderFooter;
      puts2(CurHeaderFooter[0]);
	  GenStaticNumLabel(lb);
      GetString('"', 'D');
	  puts2("db 0\n");
	  puts2(CurHeaderFooter[1]);
      if (oldHeaderFooter)  puts2(oldHeaderFooter[0]);
      CurHeaderFooter = oldHeaderFooter;
	  return;

   }

   else if(mtok == '{' )
   {
	   mtok = ParseExpr(GetToken(), &gotUnary, &synPtr, &constExpr, &exprVal, '}', 0); 
       
       if (gotUnary)
         {
			 //fseek(OutFile,-5,SEEK_CUR);
			 fseek(OutFile,fpos,SEEK_SET);
			 static char inBuf[1024];
			 inBuf[0]=0;
             fgets(inBuf, 1048576,OutFile);
             if(inBuf[strlen(inBuf)-1]==10)inBuf[strlen(inBuf)-1]=0;
			 fseek(OutFile,fpos,SEEK_SET);
			 GenExpr();
			// if(mtokExt == tokAsmPUSH) puts2("PUSH  EAX\n");
			// if(mtokExt == tokAsmMOV)
			
			 if(oprnd1!= tokEax) printf2("%s EAX\n",inBuf);
	     }
   }

   else
     //error("bad second operand");
     //printf2("\n");
	 printf2("%s",GetTokenName(mtok));
	 while(*CharQueue != 10)
      {
          fputc(*CharQueue, OutFile);
          ShiftCharN(1);
      }
     fputc('\n', OutFile);
  oprnd1=0;
}


void doMovExpr()
{
again:
   fpos = ftell( OutFile );
   printf2("  MOV ");
   doOprnd1(GetToken(),tokAsmMOV);
   if(GetToken() != ',') error("expecting ',' token");
   printf2(" , ");
   doOprnd2(GetToken(),tokAsmMOV);
   printf2("\n");
   if(SkipAsmComment()) return;
   goto again;
}

void doLeaExpr()
{
again:
   fpos = ftell( OutFile );
   printf2("  LEA ");
   doOprnd1(GetToken(),tokAsmLEA);
   if(GetToken() != ',') error("expecting ',' token");
   printf2(" , ");
   doOprnd2(GetToken(),tokAsmLEA);
   printf2("\n");
   if(SkipAsmComment()) return;
   goto again;
}

void doPushExpr()
{
  int tok = GetToken();
again:
   fpos = ftell( OutFile );
   printf2("  PUSH  ");
   doOprnd2( tok ,tokAsmPUSH);
   printf2("\n");
   if(SkipAsmComment()) return;
   tok = GetToken();
   if(tok == ',' ) tok = GetToken();
   goto again;

}

void doPopExpr()
{
   int tok = GetToken();
  again:
   printf2("  POP ");
   doOprnd2(tok,tokAsmPOP);
   printf2("\n");
   if(SkipAsmComment()) return;
   tok = GetToken();
   if(tok == ',' ) tok = GetToken();
   goto again;

}

void doRPushExpr()
{
   int v,ndx,cnt=0;
   int mtok =  GetToken();
   while(mtok != ';')
   {
      // printf("%x\n",mtok);
       push2(mtok,AddIdent(TokenIdentName));
       cnt++;
       mtok =  GetToken();
   }
   ndx = 98;
   buf[99]=0;
   while( cnt != 0 )
   {
       mtok = pop2(&v);
       if(mtok==']')
        {
           buf[ndx--]=mtok;
           mtok = pop2(&v);
           if(mtok == tokIdent);
             {
                 char *p = IdentTable+stack[sp][1];
                 int   l = strlen(p);
                 while (l>0)
                 {
                     buf[ndx--]=p[l-1];
                     l--;
                 }
             }
            mtok = pop2(&v);
            buf[ndx]=mtok;
            if(_stricmp(IdentTable+stack[sp-1][1],"dword")==0) {pop2(&v);cnt--;buf[--ndx]='d';buf[--ndx]='r';buf[--ndx]='o';buf[--ndx]='w';buf[--ndx]='d'; }
            printf2("  push %s\n",&buf[ndx]);
            cnt-=3;
        }
       else
       {
          char *p = IdentTable+stack[sp][1];
          printf2("  push %s\n",p);
          cnt--;
       }
      ndx = 98;
   }
 // SkipAsmComment();
}

void doCallExpr()
{
   printf2("  CALL ");
   doOprnd2(GetToken(),tokAsmCALL);
   printf2("\n");
   if(SkipAsmComment()) return;
}

void EmitDword()
{
     printf2("  DD ");
     while(*CharQueue != 10)
      {
          fputc(*CharQueue, OutFile);
          ShiftCharN(1);
      }
     fputc('\n', OutFile);
}

void EmitWord()
{
     printf2("  DW ");
     while(*CharQueue != 10)
      {
          fputc(*CharQueue, OutFile);
          ShiftCharN(1);
      }
     fputc('\n', OutFile);
}

void EmitByte()
{
     printf2("  DB ");
     while(*CharQueue != 10)
      {
          fputc(*CharQueue, OutFile);
          ShiftCharN(1);
      }
     fputc('\n', OutFile);
}

void EmitQword()
{
     printf2("  DQ ");
     while(*CharQueue != 10)
      {
          fputc(*CharQueue, OutFile);
          ShiftCharN(1);
      }
     fputc('\n', OutFile);
}


void dogoto()
{
       int mtok;
         if ((mtok = GetToken()) == '@' )
          {
             ShiftCharN(1); //skip @
             if((*CharQueue|32) == 'b') printf2("    jmp LAB__%u\n",labalCnt);
             if((*CharQueue|32) == 'f') printf2("    jmp LAB__%u\n",labalCnt+1);
             ShiftCharN(1);
          }
        else
         {
            if (mtok  != tokIdent)
                errorUnexpectedToken(mtok);

               GenJumpUncond( AddGotoLabel(TokenIdentName, 0)  );
			 
         }

}


void doAddr()
{

}

void doOffset()
{

}

void doProto(char* name)
{
 
}

#define MaxReglistcnt 20
int reglist[MaxReglistcnt];
int reglistcnt=0;
/*
int doMasmProc(char* name)
{
   char tmp[255]={0};
   int callCon=0;
   buf[0]=0;
   strcpy(CurMasmProcName,name);
   int tok = GetToken();
   if(tok == tokIdent || tok == tokExtension || tok == tokUses || tok=='<')
   {
      if(tok == tokExtension && tokExt == tokExtStdcall)
       {
           callCon = tokExtStdcall;
           strcpy(buf,"  stdcall  ");
       }
      if(tok == tokIdent)
       {
          if(TokenIdentName[0]=='C' && TokenIdentName[1]==0)
           {
                callCon = tokExtCdecl;
                strcpy(buf,"  cdecl  ");
           }
          else if(strcmp(TokenIdentName,"SYSCALL"))
           {
                callCon = tokExtFastcall;
                strcpy(buf,"  fastcall  ");
           }
          else if(strcmp(TokenIdentName,"STDCALL"))
           {
                callCon = tokExtStdcall;
                strcpy(buf,"  stdcall  ");
           }
       }
      if(callCon) tok = GetToken();

      strcat(buf,name);
      strcat(buf,"(");

      if(tok == tokUses)
       {
          tok = GetToken();
          while(tok>=tokEax && tok<=tokEbp)
           {
              if(reglistcnt>=MaxReglistcnt)error("uses has exceeding MaxReglist");
              reglist[reglistcnt++]=tok;
              tok = GetToken();
           }
       }

      while(tok==tokIdent)  //error("expecting Ident param here");
       {
          strcpy(tmp,TokenIdentName);
          if(':'!=GetToken())error("expecting ':' here");
          tok = GetToken();
          if (!TokenStartsDeclaration(tok, 1))
                              errorUnexpectedToken(tok);
          strcat(buf,TokenIdentName);
          strcat(buf," ");
          strcat(buf,tmp);
          tok = GetToken();
          if(tok==',')
           {
             strcat(buf,",");
             tok = GetToken();
           }
       }

      if(tok=='<')
       {
          tok = GetToken();
          strcpy(tmp,TokenIdentName);
          if('>'!=GetToken())error("expecting '>' here");
          tok = GetToken();
       }
      else
       {
          strcpy(tmp,"void  ");
          tok = GetToken();
       }
   }
  else
   {
      strcpy(tmp,"void  ");
      strcat(buf,name);
      strcat(buf,"(");
   }
  strcat(buf,"){");

  strcat(tmp,buf);

  int len = strlen(tmp);
  Reserve4Expansion(tmp, len);
  memcpy(CharQueue , tmp, len);
  #ifndef NO_ANNOTATIONS
  GenStartCommentLine(); puts2(tmp);
  #endif
  tok = GetToken();
//  ParseLevel++;
//  tok = ParseDecl(tok, NULL, 0, 0);
//  ParseLevel--;
   for(int i=0; i<reglistcnt; i++)
    {
        printf2("  push %s \n",GetTokenName(reglist[i]));
    }
   return  tok;
}

*/

int doMasmProc(char* name)
{
    strcpy(CurMasmProcName,name);
       sp = 0;

          push2('(', SizeOfWord * 3);
          push2(tokLocalOfs, 8);
          push(',');
          push2(tokIdent, AddNumericIdent(50));
          push(',');
          AddIdent("EDX");
          push2(tokIdent, FindIdent("EDX"));
          push(',');

          push2(tokEsi,150);

        //  push(',');
          push2(tokUnaryStar,4);
          push2(tokIdent, FindIdent("EDX"));
          push(',');
          push2(tokNumUint, 10);
          push(',');
          push2(tokIdent, AddIdent("Emil"));
          push2(')', SizeOfWord * 3);

          GenExpr();

		  return 0;
}

void doRet()
{
   if(reglistcnt)
   {
      for(int i=reglistcnt-1; i>=0; i--)
      {
          printf2("  pop %s \n",GetTokenName(reglist[i]));
      }
   }
   if(inMasmProc&&EpilogFlg)GenFxnEpilog(0,0);
   printf2(" RET");
   SkipSpace(0);
   if(CharQueue[0]==10) { puts2(""); return; }
   int mtok = GetToken();
   if(mtok != tokNumInt )
     error("int or Unit value expecting");
   printf2(" %d \n",TokenValueInt);
}

void doEnd()
{
   SkipSpace(0);
   if(CharQueue[0]==10) { puts2(""); return; }
   int tok = GetToken();
   if(tok==tokIdent)
   {
       if(!strcmp(TokenIdentName,"start"))
       {
           printf2(" RET");
           return;
       }
   }
}

int doMasmLocal()
{
   char tmp[255];
   int tok = GetToken();
   if(tok!=tokIdent)error("error : expecting variable Ident here");
   while(tok==tokIdent)
    {
        strcpy(buf,TokenIdentName);
        if(':'!=GetToken())error("expecting ':' here");
        tok = GetToken();
        if (!TokenStartsDeclaration(tok, 0))
                              errorUnexpectedToken(tok);
        strcpy(tmp,TokenIdentName);
        strcat(tmp," ");
        strcat(tmp,buf);
        strcat(tmp,"; ");

        int len = strlen(tmp);
        Reserve4Expansion(tmp, len);
        memcpy(CharQueue , tmp, len);
        #ifndef NO_ANNOTATIONS
        GenStartCommentLine(); puts2(tmp);
        #endif
        tok = GetToken();
        ParseLevel++;
        tok = ParseDecl(tok, NULL, 0, 0);
        ParseLevel--;
        if(tok==',') tok = GetToken();
        //if(tok!=tokIdent)error("error : expecting variable Ident here");
    }
   return  tok;
}

int doInvoke()
{
   printf2(" invoke  ");
   while(*CharQueue != 10)
    {
       fputc(*CharQueue, OutFile);
       ShiftCharN(1);
    }
    fputc(*CharQueue, OutFile);
	return 0;
}

int doCInvoke()
{

   printf2(" cinvoke  ");
   while(*CharQueue != 10)
    {
       fputc(*CharQueue, OutFile);
       ShiftCharN(1);
    }
    fputc(*CharQueue, OutFile);
	return 0;
}

int doFasmProc()
{
 
   printf2(" proc  ");
   while(*CharQueue != 10)
    {
       fputc(*CharQueue, OutFile);
       ShiftCharN(1);
    }
    fputc(*CharQueue, OutFile);
	return 0;
}


void passline()
{
      char* p = CharQueue;
      while(*p != 10)
      {
          if(OutFile) fputc(*p, OutFile);
          ShiftCharN(1);
      }
	  fputc('\n', OutFile);
}

int ProcessAsm(int mtok)
{

      char ln[255],*p;
      int  Cnt=0;
      int ftok;

	  
      if(mtok == ';' ) { SkipLine();  return GetToken();}
      if(mtok == '!' ) { passline();   return GetToken();}

      if(mtok>=tokEax && mtok<=tokEbp)
       {
          doReg32Expr(mtok);
          return GetToken();
       }

      else if(mtok>=tokAx && mtok<=tokDi)
          puts("rigester 16\n");

      else if(mtok>=tokAL && mtok<=tokDH)
          puts("rigester 8\n");

      else if(mtok==tokChar || /*mtok==tokSChar || mtok==tokUChar ||*/
         mtok==tokShort || /*tokUShort ||*/ mtok==tokInt || /*tokUnsigned ||*/
         mtok==tokLong  )
          puts("--type--\n");

      else if(mtok==tokDouble || mtok==tokFloat)
          puts("floating type\n");

      else if(mtok=='[')
      {
          strcpy(buf,"dword ptr ");
          doIndirectExpr(GetToken());
          return GetToken();
      }

      else if(mtok=='.')
       {
           mtok = GetToken();
           switch(mtok)
           {
              case tokIf:

                   break;
              case tokElseIf:

                   break;
              case tokElse:

                   break;
              case tokEndIf:

                   break;
              case tokWhile:

                   break;
              case tokBreakw:

                   break;
              case tokEndw:

                   break;
              case tokFor:

                   break;
              case tokEndFor:

                   break;
              case tokRepeat:

                   break;
              case tokUntil:

                   break;
              case tokData:
                   puts2(RoDataHeaderFooter[0]);
                   inData=1;
                   return GetToken();
                   break;
              case tokCode:
                   puts2(CodeHeaderFooter[0]);
                   inData=0;
                   return GetToken();
                   break;
           }

       }

      else if(mtok==tokGosub)
          puts("gosub 16\n");

      else if(mtok==tokInvoke)
          {
             doInvoke();
             return GetToken();
          }
      else if(mtok==tokCInvoke)
         {
             doCInvoke();
             return GetToken();
          }
      else if(mtok==tokProc)
         {
             doFasmProc();
             return GetToken();
          }
      else if(mtok==tokGoto)
       {
         dogoto();
         return GetToken();
       }
      else if (mtok == tokMasmLocalLabal)
       {
           p = CharQueue;
           ShiftCharN(3);
           printf2("LAB__%u:\n",++labalCnt);
           return GetToken();
       }

      else if(mtok==':')
          puts("Dynamic function\n");


      else if(mtok==tokMasmLocal && inMasmProc==1)
      {
         return doMasmLocal();
      }
      else if(mtok==tokExtension)
      {
         if(tokExt == tokExtOption)
          {
             doAsmOption();
             return GetToken();
          }
      }
      else if(mtok==tokEnd)
      {
          doEnd();
          return GetToken();
      }
      else if(mtok==tokAsmKeyword)
      {

          switch( tokExt )
          {
             case  tokAsmCALL:
                      doCallExpr();
                      return GetToken();
             case  tokAsmLEA:
                      doLeaExpr();
                      return GetToken();
             case  tokAsm_dd:
                      EmitDword();
                      return GetToken();
             case  tokAsm_dw:
                      EmitWord();
                      return GetToken();
             case  tokAsm_db:
                      EmitByte();
                      return GetToken();
             case  tokAsm_dq:
                      EmitQword();
                      return GetToken();
             case  tokAsmRET:
                      doRet();
                      return GetToken();
             case  tokAsmMOV:
                      doMovExpr();
                      return GetToken();
             case  tokAsmPUSH:
                      doPushExpr();
                      return GetToken();
             case  tokAsmPOP:
                      doPopExpr();
                      return  GetToken();
             case  tokAsmDWORD:
                      doDwordExpr(true);
                      return GetToken();
             case  tokAsmWORD:
                      doWordExpr(true);
                      return GetToken();
             case  tokAsmBYTE:
                      doByteExpr(true);
                      return GetToken();
             case  tokAsmJMP:
                      dogoto();
                      return GetToken();
             case  tokAsmRPUSH:
                      doRPushExpr();
                      return GetToken();
             case  tokAsmFROM:
                      dofrom();
                      return GetToken();
             case  tokAsmEXTRACT:
                      doExtract();
                      return GetToken();
             case  tokAsmADDR:
                      doAddr();
                      return GetToken();
             case  tokAsmOFFSET:
                      doOffset();
                      return GetToken();
			 default:
				 printf2("  %s ",TokenIdentName);
				 passline();
				 return GetToken();
		  }

      }

  //check for second token
      ftok = mtok;
      strcpy(ln,TokenIdentName);
      mtok = GetToken();  
	  if(mtok==':' && ftok == tokIdent)
      {
          // found a label
		    int i; 
		    for ( i = 0; i < gotoLabCnt; i++)
             {
                 if (!strcmp(IdentTable + gotoLabels[i][0], TokenIdentName))
                  {
                       printf2("l%d:\n", gotoLabels[i][1]);
					   return GetToken();
                  }
             }

            GenLabel(TokenIdentName,0);
            return GetToken();
      }

	  else if(mtok=='[')
      {
          doIndirect(GetToken());
          return GetToken();
      }
      else if(mtok==tokProc)
      {
          inMasmProc=1;
          doMasmProc(ln);
          return  GetToken();
      }
      else if(mtok==tokEndp)
      {
          if(strcmp(ln,CurMasmProcName))
            error("bad '%s' Proc name , must be %s",ln,CurMasmProcName);
          inMasmProc=0;
          if(reglistcnt) reglistcnt=0;
          return GetToken();
      }
/*     
	  else if(mtok == tokAsmKeyword && tokExt == tokAsmPROTO)
      {
         doProto(ln);
      }
 /*     
	  else
      {
	          printf2("%s ",ln);
      }
*/

	  if(ln[0])printf2("%s ", ln);
      p = CharQueue;
      while(*p != 10)
      {
          //if(OutFile) fputc(*p, OutFile);
          ln[Cnt++]=*p;
          ShiftCharN(1);
      }
      ln[Cnt]=0;

      printf2("%s %s\n",TokenIdentName,ln);
	  ln[0] = 0;
      return GetToken();

}

/*
int _stricmp(char* s1, char* s2)
{
  int k=0;
  while ( (s1[k]|32) == (s2[k]|32) )
  {
    if (!s1[k]) return 0;
    ++k;
  }
  return ((unsigned char)s1[k]) - ((unsigned char)s2[k]);
}
*/

int doasma()
{
  switch((TokenIdentName[1]|32))
	{
       case 'a':
             switch((TokenIdentName[2]|32))
              {
                  case 'a':
                          tokExt = tokAsmAAA;
                  case 'd':
	              case 'm':
	              case 's':
                            if(TokenIdentName[3]==0)return 1;
              }
              break;
        case 'd':
             switch((TokenIdentName[2]|32))
              {
                  case 'c':
                  case 'd':
                            if(TokenIdentName[3]==0)return 1;
                            if((TokenIdentName[3]|32)=='r' && TokenIdentName[4]==0)
                            {
                                tokExt = tokAsmADDR;
                                return 1;
                            }
              }
              break;
        case 'n':

              if( (TokenIdentName[2]|32)=='d' && TokenIdentName[3]==0)return 1;
              break;
	}
  	return 0;
}

int doasmb ()
{
   if(_stricmp("OUND",TokenIdentName+1)==0) return 1;
   if(_stricmp("SWAP",TokenIdentName+1)==0) return 1;
   if(_stricmp("YTE",TokenIdentName+1)==0) { tokExt = tokAsmBYTE; return 1;}
     return 0;
}

int doasmc()
{
   if(_stricmp("ALL",TokenIdentName+1)==0) { tokExt = tokAsmCALL; return 1;}
   //if( _stricmp("FAR",TokenIdentName+1)==0) return 1;
   //if( _stricmp("NEAR",TokenIdentName+1)==0) return 1;
   if(_stricmp("BD",TokenIdentName+1)==0) return 1;
   if(_stricmp("BW",TokenIdentName+1)==0) return 1;
   if(_stricmp("DQ",TokenIdentName+1)==0) return 1;
   if(_stricmp("LC",TokenIdentName+1)==0) return 1;
   if(_stricmp("LD",TokenIdentName+1)==0) return 1;
   if(_stricmp("LI",TokenIdentName+1)==0) return 1;
   if(_stricmp("LTS",TokenIdentName+1)==0) return 1;
   if(_stricmp("MC",TokenIdentName+1)==0) return 1;
   if(_stricmp("MP",TokenIdentName+1)==0) return 1;
   if(_stricmp("MPSB",TokenIdentName+1)==0) return 1;
   if(_stricmp("MPSW",TokenIdentName+1)==0) return 1;
   if(_stricmp("MPSD",TokenIdentName+1)==0) return 1;
   if(_stricmp("MPXCHG",TokenIdentName+1)==0) return 1;
   if(_stricmp("WD",TokenIdentName+1)==0) return 1;
   return 0;
}

int doasmd()
{
    if(_stricmp("AA",TokenIdentName+1)==0) return 1;
    if(_stricmp("AS",TokenIdentName+1)==0) return 1;
    if(_stricmp("B",TokenIdentName+1)==0) { tokExt = tokAsm_db; return 1;}
    if(_stricmp("D",TokenIdentName+1)==0) { tokExt = tokAsm_dd; return 1;}
    if(_stricmp("W",TokenIdentName+1)==0) { tokExt = tokAsm_dw; return 1;}
    if(_stricmp("Q",TokenIdentName+1)==0) { tokExt = tokAsm_dq; return 1;}
    if(_stricmp("EC",TokenIdentName+1)==0) return 1;
    if(_stricmp("IV",TokenIdentName+1)==0) return 1;
    if(_stricmp("WORD",TokenIdentName+1)==0) { tokExt = tokAsmDWORD; return 1;}
    return 0;
}

int doasme ()
{
   if(_stricmp("XTRACT",TokenIdentName+1)==0) { tokExt = tokAsmEXTRACT; return 1;}
   /*
   if(_stricmp("AX", TokenIdentName + 1) == 0) { tokExt = tokEax; return 1; }
   if(_stricmp("BX", TokenIdentName + 1) == 0) { tokExt = tokEbx; return 1; }
   if(_stricmp("CX", TokenIdentName + 1) == 0) { tokExt = tokEcx; return 1; }
   if(_stricmp("DX", TokenIdentName + 1) == 0) { tokExt = tokEdx; return 1; }
   if(_stricmp("SI", TokenIdentName + 1) == 0) { tokExt = tokEsi; return 1; }
   if(_stricmp("DI", TokenIdentName + 1) == 0) { tokExt = tokEdi; return 1; }
   if(_stricmp("SP", TokenIdentName + 1) == 0) { tokExt = tokEsp; return 1; }
   if(_stricmp("BP", TokenIdentName + 1) == 0) { tokExt = tokEbp; return 1; }
   */
     return 0;
}

int doasmf ()
{
   if(_stricmp("ROM",TokenIdentName+1)==0) { tokExt = tokAsmFROM; return 1;}
     return 0;
}

int doasmi ()
{
    if(_stricmp("DIV",TokenIdentName+1)==0) return 1;
    if(_stricmp("MUL",TokenIdentName+1)==0) return 1;
    if(_stricmp("N",TokenIdentName+1)==0) return 1;
    if(_stricmp("NSB",TokenIdentName+1)==0) return 1;
    if(_stricmp("NSW",TokenIdentName+1)==0) return 1;
    if(_stricmp("NSD",TokenIdentName+1)==0) return 1;
    if(_stricmp("NT",TokenIdentName+1)==0) return 1;
    if(_stricmp("NC",TokenIdentName+1)==0) return 1;
    if(_stricmp("NTO",TokenIdentName+1)==0) return 1;
    if(_stricmp("NVD",TokenIdentName+1)==0) return 1;
    if(_stricmp("NVLPG",TokenIdentName+1)==0) return 1;
    if(_stricmp("RET",TokenIdentName+1)==0) return 1;
    return 0;
}

int doasmj ()
{
    if(_stricmp("O",TokenIdentName+1)==0) return 1;
    if(_stricmp("NO",TokenIdentName+1)==0) return 1;
    if( _stricmp("B",TokenIdentName+1)==0 || _stricmp("NAE",TokenIdentName+1)==0 || _stricmp("C",TokenIdentName+1)==0 ) return 1;
    if( _stricmp("NB",TokenIdentName+1)==0 || _stricmp("AE",TokenIdentName+1)==0 || _stricmp("NC",TokenIdentName+1)==0 ) return 1;
    if( _stricmp("E",TokenIdentName+1)==0 || _stricmp("Z",TokenIdentName+1)==0   )  return 1;
    if( _stricmp("NE",TokenIdentName+1)==0 || _stricmp("NZ",TokenIdentName+1)==0 ) return 1;
    if( _stricmp("BE",TokenIdentName+1)==0 || _stricmp("NA",TokenIdentName+1)==0 ) return 1;
    if( _stricmp("NBE",TokenIdentName+1)==0 || _stricmp("A",TokenIdentName+1)==0 ) return 1;
    if(_stricmp("S",TokenIdentName+1)==0) return 1;
    if(_stricmp("NS",TokenIdentName+1)==0) return 1;
    if( _stricmp("P",TokenIdentName+1)==0 || _stricmp("PE",TokenIdentName+1)==0  ) return 1;
    if( _stricmp("NP",TokenIdentName+1)==0 || _stricmp("PO",TokenIdentName+1)==0 )  return 1;
    if( _stricmp("L",TokenIdentName+1)==0 || _stricmp("NGE",TokenIdentName+1)==0 ) return 1;
    if( _stricmp("NL",TokenIdentName+1)==0 || _stricmp("GE",TokenIdentName+1)==0 ) return 1;
    if( _stricmp("LE",TokenIdentName+1)==0 || _stricmp("NG",TokenIdentName+1)==0 ) return 1;
    if( _stricmp("NLE",TokenIdentName+1)==0 || _stricmp("G",TokenIdentName+1)==0 ) return 1;
    if(_stricmp("CXZ",TokenIdentName+1)==0) return 1;
    if(_stricmp("MP",TokenIdentName+1)==0) { tokExt = tokAsmJMP; return 1;}
    if( _stricmp("FAR",TokenIdentName+1)==0) return 1;
	if( _stricmp("NEAR",TokenIdentName+1)==0) return 1;
	if( _stricmp("SHORT",TokenIdentName+1)==0) return 1;
	return 0;
}

int doasml ()
{
    if(_stricmp("AHF",TokenIdentName+1)==0) return 1;
    if(_stricmp("DS",TokenIdentName+1)==0) return 1;
    if(_stricmp("EA",TokenIdentName+1)==0)  { tokExt = tokAsmLEA; return 1;}
    if(_stricmp("EAVE",TokenIdentName+1)==0) return 1;
    if(_stricmp("ES",TokenIdentName+1)==0) return 1;
    if(_stricmp("FS",TokenIdentName+1)==0) return 1;
    if(_stricmp("GS",TokenIdentName+1)==0) return 1;
    if(_stricmp("OADALL",TokenIdentName+1)==0) return 1;
    if(_stricmp("ODSB",TokenIdentName+1)==0) return 1;
    if(_stricmp("ODSW",TokenIdentName+1)==0) return 1;
    if(_stricmp("ODSD",TokenIdentName+1)==0) return 1;
    if(_stricmp("OCK",TokenIdentName+1)==0) return 1;
    if(_stricmp("OOP",TokenIdentName+1)==0) return 1;
    if( _stricmp("OOPE",TokenIdentName+1)==0 || _stricmp("OOPZ",TokenIdentName+1)==0 ) return 1;
    if( _stricmp("OOPNE",TokenIdentName+1)==0 || _stricmp("OOPNZ",TokenIdentName+1)==0 ) return 1;
    if(_stricmp("SS",TokenIdentName+1)==0) return 1;
    return 0;
}

int doasmm ()
{
    if(_stricmp("OV",TokenIdentName+1)==0) { tokExt = tokAsmMOV; return 1;}
    if(_stricmp("OVSB",TokenIdentName+1)==0) return 1;
    if(_stricmp("OVSD",TokenIdentName+1)==0) return 1;
    if(_stricmp("OVSW",TokenIdentName+1)==0) return 1;
    if(_stricmp("OVSX",TokenIdentName+1)==0) return 1;
    if(_stricmp("OVZX",TokenIdentName+1)==0) return 1;
    if(_stricmp("UL",TokenIdentName+1)==0) return 1;
    return 0;
}

int doasmn ()
{
    if(_stricmp("EG",TokenIdentName+1)==0) return 1;
    if(_stricmp("OP",TokenIdentName+1)==0) return 1;
    if(_stricmp("OT",TokenIdentName+1)==0) return 1;
    return 0;
}

int doasmo ()
{
    if(_stricmp("PSIZ",TokenIdentName+1)==0 || strcmp("PSIZE",TokenIdentName+1)==0) return 1;
    if(_stricmp("R",TokenIdentName+1)==0) return 1;
    if(_stricmp("UT",TokenIdentName+1)==0) return 1;
    if(_stricmp("UTSB",TokenIdentName+1)==0) return 1;
    if(_stricmp("UTSW",TokenIdentName+1)==0) return 1;
    if(_stricmp("UTSD",TokenIdentName+1)==0) return 1;
    if(_stricmp("FFSET",TokenIdentName+1)==0) { tokExt = tokAsmOFFSET; return 1;}
    return 0;
}

int doasmp ()
{
    if(_stricmp("TR",TokenIdentName+1)==0) { tokExt = tokAsmPTR; return 1;}
    if(_stricmp("OP",TokenIdentName+1)==0) { tokExt = tokAsmPOP; return 1;}
    if(_stricmp("OPA",TokenIdentName+1)==0) return 1;
    if(_stricmp("OPAD",TokenIdentName+1)==0) return 1;
    if(_stricmp("OPF",TokenIdentName+1)==0) return 1;
    if(_stricmp("OPFD",TokenIdentName+1)==0) return 1;
    if(_stricmp("USH",TokenIdentName+1)==0) { tokExt = tokAsmPUSH; return 1;}
    if(_stricmp("USHA",TokenIdentName+1)==0) return 1;
    if(_stricmp("USHAD",TokenIdentName+1)==0) return 1;
    if(_stricmp("USHF",TokenIdentName+1)==0) return 1;
    if(_stricmp("USHFD",TokenIdentName+1)==0) return 1;
    if(_stricmp("ROTO",TokenIdentName+1)==0) { tokExt = tokAsmPROTO; return 1;}
    return 0;
}

int doasmr ()
{
    if(_stricmp("CL",TokenIdentName+1)==0) return 1;
    if(_stricmp("CR",TokenIdentName+1)==0) return 1;
    if(_stricmp("EP",TokenIdentName+1)==0) return 1;
    if(_stricmp("EPE",TokenIdentName+1)==0) return 1;
    if(strcmp("EPNE",TokenIdentName+1)==0) return 1;
    if(_stricmp("EPNZ",TokenIdentName+1)==0) return 1;
    if(_stricmp("EPZ",TokenIdentName+1)==0) return 1;
    if(_stricmp("ET",TokenIdentName+1)==0) { tokExt = tokAsmRET; return 1;}
    if(_stricmp("ETF",TokenIdentName+1)==0) return 1;
    if(_stricmp("OL",TokenIdentName+1)==0) return 1;
    if(_stricmp("OR",TokenIdentName+1)==0) return 1;
    if(_stricmp("PUSH",TokenIdentName+1)==0) { tokExt = tokAsmRPUSH; return 1;}
    return 0;
}

int doasms ()
{
    if(_stricmp("AHF",TokenIdentName+1)==0) return 1;
    if(_stricmp("AL",TokenIdentName+1)==0) return 1;
    if(_stricmp("AR",TokenIdentName+1)==0) return 1;
    if(_stricmp("BB",TokenIdentName+1)==0) return 1;
    if(_stricmp("CASB",TokenIdentName+1)==0) return 1;
    if(_stricmp("CASW",TokenIdentName+1)==0) return 1;
    if(_stricmp("CASD",TokenIdentName+1)==0) return 1;
    if(_stricmp("HL",TokenIdentName+1)==0) return 1;
    if(_stricmp("HR",TokenIdentName+1)==0) return 1;
    if(_stricmp("TC",TokenIdentName+1)==0) return 1;
    if(_stricmp("TD",TokenIdentName+1)==0) return 1;
    if(_stricmp("TI",TokenIdentName+1)==0) return 1;
    if(_stricmp("TOSB",TokenIdentName+1)==0) return 1;
    if(_stricmp("TOSW",TokenIdentName+1)==0) return 1;
    if(_stricmp("TOSD",TokenIdentName+1)==0) return 1;
    if(_stricmp("UB",TokenIdentName+1)==0) return 1;
    return 0;
}

int doasmw ()
{
    if(_stricmp("AIT",TokenIdentName+1)==0) return 1;
    if(_stricmp("BINVD",TokenIdentName+1)==0) return 1;
    if(_stricmp("ORD",TokenIdentName+1)==0) { tokExt = tokAsmWORD; return 1;}
    return 0;
}

int doasmx ()
{
    if(_stricmp("ADD",TokenIdentName+1)==0) return 1;
    if(_stricmp("CHG",TokenIdentName+1)==0) return 1;
    if(_stricmp("LAT",TokenIdentName+1)==0) return 1;
    if(_stricmp("LATB",TokenIdentName+1)==0) return 1;
    if(_stricmp("OR",TokenIdentName+1)==0) return 1;
    return 0;
}

int doasmleftovers ()
{
    if(_stricmp("ENTER",TokenIdentName)==0) return 1;
    if(_stricmp("HALT",TokenIdentName)==0 || _stricmp("HLT",TokenIdentName)==0) return 1;
    if(_stricmp("TEST",TokenIdentName)==0) return 1;
    return 0;
}

int checkAsm()
{
	tokExt = 0;
    switch((TokenIdentName[0]|32))
	{
	    case 'a':   return doasma();  break;
	    case 'b':   return doasmb();  break;
	    case 'c':   return doasmc();  break;
	    case 'd':   return doasmd();  break;
	    case 'e':   return doasme();  break;
	    case 'f':   return doasmf();  break;
	    case 'i':   return doasmi();  break;
	    case 'j':   return doasmj();  break;
	    case 'l':   return doasml();  break;
	    case 'm':   return doasmm();  break;
	    case 'n':   return doasmn();  break;
	    case 'o':   return doasmo();  break;
	    case 'p':   return doasmp();  break;
	    case 'r':   return doasmr();  break;
	    case 's':   return doasms();  break;
	    case 'w':   return doasmw();  break;
	    case 'x':   return doasmx();  break;
        default:    return doasmleftovers();  break;
    }
   return 0;
}


int checkExtension()
{
    switch(TokenIdentName[0])
	{
          case 'c':
                    if(strcmp("decl",TokenIdentName+1)==0)
                     {
                         tokExt = tokExtCdecl;
                         return 1;
                     }
                    break;
          case 'f':
                    if(strcmp("astcall",TokenIdentName+1)==0)
                     {
                         tokExt = tokExtFastcall;
                         return 1;
                     }
                    break;
          case 's':
                    if(strcmp("tdcall",TokenIdentName+1)==0)
                     {
                         tokExt = tokExtStdcall;
                         return 1;
                     }
                    break;
          case 'p':
                    if(strcmp("ascall",TokenIdentName+1)==0)
                     {
                         tokExt = tokExtPascall;
                         return 1;
                     }
                    break;
          case 'O':
                    if(strcmp("PTION",TokenIdentName+1)==0)
                     {
                         tokExt = tokExtOption;
                         return 1;
                     }
                    break;
	}
   return 0;
}

int checkBlitz()
{
   switch(TokenIdentName[0])
	{
       /*
          case '':
                    if(strcmp("",TokenIdentName)==0)
                     {
                         tokExt2 = tokExtCdecl;
                         return 1;
                     }
                    break;
      */
          case 'N':
                    if(strcmp("Newtype",TokenIdentName)==0)
                     {
                         tokExt2 = tokBltzNewtype;
                         return 1;
                     }
                    break;

          case 's':
                    if(TokenIdentName[1]==0)
                     {
                         tokExt2 = tokBltzStrtype;
                         return 1;
                     }
                    break;
          case 'w':
                    if(TokenIdentName[1]==0)
                     {
                         tokExt2 = tokBltzWordtype;
                         return 1;
                     }
                    break;
          case 'b':
                    if(TokenIdentName[1]==0)
                     {
                         tokExt2 = tokBltzBytetype;
                         return 1;
                     }
                    break;
          case 'i':
                    if(TokenIdentName[1]==0)
                     {
                         tokExt2 = tokBltzInttype;
                         return 1;
                     }
                    break;
          case 'l':
                    if(TokenIdentName[1]==0)
                     {
                         tokExt2 = tokBltzLongtype;
                         return 1;
                     }
                    break;
          case 'f':
                    if(TokenIdentName[1]==0)
                     {
                         tokExt2 = tokBltzFloattype;
                         return 1;
                     }

                    break;
           case 'F':
                    if(strcmp("Function",TokenIdentName)==0)
                     {
                         tokExt2 = tokBltzFnc;
                         return 1;
                     }
                    break;
           case 'D':
                    if(strcmp("Dim",TokenIdentName)==0)
                     {
                         tokExt2 = tokBltzDim;
                         return 1;
                     }
                    else if(strcmp("Deftype",TokenIdentName)==0)
                     {
                         tokExt2 = tokBltzDeftype;
                         return 1;
                     }
                    break;
           case 'S':
                    if(strcmp("Statement",TokenIdentName)==0)
                     {
                         tokExt2 = tokBltzStmt;
                         return 1;
                     }
                    break;
           case 'U':
                    if(strcmp("Usepath",TokenIdentName)==0)
                     {
                         tokExt2 = tokBltzUsepath;
                         return 1;
                     }
                    break;
           case 'M':
                    if(strcmp("Macro",TokenIdentName)==0)
                     {
                         tokExt2 = tokBltzMacro;
                         return 1;
                     }
                    break;

	}

     return 0;
}
