#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef RETRO
#include <SDL/SDL.h>
#else
#include <unistd.h>
extern void osd_update_audio_stream(unsigned int fill);
#endif

#include "freedocore.h"
#include "frame.h"
#include "common.h"
#include "sound.h"
#include "video.h"
#include "cdrom.h"
#include "input.h"
#include "config.h"


char* pNVRam;
extern _ext_Interface  io_interface;
_ext_Interface  fd_interface;
#ifndef RETRO
SDL_Event event;
#endif
int onsector=0;
char biosFile[100];
char imageFile[100];

bool __temporalfixes;
int HightResMode;
int __tex__scaler;

int initEmu(int xres,int yres, int bpp, int armclock);

UInt32 ReverseBytes(UInt32 value)
{
			return (value & 0x000000FFU) << 24 | (value & 0x0000FF00U) << 8 |
				   (value & 0x00FF0000U) >> 8 | (value & 0xFF000000U) >> 24;
}


void readNvRam(void *pnvram)
{

	FILE* bios1;
	long fsize;
	char *buffer;
	NvRamStr *nvramStruct;

//	nvramStruct=(NvRamStr *)malloc(sizeof(NvRamStr));
	nvramStruct=(NvRamStr *)pnvram;
			////////////////
			// Fill out the volume header.
			nvramStruct->recordType = 0x01;
			for (int x = 0; x < 5; x++) nvramStruct->syncBytes[x] = (char)'Z';
			nvramStruct->recordVersion = 0x02;
			nvramStruct->flags = 0x00;
			for (int x = 0; x < 32; x++) nvramStruct->comment[x] = 0;

			nvramStruct->label[0] = (char)'n';
			nvramStruct->label[1] = (char)'v';
			nvramStruct->label[2] = (char)'r';
			nvramStruct->label[3] = (char)'a';
			nvramStruct->label[4] = (char)'m';
			for (int x = 5; x < 32; x++) nvramStruct->label[x] = 0;

			nvramStruct->id         = ReverseBytes(0xFFFFFFFF);
			nvramStruct->blockSize  = ReverseBytes(0x00000001); // Yep, one byte per block.
			nvramStruct->blockCount = ReverseBytes(0x00008000); // 32K worth of NVRAM data.

			nvramStruct->rootDirId        = ReverseBytes(0xFFFFFFFE);
			nvramStruct->rootDirBlocks    = ReverseBytes(0x00000000);
			nvramStruct->rootDirBlockSize = ReverseBytes(0x00000001);
			nvramStruct->lastRootDirCopy  = ReverseBytes(0x00000000);

			nvramStruct->rootDirCopies[0] = ReverseBytes(0x00000084);
			for (int x = 1; x < 8; x++) nvramStruct->rootDirCopies[x] = 0;

			////////////////
			// After this point, I could not find the proper structure for the data.
			int w = sizeof(NvRamStr) / 4;

	/*		uint32_t nvramData 
			nvramData[w++] = ReverseBytes(0x855A02B6);
			nvramData[w++] = ReverseBytes(0x00000098);
			nvramData[w++] = ReverseBytes(0x00000098);
			nvramData[w++] = ReverseBytes(0x00000014);
			nvramData[w++] = ReverseBytes(0x00000014);
			nvramData[w++] = ReverseBytes(0x7AA565BD);
			nvramData[w++] = ReverseBytes(0x00000084);
			nvramData[w++] = ReverseBytes(0x00000084);
			nvramData[w++] = ReverseBytes(0x00007668); // This is blocks remaining.
			nvramData[w++] = ReverseBytes(0x00000014);
*/


//	buffer = (char*)malloc(32*1024);


//	pnvram=(void *)nvramStruct;

}




void loadRom1(void *prom)
{
	FILE* bios1;
	long fsize;
	int readcount;

	printf("INFO: loading bios in %s\n",biosFile);
	bios1=fopen(biosFile,"rb");
	if (bios1==NULL)printf("ERROR: Bios load error\n");
	printf("INFO: Bios load success\n");
    fseek (bios1 , 0 , SEEK_END);
	fsize = ftell(bios1);
	rewind (bios1);

	readcount=fread(prom,1,fsize,bios1);
	fclose(bios1);


//	return buffer;


}


void *swapFrame(void *curr_frame)
{
//		printf("swap frame\n");
		return curr_frame;
	
}

