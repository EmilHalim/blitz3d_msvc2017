// *********************************************************************
// Created with BCX32 - BASIC To C/C++ Translator (V) bcx 6.9.9.1 (2012/10/14)
//                 BCX (c) 1999 - 2009 by Kevin Diggins
// *********************************************************************
//              Translated for compiling with a C++ Compiler
// *********************************************************************
#include <windows.h>    // Win32 Header File 
#include <windowsx.h>   // Win32 Header File 
#include <commctrl.h>   // Win32 Header File 
#include <commdlg.h>    // Win32 Header File 
#include <mmsystem.h>   // Win32 Header File 
#include <shellapi.h>   // Win32 Header File 
#include <shlobj.h>     // Win32 Header File 
#include <richedit.h>   // Win32 Header File 
#include <wchar.h>      // Win32 Header File 
#include <objbase.h>    // Win32 Header File 
#include <ocidl.h>      // Win32 Header File 
#include <winuser.h>    // Win32 Header File 
#include <olectl.h>     // Win32 Header File 
#include <oaidl.h>      // Win32 Header File 
#include <ole2.h>       // Win32 Header File 
#include <oleauto.h>    // Win32 Header File 
#include <winsock.h>    // Win32 Header File 
#include <process.h>    // dos
#include <conio.h>      // dos
#include <direct.h>     // dos
#include <io.h>         // dos
#include <ctype.h>      // dos/linux
#include <fcntl.h>      // dos/linux
#include <math.h>       // dos/linux
#include <stdio.h>      // dos/linux
#include <string.h>     // dos/linux
#include <stddef.h>     // dos/linux
#include <stdlib.h>     // dos/linux
#include <setjmp.h>     // dos/linux
#include <time.h>       // dos/linux
#include <stdarg.h>     // dos/linux


// ***************************************************
// Compiler Defines
// ***************************************************

// C++
#if defined( __cplusplus )
  #define overloaded
  #define C_EXPORT EXTERN_C __declspec(dllexport)
  #define C_IMPORT EXTERN_C __declspec(dllimport)
#else
  #define C_EXPORT __declspec(dllexport)
  #define C_IMPORT __declspec(dllimport)
#endif

// Open Watcom defs
#if defined( __WATCOM_CPLUSPLUS__ ) || defined( __TINYC__ )
  #define atanl atan
  #define sinl  sin
  #define cosl  cos
  #define tanl  tan
  #define asinl asin
  #define acosl acos
  #define log10l log10
  #define logl   log
  #define _fcloseall fcloseall
#endif

// Borland C++ 5.5.1 defs - bcc32.exe
#if defined( __BCPLUSPLUS__ )
  // ===== Borland Libraries ==========
  #include <dos.h>
  #pragma comment(lib,"import32.lib")
  #pragma comment(lib,"cw32.lib")
  // ==================================
#endif

// Microsoft VC++
#ifndef DECLSPEC_UUID
  #if (_MSC_VER >= 1100) && defined ( __cplusplus )
    #define DECLSPEC_UUID(x)    __declspec(uuid(x))
  #else
    #define DECLSPEC_UUID(x)
  #endif
#endif

// *************************************************
#ifndef __cplusplus
  #error A C++ compiler is required
#endif

// *************************************************
// Instruct Linker to Search Object/Import Libraries
// *************************************************
#if !defined( __LCC__ )
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"comctl32.lib")
#pragma comment(lib,"advapi32.lib")
#pragma comment(lib,"winspool.lib")
#pragma comment(lib,"shell32.lib")
#pragma comment(lib,"ole32.lib")
#pragma comment(lib,"oleaut32.lib")
#pragma comment(lib,"uuid.lib")
#pragma comment(lib,"odbc32.lib")
#pragma comment(lib,"odbccp32.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"comdlg32.lib")
#pragma comment(lib,"imagehlp.lib")
#pragma comment(lib,"version.lib")
#else
#pragma lib <winspool.lib>
#pragma lib <shell32.lib>
#pragma lib <ole32.lib>
#pragma lib <oleaut32.lib>
#pragma lib <uuid.lib>
#pragma lib <odbc32.lib>
#pragma lib <odbccp32.lib>
#pragma lib <winmm.lib>
#pragma lib <imagehlp.lib>
#pragma lib <version.lib>
#endif
// *************************************************
// End of Object/Import Libraries To Search
// *************************************************

// *************************************************
//        User's GLOBAL ENUM blocks
// *************************************************


