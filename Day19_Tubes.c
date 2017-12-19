#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAXCHAR 1000
#define MAXROWS 1000
#define MAXCOLS 1000
#include "Utilities.c"

int main(int argc, char const *argv[]) 
{
	FILE *FilePointer = fopen("./Day19_input.txt", "r");
	char *Line = (char *)malloc(MAXCHAR);
	size_t Count; ssize_t nread;
	if (FilePointer == NULL) { printf("Cannot open file!\n"); return -1; }
	char *Matrix[MAXROWS];

	int NumRows = 0;
	int NumCols = 0;
	while ((nread = getline(&Line, &Count, FilePointer)) != -1) {
		Matrix[NumRows] = (char *)calloc(MAXCOLS, MAXROWS);
		for (int i = 0;;i++) {
			if (Line[i] == '\n' || Line[i] == 0) {
				NumCols = i;
				break;
			} else {
				Matrix[NumRows][i] = Line[i];
			}
		}
		NumRows++;
	}
	printf("NumCols = %d\n", NumCols);
	printf("NumRows = %d\n", NumRows);

	int y = 0;
	int x = -1;
	for (int i = 0; i < NumCols; i++) {
		if (Matrix[y][i] == '|') {
			x = i;
			break;
		}
	}
	if (x < 0) {
		printf("Error! Could not find the starting position.\n");
		return -1;
	} else {
		printf("Starting Coordinates are (x = %d, y = %d)\n", x, y);
	}

	int dx = 0;
	int dy = 1;
	char Letters[MAXCHAR];
	Letters[0] = 0;
	int CharIndex = 0;
	int NumSteps = 1; // The first step "onto the grid" counts as a step.
	while (true) {
		x += dx;
		y += dy;
		NumSteps++;
		// printf("(x = %d, y = %d))\n", x, y);
		if ('A' <= Matrix[y][x] && Matrix[y][x] <= 'Z') {
			// printf("Found character %c\n", Matrix[y][x]);
			Letters[CharIndex] = Matrix[y][x];
			Letters[CharIndex+1] = 0;
			CharIndex++;
			// printf("Letters = %s\n", Letters);
		} else if (Matrix[y][x] == '|' || Matrix[y][x] == '-') {
			// continue 
		} else if (Matrix[y][x] == '+') {
			// If we reach a corner we first check left, then right to see whether the path continues.
			// NOTE: If the pipes wind too closely together this could fail as we don't check whether the next
			// pipe continues in the right direction
			int ndx = -dy;
			int ndy = dx; 
			if (0 <= (x+ndx) && (x+ndx) < NumCols && 0 <= (y+ndy) && (y+ndy) < NumRows) {
				if (Matrix[y+ndy][x+ndx] != ' ') {
					dx = ndx;
					dy = ndy;
					continue;
				} 
			}
			ndx = dy;
			ndy = -dx;
			if (0 <= (x+ndx) && (x+ndx) < NumCols && 0 <= (y+ndy) && (y+ndy) < NumRows) {
				if (Matrix[y+ndy][x+ndx] != ' ') {
					dx = ndx;
					dy = ndy;
					continue;
				}
			}
		} else if (Matrix[y][x] == ' ') {
			printf("We walked off the pipe! Terminating\n");
			NumSteps--; // Since the last step wasn't valid, decrement it.
			break;
		} else {
			printf("Error! We ended up on square (x=%d, y=%d) = %c\n", x, y, Matrix[y][x]);
			break;
		}
	}

	printf("Letters collected = %s\n", Letters);
	printf("Number of steps taken = %d\n", NumSteps);
	return 0;
}