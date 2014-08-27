#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/time.h>
#include <time.h>

#define SCREEN_W 320
#define SCREEN_H 240 
#define SDL_Delay(a) usleep((a)*1000)
#define write_log printf

long SDL_GetTicks(void)
{ // in MSec

    struct timeval tv;
    gettimeofday (&tv, NULL);
    return (tv.tv_sec*1000000 + tv.tv_usec)/1000;                                                                             
} 

#include "libretro.h" 

void osd_update();
void osd_init();
void osd_exit();
void osd_update_audio_stream();

//3doh extern
extern void videoFlip();
extern void soundRun();
extern int do_main(int argc, char *argv[]);
extern void doh3_exit();

#include "../input.h"
extern inputState internal_input_state;

//============================================================
//  RETRO
//============================================================
int rtwi=320,rthe=240;
static int topw=SCREEN_W;

unsigned int videoBuffer[SCREEN_W*SCREEN_H];

retro_video_refresh_t video_cb = NULL;
retro_environment_t environ_cb = NULL;

static retro_input_state_t input_state_cb = NULL;
static retro_audio_sample_batch_t audio_batch_cb = NULL;
static retro_audio_sample_t audio_cb;

void retro_set_audio_sample(retro_audio_sample_t cb){ audio_cb = cb; }
void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb) { audio_batch_cb = cb; }
static retro_input_poll_t input_poll_cb;

void retro_set_input_state(retro_input_state_t cb) { input_state_cb = cb; }
void retro_set_input_poll(retro_input_poll_t cb) { input_poll_cb = cb; }

void retro_set_video_refresh(retro_video_refresh_t cb) { video_cb = cb; }
void retro_set_environment(retro_environment_t cb) { environ_cb = cb; }

static struct retro_system_av_info g_av_info;

unsigned retro_api_version(void)
{
   return RETRO_API_VERSION;
}

void retro_get_system_info(struct retro_system_info *info)
{
   info->library_name = "retro 3doh";
   info->library_version = "0.1";
   info->valid_extensions = "iso";
   info->need_fullpath = true;   
   info->block_extract = true;
}

void retro_get_system_av_info(struct retro_system_av_info *info)
{
	*info = g_av_info;
}

void retro_init (void){ }

void retro_deinit(void){ }

void retro_reset (void){ }

void retro_run (void)
{
        osd_update();
	video_cb(videoBuffer,rtwi, rthe, topw << 2*1);
}

static void keyboard_cb(bool down, unsigned keycode, uint32_t character, uint16_t mod)
{
	unsigned char retrok=keycode;
#ifdef KEYDBG
  	 printf( "Down: %s, Code: %d, Char: %u, Mod: %u. ,(%d)\n",
  	       down ? "yes" : "no", keycode, character, mod,retrok);
#endif
	if (keycode>=320);
	else{

		if(down && keycode==RETROK_LSHIFT){
//			 printf( "LSHIFT:%d\n",SHIFTON);  
//			if(SHIFTON == 1)retrokbd_state[keycode]=1;
//			else retrokbd_state[keycode]=0;
//			SHIFTON=-SHIFTON;
		}
		else if(keycode!=RETROK_LSHIFT) {
//			if(down)retrokbd_state[keycode]=1;	
//			else if (!down)retrokbd_state[keycode]=0;
		}
	}
}

static const char* xargv[] = {
"3doh",
"-b",
"bios.bin",
"-i",
NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
};

static char MgamePath[1024];
static char MgameName[512];

static int parsePath(char* path, char* gamePath, char* gameName) {

	int i;
	int slashIndex = -1;
	int dotIndex = -1;
	int len = strlen(path);

	if (len < 1) {
		return 0;
	}

	for (i = len - 1; i >=0; i--) {
		if (path[i] == '/') {
			slashIndex = i;
			break;
		} else
		if (path[i] == '.') {
			dotIndex = i;
		}
	}
	if (slashIndex < 0 || dotIndex < 0) {
		return 0;
	}

	strncpy(gamePath, path, slashIndex);
	gamePath[slashIndex] = 0;
	strncpy(gameName, path + (slashIndex + 1), dotIndex - (slashIndex + 1));
	gameName[dotIndex - (slashIndex + 1)] = 0;

	write_log("gamePath=%s gameName=%s\n", gamePath, gameName);
	return 1;

}

bool retro_load_game(const struct retro_game_info *game)
{
	struct retro_keyboard_callback cb = { keyboard_cb };
    	environ_cb(RETRO_ENVIRONMENT_SET_KEYBOARD_CALLBACK, &cb);

	memset(videoBuffer,0,SCREEN_W*SCREEN_H*2*2);
	enum retro_pixel_format fmt =RETRO_PIXEL_FORMAT_XRGB8888;

    	if (!environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt))
    	{
    		fprintf(stderr, "RGB Format is not supported.\n");
    		return false;
    	}
	osd_init();

	//FIXME
	int result = 0;
	int paramCount;

	//split the path to directory and the name without the zip extension
	result = parsePath((char *)game->path, MgamePath, MgameName);
	if (result == 0) {
		write_log("parse path failed! path=%s\n", game->path);
		strcpy(MgameName,game->path );
		return -1;
	}

	//find how many parameters we have
	for (paramCount = 0; xargv[paramCount] != NULL; paramCount++);
	strcat(MgameName,".iso\0");
	xargv[paramCount++] = MgameName;


	for (int i = 0; xargv[i] != NULL; i++)write_log("%s ",xargv[i]);
	write_log("------------");

	do_main(5,(char**)xargv);

	return 1;
}

