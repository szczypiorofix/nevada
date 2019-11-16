#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

#include "main.h"
#include "luac.h"
#include "font.h"
#include "resources.h"



void render(SpriteSheet* ss[], SDL_Rect** layersRects, Ground** grounds, NPC** npcs) {
	/** ---------- Render part ---------- */
	SDL_SetRenderDrawColor(engine->renderer, 0x0, 0x0, 0x0, 0xFF);
	SDL_RenderClear(engine->renderer);
	

	// ------------------ RENDER START ------------------
	SDL_SetRenderDrawColor(engine->renderer, 0x7F, 0x6F, 0x7F, 0xFF);

	/** Tilesy */
	for (int i = -engine->tilesOnScreenFromCenterX; i < engine->tilesOnScreenFromCenterX; i++) {
		for (int j = -engine->tilesOnScreenFromCenterY; j < engine->tilesOnScreenFromCenterY; j++) {
			for (int layer = 0; layer < level->layers; layer++) {

				if (
					((player->vec.x + (i * ss[SS_BACKGROUND]->tileWidth) + (player->width / 2)) / ss[SS_BACKGROUND]->tileWidth) >= 0 &&
					((player->vec.x + (i * ss[SS_BACKGROUND]->tileWidth) + (player->width / 2)) / ss[SS_BACKGROUND]->tileWidth) < level->map->width &&
					((player->vec.y + (j * ss[SS_BACKGROUND]->tileHeight) + (player->height / 2)) / ss[SS_BACKGROUND]->tileHeight) >= 0 &&
					((player->vec.y + (j * ss[SS_BACKGROUND]->tileHeight) + (player->height / 2)) / ss[SS_BACKGROUND]->tileHeight) < level->map->height
					) {

					if ((player->tileY + j) * level->width + player->tileX + i >= 0) {
						if (layersRects[layer][(player->tileY + j) * level->width + player->tileX + i].w > 0) {

							renderTexture(
								ss[SS_BACKGROUND],
								engine->renderer,
								&layersRects[layer][(player->tileY + j) * level->width + player->tileX + i],
								( (( ( grounds[layer][player->tileIndex].vec.x )  + (i*ss[SS_BACKGROUND]->tileWidth  )) * engine->scale) - engine->camera->vec.x),
								( (( ( grounds[layer][player->tileIndex].vec.y )  + (j*ss[SS_BACKGROUND]->tileHeight )) * engine->scale) - engine->camera->vec.y),
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
		}
	}


	// NPCs
	for (int n = 0; n < NPC_COUNT; n++) {
		Animation* curAnim = npcs[n]->walkingAnimation;
		SDL_Rect* clip;
		if (npcs[n]->takingAction == 1)
			clip = &curAnim->frames[nextFrame(curAnim)];
		else
			clip = &curAnim->frames[curAnim->curFrame];
		renderTexture(
			curAnim->spriteSheet,
			engine->renderer,
			clip,
			( (npcs[n]->vec.x - (npcs[n]->width / 2) ) * engine->scale) - engine->camera->vec.x,
			( (npcs[n]->vec.y - (npcs[n]->height / 2) ) * engine->scale) - engine->camera->vec.y,
			engine->scale,
			0,
			NULL,
			SDL_FLIP_NONE,
			engine->displayMode
		);
	}


	/** RENDER PLAYER */
	Animation* curAnim = &player->walkingAnimation[player->direction];
	renderTexture(
		ss[SS_PLAYER],
		engine->renderer,
		&curAnim->frames[curAnim->curFrame],
		( (player->vec.x - (player->width / 2) ) * engine->scale) - engine->camera->vec.x,
		( (player->vec.y - (player->height / 2) ) * engine->scale) - engine->camera->vec.y,
		engine->scale,
		0,
		NULL,
		SDL_FLIP_NONE,
		engine->displayMode
	);

	renderText(engine->coordinates, engine->renderer, 10, 10, 240, 30);

	// ------------------- RENDER END -------------------
	
	SDL_RenderPresent(engine->renderer);
}


char* strdup(const char* p) {
    char* np = (char*)malloc(strlen(p)+1);
    return np ? strcpy(np, p) : np;
}




/**
 * ================================================================
 * ............................. MAIN .............................
 * ================================================================
 */
int main(int argc, char* args[]) {

	if (argc > 1) {
		printf("Parameters: (%i)\n", argc);
		for (int i = 1; i < argc; i++) {
			printf("%i: %s\n", i, args[i]);
		}
	}


	// luaScriptTest();
	
	// ASM test
    // printf("%i\n", compare(2, 4));





	char buffor[10];
	LoadString(GetModuleHandleA(NULL), 5001, buffor, 10);
	printf("Buforek: ");
	printf(buffor);
	printf("\n");	





	printf("Nevada is starting... Press F5 for toggle VSYNC, mouse roll - zoom-in & zoom-out.\n");
	printf("SPACE - pause music, 1 - increase music volume, 2 - decrease music volume.\n");
	printf("RSHIFT - change display mode\n");

	engine = engineStart();

	loadMusic("res/a_funny_moment.mod");


	// ------------------- BITMAP FONT -------------------

	BitmapFont* bitmapFont = parseBitmapFile("vingue.png", "res/images/vingue.xml", engine->renderer);
	printf("Bitmap font: %i\n", bitmapFont->bitmapFontChar[0].ascii);

	// ---------------------------------------------------


	player = NULL;

	engine->camera = malloc(sizeof(Camera));
	engine->camera->vec = setVector(0, 0);


	level = getLevel("res/images/worldmap.tmx");

	spriteSheetAssets[SS_PLAYER] = loadSpriteSheet("characters.png", engine->renderer, 16, 16);
	if (spriteSheetAssets[SS_PLAYER] == NULL) {
		fprintf(stderr, "SpriteSheet cannot be null. Stop.");
		exit(1);
	}


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

	spriteSheetAssets[SS_BACKGROUND] = loadSpriteSheet(
		*level->textureName,
		engine->renderer,
		level->map->tileWidth,
		level->map->tileWidth 
	);

	SDL_Rect* layersRects[level->layers];
	for (int i = 0; i < level->layers; i++) {
		layersRects[i] = createRectsForSprites(level, i, spriteSheetAssets[SS_BACKGROUND]);
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
			g[i].vec.x = (i % level->columns) * spriteSheetAssets[SS_BACKGROUND]->tileWidth;
			g[i].vec.y = (i / level->columns) * spriteSheetAssets[SS_BACKGROUND]->tileHeight;
		}
		grounds[l] = g;
	}

	player->walkingAnimation = malloc(sizeof(Animation) * 4);

	player->walkingAnimation[WALK_UP]    = *prepareAnimation(spriteSheetAssets[SS_PLAYER], 6, player->width, player->height, 3, framesPlayerUp);
	player->walkingAnimation[WALK_RIGHT] = *prepareAnimation(spriteSheetAssets[SS_PLAYER], 6, player->width, player->height, 3, framesPlayerRight);
	player->walkingAnimation[WALK_DOWN]  = *prepareAnimation(spriteSheetAssets[SS_PLAYER], 6, player->width, player->height, 3, framesPlayerDown);
	player->walkingAnimation[WALK_LEFT]  = *prepareAnimation(spriteSheetAssets[SS_PLAYER], 6, player->width, player->height, 3, framesPlayerLeft);		


	for (int i = 0; i < NPC_COUNT; i++) {
		npcs[i] = setNPC(
			10 + (i * 17),
			10 + (i * 17),
			spriteSheetAssets[SS_PLAYER]->tileWidth,
			spriteSheetAssets[SS_PLAYER]->tileHeight,
			DIR_DOWN);
		
		unsigned int framesNPC1Left[]  = {13, 14, 15};
		unsigned int framesNPC1Right[] = {25, 26, 27};
		unsigned int framesNPC1Up[]    = {37, 38, 39};
		unsigned int framesNPC1Down[]  = {1,  2,  3};

		npcs[i]->walkingAnimation = malloc(sizeof(Animation) * 4);
		npcs[i]->walkingAnimation[WALK_UP] = *prepareAnimation(spriteSheetAssets[SS_PLAYER], 6, npcs[i]->width, npcs[i]->height, 3, framesNPC1Up);
		npcs[i]->walkingAnimation[WALK_RIGHT] = *prepareAnimation(spriteSheetAssets[SS_PLAYER], 6, npcs[i]->width, npcs[i]->height, 3, framesNPC1Right);
		npcs[i]->walkingAnimation[WALK_DOWN] = *prepareAnimation(spriteSheetAssets[SS_PLAYER], 6, npcs[i]->width, npcs[i]->height, 3, framesNPC1Down);
		npcs[i]->walkingAnimation[WALK_LEFT] = *prepareAnimation(spriteSheetAssets[SS_PLAYER], 6, npcs[i]->width, npcs[i]->height, 3, framesNPC1Left);
	}


	player->direction = DIR_RIGHT;
	player->walkingAnimation[player->direction].curFrame = 1;


	engine->tilesOnScreenFromCenterX = (SCREEN_WIDTH /  (spriteSheetAssets[SS_BACKGROUND]->tileWidth * engine->scale) / 2);
	engine->tilesOnScreenFromCenterY = (SCREEN_HEIGHT /  (spriteSheetAssets[SS_BACKGROUND]->tileHeight * engine->scale) / 2) ;

	/* ------------------------------ GAME LOOP ------------------------------ */
	while(engine->quit == 0) {	

		engine->ns = 1000.0f / engine->amountOfTicks;
		engine->now = SDL_GetTicks();
		engine->delta += (engine->now - engine->lastTime) / engine->ns;
		engine->lastTime = engine->now;

		while (engine->delta >= 1) {
			
			while(SDL_PollEvent(&engine->event) != 0) {

				if (engine->event.type == SDL_QUIT) {
					engine->quit = 1;
				} else {
					if (engine->event.type == SDL_MOUSEMOTION ) {
						engine->mouseX = engine->event.motion.x;
						engine->mouseY = engine->event.motion.y;
					}
					// ZOOM
					if (engine->event.type == SDL_MOUSEWHEEL) {
						if (engine->event.button.x == 1) {
							if (engine->scale < engine->maxScale) engine->scale += 0.25f;
						}
						else if (engine->event.button.x == -1) {
							if (engine->scale > engine->minScale) engine->scale -= 0.25f;
						}
					}
					if (engine->event.type == SDL_KEYDOWN) {
						switch (engine->event.key.keysym.sym) {
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
										printf("Music resumed.\n");
										Mix_ResumeMusic();
									} else {
										Mix_PauseMusic();
										printf("Music paused.\n");
									}
								}
								break;
						}
					}
					else if (engine->event.type == SDL_KEYUP) {

						switch (engine->event.key.keysym.sym) {
							case SDLK_F5:
								if (engine->fpsCap == VSYNC_OFF) engine->fpsCap = VSYNC_ON;
								else engine->fpsCap = VSYNC_OFF;
								printf("VSYNC: %s\n", engine->fpsCap == VSYNC_ON ? "on" : "off");
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

			// ###### PLAYER ######

			for (int i = 0; i < NPC_COUNT; i++) {
				updateCollisionsNPC(npcs[i], engine->camera, engine->scale);
			}
						
			// // ##########################


			// /** ------------------- COLLISSIONS -------------- */
			for (int i = 0; i < NPC_COUNT; i++) {
				updateCollisionsNPC(npcs[i], engine->camera, engine->scale);
			}

			
			// // ###### NPCs UPDATE #######
			for (int i = 0; i < NPC_COUNT; i++) {
			
				updateNPC(npcs[i], level);

				for (int n = i + 1; n < NPC_COUNT; n++) {
					SDL_Rect npc1TempRect = {
						( (npcs[i]->vec.x + npcs[i]->moveVec.x ) * engine->scale) - engine->camera->vec.x,
						( (npcs[i]->vec.y + npcs[i]->moveVec.y ) * engine->scale) - engine->camera->vec.y,
						npcs[i]->width * engine->scale,
						npcs[i]->height * engine->scale
					};
					SDL_Rect npc2TempRect = {
						( (npcs[n]->vec.x + npcs[n]->moveVec.x ) * engine->scale) - engine->camera->vec.x,
						( (npcs[n]->vec.y + npcs[n]->moveVec.y ) * engine->scale) - engine->camera->vec.y,
						npcs[n]->width * engine->scale,
						npcs[n]->height * engine->scale
					};
					
					SDL_Rect pTempCol = {
					( (player->vec.x + player->moveVec.x ) * engine->scale) - engine->camera->vec.x,
					( (player->vec.y + player->moveVec.y ) * engine->scale) - engine->camera->vec.y,
					player->width * engine->scale,
					player->height * engine->scale};

					if (checkCollision(npc1TempRect, npc2TempRect) != 0 || checkCollision(npc2TempRect, pTempCol) != 0) {
						npcs[i]->moveVec = setVector(0, 0);
						npcs[n]->moveVec = setVector(0, 0);
						
						int rx = rand() % 3;
						if (rx > 0) {
							npcs[i]->maxTakingActionCounter = 0;
							npcs[n]->maxTakingActionCounter = 0;
						}
						if (rx > 1) {
							npcs[i]->takingActionCounter = 0;
							npcs[n]->takingActionCounter = 0;
						}
						npcs[i]->takingAction = 0;
						npcs[n]->takingAction = 0;
					}
					
				}
				addVector(&npcs[i]->vec, &npcs[i]->moveVec);
			}

			SDL_Rect pTempCol = {
				( (player->vec.x - (player->width / 2) + player->moveVec.x ) * engine->scale) - engine->camera->vec.x,
				( (player->vec.y - (player->height / 2) + player->moveVec.y ) * engine->scale) - engine->camera->vec.y,
				player->width * engine->scale,
				player->height * engine->scale};
			
			int collisionsPlayerAndNPC = 0;

			for (int i = 0; i < NPC_COUNT; i++) {
				if (checkCollision(pTempCol, (npcs[i])->col) != 0)
					collisionsPlayerAndNPC = 1;
			}
			
			if (collisionsPlayerAndNPC == 0) {
				addVector(&player->vec, &player->moveVec);
			}


			// --------------------------------------------


			updateCollisionsPlayer(player, engine->camera, engine->scale);
			

			// ###### CAMERA UPDATE ######
			updateCamera(player, level);
			// ###########################


			if (player->isMoving) nextFrame( &((player)->walkingAnimation[player->direction]) );

			player->tileX = getTileX(player, spriteSheetAssets[SS_BACKGROUND]->tileWidth);
			player->tileY = getTileY(player, spriteSheetAssets[SS_BACKGROUND]->tileHeight);
			player->tileIndex = (player->tileY * level->width) + player->tileX;

			// ---------------------- ENGINE UPDATE ----------------------
			engine->tilesOnScreenFromCenterX = (SCREEN_WIDTH /  (spriteSheetAssets[SS_BACKGROUND]->tileWidth * engine->scale) / 2) + 2 ;
			engine->tilesOnScreenFromCenterY = (SCREEN_HEIGHT /  (spriteSheetAssets[SS_BACKGROUND]->tileHeight * engine->scale) / 2) + 2 ;
			if (Mix_PlayingMusic() == 0) {
				Mix_PlayMusic(engine->music, -1);
			}		
			// -------------------------------------------


			// ---------------------------- TEXT UPDATE ------------------------
			sprintf(engine->coordinatesText, "FPS: %d, MOUSE: %d : %d", engine->fps_count, engine->mouseX, engine->mouseY);
			changeText(engine->coordinates, engine->renderer, engine->coordinatesText);
			// -----------------------------------------------------------------


			// ############## UPDATE END ##############
			if (engine->fpsCap) {
				render(spriteSheetAssets, layersRects, grounds, npcs);
				engine->frames++;
			}
			engine->updates++;
			engine->delta--;
		}

		if (!engine->fpsCap) {
			render(spriteSheetAssets, layersRects, grounds, npcs);
			engine->frames++;
		}

		if (SDL_GetTicks() - engine->timer > 1000) {
			engine->timer += 1000;
			engine->fps_count = engine->frames;
			engine->ticks_count = engine->updates;
			engine->frames = 0;
			engine->updates = 0;
			printf("FPS: %i , TICKS: %i, delta: %f\n", engine->fps_count, engine->ticks_count, engine->delta);
		}

		if (engine->fpsCap) {
			SDL_Delay(1);
		}

	}

	// ------------------ RELEASING ... ----------------------

	SDL_DestroyTexture(spriteSheetAssets[SS_BACKGROUND]->mTexture);
	spriteSheetAssets[SS_BACKGROUND]->mTexture = NULL;
	free(spriteSheetAssets[SS_BACKGROUND]->name);
	spriteSheetAssets[SS_BACKGROUND]->name = NULL;
	free(spriteSheetAssets[SS_BACKGROUND]);
	spriteSheetAssets[SS_BACKGROUND] = NULL;

	SDL_DestroyTexture(spriteSheetAssets[SS_PLAYER]->mTexture);
	spriteSheetAssets[SS_PLAYER]->mTexture = NULL;
	free(spriteSheetAssets[SS_PLAYER]->name);
	spriteSheetAssets[SS_PLAYER]->name = NULL;
	free(spriteSheetAssets[SS_PLAYER]);
	spriteSheetAssets[SS_PLAYER] = NULL;

	releaseAnimation( &((*player).walkingAnimation) );

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

	for (int i = 0; i < NPC_COUNT; i++) {
		free(npcs[i]->walkingAnimation);
		npcs[i]->walkingAnimation = NULL;
		free(npcs[i]->name);
		npcs[i]->name = NULL;
		free(npcs[i]);
		npcs[i] = NULL;
	}
	// free(npcs);
	
	free(player->walkingAnimation);
	player->walkingAnimation = NULL;
	free(player->name);
	player->name = NULL;

	engineStop(&engine);

	printf("Game engine has stopped.\n");
	// getchar();
	
	return 0;
}