#define cSizeOfDefaultString 2048 

// *************************************************
//            User Defined Constants
// *************************************************

#ifndef  __cplusplus
#define false 0
#define true 1
#endif

// *************************************************
//               Standard Prototypes
// *************************************************

int     str_cmp(char*, char*);
int     EoF (FILE*);
char*   BCX_TmpStr(size_t,size_t= 128,int= 1);
char*   lcase (char*);
char*   ltrim (char*,char=32);
char*   trim (char*);
char*   left (char*,int);
char*   right (char*,int);
char*   replace (char*,char*,char*);
char*   BcxSplitPath (char *, int);
char*   IRemoveStr (char*,char*);
char*   join (int, ... );
char*   chr(int,int=0,int=0,int=0,int=0,int=0,int=0,int=0,int=0,int=0);
int     instr(char*,char*,int=0,int=0);
char    *MakeLCaseTbl(void);
char    *_stristr_(char*,char*);
char    *_strstr_(char*,char*);
int     inchr (char*,char*);
// *************************************************
//          User Defined Types And Unions
// *************************************************


// *************************************************
//                System Variables
// *************************************************

static char *LowCase;

// *************************************************
//            User Global Variables
// *************************************************

static HINSTANCE BCX_hInstance;
static FILE   *fp1=NULL;
static FILE   *fp2=NULL;
static FILE   *FP_TMP;
static FILE   *FP;
static int     ExtrnCnt=0;
static char    ExtrnName[100][cSizeOfDefaultString];
static char    inBuf[cSizeOfDefaultString];
static char    cNam[cSizeOfDefaultString];
static char    cNamPP[cSizeOfDefaultString];
static char    cAsm[cSizeOfDefaultString];
static char    cFAsm[cSizeOfDefaultString];
char bf[1024];

// *************************************************
//               Standard Macros
// *************************************************

#define FDRV   2
#define FPATH  4
#define FNAME  8
#define FEXT  16


// *************************************************
//               User Prototypes
// *************************************************
struct PlugInInfo
{
	 float version;
	 int   order;
     char* disc;
};
char disc[] =" smallerC For Masm plugin ";
static PlugInInfo piInfo = { 1.0 , 2 , disc}; 

extern "C" int DoMAsm(char* name, char* oname);

// *************************************************


void    Abort (char*);
void    Doccode (void);
void    Do_ptr (void);

static int     SmlC_nn=1;
// *************************************************
//            User Global Initialized Arrays
// *************************************************


// *************************************************
//              User GLOBAL SET Statements
// *************************************************

static char* m_Reg[]=
  {
 "ah","al","ax","bh","bl","bx","ch","cl","cx","dh","dl","dx","si","di","bp","eax","ebx","ecx","edx","esi","edi","ebp","esp","mm0","mm1","mm2","mm3","mm4","mm5","mm6","mm7","st","xmm0","xmm1","xmm2","xmm3","xmm4","xmm5","xmm6","xmm7"
};

static char* m_RegB[]=
  {
 "ah","al","bh","bl","ch","cl","dh","dl"
};

static char* m_RegW[]=
  {
 "ax","bx","cx","dx","si","di","bp"
};

static char* m_RegD[]=
  {
 "eax","ebx","ecx","edx","esi","edi","ebp","esp"
};

static char* m_RegF[]=
  {
 "st0","st1","st2","st3","st4","st5","st6","st7"
};



// *************************************************
//                 Runtime Functions
// *************************************************

#ifndef BCXTmpStrSize
#define BCXTmpStrSize  2048
#endif
char *BCX_TmpStr (size_t Bites,size_t  iPad,int iAlloc)
{
  static int   StrCnt;
  static char *StrFunc[BCXTmpStrSize];
  StrCnt=(StrCnt + 1) & (BCXTmpStrSize-1);
  if(StrFunc[StrCnt]) {free (StrFunc[StrCnt]); StrFunc[StrCnt] = NULL;}
#if defined BCX_MAX_VAR_SIZE
  if(Bites*sizeof(char)>BCX_MAX_VAR_SIZE)
  {
  printf("Buffer Overflow caught in BCX_TmpStr - requested space of %d EXCEEDS %d\n",(int)(Bites*sizeof(char)),BCX_MAX_VAR_SIZE);
  abort();
  }
#endif
  if(iAlloc) StrFunc[StrCnt]=(char*)calloc(Bites+128,sizeof(char));
  return StrFunc[StrCnt];
}


