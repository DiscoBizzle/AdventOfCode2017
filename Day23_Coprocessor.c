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
	int Arg1Index = 0;
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

	} else if (strcmp(OpCode, "sub") == 0) {
		if (Arg2Exists) {
			Program->Registers[Arg1Index] -= Arg2Value;
		} else {
			printf("Error! Invalid 'sub' command. Second argument was not a valid register or integer\n");
		}

	} else if (strcmp(OpCode, "mul") == 0) {
		if (Arg2Exists) {
			Program->Registers[Arg1Index] *= Arg2Value;
			Program->Registers['z'-'a']++; // Keep track of the number of mul commands in the z register
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

	} else if (strcmp(OpCode, "jnz") == 0) {
		if (Arg2Exists) {
			if (Arg1Value != 0) {
				Program->InstructionIndex += (Arg2Value - 1); // NOTE: the -1 is required because InstructionIndex will be incremented at the end of the loop.
				if (Program->InstructionIndex < -1 || Program->InstructionIndex >= (Program->Length-1)) {
					printf("InstructionIndex = %d, Terminating Program %d\n", Program->InstructionIndex+1, Program->Index);
					Program->Status = STATUS_DEAD; // Kill the program if we jump outside of the bounds
					return;
				}
			}
		}
			
	} else {
		printf("Error! Instruction '%s' not recognised!\n", OpCode);
	}
	Program->InstructionIndex++;
}

int main(int argc, char const *argv[]) 
{
	// Part 1 (copied from Day 18)
	FILE *FilePointer = fopen("./Day23_input.txt", "r");
	char *Line = (char *)malloc(MAXCHAR);
	size_t Count; ssize_t nread;
	if (FilePointer == NULL) { printf("Cannot open file!\n"); return -1; }

	char *WordArray[MAXWORDS];
	char *Instructions[MAXWORDS]; 

	int InstructionIndex = 0;
	while ((nread = getline(&Line, &Count, FilePointer)) != -1) {
		int NumWords = LineToWordArray(WordArray, Line);
		
		printf("%d: ", InstructionIndex);
		PrintWordArray(WordArray, NumWords);
		InsertWordArrayIntoProgram(Instructions, InstructionIndex, WordArray, NumWords);
		InstructionIndex++;
	}
	int NumInstructions = InstructionIndex;

	program Program0 = {};

	Program0.Index = 0;
	Program0.Status = STATUS_RUNNING;
	Program0.InstructionIndex = 0;
	Program0.Length = NumInstructions;
	Program0.Instructions = Instructions; 

	Program0.Registers = (int64 *)calloc(26, sizeof(int64)); 
	Program0.Registers['a'-'a'] = 0; 

	Program0.MessageQueue = (int64 *)calloc(MAXMESSAGES, sizeof(int64));
	Program0.NumMessages = 0;

	int TotalSentBy0 = 0;

	int RunCount = 0;
	while (RunCount < 100) {
		while (Program0.Status == STATUS_RUNNING) {
			// NOTE: Program 0 reads from queue 0 and writes to queue 1 and vice-versa
			ExecuteInstructionStruct(&Program0, Program0.MessageQueue, &Program0.NumMessages, &TotalSentBy0);
			RunCount++;
			if (RunCount%10000 == 0) {
				// printf("RunCount = %d, h = %lld\n", RunCount, Program0.Registers['h'-'a']);
			}
		}
		if (Program0.Status != STATUS_RUNNING) {
			break;
		}
	}
	PrintRegisters(Program0.Registers, 8, 0);
	printf("TotalMulCommands = %lld\n", Program0.Registers['z'-'a']);

	int64 a = 1; int64 b = 0; int64 c = 0; int64 d = 0; int64 e = 0; int64 f = 0; int64 g = 0; int64 h = 0; 
	int NumMuls = 0;

	// Part 2 - computes how many composites are in the sequence b = 107900 + m*17.
	b = 107900;
	c = b + 17000; 
	for (; b <= c; b += 17) {
		bneqc: f = 1;
		d = 2;
		for (d = 2; d < b; d++) {
			if (b%d == 0) {
				h++;
				break;
			}
		}
	}

	printf("0: a = %llu, b = %llu, c = %llu, d = %llu, e = %llu, f = %llu, g = %llu, h = %llu,\n", a, b, c, d, e, f, g, h);
	printf("NumMuls = %d, h = %lld\n", NumMuls, h);
	return 0;
}