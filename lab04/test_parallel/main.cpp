#include <iostream>
#include <thread>
#include <vector>
#include <cstdlib>
#include <string>
#include <fstream>
#include <chrono>

using namespace std;

typedef const double& myT;


class Matrix
{
public:
    int **mtr;
    int n, m;
    Matrix(int nc, int mc)
    {
        n = nc;
        m = mc;
        mtr = new int *[n];
        for (auto i = 0; i < n; i++)
        {
            mtr[i] = new int[m];
            for (auto j = 0; j < m; j++)
            {
                mtr[i][j] = 0;
            }
        }
    }
    ~Matrix()
    {
        for (auto i = 0; i < n; i++)
        {
            delete mtr[i];
        }
        delete mtr;
    }
    void print()
    {
        for (auto i = 0; i < n; i++)
        {
            for (auto j = 0; j < m; j++)
            {
                cout << mtr[i][j] << ' ';
            }
            cout << endl;
        }
        cout << endl;
    }
    void random()
    {
        for (auto i = 0; i < n; i++)
        {
            for (auto j = 0; j < m; j ++)
            {
                mtr[i][j] = rand() % 8;
            }
        }
    }
    void zero()
    {
        for (auto i = 0; i < n; i++)
        {
            for (auto j = 0; j < m; j++)
            {
                mtr[i][j] = 0;
            }
        }
    }
};


unsigned long long tick(void)
{
    unsigned long long d;
    __asm__ __volatile__ ("rdtsc" : "=A" (d) );

    return d;
}


void classic(Matrix &mtr1, Matrix &mtr2, Matrix &res)
{
    for (auto i = 0; i < mtr1.n; i++)
    {
        for (auto j = 0; j < mtr2.m; j++)
        {
            for (auto k = 0; k < mtr1.m; k++)
            {
                (res.mtr)[i][j] += (mtr1.mtr)[i][k] * (mtr2.mtr)[k][j];
            }
        }
    }
}


void create_row(Matrix& mtr, vector<int>& row, const int& n_start, const int& n_end, const int& m)
{
    for (auto i = n_start; i < n_end; i++)
        for (auto j = 0; j < m; j += 2)
            row[i] -= (mtr.mtr)[i][j] * (mtr.mtr)[i][j + 1];
}


void create_column(Matrix& mtr, vector<int>& column, const int& n_start, const int& n_end, const int& m)
{
    for (auto i = n_start; i < n_end; i++)
        for (auto j = 0; j < m; j += 2)
            column[i] -= (mtr.mtr)[j][i] * (mtr.mtr)[j + 1][i];
}


void calculate(Matrix& mtr1, Matrix& mtr2, Matrix& res, vector<int>& row, vector<int>& column, const int& n_start, const int& n_end)
{
    int d = mtr1.m - 1;
    int eq = mtr1.m % 2;
    int buf = 0;
    for (auto i = n_start; i < n_end; i++)
    {
        for (auto j = 0; j < mtr2.m; j++)
        {
            buf = row[i] + column[j];
            for (auto k = 0; k < d; k += 2)
            {
                buf += ((mtr1.mtr)[i][k] + (mtr2.mtr)[k + 1][j]) * ((mtr1.mtr)[i][k + 1] + (mtr2.mtr)[k][j]);
            }
            if (eq)
                buf += (mtr1.mtr)[i][d] * (mtr2.mtr)[d][j];
            (res.mtr)[i][j] = buf;
        }
    }
}


double multiply_one(Matrix& mtr1, Matrix& mtr2, Matrix& res)
{
    vector<int> row(mtr1.n);
    vector<int> column(mtr2.m);
    int d = mtr1.m - 1;
    res.zero();

    int n_end = mtr1.n;
    int m_end = mtr2.m;

    auto start = chrono::steady_clock::now();
    thread thr_row(create_row, ref(mtr1), ref(row), 0, ref(n_end), ref(d));
    thr_row.join();
    thread thr_column(create_column, ref(mtr2), ref(column), 0, ref(m_end), ref(d));
    thr_column.join();
    thread thr_calculate(calculate, ref(mtr1), ref(mtr2), ref(res), ref(row), ref(column), 0, ref(n_end));
    thr_calculate.join();
    auto end = chrono::steady_clock::now();
    double elapsed_seconds = chrono::duration_cast<chrono::duration<double>>(end - start).count();
    return elapsed_seconds;
}


