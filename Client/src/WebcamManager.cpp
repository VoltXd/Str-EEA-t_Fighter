#include "WebcamManager.hpp"

WebcamManager::WebcamManager() 
	: cap(0),
	  ellipseSize(37, 50),
	  ellipseColor(0, 255, 0),
	  handSquareSize(ellipseSize.width * 3 / 2),
	  imageCenter(cap.get(cv::CAP_PROP_FRAME_WIDTH) / 2, cap.get(cv::CAP_PROP_FRAME_HEIGHT) / 3),
	  topLeftLhandCorner(imageCenter.x - 2 * ellipseSize.width, imageCenter.y + ellipseSize.height * 3 / 2),
	  bottomRightLhandCorner(imageCenter.x - 2 * ellipseSize.width + handSquareSize, imageCenter.y + ellipseSize.height * 3 / 2 + handSquareSize),
	  topLeftRhandCorner(imageCenter.x + 2 * ellipseSize.width - handSquareSize, imageCenter.y + ellipseSize.height * 3 / 2),
	  bottomRightRhandCorner(imageCenter.x + 2 * ellipseSize.width, imageCenter.y + ellipseSize.height * 3 / 2 + handSquareSize)
{
	leftHandCenter = cv::Point((topLeftLhandCorner.x + handSquareSize / 2), (topLeftLhandCorner.y + handSquareSize / 2));
	rightHandCenter = cv::Point((topLeftRhandCorner.x + handSquareSize / 2), (topLeftRhandCorner.y + handSquareSize / 2));
	duringCalibration = false;
	endCalibrationTimer = false;
	rThreshold = 0., gThreshold = 0., bThreshold = 0.;

	headCenter = imageCenter;
}

void WebcamManager::startCalibration()
{
	start = std::chrono::high_resolution_clock::now();
	end = start;
	duringCalibration = true;
	endCalibrationTimer = false;
}

void WebcamManager::nextAction()
{
	// Get the next camera video frame and pre-processes it
	cap >> frame;
	
	// Check if the frame has been correctly received
	if (frame.empty())
		return;
	
	cv::flip(frame, frame, 1);

	// Color space to detect the gloves
	rgbFrame = RGBtorgb(frame);
	rgbThreshold(rgbFrame, rThreshold, gThreshold, bThreshold);
	cv::erode(rgbFrame, rgbFrame, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
	cv::dilate(rgbFrame, rgbFrame, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));

	cv::cvtColor(frame, gFrame, cv::COLOR_BGR2GRAY);
	cv::equalizeHist(gFrame, gFrame);

	if (duringCalibration && std::chrono::duration_cast<std::chrono::seconds>(end - start).count() < 5) 
	{
		end = std::chrono::high_resolution_clock::now();
		if (std::chrono::duration_cast<std::chrono::seconds>(end - start).count() >= 5)
			endCalibrationTimer = true;
	}

	if (duringCalibration && endCalibrationTimer) 
	{
		duringCalibration = false;
		// Saving the current frame for processing purpose
		screenshotCalibration = frame.clone();

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
}

void WebcamManager::SDL_renderCalibration()
{
	
}

void WebcamManager::CV_render()
{
	cv::imshow("CV_RENDER", frame);
}

int main()
{
	WebcamManager webcamManager;

	if (!webcamManager.isCameraOpened())
	{
		std::cout << "Impossible d'ouvrir la cam�ra." << std::endl;
		return -1;
	}

	while (true)
	{
		if (cv::waitKey(10) == 13) 
			webcamManager.startCalibration();
		
		webcamManager.nextAction();
		webcamManager.CV_render();

		// Waitkey for image rendering purpose
		if (cv::waitKey(1) == 27)
			break;
	}

	cv::destroyAllWindows();

	return 0;
}