#ifndef RETRO
#include <SDL/SDL.h>
//#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>

#endif


int initVideo(int w,int h, int bpp);
void toggleFullscreen();
void videoFlipGL();
void videoFlip();
#ifndef RETRO
SDL_Surface *videoInitGL(int width,int height,int bpp);
void videoDrawGL(GLuint texture);
GLuint loadTexture(SDL_Surface *surface);
#endif
