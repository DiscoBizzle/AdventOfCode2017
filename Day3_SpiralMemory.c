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

void itoxy(int *x, int *y, int i) {
	double bf = 0.5 + 0.5*sqrt(i);
	int b = floor(bf);

	int IndexWithinBlock = (i - (1 + 4*b*(b-1)));

	int BlockFace = IndexWithinBlock/(2*b);
	int SubBlockIndex = (i - (1 + 4*b*(b-1)))%(2*b);
	// printf("%s\n", );
	switch (BlockFace) {
		case 0: {
			// printf("Right Face\n");
			*x = b;
			*y = 1-b+SubBlockIndex;
		}break;
		case 1: {
			// printf("Top Face\n");
			*y = b;
			*x = b-1-SubBlockIndex;
		}break;
		case 2: {
			// printf("Left Face\n");
			*x = -b;
			*y = b-1-SubBlockIndex;
		} break;
		case 3: {
			// printf("Bottom Face\n");
			*y = -b;
			*x = 1-b+SubBlockIndex;
		}
	}
}

int xytoi(int x, int y) {
	int b = Max(abs(x), abs(y));

	if (x == 0 && y == 0) {
		return 0;
	} 

	int BlockStartIndex = (1 + 4*b*(b-1));
	int i = 0;

	if (x == b && y > -b) {
		// printf("Right Face\n");
		i = BlockStartIndex + (y+b-1);
	} else if (y == b && x < b) {
		// printf("Top Face\n");
		i = BlockStartIndex + 2*b + ((b-1)-x);
	} else if (x == -b && y < b) {
		i = BlockStartIndex + 4*b + ((b-1)-y);
	} else if (y == -b && x > -b) {
		i = BlockStartIndex + 6*b + (x+b-1);
	}
	return i;
}

int main(int argc, char const *argv[]) 
{
	// PART 1
	int n = 289326;
    // Data from square 1 is carried 0 steps, since it's at the access port.
    // Data from square 12 is carried 3 steps, such as: down, left, left.
    // Data from square 23 is carried only 2 steps: up twice.
    // Data from square 1024 must be carried 31 steps.

	// double bf = 0.5 + 0.5*sqrt(n);
	// int b = floor(bf);

	// int si = (n - (1 + 4*b*(b-1)))%(2*b);

	// printf("n = %d, b = %d, si = %d\n", n, b, si);
	// printf("2b-si = %d\n", 2*b-si);

	// int d = b + abs(b-si);
	// printf("d = %d\n", d);

	// PART 1 ALTERNATIVE

	int x; int y;

	int i = n-1;
	itoxy(&x, &y, i);
	printf("n = %d, distance = %d \n", n, abs(x)+abs(y));
	// PART 2

	for (int i = 1; i < 17; i++) {
		itoxy(&x, &y, i);
		// printf("1) i = %d, x = %d, y = %d\n", i, x, y);
		// printf("2) i = %d, x = %d, y = %d\n", xytoi(x,y), x, y);
	}


	int Length = 1000;
	int *List;
	List = (int *)malloc(sizeof(int)*Length);
	List[0] = 1;
	for (i = 1; i < Length; i++) {
		int x, y;
		int CurrEl = 0; 
		itoxy(&x, &y, i);

		for (int dx = -1; dx <= 1; dx++) {
			for (int dy = -1; dy <= 1; dy++) {
				int j = xytoi(x+dx, y+dy);
				// printf("x+dx = %d, y+dy = %d, j = %d\n", x+dx, y+dy, j);
				if (j < i) {
					// printf("i = %d, j = %d < %d\n", i,j,i);
					CurrEl += List[j];
				}
			}	

		}
		printf("n = %d, List[%d] = %d\n", n, i, CurrEl);
		List[i] = CurrEl;
		if (CurrEl > n) {
			break;
		}
	}
	
	return 0;
}