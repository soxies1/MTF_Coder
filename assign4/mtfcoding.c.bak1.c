#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mtfcoding.h"



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

void addword(Word newname)
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

int is_in_dict(Word foo){
	Word* p = dict.word;
	int pos = 0;
	while ((p->val)){
		if(strcmp(foo.val, (p->val)) == 0){
			return pos;
		}
		printf("%s ", (p->val));
		pos++;
		p++;
		}
	return -1;
}
void move_to_top(int g){
	return;
}

void add_to_dict(char* word, FILE* output){
	int i = 0;
	if(!(*word)) return;
	Word *temp = new_word(word);
	if(count == 0){
		addword(*temp);
		count++;
		fputc(129, output);
		printf("heres where it stops 89");
		fprintf(output,  "%s",word);
		return;
	}else{
		i = is_in_dict(*temp);
		if(i == -1){
			addword(*temp);
			fputc((129+count), output);
			printf("heres where it stops 97");
			fprintf(output, "%s",word);
			count++;
		}else{
			move_to_top(i);
			fputc((i+129), output);
		}
	}
	return;

}


void sub_encode(char* line, FILE *output){
	//printf("got to here line is: %s", line);
	char* pos;
	char* t;
	printf("heres where it stops 114");
	printf("tokenizing");
	t = strtok(line, " ");
	while (t) {
		printf("tokenizing");
		if((pos=strchr(t,'\n')) != NULL)
			*pos = '\0';
			add_to_dict(t, output);
			t = strtok(NULL, " ");
		}
		printf("about to put the endline");
		fputc('\n', output);
		printf("heres where it stops 123");
	printf("%s ", dict.word->val);	
	
	return;
}

int encode(FILE *input, FILE *output) {
	fputc(0xfa, output);
	fputc(0xce, output);
	fputc(0xfa, output);
	fputc(0xdf, output);
	char* line = NULL;
	size_t linesize = 0;
	ssize_t linelen;
	printf("heres where it stops 140");
	while((linelen = getline(&line, &linesize, input)) > 0){
		printf("heres where it stops 142");
	sub_encode(line, output);
	printf("heres where it stops 139");
	free(line);
	line = NULL;
	}    
	
	return 0;
}



int decode(FILE *input, FILE *output) {
    
	char* line = NULL;
	size_t linesize =0;
	ssize_t linelen=0;
	while((linelen=getline(&line, &linesize, input)) > 0){
	//function(linebuf, linesize);
	free(line);
	line = NULL;
	}
	return 0;    
}
