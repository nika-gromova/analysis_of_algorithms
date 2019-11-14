def levenshtein_distance_mtr(s1, s2):
    len_s1 = len(s1)
    len_s2 = len(s2)

    if (len_s1 > len_s2):
        len_s1, len_s2 = len_s2, len_s1
        s1, s2 = s2, s1
        
    
    cur_row = [i for i in range(len_s1 + 1)]

    for i in range(1, len_s2 + 1):
        prev_row, cur_row = cur_row, [0 for i in range(len_s1 + 1)]
        cur_row[0] = i;
        for j in range(1, len_s1 + 1):
            tmp = 1
            if (s1[j - 1] == s2[i - 1]):
                tmp = 0
            cur_row[j] = min(prev_row[j] + 1, cur_row[j - 1] + 1, prev_row[j - 1] + tmp)

    return cur_row[len_s1]

def levenshtein_distance_rec(s1, s2):
    len_s1 = len(s1)
    len_s2 = len(s2)
    if (len_s1 == 0):
        return len_s2
    if (len_s2 == 0):
        return len_s1

    insert = levenshtein_distance_rec(s1, s2[:(len_s2 - 1)]) + 1
    delete = levenshtein_distance_rec(s1[: (len_s1 - 1)], s2) + 1
    tmp = 0
    if (s1[len_s1 - 1] != s2[len_s2 - 1]):
        tmp = 1
    replace = levenshtein_distance_rec(s1[: (len_s1 - 1)], s2[: (len_s2 - 1)]) + tmp

    return min(insert, delete, replace)

def damerau_levenshtein_distance(s1, s2):
    len_s1 = len(s1)
    len_s2 = len(s2)

    if (len_s1 > len_s2):
        len_s1, len_s2 = len_s2, len_s1
        s1, s2 = s2, s1

    cur_row = [i for i in range(len_s1 + 1)]
    save_row = [0] * (len_s1 + 1)
    
    for i in range(1, len_s2 + 1):
        prev_row, cur_row = cur_row, [0 for i in range(len_s1 + 1)]
        cur_row[0] = i;
        for j in range(1, len_s1 + 1):  
            tmp = 1
            if (s1[j - 1] == s2[i - 1]):
                tmp = 0
            insert = cur_row[j - 1] + 1
            delete = prev_row[j] + 1
            replace = prev_row[j - 1] + tmp
            cur_row[j] = min(insert, delete, replace)
            if (i > 1 and j > 1):
                if ((s1[j - 2] == s2[i - 1]) and (s1[j - 1] == s2[i - 2])):
                    cur_row[j] = min(cur_row[j], save_row[j - 2] + 1)
        save_row = prev_row
                

    return cur_row[len_s1]
    

    
print(damerau_levenshtein_distance('угол', 'гулл'))

print(levenshtein_distance_rec('угол', 'гулл'))

print(levenshtein_distance_mtr('угол', 'гулл'))
