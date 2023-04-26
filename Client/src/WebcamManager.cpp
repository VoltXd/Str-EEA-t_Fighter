#include "WebcamManager.hpp"
#include "Settings.hpp"

#include <opencv2/core/types_c.h>

WebcamManager::WebcamManager(SDL_Renderer* renderer) 
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
	firstCalibration = true;
	rThreshold = 0., gThreshold = 0., bThreshold = 0.;
	
	headCenter = imageCenter;

	// Texture creation (WARNING : REALLY UNSAFE)
	// TODO: Make it safer
	cap >> frame;
	
	m_renderer = renderer;
	frameTexture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_BGR24, SDL_TEXTUREACCESS_STREAMING, frame.cols, frame.rows);
}


WebcamManager::~WebcamManager()
{
	SDL_DestroyTexture(frameTexture);
}

bool WebcamManager::calibrate()
{
	bool hasCalibrationSucceed = false;

	while (firstCalibration)
	{
		// Input
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					firstCalibration = false;
					hasCalibrationSucceed = false;
					break;

				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE)
					{
						firstCalibration = false;
						hasCalibrationSucceed = false;
					}
					else if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER)
					{
						startCalibration();
						hasCalibrationSucceed = true;
					}
					break;
				
				default:
					break;
			}
		}

		// Update
		if (nextAction() == false)
			return hasCalibrationSucceed;

		// Render
		SDL_renderCalibration();
	}
	
	return hasCalibrationSucceed;
}

void WebcamManager::startCalibration()
{
	start = std::chrono::high_resolution_clock::now();
	end = start;
	duringCalibration = true;
	endCalibrationTimer = false;
}

bool WebcamManager::nextAction()
{
	// Get the next camera video frame and pre-processes it
	cap >> frame;
	
	// Check if the frame has been correctly received
	if (frame.empty())
	{
   	 	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERREUR CAMERA", "Camera indisponible", nullptr);
		return false;
	}
	
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
		firstCalibration = false;
		// Saving the current frame for processing purpose
		screenshotCalibration = frame.clone();

		cvtColor(screenshotCalibration, gScreenshot, cv::COLOR_BGR2GRAY);
		rgbScreenshotCalibration = RGBtorgb(screenshotCalibration);
		handRgbCalibration = rgbScreenshotCalibration.at<cv::Vec3f>(cv::Point(imageCenter.x - 2 * ellipseSize.width + handSquareSize / 2, imageCenter.y + ellipseSize.height * 3 / 2 + handSquareSize / 2) - cv::Point(6, 0));
		rThreshold = handRgbCalibration[2] * (1.0 - THRESHOLD_RATIO);
		gThreshold = handRgbCalibration[1] * (1.0 + THRESHOLD_RATIO);
		bThreshold = handRgbCalibration[0] * (1.0 + THRESHOLD_RATIO);
		leftHandCenter = cv::Point((topLeftLhandCorner.x + handSquareSize / 2), (topLeftLhandCorner.y + handSquareSize / 2));
		rightHandCenter = cv::Point((topLeftRhandCorner.x + handSquareSize / 2), (topLeftRhandCorner.y + handSquareSize / 2));
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

	return true;
}

void WebcamManager::SDL_renderCalibration()
{
	// Clear
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
	SDL_RenderClear(m_renderer);

	// Update texture
	SDL_UpdateTexture(frameTexture, nullptr, (void*)frame.data, frame.step1());
	SDL_RenderCopy(m_renderer, frameTexture, nullptr, nullptr);

	// Render
	SDL_RenderPresent(m_renderer);
}

void WebcamManager::SDL_renderCalibrationWhilePlaying()
{
	// Update texture
	SDL_UpdateTexture(frameTexture, nullptr, (void*)frame.data, frame.step1());
	SDL_RenderCopy(m_renderer, frameTexture, nullptr, nullptr);
}

void WebcamManager::CV_render()
{
	cv::imshow("CV_RENDER", frame);
}
