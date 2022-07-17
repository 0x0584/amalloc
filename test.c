#include <stdlib.h>
#include <stdio.h>

int main() {
	int a = sizeof(int);
	printf("%d", a);
	void *b = malloc(a);
	*(int *)b = a;
	printf(" %d\n", *(int *)b);
	free(b);
	return a;
}
