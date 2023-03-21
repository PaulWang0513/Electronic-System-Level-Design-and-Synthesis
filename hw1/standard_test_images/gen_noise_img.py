import cv2
import numpy as np

def add_noise(img, n):
    img2 = img
    for _ in range(n):
        x = int(np.random.random() * img.shape[0])
        y = int(np.random.random() * img.shape[1])
        img2[x, y] = 255 # white in gray scale is 255
    return img2

if __name__ == '__main__':
    # input original image file
    img = cv2.imread('./standart_test_images/lake.tif')
    # Write out original image in bmp format
    cv2.imwrite('./lake.bmp', img)

    # Add white noise points and write out another bmp file
    noise_img = add_noise(img, 100)
    cv2.imwrite('./lake_noise.bmp', noise_img)