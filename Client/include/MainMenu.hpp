#pragma once

class MainMenu
{
    public:
    MainMenu();
    ~MainMenu();

    int run();

    private:
    void input();
    void update();
    void render();

};