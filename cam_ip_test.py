import cv2
import numpy as np

cam = cv2.VideoCapture(0)

while True:
    ret, img = cam.read()

    hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV) #convert colorspace
    
    lower_bound = np.array([90,150,50])
    upper_bound = np.array([150,255,255])
    mask_b = cv2.inRange(hsv, lower_bound, upper_bound)
    kernel = np.ones((7,7), np.uint8)
    mask_b = cv2.morphologyEx(mask_b, cv2.MORPH_CLOSE, kernel) #remove black noise from white
    mask_b = cv2.morphologyEx(mask_b, cv2.MORPH_OPEN, kernel) #remove white noise from black
    mask_img_b = cv2.bitwise_and(img, img, mask=mask_b)
    contours_b, hierarchy_b = cv2.findContours(mask_b.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    cont_img_b = cv2.drawContours(img.copy(), contours_b, -1, (255,50,50), 2) #img, contours, contour num (-1 for all), color, thickness
    
    lower_bound = np.array([160,200,40])
    upper_bound = np.array([180,255,255])
    mask_r1 = cv2.inRange(hsv, lower_bound, upper_bound)
    lower_bound = np.array([0,200,40])
    upper_bound = np.array([15,255,255])
    mask_r2 = cv2.inRange(hsv, lower_bound, upper_bound)
    mask_r = mask_r1 + mask_r2
    kernel = np.ones((7,7), np.uint8)
    mask_r = cv2.morphologyEx(mask_r, cv2.MORPH_CLOSE, kernel) #remove black noise from white
    mask_r = cv2.morphologyEx(mask_r, cv2.MORPH_OPEN, kernel) #remove white noise from black
    mask_img_r = cv2.bitwise_and(img, img, mask=mask_r)
    contours_r, hierarchy_r = cv2.findContours(mask_r.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    cont_img_r = cv2.drawContours(img.copy(), contours_r, -1, (50,50,255), 2)
    
    #fimg = cont_img_r
    #fimg = cont_img_b
    fimg = cv2.drawContours(cont_img_b.copy(), contours_r, -1, (50,50,255), 2)

    cv2.imshow('test', fimg)
    
    
    
    k = cv2.waitKey(1)
    if k != -1:
        break
#cv2.imwrite('/home/monkey/Desktop/cornhole/test2_img.jpg', image)
cam.release()
cv2.destroyAllWindows()