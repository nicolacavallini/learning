import numpy as np

A = np.array([[11,12,13,14],[21,22,23,24],[31,32,33,34]])
print(A)
r = [0,2]
c = [1,3]

C,R = np.meshgrid(c,r)
print(A[R,C])
 
