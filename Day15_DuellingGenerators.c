#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAXCHAR 1000
#define MAXWORDS 100
#include "Utilities.c"

typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;

char *Int64ToBin(uint64 Number, int SizeInBits)
{
	char *BitString = (char *)calloc(SizeInBits+1, sizeof(char));
	for(int i = 0; i < SizeInBits; i++) {
		uint64 Shift = SizeInBits-i-1;
		if ((1ULL<<Shift)&Number) { //NOTE: When bit-shifting literals lke this, we need to specify that the literal is 64 bit.
			sprintf(BitString, "%s1",BitString); // Add a 1 to the bit string
		} else {
			sprintf(BitString, "%s0",BitString); // Add a 0 to the bit string
		}
	}
	return BitString;
}

int min(int a, int b) {
	if (a < b) {
		return a;
	} else {
		return b;
	}
}

int main(int argc, char const *argv[]) 
{
	// TODO: Can we exploit the fact that we only compare the lowerst 16 bits to speed this up?
	// TODO: By thinking about the sequences can we reduce the number of iterations we have to check?
	uint64 Mod = 2147483647;

	uint64 ABase = 16807; 
	uint64 BBase = 48271;
	uint64 A = 591;
	uint64 B = 393;

	uint64 Mask = 65535ULL;

	int Judge = 0;
	int MaxRuns = 40000000;

	for (int i = 0; i < MaxRuns; i++) {
		A = (A*ABase)%Mod;
		B = (B*BBase)%Mod;

		if ((A&Mask) == (B&Mask)) {
			Judge++;
		}
	}
	printf("Part1: Judge = %d\n", Judge);

	A = 591;
	B = 393;
	Judge = 0;
	MaxRuns = 5000000;

	int LastChecked = 0;
	for (int i = 0; i < MaxRuns; i++) {
		int AGen = 0;
		while (!AGen) {
			A = (A*ABase)%Mod;
			if (A%4 == 0) {
				AGen = 1;
			}
		}
		int BGen = 0;
		while (!BGen) {
			B = (B*BBase)%Mod;
			if (B%8 == 0) {
				BGen = 1;
			}
		}
		if ((A&Mask) == (B&Mask)) {
			Judge++;
		}
	}
	printf("Part2: Judge = %d\n", Judge);
	return 0;
}