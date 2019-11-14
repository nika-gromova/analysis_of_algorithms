from lab_1_full_mtr import *
import matplotlib.pyplot as plt
import numpy as np
from time import time
import random
import string

def random_word(length):
   return ''.join(random.choice(string.ascii_lowercase) for i in range(length))

def print_mtr(mtr):
    for i in range(len(mtr)):
        for j in range(len(mtr[i])):
            print(mtr[i][j], end = ' ')
        print()
if __name__ == '__main__':
    
    ####### tests #######
    lens = []
    times_l = []
    times_d_l = []
    times_d_l_rec = []

    for length in range(0, 7, 1):
        lens.append(length)
        s1 = random_word(length)
        s2 = random_word(length)
        print(s1, s2)
        print('*********************************************')
        '''
        t_start = time()
        for i in range(100):
            levenshtein_distance_mtr(s1, s2)
        times_l.append((time() - t_start) / 100)
        '''
        t_start = time()
        for i in range(10):
            damerau_levenshtein_distance_mtr(s1, s2)
        times_d_l.append((time() - t_start) / 10)
        
        t_start = time()
        for i in range(10):
            damerau_levenshtein_distance_rec(s1, s2)
        times_d_l_rec.append((time() - t_start) / 10)
        
        print('cur len', length, 'finished')

       
    mtr_l = np.array(times_l)
    mtr_d_l = np.array(times_d_l)
    rec_d_l = np.array(times_d_l_rec)

    
    plt.plot(lens, mtr_d_l, 'y', lens, rec_d_l, 'g')
    plt.xlabel('длина строк')
    plt.ylabel('время (секунды)')
    plt.legend(['матричная реализация','рекурсивная реализация'])
    plt.show()
    
    '''
    plt.plot(lens, mtr_l, lens, mtr_d_l, 'r')
    plt.xlabel('длина строк')
    plt.ylabel('время (секунды)')
    plt.legend(['расстояние Левенштейна','расстояние Дамерау-Левенштейна'])
    plt.show()
    '''

    '''
    while 1:
        s1 = input('Введите первую сроку: ')
        s2 = input('Введите вторую строку: ')

        print('Результат:')
        mtr, res = levenshtein_distance_mtr(s1, s2)
        print('Расстояние Левенштейна (матричный алгоритм):', res)
        print_mtr(mtr)

        mtr, res = damerau_levenshtein_distance_mtr(s1, s2)
        print('Расстояние Дамерау-Левенштейна (матричный алгоритм):', res)
        print_mtr(mtr)

        res = damerau_levenshtein_distance_rec(s1, s2)
        print('Расстояние Дамерау-Левенштейна (рекурсивный алгоритм):', res)
    '''
