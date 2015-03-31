#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mtfcoding.h"



//########################### Default init global data #######
int count = 0;

typedef struct Word Word;
struct Word
{
	char *val;
};


struct Dict{	
	int nval;
	int max;
	Word *word;
} dict;

enum { DICTINIT = 1, DICTGROW = 2} ;

char magic_number1[] = {0xFA, 0xCE, 0xFA, 0xDE};
char magic_number2[] = {0xFA, 0xCE, 0xFA, 0xDF};





//######################################### 
Word *new_word(char *name)
{
	Word *temp;
	temp = (Word *)malloc(sizeof(Word));
	if (temp == NULL) {
		fprintf(stderr, "Error mallocing a Word");
		exit(1);
	}
	/* temp->name === (*temp).name */
	temp->val = (char *)malloc((strlen(name)+1) * sizeof(char));
	if (temp->val == NULL) {
		fprintf(stderr, "Error mallocing a memory for string");
		exit(1);
	}
	strncpy(temp->val, name, strlen(name)+1);
	return temp;
} 


//####################Debug purpose function####

void print_dict(){
	int g;
	for(g = 0; g < dict.nval; g++){
		printf("%s ", dict.word[g].val);
	}
	printf("\n");
	return;
}

void progress(char* in, int g, FILE *log){
	fprintf(log, "%s: %d\n", in, g);
}


//##########################Delete name functions######

int delname (Word *name)
{
 int i;
	 for (i = 0; i < dict.nval; i++) {
		 if (strcmp(dict.word[i].val, name->val) == 0) {
		 	memmove(dict.word + i, dict.word + i + 1, (dict.nval-(i+1)) * sizeof(Word));
		 	dict.nval--;
		 return i;
		 }
	 }
 return -1;
}

int delname_decode(Word *name)
{
 int i;
	 for (i = 0; i < dict.nval; i++) {
		 if (strcmp(dict.word[i].val, name->val) == 0) {
		 	memmove(dict.word + i, dict.word + i + 1, (dict.nval-(i+1)) * sizeof(Word));
		 	dict.nval--;
		 	count--;
		 return i;
		 }
	 }
 return -1;
}

//############main dictionary functions########

void add_word_to_front(Word newname)
{
 	Word *nvp;
 	if (dict.word == NULL) { /* first use of array */
 		dict.word = (Word *) malloc(DICTINIT * sizeof(Word));
 		if (dict.word == NULL) { exit(1); }
 		dict.max = DICTINIT;
 		dict.nval = 0;
 	} else if (dict.nval >= dict.max) {
 		nvp = (Word *) realloc(dict.word, (DICTGROW * dict.max) * sizeof(Word));
		if (nvp == NULL) { exit(1); }
		dict.max = DICTGROW * dict.max;
		dict.word = nvp;
	}
	dict.word[dict.nval] = newname;
	dict.nval++;
	return;
} 

int is_in_dict(char* word){
	int i;
	 for (i = 0; i < dict.nval; i++) {
		 if (strcmp(dict.word[i].val, word) == 0) {
		 	return dict.nval - i - 1;
		 }
	 }
 return -1;
}

void int_to_enval(int i, FILE* out){
	int mid;
	int end;
	if(i < 120){
		i += 129;
		fputc(i, out);
	}else if(i < 375){
		fputc(249, out);
		fputc(i - 120, out);
	}else if(i < 65912){
		fputc(250, out);
		mid = ((i - 375)/256);
		fputc(mid, out);
		end = ((i-375)%256);
		fputc(end, out);
	}
	return;
}

void add_to_dict(char *in, FILE* out){
	
	Word* temp = new_word(in);
	int pos;
	if(!(*in)) return;
	if (count == 0){
		add_word_to_front(*temp);
		count +=1;
		fputc(129, out);
		fprintf(out, "%s", in);
	}else if((pos = is_in_dict(in)) == -1){
		add_word_to_front(*temp);
		int_to_enval(count, out);
		fprintf(out, "%s", in);
		count +=1;
	}else{
		delname(temp);
		int_to_enval(pos, out);
		add_word_to_front(*temp);
	}
	return;
}

