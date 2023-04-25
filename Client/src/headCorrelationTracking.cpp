#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "headCorrelationTracking.hpp"

cv::Point headTracking(cv::Mat& croppedHead, cv::Mat& currentImage, cv::Mat& screenshot, cv::Point headCenter, cv::Size ellipseSize) {

	// Normalization of the calibration image and the current image
	cv::Scalar meanCalibration, meanCurrentImage;
	cv::Scalar stdCalibration, stdCurrentImage;
	cv::meanStdDev(screenshot, meanCalibration, stdCalibration);
	cv::meanStdDev(currentImage, meanCurrentImage, stdCurrentImage);
	cv::Mat normalizedHead = croppedHead.clone();
	normalizedHead.convertTo(normalizedHead, CV_32F);
	subtract(normalizedHead, meanCalibration, normalizedHead);
	normalizedHead = normalizedHead / stdCalibration;
	cv::Mat normalizedImage = currentImage.clone();
	normalizedImage.convertTo(normalizedImage, CV_32F);
	cv::subtract(normalizedImage, meanCurrentImage, normalizedImage);
	normalizedImage = normalizedImage / stdCurrentImage;

	// Matrix of the cross-correlation between the current image and the calibration image
	cv::Mat correlation = cv::Mat::zeros(currentImage.size(), CV_32F);
	// Cropped image of the current image around the area of interest
	cv::Mat croppedImage = cv::Mat::zeros(currentImage.size(), CV_32F);
	// Mask that will crop the current image around the area of interest
	cv::Mat mask = cv::Mat::zeros(currentImage.size(), CV_8U);

	// Area of interest : rectangle whose center is the last known center of the head and is a bit larger than the calibration zone (one tenth of the
	//					  image dimension) to make sure to get the new position of the head
	cv::rectangle(mask, cv::Point(headCenter.x - ellipseSize.width - currentImage.rows / 10, headCenter.y - ellipseSize.height - currentImage.cols / 10),
						cv::Point(headCenter.x + ellipseSize.width + currentImage.rows / 10, headCenter.y + ellipseSize.height + currentImage.cols / 10),
						cv::Scalar(255, 255, 255), -1, cv::LINE_8, 0);
	normalizedImage.copyTo(croppedImage, mask);
	// Cross-correlation is made with filter2D because it is an already optimized function (important for real time image processing)
	cv::filter2D(croppedImage, correlation, -1, normalizedHead, cv::Point(-1, -1), 0, cv::BORDER_REPLICATE);

	// Looking for the location of the maximum of the cross-correlation
	double minVal, maxVal;
	cv::Point minLoc, maxLoc;
	cv::minMaxLoc(correlation, &minVal, &maxVal, &minLoc, &maxLoc);

	if (maxLoc.x >= headCenter.x - currentImage.rows / 10 && maxLoc.x < headCenter.x + currentImage.rows / 10
		&& maxLoc.y >= headCenter.y - currentImage.cols / 10 && maxLoc.y < headCenter.y + currentImage.cols / 10) {
		return maxLoc;
	}
	// If the maximum is found outside of the area of interest, the head center isn't changed
	else {
		return headCenter;
	}
}