double multiply_two(Matrix& mtr1, Matrix& mtr2, Matrix& res)
{
    vector<int> row(mtr1.n);
    vector<int> column(mtr2.m);
    int d = mtr1.m - 1;
    res.zero();

    int n_end = mtr1.n;
    int m_end = mtr2.m;
    int n_end2 = n_end / 2;

    auto start = chrono::steady_clock::now();
    thread thr_row(create_row, ref(mtr1), ref(row), 0, ref(n_end), ref(d));
    thread thr_column(create_column, ref(mtr2), ref(column), 0, ref(m_end), ref(d));
    thr_row.join();
    thr_column.join();
    thread thr_calculate1(calculate, ref(mtr1), ref(mtr2), ref(res), ref(row), ref(column), 0, ref(n_end2));
    thread thr_calculate2(calculate, ref(mtr1), ref(mtr2), ref(res), ref(row), ref(column), ref(n_end2), ref(n_end));
    thr_calculate1.join();
    thr_calculate2.join();
    auto end = chrono::steady_clock::now();
    double elapsed_seconds = chrono::duration_cast<chrono::duration<double>>(end - start).count();
    return elapsed_seconds;
}


double multiply_four(Matrix& mtr1, Matrix& mtr2, Matrix& res)
{
    vector<int> row(mtr1.n);
    vector<int> column(mtr2.m);
    int d = mtr1.m - 1;
    res.zero();

    int n_end = mtr1.n;
    int m_end = mtr2.m;
    int n_end2 = n_end / 2;
    int m_end2 = m_end / 2;
    int n_end4 = n_end / 4;
    int n_end34 = n_end * 3 / 4;

    auto start = chrono::steady_clock::now();
    thread thr_row1(create_row, ref(mtr1), ref(row), 0, ref(n_end2), ref(d));
    thread thr_row2(create_row, ref(mtr1), ref(row), ref(n_end2), ref(n_end), ref(d));
    thread thr_column1(create_column, ref(mtr2), ref(column), 0, ref(m_end2), ref(d));
    thread thr_column2(create_column, ref(mtr2), ref(column), ref(m_end2), ref(m_end), ref(d));
    thr_column1.join();
    thr_column2.join();
    thr_row1.join();
    thr_row2.join();

    thread thr_calculate1(calculate, ref(mtr1), ref(mtr2), ref(res), ref(row), ref(column), 0, ref(n_end4));
    thread thr_calculate2(calculate, ref(mtr1), ref(mtr2), ref(res), ref(row), ref(column), ref(n_end4), ref(n_end2));
    thread thr_calculate3(calculate, ref(mtr1), ref(mtr2), ref(res), ref(row), ref(column), ref(n_end2), ref(n_end34));
    thread thr_calculate4(calculate, ref(mtr1), ref(mtr2), ref(res), ref(row), ref(column), ref(n_end34), ref(n_end));
    thr_calculate1.join();
    thr_calculate2.join();
    thr_calculate3.join();
    thr_calculate4.join();
    auto end = chrono::steady_clock::now();
    double elapsed_seconds = chrono::duration_cast<chrono::duration<double>>(end - start).count();
    return elapsed_seconds;
}


