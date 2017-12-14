#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void PrintList(int *List, int L) {
	printf("List = ");
	for (int i = 0; i < L; i++) {
		printf("%d, ", List[i]);
	}
	printf("\n");
}

void FlipSubList(int *List, int ListIndex, int Length, int SubLength)
{
	// printf("FlipSubList\n");
	// First copy the relevant chunk of the list to the sublist
	// printf("SubLength = %d\n", SubLength);
	int SubList[SubLength];
	for (int i = 0; i < SubLength; i++) {
		SubList[i] = List[(i+ListIndex)%Length];
	}
	// printf("SubList ");
	// PrintList(SubList, SubLength);

	// Now copy the sublist to the list in reverse order. 
	for (int i = 0; i < SubLength; i++) {
		// printf("List[%d] = SubList[%d] = %d\n", (i+ListIndex)%Length, SubLength-i, SubList[SubLength-1-i]);
		List[(i+ListIndex)%Length] = SubList[(SubLength-1)-i];
	}
}

void HashRound(int *List, int ListLength, int *LengthList, int NumLengths, int *ListIndex, int *SkipSize)
{
	for (int i = 0; i < NumLengths; i++) {
		FlipSubList(List, (*ListIndex), ListLength, LengthList[i]);
		*ListIndex = (*ListIndex) + LengthList[i] + (*SkipSize);
		(*SkipSize)++;
	}
}

void KnotHash(char *Output, char *Input)
{
	int L = 256;
	// int L = 5;
	int List[L];
	for (int i = 0; i < L; i++) {
		List[i] = i;
	}

	int InputLength = strlen(Input);
	char Suffix[] = {17, 31, 73, 47, 23};
	
	for(int i = InputLength; i < InputLength+5; i++) {
		Input[i] = Suffix[i-InputLength];
		Input[i+1] = 0;
	}

	InputLength = strlen(Input);

	int *Lengths = (int *)malloc(sizeof(int)*InputLength);
	for (int i = 0; i < InputLength; i++) {
		Lengths[i] = (int) Input[i];
	}

	int ListIndex = 0;
	int SkipSize = 0;
	for (int LoopIndex = 0; LoopIndex < 64; LoopIndex++) {
		HashRound(List, L, Lengths, InputLength, &ListIndex, &SkipSize);
	}

	int DenseHash[16];
	for (int BlockIndex = 0; BlockIndex < 16; BlockIndex++) {
		DenseHash[BlockIndex] = 0;
		for (int ByteIndex = 0; ByteIndex < 16; ByteIndex++) {
			int CurrEl = (List[BlockIndex*16 + ByteIndex]);
			DenseHash[BlockIndex] = DenseHash[BlockIndex]^CurrEl; 
		} 
	}

	// char *HashString = (char*)calloc(32+1, sizeof(char));
	// char *CurrString = (char*)calloc(2+1, sizeof(char));
	Output[0] = 0;
	for(int i = 0; i < 16; i++){
		sprintf(Output, "%s%02x", Output, DenseHash[i]);
	}
	// free(CurrString);
	// return HashString;
}