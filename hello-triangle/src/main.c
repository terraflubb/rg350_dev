#include <stdio.h>
#include <stdint.h>

#include "shell.h"

static const char *VERT_SHADER =
    "#version 100\n\n"
    "precision highp float;\n"
    "attribute vec2 vPos;\n"
    "uniform float u_time, u_sin, u_cos;\n"
    "void main() {\n"
        "vec2 vp,pp;\n"
        "vp = vPos;\n"
        "pp.x =  u_cos * vp.x + u_sin * vp.y;\n" // Apparently mat2 isn't supported in version 100
        "pp.y = -u_sin * vp.x + u_cos * vp.y;\n"
        "pp.x *= 240. / 320.;\n"
        "gl_Position = vec4(pp, 0.0, 1.0);\n"
    "}\n";

static const char *FRAG_SHADER =
    "#version 100\n\n"
    "precision highp float;\n"
    "uniform float u_time, u_sin, u_cos;\n"
    "void main() {\n"
        "vec2 p = gl_FragCoord.xy / 200.;\n"
        "gl_FragColor = vec4(0, .5+.5*u_sin, .5+.5*u_cos, 1);\n"
    "}\n";

static const float vertices[] = { -0.5f, -0.5f, 0.0f, 0.5f, 0.5f, -0.5f };
static const uint16_t indices[] = { 0, 1, 2 };

GLuint vertexShader;
GLuint fragmentShader;
GLuint programObject;

int main( void )
{
    GLint compiled;
    ShellContext *ctx = shell_new();

    vertexShader = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertexShader, 1, &VERT_SHADER, NULL );
    glCompileShader( vertexShader );

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiled);
    if(!compiled)
    {
        GLint infoLen = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLen);

        if(infoLen > 1)
        {
            char* infoLog = (char*)malloc(sizeof(char) * infoLen);
            glGetShaderInfoLog(vertexShader, infoLen, NULL, infoLog);
            printf("Error compiling vert:\n%s\n", infoLog);
        }
        return 0;
    }

    fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragmentShader, 1, &FRAG_SHADER, NULL );
    glCompileShader( fragmentShader );

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled);
    if(!compiled)
    {
        GLint infoLen = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLen);

        if(infoLen > 1)
        {
            char* infoLog = (char*)malloc(sizeof(char) * infoLen);
            glGetShaderInfoLog(fragmentShader, infoLen, NULL, infoLog);
            printf("Error compiling frag:\n%s\n", infoLog);
        }
        return 0;
    }

    programObject = glCreateProgram();
    glAttachShader( programObject, vertexShader );
    glAttachShader( programObject, fragmentShader );
    glBindAttribLocation( programObject, 0, "vPos" );
    glLinkProgram( programObject );

    GLint linked;
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
    if(!linked)
    {
        GLint infoLen = 0;
        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);
        if(infoLen > 1)
        {
            char* infoLog = (char*)malloc(sizeof(char) * infoLen);
            glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
            printf("Error linking program:\n%s\n", infoLog);
        }
        return 0;
    }

    float t = 0;

    do
    {
        t += 0.01f;
        glClearColor( .5f + .5*sinf(t), 0, 0, 1 );
        glClear( GL_COLOR_BUFFER_BIT );

        glUseProgram( programObject );
        glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, vertices );
        glEnableVertexAttribArray( 0 );
        glUniform1f( glGetUniformLocation( programObject, "u_time" ), t );
        glUniform1f( glGetUniformLocation( programObject, "u_sin" ), sinf(t) );
        glUniform1f( glGetUniformLocation( programObject, "u_cos" ), cosf(t) );
        glDrawElements( GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, indices );
    }
    while( shell_flip_frame_poll_events( ctx ) );

    shell_delete( ctx );

    return 0;
}