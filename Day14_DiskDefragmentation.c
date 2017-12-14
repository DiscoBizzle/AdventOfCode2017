#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAXCHAR 1000
#define MAXWORDS 100
#include "Utilities.c"
#include "KnotHash.c"

char *IntToBin(int Number, int SizeInBits)
{
	char *BitString = (char *)calloc(SizeInBits+1, sizeof(char));
	for(int i = 0; i < SizeInBits; i++) {
		int Shift = SizeInBits-i-1;
		if ((1<<Shift)&Number) {
			sprintf(BitString, "%s#",BitString); // Add a 1 to the bit string
		} else {
			sprintf(BitString, "%s.",BitString); // Add a 0 to the bit string
		}
	}
	return BitString;
}

int HexCharToInt(char A) {
	if ('A' <= A && A <= 'F') {
		return (A-'A'+10);
	}  else if ('a' <= A && A <= 'f'){
		return (A-'a'+10);
 	} else if ('0' <= A && A <= '9') {
		return (A-'0');
	} else {
		printf("Error in HexCharToInt - %c is not a valid hex character!\n", A);
		return -1;
	}

}

char *HexStringToBinString(char *HexString)
{
	int L = strlen(HexString);
	char *BinString = (char *)calloc((4*L)+1, sizeof(char));
	for (int i = 0; i < L; i++) {
		char *BinSegment = IntToBin(HexCharToInt(HexString[i]), 4);
		sprintf(BinString, "%s%s", BinString, BinSegment);
		free(BinSegment);
	}
	return BinString;
}

void PaintCluster(int *Painted, char **Grid, int L, int i, int j, int CurGroup){
	// Paint the specified block
	Painted[i*L+j] = CurGroup;
	// Now Check all blocks connected to it and paint those recursively
	int ci = i-1;
	int cj = j;
	if (ci >= 0 && ci < L && cj >= 0 && cj < L) { 
		if ((Grid[ci][cj] != '.') && (!Painted[ci*L+cj])) {
			PaintCluster(Painted, Grid, L, ci, cj, CurGroup);
		}
	}
	ci = i+1;
	cj = j;
	if (ci >= 0 && ci < L && cj >= 0 && cj < L) { 
		if (Grid[ci][cj] != '.' && (!Painted[ci*L+cj])) {
			PaintCluster(Painted, Grid, L, ci, cj, CurGroup);
		}
	}
	ci = i;
	cj = j-1;
	if (ci >= 0 && ci < L && cj >= 0 && cj < L) { 
		if (Grid[ci][cj] != '.' && (!Painted[ci*L+cj])) {
			PaintCluster(Painted, Grid, L, ci, cj, CurGroup);
		}
	}
	ci = i;
	cj = j+1;
	if (ci >= 0 && ci < L && cj >= 0 && cj < L) { 
		if (Grid[ci][cj] != '.' && (!Painted[ci*L+cj])) {
			PaintCluster(Painted, Grid, L, ci, cj, CurGroup);
		}
	}
}

int main(int argc, char const *argv[]) 
{
	char *Input = "hfdlxzhv";
	int L = 128;

	char LineInput[16];
	char *Grid[L];
	char HexString[33];
	for (int i = 0; i < L; i++) {
		sprintf(LineInput, "%s-%d", Input, i);
		KnotHash(HexString, LineInput);
		Grid[i] = HexStringToBinString(HexString);
	}

	int *Painted = (int *)calloc(L*L, sizeof(int));
	int NumUsed = 0;
	
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < L; j++) {
			if (Grid[i][j] == '.') {
				Painted[i*L+j] = -1;
			} else {
				NumUsed++;
			}
		}
	}
	printf("NumUsed = %d\n", NumUsed);

	int CurGroup = 1;
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < L; j++) {
			if (Grid[i][j] != '.' && !Painted[i*L+j]) {
				PaintCluster(Painted, Grid, L, i, j, CurGroup);
				CurGroup++;
			}
		}
	}
	int NumGroups = CurGroup - 1;
	printf("Number of groups is %d \n", NumGroups);

	return 0;
}