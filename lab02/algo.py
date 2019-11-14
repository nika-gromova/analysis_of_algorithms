def standart_mult(mtr1, n1, mtr2, m2, eq):
    res = [[0] * m2 for i in range(n1)]

    for i in range(n1):
        for j in range(m2):
            for k in range(eq):
                res[i][j] = res[i][j] + mtr1[i][k] * mtr2[k][j]
    return res


def opt_vin_mult(mtr1, n1, mtr2, m2, eq):
    res = [[0] * m2 for i in range(n1)]

    mul_row = [0] * n1
    mul_column = [0] * m2

    d = eq - 1
    eq2 = eq % 2
    for i in range(n1):
        for k in range(0, d, 2):
            mul_row[i] -= mtr1[i][k] * mtr1[i][k + 1]

    for i in range(m2):
        for k in range(0, d, 2):
            mul_column[i] -= mtr2[k][i] * mtr2[k + 1][i]

    for i in range(n1):
        for j in range(m2):
            buf = mul_row[i] + mul_column[j]
            for k in range(0, d, 2):
                buf += (mtr1[i][k] + mtr2[k + 1][j]) * (mtr1[i][k + 1] + mtr2[k][j])
            if eq2:
                buf += mtr1[i][d] * mtr2[d][j]
            res[i][j] = buf

    return res


def vin_mult(mtr1, n1, mtr2, m2, eq):
    res = [[0] * m2 for i in range(n1)]

    mul_row = [0] * n1
    mul_column = [0] * m2

    for i in range(n1):
        for k in range(eq // 2):
            mul_row[i] = mul_row[i] + mtr1[i][2 * k] * mtr1[i][2 * k + 1]

    for i in range(m2):
        for k in range(eq // 2):
            mul_column[i] = mul_column[i] + mtr2[2 * k][i] * mtr2[2 * k + 1][i]

    for i in range(n1):
        for j in range(m2):
            res[i][j] = - mul_row[i] - mul_column[j]
            for k in range(eq // 2):
                res[i][j] = res[i][j] + (mtr1[i][2 * k] + mtr2[2 * k + 1][j]) * (mtr1[i][2 * k + 1] + mtr2[2 * k][j])

    if eq % 2:
        for i in range(n1):
            for j in range(m2):
                res[i][j] = res[i][j] + mtr1[i][eq - 1] * mtr2[eq - 1][j]

    return res