from algo_sort import *


def input_array():
    tmp = input('Введите массив: ').split()
    array = []
    n = len(tmp)
    for i in range(n):
        array.append(int(tmp[i]))
    return array


def main():
    arr = input_array()
    arr1 = arr.copy()
    selection_sort(arr1)
    print('Сортировка выбором: ', arr1)
    arr2 = arr.copy()
    heap_sort(arr2)
    print('Пирамидальная сортировка: ', arr2)
    arr3 = arr.copy()
    merge_sort(arr3)
    print('Сортировка слиянием: ', arr3)


if __name__ == '__main__':
    main()
