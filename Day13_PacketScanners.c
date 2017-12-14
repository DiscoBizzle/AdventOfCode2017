#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAXCHAR 1000
#define MAXWORDS 100
#include "Utilities.c"

int main(int argc, char const *argv[]) 
{
	FILE *FilePointer = fopen("./Day13_input.txt", "r");
	char *Line = (char *)malloc(MAXCHAR);
	size_t Count; ssize_t nread;
	if (FilePointer == NULL) { printf("Cannot open file!\n"); return -1; }

	char *WordArray[MAXWORDS];


	int NzLayerIndex = 0;
	int LayerDepth[44]; 
	int LayerIndices[44];
	while ((nread = getline(&Line, &Count, FilePointer)) != -1) {
		int NumWords = LineToWordArray(WordArray, Line);
		StripLastChar(WordArray[0]);
		PrintWordArray(WordArray, NumWords);

		LayerIndices[NzLayerIndex] = atoi(WordArray[0]);
		LayerDepth[NzLayerIndex] = atoi(WordArray[1]);

		NzLayerIndex++;
	}
	int NumNonZeroLayers = NzLayerIndex;
	int NumLayers = atoi(WordArray[0]);
	printf("NumNonZeroLayers = %d, NumLayers = %d\n", NumNonZeroLayers, NumLayers);

	for (int i = 0; i < NumNonZeroLayers; i++) {
		printf("LayerIndices[%d] = %d, LayerDepth[%d] = %d\n",i, LayerIndices[i], i, LayerDepth[i]);
	}

	for (int Delay = 0; Delay < 10000000; Delay++) {
		// printf("Delay = %d\n", Delay);
		int Severity = 0;
		int Caught = 0;
		for (int i = 0; i < NumNonZeroLayers; i++) {
			// printf("LayerIndices[%d] mod 2*(LayerDepth[%d]-1) = %d mod %d\n", i, i, LayerIndices[i], 2*(LayerDepth[i]-1));
			if ((LayerIndices[i]+Delay) % (2*(LayerDepth[i]-1)) == 0) {
				// printf("Collision on layer %d, depth %d\n", LayerIndices[i], LayerDepth[i]);
				Severity += LayerDepth[i]*LayerIndices[i];
				Caught = 1;
				break;
			} 
		}
		// printf("\n");
		if (Caught == 0) {
			printf("Total Severity for Delay %d = %d, Caught = %d\n", Delay, Severity, Caught);
			return 0;
		}

	}					
	return 0;
}