#include "tsf.h"

void Travelling() {
	int n, S, F;
	int*  value;
	int*  previous;
	int** C;

	std::cin >> n;

	// allocating memory
	value = (int*)malloc(sizeof(int) * n);
	previous = (int*)malloc(sizeof(int) * n);
	C = (int**)malloc(sizeof(int*) * n);
	for (int i = 0; i < n; i++)
		C[i] = (int*)malloc(sizeof(int) * n);

	// input
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			std::cin >> C[i][j];

	std::cin >> S >> F;

	// calculating
	bool result = BF(C, n, S, value, previous);
	if (result) {
		BF_Path(n, previous, F);
	} else {
		std::cout << "Contains circuit of negative weight";
		return;
	}

	for (int i = 0; i < n; i++)
		std::cin >> value[i];
	for (int i = 0; i < n; i++)
		std::cin >> previous[i];

	// deallocating memory
	free(value);
	free(previous);
	for (int i = 0; i < n; i++)
		free(C[i]);
	free(C);
}