//##########sub-encode###########

void sub_encode(char* line, FILE *output){
	char* pos;
	char* t;
	t = strtok(line, " ");
	while (t) {
		if((pos=strchr(t,'\n')) != NULL)
			*pos = '\0';
			add_to_dict(t, output);
			t = strtok(NULL, " ");
		}
		fputc('\n', output);
	
	return;
}

//########################Encode main############

int encode(FILE *input, FILE *output) {
	fputc(0xfa, output);
	fputc(0xce, output);
	fputc(0xfa, output);
	fputc(0xdf, output);
	FILE* log;
	log = fopen("mtfcoding.log", "a");
	char* line = NULL;
	size_t linesize = 0;
	ssize_t linelen;
	int linecount = 1;
	while((linelen = getline(&line, &linesize, input)) > 0){
	progress("Encoding line", linecount, log);
	sub_encode(line, output);
	linecount += 1;
	free(line);
	line = NULL;
	}    
	free(dict.word);
	fprintf(log, "%s: %d\n", "Number of unique words", dict.nval);
	fclose(log);	
	return 0;
}


//############## magic number checker###########

void check_magic(FILE* temp){
	char *c1, *c2, in_c;

	if(temp != NULL){
		for (c1 = magic_number1, c2 = magic_number2; *c1 && *c2; c1++, c2++){
			in_c = fgetc(temp);
			if((*c1 != in_c) && (*c2 != in_c)){
				fprintf(stderr, "%s\n","Error: input file is not mtf");
				exit(3);
			}
		}
	}
	return;
}

int enval_to_int(int in_char, FILE* input){
	int k;
	int g;
	if(in_char < 249){
		return in_char - 129;
	}
	else if(in_char == 249){
		k = fgetc(input);
		return k + 120;
	}else if(in_char == 250){
		k = fgetc(input);
		g = fgetc(input);
		return (((k*256)+376) + g-1);
	}else{
		fprintf(stderr, "%s\n","Error: read num wrong");
		exit(3);
	}
}

void add_to_dict_decode(char *in, FILE* out){
	
	Word* temp = new_word(in);
	int pos;
		add_word_to_front(*temp);
		count +=1;
		fprintf(out, "%s", in);
	return;
}
//#####################Decode main##########

int decode(FILE *input, FILE *output) {
	int in_char;
	int tester;
	int pos;
	char* copy;
	char temp[200];
	char *c;
	Word* hold;
	FILE *log;
	log = fopen("mtfcoding.log", "a");
	count = 0;
	int linecount = 1;
	check_magic(input);
	in_char = fgetc(input);
	while(in_char != EOF){
		if(in_char > 128){
			c = temp;
			pos = enval_to_int(in_char, input);
			if(pos >= count){
				in_char = fgetc(input);
				while(in_char < 128 && in_char != '\n'){
					*c = (char)in_char;
					c++;
					in_char = fgetc(input);
				}
				*c = '\0';
				ungetc(in_char, input);
				add_to_dict_decode(temp, output);
				tester = fgetc(input);
				if(tester != 10){
					fputc(' ', output);
					ungetc((char)tester, input);
				}else{
					ungetc((char)tester, input);
				}
			}else if(pos < count){
				strcpy(temp, dict.word[dict.nval - pos -1].val);
				delname_decode(&(dict.word[dict.nval - pos -1]));
				add_to_dict_decode(temp, output);
				tester = fgetc(input);
				if(tester != 10){
					fputc(' ', output);
					ungetc((char)tester, input);
				}else{
					ungetc((char)tester, input);
				}
			}
		}else{
			fputc('\n', output);
			progress("Decoded line",linecount, log);
			linecount +=1;		
		}
		in_char= fgetc(input);
	}
	free(dict.word);
	fprintf(log, "%s: %d\n", "Number of unique words", dict.nval);
	fclose(log);
return 0;  
}