double multiply_eight(Matrix& mtr1, Matrix& mtr2, Matrix& res)
{
    vector<int> row(mtr1.n);
    vector<int> column(mtr2.m);
    int d = mtr1.m - 1;
    res.zero();

    int n_end = mtr1.n;
    int m_end = mtr2.m;
    int n_end2 = n_end / 2;
    int m_end2 = m_end / 2;
    int n_end4 = n_end / 4;
    int m_end4 = m_end / 4;
    int n_end34 = n_end * 3 / 4;
    int m_end34 = m_end * 3 / 4;
    int n_end8 = n_end / 8;
    int n_end38 = n_end * 3 / 8;
    int n_end58 = n_end * 5 / 8;
    int n_end78 = n_end * 7 / 8;

    auto start = chrono::steady_clock::now();
    thread thr_row1(create_row, ref(mtr1), ref(row), 0, ref(n_end4), ref(d));
    thread thr_row2(create_row, ref(mtr1), ref(row), ref(n_end4), ref(n_end2), ref(d));
    thread thr_row3(create_row, ref(mtr1), ref(row), ref(n_end2), ref(n_end34), ref(d));
    thread thr_row4(create_row, ref(mtr1), ref(row), ref(n_end34), ref(n_end), ref(d));
    thread thr_column1(create_column, ref(mtr2), ref(column), 0, ref(m_end4), ref(d));
    thread thr_column2(create_column, ref(mtr2), ref(column), ref(m_end4), ref(m_end2), ref(d));
    thread thr_column3(create_column, ref(mtr2), ref(column), ref(m_end2), ref(m_end34), ref(d));
    thread thr_column4(create_column, ref(mtr2), ref(column), ref(m_end34), ref(m_end), ref(d));
    thr_column1.join();
    thr_column2.join();
    thr_column3.join();
    thr_column4.join();
    thr_row1.join();
    thr_row2.join();
    thr_row3.join();
    thr_row4.join();

    thread thr_calculate1(calculate, ref(mtr1), ref(mtr2), ref(res), ref(row), ref(column), 0, ref(n_end8));
    thread thr_calculate2(calculate, ref(mtr1), ref(mtr2), ref(res), ref(row), ref(column), ref(n_end8), ref(n_end4));
    thread thr_calculate3(calculate, ref(mtr1), ref(mtr2), ref(res), ref(row), ref(column), ref(n_end4), ref(n_end38));
    thread thr_calculate4(calculate, ref(mtr1), ref(mtr2), ref(res), ref(row), ref(column), ref(n_end38), ref(n_end2));
    thread thr_calculate5(calculate, ref(mtr1), ref(mtr2), ref(res), ref(row), ref(column), ref(n_end2), ref(n_end58));
    thread thr_calculate6(calculate, ref(mtr1), ref(mtr2), ref(res), ref(row), ref(column), ref(n_end58), ref(n_end34));
    thread thr_calculate7(calculate, ref(mtr1), ref(mtr2), ref(res), ref(row), ref(column), ref(n_end34), ref(n_end78));
    thread thr_calculate8(calculate, ref(mtr1), ref(mtr2), ref(res), ref(row), ref(column), ref(n_end78), ref(n_end));
    thr_calculate1.join();
    thr_calculate2.join();
    thr_calculate3.join();
    thr_calculate4.join();
    thr_calculate5.join();
    thr_calculate6.join();
    thr_calculate7.join();
    thr_calculate8.join();
    auto end = chrono::steady_clock::now();
    double elapsed_seconds = chrono::duration_cast<chrono::duration<double>>(end - start).count();
    return elapsed_seconds;
}


void write_res(const string& name, const int& size, myT time_1, myT time_2, myT time_4, myT time_8)
{
    ofstream file;
    file.open(name, ios_base::app);
    file << size << ';' << time_1 << ';' << time_2 << ';' << time_4 << ';' << time_8 << '\n';
    file.close();
}

int main()
{
    srand(time(0));
    system("chcp 65001");
    /*
    double time_1 = 0, time_2 = 0, time_4 = 0, time_8 = 0;;
    for (int size = 101; size < 1002; size += 100)
    {
        Matrix mtr1(size, size);
        Matrix mtr2(size, size);
        Matrix res(size, size);
        mtr1.random();
        mtr2.random();
        for (auto i = 0; i < 100; i++)
        {
            time_1 += multiply_one(mtr1, mtr2, res);
            time_2 += multiply_two(mtr1, mtr2, res);
            time_4 += multiply_four(mtr1, mtr2, res);
            time_8 += multiply_eight(mtr1, mtr2, res);
        }
        write_res("tests001.txt", size, time_1 / 100.0, time_2 / 100.0, time_4 / 100.0, time_8 / 100.0);
        time_1 = 0;
        time_2 = 0;
        time_4 = 0;
        time_8 = 0;
    }
    */


    /*
    cout << "classic column" << endl;
    for (vector<int>::const_iterator i = column.begin(); i != column.end(); ++i)
        cout << *i << ' ';
    vector<int> row(mtr1.n);
    vector<int> column(mtr2.m);
    int d = mtr1.m - 1;
    create_row(mtr1, row, 0, mtr1.n, d);
    create_column(mtr2, column, 0, mtr2.m, d);
    calculate(mtr1, mtr2, res, row, column, 0, mtr1.n);
    */

    Matrix mtr1(1, 1);
    Matrix mtr2(1, 1);
    Matrix res(1, 1);
    mtr1.random();
    mtr2.random();
    cout << "первая матрица" << endl;
    mtr1.print();
    cout << "вторая матрица" << endl;
    mtr2.print();
    multiply_two(mtr1, mtr2, res);
    cout << "результат умножения (2 потока):" << endl;
    res.print();
    multiply_four(mtr1, mtr2, res);
    cout << "результат умножения (4 потока):" << endl;
    res.print();
    multiply_eight(mtr1, mtr2, res);
    cout << "результат умножения (8 потоков):" << endl;
    res.print();
    return 0;
}
