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
	int InstructionIndex;
	int ProgramStatus0;

	int64 *Registers; 
	int64 *MessageQueue;
	int NumMessages;
} Program;

void PrintRegisters(int64 *Registers, int NumRegisters, int ProgramIndex)
{
	printf("%d: ", ProgramIndex);
	for (int i = 0; i < NumRegisters; i++) {
		printf("%c = %lld, ", i+'a', Registers[i]);
	}
	printf("\n");
}

void ExecuteInstruction(int ProgramIndex, int ProgramSize, char **Program, int64 *Registers, int64 *MessagesReceived, int *NumUnread, 
							int64 *MessagesSent, int *NumSent, int *TotalSent, int *pInstructionIndex, int *ProgramStatus)
{
	// "Decode" instruction
	int InstructionIndex = *pInstructionIndex;
	char *OpCode = Program[3*InstructionIndex];

	// Arg 2 (if it exists) is always a value.
	char *Arg2 = Program[3*InstructionIndex+2];
	int64 Arg2Value; 
	int Arg2Exists = 0;
	if ('a' <= Arg2[0] && Arg2[0] <= 'z') {
		int RegIndex = Arg2[0] - 'a';
		Arg2Value = Registers[RegIndex];
		Arg2Exists = 1;
	} else if (('0' <= Arg2[0] && Arg2[0] <= '9') || (Arg2[0] == '-')) {
		Arg2Value = atoi(Arg2);
		Arg2Exists = 1;
	} else {
		Arg2Exists = 0;
	}

	// NOTE: Argument 2 will either correspond to a value to use, or a register address to store a value in. We will compute both and 
	// the instruction decoder will decide which one to use depending on the context. 
	char *Arg1 = Program[3*InstructionIndex+1];
	int Arg1Index = 0; // NOTE: 0 means error. 1 means Register, and we give an index. 2 means Value and we provide the value
	int64 Arg1Value = 0;

	if ('a' <= Arg1[0] && Arg1[0] <= 'z') {
		Arg1Index = Arg1[0] - 'a';
		Arg1Value = Registers[Arg1Index];
	} else if (('0' <= Arg1[0] && Arg1[0] <= '9') || (Arg1[0] == '-')) {
		Arg1Value = atoi(Arg1);
	} else {
		printf("Error! Arg1 did not correspond to a register or a numerical value\n");
	}

	// Execute instruction
	if (strcmp(OpCode, "snd") == 0) {
		printf("Program %d Sending value %lld\n", ProgramIndex, Arg1Value);
		MessagesSent[*NumSent] = Arg1Value;
		*NumSent = (*NumSent) + 1;
		*TotalSent = (*TotalSent) + 1;
	} else if (strcmp(OpCode, "set") == 0) {
		if (Arg2Exists) {
			printf("Setting %c = %lld\n", Arg1Index+'a', Arg2Value);
			Registers[Arg1Index] = Arg2Value;
		} else {
			printf("Error! Invalid 'set' command. Second arguement was not a valid register or an integer\n");
		}
	} else if (strcmp(OpCode, "add") == 0) {
		if (Arg2Exists) {
			printf("Adding %c (%lld) += %lld, result %lld\n", Arg1Index+'a', Arg1Value, Arg2Value, Arg1Value+Arg2Value);
			Registers[Arg1Index] += Arg2Value;
		} else {
			printf("Error! Invalid 'add' command. Second argument was not a valid register or integer\n");
		}
	} else if (strcmp(OpCode, "mul") == 0) {
		if (Arg2Exists) {
			printf("Multiplying %c (%lld) *= %lld, result %lld\n", Arg1Index+'a', Arg1Value, Arg2Value, Arg1Value*Arg2Value);
			Registers[Arg1Index] *= Arg2Value;
		} else {
			printf("Error! Invalid 'mul' command. Second argument was not a valid register or integer\n");
		}
	} else if (strcmp(OpCode, "mod") == 0) {
		if (Arg2Exists) {
			printf("Modulo %c (%lld) %%= %lld, result %lld\n", Arg1Index+'a', Arg1Value, Arg2Value, Arg1Value%Arg2Value);
			Registers[Arg1Index] %= Arg2Value;
		} else {
			printf("Error! Invalid 'mod' command. Second argument was not a valid register or integer\n");
		}
	} else if (strcmp(OpCode, "rcv") == 0) {
		if ((*NumUnread) > 0) {

			Registers[Arg1Index] = MessagesReceived[0];
			for (int i = 1; i < *NumUnread; i++) {
				MessagesReceived[i-1] = MessagesReceived[i];
			}
			*NumUnread = (*NumUnread)-1;
			printf("Program %d Pulled value off queue, %lld. Put it in register %d (%c)\n", ProgramIndex, Registers[Arg1Index], Arg1Index, Arg1Index+'a');
			// *ProgramStatus = 2;

		} else if ((*NumUnread) == 0) {
			printf("Program %d Run out of messages. Pausing\n", ProgramIndex);
			*ProgramStatus = 2;
			return; // Note. We want to return without incrementing the instructionIndex
		} else {
			printf("Error! We have a negative number (%d) of unread messages! \n", *NumUnread);
		}
	} else if (strcmp(Program[3*InstructionIndex], "jgz") == 0) {
		if (Arg2Exists) {
			if (Arg1Value > 0) {
				printf("Jumping from line %d to %d\n", InstructionIndex, (int32)(InstructionIndex+Arg2Value));
				InstructionIndex += (Arg2Value - 1); // NOTE: the -1 is required because InstructionIndex will be incremented at the end of the loop.
				if (InstructionIndex < -1 || InstructionIndex >= (ProgramSize-1)) {
					printf("InstructionIndex = %d, Terminating Program %d\n", InstructionIndex+1, ProgramIndex);
					ProgramStatus = 0;
					return;
				}
			}
		}
	} else {
		printf("Error! Instruction not recognised!\n");
	}
	*pInstructionIndex = InstructionIndex + 1;
}

