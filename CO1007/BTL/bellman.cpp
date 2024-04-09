#include "bellman.h"

const int oo = 1e9 + 1;

bool BF(int** C, int n, char S, int* value, int* previous) {
	// khoi tao
	for (int i = 0; i < n; i++) {
		value[i] = -oo;
		previous[i] = -1;
	}
	value[S] = 0;
	// lap
	for (int u = 0; u < n - 1; u++) {
		for (int v = 0; v < n; v++) {
			if (value[u] + C[u][v] < value[v]) {
				value[v] = value[u] + C[u][v];
				previous[v] = u;
			}
		}
	}
	// kiem tra cung co trong so am
	for (int u = 0; u < n - 1; u++) {
		for (int v = 0; v < n; v++) {
			if (value[u] + C[u][v] < value[v]) {
				std::cout << "Contains circuit of negative weight";
				return false;
			}
		}
	}
	return true;
}

void BF_Path(int n, int* previous, char F) {
	int i = F - 65, count = 0;
	char trace[n];
	while (i != -1) {
		trace[count] = i + 65;
		count++;
		i = previous[i];
	}
	for (int j = 0; j < count; j++) {
		std::cout << trace[j];
		if (j < count - 1)
			std::cout << " -> ";
	}
}
