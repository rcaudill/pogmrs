#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main (int argc, char *argv[]) {
clock_t start, end;
double elapsed;

start = clock();
end = start;
do {
end = clock();
elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;
} while ( elapsed < 0.000001);



printf("elapsed: %f\n", elapsed);
return 0;
}

