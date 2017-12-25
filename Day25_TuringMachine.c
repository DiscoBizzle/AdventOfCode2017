#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h> 

#define MAXCHAR 100
#define MAXWORDS 100
#include "Utilities.c"

typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;

void SafeCharWrite(char *Array, int ArrayLength, int Index, char Value)
{
	if (Index < 0 || Index >= ArrayLength) {
		printf("Error! Index = %d is out of the array bounds [%d, %d]!\n", Index, 0, ArrayLength);
		int *Pointer = 0;
		*Pointer = 1;
	} else {
		Array[Index] = Value;
	}
}

char SafeCharRead(char *Array, int ArrayLength, int Index)
{
	if (Index < 0 || Index >= ArrayLength) {
		printf("Error reading character! Index = %d is out of the array bounds [%d, %d]!\n", Index, 0, ArrayLength);
		int *Pointer = 0;
		*Pointer = 1;
		return Array[Index];
	} else {
		return Array[Index];
	}
}
int main(int argc, char const *argv[]) 
{
	clock_t t; 
	t = clock();

	FILE *FilePointer = fopen("./Day24_test.txt", "r");
	char *Line = (char *)malloc(MAXCHAR);
	size_t Count; ssize_t nread;
	if (FilePointer == NULL) { printf("Cannot open file!\n"); return -1; }

	int TapeLength = 100001;
	char *Tape = (char *)calloc(TapeLength, sizeof(char));

	int Offset = (TapeLength-1)/2;
	int Coord = 0;
	char State = 'A';
	int CheckSum = 0;
	for (int i = 0; i < 12794428; i++) {
		switch (State) {
			case 'A': {
				if (SafeCharRead(Tape, TapeLength, Coord+Offset) == 0) {
					SafeCharWrite(Tape, TapeLength, Coord+Offset, 1);
					CheckSum++;
					Coord++;
					State = 'B';
				} else {
					SafeCharWrite(Tape, TapeLength, Coord+Offset, 0);
					CheckSum--;
					Coord--;
					State = 'F';
				}
			} break;
			case 'B': {
				if (SafeCharRead(Tape, TapeLength, Coord+Offset) == 0) {
					SafeCharWrite(Tape, TapeLength, Coord+Offset, 0);
					Coord++;
					State = 'C';
				} else {
					SafeCharWrite(Tape, TapeLength, Coord+Offset, 0);
					CheckSum--;
					Coord++;
					State = 'D';
				}
			} break;
			case 'C': {
				if (SafeCharRead(Tape, TapeLength, Coord+Offset) == 0) {
					SafeCharWrite(Tape, TapeLength, Coord+Offset, 1);
					CheckSum++;
					Coord--;
					State = 'D';
				} else {
					SafeCharWrite(Tape, TapeLength, Coord+Offset, 1);
					Coord++;
					State = 'E';
				}
			} break;
			case 'D': {
				if (SafeCharRead(Tape, TapeLength, Coord+Offset) == 0) {
					SafeCharWrite(Tape, TapeLength, Coord+Offset, 0);
					Coord--;
					State = 'E';
				} else {
					SafeCharWrite(Tape, TapeLength, Coord+Offset, 0);
					CheckSum--;
					Coord--;
					State = 'D';
				}
			} break;
			case 'E': {
				if (SafeCharRead(Tape, TapeLength, Coord+Offset) == 0) {
					SafeCharWrite(Tape, TapeLength, Coord+Offset, 0);
					Coord++;
					State = 'A';
				} else {
					SafeCharWrite(Tape, TapeLength, Coord+Offset, 1);
					Coord++;
					State = 'C';
				}
			} break;
			case 'F': {
				if (SafeCharRead(Tape, TapeLength, Coord+Offset) == 0) {
					SafeCharWrite(Tape, TapeLength, Coord+Offset, 1);
					CheckSum++;
					Coord--;
					State = 'A';
				} else {
					SafeCharWrite(Tape, TapeLength, Coord+Offset, 0);
					CheckSum--;
					Coord++;
					State = 'A';
				}
			} break;
		}
	}
	printf("CheckSum = %d\n", CheckSum);

	t = clock()-t;
	printf ("It took me %lu clicks (%f seconds) for part 1.\n", t,((float)t)/CLOCKS_PER_SEC);
	return 0;
}