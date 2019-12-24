#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <math.h>
#include <string>
#include <iomanip>
#include <chrono>

#include "matrix.h"
#include "test.h"

#define ALPHA_BETTA_CONST 1

const double teta_start = 10;
const double tmin = 5;

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
        teta.check_zero(tmin);
    }
    return pair<int, vector<int>>(min_path_length, min_path);
}

void write_res(double &ro, double &alpha, int &tmax, double &dis, const string &name)
{
    ofstream file;
    file.open(name, ios_base::app);
    if (!file)
        cout << "error" << endl;
    else
    {
        file << ro << "," << alpha << "," << tmax << "," << dis << '\n';
        file.close();
    }
}

void write_time(int &n, double &time_brute, double &time_aco, const string &name)
{
    ofstream file;
    file.open(name, ios_base::app);
    if (!file)
        cout << "error" << endl;
    else
    {
        file << n << "," << fixed << setprecision(8) << time_brute << "," << time_aco << '\n';
        file.close();
    }
}

int main(void)
{
    srand(time(0));
    system("chcp 65001");

    // parameterization
    /*
    int test_array[10][10] = { { 0, 8, 7, 1, 5, 5, 4, 9, 4, 3 },
                               { 8, 0, 8, 7, 8, 5, 9, 4, 2, 2 },
                               { 7, 8, 0, 7, 7, 4, 6, 9, 2, 9 },
                               { 1, 7, 7, 0, 2, 1, 2, 2, 10, 5 },
                               { 5, 8, 7, 2, 0, 9, 1, 8, 3, 2 },
                               { 5, 5, 4, 1, 9, 0, 9, 4, 1, 6 },
                               { 4, 9, 6, 2, 1, 9, 0, 3, 3, 1 },
                               { 9, 4, 9, 2, 8, 4, 3, 0, 6, 8 },
                               { 4, 2, 2, 10, 3, 1, 3, 6, 0, 7 },
                               { 3, 2, 9, 5, 2, 6, 1, 8, 7, 0 } };
    Matrix<int> mtr1(10);
    mtr1.fill_array(test_array);
    mtr1.print();
    pair<int, vector<int>> brute_results = brute_force(mtr1);
    int ideal_len = brute_results.first;
    double dis;
    for (double ro = 0; ro <= 1; ro += 0.25)
    {
        for (double alpha = 0; alpha <= 1; alpha += 0.25)
        {
            for (int t = 10; t <= 310; t += 10)
            {
                for (auto i = 0; i < 100; i++)
                {
                    pair<int, vector<int>> results = aco(mtr1, t, alpha, ro);
                    dis += abs(results.first - ideal_len);
                }
                dis /= 100.0;
                write_res(ro, alpha, t, dis, "param2.txt");
            }
        }
    }
    */

    // examples of work
    /*
    Matrix<int> mtr1(5);
    mtr1.fill_int_random();
    mtr1.print();
    pair<int, vector<int>> results = aco(mtr1, 100, 0.5, 0.3);
    cout << "aco results: " << results.first << " length" << endl;
    for (auto ptr = results.second.begin(); ptr != results.second.end(); ptr++)
        cout << *ptr << " ";
    cout << endl;

    pair<int, vector<int>> brute_results = brute_force(mtr1);
    cout << "brute_force results: " << brute_results.first << " length" << endl;
    for (auto ptr = brute_results.second.begin(); ptr != brute_results.second.end(); ptr++)
        cout << *ptr << " ";
    cout << endl;
    */



    // comparison brute_force and aco
    /*
    double t1, t2;
    for (int n = 2; n <= 20; n++)
    {
        Matrix<int> mtr1(n);
        mtr1.fill_int_random();
        auto start = chrono::high_resolution_clock::now();
        for (auto i = 0; i < 10; i++)
            brute_force(mtr1);
        auto end = chrono::high_resolution_clock::now();
        t1 = chrono::duration_cast<chrono::duration<double>>(end - start).count() / 10.0;

        start = chrono::high_resolution_clock::now();
        for (auto i = 0; i < 10; i++)
            aco(mtr1, 290, 0.5, 0.5);
        end = chrono::high_resolution_clock::now();
        t2 = chrono::duration_cast<chrono::duration<double>>(end - start).count() / 10.0;
        write_time(n, t1, t2, "time.txt");
    }
    */

    return 0;
}
