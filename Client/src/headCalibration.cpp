#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "headCalibration.hpp"

cv::Mat headCalibration(cv::Point centerEllipse, cv::Size sizeEllipse, cv::Mat& image) {
	cv::Mat cropped = cv::Mat::zeros(image.size(), CV_8UC3);
	// The mask is made of the inside of the calibration ellipse
	ellipse(cropped, centerEllipse, sizeEllipse, 0, 0, 360, cv::Scalar(255, 255, 255), -1);
	image.copyTo(cropped, cropped);
	return cropped;
}