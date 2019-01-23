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

		Texture* barkText = loadFromRenderedText("woof-woof !!!", game);
		Texture* catText = loadFromRenderedText("??", game);

		
		// PLAYER
		Player* player = resetPlayer(playerSpriteSheet);


		int bark = 0;
		int barkCounter = 0;

		int catQuestion = 0;
		int catQuestionCounter = - 20;
		int catQuestionEnd = 0;

		NPC npc1;
		npc1.x = 400;
		npc1.y = 280;
		npc1.velX = 0;
		npc1.velY = 0;
		npc1.width = playerSpriteSheet->sWidth;
		npc1.height = playerSpriteSheet->sHeight;
		SDL_Rect* npcRect;
		npcRect = getSpriteI(playerSpriteSheet, 82, npc1.width, npc1.height);

		SDL_Rect* npcWalkLeftRec[3];
		npcWalkLeftRec[0] = getSpriteI(playerSpriteSheet, 69, player->width, player->height);
		npcWalkLeftRec[1] = getSpriteI(playerSpriteSheet, 70, player->width, player->height);
		npcWalkLeftRec[2] = getSpriteI(playerSpriteSheet, 71, player->width, player->height);
		int npc1WalkingAway = 0;
		int npc1WalkingAnimCounter = 0;
		int npc1WalkingAnimFrame = 0;

		Camera cam;
		cam.x = 0;
		cam.y = 0;
		cam.offsetX = 0;
		cam.offsetY = 0;

		int walking = 0;
		int currentWalk = WALK_LEFT;
		short animPlayerClip = 0, counter = 0;

		

		// LEVEL STUFF...
		Level* level = getLevel(0);
		SDL_Rect* levelBackgroundRects = createRectsForSprites(level, level->size, backgroundSpriteSheet);

		short quit = 0;


		// WALKING LEFT
		SDL_Rect* walkingLeft[3];
		walkingLeft[0] = getSpriteI(playerSpriteSheet, 15, player->width, player->height);
		walkingLeft[1] = getSpriteI(playerSpriteSheet, 16, player->width, player->height);
		walkingLeft[2] = getSpriteI(playerSpriteSheet, 17, player->width, player->height);

		

		Animation* an1 = prepareAnimation(playerSpriteSheet, 3, player->width, player->height, 27, 28, 29);
		printf("Size: %i\n", an1->size);

		// WALKING RIGHT
		SDL_Rect* walkingRight[3];
		walkingRight[0] = getSpriteI(playerSpriteSheet, 27, player->width, player->height);
		walkingRight[1] = getSpriteI(playerSpriteSheet, 28, player->width, player->height);
		walkingRight[2] = getSpriteI(playerSpriteSheet, 29, player->width, player->height);

		// WALKING UP
		SDL_Rect* walkingUp[3];
		walkingUp[0] = getSpriteI(playerSpriteSheet, 39, player->width, player->height);
		walkingUp[1] = getSpriteI(playerSpriteSheet, 40, player->width, player->height);
		walkingUp[2] = getSpriteI(playerSpriteSheet, 41, player->width, player->height);

		// WALKING DOWN
		SDL_Rect* walkingDown[3];
		walkingDown[0] = getSpriteI(playerSpriteSheet, 3, player->width, player->height);
		walkingDown[1] = getSpriteI(playerSpriteSheet, 4, player->width, player->height);
		walkingDown[2] = getSpriteI(playerSpriteSheet, 5, player->width, player->height);


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
						if (e.key.keysym.sym == SDLK_RETURN) {
							bark = 1;
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



			updateCamera(&cam, *player);
			



			if (Mix_PlayingMusic() == 0) {
				Mix_PlayMusic(game->gMusic, -1);
			}

			SDL_SetRenderDrawColor(game->gRenderer, 0x1F, 0x1F, 0x1F, 0xFF);
			SDL_RenderClear(game->gRenderer);

			/**
			 * RENDER ...
			 */ 
			

			// for (int i = 0; i < level->size; i++) {
			// 	renderTexture(
			// 		backgroundSpriteSheet,
			// 		game,
			// 		&levelBackgroundRects[i],
			// 		((i % level->width) * backgroundSpriteSheet->sWidth) + cam.offsetX,
			// 		((i / level->width) * backgroundSpriteSheet->sHeight) + cam.offsetY,
			// 		backgroundSpriteSheet->sWidth,
			// 		backgroundSpriteSheet->sHeight
			// 	);
			// }

			// BUT I NEED TO RENDER ONLY VISIBLE SPRITES ...
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

			if (npc1WalkingAway == 0 && catQuestion > -1) {
				renderTexture(
					playerSpriteSheet,
					game,
					npcRect,
					npc1.x + cam.offsetX,
					npc1.y + cam.offsetY,
					playerSpriteSheet->sWidth,
					playerSpriteSheet->sHeight
				);
			} else {
				npc1.x += - SPEED;

				npc1WalkingAnimCounter++;
				if (npc1WalkingAnimCounter <= 10) npc1WalkingAnimFrame = 0;
				if (npc1WalkingAnimCounter > 10 && npc1WalkingAnimCounter <= 20) npc1WalkingAnimFrame = 1;
				if (npc1WalkingAnimCounter > 20 && npc1WalkingAnimCounter <= 30) npc1WalkingAnimFrame = 2;

				if (npc1WalkingAnimCounter > 30) npc1WalkingAnimCounter = 0;


				renderTexture(
					playerSpriteSheet,
					game,
					npcWalkLeftRec[npc1WalkingAnimFrame],
					npc1.x + cam.offsetX,
					npc1.y + cam.offsetY,
					playerSpriteSheet->sWidth,
					playerSpriteSheet->sHeight
				);
			}

			if (bark == 1) {
				catQuestion = 1;
				renderText(barkText, game, player->x + cam.offsetX, player->y + cam.offsetY - 5, 170, 40);
				barkCounter++;
				if (barkCounter > 30) {
					barkCounter = 0;
					bark = 0;
				}
			}

			// RENDER PLAYER
			renderTexture(
				playerSpriteSheet,
				game,
				&currentWalkAnim[animPlayerClip],
				player->x + cam.offsetX,
				player->y + cam.offsetY,
				playerSpriteSheet->sWidth,
				playerSpriteSheet->sHeight
			);
		
			if (catQuestionEnd > 2) {
				catQuestion = -1;
				catQuestionCounter = -30;
			}
			
			renderText(font1, game, 100, 50, 400, 50);
			
			if (catQuestion == 1) {
				if (catQuestionCounter > 30) {
					renderText(catText, game, npc1.x + cam.offsetX, npc1.y + cam.offsetY - 5, 60, 25);
				}
				catQuestionCounter++;
				if (catQuestionCounter > 80) {
					catQuestionCounter = - 30;
					catQuestion = 0;
					catQuestionEnd++;
				}
			}

			if (walking == 1) {
				// printf("px: %i, py: %i, pindex: %i\n", 
				// 	player->tileX,
				// 	player->tileY,
				// 	player->tileIndex
				// );
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