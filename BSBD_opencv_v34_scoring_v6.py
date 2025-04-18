import cv2
import numpy as np
import time
import RPi.GPIO as GPIO
import serial

''' VISUALIZE '''
show_cam = False #fimg: cam output - contours for board and bags - board reconstruction outline - ignored area greyed - board island decision
show_warped = False #pimg: cam output warped to perfect board - contours for bags - size and area text for bag islands
show_graph = False #img_ma2: area vs shape graph - bag amount text for each bag island
show_scoreboard = True #scoreboard: detected bags for each color - fps text

''' FPS SETUP '''
# region FPS SETUP
start = time.time()
fps = 0.0
end_but = time.time()
start_advance = time.time()
start_b_plus = time.time()
start_r_plus = time.time()
if(show_scoreboard): viz_dur = 1.0
# endregion

''' CAMERA SETUP '''
# region CAMERA SETUP
cam = cv2.VideoCapture(0)
allowed_area_mult_p = 0.1
#square to compare
img_sq = cv2.imread('/home/monkey/Desktop/cornhole/image_processing/images/square', cv2.IMREAD_GRAYSCALE)
assert img_sq is not None, "square image could not be read"
ret, thresh = cv2.threshold(img_sq, 127, 255, 0)
contours_sq, _ = cv2.findContours(thresh, 2, 1)
#boardshape to compare
img_bs = cv2.imread('/home/monkey/Desktop/cornhole/image_processing/images/boardshape_h.png', cv2.IMREAD_GRAYSCALE) #HORIZONTAL / VERTICAL
assert img_sq is not None, "boardshape image could not be read"
ret, thresh = cv2.threshold(img_bs, 127, 255, 0)
contours_bs, _ = cv2.findContours(thresh, 2, 1)
#match/area image
img_ma = cv2.imread('/home/monkey/Desktop/cornhole/image_processing/images/match_area10.png')
if(show_graph): img_ma2 = img_ma
ph, pw = 400, 800 #HORIZONTAL / VERTICAL
pb = 40
ppts = np.float32([[pb, pb], [pw-pb, pb], [pw-pb, ph-pb], [pb, ph-pb]])
if(show_warped): pimg = np.ones((ph, pw, 3), dtype=np.uint8)
ratio_p = 0.0294
contours_b = np.array([[[100,100]], [[200,100]], [[200,200]], [[100,200]]]) #create contours object to save between loops (for interval)
contours_r = np.array([[[100,100]], [[200,100]], [[200,200]], [[100,200]]]) #create contours object to save between loops (for interval)
contours_b_p = np.array([[[100,100]], [[200,100]], [[200,200]], [[100,200]]]) #create contours object to save between loops (for interval)
contours_r_p = np.array([[[100,100]], [[200,100]], [[200,200]], [[100,200]]]) #create contours object to save between loops (for interval)
contours_s = np.array([[[100,100]], [[200,100]], [[200,200]], [[100,200]]]) #create contours object to save between loops (for interval)
contours_s_simp = np.array([[[100,100]], [[200,100]], [[200,200]], [[100,200]]]) #create contours object to save between loops (for interval)
lb_b = np.array([85,160,40])
ub_b = np.array([135,255,255])
lb_r = np.array([0,150,65])
ub_r = np.array([30,255,255])
lb_s = np.array([30, 100, 120])
ub_s = np.array([90, 255,255])
bags_b = 0
bags_r = 0
area_s = 0
small_list = [0, 1, 1, 2, 2, 3, 3, 4]
# endregion

