//
//  main.cpp
//  Budding
//
//  Created by Yan An on 15/12/15.
//  Copyright © 2015年 Yan An. All rights reserved.
//

#include "SDL.h"
#include "SDL_opengles2.h"

#include <time.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 480

static int screen_w, screen_h;
static float clear_color_r = 0, clear_color_g = 0, clear_color_b = 0;
static float milleseconds_per_frame = 16.0f;

int
main(int argc, char *argv[])
{
    
    SDL_Window *window;
    SDL_GLContext context;
    int done;
    Uint32 startFrame;
    Uint32 endFrame;
    Uint32 delay;
    SDL_Event event;
    
    /* initialize SDL */
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialize SDL\n");
        return 1;
    }
    
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_RETAINED_BACKING, 0);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    
    window =
    SDL_CreateWindow(NULL, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                     SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS);
    
    context = SDL_GL_CreateContext(window);
    
    if (!window) {
        printf("Could not initialize Window\n");
        return 1;
    }
    
    SDL_GL_MakeCurrent(window, context);
    SDL_GetWindowSize(window, &screen_w, &screen_h);
    glViewport(0, 0, screen_w, screen_h);
    
    /* Enter render loop, waiting for user to quit */
    done = 0;
    while (!done) {
        startFrame = SDL_GetTicks();
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = 1;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x > y) {
                    clear_color_g += 0.1;
                }
                else {
                    clear_color_b += 0.1;
                }
                clear_color_r += 0.1;
                
            }
        }
        
        glClearColor(clear_color_r, clear_color_g, clear_color_b, 0.0f);
        glClearDepthf(0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        SDL_GL_SwapWindow(window);
        
        endFrame = SDL_GetTicks();
        delay = milleseconds_per_frame - (endFrame - startFrame);
        if (delay > milleseconds_per_frame) {
            delay = milleseconds_per_frame;
        }
        if (delay > 0) {
            SDL_Delay(delay);
        }
        
    }
    
    /* shutdown SDL */
    SDL_Quit();
    
    return 0;
}
