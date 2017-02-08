import numpy as np
from scipy.sparse import coo_matrix

def evaluate_geometrical_roule(image,distance,angle):

    (rows,cols) = image.shape

    start_pixel = []
    end_pixel = []

    length = rows*cols

    delta_r = int(round(np.sin(angle) * distance))
    delta_c = int(round(np.cos(angle) * distance))

    for r in range(rows):
        for c in range(cols):

            row = r + delta_r
            col = c + delta_c

            # make sure the offset is within bounds
            if row >= 0 and row < rows and \
               col >= 0 and col < cols:

                start_pixel_id = r * cols + c
                end_pixel_id = row * cols + col

                start_pixel.append(start_pixel_id)
                end_pixel.append(end_pixel_id)

    return np.array(start_pixel), np.array(end_pixel)

def grey_level_co_occurence_m(image,levels,distance,angle):
    start_pixel, end_pixel = \
    evaluate_geometrical_roule(image,distance,angle)
    start_grey_vals = image.flatten()[start_pixel]

    start_matrix = coo_matrix((np.ones(start_pixel.shape),\
                               (start_grey_vals,range(start_pixel.shape[0]))),
                               (levels,start_pixel.shape[0]),\
                               dtype=np.int8)

    end_grey_vals = image.flatten()[end_pixel]
    end_matrix = coo_matrix((np.ones(end_pixel.shape),\
                               (range(end_pixel.shape[0]),end_grey_vals)),
                               (end_pixel.shape[0],levels),\
                               dtype=np.uint8)

    return start_matrix * end_matrix

def scikit(image,levels,distance,angle):
    rows = image.shape[0]
    cols = image.shape[1]

    out = np.zeros((levels, levels),
                 dtype=np.uint8)

    for r in range(rows):
        for c in range(cols):
            i = image[r, c]

            # compute the location of the offset pixel
            row = r + int(round(np.sin(angle) * distance))
            col = c + int(round(np.cos(angle) * distance))

            # make sure the offset is within bounds
            if row >= 0 and row < rows and \
               col >= 0 and col < cols:
                j = image[row, col]

                if i >= 0 and i < levels and \
                   j >= 0 and j < levels:
                    out[i, j] += 1

    return out