int str_cmp (char *a, char *b)
{
  register int counter=0;
  for(;;)
   {
    if((a[counter]^b[counter]))
     {
      if((UINT) a[counter]>= (UINT) b[counter])
      return  1;
      return -1;
     }
    if(!a[counter]) return 0;
    counter++;
   }
}


int EoF (FILE* stream)
{
  register int c, status = ((c = fgetc(stream)) == EOF);
  ungetc(c,stream);
  return status;
}


char *left (char *S, int length)
{
  register int tmplen = strlen(S);
  if(length<1) return BCX_TmpStr(1);
  if(length<tmplen) tmplen=length;
  char *strtmp = BCX_TmpStr(tmplen);
  return (char*)memcpy(strtmp,S,tmplen);
}


char *right (char *S, int length)
{
  int tmplen = strlen(S);
  char *BCX_RetStr = BCX_TmpStr(tmplen);
  tmplen -= length;
  if (tmplen<0) tmplen = 0;
  return strcpy(BCX_RetStr, &S[tmplen]);
}


char *trim (char *S)
{
  if(S[0]==0) return S;
  while(*S==32 || (*S>=9 && *S<=13)) S++;
  register int i = strlen(S);
  while( i>0 && (S[i-1]==32 || (S[i-1]>=9 && S[i-1]<=13))) i--;
  char *strtmp=BCX_TmpStr(i);
  return (char*)memcpy(strtmp,S,i);
}


char *ltrim (char *S, char c)
{
  if(S[0]==0) return S;
  while(*S==32 || (*S>=9 && *S<=13) || *S==c) S++;
  char *strtmp = BCX_TmpStr(strlen(S));
  return strcpy(strtmp,S);
}


char *replace (char *src, char *pat, char *rep)
{
  register size_t patsz, repsz, tmpsz, delta;
  register char *strtmp, *p, *q, *r;
  if (!pat || !*pat)
   {
     strtmp = BCX_TmpStr(strlen(src));
     if (!strtmp) return NULL;
     return strcpy(strtmp, src);
   }
  repsz = strlen(rep);
  patsz = strlen(pat);
  for (tmpsz=0, p=src; (q=_strstr_(p,pat))!=0; p=q+patsz)
   tmpsz += (size_t) (q - p) + repsz;
   tmpsz += strlen(p);
   strtmp = BCX_TmpStr(tmpsz);
   if (!strtmp) return NULL;
    for (r=strtmp,p=src; (q=_strstr_(p,pat))!=0;p=q+patsz)
     {
       delta = (size_t) (q-p);
       memcpy(r,p,delta); r += delta;
       strcpy(r,rep);      r += repsz;
     }
  strcpy(r,p);
  return strtmp;
}


char *lcase (char *S)
{
  register char *strtmp = BCX_TmpStr(strlen(S));
  return _strlwr(strcpy(strtmp,S));
}


char *IRemoveStr (char *a, char *b)
{
  char *strtmp, *p, *d;
  int  tmplen;
  strtmp = d = BCX_TmpStr(strlen(a));
  if(!b || !*b) return strcpy(strtmp,a);
  p=_stristr_(a,b); tmplen = strlen(b);
  while(p)
   {
     memcpy(d,a,p-a);
     d+= (p-a);
     a=p+tmplen;
     p=_stristr_(a,b);
   }
  strcpy(d,a);
  return strtmp;
}


char *chr (int a,int b,int c,int d,int e,int f,int g,int h,int i,int j)
{
  register char *strtmp = BCX_TmpStr(11);
  strtmp[0]  = a;
  strtmp[1]  = b;
  strtmp[2]  = c;
  strtmp[3]  = d;
  strtmp[4]  = e;
  strtmp[5]  = f;
  strtmp[6]  = g;
  strtmp[7]  = h;
  strtmp[8]  = i;
  strtmp[9]  = j;
  strtmp[10] = 0;
  return strtmp;
}


char * join(int n, ...)
{
  register int i = n, tmplen = 0;
  register char *s_;
  register char *strtmp;
  va_list marker;
  va_start(marker, n); // Initialize variable arguments
  while(i-- > 0)
  {
    s_ = va_arg(marker, char *);
    if(s_) tmplen += strlen(s_);
  }
  strtmp = BCX_TmpStr(tmplen);
  va_end(marker); // Reset variable arguments
  i = n;
  va_start(marker, n); // Initialize variable arguments
  while(i-- > 0)
  {
    s_ = va_arg(marker, char *);
    if(s_) strcat(strtmp, s_);
  }
  va_end(marker); // Reset variable arguments
  return strtmp;
}


