#ifndef HAND_TRACKING
#define HAND_TRACKING

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

#define BORDER_MARGIN 2

cv::Point barycenter(cv::Mat& image);
cv::Point handTracking(cv::Point lastHandCenter, int squareSize, cv::Mat& image);
cv::Mat RGBtorgb(cv::Mat& image);
void rgbThreshold(cv::Mat& image, float rTh, float gTh, float bTh);


#endif