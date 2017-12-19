#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAXCHAR 10
#define MAXWORDS 300
#include "Utilities.c"

typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;

void InsertWordArrayIntoProgram(char **Program, int InstructionIndex, char **WordArray, int NumWords)
{
	int InsertionIndex = 3*InstructionIndex;
	for (int i = 0; i < 3; i++) {
		Program[InsertionIndex + i] = (char *)calloc(MAXCHAR, sizeof(char));
		if (i < NumWords) {
			sprintf(Program[InsertionIndex + i], "%s", WordArray[i]);
		}
	}
}

void PrintProgram(char **Program, int NumInstructions)
{
	for (int i = 0; i < NumInstructions; i++) {
		printf("%d: ", i);
		for (int j = 0; j < 3; j++) {
			printf("%s / ", Program[3*i+j]);
		}
		printf("\n");
	}
} 

typedef struct _program {
	int Index;
	int Status;

	char **Instructions;
	int InstructionIndex;
	int Length;

	int64 *Registers; 
	int64 *MessageQueue;
	int NumMessages;
} program;

void PrintRegisters(int64 *Registers, int NumRegisters, int ProgramIndex)
{
	printf("%d: ", ProgramIndex);
	for (int i = 0; i < NumRegisters; i++) {
		printf("%c = %lld, ", i+'a', Registers[i]);
	}
	printf("\n");
}

#define MAXMESSAGES 1000
#define STATUS_RUNNING 1
#define STATUS_PAUSED 2
#define STATUS_DEAD 0

void ExecuteInstructionStruct(program *Program, int64 *MessagesSent, int *NumSent, int *TotalSent)
{
	// "Decode" instruction
	char *OpCode = Program->Instructions[3*Program->InstructionIndex];

	// Arg 2 (if it exists) is always a value.
	char *Arg2 = Program->Instructions[3*Program->InstructionIndex+2];
	int64 Arg2Value; 
	int Arg2Exists = 0;
	if ('a' <= Arg2[0] && Arg2[0] <= 'z') {
		int RegIndex = Arg2[0] - 'a';
		Arg2Value = Program->Registers[RegIndex];
		Arg2Exists = 1;
	} else if (('0' <= Arg2[0] && Arg2[0] <= '9') || (Arg2[0] == '-')) {
		Arg2Value = atoi(Arg2);
		Arg2Exists = 1;
	} else {
		Arg2Exists = 0;
	}

	// NOTE: Argument 2 will either correspond to a value to use, or a register address to store a value in. We will compute both and 
	// the instruction decoder will decide which one to use depending on the context. 
	char *Arg1 = Program->Instructions[3*Program->InstructionIndex+1];
	int Arg1Index = 0; // NOTE: 0 means error. 1 means Register, and we give an index. 2 means Value and we provide the value
	int64 Arg1Value = 0;

	if ('a' <= Arg1[0] && Arg1[0] <= 'z') {
		Arg1Index = Arg1[0] - 'a';
		Arg1Value = Program->Registers[Arg1Index];
	} else if (('0' <= Arg1[0] && Arg1[0] <= '9') || (Arg1[0] == '-')) {
		Arg1Value = atoi(Arg1);
	} else {
		printf("Error! Arg1 did not correspond to a register or a numerical value\n");
	}

	// Execute instruction
	if (strcmp(OpCode, "snd") == 0) {
		MessagesSent[*NumSent] = Arg1Value;
		*NumSent = (*NumSent) + 1;
		*TotalSent = (*TotalSent) + 1;

	} else if (strcmp(OpCode, "set") == 0) {
		if (Arg2Exists) {
			Program->Registers[Arg1Index] = Arg2Value;
		} else {
			printf("Error! Invalid 'set' command. Second arguement was not a valid register or an integer\n");
		}

	} else if (strcmp(OpCode, "add") == 0) {
		if (Arg2Exists) {
			Program->Registers[Arg1Index] += Arg2Value;
		} else {
			printf("Error! Invalid 'add' command. Second argument was not a valid register or integer\n");
		}

	} else if (strcmp(OpCode, "mul") == 0) {
		if (Arg2Exists) {
			Program->Registers[Arg1Index] *= Arg2Value;
		} else {
			printf("Error! Invalid 'mul' command. Second argument was not a valid register or integer\n");
		}

	} else if (strcmp(OpCode, "mod") == 0) {
		if (Arg2Exists) {
			Program->Registers[Arg1Index] %= Arg2Value;
		} else {
			printf("Error! Invalid 'mod' command. Second argument was not a valid register or integer\n");
		}

	} else if (strcmp(OpCode, "rcv") == 0) {
		if (Program->NumMessages > 0) {
			// If we have an unread message, pull it off the queue
			Program->Registers[Arg1Index] = Program->MessageQueue[0];
			for (int i = 1; i < Program->NumMessages; i++) {
				Program->MessageQueue[i-1] = Program->MessageQueue[i];
			}
			Program->NumMessages--;
		} else if (Program->NumMessages == 0) {
			Program->Status = STATUS_PAUSED;
			return; // NOTE: We want to return without incrementing the instructionIndex
		} else {
			printf("Error! We have a negative number (%d) of unread messages! \n", Program->NumMessages);
		}

	} else if (strcmp(OpCode, "jgz") == 0) {
		if (Arg2Exists) {
			if (Arg1Value > 0) {
				Program->InstructionIndex += (Arg2Value - 1); // NOTE: the -1 is required because InstructionIndex will be incremented at the end of the loop.
				if (Program->InstructionIndex < -1 || Program->InstructionIndex >= (Program->Length-1)) {
					printf("InstructionIndex = %d, Terminating Program %d\n", Program->InstructionIndex+1, Program->Index);
					Program->Status = STATUS_DEAD; // Kill the program if we jump outside of the bounds
					return;
				}
			}
		}
	} else {
		printf("Error! Instruction not recognised!\n");
	}
	Program->InstructionIndex++;
}

