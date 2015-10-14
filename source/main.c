#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <3ds.h>

#include "filesystem.h"
#include "text.h"

#define bitcheck(x) (number >> x) & 1

int main(int argc, char**argv)
{

    amInit();
    filesystemInit();

	gfxInitDefault();
	//gfxSet3D(true); // uncomment if using stereoscopic 3D
	consoleInit(GFX_TOP, NULL);
/*	
	//u8 mediatype;
	u64 TID;
	//FSUSER_GetMediaType(&saveGameFsHandle,&mediatype);
	aptOpenSession();
	APT_GetProgramID(NULL, &TID);
	aptCloseSession();
*/
	printf("Pokemon Dream Radar Pocket Save Editor v0.1\n");
	printf("------------------------------by suloku '15\n\n");

	int exitnow = 0;

	// Load savegame
	u64 size;
	u8* buffer = NULL;
	//Try to open savefile
	Result res = getSaveGameFileSize("/cygsavedata.dat",&size);
	if (res != 0 ){
		printf ("Can't open cygsavedata.dat\n");
		exitnow = 1;
	} else{
		buffer = (u8*)malloc(size);
		res = readBytesFromSaveFile("/cygsavedata.dat",0,buffer,size);
		if (res != 0 ){
		printf ("Failed to read cygsavedata.dat\n\n");
		exitnow = 1;
		}
	}

	//Save is loaded in buffer
	
	//Variables we'll modify
	u32 orbs_cash;
	u32 orbs_totalcol;
	u32 orbs_reward;
	u8 gen4ext;
	u16 gen4catch;
	
	int save = 0;

	// Main loop
	if (!exitnow){
		//Copy Data
		memcpy (&orbs_cash, buffer+0x68, sizeof(u32));
		memcpy (&orbs_totalcol, buffer+0x6C, sizeof(u32));
		memcpy (&orbs_reward, buffer+0x288, sizeof(u32));
		memcpy (&gen4ext, buffer+0x26E, sizeof(u8));
		memcpy (&gen4catch, buffer+0x272, sizeof(u16));
		int cursor = 0;
		while (aptMainLoop())
		{
			gspWaitForVBlank();
			hidScanInput();
			gotoxy(0,0);
			
			printf("Pokemon Dream Radar Pocket Save Editor v0.1\n");
			printf("------------------------------by suloku '15\n\n");
			
			printf ("\tOrbs Cash: %06lu                           \n", orbs_cash);
			printf ("\tTotal Collected Orbs: %06lu                \n", orbs_totalcol);
			printf ("\tOrb count for next reward: %06lu           \n", orbs_reward);
			
			printf ("\nGen IV Extension:           0x%02X -    0x%04X\n", gen4ext, gen4catch);
			printf ("\tTemoral  (Dialga):   ");
	            if ((gen4ext & 0x4) >1) printf("unlocked - ");
	            else printf ("locked   - ");
	            if ((gen4catch & 0x10) >1) printf("catched  \n");
	            else printf ("uncatched\n");
			printf ("\tSpatial  (Palkia):   ");
	            if ((gen4ext & 0x8) >1) printf("unlocked - ");
	            else printf ("locked   - ");
	            if ((gen4catch & 0x20) >1) printf("catched  \n");
	            else printf ("uncatched\n");
			printf ("\tRenegade (Giratina): ");
	            if ((gen4ext & 0x10) >1) printf("unlocked - ");
	            else printf ("locked   - ");
	            if ((gen4catch & 0x40) >1) printf("catched  \n");
	            else printf ("uncatched\n");
			printf ("\tRainbow  (Ho-Oh):    ");
	            if ((gen4ext & 0x20) >1) printf("unlocked - ");
	            else printf ("locked   - ");
	            if ((gen4catch & 0x80) >1) printf("catched  \n");
	            else printf ("uncatched\n");
			printf ("\tDiving   (Lugia):    ");
	            if ((gen4ext & 0x40) >1) printf("unlocked - ");
	            else printf ("locked   - ");
	            if ((gen4catch & 0x100) >1) printf("catched  \n");
	            else printf ("uncatched\n");

			printf("\n\n\nControls:\n");
			if (cursor < 3){
				printf("\tLEFT/RIGHT: +-100                    \n");
				printf("\tLEFT/RIGHT (hold R): +-1             \n");
				printf("\tLEFT/RIGHT (hold L): +-1000          \n");
			}else{
				printf("\tA: toogle all gen IV extensions      \n");
				printf("\tY: toogle gen IV extensions          \n");
				printf("\tB: toogle gen IV as catched/uncatched\n");
			}
			
			printf("\n\tX: unlock next reward\n");
			printf("\n\nPress START to exit\n");
			printf("Press SELECT to save & exit\n");
			
			u32 kDown = hidKeysDown();
			u32 kHeld = hidKeysHeld();
			if (kDown & KEY_START)
				break; // break in order to return to hbmenu

			if (kDown & KEY_SELECT){
				save = 1;
				break; // save and exit
			}

			if (kDown & KEY_DOWN){
				cursor++;
				if (cursor > 7) cursor = 7;
			}
			if (kDown & KEY_UP){
				cursor--;
				if (cursor < 1) cursor = 0;
			}

			switch (cursor){
				case 0:
					if (kDown & KEY_RIGHT && kHeld & KEY_R){
						orbs_cash++;
					} else if (kHeld & KEY_RIGHT && !(kHeld & (KEY_R|KEY_L))){
						orbs_cash+=100;
					} else if (kHeld & KEY_RIGHT && (kHeld & KEY_L)){
						orbs_cash+=1000;
					}
					if (orbs_cash > 999998) orbs_cash = 999999;
					if (kDown & KEY_LEFT && kHeld & KEY_R){
						orbs_cash--;
					} else if (kHeld & KEY_LEFT && !(kHeld & (KEY_R|KEY_L))){
						orbs_cash-=100;
					} else if (kHeld & KEY_LEFT && (kHeld & KEY_L)){
						orbs_cash-=1000;
					}
					if (orbs_cash < 1 || orbs_cash > 999999) orbs_cash = 0;
					break;

				case 1:
					if (kDown & KEY_RIGHT && kHeld & KEY_R){
						orbs_totalcol++;
					} else if (kHeld & KEY_RIGHT && !(kHeld & (KEY_R|KEY_L))){
						orbs_totalcol+=100;
					} else if (kHeld & KEY_RIGHT && (kHeld & KEY_L)){
						orbs_totalcol+=1000;
					}
					if (orbs_totalcol > 999998) orbs_totalcol = 999999;
					if (kDown & KEY_LEFT && kHeld & KEY_R){
						orbs_totalcol--;
					} else if (kHeld & KEY_LEFT && !(kHeld & (KEY_R|KEY_L))){
						orbs_totalcol-=100;
					} else if (kHeld & KEY_LEFT && (kHeld & KEY_L)){
						orbs_totalcol-=1000;
					}
					if (orbs_totalcol < 1 || orbs_totalcol > 999999) orbs_totalcol = 0;
					break;

				case 2:
					if (kDown & KEY_RIGHT && kHeld & KEY_R){
						orbs_reward++;
					} else if (kHeld & KEY_RIGHT && !(kHeld & (KEY_R|KEY_L))){
						orbs_reward+=100;
					} else if (kHeld & KEY_RIGHT && (kHeld & KEY_L)){
						orbs_reward+=1000;
					}
					if (orbs_reward > 999998) orbs_reward = 999999;
					if (kDown & KEY_LEFT && kHeld & KEY_R){
						orbs_reward--;
					} else if (kHeld & KEY_LEFT && !(kHeld & (KEY_R|KEY_L))){
						orbs_reward-=100;
					} else if (kHeld & KEY_LEFT && (kHeld & KEY_L)){
						orbs_reward-=1000;
					}
					if (orbs_reward < 1 || orbs_reward > 999999) orbs_reward = 0;
					break;
			}
			
			if (kDown & KEY_X){
				orbs_reward = 3000;
			}
			if (kDown & KEY_A){
				//Disable if there's one enabled
				if (gen4ext & 0x7C){
					gen4ext &= ~0x7C;
					//And disable catched flags
					gen4catch &= ~0x10;
					gen4catch &= ~0x20;
					gen4catch &= ~0x40;
					gen4catch &= ~0x80;
					gen4catch &= ~0x100;
				}else //enable all
					gen4ext |= 0x7C;
			}
			if (kDown & KEY_Y){
					if (cursor == 3) gen4ext ^= 0x4;
					if (cursor == 4) gen4ext ^= 0x8;
					if (cursor == 5) gen4ext ^= 0x10;
					if (cursor == 6) gen4ext ^= 0x20;
					if (cursor == 7) gen4ext ^= 0x40;
					//Disable catched if unlocked
					if (!(gen4ext & 0x4)) gen4catch &= ~0x10;
					if (!(gen4ext & 0x8)) gen4catch &= ~0x20;
					if (!(gen4ext & 0x10)) gen4catch &= ~0x40;
					if (!(gen4ext & 0x20)) gen4catch &= ~0x80;
					if (!(gen4ext & 0x40)) gen4catch &= ~0x100;
					
			}
			if (kDown & KEY_B){
					if (gen4ext & 0x4 && cursor == 3) gen4catch ^= 0x10;
					if (gen4ext & 0x8 && cursor == 4) gen4catch ^= 0x20;
					if (gen4ext & 0x10 && cursor == 5) gen4catch ^= 0x40;
					if (gen4ext & 0x20 && cursor == 6) gen4catch ^= 0x80;
					if (gen4ext & 0x40 && cursor == 7) gen4catch ^= 0x100;
			}
			
			gotoxy(0, 3); printf ("  ");
			gotoxy(0, 4); printf ("  ");
			gotoxy(0, 5); printf ("  ");
			gotoxy(0, 8); printf ("  ");
			gotoxy(0, 9); printf ("  ");
			gotoxy(0, 10); printf ("  ");
			gotoxy(0, 11); printf ("  ");
			gotoxy(0, 12); printf ("  ");
			if (cursor < 3)
				gotoxy(0, 3+cursor);
			else if (cursor > 2)
				gotoxy(0, 3+cursor+2);
			printf(">>");

			//if (kHeld || kDown) consoleClear();
			// Flush and swap framebuffers
			gfxFlushBuffers();
			gfxSwapBuffers();
		}
	}else{
		printf("\nPress START to exit.\n");
		while(aptMainLoop()){
			gspWaitForVBlank();
			hidScanInput();

			u32 kDown = hidKeysDown();
			if (kDown & KEY_START)
				break; // break in order to return to hbmenu

			// Flush and swap framebuffers
			gfxFlushBuffers();
			gfxSwapBuffers();
		}
	}
	
	if (save){
		//set data to buffer
			memcpy (buffer+0x68, &orbs_cash, sizeof(u32));
			memcpy (buffer+0x6C, &orbs_totalcol, sizeof(u32));
			memcpy (buffer+0x288, &orbs_reward, sizeof(u32));
			memcpy (buffer+0x26E, &gen4ext, sizeof(u8));
			memcpy (buffer+0x272, &gen4catch, sizeof(u16));
		// Save savegame
		res = writeBytesToSaveFile("/cygsavedata.dat", 0, buffer, size);
	}

	if (buffer != NULL) free(buffer);

    filesystemExit();
    amExit();
	gfxExit();

	return 0;
}
