#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "headCalibration.hpp"
#include "headCorrelationTracking.hpp"
#include "handTracking.hpp"
#include <chrono>

#define THRESHOLD_RATIO 0.1

int main()
{
	cv::Mat frame, croppedHead, screenshotCalibration;
	cv::Mat gCroppedHead, gScreenshot, gFrame;
	cv::Mat rgbFrame, rgbScreenshotCalibration;
	cv::Vec3f handRgbCalibration;
	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
	bool firstCalibration = true;
	bool duringCalibration = false;
	bool endCalibrationTimer = false;
	float rThreshold = 0., gThreshold = 0., bThreshold = 0.;
	// Open the defautl camera
	cv::VideoCapture cap(0);
	// Calibration ellipse's variables
	const cv::Point imageCenter = cv::Point(cap.get(cv::CAP_PROP_FRAME_WIDTH) / 2, cap.get(cv::CAP_PROP_FRAME_HEIGHT) / 3);
	const cv::Size ellipseSize = cv::Size(37, 50);
	const cv::Scalar ellipseColor = cv::Scalar(0, 255, 0);
	cv::Point headCenter = imageCenter;
	int handSquareSize = ellipseSize.width * 3 / 2;

	// Hand calibration data
	cv::Point topLeftLhandCorner(imageCenter.x - 2 * ellipseSize.width, imageCenter.y + ellipseSize.height * 3 / 2);
	cv::Point bottomRightLhandCorner(imageCenter.x - 2 * ellipseSize.width + handSquareSize, imageCenter.y + ellipseSize.height * 3 / 2 + handSquareSize);
	cv::Point leftHandCenter((topLeftLhandCorner.x + handSquareSize / 2), (topLeftLhandCorner.y + handSquareSize / 2));
	cv::Point topLeftRhandCorner(imageCenter.x + 2 * ellipseSize.width - handSquareSize, imageCenter.y + ellipseSize.height * 3 / 2);
	cv::Point bottomRightRhandCorner(imageCenter.x + 2 * ellipseSize.width, imageCenter.y + ellipseSize.height * 3 / 2 + handSquareSize);
	cv::Point rightHandCenter((topLeftRhandCorner.x + handSquareSize / 2), (topLeftRhandCorner.y + handSquareSize / 2));

	cv::Point oldLHCenter, oldRHCenter;
	if (!cap.isOpened())
	{
		std::cout << "Impossible d'ouvrir la cam�ra." << std::endl;
		return -1;
	}

	while (true)
	{
		// Get the next camera video frame and pre-processes it
		cap >> frame;
		cv::flip(frame, frame, 1);

		// Color space to detect the gloves
		rgbFrame = RGBtorgb(frame);
		rgbThreshold(rgbFrame, rThreshold, gThreshold, bThreshold);
		cv::erode(rgbFrame, rgbFrame, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
		cv::dilate(rgbFrame, rgbFrame, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));

		cv::cvtColor(frame, gFrame, cv::COLOR_BGR2GRAY);
		cv::equalizeHist(gFrame, gFrame);

		// Check if the frame has been correctly received
		if (frame.empty())
			break;

		// Calibration starts when pressing the ENTER key
		if (cv::waitKey(10) == 13) {
			start = std::chrono::high_resolution_clock::now();
			end = start;
			duringCalibration = true;
			endCalibrationTimer = false;
		}

		// 5 seconds are given to strike a pose
		if (duringCalibration && std::chrono::duration_cast<std::chrono::seconds>(end - start).count() < 5) {
			end = std::chrono::high_resolution_clock::now();
			// cap >> frame;
			// cv::flip(frame, frame, 1);
			// cv::ellipse(frame, imageCenter, ellipseSize, 0, 0, 360, ellipseColor, 5);
			// cv::rectangle(frame, topLeftLhandCorner, bottomRightLhandCorner, cv::Scalar(0, 255, 0), 5);
			// cv::rectangle(frame, topLeftRhandCorner, bottomRightRhandCorner, cv::Scalar(0, 255, 0), 5);
			// cv::circle(frame, headCenter, 5, cv::Scalar(0, 0, 255), -1);
			// cv::circle(frame, leftHandCenter, 5, cv::Scalar(0, 255, 0), -1);
			// cv::circle(frame, rightHandCenter, 5, cv::Scalar(0, 255, 0), -1);
			// cv::imshow("Flux vid�o de la cam�ra", frame);
			// cv::waitKey(1);
		}

		if (std::chrono::duration_cast<std::chrono::seconds>(end - start).count() >= 5) {
			endCalibrationTimer = true;
		}

		if (duringCalibration && endCalibrationTimer) {
			duringCalibration = false;
			// Saving the current frame for processing purpose
			screenshotCalibration = frame.clone();

			std::string validate_calibration;
			// The player is asked to validate the calibration only once at the beginning of the game
			if (firstCalibration) {
				std::cout << "�tes-vous satisfait de la calibration? (y/n) : ";
				std::cin >> validate_calibration;
			}
			else {
				validate_calibration = "y";
			}

			if (validate_calibration == "y") {
				firstCalibration = false;
				cvtColor(screenshotCalibration, gScreenshot, cv::COLOR_BGR2GRAY);
				rgbScreenshotCalibration = RGBtorgb(screenshotCalibration);
				handRgbCalibration = rgbScreenshotCalibration.at<cv::Vec3f>(leftHandCenter - cv::Point(6, 0));
				rThreshold = handRgbCalibration[2] * (1.0 + THRESHOLD_RATIO);
				gThreshold = handRgbCalibration[1] * (1.0 + THRESHOLD_RATIO);
				bThreshold = handRgbCalibration[0] * (1.0 + THRESHOLD_RATIO);
				cv::equalizeHist(gScreenshot, gScreenshot);
				// Saving the cropped head
				croppedHead = headCalibration(imageCenter, ellipseSize, frame);
				cv::Rect croppedRegion(imageCenter.x - ellipseSize.width, imageCenter.y - ellipseSize.height, 2 * ellipseSize.width, 2 * ellipseSize.height);
				// Re-sizing the cropped head as small as possible to make it a convolution kernel
				croppedHead = croppedHead(croppedRegion);
				cv::cvtColor(croppedHead, gCroppedHead, cv::COLOR_BGR2GRAY);
				cv::equalizeHist(gCroppedHead, gCroppedHead);
			}
		}

		// Tracking the head if the calibration has already been done
		if (!croppedHead.empty()) {
			headCenter = headTracking(gCroppedHead, gFrame, gScreenshot, headCenter, ellipseSize);
			oldLHCenter = leftHandCenter;
			oldRHCenter = rightHandCenter;

			leftHandCenter = handTracking(leftHandCenter, handSquareSize, rgbFrame);
			rightHandCenter = handTracking(rightHandCenter, handSquareSize, rgbFrame);
		}

		// Display the video frame with the callibration ellipse and the head center
		if (croppedHead.empty() || duringCalibration) {
			cv::ellipse(frame, imageCenter, ellipseSize, 0, 0, 360, ellipseColor, 5);
			cv::rectangle(frame, topLeftLhandCorner, bottomRightLhandCorner, cv::Scalar(0, 255, 0), 5);
			cv::rectangle(frame, topLeftRhandCorner, bottomRightRhandCorner, cv::Scalar(0, 255, 0), 5);
		}
		cv::circle(frame, headCenter, 5, cv::Scalar(0, 0, 255), -1);
		cv::circle(frame, leftHandCenter, 5, cv::Scalar(0, 255, 0), -1);
		cv::circle(frame, rightHandCenter, 5, cv::Scalar(0, 255, 0), -1);
		cv::imshow("Flux vid�o de la cam�ra", frame);

		// Waitkey for image rendering purpose
		if (cv::waitKey(1) == 27)
			break;
	}

	cv::destroyAllWindows();

	return 0;
}