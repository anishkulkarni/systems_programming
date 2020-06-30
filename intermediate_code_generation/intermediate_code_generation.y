%{
	#include<stdio.h>
	#include<string.h>
	int yylex();
	void push();
	void push_eq();
	void push_le();
	void push_ge();
	void push_and();
	void push_or();
	void codegen();
	void codegen_2();
	void codegen_assign();
	void codegen_umin();
	void codegen_condition();
	void label_while();
	void label_condition();
	void label_end();
	void yyerror(char *);
	extern char *yytext;
%}

%token ID NUM WHILE EQ LE GE AND OR
%right AND OR
%right '='
%left '+' '-'
%left '*' '/'
%left UMINUS

%%

while_statement:	WHILE {label_while();} '(' condition ')' {label_condition();} '{' statement '}' {label_end();}
	;
	
statement:	while_statement
	|	expression ';' statement
	|	expression ';'
	;

condition:	'(' condition ')' AND {push_and();} '(' condition ')' {codegen();}
	|	'(' condition ')' OR {push_or();} '(' condition ')' {codegen();}
	|	expression '<' {push();} expression {codegen();}
	|	expression '>' {push();} expression {codegen();}
	|	expression EQ {push_eq();} expression {codegen_2();}
	|	expression LE {push_le();} expression {codegen_2();}
	|	expression GE {push_ge();} expression {codegen_2();}
	;
	
expression:	variable '=' {push();} expression {codegen_assign();}
	|	expression '+' {push();} expression {codegen();}
	|	expression '-' {push();} expression {codegen();}
	|	expression '*' {push();} expression {codegen();}
	|	expression '/'{push();} expression {codegen();}
	|	'(' expression ')'
	|	'-' {push();} expression {codegen_umin();} %prec UMINUS
	|	variable
	|	NUM {push();}
	;
	
variable:	ID {push();}
	;
	
%%

char st[100][10];
int top=0;
char token[3] = "t0";
int label_number=0;
char label_char = 'A' - 1;

void main()
{
	printf("Enter the expression : ");
	yyparse();
}

void push()
{
	strcpy(st[++top],yytext);
}

void push_eq()
{
	strcpy(st[++top],"==");
}

void push_le()
{
	strcpy(st[++top],"<=");
}

void push_ge()
{
	strcpy(st[++top],">=");
}

void push_and()
{
	strcpy(st[++top],"&&");
}

void push_or()
{
	strcpy(st[++top],"||");
}

void codegen()
{
	printf("%s = %s %s %s\n",token,st[top-2],st[top-1],st[top]);
	top-=2;
	strcpy(st[top],token);
	token[1] = token[1] + 1;
}

void codegen_2()
{
	printf("%s = %s %s %s %s\n",token, st[top-3], st[top-2], st[top-1], st[top]);
	top-=3;
	strcpy(st[top],token);
	token[1] = token[1] + 1;
}

void codegen_umin()
{
	printf("%s = -%s\n",token,st[top]);
	top--;
	strcpy(st[top],token);
	token[1] = token[1] + 1;
}

void codegen_assign()
{
	printf("%s = %s\n",st[top-2],st[top]);
	top-=2;
}

void codegen_condition()
{
	printf("%s = %s\n",st[top-2],st[top]);
	top-=2;
}

void label_while()
{
	label_char ++;
	label_number = 0;
	printf("%c%d: \n",label_char, label_number);
}


void label_condition()
{
	printf("%s = not %s\n",token,st[top]);
	printf("if %s goto %c%d\n",token, label_char, label_number+1);
	token[1] = token[1] + 1;
}

void label_end()
{
	printf("goto %c%d \n", label_char, label_number);
	//label_number++;
	printf("%c%d: \n", label_char, label_number+1);
	label_char--;
}
