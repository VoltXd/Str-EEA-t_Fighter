#pragma once

#include <ctime>

class Timer	{
private : 
	bool started = false;
	bool stopped = true;

	clock_t startNbrTicks = 0; // nombre de pulsations (absolu) lors du démarrage du timer 
	clock_t pauseNbrTicks = 0; // nombre de pulsations relatives écoulées au moment de la pause du timer
	clock_t relativeNbrTicks = 0; // nombre de pulsations relatives au démarrage du timer (et avec prise en compte des pauses)

	clock_t computeRelativeNbrTicks() { return (clock() - startNbrTicks + pauseNbrTicks); };

public : 
	Timer() {};
	void start() { 
		startNbrTicks = clock();
		stopped = false;
		started = true;
	};
	void pause() {
		pauseNbrTicks = computeRelativeNbrTicks();
		started = false;
	};
	void stop() {
		pauseNbrTicks = 0;
		started = false;
		stopped = true;
	};

	double getTime() {
		/*** retourne la valeur du timer en ms ***/
		if (started) relativeNbrTicks = computeRelativeNbrTicks();
		if (stopped) return 0.; 
		return (double)relativeNbrTicks / CLOCKS_PER_SEC * 1000;
	};
};

