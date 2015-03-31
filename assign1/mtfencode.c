/*
 * By: Tyson Bulmer 
 * February 2015
 * SENG265 mtfencoder
 *
 * This "move to front" encoder makes compressed files using ascii
 * character manipulations.
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENGTH 21
#define MAX_NUMBER_OF_WORDS 120
#define INPUT_LINE_LENGTH 81
#define MAX_NUMER_OF_LINES 100

char dict[MAX_NUMBER_OF_WORDS][MAX_WORD_LENGTH];
int count = 0;

void add_to_dict(char *in, FILE *put){
	int i = 0;
	int ret = 0;
	char *ch;
	char temp[MAX_WORD_LENGTH];
	/*if sent in null do nothing*/
	if(!(*in)) return;
	/*if very first word enter it*/
	if(count == 0){
		strcpy(dict[0], in);
		fputc(129, put);
		ch = in;
		while(*ch){
			fputc(*ch, put);
			ch++;
		}
		count++;
		return;
	/* if the first word on the list is the word*/
	}else{
		while(i < count){
			ret = strcmp(dict[i], in);
			if(ret == 0){
				fputc(i+129, put);
				if((i+1) == 1){
				}else{
					while(i > 0){
						strcpy(temp, dict[i]);
						strcpy(dict[i], dict[i-1]);
						strcpy(dict[i-1], temp);
						i--;
					}
				}
				return;
			}else{
				i++;
			}
		}
		strcpy(dict[i], in);
		count++;
		ch = dict[i];
		fputc(i+129, put);
		while(*ch){
			fputc(*ch, put);
			ch++;
		}
		while(i > 0){
			strcpy(temp, dict[i]);
			strcpy(dict[i], dict[i-1]);
			strcpy(dict[i-1], temp);
                        i--;
                }
	}
	return;
}

int main(int argc, char **argv){
	char file_name [15];
	int file_name_len;
	char line[INPUT_LINE_LENGTH];
	int totalcount = 0;
	char *t;
	char *pos;
	char in_string[MAX_WORD_LENGTH];
	FILE *data;
	FILE *out; 
	data = fopen(argv[1], "r"); 
	/*get file to read from*/
	if(data == NULL){
			printf("file could not be opened\n");
			exit(EXIT_FAILURE);
		}
			printf("file opened successfully!\n");
	/*change filename to .mtf*/ 
	file_name_len = strlen(argv[1]);
	strncpy(file_name, argv[1], (file_name_len - 3));
	strcat(file_name, "mtf");
	/*make out file with new name write with binary*/ 
	out = fopen(file_name, "wb");
        if(out == NULL){
                        printf("file could not be created\n");
                        exit(EXIT_FAILURE);
                }
                        printf("file created successfully! With name %s\n", file_name);
	/*add the magic numbers*/
	fputc(0xfa, out);
	fputc(0xce, out);
	fputc(0xfa, out);
	fputc(0xde, out);
	/*get line, tokenize, add to dictionanry*/ 
	while(fgets(line, INPUT_LINE_LENGTH, data) != NULL && totalcount < MAX_NUMER_OF_LINES){
		t = strtok(line, " ");
		while (t && count < MAX_NUMBER_OF_WORDS){		
			if ((pos=strchr(t, '\n')) != NULL)
			*pos = '\0';
			strcpy(in_string, t);
			add_to_dict(in_string, out);
			t = strtok(NULL , " ");	
		}	
		fputc('\n', out);
		totalcount++;
	}	
	/*close data streams*/
	fclose(data);
	fclose(out);
	
	return EXIT_SUCCESS;
}
