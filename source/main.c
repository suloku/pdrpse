#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <3ds.h>

#include "filesystem.h"
#include "text.h"

#define bitcheck(x) (number >> x) & 1

enum Menu {
	ORBS,
	TOTAL_ORBS,
	REWARD_ORBS,
	DIALGA,
	PALKIA,
	GIRATINA,
	HOHO,
	LUGIA,
	BEAM,
	BAT,
	VISOR,
	VORTEX,
	NET,
	BATCHAR,
	LAST_MENU
};

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
	printf("Pokemon Dream Radar Pocket Save Editor v0.3\n");
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
	u8 clouds;
	u8 gen4ext;
	u16 gen4catch;
	u8 beam;
	u8 battery;
	u8 visor;
	u8 vortex;
	u8 dragnet;
	u8 batcharge;
	
	int save = 0;

	// Main loop
	if (!exitnow){
		//Copy Data
		memcpy (&orbs_cash, buffer+0x68, sizeof(u32)); //Current orbs
		memcpy (&orbs_totalcol, buffer+0x6C, sizeof(u32)); //Total stored orbs (highscore data)
		memcpy (&orbs_reward, buffer+0x288, sizeof(u32)); //Orb count until next story legendary is catched (unlocks events, then resets to 0)
		memcpy (&gen4ext, buffer+0x26E, sizeof(u8)); //Gen IV extension unlocked flags
		memcpy (&gen4catch, buffer+0x272, sizeof(u16)); //Gen IV legendary catched flags
		//Upgrades
		memcpy (&beam, buffer+0x71, sizeof(u8));
		memcpy (&battery, buffer+0x72, sizeof(u8));
		memcpy (&visor, buffer+0x73, sizeof(u8));
		//Clouds
		memcpy (&clouds, buffer+0x74, sizeof(u8));
		//Stockable items
		memcpy (&vortex, buffer+0x268, sizeof(u8));
		memcpy (&dragnet, buffer+0x269, sizeof(u8));
		memcpy (&batcharge, buffer+0x26A, sizeof(u8));

		int cursor = 0;
		while (aptMainLoop())
		{
			gspWaitForVBlank();
			hidScanInput();
			gotoxy(0,0);
			
			printf("Pokemon Dream Radar Pocket Save Editor v0.3\n");
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
			
			printf ("\nUpgrades:    \t\tItems:\n");
			printf("\tBeam:      %d \t\tVortex charge:    %02d\n", beam+1, vortex);
			printf("\tBattery:   %d \t\tDragnet charge:   %02d\n", battery+1, dragnet);
			printf("\tVisoscope: %d \t\tBattery recharge: %02d\n", visor+1, batcharge);

			printf("\nControls:\n");
			if (cursor < DIALGA){
				printf("\tLEFT/RIGHT: +-100                    \n");
				printf("\tLEFT/RIGHT (hold R): +-1             \n");
				printf("\tLEFT/RIGHT (hold L): +-1000          \n");
			}else if (cursor > REWARD_ORBS && cursor < BEAM){
				printf("\tA: toogle all gen IV extensions      \n");
				printf("\tY: toogle gen IV extensions          \n");
				printf("\tB: toogle gen IV as catched/uncatched\n");
			}else if (cursor > LUGIA && cursor < LAST_MENU){
				printf("\tLEFT/RIGHT: decrease/increase        \n");
				printf("\tA: set upgrades to max               \n");
				printf("\tY: set items to max                  \n");
			}
	
			printf("\n\tX: unlock next reward\n");
			printf("\tL: refill clouds (current: %02d/%02d)\n", clouds, 10+(5*visor));
			printf("\nPress START to exit\n");
			printf("Press SELECT to save & exit");
			
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
				if (cursor >= LAST_MENU) cursor = LAST_MENU-1;
			}
			if (kDown & KEY_UP){
				cursor--;
				if (cursor < ORBS) cursor = ORBS;
			}

			switch (cursor){
				case ORBS:
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

				case TOTAL_ORBS:
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

				case REWARD_ORBS:
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
				case DIALGA:
				case PALKIA:
				case GIRATINA:
				case HOHO:
				case LUGIA:
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
							if (cursor == DIALGA) gen4ext ^= 0x4;
							if (cursor == PALKIA) gen4ext ^= 0x8;
							if (cursor == GIRATINA) gen4ext ^= 0x10;
							if (cursor == HOHO) gen4ext ^= 0x20;
							if (cursor == LUGIA) gen4ext ^= 0x40;
							//Disable catched if unlocked
							if (!(gen4ext & 0x4)) gen4catch &= ~0x10;
							if (!(gen4ext & 0x8)) gen4catch &= ~0x20;
							if (!(gen4ext & 0x10)) gen4catch &= ~0x40;
							if (!(gen4ext & 0x20)) gen4catch &= ~0x80;
							if (!(gen4ext & 0x40)) gen4catch &= ~0x100;
							
					}
					if (kDown & KEY_B){
							if (gen4ext & 0x4 && cursor == DIALGA) gen4catch ^= 0x10;
							if (gen4ext & 0x8 && cursor == PALKIA) gen4catch ^= 0x20;
							if (gen4ext & 0x10 && cursor == GIRATINA) gen4catch ^= 0x40;
							if (gen4ext & 0x20 && cursor == HOHO) gen4catch ^= 0x80;
							if (gen4ext & 0x40 && cursor == LUGIA) gen4catch ^= 0x100;
					}
					break;
				case BEAM:
					if (kDown & KEY_LEFT){
						beam--;
						if (beam < 1 || beam > 4) beam = 0;
					}
					if (kDown & KEY_RIGHT){
						beam++;
						if (beam > 4) beam = 4;
					}
					break;
				case BAT:
					if (kDown & KEY_LEFT){
						battery--;
						if (battery < 1 || battery > 4) battery = 0;
					}
					if (kDown & KEY_RIGHT){
						battery++;
						if (battery > 4) battery = 4;
					}
					break;
				case VISOR:
					if (kDown & KEY_LEFT){
						visor--;
						if (visor < 1 || visor > 4) visor = 0;
					}
					if (kDown & KEY_RIGHT){
						visor++;
						if (visor > 4) visor = 4;
					}
					break;
				case VORTEX:
					if (kDown & KEY_LEFT){
						vortex--;
						if (vortex < 1 || vortex > 99) vortex = 0;
					}
					if (kDown & KEY_RIGHT){
						vortex++;
						if (vortex > 99) vortex = 99;
					}
					break;
				case NET:
					if (kDown & KEY_LEFT){
						dragnet--;
						if (dragnet < 1 || dragnet > 99) dragnet = 0;
					}
					if (kDown & KEY_RIGHT){
						dragnet++;
						if (dragnet > 99) dragnet = 99;
					}
					break;
				case BATCHAR:
					if (kDown & KEY_LEFT){
						batcharge--;
						if (batcharge < 1 || batcharge > 99) batcharge = 0;
					}
					if (kDown & KEY_RIGHT){
						batcharge++;
						if (batcharge > 99) batcharge = 99;
					}
					break;
			}
			
			if (kDown & KEY_X){
				orbs_reward = 3000;
			}
			if (kDown & KEY_L){
				switch (visor){
					case 0:
						clouds = 10;
						break;
					case 1:
						clouds = 15;
						break;
					case 2:
						clouds = 20;
						break;
					case 3:
						clouds = 25;
						break;
					case 4:
						clouds = 30;
						break;
				}
			}
			if (kDown & KEY_A && cursor > LUGIA){
				beam = battery = visor = 4;
			}
			if (kDown & KEY_Y && cursor > LUGIA){
				vortex = dragnet = batcharge = 99;
			}

			int i;
			for (i=3; i<21; i++){
				if (i>17){
					gotoxy(18, i-3); printf ("  ");
				}else if (i == 7 || i == 14){
					//Do nothing
				}else{
					gotoxy(0, i); printf ("  ");
				}
			}
			if (cursor < DIALGA)
				gotoxy(0, 3+cursor);
			else if (cursor > REWARD_ORBS && cursor < BEAM)
				gotoxy(0, 3+cursor+2);
			else if (cursor > LUGIA && cursor < VORTEX)
				gotoxy(0, 3+cursor+2+2);
			else if (cursor > VISOR)
				gotoxy(18, 3+cursor+2+2-3);
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
			//Upgrades
			memcpy (buffer+0x71, &beam, sizeof(u8));
			memcpy (buffer+0x72, &battery, sizeof(u8));
			memcpy (buffer+0x73, &visor, sizeof(u8));
			//Clouds
			memcpy (buffer+0x74, &clouds, sizeof(u8));
			//Stockable items
			memcpy (buffer+0x268, &vortex, sizeof(u8));
			memcpy (buffer+0x269, &dragnet, sizeof(u8));
			memcpy (buffer+0x26A, &batcharge, sizeof(u8));
		// Save savegame
		res = writeBytesToSaveFile("/cygsavedata.dat", 0, buffer, size);
	}

	if (buffer != NULL) free(buffer);

    filesystemExit();
    amExit();
	gfxExit();

	return 0;
}
