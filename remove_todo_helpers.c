#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"
#include "remove_todo_helpers.h"
#include "print_subject_helpers.h"


void delete_todo(char subject[]) {
	
	char username[USERNAME_SIZE];
    FILE* file_usr = fopen(FILE_USERNAME, "r");
    fscanf(file_usr, "%s", username);
    fclose(file_usr);

	print_full_subject(subject);
	int del_id;
	printf("| Bot> Enter the number of the todo you want to delete.\n");
	print_line();
	char line[MAX_STR_SIZE];
	int accepted = 0;
	do {
		printf("| %s> (X to exit): ", username); 
		fgets(line, MAX_STR_SIZE, stdin);
		if(isNumber(line) == TRUE) {
			del_id = atoi(line);
			accepted = 1;
		}
		else if(isX(line) == TRUE) {
			printf("| Bot> No changes.\n");
			return;
		}
	} while(accepted == 0);
	print_line();

	if(todoIdExists(subject, del_id) == FALSE) {
		printf("| Bot> The number you entered does not match any todo. Aborted.\n");
		return;
	}
	
	remove_todo(subject, del_id);
	printf("| Bot> Todo removed successfully.\n");
 
}

void print_full_subject(char toPrint[]) {
	FILE* f_subj = fopen(toPrint, "r");
	int lineId;
	char separator;
	char todo[MAX_STR_SIZE];
	char dummy[MAX_STR_SIZE];
	//printf("####");
	print_subject_name(toPrint, FALSE);
	printf("########");
	printf("\n");

	while(!feof(f_subj)) {
        fscanf(f_subj, "%d", &lineId);
        fscanf(f_subj, "%c", &separator);
        fgets(todo, MAX_STR_SIZE, f_subj);
        strncpy(dummy, todo, MAX_TODO_SIZE);
        for(int i = 0; i < MAX_TODO_SIZE; i++) {
            if(dummy[i] == '\n') {
                dummy[i] = '\0';
                break;
            }
        }
        if(!feof(f_subj)) {
            printf("|  # %-6d: %-42s #\n", lineId, dummy);
        }
    }
    printf("|  ");
    for(int i = 0; i < SUBJECT_TITLE_SIZE+8; i++) {
        printf("#");
    }
    printf("\n");
}


int isNumber(char str[]) {
	char currentChar;
	int index = 0;
	if(str[0] == '\n') {return FALSE;}
	while(currentChar != '\n') {
		currentChar = str[index];
		if(currentChar == '\n') {
			break;
		}
		index++;
		if(currentChar < 48 || currentChar > 57) {
			return FALSE;
		}
	}
	return TRUE;
}

int isX(char str[]) {
	if(str[0] == 'X' && str[1] == '\n') {
		return TRUE;
	}
	if(str[0] == 'X' && str[1] == '\0') {
		return TRUE;
	}
	return FALSE;
}


int todoIdExists(char subject[], int id) {
	FILE* f_subj = fopen(subject, "r");
	int currentId;
	char dummy[MAX_STR_SIZE];
	while(!feof(f_subj)) {
		fscanf(f_subj, "%d", &currentId);
		fgets(dummy, MAX_STR_SIZE, f_subj);
		if(currentId == id) {
			return TRUE;
		}
	}
	return FALSE;
}

void remove_todo(char subject[], int id) {
	FILE* f_subj = fopen(subject, "r");
	FILE* f_swap = fopen(FILE_SWAP, "w");
	
	char text[MAX_STR_SIZE];
	int currentId;
	while(!feof(f_subj)) {
		fscanf(f_subj, "%d", &currentId);
			if(feof(f_subj)) {
				break;
			}
		fgets(text, MAX_STR_SIZE, f_subj);
		if(currentId != id) {
			fprintf(f_swap, "%d%s", currentId, text);
		}
	}
	fclose(f_subj);
	fclose(f_swap);	

	remove(subject);
	rename(FILE_SWAP, subject);
	FILE* newSwap = fopen(FILE_SWAP, "w");
	fprintf(newSwap, "%s", EMPTY_STRING);
	fclose(newSwap);
}