''' BUTTONS SETUP '''
# region BUTTONS SETUP
GPIO.setmode(GPIO.BCM)
GPIO.setup(23, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(24, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(25, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
time.sleep(0.5)

class ButtonTracker:
    def __init__(self):
        self.register = 0

    def update(self, button_pressed: bool):
        self.register = ((self.register << 1) | button_pressed) & 0b11111

    def check_single(self):
        last_three = self.register & 0b111
        last_four = self.register & 0b1111
        return last_three == 0b010 or last_four == 0b0110

    def check_hold(self):
        return self.register == 0b11111

track_r_plus = ButtonTracker()
track_r_min = ButtonTracker()
track_b_plus = ButtonTracker()
track_b_min = ButtonTracker()
track_reset = ButtonTracker()
# endregion

''' RFID SETUP '''
# region RFID
ser_RFID = serial.Serial ("/dev/ttyS0", 115200, timeout=0.03)    #Open port with baud rate / timeout 0.05
red_RFID = np.zeros(4, dtype=int)
blue_RFID = np.zeros(4, dtype=int)
# endregion

''' SCORING SETUP '''
# region SCORING SETUP
IN_GAME = False #SET TO FALSE AFTER TESTING
MODE = 1 #0 = game over, 1 = standard, 2 = no cancel, 3 = 21 exact
prev_MODE = 1
mode_pts_RFID = [0, 3, 3, 3]
mode_pts_bag = [0, 1, 1, 1]
#ALWAYS CURRENT FOR ROUND
red_score_cam = 0
blue_score_cam = 0
#ALWAYS CURRENT FOR ROUND
red_score_but = 0
blue_score_but = 0
#ALWAYS CURRENT FOR ROUND
red_score_RFID = 0
blue_score_RFID = 0
#ALWAYS CURRENT FOR ROUND
red_score_rnd = 0
blue_score_rnd = 0
#NEVER USES CURRENT ROUND
red_score_prev = 0
blue_score_prev = 0
#PREV + CURR
red_score_tot = 0
blue_score_tot = 0
diff_rb = 0
if(show_scoreboard):
    scoreboard = np.ones((400, 800, 3), dtype = np.uint8)
    scoreboard *= 150
# endregion

''' OUTPUT SETUP '''
# region OUTPUT SETUP
ser_out = serial.Serial ("/dev/ttyAMA3", 115200)
# endregion

while True:
    
    ''' POLL BUTTONS '''
    # region POLL BUTTONS
    start_but = time.time()
    if(start_but - end_but < 0.09):
        time.sleep(max(0, 0.09 - (start_but - end_but)))
    if(GPIO.input(23) == True and GPIO.input(24) == False and GPIO.input(25) == False):
        #1
        track_r_plus.update(0)
        track_r_min.update(0)
        track_b_plus.update(0)
        track_b_min.update(0)
        track_reset.update(0)
    elif(GPIO.input(23) == False and GPIO.input(24) == True and GPIO.input(25) == False):
        #2
        track_r_plus.update(0)
        track_r_min.update(0)
        track_b_plus.update(0)
        track_b_min.update(0)
        track_reset.update(1)
    elif(GPIO.input(23) == True and GPIO.input(24) == True and GPIO.input(25) == False):
        #3
        track_r_plus.update(0)
        track_r_min.update(0)
        track_b_plus.update(0)
        track_b_min.update(1)
        track_reset.update(0)
    elif(GPIO.input(23) == False and GPIO.input(24) == False and GPIO.input(25) == True):
        #4
        track_r_plus.update(0)
        track_r_min.update(0)
        track_b_plus.update(1)
        track_b_min.update(0)
        track_reset.update(0)
    elif(GPIO.input(23) == True and GPIO.input(24) == False and GPIO.input(25) == True):
        #5
        track_r_plus.update(0)
        track_r_min.update(1)
        track_b_plus.update(0)
        track_b_min.update(0)
        track_reset.update(0)
    elif(GPIO.input(23) == False and GPIO.input(24) == True and GPIO.input(25) == True):
        #6
        track_r_plus.update(1)
        track_r_min.update(0)
        track_b_plus.update(0)
        track_b_min.update(0)
        track_reset.update(0)
    elif(GPIO.input(23) == True and GPIO.input(24) == True and GPIO.input(25) == True):
        #7
        track_r_plus.update(0)
        track_r_min.update(0)
        track_b_plus.update(0)
        track_b_min.update(0)
        track_reset.update(0)
        end_advance = time.time()
        if(end_advance - start_advance > 2.5):
            start_advance = time.time()
            if(IN_GAME):
                red_score_prev = red_score_tot
                blue_score_prev = blue_score_tot
                if(MODE == 1): #standard
                    if(red_score_tot >= 21 or blue_score_tot >= 21):
                        red_score_but = 0
                        blue_score_but = 0
                        red_score_prev = 0
                        blue_score_prev = 0
                        prev_MODE = MODE
                        MODE = 0
                        IN_GAME = False
                elif (MODE == 2): #no cancel
                    if((red_score_tot >= 21 and red_score_tot > blue_score_tot + 1) or (blue_score_tot > 21 and blue_score_tot > red_score_tot + 1)):
                        red_score_but = 0
                        blue_score_but = 0
                        red_score_prev = 0
                        blue_score_prev = 0
                        PREV_MODE = MODE
                        MODE = 0
                        IN_GAME = False
                elif (MODE == 3): #21 exact
                    if(red_score_tot == 21 or blue_score_tot == 21):
                        red_score_but = 0
                        blue_score_but = 0
                        red_score_prev = 0
                        blue_score_prev = 0
                        PREV_MODE = MODE
                        MODE = 0
                        IN_GAME = False
                    elif(red_score_tot > 21):
                        red_score_prev = 13
                    elif(blue_score_tot > 21):
                        blue_score_prev = 13
                red_score_but = 0
                blue_score_but = 0
                red_RFID = np.zeros(4, dtype=int)
                blue_RFID = np.zeros(4, dtype=int)
            else:
                if(MODE == 0):
                    MODE = prev_MODE
                else:
                    red_score_tot = 0
                    blue_score_tot = 0
                    red_score_but = 0
                    blue_score_but = 0
                    red_score_prev = 0
                    blue_score_prev = 0
                    red_RFID = np.zeros(4, dtype=int)
                    blue_RFID = np.zeros(4, dtype=int)
                    IN_GAME = True
                    prev_MODE = MODE
    else:
        track_r_plus.update(0)
        track_r_min.update(0)
        track_b_plus.update(0)
        track_b_min.update(0)
        track_reset.update(0)
    if(IN_GAME):
        if(track_reset.check_hold()):
            IN_GAME = False
            red_score_tot = 0
            blue_score_tot = 0
            red_score_but = 0
            blue_score_but = 0
            red_score_prev = 0
            blue_score_prev = 0
            red_RFID = np.zeros(4, dtype=int)
            blue_RFID = np.zeros(4, dtype=int)
        if(track_b_min.check_single()):
            blue_score_but -= 1
        if(track_b_plus.check_single()):
            blue_score_but += 1
        elif(track_b_min.check_hold()):
            blue_score_but = 0
        elif(track_b_plus.check_hold() and (time.time() - start_b_plus) > 0.6):
            start_b_plus = time.time()
            blue_score_but += 3
        if(track_r_min.check_single()):
            red_score_but -= 1
        if(track_r_plus.check_single()):
            red_score_but += 1
        elif(track_r_min.check_hold()):
            red_score_but = 0
        elif(track_r_plus.check_hold() and (time.time() - start_r_plus) > 0.6):
            start_r_plus = time.time()
            red_score_but += 3
    else:
        if(track_reset.check_single()):
            if(MODE == 1 or MODE == 2):
                MODE += 1
            elif(MODE == 3):
                MODE = 1
            elif(MODE == 0):
                MODE = prev_MODE
    
    end_but = time.time()
    # endregion
    
    ''' POLL RFID '''
    # region RUN RFID
    start_RFID = time.time()
    received_data = ser_RFID.read()
    time.sleep(0.02) #time.sleep(0.03)
    data_left = ser_RFID.inWaiting()
    received_data += ser_RFID.read(data_left)
    decimal = int.from_bytes(received_data, byteorder='big')
    digit = int(np.floor((decimal % 100) / 10))
    ser_RFID.write(received_data)
    if(digit < 5 and digit > 0):
        red_RFID[digit - 1] = 1
    elif(digit > 4 and digit < 9):
        blue_RFID[digit - 5] = 1

    red_score_RFID = np.sum(red_RFID) * mode_pts_RFID[MODE]
    blue_score_RFID = np.sum(blue_RFID) * mode_pts_RFID[MODE]

    end_RFID = time.time()
    # endregion

    ''' RUN CAMERA '''
    # region RUN CAMERA
    # region REST CAMERA LOOP VARS
    start_cam = time.time()
    ret, img = cam.read()
    if(not ret):
        img = np.ones((480, 620, 3), dtype = np.uint8)
        cam = cv2.VideoCapture(0)
    img_o = img
    if(show_cam): fimg = img
    mask_s_simp = img.copy()
    mask_s_simp *= 0
    # endregion
    
    if(show_scoreboard): 
        scoreboard = np.ones((400, 800, 3), dtype = np.uint8)
        scoreboard *= 150
    
    if(show_graph): img_ma2 = img_ma #reset for match area graphic
    
    bags_b = 0 #reset for new calculation
    bags_r = 0 #reset for new calculation
    
    #DENOISE (SLOWS PERFORMANCE)
    img = cv2.bilateralFilter(img, 8, 110, 110) #9, 120, 120

    hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    
    #SURFACE
    mask_s = cv2.inRange(hsv, lb_s, ub_s)
    kernel = np.ones((6,6), np.uint8)
    mask_s = cv2.morphologyEx(mask_s, cv2.MORPH_CLOSE, kernel) #remove black noise from white
    mask_s = cv2.morphologyEx(mask_s, cv2.MORPH_OPEN, kernel) #remove white noise from black
    mask_img_s = cv2.bitwise_and(img, img, mask=mask_s)
    contours_s, _ = cv2.findContours(mask_s.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    if len(contours_s) >= 2:
        contours_s = sorted(contours_s, key=cv2.contourArea, reverse=True) #sort contours from largest to smallest
        contours_s_comb = []
        contours_s_comb_simp = []
        best_match = 100
        best_islands = 99
        for i in range(1, len(contours_s) + 1):
            contours_s_comb_c = [np.vstack(contours_s[:i])]
            simp = [cv2.approxPolyN(contours_s_comb_c[0], 4, -1)]
            contours_s_comb_simp_c = [np.vstack(simp).reshape(-1,1,2)]
            match_comb = cv2.matchShapes(contours_s_comb_simp_c[0], contours_bs[0], 2, 0)
            if(match_comb < best_match):
                best_match = match_comb
                contours_s_comb = contours_s_comb_c
                contours_s_comb_simp = contours_s_comb_simp_c
                best_islands = i
        contours_s = contours_s_comb
        contours_s_simp = contours_s_comb_simp
        if(show_cam): cv2.putText(fimg, "Best islands: " + f"{best_islands:.0f} " + f"({best_match:.4f})", (200,10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255,255,255), 1, cv2.LINE_AA)
        
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
    if(show_warped): pimg = cv2.warpPerspective(img.copy(), pM, (pw, ph))
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
            if(area_mob < 0.32):
                bags_c = 0
                bags_c_small += 1
            else:
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
                    bags_c = 1
            if(show_graph): img_ma2 = cv2.putText(img_ma2.copy(), f"{bags_c:.0f}", (ma_a, ma_m), cv2.FONT_HERSHEY_SIMPLEX, 0.2, (0,0,0), 1, cv2.LINE_AA)
            if(show_warped): 
                cv2.putText(pimg, f"{bags_c:.0f}", (x,y), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255,255,255), 1, cv2.LINE_AA)    
                cv2.putText(pimg, "s: " + f"{match:.2f}", (x-20,y+10), cv2.FONT_HERSHEY_SIMPLEX, 0.4, (255,255,255), 1, cv2.LINE_AA)
                cv2.putText(pimg, "a1: " + f"{area_rat:.3f}", (x-20,y+25), cv2.FONT_HERSHEY_SIMPLEX, 0.4, (255,255,255), 1, cv2.LINE_AA)
                cv2.putText(pimg, "a2: " + f"{area_mob:.3f}", (x-20,y+40), cv2.FONT_HERSHEY_SIMPLEX, 0.4, (255,255,255), 1, cv2.LINE_AA)
            bags_b += bags_c
    bags_b += small_list[min(bags_c_small, len(small_list) - 1)]
    if(show_warped): contours_b_p = contours_b_p_valid #only for outline visualization
    
    
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
            if(area_mob < 0.32):
                bags_c = 0
                bags_c_small += 1
            else:
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
                    bags_c = 1
            if(show_graph): img_ma2 = cv2.putText(img_ma2.copy(), f"{bags_c:.0f}", (ma_a, ma_m), cv2.FONT_HERSHEY_SIMPLEX, 0.2, (0,0,0), 1, cv2.LINE_AA)
            if(show_warped): 
                cv2.putText(pimg, f"{bags_c:.0f}", (x,y), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255,255,255), 1, cv2.LINE_AA)    
                cv2.putText(pimg, "s: " + f"{match:.2f}", (x-20,y+10), cv2.FONT_HERSHEY_SIMPLEX, 0.4, (255,255,255), 1, cv2.LINE_AA)
                cv2.putText(pimg, "a1: " + f"{area_rat:.3f}", (x-20,y+25), cv2.FONT_HERSHEY_SIMPLEX, 0.4, (255,255,255), 1, cv2.LINE_AA)
                cv2.putText(pimg, "a2: " + f"{area_mob:.3f}", (x-20,y+40), cv2.FONT_HERSHEY_SIMPLEX, 0.4, (255,255,255), 1, cv2.LINE_AA)
            bags_r += bags_c
    bags_r += small_list[min(bags_c_small, len(small_list) - 1)]
    if(show_warped): contours_r_p = contours_r_p_valid #only for outline visualization

    red_score_cam = max(min(bags_r, 4), 0) * mode_pts_bag[MODE]
    blue_score_cam = max(min(bags_b, 4), 0) * mode_pts_bag[MODE]

    end_cam = time.time()

    # endregion

    '''SCORE CALC'''
    red_score_cam = min(red_score_cam, 4-np.sum(red_RFID))
    blue_score_cam = min(blue_score_cam, 4-np.sum(blue_RFID))
    red_score_rnd = min(max(red_score_cam + red_score_but + red_score_RFID, 0), 99)
    blue_score_rnd = min(max(blue_score_cam + blue_score_but + blue_score_RFID, 0), 99)
    if(MODE == 1 or MODE == 3):
        diff_rb = red_score_rnd - blue_score_rnd
        red_score_tot = min(max(red_score_prev + max(diff_rb, 0), 0), 99)
        blue_score_tot = min(max(blue_score_prev + max(-diff_rb, 0), 0), 99)
    elif(MODE == 2):
        red_score_tot = min(max(red_score_prev + red_score_rnd, 0), 99)
        blue_score_tot = min(max(blue_score_prev + blue_score_rnd, 0), 99)

    ''' OUTPUT SETUP '''
    #X_score_rnd (2x)
    #X_score_prev (2x)
    #X_score_tot (2x) could be calculated based on rnd, prev and mode
    #IN_GAME (1x)
    #MODE (1x)
    message = "{:02d}".format(red_score_rnd) + "{:02d}".format(blue_score_rnd) + "{:02d}".format(red_score_prev) + "{:02d}".format(blue_score_prev) + "{:02d}".format(red_score_tot) + "{:02d}".format(blue_score_tot) + "{:01d}".format(IN_GAME) + "{:01d}".format(MODE)
    ser_out.write(message.encode('utf-8'))

    ''' VISUALIZATION '''
    # region VISUALIZATION

    '''scoreboard'''
    if(show_scoreboard): 
        start_viz = time.time()
        cv2.putText(scoreboard, 'fps: ' + f"{fps:.2f}", (0,12), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0,0,0), 1, cv2.LINE_AA)
        cv2.putText(scoreboard, 'fps_but: ' + f"{(1/(end_but - start_but)):.2f}", (0,25), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0,0,0), 1, cv2.LINE_AA)
        cv2.putText(scoreboard, 'fps_RFID: ' + f"{(1/(end_RFID - start_RFID)):.2f}", (0,38), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0,0,0), 1, cv2.LINE_AA)
        cv2.putText(scoreboard, 'fps_cam: ' + f"{(1/(end_cam - start_cam)):.2f}", (0,51), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0,0,0), 1, cv2.LINE_AA)
        cv2.putText(scoreboard, 'fps_viz: ' + f"{(1/(viz_dur)):.2f}", (0,64), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0,0,0), 1, cv2.LINE_AA)
        cv2.putText(scoreboard, str(red_score_rnd), (0,300), cv2.FONT_HERSHEY_DUPLEX, 10, (0, 0, 255), 3, cv2.LINE_AA)
        cv2.putText(scoreboard, str(red_score_prev), (200,150), cv2.FONT_HERSHEY_DUPLEX, 5, (0, 0, 255), 2, cv2.LINE_AA)
        cv2.putText(scoreboard, str(blue_score_rnd), (400,300), cv2.FONT_HERSHEY_DUPLEX, 10, (255, 0, 0), 3, cv2.LINE_AA)
        cv2.putText(scoreboard, str(blue_score_prev), (600,150), cv2.FONT_HERSHEY_DUPLEX, 5, (255, 0, 0), 2, cv2.LINE_AA)
        cv2.putText(scoreboard, str(red_score_tot), (200,350), cv2.FONT_HERSHEY_DUPLEX, 5, (0, 0, 255), 2, cv2.LINE_AA)
        cv2.putText(scoreboard, str(blue_score_tot), (600,350), cv2.FONT_HERSHEY_DUPLEX, 5, (255, 0, 0), 2, cv2.LINE_AA)
        cv2.putText(scoreboard, 'MODE: ' + f"{MODE:.0f}", (0,376), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0,0,0), 1, cv2.LINE_AA)
        cv2.putText(scoreboard, 'IN_GAME: ' + f"{IN_GAME:.0f}", (0,388), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0,0,0), 1, cv2.LINE_AA)
        cv2.imshow('scoreboard', scoreboard)
        end_viz = time.time()
        viz_dur = end_viz - start_viz

    # region TEMP VISUALIZATION
    '''draw contours'''
    if(show_cam): 
        cv2.drawContours(fimg, contours_b, -1, (255,50,50), 1)
        cv2.drawContours(fimg, contours_r, -1, (50,50,255), 1)
        cv2.drawContours(fimg, contours_s, -1, (255,255,255), 1)
        cv2.drawContours(fimg, contours_s_simp, -1, (50,255,50), 1)
    
    if(show_warped): 
        cv2.drawContours(pimg, contours_r_p, -1, (50, 50, 255), 2)
        cv2.drawContours(pimg, contours_b_p, -1, (255, 50, 50), 2)
        cv2.drawContours(pimg, contours_s_simp_p, -1, (50, 255, 50), 2)
            
    '''warped image graphic'''
    if(show_warped): cv2.imshow('warped image', pimg)
    
    '''match area square graphic'''
    if(show_graph): cv2.imshow('match_area2', img_ma2)
    
    '''add text and display image'''
    #MASK SURFACE DILATED
    if(show_cam): 
        mask_s_simp = cv2.bitwise_not(mask_s_simp)
        mask_s_simp = cv2.merge([mask_s_simp] * 3)
        fimg = cv2.addWeighted(fimg, 1.0, mask_s_simp, -0.3, 0) #darken board surface mask
        cv2.putText(fimg, str(bags_b), (0,96), cv2.FONT_HERSHEY_SIMPLEX, 2, (255,10,10), 3, cv2.LINE_AA)
        cv2.putText(fimg, str(bags_r), (0,160), cv2.FONT_HERSHEY_SIMPLEX, 2, (10,10,255), 3, cv2.LINE_AA)
        cv2.imshow('test', fimg)

    # endregion
    # endregion

    '''fps calculation and handling'''
    end = time.time()
    fps = 1/(end - start)
    start = time.time()
    
    '''close window'''
    k = cv2.waitKey(1)
    if k != -1:
        break

cam.release()
cv2.destroyAllWindows()