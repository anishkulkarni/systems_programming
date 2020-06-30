%{
	#include "y.tab.h"
%}

%%

"while"	return WHILE;

[a-zA-Z]([a-zA-Z]|[0-9])*	return ID;

[0-9]+	{
		yylval=atoi(yytext);
		return NUM;
	}

"==" return EQ;

"<=" return LE;

">=" return GE;

"&&" return AND;

"||" return OR;

[ \t]	;

\n	return 0;

.	return yytext[0];

%%
