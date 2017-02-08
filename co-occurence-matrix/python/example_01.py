"""
Co-Occurence matrix example tacken form
scikit documentation:
http://scikit-image.org/docs/dev/api/skimage.feature.html?highlight=greycomatrix#skimage.feature.greycomatrix
"""
import numpy as np
from skimage.feature import greycomatrix, greycoprops

import glcm


if __name__== "__main__":

    #horizontal_difference()
    #vertical_difference()
    image = np.array([[0, 0, 1],\
                      [0, 0, 1],\
                      [0, 2, 2]])

    ray = [1]
    theta = [np.pi/2.]

    levels = 3

    start_pixel, end_pixel= \
    glcm.evaluate_geometrical_roule(image,ray[0],theta[0],levels)
    print "start = ", start_pixel
    print "end = ", end_pixel

    glc_matrix = glcm.grey_level_co_occurence_m(image,levels,start_pixel,end_pixel)



    #print start_matrix.todense()
    #print end_matrix.todense()
    print glc_matrix.todense()
    #print start_grey_vals




    result = greycomatrix(image, ray, theta, levels=3, symmetric=False)

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
