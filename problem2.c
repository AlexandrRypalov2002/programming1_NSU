# define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define N 64
#define L 1024

void check_for_d_or_D_orOther(char* str, char* word, int *w, int *j, int *g, int *r) {
	while (*j != *r) {
		if ((str[*j] == '\\') && (str[*j + 1] == 'd') && (isdigit(word[*w]))) {
			(*w) += 1;
			(*j) += 2;
		}
		else if ((str[*j] == '\\') && (str[*j + 1] == 'D') && (isalpha(word[*w]))) {
			*w += 1;
			*j += 2;
		}
		else if ((str[*j] == '~') && (word[*w] != str[*j + 1])) {
			*w += 1;
			*j += 2;
		}
		else if ((str[*j] == word[*w])) {
			*w += 1;
			*j += 1;
		}
		else {
			*g += 1;
			break;
		}
	}
}

void square_bracket(char* first, char* second, int a) {
	int b = 0;
	int c = a;
	if (first[c] == '[') {
		while (isdigit(first[c + 1])) {
			b *= 10;
			b += first[c + 1] - '0';
			c++;
		}
		c = c + 3;
		int j = 0;
		char third[L] = { '\0' };
		while (first[c] != ')') {
			third[j] = first[c];
			c++;
			j++;
		}
		while (b != 0) {
			strcat(second, third);
			b--;
		}
	}
}

int main() {
	char str[N]; //pattern
	char word[L]; //word to check
	char trash[L] = { '\0' }; //everything inside () inside square brackets
	char str_n[L] = { '\0' }; //new pattern without square brackets
	int numbers[400] = { 0 };
	int n;
	int h = 0;
	int k = 0;
	int m;
	scanf("%s", str);
	for (int i = 0; i < N; i++) { //creates new pattern
		if (str[i] != '[') {
			str_n[k] = str[i];
			k++;
		}
		else {
			int q = i;
			square_bracket(str, trash, q);
			int l = strlen(trash);
			while (str[i] != ']') {
				i++;
			}
			int j = 0;
			strcat(str_n, trash);
			k += l;
		}
	}
	int r = strlen(str_n);
	int g = 0;
	int l = 0;
	scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		scanf("%s ", word);
		m = strlen(word);
		int w = 0;
		int j = 0;
		check_for_d_or_D_orOther(str_n, word, &w, &j, &g, &r);
		if (w == m){
			printf("%d ", i);
		}
	}
	if (g == n) {
		printf("none");
	}
	/*
	else if (g != n) {
		int d = 0;
		n -= g;
		while (d < n) {
			printf("%d ", numbers[d]);
			d++;
		}
	}
	*/
	return 0;
}