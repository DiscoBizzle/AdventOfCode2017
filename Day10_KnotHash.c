#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "KnotHash.c"

int Max(int x, int y) {
	if (x > y) {
		return x;
	} else {
		return y;
	}
} 

#define MAXCHAR 100000
#define MAXWORDS 100

int main(int argc, char const *argv[]) 
{
	// Part 1
	int L = 256;
	// int L = 5;
	int List[L];
	for (int i = 0; i < L; i++) {
		List[i] = i;
	}
	int Lengths[] = {197,97,204,108,1,29,5,71,0,50,2,255,248,78,254,63};
	int NumLengths = 16;
	int ListIndex = 0;
	int SkipSize = 0;
	HashRound(List, L, Lengths, NumLengths, &ListIndex, &SkipSize);
	printf("Product of first two elements = %d \n", List[0]*List[1]);
	

	// Part 2
	char *Input = (char*)malloc(MAXCHAR);
	sprintf(Input, "197,97,204,108,1,29,5,71,0,50,2,255,248,78,254,63");

	char HashResult[32];
	KnotHash(HashResult, Input);
	printf("%s\n", HashResult);
	
	return 0;
}