int instr(char* mane,char* match,int offset,int sensflag)
{
  register char *s;
  if (!mane || !match || ! *match || offset>(int)strlen(mane)) return 0;
  if (sensflag)
    s = _stristr_(offset>0 ? mane+offset-1 : mane,match);
  else
    s = _strstr_(offset>0 ? mane+offset-1 : mane,match);
  return s ? (int)(s-mane)+1 : 0;
}


char  *MakeLCaseTbl (void)
{
  static char tbl[257];
      int i; for (i=0; i < 256; i++)
  tbl[i] = i;
  tbl[0] = 1;
  tbl[256] = 0;
  CharLowerA((char *)tbl);
  tbl[0] = 0;
  return tbl;
}


char *_stristr_(char *String, char *Pattern)
{
  int   mi=-1;
  while(Pattern[++mi])
   {
     if(String[mi]==0) return 0;
      if((unsigned char)(String[mi]|32)!=(unsigned char)(Pattern[mi]|32))
       { String++; mi=-1; }
   }
  return String;
}


char *_strstr_(char *String, char *Pattern)
{
  int   mi=-1;
  while(Pattern[++mi])
   {
     if(String[mi]==0) return 0;
     if(String[mi]!=Pattern[mi])
       { String++; mi=-1; }
   }
  return String;
}


int inchr (char *A, char *B)
{
  register char* C=A;
  while(*C)
    {
      if(*C==*B) return C-A+1;
      C++;
    }
  return 0;
}


char *BcxSplitPath (char *FPath, int mask)
{
  if(!FPath) return BCX_TmpStr(1);
  char *BCX_RetStr=BCX_TmpStr(strlen(FPath));
  char tmp[MAX_PATH*4];
  _splitpath(FPath,tmp,&tmp[MAX_PATH],&tmp[MAX_PATH*2],&tmp[MAX_PATH*3]);
  if(mask & FDRV) strcat(BCX_RetStr,tmp);
  if(mask & FPATH)strcat(BCX_RetStr,&tmp[MAX_PATH]);
  if(mask & FNAME)strcat(BCX_RetStr,&tmp[MAX_PATH*2]);
  if(mask & FEXT) strcat(BCX_RetStr,&tmp[MAX_PATH*3]);
  return BCX_RetStr;
}

char *str (double d)
{
  register char *strtmp = BCX_TmpStr(24);
  sprintf(strtmp,"% .15G",d);
  return strtmp;
}


// ************************************
//       User C code
// ************************************

 #define MaxStk 4096
 #define cSizeOfDefaultString 2048
 #define c_DblQt 34

 static int     Ndx;
 static char    Stk[MaxStk][cSizeOfDefaultString];


 #define iMatchLft(A,B) iMatch(A,B, 0)
 #define iMatchWrd(A,B) iMatch(A,B, 1)
 #define iMatchRgt(A,B) iMatch(A,B, 2)

int iMatch (char* Arg,char* MatchStr,int mt)
{
  if(mt==2 )
    {
      int     L1;
      int     L2;
      L1= strlen( Arg);
      L2= strlen( MatchStr);
      if(L1<L2 )
        {
          return FALSE;
        }
      Arg=( Arg+ L1)- L2;
    }
  while(*MatchStr)
    {
      if(*Arg==0 )
        {
          return FALSE;
        }
      if((*Arg  |  32)!=(*MatchStr  |  32))
        {
          return FALSE;
        }
      Arg++;
      MatchStr++;
    }

  if(mt&&*Arg!=0 )
    {
      return FALSE;
    }
  return TRUE;
}

