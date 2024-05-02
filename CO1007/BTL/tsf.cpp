#include "tsf.h"

int perm[30];
bool used[30];
int bestPerm[30], cost = 0, bestCost = 1e9 + 1;

void Try(int n, int** C, char S, int k) {
	for (int i = 0; i < n; i++) {
		int h = perm[k - 1];
		if (!used[i] && C[h][i] > 0) {
			perm[k] = i;
			cost += C[h][i];
			used[i] = true;
			if (k == n - 1) {
				// go back to the starting town
				if (cost + C[i][S - 65] < bestCost) {
					bestCost = cost + C[i][S - 65];
					for (int j = 0; j <= k; j++)
						bestPerm[j] = perm[j];
				}
			} else {
				// check if cost < bestCost?
				if (cost < bestCost)
					Try(n, C, S, k + 1);
			}
			cost -= C[h][i];
			used[i] = false;
		}
	}
}

void Travelling(int n, int** C, char S) {
	perm[0] = S - 65;
	used[S - 65] = true;
	// recursion
	Try(n, C, S, 1);
	for (int i = 0; i < n; i++)
		std::cout << char(bestPerm[i] + 65) << ' ';
	std::cout << S << "\nCost: " << bestCost;
}
