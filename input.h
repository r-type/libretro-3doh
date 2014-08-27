#ifndef RETRO
#include <SDL/SDL.h>
#endif

typedef struct{
	int buttons;			/* buttons bitfield */

}inputState;

typedef struct{
	int buttonup;		
	int buttondown;		
	int buttonleft;		
	int buttonright;		
	int buttona;		
	int buttonb;		
	int buttonc;
	int buttonl;		
	int buttonr;		
	int buttonx;
	int buttonp;

}inputMapping;

#define INPUTBUTTONL     (1<<4)
#define INPUTBUTTONR     (1<<5)
#define INPUTBUTTONX     (1<<6)
#define INPUTBUTTONP     (1<<7)
#define INPUTBUTTONC     (1<<8)
#define INPUTBUTTONB     (1<<9)
#define INPUTBUTTONA     (1<<10)
#define INPUTBUTTONLEFT  (1<<11)
#define INPUTBUTTONRIGHT (1<<12)
#define INPUTBUTTONUP    (1<<13)
#define INPUTBUTTONDOWN  (1<<14)


unsigned char *inputRead();
int inputLength();
int inputEnum();
int inputInit();
#ifndef RETRO
SDL_Joystick *inputOpen(int joyid);
void inputPoll(SDL_Joystick *joy);
#endif
int inputQuit();
int inputFullscreen();
