#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define KT 0
#define TT 1
#define IT 2
#define LT 3

typedef struct keyword_table
{
	char keyword[20];
	char token[5];
}keyword_table;

typedef struct terminal_table
{
	char terminal;
	char token[5];
}terminal_table;

typedef struct universal_symbol_table
{
	char lexeme[20];
	int table;
	char token[5];
	int location;
}universal_symbol_table;

typedef struct identifier_table
{
	char symbol[20];
	char token[5];
}identifier_table;

typedef struct literal_table
{
	char literal[20];
	char token[5];
}literal_table;

keyword_table kt[50];
int kt_size=0;
terminal_table tt[50];
int tt_size=0;
universal_symbol_table ust[50];
int ust_index=0;
identifier_table it[50];
int it_index=0;
literal_table lt[50];
int lt_index=0;

char *itoa(int num, char *str)
{
        if(str == NULL)
        {
                return NULL;
        }
        sprintf(str, "%d", num);
        return str;
}

int terminal_index(char t)
{
	int i=0;
	for(i=0; i<tt_size; i++)
	{
		if(tt[i].terminal == t)
			return i;
	}
	return -1;
}

int keyword_index(char *key)
{
	int i=0;
	for(i=0;i<kt_size;i++)
	{
		if(strcmp(key,kt[i].keyword)==0)
			return i;
	}
	return -1;
}

void add_universal_symbol(char *lexeme, char *token, int table, int location)
{
	strcpy(ust[ust_index].lexeme, lexeme);
	strcpy(ust[ust_index].token, token);
	ust[ust_index].table=table;
	ust[ust_index].location=location;
	ust_index++;
}

int add_literal(char *symbol)
{
		int i=0;
		for(i=0; i<lt_index; i++)
		{
			if(strcmp(lt[i].literal, symbol) == 0)
			{
				return i;
			}
		}
		strcpy(lt[lt_index].literal, symbol);
		lt[lt_index].token[0]='L';
		itoa(lt_index, lt[lt_index].token+1);
		lt_index++;
		return lt_index-1;
}

int add_identifier(char *symbol)
{
		int i=0;
		for(i=0; i<it_index; i++)
		{
			if(strcmp(it[i].symbol, symbol) == 0)
			{
				return i;
			}
		}
		strcpy(it[it_index].symbol, symbol);
		it[it_index].token[0]='I';
		itoa(it_index, it[it_index].token+1);
		it_index++;
		return it_index-1;
}

void construct_keyword_table()
{
	FILE *keyword_file = fopen("keyword_table.txt", "r");
	char *token = NULL;
	char line[100] = {'\0'};
	int i=0,j=0;
	while(fgets(line, 99, keyword_file) != NULL)
	{
		token = strtok(line, " ");
		if(token!=NULL)
		{
			for(j=0; j<strlen(token); j++)
			{
				if(token[j]=='\n')
				{
					token[j]='\0';
				}
			}
		}
		strcpy(kt[i].keyword, token);
		token = strtok(NULL, " ");
		if(token!=NULL)
		{
			for(j=0; j<strlen(token); j++)
			{
				if(token[j]=='\n')
				{
					token[j]='\0';
				}
			}
		}
		strcpy(kt[i].token, token);
		i++;
	}
	fclose(keyword_file);
	kt_size=i;
}

void print_keyword_table()
{
	int i=0;
	for(i=0; i<kt_size; i++)
	{
		printf("%s %s\n", kt[i].keyword, kt[i].token);
	}
}

void construct_terminal_table()
{
	FILE *terminal_file = fopen("terminal_table.txt", "r");
	char *token = NULL;
	char line[100] = {'\0'};
	int i=0, j=0;
	while(fgets(line, 99, terminal_file) != NULL)
	{
		token = strtok(line, " ");
		if(token!=NULL)
		{
			for(j=0; j<strlen(token); j++)
			{
				if(token[j]=='\n')
				{
					token[j]='\0';
				}
			}
		}
		tt[i].terminal=token[0];
		token = strtok(NULL, " ");
		if(token!=NULL)
		{
			for(j=0; j<strlen(token); j++)
			{
				if(token[j]=='\n')
				{
					token[j]='\0';
				}
			}
		}
		strcpy(tt[i].token, token);
		i++;
	}
	fclose(terminal_file);
	tt_size=i;
}

void print_terminal_table()
{
	int i=0;
	for(i=0; i<tt_size; i++)
	{
		printf("%c %s\n", tt[i].terminal, tt[i].token);
	}
}

