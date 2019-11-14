from algo import *


def print_mtr(mtr, n, m):
    for i in range(n):
        for j in range(m):
            print(mtr[i][j], ' ', end = '')

        print()


def input_mtr():
    n, m = map(int, input('Введите размеры матрицы: ').split())
    mtr = []

    for i in range(n):
        tmp = input('Введите ' + str(i) + ' строку матрицы: ').split()
        row = []
        for j in range(m):
            row.append(float(tmp[j]))
        mtr.append(row)

    return mtr, n, m


def main():
    mtr1, n1, m1 = input_mtr()
    mtr2, n2, m2 = input_mtr()
    if m1 != n2:
        print('Умножение матриц невозможно')
    else:
        res = standart_mult(mtr1, n1, mtr2, m2, m1)
        print('Standart algo:')
        print_mtr(res, n1, m2)
        res = vin_mult(mtr1, n1, mtr2, m2, m1)
        print('Vinograd algo:')
        print_mtr(res, n1, m2)
        res = opt_vin_mult(mtr1, n1, mtr2, m2, m1)
        print('Optimized Vinograd algo:')
        print_mtr(res, n1, m2)


if __name__ == '__main__':
    main()