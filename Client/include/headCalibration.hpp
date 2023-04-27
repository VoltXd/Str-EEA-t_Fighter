#ifndef HEAD_CALIBRATION
#define HEAD_CALIBRATION

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

cv::Mat headCalibration(cv::Point centerEllipse, cv::Size sizeEllipse, cv::Mat& image);

#endif