#include <stdio.h>

#include "shell.h"

int main( void )
{
    ShellContext *ctx = shell_new();

    float t = 0;

    do
    {
        if( t > 10 ) break;
        t += 0.1f;
        glClearColor( .5f+.5f*sinf(t), 0, .5f+.5f*cosf(t), 1 );
        glClear( GL_COLOR_BUFFER_BIT );
    }
    while( shell_flip_frame_poll_events( ctx ) );

    shell_delete( ctx );

    return 0;
}