#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAXCHAR 100000
#define MAXWORDS 10000
 
int LineToWordArray(char **WordArray, char *Line) {
	int CharIndex = 0;
	int LineIndex = 0;
	int WordIndex = 0;
	char CurrWord[MAXCHAR];
	int Running = 1;
	while (Running) {
		if (Line[LineIndex] == '\n' || Line[LineIndex] == 0) {
			CurrWord[CharIndex] = 0;
			WordArray[WordIndex] = (char*)malloc(MAXCHAR);
			strcpy(WordArray[WordIndex], CurrWord);
			Running = false;
			WordIndex++;
		} else if (Line[LineIndex] == ' ' || Line[LineIndex] == ',') {
			CurrWord[CharIndex] = 0;
			WordArray[WordIndex] = (char*)malloc(MAXCHAR);
			strcpy(WordArray[WordIndex], CurrWord);
			LineIndex++; WordIndex++; CharIndex = 0;
		} else {
			CurrWord[CharIndex] = Line[LineIndex];
			CurrWord[CharIndex+1] = 0;
			CharIndex++; LineIndex++; 
		}
	}
	return WordIndex;
}

int Sign(int x) {
	if (x > 0) return 1;
	if (x < 0) return -1;
	return 0;
}

typedef struct _v3i {
	int x;
	int y;
	int z;
} v3i;

v3i OptimalPath(v3i V) {
	// First get rid of the linear dependence by eliminating c using c = b-a.
	v3i VOpt = {V.x-V.z, V.y+V.z, 0};

	// Now, if both a and b have the same sign, this is the decomposition with the miniumum hex distance,
	if (Sign(VOpt.x) == Sign(VOpt.y)) {

	} else if (abs(V.x) >= abs(V.y)) {
		// a is the longer part, so we can convert all of the bs into cs using b = a + c
		VOpt.x += VOpt.y; 
		VOpt.z += VOpt.y; 
		VOpt.y -= VOpt.z;
	} else {
		// b is the longer part, so convert all of the as into cs using a = b-c
		VOpt.y += VOpt.x;
		VOpt.z -= VOpt.x;
		VOpt.x -= VOpt.x;
	}
	return VOpt;
}

int OneNormi(v3i V) {
	return (abs(V.x) + abs(V.y) + abs(V.z));
}

int main(int argc, char const *argv[]) 
{
	FILE *FilePointer = fopen("./Day11_input.txt", "r");
	if (FilePointer == NULL) { printf("Cannot open file!\n"); return -1; }
	char *Line = (char *)malloc(MAXCHAR);
	size_t Count; ssize_t nread;

	char *WordArray[MAXWORDS];

	if ((nread = getline(&Line, &Count, FilePointer)) != -1) {
		// sprintf(Line, "ne,ne,sw,sw"); // Test case
		int NumWords = LineToWordArray(WordArray, Line);
		printf("We read %d words!\n", NumWords);

		v3i RawPath = {0,0,0};
		int MaxDist = 0;

		for (int i = 0; i < NumWords; i++) {
			if (strcmp(WordArray[i], "n") == 0) {
				RawPath.x++;
			} else if (strcmp(WordArray[i], "s") == 0) {
				RawPath.x--;
			} else if (strcmp(WordArray[i], "ne") == 0) {
				RawPath.y++;
			} else if (strcmp(WordArray[i], "sw") == 0) {
				RawPath.y--;
			} else if (strcmp(WordArray[i], "se") == 0) {
				RawPath.z++;
			} else if (strcmp(WordArray[i], "nw") == 0) {
				RawPath.z--;
			} else {
				printf("Error! unrecognised input \"%s\"\n", WordArray[i]);
				return -1;
			}
			int Dist = OneNormi(OptimalPath(RawPath));
			if (Dist > MaxDist) {
				MaxDist = Dist;
			}
		}
		printf("x = %d, y = %d, z = %d\n", RawPath.x, RawPath.y, RawPath.z);
		v3i OptPath = OptimalPath(RawPath);
		printf("Distance (Vec) = %d, ox = %d, oy = %d, oz = %d\n", OneNormi(OptPath), OptPath.x, OptPath.y, OptPath.z);
		printf("Max Distance = %d\n", MaxDist);
	}
	return 0;
}