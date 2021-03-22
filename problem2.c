# define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define N 64
#define L 1024

void skobka_1(char* first, char* second, int a) {
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
	char str[N]; //шаблон
	char word[L]; //слово, которое нужно проверить
	char trash[L] = { '\0' }; //тут лежит все, что находится внутри () в квадратных скобках
	char str_n[L] = { '\0' }; //новый шаблон, где нет квадратных скобочек
	int numbers[400] = { 0 };
	int n;
	int h = 0;
	int k = 0;
	int m;
	scanf("%s", str);
	for (int i = 0; i < N; i++) { //вся эта конструкция размножает шаблон
		if (str[i] != '[') {
			str_n[k] = str[i];
			k++;
		}
		else {
			int q = i;
			skobka_1(str, trash, q);
			int l = strlen(trash);
			while (str[i] != ']') {
				i++;
			}
			int j = 0;
			while (j < l) {
				str_n[k] = trash[j];
				k++;
				j++;
			}
		}
	}
	int r = strlen(str_n);
	int g = 0;
	int l = 0;
	scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		scanf("%s", word);
		m = strlen(word);
		int w = 0;
		int j = 0;
		while (j != r) {
			if (((str_n[j] == '\\') && (str_n[j + 1] == 'D') && (isalpha(word[w]) != NULL)) || ((str_n[j] == '\\') && (str_n[j + 1] == 'd') && (isdigit(word[w]) != NULL))) {
				w++;
				j += 2;
			}
			else if ((str_n[j] == '~') && (word[w] != str_n[j + 1])) {
				w++;
				j += 2;
			}
			else if (str_n[j] == word[w]) {
				w++;
				j++;
			}
			else {
				g++;
				break;
			}
		}
		if (w == m) {
			numbers[l] = i;
			l++;
		}
	}
	if (g == n) {
		printf("none");
	}
	else if (g != n) {
		int d = 0;
		n -= g;
		while (d < n) {
			printf("%d ", numbers[d]);
			d++;
		}
	}
	return 0;
}