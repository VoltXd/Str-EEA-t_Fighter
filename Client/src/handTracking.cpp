#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "handTracking.hpp"

cv::Mat RGBtorgb(cv::Mat& image) {
	cv::Mat rgbImage = cv::Mat::zeros(image.size(), CV_32FC3);
	cv::Vec3b value;
	float sum;
	for(int i = 0; i < image.rows; i++)
		for (int j = 0; j < image.cols; j++) {
			value = image.at<cv::Vec3b>(i, j);
			sum = value[0] + value[1] + value[2];
			rgbImage.at<cv::Vec3f>(i, j) = cv::Vec3f(value[0] / sum, value[1] / sum, value[2] / sum);
		}
	return rgbImage;
}

void rgbThreshold(cv::Mat& image, float rTh, float gTh, float bTh) {
	cv::Vec3f value;
	for (int i = 0; i < image.rows; i++)
		for (int j = 0; j < image.cols; j++) {
			value = image.at<cv::Vec3f>(i, j);
			if (value[0] >= bTh || value[1] >= gTh || value[2] <= rTh) {
				image.at<cv::Vec3f>(i, j) = cv::Vec3f(0, 0, 0);
			}
		}
}

cv::Point barycenter(cv::Mat& image) {
	//Computes the barycenter of the current image
	int sum = 0;
	cv::Point tmp(0,0);
	for (int i = 0; i < image.rows; i++)
		for (int j = 0; j < image.cols; j++) {
			float value = image.at<cv::Vec3f>(i, j)[2];
			if (value > 0) {
				sum += 1;
				tmp += cv::Point(j, i);

			}
		}
	if (sum > 0) {
		tmp.x = (int)(tmp.x / sum);
		tmp.y = (int)(tmp.y / sum);
		return tmp;
	}
	return cv::Point(0,0);
}

cv::Point handTracking(cv::Point lastHandCenter, int squareSize, cv::Mat& image) {
	//Estimates the current center of the hand
	cv::Point topLeftPoint = lastHandCenter;
	cv::Mat researchArea;

	int croppedRegionWidth = 2 * std::min(squareSize / 2, std::min(topLeftPoint.x, std::abs(image.cols - topLeftPoint.x)));
	int croppedRegionHeight = 2 * std::min(squareSize / 2, std::min(topLeftPoint.y, std::abs(image.rows - topLeftPoint.y)));
	topLeftPoint.x -= croppedRegionWidth / 2;
	topLeftPoint.y -= croppedRegionHeight / 2;


	//Crops the originale image around the possible hand area
	cv::Rect croppedRegion(topLeftPoint.x, topLeftPoint.y, croppedRegionWidth, croppedRegionHeight);
	researchArea = image(croppedRegion);

	//Center is computed as a barycenter because the closest points to the camera should be the points that belongs to the hand
	cv::Point handCenter = barycenter(researchArea);

	if (handCenter == cv::Point(0, 0) || handCenter.x + topLeftPoint.x <= 0 || handCenter.x + topLeftPoint.x >= image.cols
		|| handCenter.y + topLeftPoint.y <= 0 || handCenter.y + topLeftPoint.y >= image.rows || handCenter.x <= BORDER_MARGIN || handCenter.y <= BORDER_MARGIN) {
		return lastHandCenter;
	}
	else {
		return handCenter + topLeftPoint;
	}
}