import matplotlib.pyplot as plt

from skimage.feature import greycomatrix, greycoprops
from skimage import data

from skimage.io import imsave

from matplotlib.backends.backend_pdf import PdfPages

import glcm

import numpy as np


PATCH_SIZE = 21

# open the camera image
image = data.camera()
#print(type(image[0]))
#print(image.shape)

loc = (474, 291)

patch = image[loc[0]:loc[0] + PATCH_SIZE,
              loc[1]:loc[1] + PATCH_SIZE]

scikit_glcm = greycomatrix(patch, [5], [0], 256)

start_pixel, end_pixel= \
glcm.evaluate_geometrical_roule(patch,5.,0,256)
nico_glcm = glcm.grey_level_co_occurence_m(patch,256,start_pixel,end_pixel)

print np.all(nico_glcm.todense()== scikit_glcm[:,:,0,0])

#print type(nico_glcm)

#plt.spy(scikit_glcm[:,:,0,0])
#plt.show()

#plt.spy(nico_glcm.todense())
#plt.show()

# select some patches from grassy areas of the image
"""grass_locations = [(474, 291), (440, 433), (466, 18), (462, 236)]
grass_patches = []
for loc in grass_locations:
    grass_patches.append(image[loc[0]:loc[0] + PATCH_SIZE,
                               loc[1]:loc[1] + PATCH_SIZE])

#print grass_patches[0]

# select some patches from sky areas of the image
sky_locations = [(54, 48), (21, 233), (90, 380), (195, 330)]
sky_patches = []
for loc in sky_locations:
    sky_patches.append(image[loc[0]:loc[0] + PATCH_SIZE,
                             loc[1]:loc[1] + PATCH_SIZE])

# compute some GLCM properties each patch
xs = []
ys = []
pp = PdfPages('sparsity_patterns.pdf')
for patch in (grass_patches + sky_patches):
    glcm = greycomatrix(patch, [5], [0], 256, symmetric=True, normed=True)"""
