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

#define MAXCHAR 1000
#define MAXWORDS 100
 
int LineToWordArray(char **WordArray, char *Line) {
	int CharIndex = 0;
	int LineIndex = 0;
	int WordIndex = 0;
	char CurrWord[MAXCHAR];
	int Running = 1;
	while (Running) {
		if (Line[LineIndex] == '\n') {
			CurrWord[CharIndex] = 0;
			WordArray[WordIndex] = (char*)malloc(MAXCHAR);
			strcpy(WordArray[WordIndex], CurrWord);
			Running = false;
			WordIndex++;
		} else if (Line[LineIndex] == ' ' || Line[LineIndex] == '\t') {
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

#define NAMELEN 10
#define NUMNODES 1416
//#define NUMNODES 13
#define NUMCONNECTIONS 10

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

typedef struct _node {
	char Name[NAMELEN];
	int Weight;
	int TotalWeight;

	int ComesFrom[NUMCONNECTIONS];
	int NumComesFrom;
	int GoesTo[NUMCONNECTIONS];
	int GoesToWeights[NUMCONNECTIONS];
	int NumGoesTo;
} node;

typedef struct _sentence {
	char *WordArray[MAXWORDS];
	int NumWords;
} sentence;

node GetNodeFromWordArray(char **WordArray, int NumWords)
{
	node Node = {};
	strcpy(Node.Name, WordArray[0]);
	//StripParentheses(WordArray[1]);
	Node.Weight = atoi(WordArray[1]);
	if (NumWords < 3) {
		// printf("No forward legs. On Layer 0\n");
		Node.NumGoesTo = 0;
	} else {
		Node.NumGoesTo = NumWords - 3;
	}
	Node.NumComesFrom = 0;
	Node.TotalWeight = 0;
	return Node;
}

void PrintNode(node Node, int Index)
{
	printf("%d \t %s (%d) ((%d)) ", Index, Node.Name, Node.Weight, Node.TotalWeight);
	printf("GoesTo = {");
	for (int i = 0; i < Node.NumGoesTo; i++) {
		printf("%d, ", Node.GoesTo[i]);
	}
	printf("} ComesFrom {");
	for (int i = 0; i < Node.NumComesFrom; i++) {
		printf("%d", Node.ComesFrom[i]);
	}
	printf("}\n");
}

void PrintTree(node *Tree)
{
	for (int i = 0; i < NUMNODES; i++) {
		PrintNode(Tree[i], i);
	}
}

void ConnectTree(node *Tree, sentence *RawTree)
{
	for (int i = 0; i < NUMNODES; i++) {
		if (RawTree[i].NumWords > 3) {
			for (int GoesToIndex = 0; GoesToIndex < Tree[i].NumGoesTo; GoesToIndex++) {
				for (int j = 0; j < NUMNODES; j++){
					if (strcmp(Tree[j].Name, RawTree[i].WordArray[3+GoesToIndex]) == 0) {
						Tree[i].GoesTo[GoesToIndex] = j;
						Tree[j].ComesFrom[Tree[j].NumComesFrom++] = i;
						break;
					}
				}
			}
		}
	}
}

int CheckDifferent(int *List, int L, int *DiffIndex) 
{
	// NOTE: This expects exactly one item different from the others
	// printf("List = {");
	// for (int i = 0; i < L; i++) {
	// 	printf("%d, ", List[i]);
	// }
	// printf("}\n");
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

int SumWeights(node *Tree, int NodeIndex) 
{
	Tree[NodeIndex].TotalWeight = Tree[NodeIndex].Weight;
	if (Tree[NodeIndex].NumGoesTo == 0) {
		return Tree[NodeIndex].Weight;
	} else {
		for (int i = 0; i < Tree[NodeIndex].NumGoesTo; i++) {
			Tree[NodeIndex].GoesToWeights[i] = SumWeights(Tree, Tree[NodeIndex].GoesTo[i]);
			Tree[NodeIndex].TotalWeight += Tree[NodeIndex].GoesToWeights[i];
		} 
		int DiffNodeIndex = -1;
		int WeightDifference = CheckDifferent(Tree[NodeIndex].GoesToWeights, Tree[NodeIndex].NumGoesTo, &DiffNodeIndex);
		if (WeightDifference != 0) {
			// NOTE: The deepest level of recursion will be printed first. This is the node that should be modified.
			printf("CONNECTED WEIGHTS ARE DIFFERENT!");
			int WrongNodeIndex = Tree[NodeIndex].GoesTo[DiffNodeIndex];
			printf("Next Node (%s) Weight is %d, difference is %d, Weight should be %d \n", 
				Tree[WrongNodeIndex].Name, Tree[WrongNodeIndex].Weight, WeightDifference, Tree[WrongNodeIndex].Weight - WeightDifference); 
			PrintNode(Tree[NodeIndex], NodeIndex);
			PrintNode(Tree[WrongNodeIndex], WrongNodeIndex);
			int *np = 0;
			*np = 1;
		}
		return Tree[NodeIndex].TotalWeight;
	}
}

int main(int argc, char const *argv[]) 
{
	FILE *FilePointer = fopen("./Day7_input.txt", "r");
	char *Line = (char *)malloc(MAXCHAR);
	size_t Count; ssize_t nread;
	if (FilePointer == NULL) { printf("Cannot open file!\n"); return -1; }

	node *Tree = (node *)malloc(sizeof(node) * NUMNODES);
	sentence *RawTree = (sentence *)malloc(sizeof(sentence)*NUMNODES);
	// Read the file and convert the string into an array of nodes.
	int NodeIndex = 0;
	while ((nread = getline(&Line, &Count, FilePointer)) != -1) {
		RawTree[NodeIndex].NumWords = LineToWordArray(RawTree[NodeIndex].WordArray, Line);
		StripParentheses(RawTree[NodeIndex].WordArray[1]);
		if (RawTree[NodeIndex].NumWords > 4) {
			for (int i = 3; i < RawTree[NodeIndex].NumWords-1; i++) {
				StripLastChar(RawTree[NodeIndex].WordArray[i]);
			}
		}
		// PrintWordArray(RawTree[NodeIndex].WordArray, RawTree[NodeIndex].NumWords);
		Tree[NodeIndex] = GetNodeFromWordArray(RawTree[NodeIndex].WordArray, RawTree[NodeIndex].NumWords);
		// PrintNode(Tree[NodeIndex]);
		NodeIndex++;
	}

	ConnectTree(Tree, RawTree);
	// PrintTree(Tree);
	int RootIndex = 0;
	for (int i = 0; i < NUMNODES; i++) {
		if (Tree[i].NumComesFrom == 0) {
			RootIndex = i;
			printf("Root node is %s\n", Tree[i].Name);
			break;
		}
	}

	SumWeights(Tree, RootIndex);

	printf("Weights attached to wrongnode (%s)\n", Tree[1330].Name);
	for(int i = 0; i < Tree[1330].NumGoesTo; i++) {
		int CurrWeight = Tree[Tree[1330].GoesTo[i]].TotalWeight;
		printf("%d, ", CurrWeight);
	}
	printf("\n");
	// PrintTree(Tree);
	return 0;
}