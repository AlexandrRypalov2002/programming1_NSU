# define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define N 64
#define L 1024

int check_for_d_or_D_or_Other(char* str, char* word, int *w, int *j, int *len_of_str_new, int *m) {
	while (*j != *len_of_str_new) {
		if ((*m)*2 < *len_of_str_new) {
			return 0;
		}
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
			return 0;
		}
	}
	if (*w == *m) {
		return 1;
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
	int len_of_str_new = strlen(str_n);
	int l = 0;
	scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		scanf("%s ", word);
		m = strlen(word);
		int w = 0;
		int j = 0;
		int g = check_for_d_or_D_or_Other(str_n, word, &w, &j, &len_of_str_new, &m);
		if (g == 1){
			printf("%d ", i);
		}
		else {
			l += 1;
		}
	}
	if (l == n) {
		printf("none");
	}
	return 0;
}