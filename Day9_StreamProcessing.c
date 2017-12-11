#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int Max(int x, int y) {
	if (x > y) {
		return x;
	} else {
		return y;
	}
} 

#define MAXCHAR 100000
#define MAXWORDS 100

int ComputeLineScore(char *Line, int *NumGroups)
{
	int L = strlen(Line);
	char c;
	int NumOpenBrackets;
	int CurDepth = 0; 
	int Score = 0;
	int InGarbage = 0;
	int CharsInGarbage = 0;
	for (int i = 0; i < L; i++) {
		// printf("%c\n", Line[i]);
		if (InGarbage) {
			if (Line[i] == '!') {
				i++;
				continue;
			} else if (Line[i] == '>') {
				InGarbage = 0;
			} else {
				CharsInGarbage++;
			}
		} else {	
			if (Line[i] == '{') {
				CurDepth++;
				printf("CurDepth = %d\n", CurDepth);
			} else if (Line[i] == '}') {
				
					Score += CurDepth;
					CurDepth--;
					(*NumGroups)++;
					printf("CurDepth = %d\n", CurDepth);

			} else if (Line[i] == '<') {
				InGarbage = 1; 
			} else if (Line[i] != ',') {
				printf("ERROR! Line[%d] = '%c' outside garbage!\n",i, Line[i]);
			}	
		}
	}
	if (CurDepth != 0) {
		printf("Error! not all brackets were closed!\n");
	}

	printf("CharsInGarbage = %d\n", CharsInGarbage);
	return Score;
}

int main(int argc, char const *argv[]) 
{
	FILE *FilePointer = fopen("./Day9_input.txt", "r");
	char *Line = (char *)malloc(MAXCHAR);
	size_t Count; ssize_t nread;
	if (FilePointer == NULL) { printf("Cannot open file!\n"); return -1; }

	char Test1[MAXCHAR];
	sprintf(Test1, "{<random characters>}\n");
	printf("%s\n", Test1);
	int NumGroups = 0; 
	int Score1 = ComputeLineScore(Test1, &NumGroups);
	printf("NumGroups = %d, Score = %d\n", NumGroups, Score1);

	// Read the file and convert the string into an array of integers.
	if ((nread = getline(&Line, &Count, FilePointer)) != -1) {
		printf("%s\n", Line);	
		int Score2 = ComputeLineScore(Line, &NumGroups);
		printf("NumGroups = %d, Score = %d\n", NumGroups, Score2);
	}
	return 0;
}