#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define N 255

void swap(int* a, int* b) {
	int c = *a;
	*a = *b;
	*b = c;
}

int main() {
	int w, h;
	int A[N][N];
	int B[N][N];
	int C[N] = { 0 };
	int D[N] = { 0 };
	scanf("%d %d", &w, &h);
	for (int i = 0; i < w; i++) {
		D[i] = i;
	}
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			scanf("%d", &A[i][j]);
		}
	}
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if ((A[i][j] < 0) && (A[i][j] % 2 != 0)) {
				C[j] += A[i][j];
			}
		}
	}
	for (int i = 1; i < w; i++) {
		for (int j = i; j > 0 && C[j - 1] > C[j]; j--) {
			swap(&C[j - 1], &C[j]);
			swap(&D[j - 1], &D[j]);
		}
	}
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			B[i][j] = A[i][D[j]];
		}
	}
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			printf("%d ", B[i][j]);
		}
		printf("\n");
	}
	printf("%d %d", C[0], C[w - 1]);
	return 0;
}