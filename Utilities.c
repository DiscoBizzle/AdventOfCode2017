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
 
int LineToWordArray(char **WordArray, char *Line) {
	int CharIndex = 0;
	int LineIndex = 0;
	int WordIndex = 0;
	char CurrWord[MAXCHAR];
	int Running = 1;
	while (Running) {
		if (Line[LineIndex] == '\n') { // We have reched the end of the line 
			if (CharIndex > 0) { // Only add a new word if it has a nonzero length
				CurrWord[CharIndex] = 0;
				WordArray[WordIndex] = (char*)malloc(MAXCHAR);
				strcpy(WordArray[WordIndex], CurrWord);
				WordIndex++;
			}
			Running = false;
		} else if (Line[LineIndex] == ' ' || Line[LineIndex] == '\t' || Line[LineIndex] == ',') {
			if (CharIndex > 0) {
				CurrWord[CharIndex] = 0;
				WordArray[WordIndex] = (char*)malloc(MAXCHAR);
				strcpy(WordArray[WordIndex], CurrWord);
				WordIndex++; 
			}
			LineIndex++; CharIndex = 0;
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

void PrintWordArray(char **WordArray, int NumWords)
{
	for (int i = 0; i < NumWords; i++) {
		printf("%s / ", WordArray[i]);
	}
	printf("\n");
}

void StripParentheses(char *Word) {
	int L = strlen(Word);
	for (int i = 0; i < L; i++) {
		Word[i] = Word[i+1];
	}
	Word[L-2] = 0;
}

void StripLastChar(char *Word) {
	int L = strlen(Word);
	Word[L-1] = 0;
}

int CheckDifferent(int *List, int L, int *DiffIndex) 
{
	// NOTE: This expects exactly one item different from the others
	int PrevItem = List[0];
	for (int i = 1; i < L; i++) {
		if (PrevItem != List[i]) {
			if (PrevItem != List[(i+1)%L]) {
				// PrevItem is the different one
				*DiffIndex = i-1;
				return (PrevItem - List[i]);
			} else {
				// List[i] is the different one
				*DiffIndex = i;
				return (List[i] - PrevItem);
			}
		}
		PrevItem = List[i];
	}
	return 0; 
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

	for (int RowIndex = 0; RowIndex < NewMatrix.NumRows; RowIndex++) {
		for (int ColIndex = 0; ColIndex < NewMatrix.NumCols; ColIndex++) {
			NewMatrix.Vals[RowIndex*NewMatrix.NumCols + ColIndex] = 0;
		}
	}

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