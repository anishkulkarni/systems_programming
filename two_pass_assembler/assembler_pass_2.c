#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct opcode_table
{
    char mnemonic[20];
    int opcode;
    int type;
    int size;
}opcode_table;

typedef struct symbol_table
{
    char symbol[20];
    int address;
}symbol_table;

typedef struct literal_table
{
    char literal[10];
    int address;
}literal_table;

opcode_table ot[20];
symbol_table st[50];
literal_table lt[50];
int ot_size = 0;
int st_size = 0;
int lt_size = 0;

void create_opcode_table()
{
    FILE *opcode_file = fopen("opcode_table.txt", "r");
    char *token = NULL;
    char line[100] = {'\0'};
    int i=0;
    while(fgets(line, 99, opcode_file) != NULL)
    {
        token = strtok(line, " ");
        strcpy(ot[i].mnemonic, token);
        token = strtok(NULL, " ");
        ot[i].opcode = atoi(token);
        token = strtok(NULL, " ");
        ot[i].type = atoi(token);
        token = strtok(NULL, " ");
        ot[i].size = atoi(token);
        i++;
    }
    fclose(opcode_file);
    ot_size = i;
}

void create_symbol_table()
{
        FILE *opcode_file = fopen("symbol_table.txt", "r");
        char *token = NULL;
        char line[100] = {'\0'};
        int i=0;
        while(fgets(line, 99, opcode_file) != NULL)
        {
                token = strtok(line, " ");
                strcpy(st[i].symbol, token);
                token = strtok(NULL, " ");
                st[i].address = atoi(token);
                i++;
        }
        fclose(opcode_file);
        st_size = i;
}

void create_literal_table()
{
        FILE *opcode_file = fopen("literal_table.txt", "r");
        char *token = NULL;
        char line[100] = {'\0'};
        int i=0;
        while(fgets(line, 99, opcode_file) != NULL)
        {
                token = strtok(line, " ");
                strcpy(lt[i].literal, token);
                token = strtok(NULL, " ");
                lt[i].address = atoi(token);
                i++;
        }
        fclose(opcode_file);
        lt_size = i;
}

int is_imperative(char *token)
{
        if(token[1] == 'I' && token[2] == 'S')
        {
                return 1;
        }
        else
        {
                return 0;
        }
        
}

int is_printable_ad(char *token)
{
        if(token[1] == 'A' && token[2] == 'D')
        {
                if(token[4] == '2' || token[4] == '5')
                {
                        return 1;
                }
                else
                {
                        return 0;
                }   
        }
        else
        {
                return 0;
        }
        
}

int is_declarative(char *token)
{              
        if(token[1] == 'D' && token[2] == 'L')
        {
                return 1;       
        }
        else
        {
                return 0;
        }
}

int extract_value(char *token)
{
        int i=0;
        for(i=0; token[i]!=')'; i++)
        {
                if(token[i+1] == ')')
                {
                        token[i+1]='\0';
                        break;
                }
        }
        return atoi(token+4);
}

int get_operand(char *token)
{
        int i=0;
        for(i=0; token[i]!=')'; i++)
        {
                if(token[i+1] == ')')
                {
                        token[i+1]='\0';
                        break;
                }
        }
        //printf("\n%c  %s",token[1], token+3);
        if(token[1] == 'S')
        {
                return st[atoi(token+3)].address;
        }
        else if(token[1] == 'L')
        {
                //printf("\n%d", lt[atoi(token+3)].address);
                return lt[atoi(token+3)].address;
        }
}

void assemble(char *words[])
{
        FILE *output;
        output = fopen("output_final.txt", "a");
        fclose(output);
        int opcode=0, operand_1=0, operand_2=0, lc=0, value=0;
        if(strcmp(words[1], "-") == 0)
        {

        }
        else
        {
                if(is_imperative(words[1]))
                {
                        if(strcmp(words[2], "-")==0)
                        {
                                lc = atoi(words[0]);
                                opcode = extract_value(words[1]);
				if(strcmp(words[3], "-") == 0)
				{
					output = fopen("output_final.txt", "a");
					//fprintf(output, "%d %d %s %s\n", lc, opcode, words[2], words[3]);
					fprintf(output, "%d %d 0 0\n", lc, opcode);
					fclose(output);
				}
				else
				{
                                	operand_2 = get_operand(words[3]);
                                	output = fopen("output_final.txt", "a");
                                	//fprintf(output, "%d %d %s %s\n", lc, opcode, words[2], operand_2);
                                	fprintf(output, "%d %d 0 %d\n", lc, opcode, operand_2);
                                	fclose(output);
				}

                        }
                        else
                        {  
                                lc = atoi(words[0]);
                                opcode = extract_value(words[1]);
                                operand_1 = atoi(words[2]);
                                operand_2 = get_operand(words[3]);
                                output = fopen("output_final.txt", "a");
                                fprintf(output, "%d %d %d %d\n", lc, opcode, operand_1, operand_2);
                                fclose(output);
                        }
                        
                }
                else if(is_printable_ad(words[1]))
                {
                        lc = atoi(words[0]);
                        value = atoi(words[3]);
                        output = fopen("output_final.txt", "a");
                        //fprintf(output, "%d - - %d\n", lc, value);
                        fprintf(output, "%d 0 0 %d\n", lc, value);
                        fclose(output);
                }
                else if(is_declarative(words[1]))
                {
                        lc = atoi(words[0]);
                        value = atoi(words[3]);
                        output = fopen("output_final.txt", "a");
                        //fprintf(output, "%d - - %d\n", lc, value);
                        fprintf(output, "%d 0 0 %d\n", lc, value);
                        fclose(output);
                }
                
        }
        
}

int main()
{
        FILE *input_file, *output_file;
        input_file = fopen("output.txt", "r");
        //output_file = fopen(argv[2], "a");
        char *words[4] = {NULL};
        char line[100] = {'\0'};
        char *token = NULL;
        create_opcode_table();
        create_symbol_table();
        create_literal_table();
        int i = 0;
        //print_opcode_table();
        while(fgets(line, 99, input_file) != NULL)
        {
                token = strtok(line, " ");
                if(token != NULL)
                        for(i=0; token[i]!='\0'; i++)
                                if(token[i] == '\n')
                                        token[i] = '\0';
                words[0] = token;
                //printf("\t%s", token);
                token = strtok(NULL, " ");
                if(token != NULL)
                        for(i=0; token[i]!='\0'; i++)
                                if(token[i] == '\n')
                                        token[i] = '\0';
                //printf("\t%s", token);
                words[1] = token;
                token = strtok(NULL, " ");
                if(token != NULL)
                        for(i=0; token[i]!='\0'; i++)
                                if(token[i] == '\n')
                                        token[i] = '\0';
                //printf("\t%s", token);
                words[2] = token;
                token = strtok(NULL, " ");
                if(token != NULL)
                        for(i=0; token[i]!='\0'; i++)
                                if(token[i] == '\n')
                                        token[i] = '\0';
                //printf("\t%s\n", token);
                words[3] = token;
                assemble(words);
        }
        return 0;
}
