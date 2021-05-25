#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

void go_through_graph(int** g, int* v, int* d, int n, int s) {
	int search = INT_MAX;
	int s_next = 0;
	int b = 0;
	v[s] = 1;
	for (int i = 0; i < n; i++) {
		if (g[s][i] != 0) {
			if (d[i] > (g[s][i] + d[s])) {
				d[i] = g[s][i] + d[s];
			}
			if (search > d[i]) {
				search = d[i];
				s_next = i;
			}
		}
		else {
			b++;
		}
	}
	if (b == n) {
		return;
	}
	if (v[s_next] != 1) {
		go_through_graph(g, v, d, n, s_next);
	}
}		

void Dijkstra(int** graph, int* visited, int* dists, int* map, int island, int n) {
	for (int i = 0; i < n; i++) {
		if (visited[i] != 1) {
			go_through_graph(graph, visited, dists, n, i);
		}
	}
}

int main() {
	int island;
	int n_of_ports;
	int ways;
	int from;
	int to;
	int time;
	FILE* input, * output;
	input = fopen("input.txt", "r");
	if (input == NULL) {
		printf("Input file doesn't exist");
		return 1;
	}
	fscanf(input, "%d %d %d", &island, &n_of_ports, &ways);
	int** graph = (int**)malloc(n_of_ports * sizeof(int*));
	if (graph) {
		for (int i = 0; i < n_of_ports; i++) {
			graph[i] = (int*)malloc(n_of_ports*sizeof(int));
		}
	}
	else {
		return 2;
	}

	int* visited = (int*)malloc(n_of_ports*sizeof(int));
	if (visited == NULL) {
		return 2;
	}
	for (int i = 0; i < n_of_ports; i++) {
		visited[i] = 0;
	}

	int* dists = (int*)malloc(n_of_ports * sizeof(int));
	if (dists) {
		dists[0] = 0;
		for (int i = 1; i < n_of_ports; i++) {
			dists[i] = INT_MAX;
		}
	}
	else {
		return 2;
	}

	for (int i = 0; i < n_of_ports; i++) {
		for (int j = 0; j < n_of_ports; j++) {
			graph[i][j] = 0;
		}
	}

	for (int i = 0; i < ways; i++) {
		fscanf(input, "%d %d %d", &from, &to, &time);
		graph[from][to] = time;
	}
	fclose(input);

	output = fopen("output.txt", "w");
	if (output == NULL) {
		printf("Output file doesn't exist");
		return 1;
	}

	int* map = (int*)malloc((n_of_ports+1)*sizeof(int));
	if (map == NULL) {
		return 3;
	}
	for (int i = 0; i < (n_of_ports+1); i++) {
		map[i] = 0;
	}

	Dijkstra(graph, visited, dists, map, island, n_of_ports);
	fprintf(output, "%d\n", dists[island]);
	map[0] = island;
	int k = 1;
	int distance = dists[island];
	while (island != 0) {
		for (int i = 0; i < n_of_ports; i++) {
			if (graph[i][island] != 0) {
				int t = distance - graph[i][island];
				if (t == dists[i]) {
					distance = t;
					island = i;
					map[k] = i;
					k++;
				}
			}
		}
	}
	for (int i = k-1; i >= 0; i--) {
		fprintf(output, "%d ", map[i]);
	}
	fclose(output);
	for (int i = 0; i < n_of_ports; i++) {
		free(graph[i]);
	}
	free(graph);
	free(visited);
	free(dists);
	free(map);
	return 0;
}