void * emuinterface(int procedure, void *datum=0)
{
	typedef void *(*func_type)(void);
	void *therom;
	switch(procedure)
	{
	case EXT_READ_ROMS:
		loadRom1(datum);
		break;
	case EXT_READ2048:
	cdromReadBlock(datum,onsector);
		break;
	case EXT_GET_DISC_SIZE:
		return (void *) cdromDiscSize();
		break;
	case EXT_ON_SECTOR:
		onsector=(int)datum;
		break;
	case EXT_READ_NVRAM:
		readNvRam(datum);
		break;
	case EXT_WRITE_NVRAM:
		break;
	case EXT_PUSH_SAMPLE:
#ifndef RETRO
		soundFillBuffer((unsigned int)datum);
#else
		osd_update_audio_stream((unsigned int)datum);
#endif
		break;
	case EXT_SWAPFRAME:
		return swapFrame(datum);
		break;
	case EXT_GETP_PBUSDATA:
		return (void *)inputRead();
		break;
	case EXT_GET_PBUSLEN:
		return (void *)inputLength();
		break;
	case EXT_FRAMETRIGGER_MT:
		break;
	default:
	//	return _freedo_Interface(procedure,datum);
		break;
	};

return (void *)readNvRam;
}


void readConfiguration()
{
  #ifndef RETRO_AND
	configOpen("config.ini");
  #else
	configOpen("/mnt/sdcard/3doh/config.ini");
  #endif

//	configReadInt("general","biosfile");
//	configReadInt("general","screenwidth");
//	configReadInt("general","screenheight");
	configClose();

}

#ifndef RETRO
int main(int argc, char *argv[])
#else
int do_main(int argc, char *argv[])
#endif
{

	int biosset=0;
	int imageset=0;

	readConfiguration();	

	if(argc>1){


		int i;
		for(i=0;i<argc;i++)
		{
//			printf("%s\n",argv[i]);   
			if(strcmp(argv[i],"-b")==0){
			/*set bios filename*/   
  #ifndef RETRO_AND
			sprintf(biosFile,"bios/%s",argv[i+1]);
  #else
			sprintf(biosFile,"/mnt/sdcard/3doh/bios/%s",argv[i+1]);
  #endif
			biosset=1;
			}
			if(strcmp(argv[i],"-i")==0){
			/*set image (iso) filename*/
  #ifndef RETRO_AND
			sprintf(imageFile,"games/%s",argv[i+1]);
  #else
			sprintf(imageFile,"/mnt/sdcard/3doh/games/%s",argv[i+1]);
  #endif
			imageset=1;
			}
		}
	}

	if((biosset)&&(imageset))
	{
#ifndef RETRO
		if(!initEmu(800,600,32,12500000)) return 0;
#else
		initEmu(640,480,32,12500000);

		return 0;
#endif
		/*free resources*/
		fd_interface(FDP_DESTROY,(void *)0);
		soundClose();
#ifndef RETRO
		SDL_Quit();
#endif
	}else{

		printf("3d'oh! - a 3do emulator\nUsage 3doemu -b biosfile -i isofile\n");

	}


	return 0;
}

#ifdef RETRO
void doh3_exit(){
	fd_interface(FDP_DESTROY,(void *)0);
}
#endif

///int main(int argc, char *argv[])
int initEmu(int xres,int yres, int bpp, int armclock)
{

	printf("INFO: starting 3d'oh! emulator\n");

	int arm_clock=armclock;
	int tex_quality=0;
	int quit=0;	

//	memset(frame,0,sizeof(VDLFrame));
//	printf("frame %p\n",frame);
	io_interface=&emuinterface;
	fd_interface=&_freedo_Interface;

	initVideo(xres,yres,bpp);
//	toggleFullscreen(screen);

	soundInit();

	inputInit();

	if(!cdromOpenIso(imageFile)) return 0;


//init freedo core

	void *prom;
	void *profile;



	fd_interface(FDP_SET_ARMCLOCK,(void *)arm_clock);
	fd_interface(FDP_SET_TEXQUALITY,(void *)tex_quality);
//	io_interface(FDP_INIT,0);
//	profile=io_interface(FDP_GETP_PROFILE,0);
	_3do_Init();

#ifdef RETRO
	return 1;
#else
	int frame_end,framerate;
	int time_start=0;
	int frame_start=0;
	int frames=0;
	int debug=0;
	time_start=SDL_GetTicks();
	while(!quit){

	frame_start=SDL_GetTicks();


//		printf("profile %d\n",&profile);

//		printf("framerate1:%d\n",framerate);


	

		videoFlip();
		soundRun();


		quit = inputQuit();
		frame_end=SDL_GetTicks();
		framerate=frame_end-frame_start;
		if(framerate<17)SDL_Delay(17-framerate);
		frames++;
		if(framerate>20){
			debug++;
		}
		if((frame_end-time_start)>=1000) 
		{

			printf("framerate:%d fps times:%d\n",frames,debug);
			frames=0;
			debug=0;
			time_start=SDL_GetTicks();
			

		}

	}


	return 1;
#endif
}
