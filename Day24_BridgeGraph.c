#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h> 

#define MAXCHAR 100
#define MAXWORDS 100
#include "Utilities.c"

#define MAXCONNS 10

typedef struct _node {
	int valA, valB;
	int NumAIn, NumBIn; 
	int NumAOut, NumBOut;
	int *AIn;
	int *AOut;
	int *BIn;
	int *BOut; 
} node;

node NodeAlloc(int A, int B) {
	node Node;
	Node.valA = A;
	Node.valB = B;
	Node.NumAIn = Node.NumBIn = Node.NumAOut = Node.NumBOut = 0;

	Node.AIn = (int *)calloc(MAXCONNS, sizeof(int));
	Node.AOut = (int *)calloc(MAXCONNS, sizeof(int));
	Node.BIn = (int *)calloc(MAXCONNS, sizeof(int));
	Node.BOut = (int *)calloc(MAXCONNS, sizeof(int));

	return Node; 
}

void NodeFree(node *Node) 
{
	free(Node->AIn);
	free(Node->AOut);
	free(Node->BIn);
	free(Node->BOut);
	Node->valA = Node->valB = 0;
	Node->NumAIn = Node->NumAOut = Node->NumBIn = Node->NumBOut = 0;
}

// Recursive depth-first search
void NodeSearch(node *FullList, int NumNodes, int *VisitedNodes, int NumVisited, int CurrVal, int CurrNode, int *MaxStrength, 
					int CurrStrength, int *StrongestBridge, int *BridgeLength, int *LongestBridge, int *LongestLength, int *LongestStrength) 
{
	// Mark the currnode as visited
	VisitedNodes[NumVisited] = CurrNode;
	NumVisited++;

	CurrStrength += (FullList[CurrNode].valA + FullList[CurrNode].valB); 
	if (CurrStrength > (*MaxStrength)) {
		*MaxStrength = CurrStrength; 
		for (int i = 0; i < NumVisited; i++) {
			StrongestBridge[i] = VisitedNodes[i];
		}
		*BridgeLength = NumVisited;
	}
	if (NumVisited > (*LongestLength)) {
		*LongestStrength = CurrStrength;
		for (int i = 0; i < NumVisited; i++) {
			LongestBridge[i] = VisitedNodes[i];
		}	
		*LongestLength = NumVisited;
	} else if (NumVisited == (*LongestLength)) { // if it is not longer, but we tie
		if (CurrStrength > (*LongestStrength)) {
			*LongestStrength = CurrStrength;
			for (int i = 0; i < NumVisited; i++) {
				LongestBridge[i] = VisitedNodes[i];
			}	
			*LongestLength = NumVisited;
		}
	}

	for (int i = 0; i < NumNodes; i++) { // For every node
		int Visited = 0;
		for (int VisitedIndex = 0; VisitedIndex < NumVisited; VisitedIndex++) { // Check whether we have visited the node
			if (VisitedNodes[VisitedIndex] == i) { 
				Visited = 1;
			}
		}
		if (Visited) {
			continue; 
		}
		// If we haven't visited it...
		if (FullList[i].valA == CurrVal) { // If the node is compatible, move to that node.
			NodeSearch(FullList, NumNodes, VisitedNodes, NumVisited, FullList[i].valB, i, MaxStrength, CurrStrength, StrongestBridge, BridgeLength, LongestBridge, LongestLength, LongestStrength);
		} else if (FullList[i].valB == CurrVal) {
			NodeSearch(FullList, NumNodes, VisitedNodes, NumVisited, FullList[i].valA, i, MaxStrength, CurrStrength, StrongestBridge, BridgeLength, LongestBridge, LongestLength, LongestStrength);
		}
	}
	// If we have gone through all the nodes and not found a compatible one, go up one level
}

int main(int argc, char const *argv[]) 
{
	clock_t t; 
	t = clock();

	FILE *FilePointer = fopen("./Day24_input.txt", "r");
	char *Line = (char *)malloc(MAXCHAR);
	size_t Count; ssize_t nread;
	if (FilePointer == NULL) { printf("Cannot open file!\n"); return -1; }

	char *WordArray[MAXWORDS];
	int NumNodes = 1;
	node FullList[55];
	FullList[0] = NodeAlloc(0, 0);
	while ((nread = getline(&Line, &Count, FilePointer)) != -1) {
		int NumWords = LineToWordArrayChooseSplit(WordArray, Line, "/", 1);
		PrintWordArray(WordArray, NumWords);
		FullList[NumNodes] = NodeAlloc(atoi(WordArray[0]), atoi(WordArray[1]));
		NumNodes++;
	}
	printf("NumNodes = %d\n", NumNodes);
	
	int VisitedNodes[55];
	int NumVisited = 0;

	int MaxStrength = 0;
	int StrongestBridge[55];
	int StrongestLength = 0;

	int LongestBridge[55];
	int LongestLength = 0;
	int LongestStrength = 0; 
	NodeSearch(FullList, NumNodes, VisitedNodes, NumVisited, 0, 0, &MaxStrength, 0, StrongestBridge, &StrongestLength, LongestBridge, &LongestLength, &LongestStrength);

	printf("Strength of strongest bridge = %d\n", MaxStrength);
	for (int i = 0; i < StrongestLength; i++) {
		printf("(%d, %d) -- ", FullList[StrongestBridge[i]].valA, FullList[StrongestBridge[i]].valB);
	}
	printf("\n");
	printf("Length of longest bridge = %d, Strength = %d\n", LongestLength, LongestStrength);
	for (int i = 0; i < LongestLength; i++) {
		printf("(%d, %d) -- ", FullList[LongestBridge[i]].valA, FullList[LongestBridge[i]].valB);
	}
	printf("\n");

	t = clock()-t;
	printf ("It took me %lu clicks (%f seconds) for part 1 and 2.\n", t,((float)t)/CLOCKS_PER_SEC);
	return 0;
}