import cv2
import numpy as np
import time

cam = cv2.VideoCapture(0)
start = time.time()
fps = 0.0
count = 0 #used for tracking interval
interval = 1 #set how often bag detection is calculated (>= 1)
allowed_area_mult_p = 0.1

#square to compare
img_sq = cv2.imread('/home/monkey/Desktop/cornhole/image processing/images/square', cv2.IMREAD_GRAYSCALE)
assert img_sq is not None, "square image could not be read"
ret, thresh = cv2.threshold(img_sq, 127, 255, 0)
contours_sq, _ = cv2.findContours(thresh, 2, 1)

#boardshape to compare
img_bs = cv2.imread('/home/monkey/Desktop/cornhole/image processing/images/boardshape_h.png', cv2.IMREAD_GRAYSCALE) #HORIZONTAL / VERTICAL
assert img_sq is not None, "boardshape image could not be read"
ret, thresh = cv2.threshold(img_bs, 127, 255, 0)
contours_bs, _ = cv2.findContours(thresh, 2, 1)

#match/area image
img_ma = cv2.imread('/home/monkey/Desktop/cornhole/image processing/images/match_area9.png')
img_ma2 = img_ma

ph, pw = 400, 800 #HORIZONTAL / VERTICAL
pb = 40
ppts = np.float32([[pb, pb], [pw-pb, pb], [pw-pb, ph-pb], [pb, ph-pb]])
pimg = np.ones((ph, pw, 3), dtype=np.uint8)

ratio_p = 0.027

contours_b = np.array([[[100,100]], [[200,100]], [[200,200]], [[100,200]]]) #create contours object to save between loops (for interval)
contours_r = np.array([[[100,100]], [[200,100]], [[200,200]], [[100,200]]]) #create contours object to save between loops (for interval)
contours_b_p = np.array([[[100,100]], [[200,100]], [[200,200]], [[100,200]]]) #create contours object to save between loops (for interval)
contours_r_p = np.array([[[100,100]], [[200,100]], [[200,200]], [[100,200]]]) #create contours object to save between loops (for interval)
contours_s = np.array([[[100,100]], [[200,100]], [[200,200]], [[100,200]]]) #create contours object to save between loops (for interval)
contours_s_simp = np.array([[[100,100]], [[200,100]], [[200,200]], [[100,200]]]) #create contours object to save between loops (for interval)

lb_b = np.array([85,160,40])
ub_b = np.array([135,255,255])
lb_r = np.array([0,160,70])
ub_r = np.array([30,255,255])
lb_s = np.array([30, 100, 120])
ub_s = np.array([90, 255,255])

bags_b = 0
bags_r = 0
area_s = 0

small_list = [0, 1, 1, 2, 2, 3, 3, 4]

red_score = 0
blue_score = 0
scoreboard = np.ones((540, 960, 3), dtype = np.uint8)
scoreboard *= 255

