import picamera
#from time import sleep
from picamera import PiCamera
import numpy as np
import matplotlib.pyplot as plt

camera = PiCamera()
camera.resolution = (2560, 1936)
camera.start_preview()
sleep(5)
camera.capture('test.jpg')
camera.stop_preview()

'''
h = 1024
cam_res = (int(h), int(0.75*h))
cam_res = (int(16*np.floor(cam_res[1]/16)), int(32*np.floor(cam_res[0]/32)))

cam = PiCamera()
cam.resolution = (cam_res[1], cam_res[0])
data = np.empty((cam_res[0], cam_res[1], 3), dtype = np.uint8)
while True:
    try:
        cam.capture(data, 'rgb')
        plt.imshow(data)
        data = np.empty((cam_res[0], cam_res[1], 3), dtype = np.uint8)
        plt.show()
        input("Click to save a different plot")
    except KeyboardInterrupt:
        break
'''