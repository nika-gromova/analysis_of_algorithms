#include "test.h"
#include <algorithm>
#include <iostream>

using namespace std;

static int s(0);

pair<int, vector<int> > brute_force(const Matrix<int> &distances)
{
    int n = distances.n;
    vector<bool> visited(n, 0);
    vector<int> cur_path;
    vector<int> min_path;
    int cur_len = 0;
    int min_path_len = INT_MAX;
    for (int i = 0; i < n; i++)
    {
        cur_path.clear();
        cur_path.push_back(i);
        fill(visited.begin(), visited.end(), 0);
        visited[i] = 1;
        cur_len = 0;
        hamilton(distances, min_path, min_path_len, cur_path, visited, cur_len);
    }
    //cout << "brute force method found " << s << " routes" << endl;
    return pair<int, vector<int>>(min_path_len, min_path);
}

void hamilton(const Matrix<int> &distances, vector<int> &min_path, int &min_distance, vector<int> &cur_path, vector<bool> &visited, int &cur_len)
{
    if (cur_path.size() == distances.n)
    {
        s++;
        int tmp = distances[cur_path.back()][cur_path[0]];
        if (cur_len + tmp < min_distance)
        {
            min_path = cur_path;
            min_distance = cur_len + tmp;
        }
        return;
    }
    for (int i = 0; i < distances.n; i++)
    {
        if (!visited[i])
        {
            int tmp = distances[cur_path.back()][i];
            if (cur_len + tmp > min_distance)
                continue;
            cur_len += tmp;
            cur_path.push_back(i);
            visited[i] = 1;
            hamilton(distances, min_path, min_distance, cur_path, visited, cur_len);
            visited[i] = 0;
            cur_path.pop_back();
            cur_len -= tmp;
        }
    }
}