while True:
    
    ret, img = cam.read()
    if(not ret):
        img = np.ones((480, 620, 3), dtype = np.uint8)
        cam = cv2.VideoCapture(0)
    img_o = img
    fimg = img
    mask_s_simp = img.copy()
    mask_s_simp *= 0
    
    if(count == 0):
        
        scoreboard = np.ones((540, 960, 3), dtype = np.uint8)
        scoreboard *= 150
        
        img_ma2 = img_ma #reset for match area graphic
        
        bags_b = 0 #reset for new calculation
        bags_r = 0 #reset for new calculation
        
        #DENOISE (SLOWS PERFORMANCE)
        img = cv2.bilateralFilter(img, 9, 120, 120)

        hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
        
        #SURFACE
        mask_s = cv2.inRange(hsv, lb_s, ub_s)
        kernel = np.ones((6,6), np.uint8)
        mask_s = cv2.morphologyEx(mask_s, cv2.MORPH_CLOSE, kernel) #remove black noise from white
        mask_s = cv2.morphologyEx(mask_s, cv2.MORPH_OPEN, kernel) #remove white noise from black
        mask_img_s = cv2.bitwise_and(img, img, mask=mask_s)
        contours_s, _ = cv2.findContours(mask_s.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        if len(contours_s) >= 2:
            contours_s = sorted(contours_s, key=cv2.contourArea, reverse=True)
            contours_s_comb = [np.vstack((contours_s[0], contours_s[1]))]
            
            simp = [cv2.approxPolyN(contours_s[0], 4, -1)]
            contours_s_simp = [np.vstack(simp).reshape(-1,1,2)]
            
            simp = [cv2.approxPolyN(contours_s_comb[0], 4, -1)]
            contours_s_comb_simp = [np.vstack(simp).reshape(-1,1,2)]
            
            match_single = cv2.matchShapes(contours_s_simp[0], contours_bs[0], 2, 0)
            match_comb = cv2.matchShapes(contours_s_comb_simp[0], contours_bs[0], 2, 0)
            cv2.putText(fimg, "Multiple board islands detected:", (200,10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255,255,255), 1, cv2.LINE_AA)    
            cv2.putText(fimg, "Single: " + f"{match_single:.4f}", (200,22), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255,255,255), 1, cv2.LINE_AA)    
            cv2.putText(fimg, "Combined: " + f"{match_comb:.4f}", (200,34), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255,255,255), 1, cv2.LINE_AA)    
            if(match_single >= (match_comb * 1)):
                contours_s = [contours_s_comb[0]]
                contours_s_simp = contours_s_comb_simp
                
        elif len(contours_s) < 1:
            contours_s = [np.array([[[50,50]], [[50, 51]], [[52,50]], [[51,52]]], dtype=np.int32)]
            simp = [cv2.approxPolyN(contours_s[0], 4, -1)]
            contours_s_simp = [np.vstack(simp).reshape(-1,1,2)]
        else:
            simp = [cv2.approxPolyN(contours_s[0], 4, -1)]
            contours_s_simp = [np.vstack(simp).reshape(-1,1,2)]
        #REFORMAT POINT ORDER
        pts = contours_s_simp[0].squeeze()
        contours_s_simp_reo = np.zeros((4, 2), dtype="float32")
        s = pts.sum(axis=1)
        s = s.flatten()
        contours_s_simp_reo[0] = pts[np.argmin(s)]
        contours_s_simp_reo[2] = pts[np.argmax(s)]
        diff = np.diff(pts, axis=1)
        contours_s_simp_reo[1] = pts[np.argmin(diff)]
        contours_s_simp_reo[3] = pts[np.argmax(diff)]
        contours_s_simp_reo = np.float32([contours_s_simp_reo])
        #WARP
        pM = cv2.getPerspectiveTransform(contours_s_simp_reo, ppts)
        pimg = cv2.warpPerspective(img.copy(), pM, (pw, ph))
        contour_pts = contours_s_simp[0].astype(np.float32)
        contour_pts_reshaped = contour_pts.reshape(-1, 1, 2)
        warped_contour_pts = cv2.perspectiveTransform(contour_pts_reshaped, pM)
        contours_s_simp_p = [warped_contour_pts.astype(np.int32)]
        #SET VARS
        area_s = cv2.contourArea(contours_s_simp[0])
        area_s_p = cv2.contourArea(contours_s_simp_p[0])
        if(abs(area_s_p) < 0.001): area_s_p = 0.001 #avoid divide by zero error
        #DILATED MASK
        mask_s_simp = cv2.drawContours(mask_s_simp, contours_s_simp, 0, (255,255,255), thickness=-10000)
        x = np.sqrt(area_s / 8) * 0.075
        down = int(round(5 * x))
        up = int(round(1 * x))
        side = int(round(2 * x))
        mask_s_simp = cv2.bitwise_or(mask_s_simp, np.roll(mask_s_simp, shift=(-up, 0), axis=(0,1)))
        mask_s_simp = cv2.bitwise_or(mask_s_simp, np.roll(mask_s_simp, shift=(down, 0), axis=(0,1)))
        mask_s_simp = cv2.bitwise_or(mask_s_simp, np.roll(mask_s_simp, shift=(0, side), axis=(0,1)))
        mask_s_simp = cv2.bitwise_or(mask_s_simp, np.roll(mask_s_simp, shift=(0, -side), axis=(0,1)))
        mask_s_simp = cv2.cvtColor(mask_s_simp, cv2.COLOR_BGR2GRAY) #necessary for it to work idk bro

        
        #BLUE BAGS
        contours_b_valid = []
        mask_b = cv2.inRange(hsv, lb_b, ub_b)
        kernel = np.ones((3,3), np.uint8)
        mask_b = cv2.morphologyEx(mask_b, cv2.MORPH_CLOSE, kernel) #remove black noise from white
        mask_b = cv2.morphologyEx(mask_b, cv2.MORPH_OPEN, kernel) #remove white noise from black
        mask_b = cv2.bitwise_and(mask_b, mask_b, mask=mask_s_simp)
        contours_b, _ = cv2.findContours(mask_b.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        bags_c_small = 0
        contours_b_p = []      
        for contour in contours_b:
            contour_pts = contour.astype(np.float32)
            contour_pts_reshaped = contour_pts.reshape(-1, 1, 2)
            warped_contour_pts = cv2.perspectiveTransform(contour_pts_reshaped, pM)
            contours_b_p.append(warped_contour_pts.astype(np.int32))
        contours_b_p_valid = []
        for cont in contours_b_p:
            M = cv2.moments(cont)
            if(abs(M['m00']) < 0.001): M['m00'] = 100 #avoid divide by zero error
            x = int(M['m10']/M['m00'])
            y = int(M['m01']/M['m00'])
            dist = cv2.pointPolygonTest(contours_s_simp_p[0], (x,y), True)
            area = cv2.contourArea(cont)
            allowedDist = allowed_area_mult_p * np.sqrt(area)
            #
            if dist >= -allowedDist:
                contours_b_p_valid.append(cont)
                match = cv2.matchShapes(cont, contours_sq[0], 1, 0)
                area_b = cv2.contourArea(cont)
                area_rat = area_b/area_s_p
                area_mob = area_rat/ratio_p
                ma_m = int(99 - max(0, min(99, match * 62.5))) #remap match from (0,1.6) to (100,0)
                ma_a = int(max(0, min(99, area_mob * 25)))#remap area_mob from (0,4) to (0,100)
                ma_bgr = img_ma[ma_m, ma_a]
                if(ma_bgr[0] > 200 and ma_bgr[1] > 200 and ma_bgr[2] > 200):
                    bags_c = 4
                elif(ma_bgr[2] > 200):
                    bags_c = 3
                elif(ma_bgr[0] > 200):
                    bags_c = 2
                else:
                    if(area_mob < 0.32):
                        bags_c = 0
                        bags_c_small += 1
                    else:
                        bags_c = 1
                img_ma2 = cv2.putText(img_ma2.copy(), f"{bags_c:.0f}", (ma_a, ma_m), cv2.FONT_HERSHEY_SIMPLEX, 0.2, (0,0,0), 1, cv2.LINE_AA)
                cv2.putText(pimg, f"{bags_c:.0f}", (x,y), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255,255,255), 1, cv2.LINE_AA)    
                cv2.putText(pimg, "s: " + f"{match:.2f}", (x-20,y+10), cv2.FONT_HERSHEY_SIMPLEX, 0.4, (255,255,255), 1, cv2.LINE_AA)
                cv2.putText(pimg, "a1: " + f"{area_rat:.3f}", (x-20,y+25), cv2.FONT_HERSHEY_SIMPLEX, 0.4, (255,255,255), 1, cv2.LINE_AA)
                cv2.putText(pimg, "a2: " + f"{area_mob:.3f}", (x-20,y+40), cv2.FONT_HERSHEY_SIMPLEX, 0.4, (255,255,255), 1, cv2.LINE_AA)
                bags_b += bags_c
        bags_b += small_list[min(bags_c_small, len(small_list) - 1)]
        contours_b_p = contours_b_p_valid #only for outline visualization
        
        
        #RED BAGS
        contours_r_valid = []
        mask_r = cv2.inRange(hsv, lb_r, ub_r)
        kernel = np.ones((3,3), np.uint8)
        mask_r = cv2.morphologyEx(mask_r, cv2.MORPH_CLOSE, kernel) #remove black noise from white
        mask_r = cv2.morphologyEx(mask_r, cv2.MORPH_OPEN, kernel) #remove white noise from black
        mask_r = cv2.bitwise_and(mask_r, mask_r, mask=mask_s_simp)
        contours_r, _ = cv2.findContours(mask_r.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        bags_c_small = 0
        contours_r_p = []      
        for contour in contours_r:
            contour_pts = contour.astype(np.float32)
            contour_pts_reshaped = contour_pts.reshape(-1, 1, 2)
            warped_contour_pts = cv2.perspectiveTransform(contour_pts_reshaped, pM)
            contours_r_p.append(warped_contour_pts.astype(np.int32))
        contours_r_p_valid = []
        for cont in contours_r_p:
            M = cv2.moments(cont)
            if(abs(M['m00']) < 0.001): M['m00'] = 100 #avoid divide by zero error
            x = int(M['m10']/M['m00'])
            y = int(M['m01']/M['m00'])
            dist = cv2.pointPolygonTest(contours_s_simp_p[0], (x,y), True)
            area = cv2.contourArea(cont)
            allowedDist = allowed_area_mult_p * np.sqrt(area)
            if dist >= -allowedDist:
                contours_r_p_valid.append(cont)
                match = cv2.matchShapes(cont, contours_sq[0], 1, 0)
                area_r = cv2.contourArea(cont)
                area_rat = area_r/area_s_p
                area_mob = area_rat/ratio_p
                ma_m = int(99 - max(0, min(99, match * 62.5))) #remap match from (0,1.6) to (100,0)
                ma_a = int(max(0, min(99, area_mob * 25)))#remap area_mob from (0,4) to (0,100)
                ma_bgr = img_ma[ma_m, ma_a]
                if(ma_bgr[0] > 200 and ma_bgr[1] > 200 and ma_bgr[2] > 200):
                    bags_c = 4
                elif(ma_bgr[2] > 200):
                    bags_c = 3
                elif(ma_bgr[0] > 200):
                    bags_c = 2
                else:
                    if(area_mob < 0.32):
                        bags_c = 0
                        bags_c_small += 1
                    else:
                        bags_c = 1
                img_ma2 = cv2.putText(img_ma2.copy(), f"{bags_c:.0f}", (ma_a, ma_m), cv2.FONT_HERSHEY_SIMPLEX, 0.2, (0,0,0), 1, cv2.LINE_AA)
                cv2.putText(pimg, f"{bags_c:.0f}", (x,y), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255,255,255), 1, cv2.LINE_AA)    
                cv2.putText(pimg, "s: " + f"{match:.2f}", (x-20,y+10), cv2.FONT_HERSHEY_SIMPLEX, 0.4, (255,255,255), 1, cv2.LINE_AA)
                cv2.putText(pimg, "a1: " + f"{area_rat:.3f}", (x-20,y+25), cv2.FONT_HERSHEY_SIMPLEX, 0.4, (255,255,255), 1, cv2.LINE_AA)
                cv2.putText(pimg, "a2: " + f"{area_mob:.3f}", (x-20,y+40), cv2.FONT_HERSHEY_SIMPLEX, 0.4, (255,255,255), 1, cv2.LINE_AA)
                bags_r += bags_c
        bags_r += small_list[min(bags_c_small, len(small_list) - 1)]
        contours_r_p = contours_r_p_valid #only for outline visualization

    
    '''draw contours'''
    cv2.drawContours(fimg, contours_b, -1, (255,50,50), 1)
    cv2.drawContours(fimg, contours_r, -1, (50,50,255), 1)
    cv2.drawContours(fimg, contours_s, -1, (255,255,255), 1)
    cv2.drawContours(fimg, contours_s_simp, -1, (50,255,50), 1)
    
    cv2.drawContours(pimg, contours_r_p, -1, (50, 50, 255), 2)
    cv2.drawContours(pimg, contours_b_p, -1, (255, 50, 50), 2)
    cv2.drawContours(pimg, contours_s_simp_p, -1, (50, 255, 50), 2)


    '''score'''
    red_score = max(min(bags_r, 4), 0)
    blue_score = max(min(bags_b, 4), 0)
    cv2.putText(scoreboard, str(red_score), (80, 460), cv2.FONT_HERSHEY_DUPLEX, 20, (0, 0, 255), 3, cv2.LINE_AA)
    cv2.putText(scoreboard, str(blue_score), (560, 460), cv2.FONT_HERSHEY_DUPLEX, 20, (255, 0, 0), 3, cv2.LINE_AA)
    cv2.imshow('scoreboard', scoreboard)
    
    '''warped image graphic'''
    cv2.imshow('warped image', pimg)
    
    '''match area square graphic'''
    cv2.imshow('match_area2', img_ma2)

    '''fps calculation and handling'''
    end = time.time()
    fps = (1/(end - start) + fps) / 2
    fps_cont = fps / interval
    start = time.time()
    
    '''add text and display image'''
    #MASK SURFACE DILATED
    mask_s_simp = cv2.bitwise_not(mask_s_simp)
    mask_s_simp = cv2.merge([mask_s_simp] * 3)
    fimg = cv2.addWeighted(fimg, 1.0, mask_s_simp, -0.3, 0) #darken board surface mask
    cv2.putText(fimg, 'fps: ' + f"{fps:.2f}", (0,12), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255,255,255), 1, cv2.LINE_AA)
    cv2.putText(fimg, 'fps_cont: ' + f"{fps_cont:.2f}", (0,24), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255,255,255), 1, cv2.LINE_AA)
    cv2.putText(fimg, str(bags_b), (0,96), cv2.FONT_HERSHEY_SIMPLEX, 2, (255,10,10), 3, cv2.LINE_AA)
    cv2.putText(fimg, str(bags_r), (0,160), cv2.FONT_HERSHEY_SIMPLEX, 2, (10,10,255), 3, cv2.LINE_AA)
    cv2.imshow('test', fimg)
        
    '''count handling'''
    count += 1
    if(count >= interval): count = 0
    
    '''close window'''
    k = cv2.waitKey(1)
    if k != -1:
        break

cam.release()
cv2.destroyAllWindows()