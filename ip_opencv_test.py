import cv2
import numpy as np

img = cv2.imread('/home/monkey/Desktop/cornhole/test_img.jpg')

hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV) #convert colorspace

#BLUE
lower_bound = np.array([90,50,50])
upper_bound = np.array([150,255,255])
mask_b = cv2.inRange(hsv, lower_bound, upper_bound)
kernel = np.ones((7,7), np.uint8) #5x5 8bit int matric
mask_b = cv2.morphologyEx(mask_b, cv2.MORPH_CLOSE, kernel) #remove black noise from white
mask_b = cv2.morphologyEx(mask_b, cv2.MORPH_OPEN, kernel) #remove white noise from black
mask_img_b = cv2.bitwise_and(img, img, mask=mask_b)
contours_b, hierarchy_b = cv2.findContours(mask_b.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
cont_img_b = cv2.drawContours(img.copy(), contours_b, -1, (255,0,0), 2) #img, contours, contour num (-1 for all), color, thickness
num_isl_b = len(contours_b)

#RED
lower_bound = np.array([160,200,50])
upper_bound = np.array([180,255,255])
mask_r1 = cv2.inRange(hsv, lower_bound, upper_bound)
lower_bound = np.array([0,200,50])
upper_bound = np.array([15,255,255])
mask_r2 = cv2.inRange(hsv, lower_bound, upper_bound)
mask_r = mask_r1 + mask_r2
kernel = np.ones((7,7), np.uint8)
mask_r = cv2.morphologyEx(mask_r, cv2.MORPH_CLOSE, kernel) #remove black noise from white
kernel = np.ones((12,12), np.uint8)
mask_r = cv2.morphologyEx(mask_r, cv2.MORPH_OPEN, kernel) #remove white noise from black
mask_img_r = cv2.bitwise_and(img, img, mask=mask_r)
contours_r, hierarchy_r = cv2.findContours(mask_r.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
cont_img_r = cv2.drawContours(img.copy(), contours_r, -1, (0,0,255), 2) #img, contours, contour num (-1 for all), color, thickness
num_isl_r = len(contours_r)

fimg = cv2.drawContours(cont_img_b, contours_r, -1, (0,0,255), 2)

print("B Islands: ", num_isl_b)
print("R Islands: ", num_isl_r)

#cv2.imshow("Image", img)
#cv2.imshow("Mask B", mask_b)
#cv2.imshow("Mask Image B", mask_img_b)
#cv2.imshow("Cont Image B", cont_img_b)
#cv2.imshow("Mask R", mask_r)
#cv2.imshow("Mask Image R", mask_img_r)
#cv2.imshow("Cont Image R", cont_img_r)
cv2.imshow("Final Image", fimg)

cv2.waitKey(0)
cv2.destroyAllWindows()