void FastLexer (char* Arg,char* delim1,char* delim2,int TokQuote=1)
{
  int     cnt1=0;
  int     cnt2=0;
  PCHAR    pd1;
  PCHAR    pd2;
  for(int aa = 1; aa <= Ndx; aa++) Stk[aa][0] = 0;
  Ndx= 1;
  while(Arg[cnt1])
    {
      if(Arg[cnt1]==9 ) Arg[cnt1]=32;
      if(Arg[cnt1]==c_DblQt )
        {
          if(cnt2&&TokQuote )
            {
              Stk[Ndx++][cnt2]       = 0;
              cnt2= 0;
            }
          Stk[Ndx][cnt2]     = c_DblQt;
          while(Arg[++cnt1]!=c_DblQt)
            {
              Stk[Ndx][++cnt2]       = Arg[ cnt1];
              if(Arg[cnt1]==0 )
                {
                  return;
                }
            }

          Stk[Ndx][++cnt2]       = Arg[ cnt1];
          if(TokQuote )
            {
              Stk[Ndx++][++cnt2]         = 0;
              cnt2= 0;
              goto AGAIN;
            }
        }
      pd1= delim1;
      while(*pd1)
        {
          if(*(pd1++)==Arg[cnt1])
            {
              if(cnt2 )
                {
                  Stk[Ndx++][cnt2]       = 0;
                  cnt2= 0;
                }
              goto AGAIN;
            }
        }

      pd2= delim2;
      while(*pd2)
        {
          if(*(pd2++)==Arg[cnt1])
            {
              if(cnt2 )
                {
                  Stk[Ndx++][cnt2]       = 0;
                }
              Stk[Ndx][0]     = Arg[ cnt1];
              Stk[Ndx++][1]       = 0;
              cnt2= 0;
              goto AGAIN;
            }
        }

      Stk[Ndx][cnt2++]       = Arg[ cnt1];

AGAIN:;
      cnt1++;
    }

  Stk[Ndx][cnt2]     = 0;
  if(cnt2==0 )
    {
      Ndx--;
    }
}

#define MAX_FRAGS 3000
#define MAX_SECS  32
#define MAX_SEC_NAME_LEN 32

char* InName;
char* IntermidName;
char* OutName;
char* OutFormat;
FILE *fin, *fout;
int DontAssemble;
int FlatBinary;

int InCodeSeg = false;

void Cerror(char* format, ...)
{
#ifndef __SMALLER_C__
  va_list vl;
  va_start(vl, format);
#else
  void* vl = &format + 1;
#endif

  if (fout)
    fclose(fout);
  if (fin)
    fclose(fin);
  if (OutName)
    remove(OutName);
  if (IntermidName)
    remove(IntermidName);

  puts("");

  vprintf(format, vl);

#ifndef __SMALLER_C__
  va_end(vl);
#endif

  exit(EXIT_FAILURE);
}

typedef struct
{
  long start;
  long end;
  unsigned sec;
  unsigned line;
} tFrag;

tFrag frag[MAX_FRAGS];
unsigned fragCnt;
long frag_1end;

char secName[MAX_SECS][MAX_SEC_NAME_LEN + 1];
unsigned secCnt;

unsigned line;

unsigned addSection(char* name)
{
  unsigned i;
  if (secCnt >= MAX_SECS)
    Cerror("Too many sections\n");
  for (i = 0; i < MAX_SECS; i++)
    if (!strcmp(secName[i], name))
      return i;
  strcpy(secName[secCnt], name);
  return secCnt++;
}

void beginFrag(long start, char* name, unsigned line)
{
  unsigned sec;
  if (fragCnt >= MAX_FRAGS)
    Cerror("Too many section fragments\n");
  sec = addSection(name);
  frag[fragCnt].start = start;
  frag[fragCnt].sec = sec;
  frag[fragCnt++].line = line;
}

void endFrag(long end)
{
  if (fragCnt)
    frag[fragCnt - 1].end = end;
  else
    frag_1end = end;
}

int _cdecl fragCmp(const void* p1_, const void* p2_)
{
  const tFrag *p1 = (const tFrag*)p1_, *p2 = (const tFrag*)p2_;
  unsigned sec1 = p1->sec, sec2 = p2->sec;
  if (sec1 != sec2)
  {
    char *name1 = secName[sec1], *name2 = secName[sec2];


    if (!strcmp(name1, ".rodata"))
      return -1;
    if (!strcmp(name2, ".rodata"))
      return +1;

    if (!strcmp(name1, ".data"))
      return -1;
    if (!strcmp(name2, ".data"))
      return +1;

    if (!strcmp(name1, ".bss"))
      return -1;
    if (!strcmp(name2, ".bss"))
      return +1;


    if (!strcmp(name1, ".text"))
      return -1;
    if (!strcmp(name2, ".text"))
      return +1;

    return strcmp(name1, name2);
  }
  if (p1->start < p2->start)
    return -1;
  if (p1->start > p2->start)
    return +1;
  return 0;
}

