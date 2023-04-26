#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <chrono>
#include <SDL.h>

#include "headCalibration.hpp"
#include "headCorrelationTracking.hpp"
#include "handTracking.hpp"

class WebcamManager
{
    public:
    WebcamManager(SDL_Renderer* renderer);
	~WebcamManager();

    inline bool isCameraOpened() const { return cap.isOpened(); };
	inline bool isCalibrating() const { return duringCalibration; };
 
    inline float getHeadX() const { return 100.0f * headCenter.x / frame.cols; };
    inline float getHeadY() const { return 100.0f * headCenter.y / frame.rows; };
	
    inline float getRightHandX() const { return 100.0f * rightHandCenter.x / frame.cols; };
    inline float getRightHandY() const { return 100.0f * rightHandCenter.y / frame.rows; };

    inline float getLeftHandX() const { return 100.0f * leftHandCenter.x / frame.cols; };
    inline float getLeftHandY() const { return 100.0f * leftHandCenter.y / frame.rows; };

    bool calibrate();
    bool nextAction();
    void SDL_renderCalibration();
    void SDL_renderCalibrationWhilePlaying();
    void CV_render();

    void startCalibration();
    private:

    SDL_Texture* frameTexture;
	SDL_Renderer* m_renderer;

    cv::Mat frame, croppedHead, screenshotCalibration;
	cv::Mat gCroppedHead, gScreenshot, gFrame;
	cv::Mat rgbFrame, rgbScreenshotCalibration;
	cv::Vec3f handRgbCalibration;
	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    bool firstCalibration;
	bool duringCalibration;
	bool endCalibrationTimer;

	float rThreshold, gThreshold, bThreshold;
    
	cv::VideoCapture cap;
	
	cv::Point headCenter;
	cv::Point rightHandCenter;
	cv::Point leftHandCenter;

	cv::Point oldLHCenter, oldRHCenter;
    
    // Calibration ellipse's constants
	const cv::Point imageCenter;
	const cv::Size ellipseSize;
	const cv::Scalar ellipseColor;
	const int handSquareSize;

    // Hand calibration data
	const cv::Point topLeftLhandCorner;
	const cv::Point bottomRightLhandCorner;
	const cv::Point topLeftRhandCorner;
	const cv::Point bottomRightRhandCorner;

    static constexpr double THRESHOLD_RATIO = 0.1;
};