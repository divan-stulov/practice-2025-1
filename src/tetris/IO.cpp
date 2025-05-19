#include "IO.h"
#include <SDL.h>
#include <cstdio>

static SDL_Window* mWindow = nullptr;
static SDL_Renderer* mRenderer = nullptr;

static SDL_Color mColors[COLOR_MAX] = {
    {0, 0, 0, 255},       // BLACK
    {255, 0, 0, 255},     // RED
    {0, 255, 0, 255},     // GREEN
    {0, 0, 255, 255},     // BLUE
    {0, 255, 255, 255},   // CYAN
    {255, 0, 255, 255},   // MAGENTA
    {255, 255, 0, 255},   // YELLOW
    {255, 255, 255, 255}  // WHITE
};

IO::IO() {
    InitGraph();
}

int IO::InitGraph()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    mWindow = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    if (!mWindow)
    {
        fprintf(stderr, "Couldn't create window: %s\n", SDL_GetError());
        SDL_Quit();
        return 2;
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!mRenderer)
    {
        fprintf(stderr, "Couldn't create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(mWindow);
        SDL_Quit();
        return 3;
    }

    SDL_SetRenderDrawColor(mRenderer, mColors[BLACK].r, mColors[BLACK].g, mColors[BLACK].b, mColors[BLACK].a);
    SDL_RenderClear(mRenderer);
    SDL_RenderPresent(mRenderer);

    return 0;
}

void IO::ClearScreen() {
    SDL_SetRenderDrawColor(mRenderer, mColors[BLACK].r, mColors[BLACK].g, mColors[BLACK].b, mColors[BLACK].a);
    SDL_RenderClear(mRenderer);
}

void IO::DrawRectangle(int pX1, int pY1, int pX2, int pY2, enum color pC) {
    SDL_Rect rect;
    rect.x = pX1;
    rect.y = pY1;
    rect.w = pX2 - pX1;
    rect.h = pY2 - pY1;
    SDL_SetRenderDrawColor(mRenderer, mColors[pC].r, mColors[pC].g, mColors[pC].b, mColors[pC].a);
    SDL_RenderFillRect(mRenderer, &rect);
}

int IO::GetScreenHeight() {
    int w, h;
    SDL_GetWindowSize(mWindow, &w, &h);
    return h;
}

void IO::UpdateScreen() {
    SDL_RenderPresent(mRenderer);
}

int IO::Pollkey() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN:
                return event.key.keysym.sym;
            case SDL_QUIT:
                exit(3);
        }
    }
    return -1;
}

int IO::Getkey() {
    SDL_Event event;
    while (true) {
        if (SDL_WaitEvent(&event)) {
            if (event.type == SDL_KEYDOWN)
                break;
            if (event.type == SDL_QUIT)
                exit(3);
        }
    }
    return event.key.keysym.sym;
}