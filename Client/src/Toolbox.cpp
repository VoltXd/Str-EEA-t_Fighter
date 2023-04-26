#include "Toolbox.hpp"

#include <SDL.h>
#include <string>

void SDL_ShowError(const char* title, const char* file, int line)
{
    std::string message("In " + std::string(file) + ", at line " + std::to_string(line) + ":\n");
    message.append(SDL_GetError());
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, message.c_str(), nullptr);
}