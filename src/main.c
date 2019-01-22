#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "textures.h"
#include "font.h"




// FORWARD DECLARATION
char* getCurrentTime();
void close(SDL_Game* g);




char* getCurrentTime() {
	time_t t;
    time(&t);
	return asctime(localtime(&t));
}



int main(int argc, char* args[]) {

	SDL_Game* game = initGame();
	if (!game) {
		printf("ERROR!\n");
		exit(1);
	} else {
		printf("Initialization SDL - OK!\nGame.success -> %i\n", game->success);
		

		Texture* playerSpriteSheet = loadSpriteSheet("res/images/animals1.png", game, 52, 72);
		printf("Player spritesheet Width: %i, height: %i\n", playerSpriteSheet->width, playerSpriteSheet->height);

		Texture* backgroundSpriteSheet = loadSpriteSheet("res/images/grassland.png", game, 64, 64);
		printf("Background spritesheet Width: %i, height: %i\n", backgroundSpriteSheet->width, backgroundSpriteSheet->height);

		Texture* font1 = loadFromRenderedText("THE TEMPLE OF THE LOST PUPPY", game);


		// PLAYER
		Player player;
		player.x = (float) (SCREEN_WIDTH / 2);
		player.y = (float) (SCREEN_HEIGHT / 2);
		//player.x = 0.0f;
		//player.y = 0.0f;
		player.velX = 0.0f;
		player.velY = 0.0f;
		player.width = playerSpriteSheet->sWidth;
		player.height = playerSpriteSheet->sHeight;

		Camera cam;
		cam.x = 0.0f;
		cam.y = 0.0f;
		cam.offsetX = 0.0f;
		cam.offsetY = 0.0f;

		int walking = 0;
		int currentWalk = WALK_LEFT;
		short animPlayerClip = 0, counter = 0;

		

		// LEVEL STUFF...
		Level* level = getLevel(0);
		SDL_Rect* levelBackgroundRects = createRectsForSprites(level, level->size, backgroundSpriteSheet);

		short quit = 0;
		
		char str[80];
  		sprintf(str, "TileX: %i", getTileX(&player, &cam, level));

		// WALKING LEFT
		SDL_Rect* walkingLeft[3];
		walkingLeft[0] = getSpriteI(playerSpriteSheet, 12, player.width, player.height);
		walkingLeft[1] = getSpriteI(playerSpriteSheet, 13, player.width, player.height);
		walkingLeft[2] = getSpriteI(playerSpriteSheet, 14, player.width, player.height);

		// WALKING RIGHT
		SDL_Rect* walkingRight[3];
		walkingRight[0] = getSpriteI(playerSpriteSheet, 24, player.width, player.height);
		walkingRight[1] = getSpriteI(playerSpriteSheet, 25, player.width, player.height);
		walkingRight[2] = getSpriteI(playerSpriteSheet, 26, player.width, player.height);

		// WALKING UP
		SDL_Rect* walkingUp[3];
		walkingUp[0] = getSpriteI(playerSpriteSheet, 36, player.width, player.height);
		walkingUp[1] = getSpriteI(playerSpriteSheet, 37, player.width, player.height);
		walkingUp[2] = getSpriteI(playerSpriteSheet, 38, player.width, player.height);

		// WALKING DOWN
		SDL_Rect* walkingDown[3];
		walkingDown[0] = getSpriteI(playerSpriteSheet, 0, player.width, player.height);
		walkingDown[1] = getSpriteI(playerSpriteSheet, 1, player.width, player.height);
		walkingDown[2] = getSpriteI(playerSpriteSheet, 2, player.width, player.height);

		SDL_Rect currentWalkAnim[3];
		currentWalkAnim[0] = *walkingLeft[0];
		currentWalkAnim[1] = *walkingLeft[1];
		currentWalkAnim[2] = *walkingLeft[2];


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
							player.velX = -SPEED;
						}
						if (e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_d) {
							player.velX = SPEED;
						}
						if (e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_w) {
							player.velY = -SPEED;
						}
						if (e.key.keysym.sym == SDLK_DOWN || e.key.keysym.sym == SDLK_s) {
							player.velY = SPEED;
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
							if (player.velX > 0) player.velX = 0;
						}
						if (e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_a) {
							if (player.velX < 0) player.velX = 0;
						}
						if (e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_w) {
							if (player.velY < 0) player.velY = 0;
						}
						if (e.key.keysym.sym == SDLK_DOWN || e.key.keysym.sym == SDLK_s) {
							if (player.velY > 0) player.velY = 0;
						}
					}
				}
			}

		
			player.x += player.velX;
			player.y += player.velY;

			currentWalk = -1;
			walking = 0;

			if (player.velX == SPEED) {
				currentWalk = WALK_RIGHT;
			} else if (player.velX == -SPEED) {
				currentWalk = WALK_LEFT;
			}

			if (player.velY == SPEED) {
				currentWalk = WALK_DOWN;
			} else if (player.velY == -SPEED) {
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



			updateCamera(&cam, player);
			



			if (Mix_PlayingMusic() == 0) {
				Mix_PlayMusic(game->gMusic, -1);
			}

			SDL_SetRenderDrawColor(game->gRenderer, 0x1F, 0x1F, 0x1F, 0xFF);
			SDL_RenderClear(game->gRenderer);

			/**
			 * RENDER ...
			 */ 
			

			// BUT I NEED TO RENDER ONLY VISIBLE SPRITES ...
			for (int i = 0; i < level->size; i++) {
				renderTexture(
					backgroundSpriteSheet,
					game,
					&levelBackgroundRects[i],
					((i % level->width) * backgroundSpriteSheet->sWidth) + cam.offsetX,
					((i / level->width) * backgroundSpriteSheet->sHeight) + cam.offsetY,
					backgroundSpriteSheet->sWidth,
					backgroundSpriteSheet->sHeight
				);
			}

			// renderTexture(
			// 		backgroundSpriteSheet,
			// 		game,
			// 		&levelBackgroundRects[1],
			// 		(( ((int )player.x / 64) % level->width) * backgroundSpriteSheet->sWidth) + cam.offsetX,
			// 		(( ((int )player.y / 64) / level->width) * backgroundSpriteSheet->sHeight) + cam.offsetY,
			// 		backgroundSpriteSheet->sWidth,
			// 		backgroundSpriteSheet->sHeight
			// );

			renderTexture(playerSpriteSheet, game, &currentWalkAnim[animPlayerClip], player.x + cam.offsetX, player.y + cam.offsetY, playerSpriteSheet->sWidth, playerSpriteSheet->sHeight);
		
			
			renderText(font1, game, 100, 50);

			if (walking == 1) {
				printf("TileX: %i, TileY: %i\n", getTileX(&player, &cam, level), getTileY(&player, &cam, level));
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