unsigned skipSpaces(FILE* f)
{
  unsigned cnt = 0;
  int c;
  while ((c = fgetc(f)) != EOF && (c == ' ' || c == '\t'))
    cnt += (c == '\t') ? 8 : 1;
  if (c != EOF)
    ungetc(c, f);
  return cnt;
}

void skipLine(FILE* f)
{
  int c;
  while ((c = fgetc(f)) != EOF && c != '\r' && c != '\n');
  if (c == '\r' && (c = fgetc(f)) != EOF && c != '\n')
    ungetc(c, f);
}

unsigned readNonspace(FILE* f, char buf[/*len + 1*/], unsigned len)
{
  unsigned cnt = 0;
  int c;
  while (cnt < len && (c = fgetc(f)) != EOF)
  {
    switch (c)
    {
    default:
      buf[cnt++] = c;
      continue;
    case ' ': case '\t': case '\r': case '\n':
      ungetc(c, f);

    }
    break;
  }
  buf[cnt] = '\0';

  return cnt;
}

void copyLine(FILE* fin, FILE* fout)
{
  int c;
  while ((c = fgetc(fin)) != EOF && c != '\r' && c != '\n')
    fputc(c, fout);
  if (c == '\r' && (c = fgetc(fin)) != EOF && c != '\n')
    ungetc(c, fin);
  fputc('\n', fout);
}

void findFrags(FILE* f)
{
  long end;

  line = 1;

  do
  {
    union
    {
      char ident[sizeof "section" + 1/*+1 in case it's longer, e.g. "sections"; space for NUL included in sizeof*/];
      char name[MAX_SEC_NAME_LEN + 1/*+1 in case it's longer*/ + 1/*NUL*/];
    } u;

    end = ftell(f);

    skipSpaces(f);
    readNonspace(f, u.ident, sizeof u.ident - 1);

    if (!strcmp(u.ident, "section"))
    {
      unsigned len;
      long start;

      skipSpaces(f);
      len = readNonspace(f, u.name, sizeof u.name - 1);
      if (len > MAX_SEC_NAME_LEN)
        Cerror("Section name too long\n");
      if (len == 0)
        Cerror("Section name missing\n");

      skipLine(f);
      start = ftell(f);
      endFrag(end);
      beginFrag(start, u.name, line + 1);
    }
    else
    {
      skipLine(f);
    }

    line++;
  } while (!feof(f) && !ferror(f));

  end = ftell(f);
  endFrag(end);

  qsort(frag, fragCnt, sizeof(frag[0]), &fragCmp);
}

void n2f1(FILE* fin, long start, long end, FILE* fout)
{
  fseek(fin, start, SEEK_SET);

  while (start >= 0 && start < end)
  {
    char ident[sizeof "extern"/*longest of directives being replaced*/ +
               1/*+1 in case it's longer, e.g. "externs"; space for NUL included in sizeof*/];
    unsigned spc;

    spc = skipSpaces(fin);

    readNonspace(fin, ident, sizeof ident - 1);

    if (*ident == ';')
    {

      skipLine(fin);
    }
    else
    {
      int skip = 0;

      while (spc)
        if (spc >= 8)
          fputc('\t', fout), spc -= 8;
        else
          fputc(' ', fout), spc--;


      if (!strcmp(ident, "alignb"))
        fputs("align", fout);
      else if (!strcmp(ident, "global"))
      {

        if (FlatBinary)
          fputc('\n', fout), skip = 1;
        else
          fputs("public", fout);
      }
      else if (!strcmp(ident, "resb"))
        fputs("rb", fout);
      else if (!strcmp(ident, "resw"))
        fputs("rw", fout);
      else if (!strcmp(ident, "resd"))
        fputs("rd", fout);
      else if (!strcmp(ident, "extern"))
        fputs("extern ", fout);
      else if (!strcmp(ident, "bits"))
        fputs("use", fout), skipSpaces(fin);
      else
        fputs(ident, fout);


      if (skip)
        skipLine(fin);
      else
        copyLine(fin, fout);
    }

    line++;
    start = ftell(fin);
  }

}

