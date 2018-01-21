%option noyywrap
%option yylineno
%{
/*
 * scan.l
 *
 * lex input file for pascal scanner
 *
 * extensions: two ways to spell "external" and "->" ok for "^".
 */
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "node.h"
#include "parser.tab.h"
using namespace std;
#define   ARRAY   258 //OK
#define   DO      259 //OK
#define   ELSE    260 //OK
#define   END     261  //OK
#define   FUNCTION 262//OK
#define   IF       263 //OK
#define   NOT      264 //OK
#define   OF       265 //OK
#define   PBEGIN      266 //OK
#define   PROCEDURE   267 //OK
#define   PROGRAM     268 //OK
#define   THEN        269 //OK
#define   VAR        270 //OK
#define   WHILE      271 //OK
#define   IDENTIFIER 272 //OK
#define   ASSIGNMENT 273 //OK
#define   COLON      274 //OK
#define   COMMA      275 //OK
#define   INTEGER    276 //OK
#define   DOT        277 //OK
#define   DOTDOT     278 //OK
#define   EQUAL      279 //OK
#define   GE         280 //OK
#define   GT         281 //OK
#define   LBRAC      282 //OK
#define   LE         283 //OK
#define   LPAREN     284 //OK
#define   LT         285 //OK
#define   MINUS      286 //OK
#define   PLUS       287 //OK
#define   RBRAC      288 //OK
#define   REALNUMBER 289 //OK
#define   RPAREN     290 //OK
#define   SEMICOLON  291 //OK
#define   SLASH      292 //OK
#define   STAR       293 //OK
#define   NOTEQUAL   294 //OK
#define   SCIENTIFIC_EXP  295
#define   SCIENTIFIC_EXP_EXPON 296
#define   NUMBER     297
#define   INT_WORD   298
#define   REAL_WORD  299
#define   STRING_WORD     300
#define		PRINT		301
#define		WRITE		302
extern "C"
{
	int yylex(void);
}
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

%%
{A}{R}{R}{A}{Y}			{
										printf("Shift ARRAY\n");
										return(ARRAY);
									}
{D}{O}					{
								printf("Shift DO\n");
								return(DO);
							}
{E}{L}{S}{E}				{
										printf("Shift ELSE\n");
										return(ELSE);
									}
{E}{N}{D}				{
									printf("Shift END\n");
									return(END);
								}
{F}{U}{N}{C}{T}{I}{O}{N}		{
													printf("Shift FUNCTION\n");
													return(FUNCTION);
												}
{I}{F}					{printf("Shift IF\n");return(IF);}
{N}{O}{T}				{printf("Shift NOT\n");return(NOT);}
{O}{F}					{printf("Shift OF\n");return(OF);}
{B}{E}{G}{I}{N}				{printf("Shift BEGIN\n");return(PBEGIN);}
{P}{R}{O}{C}{E}{D}{U}{R}{E}		{printf("Shift PROCEDURE\n");return(PROCEDURE);}
{P}{R}{O}{G}{R}{A}{M}			{printf("Shift PROGRAM\n");return(PROGRAM);}
{T}{H}{E}{N}				{printf("Shift THEN\n");return(THEN);}
{I}{N}{T}{E}{G}{E}{R}			{printf("Shift INT_WORD\n");return(INT_WORD);}
{R}{E}{A}{L}				{printf("Shift REAL\n");return(REAL_WORD);}
{V}{A}{R}					{printf("Shift VAR\n");return(VAR);}
{W}{H}{I}{L}{E}				{printf("Shift WHILE\n");return(WHILE);}
{S}{T}{R}{I}{N}{G}			{printf("Shift STRING_WORD\n");return(STRING_WORD);}
{P}{R}{I}{N}{T}				{printf("Shift PRINT\n");return(PRINT);}
{W}{R}{I}{T}{E}				{printf("Shift WRITE\n");return(WRITE);}
[+-]?[0-9]+("."[0-9]*)?{E}[+-]?[0-9]+	{
										printf("Shift SCIENTIFIC_EXP\n");
											double realnumber;
											char key[]="eE";
											char* target = strpbrk(yytext, key);
											char* real_part = (char *)malloc(sizeof(char) * (target-yytext)+1);
											strncpy(real_part, yytext, (target-yytext));
											real_part[target-yytext] = '\0';
											realnumber = atof(real_part);

											double exponnubmer;
											char* expon_part = (char *)malloc(sizeof(char) * strlen(yytext)-strlen(real_part));
											for(unsigned int i = 0; i< (strlen(yytext) - strlen(real_part)); i++)
											{
												expon_part[i] = yytext[i+strlen(real_part)+1];
											}
											expon_part[strlen(yytext) - strlen(real_part)]='\0';
											if(expon_part[0]=='+' || expon_part[0]=='-')
											{
												char temp = expon_part[0];
												for(unsigned int i=0; i < (strlen(expon_part)-1); i++)
												{
													expon_part[i] = expon_part[i+1];
												}
												expon_part[strlen(expon_part)-1]='\0';
												exponnubmer = atof(expon_part);
												if(temp=='-')
													exponnubmer*=(-1);
											}
											else
											{
												exponnubmer = atof(expon_part);
											}
										yylval.real_value = (realnumber * pow((double)10,exponnubmer));
										free(real_part);
										free(expon_part);
										return(SCIENTIFIC_EXP);
									}
[a-zA-Z]([a-zA-Z0-9_]*)		{
								printf("Shift IDENTIFIER\n");
								yylval.id_struct.id = (char *)malloc(sizeof(char)*strlen(yytext)+1);
								memcpy(yylval.id_struct.id, yytext, strlen(yytext));
								yylval.id_struct.id[strlen(yytext)] = '\0';
								yylval.id_struct.line_no = yylineno;
								cout << "LINE NO : " << yylineno << endl;
								return(IDENTIFIER);
							}

":="						{printf("Shift ASSIGNMENT\n");return(ASSIGNMENT);}
":"							{printf("Shift COLON\n");return(COLON);}
","							{printf("Shift COMMA\n");return(COMMA);}
[0-9]+						{
								printf("Shift INTEGER\n");
								yylval.integer_value = atoi(yytext);
								return(INTEGER);
							}
"."							{printf("Shift DOT\n");return(DOT);}
".."						{printf("Shift DOTDOT\n");return(DOTDOT);}
"="							{printf("Shift EQUAL\n");return(EQUAL);}
"!="						{printf("Shift NOTEQUAL\n");return(NOTEQUAL);}
">="						{printf("Shift GE\n");return(GE);}
">"							{printf("Shift GT\n");return(GT);}
"["							{printf("Shift LBRAC\n");return(LBRAC);}
"<="						{printf("Shift LE\n");return(LE);}
"("							{printf("Shift LPAREN\n");return(LPAREN);}
"<"							{printf("Shift LI\n");return(LT);}
"-"							{printf("Shift MINUS\n");return(MINUS);}
"+"							{printf("Shift PLUS\n");return(PLUS);}
"]"							{printf("Shift RBRAC\n");return(RBRAC);}
[0-9]+"."[0-9]+			{
								printf("Shift REALNUMBER\n");
								yylval.integer_value = atof(yytext);
								return(REALNUMBER);
							}
")"							{printf("Shift RPAREN\n");return(RPAREN);}
";"							{printf("Shift SEMICOLON\n");return(SEMICOLON);}
"/"							{printf("Shift SLASH\n");return(SLASH);}
"*"							{printf("Shift STAR\n");return(STAR);}
\"[^\"|^\n|^\r]*\"			{
								printf("Shift STRING(NODE type is actually NUMBER)\n");
								yylval.id_struct.id = (char *)malloc(sizeof(char)*strlen(yytext)+1);
								memcpy(yylval.id_struct.id, yytext, strlen(yytext));
								yylval.id_struct.id[strlen(yytext)] = '\0';
								yylval.id_struct.line_no = yylineno;
								return(NUMBER);
							}
\"[^\"]*\"
\'[^\"|^\n|^\r]*\'			{
								printf("Shift STRING(NODE type is actually NUMBER)\n");
								yylval.id_struct.id = (char *)malloc(sizeof(char)*strlen(yytext)+1);
								memcpy(yylval.id_struct.id, yytext, strlen(yytext));
								yylval.id_struct.id[strlen(yytext)] = '\0';
								yylval.id_struct.line_no = yylineno;
								return(NUMBER);
							}

"//".*
[ \t\f]
\r
\n
.    {fprintf(stderr,"Lexical analyzer error at line %d : %s\n",yylineno,yytext);}
%%
