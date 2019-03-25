#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "main.h"
#include "engine.h"
#include "objects.h"
#include "textures.h"
#include "luac.h"




// FORWARD DECLARATION

typedef struct TextFont {
	char* text;
	SDL_Color textColor;
	TTF_Font* font;
	SpriteSheet* texture;
} TextFont;


void renderText(TextFont* t, SDL_Renderer* renderer, int x, int y, int w, int h) {
    SDL_Rect renderQuad = {x, y, w, h};
    SDL_RenderCopy(renderer, t->texture->mTexture, NULL, &renderQuad);
}


void changeText(TextFont* t, SDL_Renderer* renderer, char* text) {
	SDL_Surface* textSurface = TTF_RenderText_Solid(t->font, text, t->textColor);
    if (textSurface == NULL) {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    } else {
        t->texture->mTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (t->texture->mTexture == NULL) {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        } else {
            t->texture->width = textSurface->w;
            t->texture->height = textSurface->h;
        }
        SDL_FreeSurface(textSurface);
    }
}

TextFont* loadFromRenderedText(const char* textureText, SDL_Renderer* renderer) {
	TextFont* tf = malloc(sizeof(TextFont));

    tf->texture = malloc(sizeof(SpriteSheet));
    if (tf->texture == NULL) return NULL;
    
   	tf->font = TTF_OpenFont("res/camingo.ttf", 28);
    if (tf->font == NULL) {
        printf("Unable to create texture from %s! SDL Error: %s\n", "res/camingo.ttf", SDL_GetError());
    }
    tf->textColor.r = 0xFA;
	tf->textColor.g = 0xFA;
	tf->textColor.b = 0xFA;
	tf->textColor.a = 0xFF;
    SDL_Surface* textSurface = TTF_RenderText_Solid(tf->font, textureText, tf->textColor);
    if (textSurface == NULL) {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    } else {
        tf->texture->mTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (tf->texture->mTexture == NULL) {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        } else {
            tf->texture->width = textSurface->w;
            tf->texture->height = textSurface->h;
        }
        SDL_FreeSurface(textSurface);
    }
    return tf;
}






void input(Engine* engine, Player* player) {

	while(SDL_PollEvent(&engine->event) != 0) {

		if (engine->event.type == SDL_QUIT) {
			engine->quit = TRUE;
		} else {
			// ZOOM
			if (engine->event.type == SDL_MOUSEWHEEL) {
				if (engine->event.button.x == 1) {
					if (engine->scale < engine->maxScale) engine->scale++;
				}
				else if (engine->event.button.x == -1) {
					if (engine->scale > engine->minScale) engine->scale--;
				}
			}
			if (engine->event.type == SDL_KEYDOWN) {
				switch (engine->event.key.keysym.sym) {						
					case SDLK_RETURN:
						player->vec = setVector(0, 0);
						break;
					case SDLK_5:
						player->vec = setVector(16, 16);
						break;
					case SDLK_RSHIFT:
						engine->displayMode++;
						if (engine->displayMode > 2) engine->displayMode = 0;
						break;
					case SDLK_ESCAPE:
						engine->quit = 1;
						break;
					case SDLK_LEFT:
					case SDLK_a:
						player->moveVec.x = -player->speed;								
						break;
					case SDLK_RIGHT:
					case SDLK_d:
						player->moveVec.x = player->speed;
						break;
					case SDLK_UP:
					case SDLK_w:
						player->moveVec.y = -player->speed;
						break;
					case SDLK_DOWN:
					case SDLK_s:
						player->moveVec.y = player->speed;
						break;
					case SDLK_1:
						if (engine->musicVolume < MIX_MAX_VOLUME) engine->musicVolume++;
						Mix_VolumeMusic(engine->musicVolume);
						break;
					case SDLK_2:
						if (engine->musicVolume > 0) engine->musicVolume--;
						Mix_VolumeMusic(engine->musicVolume);
						break;
					case SDLK_SPACE:
						if (Mix_PlayingMusic() == 0) {
							Mix_PlayMusic(engine->music, -1);
						} else {
							if (Mix_PausedMusic() == 1) {
								Mix_ResumeMusic();
							} else {
								Mix_PauseMusic();
							}
						}
						break;
				}
			}
			else if (engine->event.type == SDL_KEYUP) {

				switch (engine->event.key.keysym.sym) {
					case SDLK_F5:
						if (engine->fpsCap == 0) engine->fpsCap = 1;
						else engine->fpsCap = 0;
						printf("VSYNC: %s\n", engine->fpsCap == 1 ? "on" : "off");
						engine->fps = 0;
						engine->ticks = 0;
						break;
					case SDLK_ESCAPE:
						engine->quit = 1;
						break;
					case SDLK_LEFT:
					case SDLK_a:
						if (player->moveVec.x < 0) {
							player->moveVec.x = 0;
						}
						break;
					case SDLK_RIGHT:
					case SDLK_d:
						if (player->moveVec.x > 0) {
							player->moveVec.x = 0;
						}								
						break;
					case SDLK_UP:
					case SDLK_w:
						if (player->moveVec.y < 0) {
							player->moveVec.y = 0;
						}
						break;
					case SDLK_DOWN:
					case SDLK_s:
						if (player->moveVec.y > 0) {
							player->moveVec.y = 0;
						}
						break;
				}

			}
		}
	}
}