int main(int argc, char const *argv[]) 
{
	FILE *FilePointer = fopen("./Day18_input.txt", "r");
	char *Line = (char *)malloc(MAXCHAR);
	size_t Count; ssize_t nread;
	if (FilePointer == NULL) { printf("Cannot open file!\n"); return -1; }

	char *WordArray[MAXWORDS];
	char *Instructions[MAXWORDS]; 

	int InstructionIndex = 0;
	while ((nread = getline(&Line, &Count, FilePointer)) != -1) {
		int NumWords = LineToWordArray(WordArray, Line);
		
		InsertWordArrayIntoProgram(Instructions, InstructionIndex, WordArray, NumWords);
		InstructionIndex++;
	}
	int NumInstructions = InstructionIndex;

	// TODO: Bounds checking on the queues. 
	// TODO: Learn how multithreading works and actually multithread this.
	program Program0 = {};

	Program0.Index = 0;
	Program0.Status = STATUS_RUNNING;
	Program0.InstructionIndex = 0;
	Program0.Length = NumInstructions;
	Program0.Instructions = Instructions; 

	Program0.Registers = (int64 *)calloc(26, sizeof(int64)); 
	Program0.MessageQueue = (int64 *)calloc(MAXMESSAGES, sizeof(int64));
	Program0.NumMessages = 0;

	program Program1 = {};

	Program1.Index = 0;
	Program1.Status = STATUS_PAUSED;
	Program1.InstructionIndex = 0;
	Program1.Length = NumInstructions;
	Program1.Instructions = Instructions;

	Program1.Registers = (int64 *)calloc(26, sizeof(int64)); 
	Program1.Registers['p'-'a'] = 1;
	Program1.MessageQueue = (int64 *)calloc(MAXMESSAGES, sizeof(int64));
	Program1.NumMessages = 0;

	int TotalSentBy0 = 0;
	int TotalSentBy1 = 0;

	while (1) {
		while (Program0.Status == STATUS_RUNNING) {
			// NOTE: Program 0 reads from queue 0 and writes to queue 1 and vice-versa
			ExecuteInstructionStruct(&Program0, Program1.MessageQueue, &Program1.NumMessages, &TotalSentBy0);
		}
		if (Program1.Status == STATUS_PAUSED && Program1.NumMessages > 0) {
			Program1.Status = STATUS_RUNNING; 
		}

		while (Program1.Status == STATUS_RUNNING) {
			ExecuteInstructionStruct(&Program1, Program0.MessageQueue, &Program0.NumMessages, &TotalSentBy1);
		}
		if (Program0.Status == STATUS_PAUSED && Program0.NumMessages > 0) {
			Program0.Status = STATUS_RUNNING;
		}

		if (Program0.Status != STATUS_RUNNING && Program1.Status != STATUS_RUNNING) {
			break;
		}
	}
	printf("TotalSentBy1 = %d\n", TotalSentBy1);

	return 0;
}