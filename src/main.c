#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "graphics/textures.h"
#include "graphics/font.h"




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

	int playerX = 400, playerY = 300;
	int playerVelX = 0, playerVelY = 0;

	int offsetX = 0;
	int offsetY = 0;


	Camera cam;
	cam.x = 0;
	cam.y = 0;
	cam.offsetX = 0;
	cam.offsetY = 0;



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



		// LEVEL STUFF...
		Level* level = getLevel(0);

		SDL_Rect* l1[level->size];
		for (int i = 0; i < level->size; i++) {
			l1[i] = getSpriteI(backgroundSpriteSheet, level->content[i], 64, 64);
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



			updateCamera(&cam, playerX, playerY, offsetX, offsetY);




			if (Mix_PlayingMusic() == 0) {
				Mix_PlayMusic(game->gMusic, -1);
			}

			SDL_SetRenderDrawColor(game->gRenderer, 0x1F, 0x1F, 0x1F, 0xFF);
			SDL_RenderClear(game->gRenderer);

			/**
			 * RENDER ...
			 */ 
			
			for (int i = 0; i < level->size; i++) {
				renderTexture(backgroundSpriteSheet, game, l1[i], ((i % level->width) * 64) + cam.offsetX, ((i / level->width) * 64) + cam.offsetY, 64, 64);
			}

			renderTexture(playerSpriteSheet, game, &currentWalkAnim[animPlayerClip], playerX + cam.offsetX, playerY + cam.offsetY, 52, 72);
		
			
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

	return 0;
}
