#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#define N 6
#define M 8
#define L 7
#define size 16

typedef struct Tree_ {
	char* value;
	struct Tree_* left;
	struct Tree_* right;
	struct Tree_* parent;
}Tree;

Tree* create(Tree* parent, char* value) {
	Tree* t = (Tree*)malloc(sizeof(Tree));
	if (t != NULL) {
		t->left = NULL;
		t->right = NULL;
		int l = strlen(value);
		t->value = (char*)malloc(sizeof(char) * (l + 2));
		if (t->value) {
			strcpy(t->value, value);
		}
		t->parent = parent;
	}
	return t;
}

void add_in_tree(Tree** head, char* value) {
	int c = 0;
	Tree* t = NULL;
	if (*head == NULL) {
		*head = create(NULL, value);
		return;
	}
	t = *head;
	while (t) {
		if ((c = strcmp(value, t->value)) > 0) {
			if (t->right) {
				t = t->right;
				continue;
			}
			else {
				t->right = create(t, value);
				return;
			}
		}
		else if (c < 0) {
			if (t->left) {
				t = t->left;
			}
			else {
				t->left = create(t, value);
				return;
			}
		}
	}
}

Tree* search_for_min(Tree* root, int f) {
	while (root->left != NULL) {
		root = root->left;
		f = 1;
	}
	if (f == 1) {
		if (root->right) {
			root->parent->left = root->right;
			root->right->parent = root->parent;
		}
		else {
			root->parent->left = root->right;
		}
	}
	else {
		if (root->right) {
			root->parent->right = root->right;
			root->right->parent = root->parent;
		}
		else {
			return root;
		}
	}
	return root;
}

Tree* check_for_word_in_tree(Tree* root, char* word) {
	int c = 0;
	while (root) {
		if ((c = strcmp(word, root->value)) < 0) {
			root = root->left;
			continue;
		}
		else if (c > 0) {
			root = root->right;
			continue;
		}
		else {
			return root;
		}
	}
	return NULL;
}

Tree* remove_from_tree(Tree* for_word, char* word) {
	Tree* remove = check_for_word_in_tree(for_word, word);
	if (remove != NULL) {
		if ((remove->right == NULL) && (remove->left == NULL)) {
			if (remove->parent) {
				if (remove == remove->parent->left) {
					remove->parent->left = NULL;
				}
				else {
					remove->parent->right = NULL;
				}
				free(remove->value);
				free(remove);
			}
			else {
				free(remove->value);
				free(remove);
				for_word = NULL;
			}
		}
		else if ((remove->right != NULL) && (remove->left == NULL)) {
			if (remove->parent) {
				if (remove == remove->parent->left) {
					remove->parent->left = remove->right;
					remove->right->parent = remove->parent;
				}
				else {
					remove->parent->right = remove->right;
					remove->right->parent = remove->parent;
				}
				free(remove->value);
				free(remove);
			}
			else {
				for_word = remove->right;
				for_word->parent = NULL;
				free(remove->value);
				free(remove);

			}
		}
		else if ((remove->right == NULL) && (remove->left != NULL)) {
			if (remove->parent) {
				if (remove == remove->parent->left) {
					remove->parent->left = remove->left;
					remove->left->parent = remove->parent;
				}
				else {
					remove->parent->right = remove->left;
					remove->left->parent = remove->parent;
				}
				free(remove->value);
				free(remove);
			}
			else {
				for_word = remove->left;
				for_word->parent = NULL;
				free(remove->value);
				free(remove);
			}
		}
		else if ((remove->right != NULL) && (remove->left != NULL)) {
			Tree* local_min = search_for_min(remove->right, 0);
			remove->value = local_min->value;
			free(local_min);
		}
	}
	else {
		return NULL;
	}
	return for_word;
}

Tree* read_words_from_file(FILE* input, Tree* root, int* number_b, int* number_d) {
	char del[M] = { 'D', 'E', 'L', 'E', 'T', 'E', ':','\0' };
	char level[L] = { 'L', 'E', 'V', 'E', 'L', ':', '\0' };
	int all = 0;
	int used = 1;
	int used_for_tree = 0;
	char* word = NULL;
	char c;
	int check_to_delete = -1;
	while ((c = fgetc(input)) && (!feof(input))) {
		if (all <= used) {
			char* new_word = (char*)realloc(word, sizeof(char) * (all + size));
			if (!new_word) {
				break;
			}
			word = new_word;
			all += size;
		}
		word[used_for_tree] = c;
		used++;
		used_for_tree++;
		if ((c == ' ') || (c == '\n') || (c == '.')) {
			if (c == '.') {
				c = fgetc(input);
			}
			word[used_for_tree - 1] = '\0';
			int check = strcmp(word, del);
			int to_break = strcmp(word, level);
			if (to_break == 0) {
				free(word);
				return root;
			}
			if (check == 0) {
				used_for_tree = 0;
				check_to_delete = check;
				continue;
			}
			if (check_to_delete == 0) {
				Tree* check_d = check_for_word_in_tree(root, word);
				if (check_d) {
					root = remove_from_tree(root, word);
					if (root) {
						(*number_d)++;
					}
				}
				used_for_tree = 0;
				continue;
			}
			if (root) {
				Tree* check_for_word = check_for_word_in_tree(root, word);
				if (check_for_word != NULL) {
					used_for_tree = 0;
					continue;
				}
			}
			add_in_tree(&root, word);
			used_for_tree = 0;
			(*number_b)++;
		}
	}
	return root;
}

void print_from_tree(Tree* root, int l, FILE* output) {
	if (root) {
		if (l == 0) {
			fprintf(output, "%s ", root->value);
		}
		else {
			if (root->left != NULL) {
				print_from_tree(root->left, l - 1, output);
			}
			if (root->right != NULL) {
				print_from_tree(root->right, l - 1, output);
			}
		}
	}
}

void cut_the_tree(Tree* root) {
	if (root == NULL) {
		return;
	}
	else
	{
		cut_the_tree(root->left);
		cut_the_tree(root->right);
		free(root->value);
		free(root);
	}
}

int main() {
	Tree* root = NULL;
	char TEXT[N];
	int l;
	int number_b = 0;
	int number_d = 0;
	FILE* input, * output;
	input = fopen("input.txt", "r");
	if (input == NULL) {
		printf("File doesn't exist");
		return 1;
	}
	fscanf(input, "%s\n", TEXT);
	root = read_words_from_file(input, root, &number_b, &number_d);
	fscanf(input, "%d", &l);
	fclose(input);
	int number_a = number_b - number_d;
	output = fopen("output.txt", "w");
	if (output == NULL) {
		printf("File doesn't exist");
		return 1;
	}
	fprintf(output, "%d\n", number_a);
	print_from_tree(root, l, output);
	fclose(output);
	cut_the_tree(root);
	return 0;
} 