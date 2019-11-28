#ifndef TEST_H
#define TEST_H

#include <vector>
#include "matrix.h"

using namespace std;
pair<int, vector<int> >brute_force(const Matrix<int> &distances);

void hamilton(const Matrix<int> &distances, vector<int> &min_path, int &min_distance, vector<int> &cur_path, vector<bool> &visited, int &cur_len);


#endif // TEST_H