void n2f(FILE* fin, FILE* fout)
{
  unsigned i, lastSec = secCnt, sec;
/*
  if (!strcmp(OutFormat, "elf"))
    fputs("format elf\n", fout);
*/

  line = 1;
  n2f1(fin, 0, frag_1end, fout);



  for (i = 0; i < fragCnt; i++)
  {
    sec = frag[i].sec;
    line = frag[i].line;

    if (lastSec != sec)
    {

      lastSec = sec;

      if (!strcmp(secName[sec], ".text"))
       {
         InCodeSeg = true;
           fprintf(fout, "\t.align	16\n");
       }
      else if (!strcmp(secName[sec], ".data") || !strcmp(secName[sec], ".bss"))
       {
        if(InCodeSeg)
            fprintf(fout, "\t.align	4\n");
        InCodeSeg = false;
       }
    }

    n2f1(fin, frag[i].start, frag[i].end, fout);
  }

}

void convert(void)
{
  secCnt = fragCnt = 0;

  char* out;

    out = OutName;



  if ((fin = fopen(InName, "rb")) == NULL)
    Cerror("Can't open file '%s'\n", InName);

  if ((fout = fopen(out, "w")) == NULL)
    Cerror("Can't create file '%s'\n", out);

  findFrags(fin);

  n2f(fin, fout);

  if (ferror(fin))
    Cerror("Failed to read file '%s'\n", InName);

  if (ferror(fout))
    Cerror("Failed to write file '%s'\n", out);

  fclose(fin);
  fin = NULL;
  fclose(fout);
  fout = NULL;
}

void fassemble(char* argv0)
{
  size_t argv0l = strlen(argv0);
  size_t cmdsz = strlen(IntermidName) + (OutName ? 1 + strlen(OutName) : 0) + 1/*NUL*/;
  char* pfasm;
  char* cmd;
  int res;


  if (argv0l > 4 &&
      argv0[argv0l-4] == '.' &&
      (argv0[argv0l-3] == 'E' || argv0[argv0l-3] == 'e') &&
      (argv0[argv0l-2] == 'X' || argv0[argv0l-2] == 'x') &&
      (argv0[argv0l-1] == 'E' || argv0[argv0l-1] == 'e'))
    pfasm = "fasm.exe";
  else
    pfasm = "fasm";

  cmdsz += strlen(pfasm) + 1;
  if ((cmd =(char*) malloc(cmdsz)) == NULL)
    Cerror("Out of memory\n");

  strcpy(cmd, pfasm);
  strcat(cmd, " ");
  strcat(cmd, IntermidName);
  if (OutName)
  {
    strcat(cmd, " ");
    strcat(cmd, OutName);
  }

  res = system(cmd);

  if (res)
    Cerror("Failed command '%s'\n", cmd);
}

int mymain(int argc, char** argv)
{
  int i, InputFileCnt = 0;

  for (i = 1; i < argc; i++)
  {
    if (!strcmp(argv[i], "-f"))
    {
      if (i + 1 < argc)
      {
        argv[i++] = NULL;
        OutFormat = argv[i];
        argv[i] = NULL;
        if (strcmp(OutFormat, "bin") &&
            strcmp(OutFormat, "elf") &&
            strcmp(OutFormat, "asm"))
          Cerror("Invalid or unsupported output format '%s'\n", OutFormat);
        DontAssemble = !strcmp(OutFormat, "asm");
        continue;
      }
    }
    else if (!strcmp(argv[i], "-o"))
    {
      if (i + 1 < argc)
      {
        OutName = argv[++i];
        continue;
      }
    }

    if (argv[i][0] == '-')
      Cerror("Invalid or unsupported command line option '%s'\n", argv[i]);

    InName = argv[i];
    InputFileCnt++;
  }

  if (InputFileCnt != 1)
    Cerror("Too many or no inputs\n");

  if (!OutFormat)
    OutFormat = "bin";

  if (!OutName && DontAssemble)
    Cerror("Output name required\n");

  FlatBinary = !strcmp(OutFormat, "bin");

  convert();

  if (!DontAssemble)
  {
    fassemble(argv[0]);
    if (IntermidName)
      remove(IntermidName);
  }

  return 0;
}

