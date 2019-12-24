def BMHsearch(search_string, pattern):
    search_string.lower()
    pattern.lower()
    n = len(search_string)
    m = len(pattern)

    shift_array = compute_shift(pattern, m)

    found = False

    i = 0
    while i <= (n - m):
        j = m - 1
        while (j >= 0) and pattern[j] == search_string[i + j]:
            j -= 1
        if j < 0:
            found = True
            print("BMH: match was found on index " + str(i) + "\n")
            i += (shift_array[search_string[i + m]] if i + m < n else 1)
        else:
            if shift_array.get(search_string[i + j]) is None:
                i += m
            else:
                i += shift_array[search_string[i + j]]
    if not found:
        print("BMH: pattern not found.\n")


def compute_shift(pattern, m):
    shift_array = {}
    value = m - 1
    for i in range(m - 1):
        shift_array[(pattern[i])] = value
        value -= 1
    return shift_array
