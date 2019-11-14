def selection_sort(arr):
    n = len(arr)
    for i in range(n):
        flag = 0
        min_index = i
        for j in range(i + 1, n):
            if arr[min_index] > arr[j]:
                min_index = j
                flag = 1
        if flag:
            tmp = arr[min_index]
            arr[min_index] = arr[i]
            arr[i] = tmp


def merge_sort(arr):
    n = len(arr)
    if n > 1:
        mid = n // 2
        left_arr = arr[:mid]
        right_arr = arr[mid:]
        merge_sort(left_arr)
        merge_sort(right_arr)

        i = j = k = 0
        nl = len(left_arr)
        nr = len(right_arr)
        while i < nl and j < nr:
            if left_arr[i] < right_arr[j]:
                arr[k] = left_arr[i]
                i += 1
            else:
                arr[k] = right_arr[j]
                j += 1
            k += 1

        while i < nl:
            arr[k] = left_arr[i]
            i += 1
            k += 1

        while j < nr:
            arr[k] = right_arr[j]
            j += 1
            k += 1


def heapify(arr, n, index):
    largest = index
    left = 2 * index + 1
    right = 2 * index + 2

    if left < n and arr[index] < arr[left]:
        largest = left

    if right < n and arr[largest] < arr[right]:
        largest = right

    if largest != index:
        arr[index], arr[largest] = arr[largest], arr[index]
        heapify(arr, n, largest)


def heap_sort(arr):
    n = len(arr)

    for i in range(n - 1, -1, -1):
        heapify(arr, n, i)
    for i in range(n - 1, 0, -1):
        arr[i], arr[0] = arr[0], arr[i]
        heapify(arr, i, 0)
