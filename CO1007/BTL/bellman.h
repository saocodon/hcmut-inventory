#include <iostream>
#include <string>
#include <algorithm>

#ifndef BELLMAN_H
#define BELLMAN_H

bool BF(int** C, int n, char S, int* value, int* previous);
std::string BF_Path(int n, int* previous, char F);

#endif
