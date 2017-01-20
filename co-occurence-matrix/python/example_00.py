"""
Co-Occurence matrix example tacken form
scikit documentation:
http://scikit-image.org/docs/dev/api/skimage.feature.html?highlight=greycomatrix#skimage.feature.greycomatrix
"""
import numpy as np
from skimage.feature import greycomatrix
from skimage.feature import greycoprops

if __name__== "__main__":

    image = np.array([[0, 0, 1, 1],\
                      [0, 0, 1, 1],\
                      [0, 2, 2, 2],\
                      [2, 2, 3, 3]],\
                      dtype=np.uint8)

    ray = [1.]
    #theta = [0, np.pi/6, np.pi/4, np.pi/3, np.pi/2, 3*np.pi/4]
    theta = [np.pi/6]

    result = greycomatrix(image, ray, theta, levels=4)

    print "energy not normed", greycoprops(result, 'energy')


    result_n = greycomatrix(image, ray, theta, levels=4,\
                            symmetric=False, normed=True)

    print "energy normed", greycoprops(result_n, 'energy')


    print("ray = ", ray)
    kont = 0
    for t in theta:
        print("theta = ", t)
        print result[:, :, 0, kont]
        print result_n[:, :, 0, kont]
        kont+=1
