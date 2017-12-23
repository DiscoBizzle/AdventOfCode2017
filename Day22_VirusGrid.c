#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h> 

#define MAXCHAR 100
#define MAXWORDS 300
#include "Utilities.c"

typedef struct _charArray {
	char *vals;
	int W, H;
	int MinX, MaxX, MinY, MaxY;
} charArray;

charArray charArrayAlloc(int W, int H)
{
	charArray Array = {};
	Array.W = W;
	Array.H = H;  
	Array.MinX = Array.MinY = Array.MaxX = Array.MaxY;
	Array.vals = (char *)calloc(W*H, sizeof(char));

	for (int j = 0; j < H; j++) {
		for (int i = 0; i < W; i++) {
			Array.vals[j*W + i] = '.';
		}
	}
	return Array;
}

void AddLineToCharArray(charArray *Array, int YCoord, char *Line) {
	int L = strlen(Line)-1;
	int XOffset = (Array->W-1)/2 - (L-1)/2;
	for (int i = 0; i < L; i++) {
		Array->vals[YCoord*Array->W + XOffset+i] = Line[i];
	}
}

void SetCharInArray(charArray *Array, int x, int y, char Val)
{
	if (0 <= x  && x < Array->W && 0 <= y && y < Array->H) {
		if (x < Array->MinX) {
			Array->MinX = x;
		} else if (x > Array->MaxX) {
			Array->MaxX = x;
		} else if (y < Array->MinY) {
			Array->MinY = y;
		} else if (y > Array->MaxY) {
			Array->MaxY = y;
		}
		Array->vals[y*Array->W + x] = Val;
	} else {
		printf("Error! Coordinate (%d, %d) are outside of the boundary.\n", x, y);
		int *Pointer = 0;
		*Pointer = 1;
	}
}

void PrintInfectionGrid(charArray Array, int CurrX, int CurrY)
{
	printf("Min = (%d, %d)\n", Array.MinX, Array.MinY);
	printf("Max = (%d, %d)\n", Array.MaxX, Array.MaxY);
	for (int j = Array.MaxY; j >= Array.MinY; j--) {
		for (int i = Array.MinX; i <= Array.MaxX; i++) {
			if (i == CurrX && j == CurrY) {
				printf("[%c]", Array.vals[j*Array.W+i]);
			} else {	
				printf(" %c ", Array.vals[j*Array.W+i]);
			}
		}
		printf("\n");
	}
}

int main(int argc, char const *argv[]) 
{
	clock_t t; 
	t = clock();

	FILE *FilePointer = fopen("./Day22_input.txt", "r");
	char *Line = (char *)malloc(MAXCHAR);
	size_t Count; ssize_t nread;
	if (FilePointer == NULL) { printf("Cannot open file!\n"); return -1; }

	int GridSize = 1001;
	charArray Grid = charArrayAlloc(GridSize, GridSize);

	int GridCentre = (GridSize-1)/2;
	int InitialYCoord = GridCentre + (25-1)/2;
	// int InitialYCoord = GridCentre + 1;

	int LineIndex = 0;
	while ((nread = getline(&Line, &Count, FilePointer)) != -1) {
		printf("%s", Line);
		int YCoord = InitialYCoord - LineIndex;
		AddLineToCharArray(&Grid, YCoord, Line);
		LineIndex++;
	}
	Grid.MinX = GridCentre-1; 
	Grid.MinY = GridCentre-1;
	Grid.MaxX = GridCentre+1;
	Grid.MaxY = GridCentre+1;

	PrintInfectionGrid(Grid, GridCentre, GridCentre);

	printf("\n\n");
	int CurrX = GridCentre;
	int CurrY = GridCentre;
	int dx = 0; int dy = 1;
	int ndx, ndy;
	int NumIterations = 10000000;

	int NumInfectionsCaused = 0;

	for (int i = 1; i <= NumIterations; i++) {
		switch (Grid.vals[CurrY*Grid.W + CurrX]) {
			case '.': {
				SetCharInArray(&Grid, CurrX, CurrY, 'W');
				// Turn Left
				ndx = -dy;
				ndy = dx;
				dy = ndy;
				dx = ndx;
				CurrX += dx;
				CurrY += dy;
			} break;
			case '#': {
				SetCharInArray(&Grid, CurrX, CurrY, 'F');
				// Turn right
				ndx = dy;
				ndy = -dx;
				dy = ndy;
				dx = ndx;
				CurrX += dx;
				CurrY += dy;
			} break;
			case 'W': {
				SetCharInArray(&Grid, CurrX, CurrY, '#');
				NumInfectionsCaused++;
				// Continues in same direction
				CurrX += dx;
				CurrY += dy;
			} break;
			case 'F': {
				SetCharInArray(&Grid, CurrX, CurrY, '.');
				// Go in reverse diraction
				dx = -dx;
				dy = -dy;
				CurrX += dx;
				CurrY += dy;
			} break;
			default: {
				printf("Error! Character not recognised!\n");
			}
		}
		// PrintInfectionGrid(Grid, CurrX, CurrY);
	}

	printf("NumIterations = %d, NumInfectionsCaused = %d\n", NumIterations, NumInfectionsCaused);
	
	t = clock()-t;
	printf ("It took me %lu clicks (%f seconds) for part 1.\n", t,((float)t)/CLOCKS_PER_SEC);
	return 0;
}