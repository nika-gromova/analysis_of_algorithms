from algo_sort import *
from random import randint
from time import process_time


def write_res(filename, size, time_st, time_v, time_opt_v):
    file = open(filename, 'a')
    file.write(str(size) + ';' + str(time_st) + ';' + str(time_v) + ';' + str(time_opt_v) + '\n')
    file.close()


def get_random_arr(size):
    res = []
    for i in range(size):
        res.append(randint(-1000, 1000))
    return res


def get_arr_for_merge(size):
    res = []
    for i in range(0, size, 2):
        res.append(i)
    for i in range(1, size, 2):
        res.append(i)
    return res


# arr = [i for i in range(size)]
# arr = get_random_arr(size)
# arr = [i for i in range(size - 1, -1, -1)]


def main():
    for size in range(100, 1001, 100):
        arr = get_arr_for_merge(size)
        time_start = process_time()
        for i in range(100):
            arr1 = arr.copy()
            selection_sort(arr1)
        time_end = process_time()
        ts = (time_end - time_start) / 100.00
        time_start = process_time()
        for i in range(100):
            arr2 = arr.copy()
            heap_sort(arr2)
        time_end = process_time()
        th = (time_end - time_start) / 100.00
        time_start = process_time()
        for i in range(100):
            arr3 = arr.copy()
            merge_sort(arr3)
        time_end = process_time()
        tm = (time_end - time_start) / 100.00
        write_res('test_merge.txt', size, ts, th, tm)


if __name__ == '__main__':
    main()
