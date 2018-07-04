#include <stdio.h>

int main() {
	int t;
	scanf("%d", &t);
	
	int sequence[8] = { 0, 0, 1, 1, 1, 1, 1, 0 };

	long long a, b;

	while (t--) {
		scanf("%lld %lld", &a, &b);
		a = a & 0x7;
		b = b & 0x7;
		if (sequence[(8 - a + b) % 8]) {
			printf("Ziya\n");
		}
		else {
			printf("Farael\n");
		}
	}

	return 0;
}