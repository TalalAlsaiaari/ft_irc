#include <unistd.h>

int main (void) {
	int a = 15;
	write(1, &a, 1);
	return 0;
}