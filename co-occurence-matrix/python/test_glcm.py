import matplotlib.pyplot as plt

from skimage.feature import greycomatrix, greycoprops
from skimage import data

from skimage.io import imsave
from skimage.io import imread

from matplotlib.backends.backend_pdf import PdfPages

import glcm

import numpy as np

def get_direction(in_angle):

    if abs(in_angle) - 0. < 1e-10:
        direction ="east"
    elif abs(in_angle - np.pi/2.) < 1e-10:
        direction ="south"
    elif abs(in_angle - np.pi) < 1e-10:
        direction ="west"
    elif abs(in_angle - np.pi) < 1e-10:
        direction ="west"
    elif abs(in_angle - 3./2. * np.pi) < 1e-10:
        direction ="north"
    elif abs(in_angle - 1./4. * np.pi) < 1e-10:
        direction ="south-east"
    elif abs(in_angle - 3./4. * np.pi) < 1e-10:
        direction ="south-west";
    elif abs(in_angle - 5./4. * np.pi) < 1e-10:
        direction ="north-west"
    elif abs(in_angle - 7./4. * np.pi) < 1e-10:
        direction ="north-east";
    return direction

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
    theta = [np.pi/4.]

    levels = 3

    glc_matrix = glcm.grey_level_co_occurence_m(image,levels,ray[0],theta[0])

    print glc_matrix.todense()


    result = greycomatrix(image, ray, theta, levels, symmetric=False)

    print "==================="
    print "3x3 test pssed?"
    print np.all(glc_matrix.todense()==result[:,:,0,0])
    print "==================="

def test_6x6():

    """grey_levels = 256
    img = imread("6x6_pixel_grayscale.png", as_grey=True)"""

    grey_levels = 4
    img = np.array([[0, 0, 1, 1,0,1],\
                  [0, 0, 1, 1,0,2],\
                  [0, 2, 2, 2,1,1],\
                  [2, 2, 3, 3,3,2],\
                  [1, 1, 2, 3,1,2],
                  [3, 3, 1, 1,2,2]],\
                  dtype=np.uint8)


    angles = np.arange(0,7)/4.*np.pi
    distance = 2.

    for i in range(2,4):
        for j in range(2,4):
            patch = img[i-2:i+2,j-2:j+2]
            for a in angles:
                glcm_ = glcm.grey_level_co_occurence_m(patch,grey_levels,distance,a)
                print get_direction(a)
                print glcm_.todense()


def camera_test():
    PATCH_SIZE = 13
    image = data.camera()

    loc = (255, 98)

    patch = image[loc[0]:loc[0] + PATCH_SIZE,
                  loc[1]:loc[1] + PATCH_SIZE]

    compiled_scikit_glcm = greycomatrix(patch, [5], [0], 256)

    nico_glcm = glcm.grey_level_co_occurence_m(patch,256,5.,0)
    interpreted_scikit_glcm = glcm.scikit(patch,256,5.,0)

    plt.spy(nico_glcm)
    plt.show()

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



    """print "==================="
    print "camera test passed?"
    print np.all(nico_glcm.todense()== compiled_scikit_glcm[:,:,0,0])
    print np.all(nico_glcm.todense()== interpreted_scikit_glcm[:,:])
    print '==================='"""


if __name__ == "__main__":
    test_4x4()
    test_3x3()
    test_6x6()
    camera_test()
    energy_4x4()
    glcm_measures()
