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
    bool m_isRunning;
};
