%{
/*
*scan.l
*
*lex input file for pascal scanner
*
*extensions: two ways to spell "external" and "->"okfor"^".
*/

#include <stdio.h>
#include <string.h>


#define		ARRAY		1//OK
#define		DO			2//OK
#define		ELSE		3//OK
#define		END			4//OK
#define		FUNCTION	5//OK
#define		IF			6//OK
#define		NOT			7//OK
#define		OF			8//OK
#define		PBEGIN		9//OK
#define		PROCEDURE	10//OK
#define		PROGRAM		11//OK
#define		THEN		12//OK
#define		VAR			13//OK
#define		WHILE		14//OK
#define		IDENTIFIER	15//OK
#define		ASSIGNMENT	16//OK
#define		COLON		17//OK
#define		COMMA		18//OK
#define		INTEGER		19//OK
#define		DOT			20//OK
#define		DOTDOT		21//OK
#define		EQUAL		22//OK
#define		GE			23//OK
#define		GT			24//OK
#define		LBRAC		25//OK
#define		LE			26//OK
#define		LPAREN		27//OK
#define		LT			28//OK
#define		MINUS		29//OK
#define		PLUS		30//OK
#define		RBRAC		31//OK
#define		REALNUMBER	32//OK
#define		RPAREN		33//OK
#define		SEMICOLON	34//OK
#define		SLASH		35//OK
#define		STAR		36//OK
#define		NOTEQUAL	37//OK
#define		COMMENT		38//Moreadd
#define		SCIENTIFIC_EXP_REAL		39
#define		SCIENTIFIC_EXP_EXPON	40
#define		NUMBER		41
#define		INT_WORD	42
#define		REAL_WORD	43
#define		STRING_WORD	44
int line_no = 1;
%}

A[aA]
B[bB]
C[cC]
D[dD]
E[eE]
F[fF]
G[gG]
H[hH]
I[iI]
J[jJ]
K[kK]
L[lL]
M[mM]
N[nN]
O[oO]
P[pP]
Q[qQ]
R[rR]
S[sS]
T[tT]
U[uU]
V[vV]
W[wW]
X[xX]
Y[yY]
Z[zZ]

%%

{A}{R}{R}{A}{Y}			return(ARRAY);
{D}{O}						return(DO);
{E}{L}{S}{E}				return(ELSE);
{E}{N}{D}					return(END);
{F}{U}{N}{C}{T}{I}{O}{N}	return(FUNCTION);
{I}{F}						return(IF);
{N}{O}{T}					return(NOT);
{O}{F}						return(OF);
{B}{E}{G}{I}{N}				return(PBEGIN);
{P}{R}{O}{C}{E}{D}{U}{R}{E}	return(PROCEDURE);
{P}{R}{O}{G}{R}{A}{M}		return(PROGRAM);
{T}{H}{E}{N}				return(THEN);
{I}{N}{T}{E}{G}{E}{R}		return(INT_WORD);
{R}{E}{A}{L}				return(REAL_WORD);
{V}{A}{R}					return(VAR);
{W}{H}{I}{L}{E}				return(WHILE);
{S}{T}{R}{I}{N}{G}			return(STRING_WORD);

[a-zA-Z]([a-zA-Z0-9_]*)		return(IDENTIFIER);
[+-]?[0-9]+[a-zA-Z]([a-zA-Z0-9_])*		{
	printf("Lexical analyzer error at line %d: %s\n",line_no, yytext);
}
[+-]?[0-9]+("."[0-9]*)?{E}[+-]?[0-9]+	return(SCIENTIFIC_EXP_REAL);
":="			return(ASSIGNMENT);
":"				return(COLON);
","				return(COMMA);
[+-]?[0-9]+		return(INTEGER);
"."				return(DOT);
".."			return(DOTDOT);
"="				return(EQUAL);
"!="			return(NOTEQUAL);
">="			return(GE);
">"				return(GT);
"["				return(LBRAC);
"<="			return(LE);
"("				return(LPAREN);
"<"				return(LT);
"-"				return(MINUS);
"+"				return(PLUS);
"]"				return(RBRAC);
[+-]?[0-9]+"."[0-9]+		return(REALNUMBER);
")"				return(RPAREN);
";"				return(SEMICOLON);
"/"				return(SLASH);
"*"				return(STAR);
\"[^\"|^\n|^\r]*\"	return(NUMBER);
\"[^\"]*\"		{
	printf("Lexical analyzer error at line %d: %s\n",line_no, yytext);
}
\'[^\"|^\n|^\r]*\'			return(NUMBER);
"//".*			return(COMMENT);
(\/\*)(.|\n|\r)*(\*\/)	return(COMMENT);


[ \t\f] ;
\r ;
\n line_no++;
.	{
		printf("Lexical analyzer error at line%d: %s\n",line_no,yytext);
	}
%%

int main(int argc,char**argv)
{
	int i,tok,j;
	int more_length=0;
	char* filename=argv[1];
	yyin=fopen(filename,"r");
	for(i=0;1==1;i++)
	{
		tok = yylex();
		printf("%3d-th token(type:%d) on line %d: %s\n",i,tok,line_no,yytext);
	}
	printf("%3d-th token(type:%d) on line %d: %s\n",i,tok,line_no,yytext);
	return(0);
}/*end of main*/

int yywrap()
{
	fclose(yyin);
	exit(0);
}
