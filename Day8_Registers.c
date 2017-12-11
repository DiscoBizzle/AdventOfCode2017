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

#define NAMELEN 10
#define NUMREGS 1500

typedef struct _reg {
	char Name[NAMELEN];
	int Val;
} reg;

reg *FindRegister(reg *Registers, int NumRegs, char *Name)
{
	for (int i = 0; i < NumRegs; i++) {
		// printf("Comparing \"%s\" to \"%s\"\n", Name, Registers[i].Name);
		if (strcmp(Registers[i].Name, Name) == 0){
			// printf("Match! returning\n");
			return &(Registers[i]);
		}
		// printf("No match! Continuing\n");
	}
	return 0;
}

void PrintRegisters(reg *Registers, int NumRegs) {
	for (int i = 0; i < NumRegs; i++) {
		printf("%s (%d): %d\n", Registers[i].Name, i, Registers[i].Val);
	}
}

int CheckCondition(char **WordArray, reg *Registers, int *NumRegisters)
{
	reg *ConditionRegister = FindRegister(Registers, *NumRegisters, WordArray[4]);
	int ConditionNumber = atoi(WordArray[6]);
	int Condition = -1;

	if (ConditionRegister == 0) {
		if (*NumRegisters < NUMREGS) {
			strcpy(Registers[*NumRegisters].Name, WordArray[4]);
			ConditionRegister = &Registers[*NumRegisters];
			*NumRegisters = *NumRegisters + 1;
		}
	}

	// printf("The comparitor is \"%s\"\n", WordArray[5]);

	if (strcmp(WordArray[5], "==") == 0) {
		Condition = (ConditionRegister->Val == ConditionNumber);
	} else if (strcmp(WordArray[5], "!=") == 0) {
		Condition = (ConditionRegister->Val != ConditionNumber);
	} else if (strcmp(WordArray[5], "<=") == 0) {
		Condition = (ConditionRegister->Val <= ConditionNumber);
	} else if (strcmp(WordArray[5], ">=") == 0) {
		Condition = (ConditionRegister->Val >= ConditionNumber);
	} else if (strcmp(WordArray[5], "<") == 0) {
		Condition = (ConditionRegister->Val < ConditionNumber);
	} else if (strcmp(WordArray[5], ">") == 0) {
		Condition = (ConditionRegister->Val > ConditionNumber);
	}

	if (Condition == -1) {
		printf("Error! Condition not checked for!\n");
	}
	return Condition;
}

void ModifyRegister(char **WordArray, reg *Registers, int *NumRegisters)
{
	reg *Reg = FindRegister(Registers, *NumRegisters, WordArray[0]);
	int ModifyNumber = atoi(WordArray[2]);

	if (Reg == 0) {
		if (*NumRegisters < NUMREGS) {
			strcpy(Registers[*NumRegisters].Name, WordArray[0]);
			Reg = &Registers[*NumRegisters];
			*NumRegisters = *NumRegisters + 1;
		}
	}

	if (strcmp(WordArray[1], "inc") == 0) {
		Reg->Val += ModifyNumber;
	} else if (strcmp(WordArray[1], "dec") == 0) {
		Reg->Val -= ModifyNumber;
	} else {
		printf("Error in ModifyRegister, instruction not recognised\n");
	}
}

int main(int argc, char const *argv[]) 
{
	FILE *FilePointer = fopen("./Day8_input.txt", "r");
	char *Line = (char *)malloc(MAXCHAR);
	size_t Count; ssize_t nread;
	if (FilePointer == NULL) { printf("Cannot open file!\n"); return -1; }

	reg *Registers = (reg *)malloc(sizeof(reg)*NUMREGS);
	char *WordArray[MAXWORDS];
	int NumRegisters = 0;
	int HighestHeld = 0;
	while ((nread = getline(&Line, &Count, FilePointer)) != -1) {
		int NumWords = LineToWordArray(WordArray, Line);
		// PrintWordArray(WordArray, NumWords);

		if (CheckCondition(WordArray, Registers, &NumRegisters)) {
			// printf("Condition True! Modifying register\n");
			ModifyRegister(WordArray, Registers, &NumRegisters);
			
			for (int i = 1; i < NumRegisters; i++) {
				if (Registers[i].Val > HighestHeld) {
					HighestHeld = Registers[i].Val;
				}
			}
		}
		// PrintRegisters(Registers, NumRegisters);
		for (int i = 0; i < NumWords; i++) { free(WordArray[i]); } 
	}
	int CurrMax = Registers[0].Val;
	for (int i = 1; i < NumRegisters; i++) {
		if (Registers[i].Val > CurrMax) {
			CurrMax = Registers[i].Val;
		}
	}
	printf("The largest register value is %d\n", CurrMax);
	printf("The largest value held is %d\n", HighestHeld);

	// PrintTree(Tree);
	return 0;
}