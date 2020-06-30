#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define IMPERATIVE 1
#define ASSEMBLER_DIRECTIVE 2
#define DECLARATIVE 3

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

typedef struct pool_table
{
    int literal_number;
}pool_table;

opcode_table ot[20];
symbol_table st[50];
literal_table lt[50];
pool_table pt[20];
int ot_size = 0;
int st_index = 0;
int lt_index = 0;
int pt_index = 0;
int location_counter = 0;

void write_symbol_table()
{
	FILE *file = fopen("symbol_table.txt", "w");
	int i=0;
	for(i=0;i<st_index;i++)
	{
		fprintf(file, "%s %d\n", st[i].symbol, st[i].address);
	}
	fclose(file);
}

void write_literal_table()
{
	FILE *file = fopen("literal_table.txt", "w");
	int i=0;
	for(i=0;i<lt_index;i++)
	{
		fprintf(file, "%s %d\n", lt[i].literal, lt[i].address);
	}
	fclose(file);
}

int is_stop(int index)
{
	if(index == 10)
		return 1;
	else
		return 0;
}

int is_arithmetic(int index)
{
	if(index>=0 && index <=7 && index!=6)
		return 1;
	else
		return 0;
	
}

int is_conditional(int index)
{
	if(index == 6)
		return 1;
	else
		return 0;	
}

int is_interactive(int index)
{
	if(index == 8 || index ==9)
		return 1;
	else
		return 0;
	
	
}

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

