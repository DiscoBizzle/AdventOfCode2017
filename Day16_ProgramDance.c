#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAXCHAR 100000
#define MAXWORDS 10000
#include "Utilities.c"

typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;

char *Int64ToBin(uint64 Number, int SizeInBits)
{
	char *BitString = (char *)calloc(SizeInBits+1, sizeof(char));
	for(int i = 0; i < SizeInBits; i++) {
		uint64 Shift = SizeInBits-i-1;
		if ((1ULL<<Shift)&Number) { //NOTE: When bit-shifting literals lke this, we need to specify that the literal is 64 bit.
			sprintf(BitString, "%s1",BitString); // Add a 1 to the bit string
		} else {
			sprintf(BitString, "%s0",BitString); // Add a 0 to the bit string
		}
	}
	return BitString;
}

int min(int a, int b) {
	if (a < b) {
		return a;
	} else {
		return b;
	}
}

void ReadIntPair(char *String, int *a, int *b) 
{
	// char *Buffer = (char *)calloc(10, sizeof(char));
	char Buffer[10] = {0};

	int Offset = 1;
	for (int i = 1; ; i++) {
		if ('0' <= String[i] && String[i] <= '9') {
			Buffer[i-Offset] = String[i];
			Buffer[i-Offset+1] = 0;
		} else {
			if (Offset == 1) {
				*a = atoi(Buffer);
				Buffer[0] = 0;
				Offset = i+1;
			} else {
				*b = atoi(Buffer);
				return;
			}
		}
	}

}

void ReadCharPair(char *String, char *a, char *b) 
{
	*a = String[1];
	*b = String[3];
}

void Spin(char *List, int L, int a)
{
	char Buffer[L]; 
	// Copy the elements from the end of the array into the buffer
	for (int i = 0; i < a; i++) {
		Buffer[i] = List[L-a+i];
	}
	for (int i = a; i < L; i++) {
		Buffer[i] = List[i-a];
	}
	for (int i = 0; i < L; i++) {
		List[i] = Buffer[i];
	}
}

void SwapByIndex(char *List, int a, int b)
{
	char Temp = List[a];
	List[a] = List[b];
	List[b] = Temp;
}

void SwapByName(char *List, int L, char a, char b)
{
	int ia, ib;
	for (int i = 0; i < L; i++) {
		if (List[i] == a) {
			ia = i;
		} 
		if (List[i] == b) {
			ib = i;
		}
	}
	SwapByIndex(List, ia, ib);
}

int *GetMap(char *InitialList, char *FinalList, int L)
{
	int *Map = (int *)malloc(sizeof(int)*L);
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < L; j++) {
			if (InitialList[i] == FinalList[j]) {
				Map[i] = j;
			}
		}
	}
	return Map;
}

void ApplyMap(int *Map, char *List, int L)
{
	char NewList[L];
	for (int i = 0; i < L; i++) {
		NewList[Map[i]] = List[i];
	}
	for (int i = 0; i < L; i++) {
		List[i] = NewList[i];
	}
}

int ListsEqual(char *List1, char *List2, int L) 
{
	for (int i = 0; i < L; i++) {
		if (List1[i] != List2[i]) {
			return 0;
		}
	}
	return 1;
}

int main(int argc, char const *argv[]) 
{
	FILE *FilePointer = fopen("./Day16_input.txt", "r");
	char *Line = (char *)malloc(MAXCHAR);
	size_t Count; ssize_t nread;
	if (FilePointer == NULL) { printf("Cannot open file!\n"); return -1; }

	char *WordArray[MAXWORDS];

	int L = 16; 
	char List[L];
	for (int i = 0; i < L; i++) {
		List[i] = i+'a';
	} 
	char InitialList[L];
	for (int i = 0; i < L; i++) {
		InitialList[i] = i+'a';
	} 
	for (int i = 0; i < L; i++) {
		printf("%c", List[i]);
	}
	printf("\n");

	if ((nread = getline(&Line, &Count, FilePointer)) != -1) {
		int NumWords = LineToWordArray(WordArray, Line);
		// int NumWords = LineToWordArray(WordArray, "s1,x3/4,pe/b\n");
		// StripLastChar(WordArray[0]);
		// PrintWordArray(WordArray, NumWords);
		int NumRepsInId = 0;
		for (int NumReps = 1; NumReps < 40; NumReps++) {
			for (int WordIndex = 0; WordIndex < NumWords; WordIndex++) {
				if (WordArray[WordIndex][0] == 's') {
					int a, b;
					ReadIntPair(WordArray[WordIndex], &a, &b);
					// printf("a = %d, b = %d\n", a, b);
					Spin(List, L, a);
				} else if (WordArray[WordIndex][0] == 'x') {
					int a,b;
					ReadIntPair(WordArray[WordIndex], &a, &b);
					// printf("a = %d, b = %d\n", a, b);
					SwapByIndex(List, a, b);
				} else if (WordArray[WordIndex][0] == 'p') {
					char a,b;
					ReadCharPair(WordArray[WordIndex], &a, &b);
					// printf("a = %c, b = %c\n", a, b);
					SwapByName(List, L, a, b);
				}
			}
			for (int i = 0; i < L; i++) {
				printf("%c", List[i]);
			}
			printf("\n");
			
			if (ListsEqual(List, InitialList, L)) {
				printf("NumReps = %d\n", NumReps);
				NumRepsInId = NumReps;
				break;
			}
		}


	}

	return 0;
}