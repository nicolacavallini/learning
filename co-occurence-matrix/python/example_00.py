"""
Co-Occurence matrix example tacken form
scikit documentation:
http://scikit-image.org/docs/dev/api/skimage.feature.html?highlight=greycomatrix#skimage.feature.greycomatrix
"""
import numpy as np
from skimage.feature import greycomatrix

def greycoprops(P, prop='contrast'):

    (num_level, num_level2, num_dist, num_angle) = P.shape
    assert num_level == num_level2
    assert num_dist > 0
    assert num_angle > 0

    # create weights for specified property
    I, J = np.ogrid[0:num_level, 0:num_level]

    print I
    print J

    if prop == 'contrast':
        weights = (I - J) ** 2
    elif prop == 'dissimilarity':
        weights = np.abs(I - J)
    elif prop == 'homogeneity':
        weights = 1. / (1. + (I - J) ** 2)
    elif prop in ['ASM', 'energy', 'correlation']:
        pass
    else:
        raise ValueError('%s is an invalid property' % (prop))

    # compute property for each GLCM
    if prop == 'energy':
        asm = np.apply_over_axes(np.sum, (P ** 2), axes=(0, 1))[0, 0]
        results = np.sqrt(asm)
    elif prop == 'ASM':
        results = np.apply_over_axes(np.sum, (P ** 2), axes=(0, 1))[0, 0]
    elif prop == 'correlation':
        print "================================================================"
        results = np.zeros((num_dist, num_angle), dtype=np.float64)
        I = np.array(range(num_level)).reshape((num_level, 1, 1, 1))
        J = np.array(range(num_level)).reshape((1, num_level, 1, 1))

        print I.shape
        print I[:,:,0,0]
        print J.shape
        print J[:,:0,0]
        print P.shape
        print P[:,:,0,0]
        print "----------------------------------------------------------------"
        print "P*I"
        Z = I*P
        print Z.shape
        print Z[:,:,0,0]
        c = np.apply_over_axes(np.sum, (I * P), axes=(0, 1))
        print c.shape
        print "----------------------------------------------------------------"
        print "P*J"
        Z = J*P
        print Z.shape
        print Z[:,:,0,0]
        print "----------------------------------------------------------------"

        #print I

        diff_i = I - np.apply_over_axes(np.sum, (I * P), axes=(0, 1))[0, 0]
        print "----------------------------------------------------------------"
        print "diff_i"
        print diff_i



        diff_j = J - np.apply_over_axes(np.sum, (J * P), axes=(0, 1))[0, 0]
        print "----------------------------------------------------------------"
        print "diff_j"
        print diff_j

        print "----------------------------------------------------------------"
        print "(P * (diff_i) ** 2)"
        print (P * (diff_i) ** 2)[:,:,0,0]

        print "diff_i = ", diff_i[:,:,0,0]
        print "diff_j = ", diff_j[:,:,0,0]

        std_i = np.sqrt(np.apply_over_axes(np.sum, (P * (diff_i) ** 2),
                                           axes=(0, 1))[0, 0])
        std_j = np.sqrt(np.apply_over_axes(np.sum, (P * (diff_j) ** 2),
                                           axes=(0, 1))[0, 0])

        print "P * (diff_i * diff_j) = "
        print np.apply_over_axes(np.sum, P*(diff_i * diff_j),
                                 axes=(0, 1))[0,0]

        cov = np.apply_over_axes(np.sum, (P * (diff_i * diff_j)),
                                 axes=(0, 1))[0, 0]

        # handle the special case of standard deviations near zero
        mask_0 = std_i < 1e-15
        print std_i
        print std_j
        print mask_0

        mask_0[std_j < 1e-15] = True
        results[mask_0] = 1

        print results

        # handle the standard case
        mask_1 = mask_0 == False
        results[mask_1] = cov[mask_1] / (std_i[mask_1] * std_j[mask_1])
        print "================================================================"
    elif prop in ['contrast', 'dissimilarity', 'homogeneity']:
        weights = weights.reshape((num_level, num_level, 1, 1))
        results = np.apply_over_axes(np.sum, (P * weights), axes=(0, 1))[0, 0]

    return results



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

    result_n = greycomatrix(image, ray, theta, levels=4,\
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

    print "vector std mean = ", np.mean(vec)
