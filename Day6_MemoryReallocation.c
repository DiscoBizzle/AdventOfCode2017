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
		} else if (Line[LineIndex] == ' ' || Line[LineIndex] == '\t') {
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

int *WordArrayToIntArray(char **WordArray, int NumWords) 
{
	int *IntArray = (int *) malloc(sizeof(int) * NumWords);
	for (int i = 0; i < NumWords; i++) {
		IntArray[i] = atoi(WordArray[i]);
	}
	return IntArray;
}

void PrintIntArray(int *Array, int L) {
	for (int i = 0; i < L; i++) {
		printf("%d\t", Array[i]);			

	} 
	printf("\n");
}

void Print2dIntArray(int *Array, int L, int H) {
	for (int j = 0; j < H; j++) {
		for (int i = 0; i < L; i++) {
			printf("%d\t", Array[L*j + i]);
		}
		printf("\n");
	}
}

int FindFirstMost(int *MemArray, int L)
{
	int LargestVal = 0;
	int Index = 0;
	for (int i = 0; i < L; i++) {
		if (MemArray[i] > LargestVal) {
			LargestVal = MemArray[i];
			Index = i;
		}
	}
	return Index;
}

int CompareToPreviousConfigs(int *ConfigArray, int NumConfigs, int *IntArray, int L)
{
	int MatchCount;
	for (int ConfigIndex = 0; ConfigIndex < NumConfigs; ConfigIndex++) {
		// PrintIntArray()
		for (int i = 0; i < L; i++) {
			if (ConfigArray[ConfigIndex*L + i] != IntArray[i]) {
				break;
			} else if (i == L-1){ // If we reached the end of the array 
				return ConfigIndex;
			}
		}
	}
	return -1;
}


#define MAXCONFIGS 20000

int main(int argc, char const *argv[]) 
{
	FILE *FilePointer = fopen("./Day6_input.txt", "r");
	char *Line = (char *)malloc(MAXCHAR);
	size_t Count; ssize_t nread;
	if (FilePointer == NULL) { printf("Cannot open file!\n"); return -1; }

	char *WordArray[MAXWORDS];
	int *MemArray;

	// Read the file and convert the string into an array of integers.
	if ((nread = getline(&Line, &Count, FilePointer)) != -1) {	
		int L = LineToWordArray(WordArray, Line);
		MemArray = WordArrayToIntArray(WordArray, L);
		for (int i = 0; i < L; i++) { free(WordArray[i]); } 

		// Create the array of configurations and store the starting config.
		int *ConfigArray = (int*)malloc(sizeof(int)*L*MAXCONFIGS);
		for (int i = 0; i < L; i++) {
			ConfigArray[0*L + i] = MemArray[i]; 
		}

		int ConfigIndex;
		for (ConfigIndex = 0; ConfigIndex < MAXCONFIGS;) {
			// Generate the next confiruration
			int Index = FindFirstMost(MemArray, L);
			int Blocks = MemArray[Index];
			MemArray[Index] = 0;
			for (int i = Index+1; Blocks > 0; i++) {
				MemArray[i%L]++; Blocks--;
			}
			ConfigIndex++;
			// Compare the new configuration agains preivious ones, extract the index
			int MatchedIndex = CompareToPreviousConfigs(ConfigArray, ConfigIndex, MemArray, L); 
			if (MatchedIndex >= 0) {
				printf("MATCH! ConfigIndex = %d, MatchedIndex = %d, Offset = %d\n", ConfigIndex, MatchedIndex, ConfigIndex-MatchedIndex);
				PrintIntArray(MemArray, L);
				return 0;
			}
			// Store the new configuration
			for (int i = 0; i < L; i++) {
				ConfigArray[ConfigIndex*L + i] = MemArray[i]; 
			}
		}
		printf("No Matches!\n");
	}
	return 0;
}