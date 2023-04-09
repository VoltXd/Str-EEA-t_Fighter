#pragma once

#include <chrono>

using clk = std::chrono::high_resolution_clock;

template<class durationType>
class Timer {
private:
	bool started;

	clk::time_point startNbrTicks; // temps lors du dernier démarrage du timer 
	clk::duration relativeNbrTicks; // temps actuel

	clk::duration computeRelativeNbrTicks() {
		return (clk::now() - startNbrTicks);
	};

public:
	Timer<durationType>() { started = false; };

	void start() {
		startNbrTicks = clk::now();
		started = true;
	};
	void stop() {
		started = false;
	};

	unsigned long long getTime() {
		/*** retourne la valeur du timer ***/
		if (!started) {
			return 0;
		}
		relativeNbrTicks = computeRelativeNbrTicks();
		return std::chrono::duration_cast<durationType>(relativeNbrTicks).count();
	};
};


