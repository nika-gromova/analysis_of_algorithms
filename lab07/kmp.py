def KMPsearch(search_string, pattern):
    search_string.lower()
    pattern.lower()
    n = len(search_string)
    m = len(pattern)

    shift_array = compute_array(pattern, m)
    found = False
    k = 0
    p = 0
    while k < n:
        if pattern[p] == search_string[k]:
            k += 1
            p += 1
            if p == m:
                found = True
                print("KMP: match was found on index " + str(k - p) + "\n")
                p = shift_array[p - 1]
        else:
            if p == 0:
                k += 1
                if k > (n - m):
                    break
            else:
                p = shift_array[p - 1]
    if not found:
        print("KMP: pattern not found")


def compute_array(pattern, m):
    shift_array = [0] * m
    j = 0
    i = 1
    while i < m:
        if pattern[i] == pattern[j]:
            shift_array[i] = j + 1
            i += 1
            j += 1
        else:
            if j == 0:
                shift_array[i] = 0
                i += 1
            else:
                j = shift_array[j - 1]
    return shift_array
