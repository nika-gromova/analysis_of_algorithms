import matplotlib.pyplot as plt


def read_data(file_name):
    sizes = []
    first = []
    second = []
    file = open(file_name, 'r')
    for line in file:
        tmp = line.split(',')
        sizes.append(int(tmp[0]))
        first.append(float(tmp[1]))
        second.append(float(tmp[2][:-1]))
    file.close()
    return sizes, first, second

def main():
    sizes, brute, aco = read_data('time.txt')
    plt.plot(sizes, brute, 'g', sizes, aco, 'b')
    plt.xlabel('размер матрицы')
    plt.ylabel('время (секунды)')
    plt.legend(['полный перебор', 'муравьиный'])
    plt.title('задача коммивояжера')
    plt.xticks(sizes)
    plt.show()


if __name__ == "__main__":
    main()
