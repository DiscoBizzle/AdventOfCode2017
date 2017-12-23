#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h> 

#define MAXCHAR 100
#define MAXWORDS 300
#include "Utilities.c"

typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;

int BinaryStringToInt32(char Zero, char One, char *String)
{
	int L = strlen(String);
	int Number = 0;
	int Base = 1; 
	for (int i = (L-1); i >= 0; i--) {
		if (String[i] == One) {
			Number += Base;
		} else if (String[i] == Zero) {
			Number += 0;
		} else {
			printf("Error in BinaryStringToInt32! Unrecognised character '%c' \n", String[i]);
		}
		Base *= 2;
	}
	// printf("%s = %d\n", String, Number);
	return Number;
}

int CountCharInString(char CountChar, char *String) 
{
	int L = strlen(String);
	int Count = 0;
	for (int i = 0; i < L; i++) {
		if (String[i] == CountChar) {
			Count++;
		}
	}
	return Count;
}

void PermuteString(char *PermutedString, char *String, int *Permutation)
{
	int L = strlen(String);
	char *Temp = (char*)calloc(L+1, sizeof(char));

	for (int i = 0; i < L; i++) {
		Temp[Permutation[i]] = String[i];
	}
	for (int i = 0; i < L; i++) {
		PermutedString[i] = Temp[i];
	}
	free(Temp);
}

void AddWordArrayToDictionary2(char **Dictionary, int *OnCount, char **WordArray)
{
	char *String = (char *)calloc(MAXCHAR, sizeof(char));
	sprintf(String, "%s%s", WordArray[0], WordArray[1]);

	/* 0 1 
	   2 3 */
	int Hash = BinaryStringToInt32('.', '#', String); // Generate hash by converting the binary string to an integer. 
	
	Dictionary[Hash] = (char *)calloc(10, sizeof(char));
	sprintf(Dictionary[Hash], "%s%s%s", WordArray[3], WordArray[4], WordArray[5]);

	int NumOn = CountCharInString('#', Dictionary[Hash]);
	OnCount[Hash] = NumOn;

	int Permutation[7][4] = {{1, 0, 3, 2}, // Hor flip
						  {2, 0, 3, 1},    // Rot 90 
						  {0, 2, 1, 3},    // Rot 90 Hor flip
						  {3, 2, 1, 0},    // Rot 180 
						  {2, 3, 0, 1},	   // Rot 180 Hor flip
						  {1, 3, 0, 2},    // Rot 270 
						  {3, 1, 2, 0}};   // Rot 270 Hor flip

	// Now perform all rotations and flips and generate the hashes for those.
	char *PString = (char *)calloc(10, sizeof(char));
	for (int i = 0; i < 7; i++) {
		PermuteString(PString, String, Permutation[i]);
		Hash = BinaryStringToInt32('.', '#', PString);
		Dictionary[Hash] = (char *)calloc(10, sizeof(char));
		sprintf(Dictionary[Hash], "%s%s%s", WordArray[3], WordArray[4], WordArray[5]);
		OnCount[Hash] = NumOn;
	}
}

void AddWordArrayToDictionary3(char **Dictionary, int *OnCount, char **WordArray)
{
	char *String = (char *)calloc(MAXCHAR, sizeof(char));
	sprintf(String, "%s%s%s", WordArray[0], WordArray[1], WordArray[2]);

	/* 0 1 2 
	   3 4 5
	   6 7 8 */
	int Hash = BinaryStringToInt32('.', '#', String); // Generate hash by converting the binary string to an integer. 
	
	Dictionary[Hash] = (char *)calloc(17, sizeof(char));
	sprintf(Dictionary[Hash], "%s%s%s%s", WordArray[4], WordArray[5], WordArray[6], WordArray[7]);

	int NumOn = CountCharInString('#', Dictionary[Hash]);
	OnCount[Hash] = NumOn;

	// Now perform all rotations and flips and generate the hashes for those.
	char *PString = (char *)calloc(17, sizeof(char));

	int Permutation[7][9] = {{2, 1, 0, 5, 4, 3, 8, 7, 6}, // Hor flip
					  		 {6, 3, 0, 7, 4 ,1, 8, 5, 2},    // Rot 90 
					  		 {0, 3, 6, 1, 4, 7, 2, 5, 8},    // Rot 90 Hor flip
					  		 {8, 7, 6, 5, 4, 3, 2, 1, 0},    // Rot 180 
					  		 {6, 7, 8, 3, 4, 5, 0, 1, 2},	   // Rot 180 Hor flip
					  		 {2, 5, 8, 1, 4, 7, 0, 3, 6},    // Rot 270 
					  		 {8, 5, 2, 7, 4, 1, 6, 3, 0}};   // Rot 270 Hor flip

	for (int i = 0; i < 7; i++) {
		PermuteString(PString, String, Permutation[i]);
		Hash = BinaryStringToInt32('.', '#', PString);
		Dictionary[Hash] = (char *)calloc(17, sizeof(char));
		sprintf(Dictionary[Hash], "%s%s%s%s", WordArray[4], WordArray[5], WordArray[6], WordArray[7]);
		OnCount[Hash] = NumOn;
	}
}

