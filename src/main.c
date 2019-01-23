#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "game.h"
#include "textures.h"



// FORWARD DECLARATION
char* getCurrentTime();
void close(SDL_Game* g);
Player* resetPlayer(Texture* t);





Player* resetPlayer(Texture* t) {
    Player* p = malloc(sizeof(Player));
	if (p == NULL) return NULL;
    p->x = SCREEN_WIDTH / 2;
    p->y = SCREEN_HEIGHT / 2;
    p->velX = 0;
    p->velY = 0;
    p->width = 52;
	p->height = 72;
    return p;
}


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
		Player* player = resetPlayer(playerSpriteSheet);

		Camera cam;
		cam.x = 0;
		cam.y = 0;
		cam.offsetX = 0;
		cam.offsetY = 0;

		int walking = 0;
		int currentWalk = WALK_LEFT;
		

		// LEVEL STUFF...
		Level* level = getLevel(0);
		SDL_Rect* levelBackgroundRects = createRectsForSprites(level, level->size, backgroundSpriteSheet);

		short quit = 0;


		// DOGS
		NPC dogs[] = {
			{650, 280, 0, 0, playerSpriteSheet->sWidth, playerSpriteSheet->sHeight},
			{400, 210, 0, 0, playerSpriteSheet->sWidth, playerSpriteSheet->sHeight},
			{500, 290, 0, 0, playerSpriteSheet->sWidth, playerSpriteSheet->sHeight},
			{400, 360, 0, 0, playerSpriteSheet->sWidth, playerSpriteSheet->sHeight},
		};

		int framesDog1[] = {72, 73, 74};
		int framesDog2[] = {18, 19, 20};
		int framesDog3[] = {21, 22, 23};
		int framesDog4[] = {33, 34, 35};

		Animation* dogsAnim[] = {
			prepareAnimation(playerSpriteSheet, 1, player->width, player->height, 3, framesDog1),
			prepareAnimation(playerSpriteSheet, 3, player->width, player->height, 3, framesDog2),
			prepareAnimation(playerSpriteSheet, 6, player->width, player->height, 3, framesDog3),
			prepareAnimation(playerSpriteSheet, 9, player->width, player->height, 3, framesDog4),
		};


		int framesPlayerLeft[] = {15, 16, 17};
		Animation* walkingLeftAnimation = prepareAnimation(playerSpriteSheet, 6, player->width, player->height, 3, framesPlayerLeft);

		int framesPlayerRight[] = {27, 28, 29};
		Animation* walkingRightAnimation = prepareAnimation(playerSpriteSheet, 6, player->width, player->height, 3, framesPlayerRight);

		int framesPlayerUp[] = {39, 40, 41};
		Animation* walkingUpAnimation = prepareAnimation(playerSpriteSheet, 6, player->width, player->height, 3, framesPlayerUp);

		int framesPlayerDown[] = {3, 4, 5};
		Animation* walkingDownAnimation = prepareAnimation(playerSpriteSheet, 6, player->width, player->height, 3, framesPlayerDown);


		Animation* currentWalkAnim = walkingLeftAnimation;


		SDL_Event e;
		
		while(!quit) {
			

			/**
			 * #################################################
			 * ..................... INPUT .....................
			 * #################################################
			 * */
			while(SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = 1;
				} else {
					if (e.type == SDL_KEYDOWN) {
						if (e.key.keysym.sym == SDLK_ESCAPE) {
							quit = 1;
						}
						if (e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_a) {
							player->velX = -SPEED;
						}
						if (e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_d) {
							player->velX = SPEED;
						}
						if (e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_w) {
							player->velY = -SPEED;
						}
						if (e.key.keysym.sym == SDLK_DOWN || e.key.keysym.sym == SDLK_s) {
							player->velY = SPEED;
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
							if (player->velX > 0) player->velX = 0;
						}
						if (e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_a) {
							if (player->velX < 0) player->velX = 0;
						}
						if (e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_w) {
							if (player->velY < 0) player->velY = 0;
						}
						if (e.key.keysym.sym == SDLK_DOWN || e.key.keysym.sym == SDLK_s) {
							if (player->velY > 0) player->velY = 0;
						}
					}
				}
			}

			/**
			 * #################################################
			 * .................... UPDATE .....................
			 * #################################################
			 * */
		
			player->x += player->velX;
			player->y += player->velY;

			currentWalk = -1;
			walking = 0;

			if (player->velX == SPEED) {
				currentWalk = WALK_RIGHT;
			} else if (player->velX == -SPEED) {
				currentWalk = WALK_LEFT;
			}

			if (player->velY == SPEED) {
				currentWalk = WALK_DOWN;
			} else if (player->velY == -SPEED) {
				currentWalk = WALK_UP;
			}

			if (currentWalk == WALK_RIGHT || currentWalk == WALK_LEFT || currentWalk == WALK_DOWN || currentWalk == WALK_UP) {
				walking = 1;
				if (currentWalk == WALK_RIGHT) {
					currentWalkAnim = walkingRightAnimation;
				}
				if (currentWalk == WALK_LEFT) {
					currentWalkAnim = walkingLeftAnimation;
				}
				if (currentWalk == WALK_UP) {
					currentWalkAnim = walkingUpAnimation;
				}
				if (currentWalk == WALK_DOWN) {
					currentWalkAnim = walkingDownAnimation;
				}
			}

			updateCamera(&cam, *player);

			if (Mix_PlayingMusic() == 0) {
				Mix_PlayMusic(game->gMusic, -1);
			}



			/**
			 * #################################################
			 * .................... RENDER .....................
			 * #################################################
			 * */
			SDL_SetRenderDrawColor(game->gRenderer, 0x1F, 0x1F, 0x1F, 0xFF);
			SDL_RenderClear(game->gRenderer);


			player->tileX = getTileX(player, &cam, level, backgroundSpriteSheet->sWidth);
			player->tileY = getTileY(player, &cam, level, backgroundSpriteSheet->sHeight);
			player->tileIndex = player->tileY * level->width + player->tileX;


			for (int i = -7; i < 8; i++) {
				for (int j = -5; j < 6; j++) {
					// if (
					// 	player.y + (j * 64) + (player.height / 2) < backgroundSpriteSheet->height &&
					// 	player.x + (i * 64) + (player.width / 2) < backgroundSpriteSheet->width
					// 	) {
						renderTexture(
							backgroundSpriteSheet,
							game,
							&levelBackgroundRects[(player->tileY + j) * level->width + player->tileX + i],
							(( ((player->x + (i * 64) + (player->width / 2)) / 64) % backgroundSpriteSheet->sWidth) * 64) + cam.offsetX,
							(( ((player->y + (j * 64) + (player->height / 2)) / 64) % backgroundSpriteSheet->sHeight) * 64) + cam.offsetY,
							backgroundSpriteSheet->sWidth,
							backgroundSpriteSheet->sHeight
						);
					//}
				}
			}

			// NPCs
			for (int i = 0; i < 4; i++)
				renderTexture(playerSpriteSheet, game, &dogsAnim[i]->frames[nextFrame(dogsAnim[i])], dogs[i].x + cam.offsetX, dogs[i].y + cam.offsetY, playerSpriteSheet->sWidth, playerSpriteSheet->sHeight);


			// RENDER PLAYER
			if (walking == 1) {
				renderTexture(
					playerSpriteSheet,
					game,
					&currentWalkAnim->frames[nextFrame(currentWalkAnim)],
					player->x + cam.offsetX,
					player->y + cam.offsetY,
					playerSpriteSheet->sWidth,
					playerSpriteSheet->sHeight
				);
			} else {
				renderTexture(
					playerSpriteSheet,
					game,
					&currentWalkAnim->frames[currentWalkAnim->curFrame],
					player->x + cam.offsetX,
					player->y + cam.offsetY,
					playerSpriteSheet->sWidth,
					playerSpriteSheet->sHeight
				);
			}
			
			
			renderText(font1, game, 100, 50, 400, 50);
		

			SDL_RenderPresent(game->gRenderer);
		}


		freeTexture(playerSpriteSheet);
		playerSpriteSheet = NULL;

		freeTexture(backgroundSpriteSheet);
		backgroundSpriteSheet = NULL;

		freeTexture(font1);
		font1 = NULL;

		free(level->content);
		level->content = NULL;
		free(level);
		level = NULL;

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