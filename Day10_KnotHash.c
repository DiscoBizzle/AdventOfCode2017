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

int main(int argc, char const *argv[]) 
{
	int L = 256;
	// int L = 5;
	int List[L];
	for (int i = 0; i < L; i++) {
		List[i] = i;
	}
	PrintList(List, L);


	// Part 1
	/*
	int Lengths[] = {197,97,204,108,1,29,5,71,0,50,2,255,248,78,254,63};
	int NumLengths = 16;
	int ListIndex = 0;
	int SkipSize = 0;
	HashRound(List, L, Lengths, NumLengths, &ListIndex, &SkipSize);
	printf("Product of first two elements = %d \n", List[0]*List[1]);
	*/

	// Part 2

	char *Input = (char*)malloc(MAXCHAR);
	sprintf(Input, "197,97,204,108,1,29,5,71,0,50,2,255,248,78,254,63");
	// sprintf(Input, "AoC 2017");
	int InputLength = strlen(Input);
	char Suffix[] = {17, 31, 73, 47, 23};
	
	for(int i = InputLength; i < InputLength+5; i++) {
		Input[i] = Suffix[i-InputLength];
		Input[i+1] = 0;
	}

	InputLength = strlen(Input);
	printf("Input = ");
	for (int i = 0; i < InputLength; i++) {
		printf("%d, ", Input[i]);
	}
	printf("\n");

	int *Lengths = (int *)malloc(sizeof(int)*InputLength);
	for (int i = 0; i < InputLength; i++) {
		Lengths[i] = (int) Input[i];
	}
	PrintList(Lengths, InputLength);

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

	for(int i = 0; i < 16; i++){
		printf("%02x", DenseHash[i]);
	}
	printf("\n");

	return 0;
}