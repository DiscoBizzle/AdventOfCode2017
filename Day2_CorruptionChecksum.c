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

void ReadFileToMatrix(char *FileName, matrix *Matrix) 
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
			if (Line[StrIndex] < '0' || Line[StrIndex] > '9') {
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
}

int main(int argc, char const *argv[]) 
{
	char *FileName = "./Day2_input.txt";

	matrix Matrix = MatrixAlloc(16, 16);
	ReadFileToMatrix(FileName, &Matrix);
	PrintMatrix(Matrix);

	int LargestNumber = 0;
	int SmallestNumber = 0;
	int CheckSum = 0;
	for (int RowIndex = 0; RowIndex < Matrix.NumRows; RowIndex++) {
		LargestNumber = SmallestNumber = Matrix.Vals[RowIndex*Matrix.NumCols + 0];
		for (int ColIndex = 0; ColIndex < Matrix.NumRows; ColIndex++) {
			int CurEl = Matrix.Vals[RowIndex*Matrix.NumCols + ColIndex];
			if (CurEl > LargestNumber) {
				LargestNumber = CurEl;
			} else if (CurEl < SmallestNumber) {
				SmallestNumber = CurEl;
			}
		}
		CheckSum += LargestNumber-SmallestNumber;
	}
	printf("CheckSum1 = %d\n", CheckSum);

	int Num1;
	int Num2;
	int CheckSum2 = 0;
	int FoundPair = 0;
	for (int RowIndex = 0; RowIndex < Matrix.NumRows; RowIndex++) {
		for (int ColIndex1 = 0; ColIndex1 < Matrix.NumRows; ColIndex1++) {
			Num1 = Matrix.Vals[RowIndex*Matrix.NumCols + ColIndex1];
			for (int ColIndex2 = ColIndex1+1; ColIndex2 < Matrix.NumRows; ColIndex2++) {
				Num2 = Matrix.Vals[RowIndex*Matrix.NumCols + ColIndex2];
				if (Num1 % Num2 == 0) {
					CheckSum2 += Num1/Num2;	
					printf("%d/%d = %d, CheckSum2 = %d\n", Num1,Num2,Num1/Num2,CheckSum2);
					FoundPair = 1; break;
				} else if (Num2 % Num1 == 0) {
					CheckSum2 += Num2/Num1;
					printf("%d/%d = %d, CheckSum2 = %d\n", Num2,Num1,Num2/Num1, CheckSum2);
					FoundPair = 1; break;
				}
			}
			if (FoundPair) {FoundPair = 0; break;}
		}
	}
	printf("CheckSum2 = %d\n", CheckSum2);

	return 0;
}