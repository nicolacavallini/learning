from skimage.io import imsave
from skimage.io import imread
from skimage import img_as_int
from skimage import img_as_ubyte
from skimage import img_as_uint

if __name__ == "__main__":

    img = imread("4x6_pixel_grayscale.png",True)
    img = img_as_ubyte(img)
    print img
    imsave("output.jpg",img)
