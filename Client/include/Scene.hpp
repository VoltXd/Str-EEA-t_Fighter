#pragma once

enum class SceneId
{
    Intro,
    MainMenu,
    SinglePlayer,
    MultiPlayer,
    Quit
};

class Scene
{
    public:
    virtual SceneId run() = 0;
    
    private:
    virtual int initialise() = 0;

    // Game loop
    virtual void input() = 0;
    virtual void update() = 0;
    virtual void render() = 0;

    protected:
    SDL_Renderer* m_renderer;

    bool m_isRunning;
    SceneId m_nextScene;
};
