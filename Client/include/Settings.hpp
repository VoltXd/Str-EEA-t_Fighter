#pragma once 

typedef struct Settings
{
    int screenWidth;
    int screenHeight;

    bool isFullscreenOn;
} Settings;

extern Settings settings;