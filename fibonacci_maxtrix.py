import numpy as np
def matrix_power(A,p):
    if p==0:
        return 1
    if p==1:
        return A
    if p%2==1:
        return A*matrix_power(A,p-1)
    B = matrix_power(A,p/2)
    return B*B


def fibonacci(a1,a2,n):
    magic = np.matrix([[1,1],[1,0]])
    initial = np.matrix([[a2,a1],[a1,0]])
    return (initial*matrix_power(magic,n-1))[0,0]


    