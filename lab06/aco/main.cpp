#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <math.h>

#include "matrix.h"
#include "test.h"

#define ALPHA_BETTA_CONST 3

const double teta_start = 10;

using namespace std;


void range(vector<int> &arr1, vector<int> &arr2, const unsigned int &count)
{
    bool add = true;
    for (auto i = 0; i < count; i++)
    {
        add = true;
        for (auto ptr2 = arr2.begin(); ptr2 != arr2.end(); ptr2++)
        {
            if (i == *ptr2)
                add = false;
        }
        if (add)
            arr1.push_back(i);
    }
}

template <typename Type>
int index(vector<Type> &arr, const Type& value)
{
    for (int i = 0; i < arr.size(); i++)
    {
        if (arr[i] == value)
            return i;
    }
    return 0;
}

double rand_double(void)
{
    int tmp = rand() % 100;
    return (tmp / 100.0);
}

pair<int, vector<int>> aco(const Matrix<int> &distances, const int &t_max, const double &alpha, const double &ro)
{
    const unsigned int town_number = distances.n;
    const double Q = distances.compute_avg() * town_number; // coefficient for calculating amount of pheromone that ant leaves
    const double betta = ALPHA_BETTA_CONST - alpha;

    Matrix <double> town_attraction(town_number); // inversed distances
    town_attraction.reverse_values(distances);

    Matrix<double> teta(town_number); // amount of pheromone on the paths
    teta.fill_value(teta_start);
    Matrix<double> delta_teta(town_number); // amount of pheromone added during the 'day'

    int min_path_length = -1;
    vector<int> min_path;
    vector<double> probabilities(town_number, 0.0);

    double coin;

    for (int t = 0; t < t_max; t++)
    {
        delta_teta.zero();
        // loop over each ant
        for (int k = 0; k < town_number; k++)
        {
            vector<int> current_path = {k};
            int current_path_length = 0;
            int i = k; // current town

            // loop to get the path for the current ant
            while (current_path.size() != town_number)
            {
                vector<int> not_visited;
                range(not_visited, current_path, town_number); // select towns that are not visited
                fill(probabilities.begin(), probabilities.end(), 0.0);

                // calculating P - probability of visiting each of yet not visited towns...
                for (auto j : not_visited)
                {
                    int cur_index = index(not_visited, j);
                    if (distances[i][j] != 0)
                    {
                        double sum = 0;
                        for (auto n : not_visited)
                            sum += pow(teta[i][n], alpha) * pow(town_attraction[i][n], betta);
                        probabilities[cur_index] = pow(teta[i][j], alpha) * pow(town_attraction[i][j], betta) / sum;
                    }
                    else
                         probabilities[cur_index] = 0;
                }

                // toss a 'coin' to choose which town is next
                coin = rand_double();
                unsigned int best_p = 0;
                double sum_p = 0;

                // calculating best_p...
                // when the sum of probabilities will be bigger than coin value than we found best_p - index of next town in not_visited
                for (unsigned int s = 0; s < town_number; s++)
                {
                    sum_p += probabilities[s];
                    if (coin < sum_p)
                    {
                        best_p = s;
                        break;
                    }
                }

                int to_add = not_visited[best_p]; // new town in the path
                // adding new town to the total path
                current_path.push_back(to_add);
                // updating total path length
                current_path_length += distances[i][to_add];
                // updtaing pheromone on the current edge
                delta_teta[i][to_add] += Q / current_path_length;

                i = to_add; // current town is changed for next iteration
                not_visited.erase(not_visited.begin() + best_p); // deleting visited town
            }

            // counting way back
            current_path_length += distances[current_path[current_path.size() - 1]][current_path[0]];
            // updating minimum path and it's length
            if (min_path_length == -1 || (current_path_length < min_path_length))
            {
                min_path_length = current_path_length;
                min_path = current_path;
            }
        }
        // updating pheromone on all edges during night
        teta *= (1.0 - ro);
        teta += delta_teta;
    }
    return pair<int, vector<int>>(min_path_length, min_path);
}


int main(void)
{
    srand(time(0));
    system("chcp 65001");
    Matrix<int> mtr1(5);
    mtr1.fill_int_random();
    mtr1.print();
    pair<int, vector<int>> results = aco(mtr1, 10, 2, 0.5);
    cout << "aco results: " << results.first << " length" << endl;
    for (auto ptr = results.second.begin(); ptr != results.second.end(); ptr++)
        cout << *ptr << " ";
    cout << endl;

    pair<int, vector<int>> brute_results = brute_force(mtr1);
    cout << "brute_force results: " << brute_results.first << " length" << endl;
    for (auto ptr = brute_results.second.begin(); ptr != brute_results.second.end(); ptr++)
        cout << *ptr << " ";
    cout << endl;



    return 0;
}