void PrintGrid(char *Block, int W, int H)
{
	for (int j = 0; j < H; j++) {
		for (int i = 0; i < W; i++) {
			printf("%c", Block[j*W + i]);
		}
		printf("\n");
	}
}

int main(int argc, char const *argv[]) 
{
	clock_t t; 
	t = clock();

	FILE *FilePointer = fopen("./Day21_input.txt", "r");
	char *Line = (char *)malloc(MAXCHAR);
	size_t Count; ssize_t nread;
	if (FilePointer == NULL) { printf("Cannot open file!\n"); return -1; }

	char *WordArray[MAXWORDS];

	// NOTE: First we will read in the rules, generate the rotations and reflections, 
	// and then hash them by converting the 4-bit or 9-bit binary number to a decimal interger.
	int LineIndex = 0;
	char *Dictionary2[16];  // Number of possible combinations for 2x2 blocks is 2^(4) = 16 
	int OnCount2[16] = {0};
	char *Dictionary3[512]; // Number of possible combinations for 3x3 blocks is 2^(9) = 512
	int OnCount3[512] = {0};

	while ((nread = getline(&Line, &Count, FilePointer)) != -1) {
		int NumWords = LineToWordArrayChooseSplit(WordArray, Line, " /", 2);
		if (LineIndex < 6) {
			AddWordArrayToDictionary2(Dictionary2, OnCount2, WordArray);
		} else {
			AddWordArrayToDictionary3(Dictionary3, OnCount3, WordArray);
		}

		LineIndex++;
	}

	int S = 3; 
	int nS = 0;
	char *CurrGrid;
	char *PrevGrid = (char*)calloc(S*S+1, sizeof(char));
	sprintf(PrevGrid, ".#...####");
	int PrevBlockSize = 0;
	int CurrBlockSize = 0;
	int NumIterations = 18;
	for (int IterationCount = 1; IterationCount <= NumIterations; IterationCount++) {
		if (S%2 == 0) {
			nS = S/2*3;
			PrevBlockSize = 2;
			CurrBlockSize = 3;
		} else {
			nS = (S/3)*4;
			PrevBlockSize = 3;
			CurrBlockSize = 4;
		}
		int CurrNumLightsOn = 0;
		CurrGrid = (char*)calloc(nS*nS, sizeof(char));
		// Get the current block and hash it
		int NumBlocksX = (S/PrevBlockSize);
		int NumBlocksY = (S/PrevBlockSize);

		char *PrevBlock = (char *)calloc(PrevBlockSize*PrevBlockSize + 1, sizeof(char));
		char *CurrBlock = (char *)calloc(CurrBlockSize*CurrBlockSize + 1, sizeof(char));

		for (int BlockIndex = 0; BlockIndex < NumBlocksX*NumBlocksY; BlockIndex++) {
			int StartX = (BlockIndex%NumBlocksX)*PrevBlockSize;
			int StartY = (BlockIndex/NumBlocksX)*PrevBlockSize;
			
			// Extract the current block 
			for (int j = 0; j < PrevBlockSize; j++) {
				for (int i = 0; i < PrevBlockSize; i++) {
					PrevBlock[j*PrevBlockSize + i] = PrevGrid[StartX + i + S*(j+StartY)];
				}
			}

			// Hash the current block to get the index of corresponding block in the next grid.
			int Hash = BinaryStringToInt32('.', '#', PrevBlock);
			if (S%2 == 0) {
				sprintf(CurrBlock, "%s", Dictionary2[Hash]);
				CurrNumLightsOn += OnCount2[Hash];
			} else {
				sprintf(CurrBlock, "%s", Dictionary3[Hash]);
				CurrNumLightsOn += OnCount3[Hash];
			} 

			// Insert the new block into the new grid
			StartX = (BlockIndex%NumBlocksX)*CurrBlockSize;
			StartY = (BlockIndex/NumBlocksY)*CurrBlockSize; 
			for (int j = 0; j < CurrBlockSize; j++) {
				for (int i = 0; i < CurrBlockSize; i++) {
					CurrGrid[StartX + i + nS*(j+StartY)] = CurrBlock[j*CurrBlockSize + i];
				}
			}
		}
		printf("IterationCount = %d, nS = %d \n", IterationCount, nS);
		// PrintGrid(CurrGrid, nS, nS);
		printf("CurrNumLightsOn = %d\n", CurrNumLightsOn);
		printf("\n");
		free(PrevGrid);
		PrevGrid = CurrGrid;
		S = nS;
	}
	
	t = clock()-t;
	printf ("It took me %lu clicks (%f seconds) for parts 1 and 2.\n", t,((float)t)/CLOCKS_PER_SEC);
	return 0;
}