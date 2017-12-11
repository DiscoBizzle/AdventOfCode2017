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

#define MAXCHAR 1000
#define MAXWORDS 100
 
int LineToWordArray(char **WordArray, char *Line) {
	int CharIndex = 0;
	int LineIndex = 0;
	int WordIndex = 0;
	char CurrWord[MAXCHAR];
	int Running = 1;
	while (Running) {
		if (Line[LineIndex] == '\n') {
			CurrWord[CharIndex] = 0;
			WordArray[WordIndex] = (char*)malloc(MAXCHAR);
			strcpy(WordArray[WordIndex], CurrWord);
			Running = false;
			WordIndex++;
		} else if (Line[LineIndex] == ' ') {
			CurrWord[CharIndex] = 0;
			WordArray[WordIndex] = (char*)malloc(MAXCHAR);
			strcpy(WordArray[WordIndex], CurrWord);
			LineIndex++; WordIndex++; CharIndex = 0;
		} else {
			CurrWord[CharIndex] = Line[LineIndex];
			CurrWord[CharIndex+1] = 0;
			CharIndex++; LineIndex++; 
		}
	}
	return WordIndex;
}

void BubbleSort(char *List) {
	int L = strlen(List);
	int Ordered = 1;
	int Done = 0;
	while (!Done) {
		Done = 1;
		for (int i = 0; i < L-1; i++) {
			if (List[i] > List[i+1]) {
				char Temp = List[i];
				List[i] = List[i+1];
				List[i+1] = Temp;
				Done = 0;
			} 
		}
	}
}

int main(int argc, char const *argv[]) 
{
	FILE *FilePointer = fopen("./Day4_input.txt", "r");
	char *Line = (char *)malloc(MAXCHAR);
	size_t Count; ssize_t nread;
	if (FilePointer == NULL) { printf("Cannot open file!\n"); return -1; }

	char *WordArray[MAXWORDS];

	int NumValid = 0;
	while ((nread = getline(&Line, &Count, FilePointer)) != -1) {
		int NumWords = LineToWordArray(WordArray, Line);
		int PassPhraseValid = 1;
		for (int i = 0; i < NumWords; i++) {
			for (int j = i+1; j < NumWords; j++) {
				BubbleSort(WordArray[i]); BubbleSort(WordArray[j]);
				if (strcmp(WordArray[i], WordArray[j]) == 0) {
					PassPhraseValid = 0;
				}
			}
		}
		NumValid += PassPhraseValid;
		for (int i = 0; i < NumWords; i++) {
			free(WordArray[i]);
		} 
	}
	printf("Number of valid lines: %d\n", NumValid);

	return 0;
}