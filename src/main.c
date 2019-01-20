#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "main.h"
#include "graphics/textures.h"
#include "graphics/font.h"
#include "core/level.h"
#include "sounds/music.h"



// #include "core/filehandler.h"
// #include "modules/writebinary.h"
// #include "modules/readbinary.h"
// #include "core/loghandler.h"
// #include "modules/writestruct.h"


// FORWARD DECLARATION
char* getCurrentTime();
void close(SDL_Game* g);



char* getCurrentTime() {
	time_t t;
    time(&t);
	return asctime(localtime(&t));
}

void close(SDL_Game* g) {

	Mix_FreeMusic(g->gMusic);
    g->gMusic = NULL;

    SDL_DestroyRenderer(g->gRenderer);
    SDL_DestroyWindow(g->gWindow);

    g->gWindow = NULL;
    g->gRenderer = NULL;
	
	TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}




int main(int argc, char* args[]) {

	int playerX = 250, playerY = 150;
	int playerVelX = 0, playerVelY = 0;
	const short SPEED = 2;

	SDL_Game* game = initGame();
	if (!game) {
		printf("ERROR!\n");
		exit(1);
	} else {
		printf("Initialization SDL - OK!\nGame.success -> %i\n", game->success);		

		Texture* playerSpriteSheet = loadTexture("res/images/animals1.png", game);
		printf("Player spritesheet Width: %i, height: %i\n", playerSpriteSheet->width, playerSpriteSheet->height);

		Texture* backgroundSpriteSheet = loadTexture("res/images/grassland.png", game);
		printf("Background spritesheet Width: %i, height: %i\n", backgroundSpriteSheet->width, backgroundSpriteSheet->height);

		Texture* font1 = loadFromRenderedText("THE TEMPLE OF THE LOST PUPPY", game);

		short quit = 0;


		const int WALK_LEFT = 0;
		const int WALK_RIGHT = 1;
		const int WALK_UP = 2;
		const int WALK_DOWN = 3;

		int walking = 0;
		int currentWalk = WALK_LEFT;
		short animPlayerClip = 0, counter = 0;
		

		// WALKING LEFT
		SDL_Rect* walkingLeft[3];
		walkingLeft[0] = getSpriteI(playerSpriteSheet, 12, 52, 72);
		walkingLeft[1] = getSpriteI(playerSpriteSheet, 13, 52, 72);
		walkingLeft[2] = getSpriteI(playerSpriteSheet, 14, 52, 72);

		// WALKING RIGHT
		SDL_Rect* walkingRight[3];
		walkingRight[0] = getSpriteI(playerSpriteSheet, 24, 52, 72);
		walkingRight[1] = getSpriteI(playerSpriteSheet, 25, 52, 72);
		walkingRight[2] = getSpriteI(playerSpriteSheet, 26, 52, 72);

		// WALKING UP
		SDL_Rect* walkingUp[3];
		walkingUp[0] = getSpriteI(playerSpriteSheet, 36, 52, 72);
		walkingUp[1] = getSpriteI(playerSpriteSheet, 37, 52, 72);
		walkingUp[2] = getSpriteI(playerSpriteSheet, 38, 52, 72);

		// WALKING DOWN
		SDL_Rect* walkingDown[3];
		walkingDown[0] = getSpriteI(playerSpriteSheet, 0, 52, 72);
		walkingDown[1] = getSpriteI(playerSpriteSheet, 1, 52, 72);
		walkingDown[2] = getSpriteI(playerSpriteSheet, 2, 52, 72);

		SDL_Rect currentWalkAnim[3];
		currentWalkAnim[0] = *walkingLeft[0];
		currentWalkAnim[1] = *walkingLeft[1];
		currentWalkAnim[2] = *walkingLeft[2];


		int level[16][20] = {
			{17, 19, 17, 18, 17, 19, 17, 19, 17, 19, 17, 19, 17, 17, 17, 19, 17, 17, 17, 17},
			{19, 17, 17, 17, 19, 17, 19, 17, 17, 17, 19, 17, 17, 17, 18, 17, 17, 17, 17, 17},
			{17, 17, 17, 17, 17, 19, 17, 17, 19, 17, 17, 17, 17, 17, 17, 17, 18, 17, 18, 17},
			{19,  0,  1,  1,  2,  3,  4, 17, 18, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17},
			{17, 16, 50, 50, 51, 50, 20, 18, 17, 17, 19, 17, 17, 17, 17, 19, 17, 17, 17, 17},
			{17, 48, 50, 51, 50, 50, 52, 17, 17, 17, 17, 17, 18, 18, 17, 17, 17, 17, 17, 17},
			{19, 64, 67, 65, 66, 67, 68, 17, 19, 17, 19, 17, 17, 17, 18, 19, 17, 19, 17, 17},
			{17, 18, 17, 19, 17, 19, 17, 17, 18, 17, 18, 17, 17, 19, 17, 17, 17, 17, 19, 17},
			{17, 17, 19, 17, 17, 17, 19, 17, 17, 19, 17, 19, 17, 17, 17, 17, 17, 18, 17, 17},
			{17, 18, 17, 19, 17, 19, 17, 17, 18, 17, 18, 17, 19, 17, 17, 17, 17, 17, 17, 17},
			{17, 18, 17, 19, 17, 19, 17, 17, 18, 17, 18, 17, 17, 19, 17, 17, 17, 18, 19, 17},
			{17, 18, 17, 19, 17, 19, 17, 17, 18, 17, 18, 17, 17, 17, 17, 19, 17, 17, 17, 17},
			{17, 18, 17, 19, 17, 19, 17, 17, 18, 17, 18, 17, 17, 17, 17, 17, 19, 17, 17, 17},
			{17, 18, 17, 19, 17, 19, 17, 17, 18, 17, 18, 17, 17, 18, 17, 19, 17, 17, 17, 17}
		};

		const int sizeOfLevelX = 20;
		const int sizeOfLevelY = 16;

		// MAGIC :D
		SDL_Rect* l1[20][16];

		for (int j = 0; j < sizeOfLevelY; j++) {
			for (int i = 0; i < sizeOfLevelX; i++) {
				l1[i][j] = getSpriteI(backgroundSpriteSheet, level[j][i], 64, 64);
			}
		}

		
		SDL_Event e;

		while(!quit) {

			while(SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = 1;
				} else {
					if (e.type == SDL_KEYDOWN) {
						if (e.key.keysym.sym == SDLK_ESCAPE) {
							quit = 1;
						}
						if (e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_a) {
							playerVelX = -SPEED;
						}
						if (e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_d) {
							playerVelX = SPEED;
						}
						if (e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_w) {
							playerVelY = -SPEED;
						}
						if (e.key.keysym.sym == SDLK_DOWN || e.key.keysym.sym == SDLK_s) {
							playerVelY = SPEED;
						}
						if (e.key.keysym.sym == SDLK_SPACE) {
                            if (Mix_PlayingMusic() == 0) {
                                Mix_PlayMusic(game->gMusic, -1);
                            } else {
                                if (Mix_PausedMusic() == 1) {
                                    Mix_ResumeMusic();
                                } else {
                                    Mix_PauseMusic();
                                }
                            }
						}
					}
					if (e.type == SDL_KEYUP) {
						if (e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_d) {
							if (playerVelX > 0) playerVelX = 0;
						}
						if (e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_a) {
							if (playerVelX < 0) playerVelX = 0;
						}
						if (e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_w) {
							if (playerVelY < 0) playerVelY = 0;
						}
						if (e.key.keysym.sym == SDLK_DOWN || e.key.keysym.sym == SDLK_s) {
							if (playerVelY > 0) playerVelY = 0;
						}
					}
				}
			}

		
			playerX += playerVelX;
			playerY += playerVelY;

			currentWalk = -1;
			walking = 0;

			if (playerVelX == SPEED) {
				currentWalk = WALK_RIGHT;
			} else if (playerVelX == -SPEED) {
				currentWalk = WALK_LEFT;
			}

			if (playerVelY == SPEED) {
				currentWalk = WALK_DOWN;
			} else if (playerVelY == -SPEED) {
				currentWalk = WALK_UP;
			}

			if (currentWalk == WALK_RIGHT || currentWalk == WALK_LEFT || currentWalk == WALK_DOWN || currentWalk == WALK_UP) {
				walking = 1;
				if (currentWalk == WALK_RIGHT) {
					currentWalkAnim[0] = *walkingRight[0];
					currentWalkAnim[1] = *walkingRight[1];
					currentWalkAnim[2] = *walkingRight[2];
				}
				if (currentWalk == WALK_LEFT) {
					currentWalkAnim[0] = *walkingLeft[0];
					currentWalkAnim[1] = *walkingLeft[1];
					currentWalkAnim[2] = *walkingLeft[2];
				}
				if (currentWalk == WALK_UP) {
					currentWalkAnim[0] = *walkingUp[0];
					currentWalkAnim[1] = *walkingUp[1];
					currentWalkAnim[2] = *walkingUp[2];
				}
				if (currentWalk == WALK_DOWN) {
					currentWalkAnim[0] = *walkingDown[0];
					currentWalkAnim[1] = *walkingDown[1];
					currentWalkAnim[2] = *walkingDown[2];
				}
			}

			if (Mix_PlayingMusic() == 0) {
				Mix_PlayMusic(game->gMusic, -1);
			}

			SDL_SetRenderDrawColor(game->gRenderer, 0x1F, 0x1F, 0x1F, 0xFF);
			SDL_RenderClear(game->gRenderer);

			/**
			 * RENDER ...
			 */ 
			
			for (int i = 0; i < sizeOfLevelX; i++) {
				for (int j = 0; j < sizeOfLevelY; j++) {
					renderTexture(backgroundSpriteSheet, game, l1[i][j], i * 64, j * 64, 64, 64);
				}
			}

			renderTexture(playerSpriteSheet, game, &currentWalkAnim[animPlayerClip], playerX, playerY, 52, 72);
		
			
			renderText(font1, game, 100, 50);

			if (walking == 1) {
				counter++;
				if (counter <= 10) animPlayerClip = 0;
				if (counter > 10 && counter <= 20) animPlayerClip = 1;
				if (counter > 20 && counter <= 30) animPlayerClip = 2;

				if (counter > 30) counter = 0;
			}
			

			SDL_RenderPresent(game->gRenderer);
		}

		printf("Exiting game ...\n");

		freeTexture(playerSpriteSheet);
		playerSpriteSheet = NULL;

		freeTexture(backgroundSpriteSheet);
		backgroundSpriteSheet = NULL;

		freeTexture(font1);
		
		font1 = NULL;
		close(game);
		free(game);
		game = NULL;
	}




	// writeStruct();

	// FILE *infile; 
    // Person input; 

    // infile = fopen ("person.dat", "r"); 
    // if (infile == NULL) 
    // { 
    //     fprintf(stderr, "\nError opening file\n"); 
    //     exit(1); 
    // } 

    // // while(fread(&input, sizeof(Person), 1, infile)) {
    // //     printf ("id = %d name = %s %s\n", input.id, input.fname, input.lname); 
    // // }

	// fread(&input, sizeof(Person), 1, infile);
	// printf("Id: %d fname: %s, lname: %s\n", input.id, input.fname, input.lname);
        
    // fclose (infile);




	// const char fileName[] = "test.bin";

	// BinaryFile* bf = writeBinaryFile(fileName);

	// if (bf->success) {
	// 	printf("Writing binary file '%s' - OK!\nFile size: %i bytes.\n", bf->name, bf->size);
	// }

	// bf = NULL;
	// bf = readBinaryFile(fileName);
	
	// if (bf->success) {
	// 	printf("Reading binary file '%s' - OK! File size: %i bytes.\n", bf->name, bf->size);
	// }

	// printf("Data (binary):\n");
	// for (int i = 0; i < bf->size; i++) {
	// 	printf("%u ", bf->content[i]);
	// }
	// printf("\nData (decoded text):\n");
	// for (int i = 0; i < bf->size; i++) {
	// 	printf("%c ", bf->content[i]);
	// }
	// printf("\n");
	
	// free(bf);

	// /*
    //  * LOG FILE...
    //  */
	// writeLog(strcat(getCurrentTime(), "An error occured!"));

	return 0;
}
