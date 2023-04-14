#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "headCalibration.h"
#include "headCorrelationTracking.h"
#include "handTracking.h"



int main()
{
	cv::Mat frame, croppedHead, screenshotCalibration;
	cv::Mat gCroppedHead, gScreenshot, gFrame;
	cv::Mat rgbFrame, rgbFrameTest;
	// Open the defautl camera
	cv::VideoCapture cap(1);
	// Calibration ellipse's variables
	cv::Point imageCenter = cv::Point(cap.get(cv::CAP_PROP_FRAME_WIDTH) / 2, cap.get(cv::CAP_PROP_FRAME_HEIGHT) / 4);
	cv::Size ellipseSize = cv::Size(37, 50);
	cv::Scalar ellipseColor = cv::Scalar(0, 255, 0);
	cv::Point headCenter = imageCenter;
	int handSquareSize = ellipseSize.width * 3 / 2;

	cv::Point topLeftLhandCorner(imageCenter.x -  2 * ellipseSize.width, imageCenter.y + ellipseSize.height * 3 / 2);
	cv::Point bottomRightLhandCorner(imageCenter.x - 2 *ellipseSize.width + handSquareSize, imageCenter.y + ellipseSize.height * 3 / 2 + handSquareSize);
	cv::Point leftHandCenter((topLeftLhandCorner.x + handSquareSize / 2), (topLeftLhandCorner.y + handSquareSize / 2));
	cv::Point topLeftRhandCorner(imageCenter.x + 2 * ellipseSize.width - handSquareSize, imageCenter.y + ellipseSize.height * 3 / 2);
	cv::Point bottomRightRhandCorner(imageCenter.x + 2 * ellipseSize.width, imageCenter.y + ellipseSize.height * 3 / 2 + handSquareSize);
	cv::Point rightHandCenter((topLeftRhandCorner.x + handSquareSize / 2), (topLeftRhandCorner.y + handSquareSize / 2));

	cv::Point oldLHCenter, oldRHCenter;
	if (!cap.isOpened())
	{
		std::cout << "Impossible d'ouvrir la caméra." << std::endl;
		return -1;
	}

	while (true)
	{
		// Get the next camera video frame and pre-processes it
		cap >> frame;
		cv::flip(frame, frame, 1);
		
		rgbFrame = RGBtorgb(frame);
		rgbThreshold(rgbFrame, 0.7, 0.2, 0.2);
		cv::erode(rgbFrame, rgbFrame, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
		cv::dilate(rgbFrame, rgbFrame, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));

		cv::cvtColor(frame, gFrame, cv::COLOR_BGR2GRAY);
		cv::equalizeHist(gFrame, gFrame);

		rgbFrameTest = cv::Mat::zeros(rgbFrame.size(), CV_32FC3);
		cv::rectangle(rgbFrameTest, cv::Point(topLeftLhandCorner.x - handSquareSize / 2, topLeftLhandCorner.y),
					cv::Point(bottomRightLhandCorner.x - handSquareSize / 2, bottomRightLhandCorner.y), cv::Scalar(0, 0, 255), -1);
		cv::rectangle(rgbFrameTest, cv::Point(topLeftRhandCorner.x - handSquareSize / 2, topLeftRhandCorner.y),
			cv::Point(bottomRightRhandCorner.x - handSquareSize / 2, bottomRightRhandCorner.y), cv::Scalar(0, 0, 255), -1);

		// Check if the frame has been correctly received
		if (frame.empty())
			break;

		// Calibration starts with pressing the ENTER key
		if (cv::waitKey(10) == 13) {
			// Saving the current frame for processing purpose
			screenshotCalibration = frame.clone();
			cvtColor(screenshotCalibration, gScreenshot, cv::COLOR_BGR2GRAY);
			cv::equalizeHist(gScreenshot, gScreenshot);
			// Saving the cropped head
			croppedHead = headCalibration(imageCenter, ellipseSize, frame);
			cv::Rect croppedRegion(imageCenter.x - ellipseSize.width, imageCenter.y - ellipseSize.height, 2 * ellipseSize.width, 2 * ellipseSize.height);
			// Re-sizing the cropped head as small as possible to make it a convolution kernel
			croppedHead = croppedHead(croppedRegion);
			cv::cvtColor(croppedHead, gCroppedHead, cv::COLOR_BGR2GRAY);
			cv::equalizeHist(gCroppedHead, gCroppedHead);
		}

		// Tracking the head if the calibration has already been done
		if (!croppedHead.empty()) {
			headCenter = headTracking(gCroppedHead, gFrame, gScreenshot, headCenter, ellipseSize);
		}

		oldLHCenter = leftHandCenter;
		oldRHCenter = rightHandCenter;

		leftHandCenter = handTracking(leftHandCenter, handSquareSize, rgbFrame);
		rightHandCenter = handTracking(rightHandCenter, handSquareSize, rgbFrame);

		if (leftHandCenter != oldLHCenter) {
			std::cout << "New LH center : " << leftHandCenter.x << ", " << leftHandCenter.y << std::endl;
		}

		if (rightHandCenter != oldRHCenter) {
			std::cout << "New RH center : " << rightHandCenter.x << ", " << rightHandCenter.y << std::endl;
		}

		// Display the video frame with the callibration ellipse and the head center
		if (croppedHead.empty())
			cv::ellipse(frame, imageCenter, ellipseSize, 0, 0, 360, ellipseColor, 5);
			cv::rectangle(frame, topLeftLhandCorner, bottomRightLhandCorner, cv::Scalar(0, 255, 0), 5);
			cv::rectangle(frame, topLeftRhandCorner, bottomRightRhandCorner, cv::Scalar(0, 255, 0), 5);
		cv::circle(frame, headCenter, 5, cv::Scalar(0, 0, 255), -1);
		cv::circle(frame, leftHandCenter, 5, cv::Scalar(0, 255, 0), -1);
		cv::circle(frame, rightHandCenter, 5, cv::Scalar(0, 255, 0), -1);
		cv::imshow("Flux vidéo de la caméra", frame);
		//cv::imshow("Caméra couleur", frame);

		// Waitkey for image rendering purpose
		if (cv::waitKey(1) == 27)
			break;
	}

	cv::destroyAllWindows();

	return 0;
}