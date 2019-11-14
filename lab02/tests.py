from algo import *
from random import randint
from time import process_time


def get_square_mtr(size):
    res = [[0] * size for i in range(size)]
    for i in range(size):
        for j in range(size):
            res[i][j] = randint(-100, 100)
    return res


def write_res(size, time_st, time_v, time_opt_v):
    file = open('tests.txt', 'a')
    file.write(str(size) + ';' + str(time_st) + ';' + str(time_v) + ';' + str(time_opt_v) + '\n')
    file.close()


def main():
    for size in range(100, 1001, 100):
        mtr1 = get_square_mtr(size)
        mtr2 = get_square_mtr(size)
        time_start = process_time()
        for i in range(10):
            standart_mult(mtr1, size, mtr2, size, size)
        time_end = process_time()
        ts = (time_end - time_start) / 10.00
        time_start = process_time()
        for i in range(10):
            vin_mult(mtr1, size, mtr2, size, size)
        time_end = process_time()
        tv = (time_end - time_start) / 10.00
        time_start = process_time()
        for i in range(10):
            opt_vin_mult(mtr1, size, mtr2, size, size)
        time_end = process_time()
        tov = (time_end - time_start) / 10.00
        write_res(size, ts, tv, tov)


if __name__ == '__main__':
    main()