int is_end(char *token)
{
    if(strcmp(token, "end")==0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int is_equ(char *token)
{
    if(strcmp(token, "equ")==0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int is_start(char *token)
{
    if(strcmp(token, "start")==0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int is_ltorg(char *token)
{
    if(strcmp(token, "ltorg")==0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int is_origin(char *token)
{
    if(strcmp(token, "origin")==0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int literal_index(char *token)
{
    int i=0;
    for(i=pt_index; i<lt_index; i++)
    {
        if(strcmp(token, lt[i].literal) == 0)
        {
            return i;
        }
    }
    return -1;
}

void add_literal(char *token, int a)
{
    strcpy(lt[lt_index].literal, token);
    lt[lt_index].address = a;
    lt_index++;
}

int literal_value(char * token)
{
    int value = 0;
    int i=0, j=0;
    int length = strlen(token);
    char *copy = (char *)malloc((length-1)*sizeof(char));
    for(i=0;i<length;i++)
    {
	    if(token[i]!='\'')
	    {
		    copy[j]=token[i];
		    j++;
	    }
    }
    copy[j] = '\0';
    //printf("%s", copy);
    value = atoi(copy);
    free(copy);
    return value;
}

void print_opcode_table()
{
	int i=0;
	for(i=0; i<ot_size; i++)
	{
		printf("\nmnemonic: %s, opcode: %d, type: %d, size: %d", ot[i].mnemonic, ot[i].opcode, ot[i].type, ot[i].size);
	}
}

int register_index(char *token)
{
    if(strcmp(token,"areg") == 0)
		return 1;
	else if(strcmp(token,"breg") == 0)
		return 2;
	else if(strcmp(token,"creg") == 0)
		return 3;
	else if(strcmp(token,"dreg") == 0)
		return 4;
    else
        return -1;
}

int condition_index(char *token)
{
    if(strcmp(token,"lt") == 0)
		return 1;
	else if(strcmp(token,"le") == 0)
		return 2;
	else if(strcmp(token,"eq") == 0)
		return 3;
	else if(strcmp(token,"ge") == 0)
		return 4;
	else if(strcmp(token,"gt") == 0)
		return 5;
	else if(strcmp(token,"ne") == 0)
		return 6;
    else
        return -1;
}

int symbol_index(char *token)
{
    int i=0;
    for(i=0; i<st_index; i++)
    {
        if(strcmp(token, st[i].symbol) == 0)
        {
            return i;
        }
    }
    return -1;
}

void add_symbol(char *token, int a)
{
    strcpy(st[st_index].symbol, token);
    st[st_index].address = a;
    st_index++;
}

int mnemonic_index(char *token)
{
    int i = 0;
    for(i=0; i<ot_size; i++)
    {
        if(strcmp(token, ot[i].mnemonic) == 0)
        {
            return i;
        }
    }
    return -1;
}

FILE *assemble(char *words[])
{
    FILE* output;
    int index = 0;
    int current = 0;
    int operand_1 = 0;
    int operand_2 = 0;
    int variable_index = 0;
    int i=0;
    index = mnemonic_index(words[0]);
    if(index == -1)
    {
        //label
        variable_index = symbol_index(words[0]);
        if(variable_index == -1)
        {
            add_symbol(words[0], location_counter);
        }
        else
        {
            if(st[variable_index].address == -1)
            {
                st[variable_index].address = location_counter;
            }
            else
            {
                printf("\nMultiple Declaration error");
            }
            
        }
        index = mnemonic_index(words[1]);
        if(index == -1)
        {
            printf("Syntax error: Label not followed by mnemonic");
        }
        else
        {
            if(ot[index].type == IMPERATIVE)
            {
                if(is_arithmetic(index))
                {
                    if(words[2] == NULL)
                    {
                        printf("Syntax error: Insufficient operands");
                    }
                    else
                    {
                        if(words[3] == NULL)
                        {
                            printf("Syntax error: Insufficient operands");
                        }
                        else
                        {
                            operand_1  = register_index(words[2]);
                            if(operand_1 == -1)
                            {
                                printf("Syntax error: Wrong operand");
                            }
                            else
                            {
                                if(words[3][0] == '\'')
                                {
                                    operand_2 = literal_index(words[3]);
                                    if(operand_2 == -1)
                                    {
                                        add_literal(words[3], -1);
                                        operand_2 = lt_index -1;
                                    }
				    output = fopen("output.txt", "a");
                                    fprintf(output, "%d (IS,%d) %d (L,%d)\n", location_counter, ot[index].opcode, operand_1, operand_2);
				    fclose(output);
                                }
                                else
                                {
                                    operand_2 = symbol_index(words[3]);
                                    if(operand_2 == -1)
                                    {
                                        add_symbol(words[3], -1);
                                        operand_2 = st_index -1;
                                    }
				    output = fopen("output.txt", "a");
                                    fprintf(output, "%d (IS,%d) %d (S,%d)\n", location_counter, ot[index].opcode, operand_1, operand_2);
				    fclose(output);
                                }
                                location_counter += ot[index].size;
                            }
                        }
                        
                    }
                    
                }
                else if(is_conditional(index))
                {
                    if(words[2] == NULL)
                    {
                        printf("Syntax error: Insufficient operands");
                    }
                    else
                    {
                        if(words[3] == NULL)
                        {
                            printf("Syntax error: Insufficient operands");
                        }
                        else
                        {
                            operand_1 = condition_index(words[2]);
                            if(operand_1 == -1)
                            {
                                printf("Syntax Error: INcompatible operands");
                            }
                            else
                            {
                                operand_2 = symbol_index(words[3]);
                                if(operand_2 == -1)
                                {
                                    add_symbol(words[3], -1);
                                    operand_2 = st_index - 1;
                                }
				output = fopen("output.txt", "a");
                                fprintf(output, "%d (IS,%d) %d (S,%d)\n", location_counter, ot[index].opcode, operand_1, operand_2);
				fclose(output);
                                location_counter = location_counter+ot[index].size;
                                
                            }
                            
                        }
                        
                    }
                }
                else if(is_interactive(index))
                {
                    operand_1 = symbol_index(words[2]);
                    if(operand_1 == -1)
                    {
                        add_symbol(words[2], -1);
                        operand_1 = st_index - 1;
                    }
                    if(words[3]!=NULL)
                    {
                        printf("Syntax Error: INcompatible operands");
                    }
                    else
                    {
			
			output = fopen("output.txt", "a");
                        fprintf(output, "%d (IS,%d) - (S,%d)\n", location_counter, ot[index].opcode, operand_1);
			fclose(output);
                        location_counter = location_counter + ot[index].size;
                    }
                    
                }
		else if(is_stop(index))
		{
			output = fopen("output.txt", "a");
                        fprintf(output, "%d (IS,%d) - -\n", location_counter, ot[index].opcode);
			fclose(output);
			location_counter = location_counter + ot[index].size;
		}
            }
            else if(ot[index].type == DECLARATIVE)
            {
                    if(ot[index].opcode == 2)
                    {
                        if(words[2] == NULL)
                        {
                            printf("Syntax error: incompatible operaand types");
                        }
                        else
                        {
                            if(words[3] == NULL)
                            {
                                variable_index = symbol_index(words[0]);
                                location_counter += atoi(words[2]);
                            }
                            else
                            {
                                printf("Syntax error: Incompatible operand count");
                            }
                            
                        }
                        
                    }
                    else if(ot[index].opcode == 1)
                    {
                        if(words[2] == NULL)
                        {
                            printf("Syntax error: incompatible operand types");
                        }
                        else
                        {
                            if(words[3] == NULL)
                            {  
                                operand_1 = literal_value(words[2]);
				                output = fopen("output.txt", "a");
                                fprintf(output, "%d (DL,%d) - %d\n", location_counter, ot[index].opcode, operand_1);
				                fclose(output);
                                location_counter += 1;
                            }
                            else
                            {
                                printf("Syntax error: invalid operand count");
                            }
                            
                        }
                    }
            }
            else if(ot[index].type == ASSEMBLER_DIRECTIVE)
            {
                if(is_end(words[1]))
                {
                    for(i=pt_index; i<lt_index; i++)
                    {
                        lt[i].address = location_counter;
			output = fopen("output.txt", "a");
                        fprintf(output, "%d (AD,%d) - %d\n", location_counter, ot[index].opcode, literal_value(lt[i].literal));
			fclose(output);
                        location_counter++;
                    }
                    pt_index = lt_index;
                }
                else if(is_start(words[1]))
                {
                    if(words[2] == NULL && words[3] == NULL)
                    {
                        location_counter = 0;
                    }
                    else
                    {
                        location_counter = atoi(words[2]);
                    }
		    output = fopen("output.txt", "a");
                    fprintf(output, "- (AD,%d) - (C,%d)\n", ot[index].opcode, location_counter);
		    fclose(output);
                }
                else if(is_equ(words[1]))
                {
                    variable_index = symbol_index(words[0]);
                    i = symbol_index(words[2]);
                    st[variable_index].address = st[i].address;
                }
                else if(is_ltorg(words[1]))
                {
                    for(i=pt_index; i<lt_index; i++)
                    {
                        lt[i].address = location_counter;
			            output = fopen("output.txt", "a");
                        fprintf(output, "%d (AD,%d) - %d\n", location_counter, ot[index].opcode, literal_value(lt[i].literal));
			            fclose(output);
                        location_counter++;
                    }
                    pt_index = lt_index;
                }
                else if(is_origin(words[1]))
                {
                    if(words[2] == NULL)
                    {
                        printf("Syntax error");
                    }
                    else
                    {
                        location_counter = atoi(words[2]);
                    }
                    
                }
            }
		}
    }
    else
    {
        //mnemonic
	if(ot[index].type == IMPERATIVE)
	{
	if(is_arithmetic(index))
	{
		if(words[1] == NULL)
		{
		printf("Syntax error: Insufficient operands");
		}
		else
		{
		if(words[2] == NULL)
		{
			printf("Syntax error: Insufficient operands");
		}
		else
		{
			operand_1  = register_index(words[1]);
			if(operand_1 == -1)
			{
			printf("Syntax error: Wrong operand");
			}
			else
			{
			if(words[2][0] == '\'')
			{
				operand_2 = literal_index(words[2]);
				if(operand_2 == -1)
				{
				add_literal(words[2], -1);
				operand_2 = lt_index -1;
				}
				output = fopen("output.txt", "a");
				fprintf(output, "%d (IS,%d) %d (L,%d)\n", location_counter, ot[index].opcode, operand_1, operand_2);
				fclose(output);
			}
			else
			{
				operand_2 = symbol_index(words[2]);
				if(operand_2 == -1)
				{
				add_symbol(words[2], -1);
				operand_2 = st_index -1;
				}
				output = fopen("output.txt", "a");
				fprintf(output, "%d (IS,%d) %d (S,%d)\n", location_counter, ot[index].opcode, operand_1, operand_2);
				fclose(output);
			}
			location_counter += ot[index].size;
			}
		}
		
		}
		
	}
	else if(is_conditional(index))
	{
		if(words[1] == NULL)
		{
		printf("Syntax error: Insufficient operands");
		}
		else
		{
		if(words[2] == NULL)
		{
			printf("Syntax error: Insufficient operands");
		}
		else
		{
			operand_1 = condition_index(words[1]);
			if(operand_1 == -1)
			{
			printf("Syntax Error: INcompatible operands");
			}
			else
			{
			operand_2 = symbol_index(words[2]);
			if(operand_2 == -1)
			{
				add_symbol(words[2], -1);
				operand_2 = st_index - 1;
			}
			output = fopen("output.txt", "a");
			fprintf(output, "%d (IS,%d) %d (S,%d)\n", location_counter, ot[index].opcode, operand_1, operand_2);
			fclose(output);
			location_counter = location_counter+ot[index].size;
			
			}
			
		}
		
		}
	}
	else if(is_interactive(index))
	{
		operand_1 = symbol_index(words[1]);
		if(operand_1 == -1)
		{
		add_symbol(words[1], -1);
		operand_1 = st_index - 1;
		}
		if(words[2]!=NULL)
		{
		printf("Syntax Error: INcompatible operands");
		}
		else
		{
		output = fopen("output.txt", "a");
		fprintf(output, "%d (IS,%d) - (S,%d)\n", location_counter, ot[index].opcode, operand_1);
		fclose(output);
		location_counter = location_counter + ot[index].size;
		}
		
	}
	else if(is_stop(index))
	{
		output = fopen("output.txt", "a");
                fprintf(output, "%d (IS,%d) - -\n", location_counter, ot[index].opcode);
		fclose(output);
		location_counter = location_counter + ot[index].size;
	}
	}
	else if(ot[index].type == ASSEMBLER_DIRECTIVE)
	{
	if(is_end(words[0]))
	{
		for(i=pt_index; i<lt_index; i++)
		{
		lt[i].address = location_counter;
		output = fopen("output.txt", "a");
		fprintf(output, "%d (AD,%d) - %d\n", location_counter, ot[index].opcode, literal_value(lt[i].literal));
		fclose(output);
		location_counter++;
		}
		pt_index = lt_index;
	}
	else if(is_start(words[0]))
	{
		if(words[1] == NULL && words[2] == NULL)
		{
		location_counter = 0;
		}
		else
		{
		location_counter = atoi(words[1]);
		}
		output = fopen("output.txt", "a");
		fprintf(output, "- (AD,%d) - (C,%d)\n", ot[index].opcode, location_counter);
		fclose(output);
	}
	else if(is_ltorg(words[0]))
	{
		for(i=pt_index; i<lt_index; i++)
		{
		lt[i].address = location_counter;
		output = fopen("output.txt", "a");
		fprintf(output, "%d (AD,%d) - %d\n", location_counter, ot[index].opcode, literal_value(lt[i].literal));
		fclose(output);
		location_counter++;
		}
		pt_index = lt_index;
	}
	else if(is_origin(words[0]))
	{
		if(words[1] == NULL)
		{
		printf("Syntax error");
		}
		else
		{
		location_counter = atoi(words[1]);
		}
		
	}
    }
		}    
	return output;
}	

int main(int argc, char *argv[])
{
    FILE *input_file, *output_file;
    input_file = fopen("input.txt", "r");
    //output_file = fopen(argv[2], "a");
    char *words[4] = {NULL};
    char line[100] = {'\0'};
    char *token = NULL;
    create_opcode_table();
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
        output_file = assemble(words);
    }
    write_symbol_table();
    write_literal_table();
    return 0;
}
