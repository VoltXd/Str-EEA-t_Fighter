#pragma once

void SDL_ShowError(const char* title, const char* file, int line);

inline double limitToInterval(double value, double min, double max)
{
    if (value < min) return min;
    else if (value > max) return max;
    else return value;
}
