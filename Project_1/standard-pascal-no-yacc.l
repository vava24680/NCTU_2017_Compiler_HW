%{
/*
 * scan.l
 *
 * lex input file for pascal scanner
 *
 * extensions: two ways to spell "external" and "->" ok for "^".
 */

#include <stdio.h>
#include <string.h>
//#define   AND      1
#define   ARRAY      2 //OK
//#define   CASE      3
//#define   CONST      4
//#define   DIV      5
#define   DO      6 //OK
//#define   DOWNTO      7
#define   ELSE      8 //OK
#define   END      9 //OK

//#define   EXTERNAL      10
//#define   FOR      11
//#define   FORWARD      12
#define   FUNCTION      13//OK
//#define   GOTO      14
#define   IF      15 //OK
//#define   IN      16
//#define   LABEL      17
//#define   MOD      18
//#define   NIL      19
#define   NOT      20 //OK
#define   OF       21 //OK
//#define   OR      22
//#define   OTHERWISE      23
//#define   PACKED      24
#define   PBEGIN      25 //OK
//#define   PFILE      26
#define   PROCEDURE      27 //OK
#define   PROGRAM      28 //OK
//#define   RECORD      29
//#define   REPEAT      30
//#define   SET      31
#define   THEN      32 //OK
//#define   TO      33
//#define   TYPE      34
//#define   UNTIL      35
#define   VAR      36 //OK
#define   WHILE      37 //OK
//#define   WITH      38
#define   IDENTIFIER      39 //OK

#define   ASSIGNMENT      40 //OK
//#define   CHARACTER_STRING      41
#define   COLON      42 //OK
#define   COMMA      43 //OK
#define   INTEGER      44 //OK
#define   DOT      45 //OK
#define   DOTDOT      46 //OK
#define   EQUAL      47 //OK
#define   GE      48 //OK
#define   GT      49 //OK
#define   LBRAC      50 //OK
#define   LE      51 //OK
#define   LPAREN      52 //OK
#define   LT      53 //OK
#define   MINUS      54 //OK
//#define   NOTEQUAL      55
#define   PLUS      56 //OK
#define   RBRAC      57 //OK
#define   REALNUMBER      58 //OK
#define   RPAREN      59 //OK
#define   SEMICOLON      60 //OK
#define   SLASH      61 //OK
#define   STAR      62 //OK
//#define   STARSTAR      63
//#define   UPARROW      64


#define   notEQUAL    65 //OK
#define   COMMENT 66 //More add
#define   SCIENTIFIC_EXP_REAL 67
#define   SCIENTIFIC_EXP_EXPON 68
#define   NUMBER 69
#define   INT_WORD 70
#define   REAL_WORD 71
int line_no = 1;


%}

A [aA]
B [bB]
C [cC]
D [dD]
E [eE]
F [fF]
G [gG]
H [hH]
I [iI]
J [jJ]
K [kK]
L [lL]
M [mM]
N [nN]
O [oO]
P [pP]
Q [qQ]
R [rR]
S [sS]
T [tT]
U [uU]
V [vV]
W [wW]
X [xX]
Y [yY]
Z [zZ]
NQUOTE [^']
%%

{A}{R}{R}{A}{Y}              return(ARRAY);



{D}{O}                       return(DO);

{E}{L}{S}{E}                 return(ELSE);
{E}{N}{D}                    return(END);
{F}{U}{N}{C}{T}{I}{O}{N}     return(FUNCTION);

{I}{F}                       return(IF);




{N}{O}{T}                    return(NOT);
{O}{F}                       return(OF);



{B}{E}{G}{I}{N}              return(PBEGIN);

{P}{R}{O}{C}{E}{D}{U}{R}{E}  return(PROCEDURE);
{P}{R}{O}{G}{R}{A}{M}        return(PROGRAM);



{T}{H}{E}{N}                 return(THEN);
{I}{N}{T}{E}{G}{E}{R}   	 return(INT_WORD);
{R}{E}{A}{L}				 return(REAL_WORD);
{V}{A}{R}                    return(VAR);
{W}{H}{I}{L}{E}              return(WHILE);

[+-]?[0-9]+("."[0-9]*)?{E}[+-]?[0-9]+	return(SCIENTIFIC_EXP_REAL);
[a-zA-Z]([a-zA-Z0-9_])+       return(IDENTIFIER);

":="                         return(ASSIGNMENT);

":"                          return(COLON);
","                          return(COMMA);
[+-]?[0-9]+                       return(INTEGER);
"."                          return(DOT);
".."                         return(DOTDOT);
"="                          return(EQUAL);
"!="                         return(notEQUAL);
">="                         return(GE);
">"                          return(GT);
"["                          return(LBRAC);
"<="                         return(LE);
"("                          return(LPAREN);
"<"                          return(LT);
"-"                          return(MINUS);

"+"                          return(PLUS);
"]"                          return(RBRAC);

[+-]?[0-9]+"."[0-9]+              return(REALNUMBER);
")"                          return(RPAREN);
";"                          return(SEMICOLON);
"/"                          return(SLASH);
"*"                          return(STAR);
\"[^\"|^\n|^\r]*\" return(NUMBER);
\'.\' return(NUMBER);
"(*"   |
"{"    { register int c;
			while ((c = input()))
			{
				if (c == '}')
					break;
				else if (c == '*')
				{
					if ((c = input()) == ')')
						break;
					else
						unput (c);
				}
				else if (c == '\n')
					line_no++;
				else if (c == 0)
					fprintf (stderr, "Unexpected EOF inside comment at line %d\n",line_no);
			}
		}
"//".* {
	return (COMMENT);
}

(\/\*)(.|\n|\r)*(\*\/) {
	return (COMMENT);
}

[ \t\f]    ;


\r         ;
\n    line_no++;

.    {fprintf(stderr,"Lexical analyzer error at line %d : %s\n",line_no,yytext);}

%%

int main(int argc, char** argv)
{
	int i, tok,j;
	int more_length=0;
	yyin = fopen("test.txt", "r");
	for (i = 0; 1==1; i ++)
	{
		tok = yylex();
		if(tok==69 || tok==66)
		{
			//fprintf(stderr,"This is a string.  ");
			int len=strlen(yytext);
			for(j=0;j<len;j++)
			{
				if(yytext[j]=='\n' || yytext[j]=='\r')
				{
					more_length++;
				}
			}
		}
		//fprintf(stderr,"more_length : %d\n",more_length);
		fprintf(stderr, "%3d-th token(type:%d) on line %d : %s\n", i, tok,line_no, yytext);
		line_no+=more_length;
		more_length=0;
	}
	fprintf(stderr, "%3d-th token(type:%d) on line %d : %s\n", i, tok,line_no, yytext);
	return(0);
} /* end of main */

yywrap()
{
	fclose(yyin);
	exit(1);
}
