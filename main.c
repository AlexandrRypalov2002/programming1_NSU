#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define N 255
int main() {
	int w, h; //h - столбцы, w - строчки
	int M[N][N];
	int a = 0;
	int b = 0;
	scanf("%d %d", &w, &h);
	int A[N];
	for (int j = 0; j < w; j++) {
		A[j] = 0;
	}
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			scanf("%d", &M[i][j]);
		}
	}
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if ((M[i][j] < 0) && ((M[i][j]) % 2 != 0)) {
				A[j] += M[i][j];
			}
		}
	}
	//здесь начинается сортировка пузырьком массива из характеристик
	for (int k = w - 1; k >= 0; k--) {
		for (int j = 0; j < k; j++) {
			if (A[j] > A[j + 1]) {
				a = A[j];
				A[j] = A[j + 1];
				A[j + 1] = a;
				for (int i = 0; i < h; i++) {
					b = M[i][j];
					M[i][j] = M[i][j+1];
					M[i][j+1] = b;
				}
			}
		}
	}
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			printf("%d ", M[i][j]);
		}
		printf("\n");
	}
	printf("%d %d", A[0], A[w - 1]);
	return 0;
}