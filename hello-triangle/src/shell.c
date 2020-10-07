#include "shell.h"

#include <string.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

struct ShellContext
{
    SDL_Window *sdl_window;
    SDL_GLContext sdl_gl_context;
    SDL_GameController *controller;
    int window_width;
    int window_height;
};

ShellContext *shell_new()
{
    ShellContext *context = (ShellContext *)malloc( sizeof( ShellContext ) );

    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK ) < 0 ) goto err;

    context->window_width = SCREEN_WIDTH;
    context->window_height = SCREEN_HEIGHT;
    context->sdl_window = SDL_CreateWindow( "", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL );

    if( !context->sdl_window ) goto err;

    context->sdl_gl_context = SDL_GL_CreateContext( context->sdl_window );

    if( !context->sdl_gl_context ) goto err;

    context->controller = NULL;

    for( int i = 0; i < SDL_NumJoysticks(); ++i ) 
    {
        if( ! SDL_IsGameController( i ) ) continue;
        context->controller = SDL_GameControllerOpen( i );
        printf( "Using game controller: %s", SDL_GameControllerName( context->controller ) );
        if( context->controller ) break;
    }

    return context;

err:
    SDL_GL_DeleteContext( context->sdl_gl_context );
    SDL_DestroyWindow( context->sdl_window );
    SDL_Quit();
    free( context );

    return NULL;
}

extern SDL_GameController *shell_get_controller( ShellContext *context )
{
    return context->controller;
}

bool shell_flip_frame_poll_events( ShellContext *context )
{
    bool still_running = true;

    SDL_GL_SwapWindow( context->sdl_window );

    SDL_Event event;

    while( SDL_PollEvent( &event ) )
    {
        switch( event.type )
        {
        case SDL_QUIT:
            still_running = false;
            break;

        case SDL_KEYDOWN:
            if( event.key.keysym.sym == SDLK_ESCAPE )
                still_running = false;
        }
    }

    return still_running;
}

void shell_delete( ShellContext *context )
{
    if( !context ) return;

    if( context->sdl_window )
    {
        if( context->controller )
            SDL_GameControllerClose( context->controller );

        SDL_GL_DeleteContext( context->sdl_gl_context );
        SDL_DestroyWindow( context->sdl_window );
        SDL_Quit();

        free( context );
    }
}
