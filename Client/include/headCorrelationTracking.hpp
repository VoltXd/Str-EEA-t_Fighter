#ifndef HEAD_CORRELATION_TRACKING
#define HEAD_CORRELATION_TRACKING

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

cv::Point headTracking(cv::Mat& croppedHead, cv::Mat& currentImage, cv::Mat& screenshot, cv::Point headCenter, cv::Size ellipseSize);

#endif