#define MAXMESSAGES 1000


int main(int argc, char const *argv[]) 
{
	FILE *FilePointer = fopen("./Day18_input.txt", "r");
	char *Line = (char *)malloc(MAXCHAR);
	size_t Count; ssize_t nread;
	if (FilePointer == NULL) { printf("Cannot open file!\n"); return -1; }

	char *WordArray[MAXWORDS];
	char *Program[MAXWORDS]; 

	int InstructionIndex = 0;
	while ((nread = getline(&Line, &Count, FilePointer)) != -1) {
		int NumWords = LineToWordArray(WordArray, Line);
		
		InsertWordArrayIntoProgram(Program, InstructionIndex, WordArray, NumWords);
		InstructionIndex++;
	}

	// TODO: Wrap up this shit in data structures
	// TODO: Learn how multithreading works and actually thread this shit. 

	int NumInstructions = InstructionIndex;
	int InstructionIndex0 = 0;
	int InstructionIndex1 = 0;
	PrintProgram(Program, NumInstructions);

	int ProgramStatus0 = 1;
	int ProgramStatus1 = 2;

	int64 *Registers0 = (int64 *)calloc(26, sizeof(int64)); 
	int64 *Registers1 = (int64 *)calloc(26, sizeof(int64)); 
	Registers1['p'-'a'] = 1;
	int64 SoundFreq = 0; // holds the frequency of the currently playing sound.
	int64 *MessageQueue0 = (int64 *)calloc(MAXMESSAGES, sizeof(int64));
	int NumMessages0 = 0;
	int64 *MessageQueue1 = (int64 *)calloc(MAXMESSAGES, sizeof(int64));
	int NumMessages1 = 0;

	int TotalSentBy0 = 0;
	int TotalSentBy1 = 0;

	while (1) {
		while (ProgramStatus0 == 1) {
			// NOTE: Program 0 reads from queue 0 and writes to queue 1 and vice-versa
			ExecuteInstruction(0, NumInstructions, Program, Registers0, MessageQueue0, &NumMessages0,  
								MessageQueue1, &NumMessages1, &TotalSentBy0, &InstructionIndex0, &ProgramStatus0);
			PrintRegisters(Registers0, 4, 0);
		}
		if (ProgramStatus1 == 2 && NumMessages1 > 0) {
			ProgramStatus1 = 1; 
		}
		while (ProgramStatus1 == 1) {
			ExecuteInstruction(1, NumInstructions, Program, Registers1, MessageQueue1, &NumMessages1,  
								MessageQueue0, &NumMessages0, &TotalSentBy1, &InstructionIndex1, &ProgramStatus1);
			PrintRegisters(Registers1, 4, 1);
		}
		// printf("MessageQueue0 \n");
		if (ProgramStatus0 == 2 && NumMessages0 > 0) {
			ProgramStatus0 = 1;
		}
		if (ProgramStatus0 != 1 && ProgramStatus1 != 1) {
			break;
		}
	}
	printf("TotalSentBy1 = %d\n", TotalSentBy1);

	return 0;
}