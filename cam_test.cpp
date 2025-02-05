#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
    
    cv::VideoCapture cam(0);
    if(!cam.isOpened()) {
        std::cerr << "Error: Couldn't open camera" << std::endl;
        return -1;
    }
    
    cv::Mat image;
    while (true) {
        cam >> image;
        if (image.empty()) {
            std::cerr << "Error: Captured empty frame" << std::endl;
            break;
        }
        
        cv::imshow("test", image);
        if (cv::waitKey(1) != -1) {
            break;
        }
    }
    
    cam.release();
    cv::destroyAllWindows();
    return 0;
}

/*
import cv2

cam = cv2.VideoCapture(0)

while True:
    ret, image = cam.read()
    cv2.imshow('test', image)
    k = cv2.waitKey(1)
    if k != -1:
        break
#cv2.imwrite('/home/monkey/Desktop/cornhole/test2_img.jpg', image)
cam.release()
cv2.destroyAllWindows()

*/
