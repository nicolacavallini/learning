"""
Co-Occurence matrix example tacken form
scikit documentation:
http://scikit-image.org/docs/dev/api/skimage.feature.html?highlight=greycomatrix#skimage.feature.greycomatrix
"""
import numpy as np
from skimage.feature import greycomatrix, greycoprops

if __name__== "__main__":

    image = np.array([[0, 0, 1],\
                      [0, 0, 1],\
                      [0, 2, 2]])

    c_left = np.array([[1, 1, 0, 1, 1, 0, 1, 0, 0 ],\
                       [0, 0, 1, 0, 0, 1, 0, 0, 0 ],\
                       [0, 0, 0, 0, 0, 0, 0, 1, 1 ]])


    perm = np.eye(9,9,1,dtype=int) + np.eye(9,9,-8,dtype=int)

    c_right = c_left.dot(perm)

    print c_left.dot(np.transpose(c_right))

    ray = [1]
    theta = [0]
    result = greycomatrix(image, ray, theta, levels=3, symmetric=True)

    print result[:,:,0,0]


    """image = np.array([[0, 0, 1, 1],\
                  [0, 0, 1, 1],\
                  [0, 2, 2, 2],\
                  [2, 2, 3, 3]],\
                  dtype=np.uint8)"""
    """connectivity = np.array([[1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0],\
                             [0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0],\
                             [0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0],\
                             [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1]],\
                      dtype=np.uint8)

    perm  = np.eye(16,16,1)
    print perm

    test = connectivity.dot(perm)
    test = test.dot(np.transpose(connectivity))

    print test"""

    """result_n = greycomatrix(image, ray, theta, levels=4,\
                            symmetric=False, normed=True)

    print("ray = ", ray)
    kont = 0
    for t in theta:
        print("theta = ", t)
        print result[:, :, 0, kont]
        print result_n[:, :, 0, kont]
        kont+=1

    print "energy not normed", greycoprops(result, 'energy')

    print "energy normed", greycoprops(result_n, 'energy')

    print "contrast", greycoprops(result, 'contrast')

    print "dissimilarity", greycoprops(result, 'dissimilarity')

    print "homogeneity", greycoprops(result, 'homogeneity')

    print "ASM", greycoprops(result, 'ASM')

    print "correlation", greycoprops(result, 'correlation')

    vec = np.array([13, 14, 15, 16])

    print "vector std dev = ", np.std(vec)

    print "vector std mean = ", np.mean(vec)"""
