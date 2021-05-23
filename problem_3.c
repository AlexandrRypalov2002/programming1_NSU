#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <math.h>
#define N 100
#define K 64

void swap(int* a, int* b) {
	int c = *a;
	*a = *b;
	*b = c;
}

void heapify(int det_s[], int numbers[], int sizes_of_matrixes[], int last, int root) {
	int biggest = root;
	int left = root * 2 + 1;
	int right = root * 2 + 2;
	if ((left < last) && (det_s[left] > det_s[biggest])) {
		biggest = left;
	}
	if ((right < last) && (det_s[right] > det_s[biggest])) {
		biggest = right;
	}
	if (biggest != root) {
		swap(&det_s[root], &det_s[biggest]);
		swap(&numbers[root], &numbers[biggest]);
		swap(&sizes_of_matrixes[root], &sizes_of_matrixes[biggest]);
		heapify(det_s, numbers, sizes_of_matrixes, last, biggest);
	}
}

void heap_sort(int det_s[], int numbers[], int sizes_of_matrixes[], int number) {
	for (int i = number / 2 - 1; i >= 0; i--) {
		heapify(det_s, numbers, sizes_of_matrixes, number, i);
	}
	for (int i = number - 1; i >= 1; i--) {
		swap(&det_s[0], &det_s[i]);
		swap(&numbers[0], &numbers[i]);
		swap(&sizes_of_matrixes[0], &sizes_of_matrixes[i]);
		heapify(det_s, numbers, sizes_of_matrixes, i, 0);
	}
}

int det_of_matrix(int matrix[][K], int strings[], int size, int minor_size, int s) {
	int final_result = 0;
	int c_minus = 0;
	for (int i = 0; i < size; i++) {
		if (strings[i] == 0) {
			if (minor_size == 1) {
				return matrix[s][i];
			}
			else {
				s++;
				strings[i]++;
				final_result += pow((-1), c_minus)*matrix[s - 1][i] * det_of_matrix(matrix, strings, size, minor_size - 1, s);
				c_minus++;
				strings[i]--;
				s--;
			}
		}
	}
	return final_result;
}

int main() {
	int matrixx_three[K][K][N] = { 0 }; //array to keep matrixes
	int matrix[K][K];
	int matrixx_sorted[K][K][N] = { 0 };
	int numb; //number of matrixes
	int size; //size of matrix
	int numbers[N] = { 0 }; //numbers
	int det_s[N] = { 0 }; //determinates of matrixes
	int sizes_of_matrixes[N] = { 0 };
	int strings[K] = { 0 };
	FILE* input, * output;
	input = fopen("input.txt", "r");
	if (input == NULL) {
		printf("File not found");
		return 1;
	}
	fscanf(input, "%d", &numb);
	for (int i = 0; i < numb; i++) {
		fscanf(input, "%d", &size);
		sizes_of_matrixes[i] = size;
		for (int j = 0; j < size; j++) {
			for (int l = 0; l < size; l++) {
				fscanf(input, "%d", &matrix[j][l]);
				matrixx_three[j][l][i] = matrix[j][l];
			}
		}
		det_s[i] = det_of_matrix(matrix, strings, size, size, 0);
	}
	fclose(input);
	for (int i = 0; i < numb; i++) {
		numbers[i] = i;
	}
	heap_sort(det_s, numbers, sizes_of_matrixes, numb);
	for (int i = 0; i < numb; i++) {
		for (int j = 0; j < sizes_of_matrixes[i]; j++) {
			for (int l = 0; l < sizes_of_matrixes[i]; l++) {
				matrixx_sorted[j][l][i] = matrixx_three[j][l][numbers[i]];
			}
		}
	}
	output = fopen("output.txt", "w");
	if (output == NULL) {
		printf("File not found");
		return 1;
	}
	for (int i = 0; i < numb; i++) {
		for (int j = 0; j < sizes_of_matrixes[i]; j++) {
			for (int l = 0; l < sizes_of_matrixes[i]; l++) {
				fprintf(output, "%d ", matrixx_sorted[j][l][i]);
			}
			fprintf(output, "\n");
		}
	}
	fclose(output);
	return 0;
}