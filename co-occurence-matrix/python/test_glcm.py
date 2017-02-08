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

compiled_scikit_glcm = greycomatrix(patch, [5], [0], 256)

nico_glcm = glcm.grey_level_co_occurence_m(patch,256,5.,0)
interpreted_scikit_glcm = glcm.scikit(patch,256,5.,0)

print np.all(nico_glcm.todense()== compiled_scikit_glcm[:,:,0,0])
print np.all(nico_glcm.todense()== interpreted_scikit_glcm[:,:])