void retro_unload_game(void)
{
	osd_exit();
}

// Stubs
size_t retro_serialize_size(void){ return 0; }
bool retro_serialize(void *data, size_t size){ return false; }
bool retro_unserialize(const void * data, size_t size){ return false; }

unsigned retro_get_region (void) {return RETRO_REGION_NTSC;}
void *retro_get_memory_data(unsigned type) {return 0;}
size_t retro_get_memory_size(unsigned type) {return 0;}
bool retro_load_game_special(unsigned game_type, const struct retro_game_info *info, size_t num_info){return false;}
void retro_cheat_reset(void){}
void retro_cheat_set(unsigned unused, bool unused1, const char* unused2){}
void retro_set_controller_port_device(unsigned in_port, unsigned device){}

void retro_set_g_av_info()
{
    	g_av_info.geometry.base_width = SCREEN_W;
    	g_av_info.geometry.base_height =SCREEN_H;
    	g_av_info.geometry.max_width = SCREEN_W;
    	g_av_info.geometry.max_height = SCREEN_H;
    	g_av_info.geometry.aspect_ratio =4/3;
    	g_av_info.timing.fps = 60;
    	g_av_info.timing.sample_rate = 44100.0;
}


void retro_poll_input(){

   	input_poll_cb();

	if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP))internal_input_state.buttons|=INPUTBUTTONUP;
	else internal_input_state.buttons&=~INPUTBUTTONUP;

	if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN))internal_input_state.buttons|=INPUTBUTTONDOWN;
	else internal_input_state.buttons&=~INPUTBUTTONDOWN;

	if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT))internal_input_state.buttons|=INPUTBUTTONLEFT;
	else internal_input_state.buttons&=~INPUTBUTTONLEFT;

	if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT))internal_input_state.buttons|=INPUTBUTTONRIGHT;

	else internal_input_state.buttons&=~INPUTBUTTONRIGHT;

	if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START))internal_input_state.buttons|=INPUTBUTTONX;
	else internal_input_state.buttons&=~INPUTBUTTONX;

	if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT))internal_input_state.buttons|=INPUTBUTTONP;
	else internal_input_state.buttons&=~INPUTBUTTONP;


	if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0,  RETRO_DEVICE_ID_JOYPAD_A))internal_input_state.buttons|=INPUTBUTTONA;
	else internal_input_state.buttons&=~INPUTBUTTONA;

	if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0,  RETRO_DEVICE_ID_JOYPAD_B))internal_input_state.buttons|=INPUTBUTTONB;
	else internal_input_state.buttons&=~INPUTBUTTONB;

	if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0,  RETRO_DEVICE_ID_JOYPAD_X))internal_input_state.buttons|=INPUTBUTTONC;
	else internal_input_state.buttons&=~INPUTBUTTONC;

	if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0,  RETRO_DEVICE_ID_JOYPAD_L))internal_input_state.buttons|=INPUTBUTTONL;
	else internal_input_state.buttons&=~INPUTBUTTONL;

	if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0,  RETRO_DEVICE_ID_JOYPAD_R))internal_input_state.buttons|=INPUTBUTTONR;
	else internal_input_state.buttons&=~INPUTBUTTONR;

}

//============================================================

//  init
//============================================================

void osd_exit()
{
    //TODO 
    doh3_exit();  
}
 
void osd_init()
{ 
    retro_set_g_av_info();

    return ;	
}
 
//============================================================
//  osd_update
//============================================================

void osd_update()
{	
	static int frame_end,framerate;
	static int time_start=0;
	static int frame_start=0;
	static int frames=0;
	static int first=1,debug=0;

	if(first==1){
		frame_start=SDL_GetTicks();
		first++;
		time_start=SDL_GetTicks();
	}	

	videoFlip();
	soundRun();
		
	frame_end=SDL_GetTicks();
	framerate=frame_end-frame_start;
	if(framerate<17)SDL_Delay(17-framerate);
	frames++;
	if(framerate>20){
		debug++;
	}

	if((frame_end-time_start)>=1000) 
	{
#ifdef EMUDBG
		printf("framerate:%d fps times:%d\n",frames,debug);
#endif
		frames=0;
		debug=0;			
		time_start=SDL_GetTicks();			

	}
}


//============================================================
//  update_audio_stream
//============================================================

void osd_update_audio_stream(unsigned int fill)
{
	//FIXME
	unsigned int x;
	memcpy(&x,&fill,4);
	audio_cb(x>>16,x&0xFFFF);   	
}

