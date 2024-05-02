#include "bellman.h"

bool BF(int** C, int n, char S, int* value, int* previous) {
	// khoi tao
	for (int i = 0; i < n; i++) {
		value[i] = -1;
		previous[i] = -1;
	}
	value[S - 65] = 0;
	// lap
	for (int i = 1; i < n; i++) {
		for (int u = 0; u < n; u++) {
			for (int v = 0; v < n; v++) {
				if (value[u] > -1 && C[u][v] > 0) {
					if (value[v] == -1||value[u] + C[u][v] < value[v]) {
						value[v] = value[u] + C[u][v];
						previous[v] = u;
					}
				}
			}
		}
	}
	// kiem tra cung co trong so am
	for (int u = 0; u < n; u++) {
		for (int v = 0; v < n; v++) {
			if (C[u][v] > 0) {
				if (value[u] + C[u][v] < value[v])
					return false;
			}
		}
	}
	return true;
}

std::string BF_Path(int n, int* previous, char F) {
	int i = F - 65, count = 0;
	std::string trace = "";
	while (i != -1) {
		trace += (char)(i + 65);
		i = previous[i];
	}
	reverse(trace.begin(), trace.end());
	return trace;
}
