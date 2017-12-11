#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ArrayCount(Array) (sizeof(Array))/sizeof(Array[0])
#define MAXCHAR 1000

int CharToInt(char a) 
{
	if (a < '0' || a > '9') { // Make sure that the input character is a number
		return -1;
	} else {
		return (int)(a-'0');
	}
}

typedef struct _matrix {
	int *Vals;
	int NumRows;
	int NumCols;
} matrix;

matrix MatrixAlloc(int NumRows, int NumCols)
{
	matrix NewMatrix;
	NewMatrix.Vals = (int*) malloc(sizeof(int)*NumRows*NumCols + 1);
	NewMatrix.NumRows = NumRows;
	NewMatrix.NumCols = NumCols;

	return NewMatrix;
}

void PrintMatrix(matrix Matrix) 
{
	for (int RowIndex = 0; RowIndex < Matrix.NumRows; RowIndex++) {
		for (int ColIndex = 0; ColIndex < Matrix.NumCols; ColIndex++) {
			printf("%d,  \t", Matrix.Vals[RowIndex*Matrix.NumCols + ColIndex]);
		}
		printf(" \n");
	}
	printf(" \n");
}

void ReadFileToMatrix(char *FileName, matrix *Matrix, int *NumRows, int *NumCols) 
{
	FILE *FilePointer;
	FilePointer = fopen(FileName, "r");
	char *Line = (char *)malloc(MAXCHAR);
	size_t Count;
	ssize_t nread;
	
	if (FilePointer == NULL) {
		printf("Cannot open file!\n");
		return;
	}

	int RowIndex = 0;
	int ColIndex = 0;
	int CurEl;
	while ((nread = getline(&Line, &Count, FilePointer)) != -1) { // Read a single line from the file
		ColIndex = 0;

		int LineLength = strlen(Line);
		char CurrString[MAXCHAR];
		int CSIndex = 0;
		for (int StrIndex = 0; StrIndex < LineLength; StrIndex++) { // NOTE: This will break if the integers are separated by more than one non-int char
			if ((Line[StrIndex] < '0' || Line[StrIndex] > '9') && Line[StrIndex] != '-') {
				Matrix->Vals[RowIndex*Matrix->NumCols + ColIndex] = atoi(CurrString);
				sprintf(CurrString, "");
				CSIndex = 0;
				ColIndex++;
			} else {
				CurrString[CSIndex] = Line[StrIndex];
				CurrString[CSIndex+1] = 0;
				CSIndex++;
			}
		}
        RowIndex++;
	}
	if (NumRows != NULL) {*NumRows = RowIndex;}
	if (NumCols != NULL) {*NumCols = ColIndex;}
}

int main(int argc, char const *argv[]) 
{
	char *FileName = "./Day5_input.txt";

	matrix Matrix = MatrixAlloc(1052, 1);
	ReadFileToMatrix(FileName, &Matrix, NULL, NULL);
	int CurrIndex = 0; int NumSteps;
	for (NumSteps = 0; (0 <= CurrIndex) && (CurrIndex < Matrix.NumRows); NumSteps++) {
		// CurrIndex += Matrix.Vals[CurrIndex]++; // Part 1
		CurrIndex += (Matrix.Vals[CurrIndex] >= 3) ? (Matrix.Vals[CurrIndex]--):(Matrix.Vals[CurrIndex]++); // Part 2
	}
	printf("NumSteps = %d\n", NumSteps);
	return 0;
}