#if defined( __cplusplus )
namespace
{
#endif

#define NO_ANNOTATIONS
#define CAN_COMPILE_32BIT
#include "msmlrc.c"

void InitC(void)
{

  int i;


  static unsigned char SyntaxStackInit[] =
  {
    tokVoid,
    tokInt,
    tokUnsigned,
    tokFloat,
    tokIdent,
    '[',
    tokNumUint,
    ']',
    tokChar
  };
  memcpy(SyntaxStack0, SyntaxStackInit, sizeof SyntaxStackInit);
  SyntaxStackCnt = sizeof SyntaxStackInit / sizeof SyntaxStackInit[0];

#ifdef __SMALLER_C__
#ifdef DETERMINE_VA_LIST
  DetermineVaListType();
#endif
#endif

#ifndef NO_FP
  {
    static float testFloat = -5915522.0f * 1.5f;
    static unsigned testUint = 0xCB076543;
    if (memcmp(&testFloat, &testUint, sizeof testFloat))
      error("IEEE 754 floating point required on host\n");
  }
  for (i = 0; (unsigned)i < sizeof FpFxnName / sizeof FpFxnName[0]; i++)
    AddIdent(FpFxnName[i]);
#endif

  DefineMacro("_WINDOWS", "1");
  OutputFormat = FormatSegmented;
  SizeOfWord = 4;
  WindowsStack = 1;
  UseBss = 0;
  UseLeadingUnderscores = 1;

  GenInitFinalize();

#ifndef NO_PREPROCESSOR

  DefineMacro("__SMALLER_C__", "0x0100");
  if (SizeOfWord == 2)
    DefineMacro("__SMALLER_C_16__", "");
#ifdef CAN_COMPILE_32BIT
  else if (SizeOfWord == 4)
    DefineMacro("__SMALLER_C_32__", "");
#endif
#ifdef CAN_COMPILE_32BIT
  if (OutputFormat == FormatSegHuge)
    DefineMacro("__HUGE__", "");
#endif
  if (CharIsSigned)
    DefineMacro("__SMALLER_C_SCHAR__", "");
  else
    DefineMacro("__SMALLER_C_UCHAR__", "");
#endif

  

  //InitAsm();
}


void CompileC(char* inName, char* OutName)
{
   CharQueueLen=0;
   FileCnt=0;

   strcpy(FileNames[0], inName);
   if ((Files[0] = fopen(FileNames[0], "r")) == NULL)
        errorFile(FileNames[0]);
   LineNos[0] = LineNo;
   LinePoss[0] = LinePos;
   FileCnt++;

   if ((OutFile = fopen(OutName, "w+")) == NULL)
        errorFile(OutName);

  if (!FileCnt)
    error("Input file not specified\n");
  if (!OutFile)
    error("Output file not specified\n");


  ShiftChar();

  puts2(FileHeader);


#ifndef NO_PPACK
  PragmaPackValue = SizeOfWord;
#endif

  ParseBlock(NULL, 0);

   GenFin();

   if (OutFile)
    fclose(OutFile);

}

void FinC(void)
{


#ifndef NO_ANNOTATIONS
  DumpSynDecls();
#endif
#ifndef NO_PREPROCESSOR
#ifndef NO_ANNOTATIONS
  DumpMacroTable();
#endif
#endif
#ifndef NO_ANNOTATIONS
  DumpIdentTable();
#endif

  GenStartCommentLine(); printf2("Next label number: %d\n", LabelCnt);

  if (warnings && warnCnt)
    printf("%d warnings\n", warnCnt);
  GenStartCommentLine(); printf2("Compilation succeeded.\n");


}

#if defined( __cplusplus )
}
#endif







void InitPP()
{
	fprintf(FP_TMP,"#ifndef _WINDOWS\n #define _WINDOWS 1\n #endif\n");
	fprintf(FP_TMP,"#ifndef __SMALLER_C__\n #define __SMALLER_C__ 0x0100\n #endif\n");
	fprintf(FP_TMP,"#ifndef __SMALLER_C_32__\n #define __SMALLER_C_32__\n #endif\n");
	fprintf(FP_TMP,"#ifndef __HUGE__\n #define __HUGE__\n #endif\n");
	fprintf(FP_TMP,"#ifndef __SMALLER_C_SCHAR__\n #define __SMALLER_C_SCHAR__\n #endif\n");
	fprintf(FP_TMP,"#ifndef __SMALLER_PP__\n #define __SMALLER_PP__ 1\n #endif\n");
}


void initSmalrc()
{
	InitC();
	
}


int  DoSmalrc(char* cNam, char* dst)
{
	strcpy(cAsm, BcxSplitPath(dst, FDRV | FPATH | FNAME));
	strcat(cAsm, ".__asm");
	
	int i, InputFileCnt = 0;
	OutFormat = "asm";
	DontAssemble = TRUE;
	OutName = dst;
	InName = cAsm;
	InputFileCnt++;;
	FlatBinary = true;
	
			CompileC(cNam, cAsm);
			//DeleteFile(cNam);
			convert();
			//DeleteFile(cAsm);
			return 1;
}
