#include <stdio.h>

#define BUFFERL 2018

void InsertInBuffer(int *Buffer, int *L, int InsertionIndex, int Value)
{
	for(int i = *L; i > InsertionIndex; i--) {
		Buffer[i] = Buffer[i-1];
	}
	Buffer[InsertionIndex] = Value;
	*L = (*L) + 1;
}

void PrintList(int *Buffer, int L, int Highlight) 
{
	for (int i = 0; i < L; i++) {
		if (i == Highlight) {
			printf("(%d), ", Buffer[i]);
		} else {
			printf("%d, ", Buffer[i]);
		}
	}
	printf("\n");
}

int main(int argc, char const *argv[]) 
{
	// Part 1 
	// TODO: do we need to keep track of the entire state here?
	int Buffer[BUFFERL] = {0};
	int L = 1;
	int CurrIndex = 0;
	int StepSize = 344; 

	for (int i = 1; i < BUFFERL; i++) {
		CurrIndex += StepSize;
		CurrIndex = CurrIndex%L + 1;
		InsertInBuffer(Buffer, &L, CurrIndex, i);
	}
	printf("Buffer[%d] = %d. Buffer[%d] = %d\n", CurrIndex, Buffer[CurrIndex], CurrIndex+1, Buffer[CurrIndex+1]);

	// Part 2
	// TODO: can we speed this up by predicting when CurrIndex = 1 rather than iterating 50,000,000 times to calculate it? 
	L = 1;
	CurrIndex = 0;
	int NumberAfter0 = -1;
	for (int i = 1; i < 50000000; i++) {
		CurrIndex += StepSize;
		CurrIndex = CurrIndex%L + 1;
		L++;
		if (CurrIndex == 1) {
			NumberAfter0 = i;
		}
	}
	printf("NumberAfter0 = %d\n", NumberAfter0);

	return 0;
}