import numpy as np
from skimage.feature import greycomatrix, greycoprops

def breakline():
    print("=====================================================")

if __name__== "__main__":

    #horizontal_difference()
    #vertical_difference()
    breakline()
    image = np.array([[0, 0, 1, 1],\
                      [0, 0, 1, 1],\
                      [0, 2, 2, 2],\
                      [2, 2, 3, 3]])

    ray = [1]
    theta = [np.pi/2.]

    result = greycomatrix(image, ray, theta, levels=4,\
                            symmetric=True, normed=False)

    glcm = result[:,:,0,0]
    print glcm

    print "energy not normed", greycoprops(result, 'energy')**2
    breakline()

    result = greycomatrix(image, ray, theta, levels=4,\
                            symmetric=False, normed=True)

    glcm = result[:,:,0,0]
    print glcm

    print "energy  normed", greycoprops(result, 'energy')**2
    breakline()


    result = greycomatrix(image, ray, theta, levels=4,\
                            symmetric=True, normed=True)

    glcm = result[:,:,0,0]
    print glcm

    print "energy sym normed", greycoprops(result, 'energy')**2
    breakline()

    #glcm_n = result[:,:,0,0]

    #I = np.diag()