void write_identifier_table()
{
	int i=0;
	FILE *identifier_file = fopen("identifier_table.txt", "w");
	for(i=0; i<it_index; i++)
	{
		fprintf(identifier_file, "%s %s\n", it[i].symbol, it[i].token);
	}
	fclose(identifier_file);
}

void write_literal_table()
{
	int i=0;
	FILE *literal_file = fopen("literal_table.txt", "w");
	for(i=0; i<lt_index; i++)
	{
		fprintf(literal_file, "%s %s\n", lt[i].literal, lt[i].token);
	}
	fclose(literal_file);
}

void write_universal_symbol_table()
{
	int i=0;
	FILE *us_file = fopen("universal_symbol_table.txt", "w");
	for(i=0; i<ust_index; i++)
	{
		fprintf(us_file, "%s %s %d %d\n", ust[i].lexeme, ust[i].token, ust[i].table, ust[i].location);
	}
	fclose(us_file);
}

int process_lexeme(char *temp, int temp_index, int line_number)
{
	int j=0, k=0, l=0, invalid_literal_flag=0, invalid_identifier_flag=0;
	char token[5];
	temp[temp_index]='\0';
	if(temp[0]=='\'' || temp[0]=='\"')
	{
		j = add_literal(temp);
		strcpy(token, lt[j].token);
		add_universal_symbol(temp, token, LT, j);
		temp_index=0;
	}
	else if(temp[0]>=48 && temp[0]<=57)
	{
		invalid_literal_flag=0;
		for(l=0; l<temp_index; l++)
		{
			if(temp[l]>='0' && temp[l]<='9')
			{
			}
			else
			{
				printf("Line: %d Invalid literal: %s\n", line_number, temp);
				invalid_literal_flag = 1;
			}
		}
		if(invalid_literal_flag == 0)
		{
			j = add_literal(temp);
			strcpy(token, lt[j].token);
			add_universal_symbol(temp, token, LT, j);
		}
		temp_index=0;
	}
	else
	{
		j = keyword_index(temp);
		if(j==-1)
		{
			invalid_identifier_flag=0;
			for(l=0; l<temp_index; l++)
			{
				if((temp[l]>='A' && temp[l]<='Z') || (temp[l]>='a' && temp[l]<='z') || (temp[l]=='_'))
				{
				}
				else
				{
					printf("Line: %d Invalid identifier: %s\n", line_number, temp);
					invalid_identifier_flag=1;
				}
			}
			if(invalid_identifier_flag==0)
			{
				j = add_identifier(temp);
				strcpy(token, it[j].token);
				add_universal_symbol(temp, token, IT, j);
			}
			temp_index=0;
		}
		else
		{
			strcpy(token, kt[j].token);
			add_universal_symbol(temp, token, KT, j);
			temp_index=0;
		}
	}
	return temp_index;
}

void scan_input(char *source_file)
{
	FILE *opcode_file = fopen(source_file, "r");
	char line[100] = {'\0'};
	char temp[100], token[5], trm[2];
	trm[1]='\0';
	int i=0, k=0, temp_index=0, line_number=0, invalid_identifier_flag=0, invalid_literal_flag=0;
	while(fgets(line, 99, opcode_file) != NULL)
	{
		line_number++;
		i=0;
		temp_index=0;
		while(line[i]!='\0')
		{
			if(line[i]=='\n')
			{
				i++;
			}
			else if(line[i]==' ' || line[i]=='\t')
			{
				if(temp_index!=0)
				{
					temp_index = process_lexeme(temp, temp_index, line_number);
				}
				i++;
			}
			else
			{
				k=terminal_index(line[i]);
				if(k==-1)
				{
					temp[temp_index]=line[i];
					temp_index++;
					i++;
				}
				else
				{
					if(temp_index!=0)
					{
						temp_index = process_lexeme(temp, temp_index, line_number);
					}
					trm[0]=line[i];
					strcpy(token, tt[k].token);
					add_universal_symbol(trm, token, TT, k);
					i++;
				}
			}
		}
	}
	if(temp_index!=0)
	{
		temp_index = process_lexeme(temp, temp_index, line_number);
	}
}

int main(int argc, char *argv[])
{
	FILE *fp;
	construct_keyword_table();
	construct_terminal_table();
	scan_input(argv[1]);
	write_identifier_table();
	write_literal_table();
	write_universal_symbol_table();
	return 0;
}