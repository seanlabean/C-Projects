#include "common.h"
#include "draw.h"

extern App app;

static void draw(void);
static void logic(void);
static void doScene(void);
static void drawScene(void);

static SDL_Texture *startTexture;
static SDL_Texture *sceneTexture;
static int sceneCounter;

void initScene(void)
{
    app.delegate.logic = logic;
    app.delegate.draw = draw;

    startTexture = loadTexture("gfx/girlScene.png");
}

static void draw(void)
{
    drawScene();
}

static void logic(void)
{
    sceneCounter = 0;
    doScene();
}

static void doScene(void)
{
    if (sceneCounter == 0)
    {
        sceneTexture = startTexture;
    }
}

static void drawScene(void)
{
    // SDL_Rect dest;
    // dest.x = 0;
    // dest.y = 0;
    // dest.w = SCREEN_WIDTH;
    // dest.h = SCREEN_WIDTH;

    SDL_RenderCopy(app.renderer, sceneTexture, NULL, NULL);
}