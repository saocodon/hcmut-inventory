#include "bellman.h"
#include "tsf.h"

int n;
char S, F;
int*  value;
int*  previous;
int** C;

int main() {
	std::cin >> n;

	// allocating memory
	value = (int*)malloc(sizeof(int) * n);
	previous = (int*)malloc(sizeof(int) * n);
	C = (int**)malloc(sizeof(int*) * n);
	for (int i = 0; i < n; i++)
		C[i] = (int*)malloc(sizeof(int) * n);

	// input (vertex 0 is used)
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			std::cin >> C[i][j];

	std::cin >> S >> F;

	// calculating
	/*
	bool result = BF(C, n, S, value, previous);
	if (result) {
		std::cout << "Minimum cost from " << S << " to " << F << ": " << value[F - 65] << "\n";
		std::cout << "Route: " << BF_Path(n, previous, F);
	} else {
		std::cout << "Contains circuit of negative weight";
		return 1;
	}
	*/
	Travelling(n, C, S);

	// deallocating memory
	free(value);
	free(previous);
	for (int i = 0; i < n; i++)
		free(C[i]);
	free(C);
}
