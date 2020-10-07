#pragma once

#include <stdbool.h>
#include "gfx.h"

typedef struct ShellContext ShellContext;

extern ShellContext *shell_new();
extern bool shell_flip_frame_poll_events( ShellContext *context );
extern SDL_GameController *shell_get_controller( ShellContext *context );
extern void shell_delete( ShellContext *context );