/**
 * ################################################################
 * ############################# MAIN #############################
 * ################################################################
 */
int main(int argc, char* args[]) {

	if (argc > 1) {
		for (int i = 1; i < argc; i++)
			printf("Paramers %i: %s\n", i, args[i]);
	}


	luaScriptTest();
	


	// printf("%d\n", compare(5, -4));
    // printf("%d\n", compare(2, -6));
    // printf("%d\n", compare(5, 3));
    // printf("%d\n", compare(-2, 4));
    // printf("%d\n", compare(0, 0));
    // printf("%d\n", compare(7, -6));
    // printf("%d\n", compare(2, 4));
    // printf("%d\n", compare(3, 2));
    // printf("%d\n", compare(6, 6));
	

	Engine* engine = engineStart();

	SpriteSheet* playerSpriteSheet = loadSpriteSheet("characters.png", engine->renderer, 16, 16);

	loadMusic(engine, "res/a_funny_moment.mod");

	Player* player = NULL;
	
	Camera cam;
	cam.vec = setVector(0, 0);

	Level* level = getLevel();

	for (int i = 0; i < level->map->objectGroupCount; i++) {
		for (int j = 0; j < level->map->ObjectGroup[i].objectsCount; j++) {
			printf("ObjectGroup: %i, object: %i %s %s %s %i %i\n", 
				i, 
				j,
				level->map->ObjectGroup[i].objects[j].name,
				level->map->ObjectGroup[i].objects[j].type,
				level->map->ObjectGroup[i].objects[j].template,
				level->map->ObjectGroup[i].objects[j].x,
				level->map->ObjectGroup[i].objects[j].y
			);

			if (strcmp(level->map->ObjectGroup[i].objects[j].name, "player") == 0) {
				player = resetPlayer(
					level->map->ObjectGroup[i].objects[j].name,
					level->map->ObjectGroup[i].objects[j].x,
					level->map->ObjectGroup[i].objects[j].y,
					level->map->ObjectGroup[i].objects[j].width,
					level->map->ObjectGroup[i].objects[j].height
				);
			}
		}
	}
	
	player = resetPlayer(
		"Player",
		16 * 14,
		16 * 12,
		16,
		16
	);

	SpriteSheet* backgroundSpriteSheet = loadSpriteSheet(
		*level->textureName,
		engine->renderer,
		level->map->tileWidth,
		level->map->tileWidth 
	);


	// TextFont* pxText = loadFromRenderedText("Player X", engine->renderer);
	// TextFont* pyText = loadFromRenderedText("Player Y", engine->renderer);
	// TextFont* cxText = loadFromRenderedText("Cam X", engine->renderer);
	// TextFont* cyText = loadFromRenderedText("Cam Y", engine->renderer);


	SDL_Rect* layersRects[level->layers];
	for (int i = 0; i < level->layers; i++) {
		layersRects[i] = createRectsForSprites(level, i, level->size, backgroundSpriteSheet);
	}

	// Ground objects
	Ground* grounds[level->layers];

	for (int l = 0; l < level->layers; l++) {

		Ground* g = malloc(sizeof(Ground) * level->size);
		if (g == NULL) {
			fprintf(stderr, "Error alocating memory for Ground layer!\n");
			exit(0);
		}

		for (unsigned int i = 0; i < level->size; i++) {
			g[i].gid = level->content[l].data[i];
			g[i].width = level->content[l].width;
			g[i].height = level->content[l].height;
			g[i].vec.x = (i % level->columns) * backgroundSpriteSheet->tileWidth;
			g[i].vec.y = (i / level->columns) * backgroundSpriteSheet->tileHeight;
		}
		grounds[l] = g;
	}


	Animation* playerWalkingAnimation[4];

	unsigned int framesPlayerLeft[]  = {16, 17, 18};
	unsigned int framesPlayerRight[] = {28, 29, 30};
	unsigned int framesPlayerUp[]    = {40, 41, 42};
	unsigned int framesPlayerDown[]  = {4,  5,  6};

	playerWalkingAnimation[WALK_UP]    = prepareAnimation(playerSpriteSheet, 6, player->width, player->height, 3, framesPlayerUp);
	playerWalkingAnimation[WALK_RIGHT] = prepareAnimation(playerSpriteSheet, 6, player->width, player->height, 3, framesPlayerRight);
	playerWalkingAnimation[WALK_DOWN]  = prepareAnimation(playerSpriteSheet, 6, player->width, player->height, 3, framesPlayerDown);
	playerWalkingAnimation[WALK_LEFT]  = prepareAnimation(playerSpriteSheet, 6, player->width, player->height, 3, framesPlayerLeft);		


	const int npcCount = 30;
	NPC* npcs[npcCount];
	Animation* animations[npcCount][4];
	
	for (int i = 0; i < npcCount; i++) {
		npcs[i] = setNPC(
			10 + (i * 17),
			10 + (i * 17),
			playerSpriteSheet->tileWidth,
			playerSpriteSheet->tileHeight,
			DIR_RIGHT);
		
		unsigned int framesNPC1Left[]  = {13, 14, 15};
		unsigned int framesNPC1Right[] = {25, 26, 27};
		unsigned int framesNPC1Up[]    = {37, 38, 39};
		unsigned int framesNPC1Down[]  = {1,  2,  3};

		animations[i][WALK_UP]    = prepareAnimation(playerSpriteSheet, 6, npcs[i]->width, npcs[i]->height, 3, framesNPC1Up);
		animations[i][WALK_RIGHT] = prepareAnimation(playerSpriteSheet, 6, npcs[i]->width, npcs[i]->height, 3, framesNPC1Right);
		animations[i][WALK_DOWN]  = prepareAnimation(playerSpriteSheet, 6, npcs[i]->width, npcs[i]->height, 3, framesNPC1Down);
		animations[i][WALK_LEFT]  = prepareAnimation(playerSpriteSheet, 6, npcs[i]->width, npcs[i]->height, 3, framesNPC1Left);

	}



	player->direction = DIR_RIGHT;
	playerWalkingAnimation[player->direction]->curFrame = 1;
	

	engine->tilesOnScreenFromCenterX = (SCREEN_WIDTH /  (backgroundSpriteSheet->tileWidth * engine->scale) / 2) + 2;
	engine->tilesOnScreenFromCenterY = (SCREEN_HEIGHT /  (backgroundSpriteSheet->tileHeight * engine->scale) / 2) + 2;


	// Variable timestep game loop:
	long curFrame = 0;
	long prevFrame = 0;
	double elapsed = 0.0f;
	
	double timer = 0.0f;
	
	short int fps = 0;
	int fpsTimer = 0;
	short int ticks = 0;
	int ticksTimer = 0;
	int TICKS = 0;
	int FPS = 0;

	int destinationTicks = 30;
	int destinationFPS = 120;


	/* ------------------------------ GAME LOOP ------------------------------ */
	while(engine->quit == FALSE) {	

		curFrame = SDL_GetTicks();
		elapsed = curFrame - prevFrame;
		prevFrame = curFrame;

		timer += elapsed;

		fpsTimer += elapsed;
		ticksTimer += elapsed;

		ticks++;
		fps++;

		// ################## INPUT ##################
		input(engine, player);
		// ###########################################

		if (ticksTimer >= destinationTicks ) {

			// WALKING
			player->isMoving = 1;
			if (player->moveVec.x == player->speed) {
				player->direction = DIR_RIGHT;
			} else if (player->moveVec.x == -player->speed) {
				player->direction = DIR_LEFT;
			} else if (player->moveVec.y == player->speed) {
				player->direction = DIR_DOWN;
			} else if (player->moveVec.y == -player->speed) {
				player->direction = DIR_UP;
			} else player->isMoving = 0;

			if (player->vec.x < player->width / 2)
				player->vec.x = player->width / 2;
			if (player->vec.y < player->height / 2)
				player->vec.y = player->height / 2;
			if (player->vec.x > (level->width * level->map->tileWidth) - player->width)
				player->vec.x = (level->width * level->map->tileWidth) - player->width;
			if (player->vec.y > (level->height * level->map->tileHeight) - player->height)
				player->vec.y = (level->height * level->map->tileHeight) - player->height;


			// ###### CAMERA UPDATE ######
			updateCamera(&cam, player, level, engine->scale);
			// ###########################
			

			// ###### PLAYER ######

			for (int i = 0; i < npcCount; i++)
				updateCollisionsNPC(npcs[i], &cam, engine->scale);
			
						
			// ##########################



			/** ------------------- COLLISSIONS -------------- */
			for (int i = 0; i < npcCount; i++)
				updateCollisionsNPC(npcs[i], &cam, engine->scale);

			
			// ###### NPCs UPDATE #######
			// for (int i = 0; i < npcCount; i++) {
			
			// 	updateNPC(npcs[i], level);

			// 	for (int n = i + 1; n < npcCount; n++) {
			// 		SDL_Rect npc1TempRect = {
			// 			( (npcs[i]->vec.x + npcs[i]->moveVec.x ) * engine->scale) - cam.vec.x,
			// 			( (npcs[i]->vec.y + npcs[i]->moveVec.y ) * engine->scale) - cam.vec.y,
			// 			npcs[i]->width * engine->scale,
			// 			npcs[i]->height * engine->scale
			// 		};
			// 		SDL_Rect npc2TempRect = {
			// 			( (npcs[n]->vec.x + npcs[n]->moveVec.x ) * engine->scale) - cam.vec.x,
			// 			( (npcs[n]->vec.y + npcs[n]->moveVec.y ) * engine->scale) - cam.vec.y,
			// 			npcs[n]->width * engine->scale,
			// 			npcs[n]->height * engine->scale
			// 		};
					
			// 		SDL_Rect pTempCol = {
			// 		( (player->vec.x + player->moveVec.x ) * engine->scale) - cam.vec.x,
			// 		( (player->vec.y + player->moveVec.y ) * engine->scale) - cam.vec.y,
			// 		player->width * engine->scale,
			// 		player->height * engine->scale};

			// 		if (checkCollision(npc1TempRect, npc2TempRect) != 0 || checkCollision(npc2TempRect, pTempCol) != 0) {
			// 			npcs[i]->moveVec = setVector(0, 0);
			// 			npcs[n]->moveVec = setVector(0, 0);
						
			// 			int rx = rand() % 3;
			// 			if (rx > 0) {
			// 				npcs[i]->maxTakingActionCounter = 0;
			// 				npcs[n]->maxTakingActionCounter = 0;
			// 			}
			// 			if (rx > 1) {
			// 				npcs[i]->takingActionCounter = 0;
			// 				npcs[n]->takingActionCounter = 0;
			// 			}
			// 			npcs[i]->takingAction = 0;
			// 			npcs[n]->takingAction = 0;
			// 		}
					
			// 	}
			// 	addVector(&npcs[i]->vec, &npcs[i]->moveVec);
			// }

			SDL_Rect pTempCol = {
				( (player->vec.x - (player->width / 2) + player->moveVec.x ) * engine->scale) - cam.vec.x,
				( (player->vec.y - (player->height / 2) + player->moveVec.y ) * engine->scale) - cam.vec.y,
				player->width * engine->scale,
				player->height * engine->scale};
			
			int collisionsPlayerAndNPC = 0;

			for (int i = 0; i < npcCount; i++) {
				if (checkCollision(pTempCol, npcs[i]->col) != 0)
					collisionsPlayerAndNPC = 1;
			}
			
			if (collisionsPlayerAndNPC == 0) {
				addVector(&player->vec, &player->moveVec);
			}

			TICKS = ticks;
			ticks = 0;
			ticksTimer = 0;
		}

		// --------------------------------------------


		updateCollisionsPlayer(player, &cam, engine->scale);
		player->tileX = getTileX(player, backgroundSpriteSheet->tileWidth);
		player->tileY = getTileY(player, backgroundSpriteSheet->tileHeight);
		player->tileIndex = (player->tileY * level->width) + player->tileX;

		// ############## UPDATE END ##############



		// ---------------------- ENGINE UPDATE ----------------------
		engine->tilesOnScreenFromCenterX = (SCREEN_WIDTH /  (backgroundSpriteSheet->tileWidth * engine->scale) / 2) + 2;
		engine->tilesOnScreenFromCenterY = (SCREEN_HEIGHT /  (backgroundSpriteSheet->tileHeight * engine->scale) / 2) + 2;
		if (Mix_PlayingMusic() == 0) {
			Mix_PlayMusic(engine->music, -1);
		}		
		// -------------------------------------------
	
		


			/** ---------- Render part ---------- */
			SDL_SetRenderDrawColor(engine->renderer, 0, 0, 0, 255);
			SDL_RenderClear(engine->renderer);
			

			// ------------------ RENDER START ------------------
			SDL_SetRenderDrawColor(engine->renderer, 120, 120, 120, 200);
		
			/** Tilesy */

			for (int i = -engine->tilesOnScreenFromCenterX; i < engine->tilesOnScreenFromCenterX; i++) {
				for (int j = -engine->tilesOnScreenFromCenterY; j < engine->tilesOnScreenFromCenterY; j++) {
					for (int layer = 0; layer < level->layers; layer++) {

					if (
						((player->vec.x + (i * backgroundSpriteSheet->tileWidth) + (player->width / 2)) / backgroundSpriteSheet->tileWidth) >= 0 &&
						((player->vec.x + (i * backgroundSpriteSheet->tileWidth) + (player->width / 2)) / backgroundSpriteSheet->tileWidth) < level->map->width &&
						((player->vec.y + (j * backgroundSpriteSheet->tileHeight) + (player->height / 2)) / backgroundSpriteSheet->tileHeight) >= 0 &&
						((player->vec.y + (j * backgroundSpriteSheet->tileHeight) + (player->height / 2)) / backgroundSpriteSheet->tileHeight) < level->map->height
						) {

						if ((player->tileY + j) * level->width + player->tileX + i >= 0)
							if (layersRects[layer][(player->tileY + j) * level->width + player->tileX + i].w > 0)
								renderTexture(
									backgroundSpriteSheet,
									engine->renderer,
									&layersRects[layer][(player->tileY + j) * level->width + player->tileX + i],
									((grounds[layer][player->tileIndex].vec.x  + (i*backgroundSpriteSheet->tileWidth)) * engine->scale) - cam.vec.x,
									((grounds[layer][player->tileIndex].vec.y  + (j*backgroundSpriteSheet->tileHeight)) * engine->scale) - cam.vec.y,
									engine->scale,
									0,
									NULL,
									SDL_FLIP_NONE,
									engine->displayMode
								);
						}
					}
				}
			}


			/** RENDER PLAYER */
			
			if (player->isMoving == 1) {
				renderTexture(
					playerSpriteSheet,
					engine->renderer,
					&playerWalkingAnimation[player->direction]->frames[nextFrame(playerWalkingAnimation[player->direction])],
					( (player->vec.x - (player->width / 2) ) * engine->scale) - cam.vec.x,
					( (player->vec.y - (player->height / 2) ) * engine->scale) - cam.vec.y,
					engine->scale,
					0,
					NULL,
					SDL_FLIP_NONE,
					engine->displayMode
				);
			} else {
				renderTexture(
					playerSpriteSheet,
					engine->renderer,
					&playerWalkingAnimation[player->direction]->frames[playerWalkingAnimation[player->direction]->curFrame],
					( (player->vec.x - (player->width / 2) ) * engine->scale) - cam.vec.x,
					( (player->vec.y - (player->height / 2) ) * engine->scale) - cam.vec.y,
					engine->scale,
					0,
					NULL,
					SDL_FLIP_NONE,
					engine->displayMode
				);
			}

			// NPCs
			for (int n = 0; n < npcCount; n++) {
				Animation* curAnim = animations[n][npcs[n]->direction];
				SDL_Rect* clip;
				if (npcs[n]->takingAction == 1)
					clip = &curAnim->frames[nextFrame(curAnim)];
				else
					clip = &curAnim->frames[curAnim->curFrame];
				
				renderTexture(
					curAnim->spriteSheet,
					engine->renderer,
					clip,
					( (npcs[n]->vec.x - (npcs[n]->width / 2) ) * engine->scale) - cam.vec.x,
					( (npcs[n]->vec.y - (npcs[n]->height / 2) ) * engine->scale) - cam.vec.y,
					engine->scale,
					0,
					NULL,
					SDL_FLIP_NONE,
					engine->displayMode
				);
			}
			


			/** ---------- TEXT -----------*/

			// char str_px[50];
			// char str_py[50];
			
			// sprintf(str_px, "%s %.1f", "FPS:", FPS);
			// changeText(pxText, engine->renderer, str_px);
			// sprintf(str_py, "%s %.1f", "TICKS:", TICKS);
			// changeText(pyText, engine->renderer, str_py);


			// renderText(pxText, engine->renderer, 10, 10, 100, 25);
			// renderText(pyText, engine->renderer, 10, 30, 100, 25);


			SDL_SetRenderDrawColor(engine->renderer, 250, 220, 220, 250);
			// Collisiond draw - test
			for (int i = 0; i < npcCount; i++)
				SDL_RenderDrawRect(engine->renderer, &npcs[i]->col);


			SDL_SetRenderDrawColor(engine->renderer, 250, 20, 20, 200);		
			SDL_RenderDrawRect(engine->renderer, &player->col);


			// ------------------- RENDER END -------------------
			
			SDL_RenderPresent(engine->renderer);

			FPS = fps;
			fps = 0;
			fpsTimer = 0;
	

		if (timer > 1000) {
			printf("FPS: %i, TICK: %i\n", FPS, TICKS);
			timer = 0;
		}


	}

	// ------------------ RELEASING ... ----------------------

	SDL_DestroyTexture(backgroundSpriteSheet->mTexture);
	backgroundSpriteSheet->mTexture = NULL;
	free(backgroundSpriteSheet->name);
	backgroundSpriteSheet->name = NULL;
	free(backgroundSpriteSheet);
	backgroundSpriteSheet = NULL;


	SDL_DestroyTexture(playerSpriteSheet->mTexture);
	playerSpriteSheet->mTexture = NULL;
	free(playerSpriteSheet->name);
	playerSpriteSheet->name = NULL;
	free(playerSpriteSheet);
	playerSpriteSheet = NULL;


	for (int i = 0; i < level->layers; i++) {
		free(layersRects[i]);
		layersRects[i] = NULL;
	}

	for (int i = 0; i < level->layers; i++) {
		free(level->content[i].encoding);
		level->content[i].encoding = NULL;
		free(level->content[i].name);
		level->content[i].name = NULL;
		free(level->content[i].data);
		level->content[i].data = NULL;
	}

	for (int i = 0; i < level->map->layersCount; i++) {
		
		free( &(level->map->layer[i]).data );
		level->map->layer[i].data = NULL;
		
		free( &level->map->layer[i].name );
		level->map->layer[i].name = NULL;
		
		free( &level->map->layer[i].encoding );
		level->map->layer[i].encoding = NULL;
		
	}

	free(level->map);
	level->map = NULL;

	free(level->content);
	level->content = NULL;

	// free(level);
	// level = NULL;
	
	engineStop(&engine);

	return 0;
}
