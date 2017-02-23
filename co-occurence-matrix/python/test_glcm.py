import matplotlib.pyplot as plt

from skimage.feature import greycomatrix, greycoprops
from skimage import data

from skimage.io import imsave

from matplotlib.backends.backend_pdf import PdfPages

import glcm

import numpy as np

def test_4x4():
    image = np.array([[0, 0, 1, 1],\
                  [0, 0, 1, 1],\
                  [0, 2, 2, 2],\
                  [2, 2, 3, 3]],\
                  dtype=np.uint8)

    ray = [1]
    theta = [np.pi/2.]

    levels = 4

    glc_matrix = glcm.grey_level_co_occurence_m(image,levels,ray[0],theta[0])
    result = greycomatrix(image, ray, theta, levels, symmetric=False)

    print "==================="
    print "4x4 test pssed?"
    print glc_matrix.todense()
    print np.all(glc_matrix.todense()==result[:,:,0,0])
    print "==================="

def test_3x3():
    image = np.array([[0, 0, 1],\
                      [0, 0, 1],\
                      [0, 2, 2]])

    ray = [1]
    theta = [np.pi/2.]

    levels = 3

    glc_matrix = glcm.grey_level_co_occurence_m(image,levels,ray[0],theta[0])
    result = greycomatrix(image, ray, theta, levels, symmetric=False)

    print "==================="
    print "3x3 test pssed?"
    print np.all(glc_matrix.todense()==result[:,:,0,0])
    print "==================="

def camera_test():
    PATCH_SIZE = 13
    image = data.camera()

    loc = (255, 98)

    patch = image[loc[0]:loc[0] + PATCH_SIZE,
                  loc[1]:loc[1] + PATCH_SIZE]

    compiled_scikit_glcm = greycomatrix(patch, [5], [0], 256)

    nico_glcm = glcm.grey_level_co_occurence_m(patch,256,5.,0)
    interpreted_scikit_glcm = glcm.scikit(patch,256,5.,0)

    print "==================="
    print "camera test passed?"
    print np.all(nico_glcm.todense()== compiled_scikit_glcm[:,:,0,0])
    print np.all(nico_glcm.todense()== interpreted_scikit_glcm[:,:])
    print "==================="

def energy_4x4():
    image = np.array([[0, 0, 1, 1],\
                  [0, 0, 1, 1],\
                  [0, 2, 2, 2],\
                  [2, 2, 3, 3]],\
                  dtype=np.uint8)

    ray = [1]
    theta = [np.pi/2.]
    levels = 4
    result = greycomatrix(image, ray, theta, levels, symmetric=True)

    print pow(greycoprops(result, 'energy'),2)


    #print "==================="
    #print "4x4 test pssed?"
    #print glc_matrix.todense()
    #print np.all(glc_matrix.todense()==result[:,:,0,0])
    #print "==================="


def glcm_measures():
    patch_size = 13
    image = data.camera()

    loc = (261, 104)

    semi_patch = int((patch_size-1)/2)

    row_px = np.arange(loc[0]-semi_patch,loc[0]+semi_patch+1)
    col_px = np.arange(loc[1]-semi_patch,loc[1]+semi_patch+1)

    [R,C] = np.meshgrid(row_px,col_px)

    #print
    #print

    patch = image[R.transpose(),C.transpose()]
    #patch=patch.transpose()


    print patch

    glcm = greycomatrix(patch, [1], [0], 256, symmetric=True)

    nnz_id = np.nonzero(glcm[:,:,0,0])
    #print glcm[nnz_id]
    print np.sum(glcm[nnz_id]**2)
    #plt.spy(glcm)
    #plt.show()
    print pow(greycoprops(glcm, 'energy'),2)


    """print "==================="
    print "camera test passed?"
    print np.all(nico_glcm.todense()== compiled_scikit_glcm[:,:,0,0])
    print np.all(nico_glcm.todense()== interpreted_scikit_glcm[:,:])
    print '==================='"""


if __name__ == "__main__":
    test_4x4()
    test_3x3()
    camera_test()
    energy_4x4()
    glcm_measures()
