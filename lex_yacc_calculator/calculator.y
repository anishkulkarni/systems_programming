%{
	#include<stdio.h>
	int flag=0; 
%} 
  
%token NUMBER
  
%left '+' '-'
  
%left '*' '/' '%'
  
%left '(' ')'

%% 
  
ArithmeticExpression:	E{
				printf("result -> %d", $$);
				return 0;
			}; 
E:E'+'E {$$=$1+$3;} 
  
|E'-'E {$$=$1-$3;} 
  
|E'*'E {$$=$1*$3;} 
  
|E'/'E {$$=$1/$3;} 
 
|E'%'E {$$=$1%$3;} 
  
|'('E')' {$$=$2;} 
  
| NUMBER {$$=$1;} 
  
; 
  
%% 

void main() 
{
	printf("\nExpression can include '+', '-', '*', '/', '%', '(' and ')'");
	printf("\n\nexpression -> ");
	yyparse();
	if(flag==0)
		printf("\n\nExpression Valid\n"); 
} 
  
void yyerror() 
{
	printf("\nExpression Invalid\n");
	flag=1; 
} 
