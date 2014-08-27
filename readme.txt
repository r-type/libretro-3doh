3d'oh! Libretro port (Wip)

3d'oh! A libre (GPL) 3do emulator for GNU/Linux, built around SDL for audio, video and input.
It also includes an optional binding for OpenGL video output, that enables a nice video scaling/stretch.

3d'oh! uses freedo as the core emulator. 
Freedo is a (great) 3do emulation library made by Alexander Troosh, Maxim Grishin, Allen Wright, John Sammons and Felix Lazarev.

3d'oh! also uses some code and ideas from 4do (www.fourdo.com), a 3do emulator that also uses freedo, but this one is written in C#.

Usage: in command line run: 3doemu -b biosfile -i isofile

License: 3d'oh! it's released under the GPLv2 license. 


Author of 3d'oh! : guaripolo

http://code.google.com/p/3doh/


Libretro port: 

	Build:  

		make -f makefile.libretro (for linux)
		make "platform=android" -f makefile.libretro

	Install:

		for linux:

		Put bios in bios/ dir (rename it to bios.bin)
		Put iso files in games/ (ext have to be *.iso)

		for android : 

		Put config.ini in /mnt/sdcard/3doh/
		Put bios in /mnt/sdcard/3doh/bios/ dir (rename it to bios.bin)
		Put iso files in /mnt/sdcard/3doh/games/ (ext have to be *.iso)

		
	Controls:
		
		RETRO_DEVICE_ID_JOYPAD_UP    =>	INPUTBUTTONUP
		RETRO_DEVICE_ID_JOYPAD_DOWN  =>	INPUTBUTTONDOWN
		RETRO_DEVICE_ID_JOYPAD_LEFT  =>	INPUTBUTTONLEFT
		RETRO_DEVICE_ID_JOYPAD_RIGHT =>	INPUTBUTTONRIGHT

		RETRO_DEVICE_ID_JOYPAD_START =>	INPUTBUTTONX
		RETRO_DEVICE_ID_JOYPAD_SELECT=>	INPUTBUTTONP

		RETRO_DEVICE_ID_JOYPAD_A     =>	INPUTBUTTONA;
		RETRO_DEVICE_ID_JOYPAD_B     =>	INPUTBUTTONB;
		RETRO_DEVICE_ID_JOYPAD_X     =>	INPUTBUTTONC;

		RETRO_DEVICE_ID_JOYPAD_L     =>	NPUTBUTTONL;
		RETRO_DEVICE_ID_JOYPAD_R     =>	INPUTBUTTONR;
	
	Status:

		Port is very WIP so Expect bugs
		Seems to have sound issue in android.


