from skimage.io import imsave
from skimage.io import imread
from skimage import img_as_int
from skimage import img_as_ubyte
from skimage import img_as_uint

from skimage import data

def camera_image():
    img = imread("camera.png",True)
    img = img_as_ubyte(img)

    print img[0:10,0:10]

    imsave("out_camera.png",img)

def test_image():
    img = imread("4x6_pixel_grayscale.png",True)
    img = img_as_ubyte(img)
    print img
    imsave("output.jpg",img)

if __name__ == "__main__":
    test_image()
    camera_image()
