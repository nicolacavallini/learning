import matplotlib.pyplot as plt

from skimage.feature import greycomatrix, greycoprops
from skimage import data

from skimage.io import imsave

from matplotlib.backends.backend_pdf import PdfPages

import glcm

import time

if __name__ == "__main__":


    PATCH_SIZE = 21

    # open the camera image
    image = data.camera()
    #print(type(image[0]))
    #print(image.shape)

    # select some patches from grassy areas of the image

    patches = []

    t0 = time.time()

    for i in range(0,image.shape[0]-PATCH_SIZE,1):
        for j in range(0,image.shape[1]-PATCH_SIZE,1):
            loc = (i,j)
            patches.append(image[loc[0]:loc[0] + PATCH_SIZE,
                                 loc[1]:loc[1] + PATCH_SIZE])

    t1 = time.time()

    print "patch slicicng time = ",str((t1-t0))

    kont = 0
    for patch in patches:
        print "patch: "+str(kont)+", out of:"+str(len(patches))
        #glc_matrix = greycomatrix(patch, [5], [0], 256, symmetric=True)
        glc_matrix = glcm.grey_level_co_occurence_m(patch,256,5.,0)
        glc_matrix += glc_matrix.transpose()
        #glc_matrix = glcm.scikit(patch,256,5.,0)
        #glc_matrix += glc_matrix.transpose()
        kont+=1

    t2 = time.time()

    print "glcm eval time = ",str((t2-t1))



    #grass_locations = [(474, 291), (440, 433), (466, 18), (462, 236)]
    #grass_patches = []
    #for loc in grass_locations:
    #    grass_patches.append(image[loc[0]:loc[0] + PATCH_SIZE,
    #                               loc[1]:loc[1] + PATCH_SIZE])

    #print(type(grass_patches))
    #print grass_patches[0]

    # select some patches from sky areas of the image
    #sky_locations = [(54, 48), (21, 233), (90, 380), (195, 330)]
    #sky_patches = []
    #for loc in sky_locations:
    #    sky_patches.append(image[loc[0]:loc[0] + PATCH_SIZE,
    #                             loc[1]:loc[1] + PATCH_SIZE])
