#ifndef MATRIX_H
#define MATRIX_H
#include <cstdlib>
#include <iostream>

#define MIN_DISTANCE 1
#define MAX_DISTANCE 10

#define TEST_SIZE 10

#define EQ_EPS 1e-90
#define	is_zero(x)	(((x) > -EQ_EPS && (x) < EQ_EPS))


using namespace std;
template<typename Type>
class Matrix
{
public:
    int n;
    Type **mtr;
public:
    Matrix() = delete;
    Matrix(int nc)
    {
        n = nc;
        mtr = new Type *[n];
        for (auto i = 0; i < n; i++)
        {
            mtr[i] = new Type[n];
            for (auto j = 0; j < n; j++)
            {
                mtr[i][j] = 0;
            }
        }
    }

    ~Matrix(void)
    {
        for (auto i = 0; i < n; i++)
        {
            delete mtr[i];
        }
        delete mtr;
    }

    void zero(void)
    {
        for (auto i = 0; i < n; i++)
        {
            for (auto j = 0; j < n; j++)
            {
                mtr[i][j] = 0;
            }
        }
    }

    void print(void) const
    {
        cout << "   | ";
        for (auto i = 0; i < n; i++)
        {
            cout.width(2);
            cout << left << i << " ";
        }
        cout << endl;
        for (auto i = 0; i < 3 * n + 4; i++)
            cout << "-";
        cout << endl;
        for (auto i = 0; i < n; i++)
        {
            cout.width(3);
            cout << i << "| ";
            for (auto j = 0; j < n; j++)
            {
                cout.width(2);
                cout << left << mtr[i][j] << ' ';
            }
            cout << endl;
        }
        cout << endl;
    }

    void fill_int_random(void)
    {
        int tmp = 1;
        for (auto i = 0; i < n; i++)
        {
            for (auto j = i + 1; j < n; j++)
            {
                tmp = MIN_DISTANCE + rand() % MAX_DISTANCE;
                mtr[i][j] = tmp;
                mtr[j][i] = tmp;
            }
        }
    }

    void fill_value(double value)
    {
        for (auto i = 0; i < n; i++)
        {
            for (auto j = 0; j < n; j++)
            {
                mtr[i][j] = static_cast<Type>(value);
            }
        }
    }

    void fill_array(Type array[TEST_SIZE][TEST_SIZE])
    {
        for (auto i = 0; i < TEST_SIZE; i++)
        {
            for (auto j = 0; j < TEST_SIZE; j++)
            {
                mtr[i][j] = array[i][j];
            }
        }
    }

    double compute_avg(void) const
    {
        double sum = 0.0;
        int num = 0;
        for (auto i = 0; i < n; i++)
        {
            for (auto j = i + 1; j < n; j++)
            {
                sum += mtr[i][j];
                num++;
            }
        }
        return sum / num;
    }

    void check_zero(Type tmin)
    {
        for (auto i = 0; i < n; i++)
        {
            for (auto j = 0; j < n; j++)
            {
                if (is_zero(mtr[i][j]))
                    mtr[i][j] = tmin;
            }
        }
    }

    Matrix &operator= (Matrix<Type> &mtr2)
    {
        if (mtr2.n != n)
            return (*this);
        for (auto i = 0; i < n; i++)
        {
            for (auto j = 0; j < n; j++)
            {
                mtr[i][j] = (mtr2.mtr)[i][j];
            }
        }
        return (*this);
    }

    Matrix &operator+= (Matrix<Type> &mtr2)
    {
        if (mtr2.n != n)
            return (*this);
        for (auto i = 0; i < n; i++)
        {
            for (auto j = 0; j < n; j++)
            {
                mtr[i][j] += (mtr2.mtr)[i][j];
            }
        }
        return (*this);
    }

    Matrix &operator*= (double value)
    {
        for (auto i = 0; i < n; i++)
        {
            for (auto j = 0; j < n; j++)
            {
                mtr[i][j] *= value;
            }
        }
        return (*this);
    }

    // only for double matrix
    void reverse_values(const Matrix<int> &mtr2)
    {
        for (auto i = 0; i < n; i++)
        {
            for (auto j = i + 1; j < n; j++)
            {
                double tmp = static_cast<double>(1.0 / (mtr2.mtr)[i][j]);
                mtr[i][j] = tmp;
                mtr[j][i] = tmp;
            }
        }
    }

    Type* operator[](const int &i) const
    {
        return (mtr[i]);
    }


};


#endif // MATRIX_H
