#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "main.h"
#include "engine.h"
#include "luac.h"




void render(Engine* engine, Player* player, Level* level, Camera* cam, SpriteSheet* ss[], SDL_Rect** layersRects, Ground** grounds, int npcCount, NPC** npcs) {
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
				((player->vec.x + (i * ss[SS_BACKGROUND]->tileWidth) + (player->width / 2)) / ss[SS_BACKGROUND]->tileWidth) >= 0 &&
				((player->vec.x + (i * ss[SS_BACKGROUND]->tileWidth) + (player->width / 2)) / ss[SS_BACKGROUND]->tileWidth) < level->map->width &&
				((player->vec.y + (j * ss[SS_BACKGROUND]->tileHeight) + (player->height / 2)) / ss[SS_BACKGROUND]->tileHeight) >= 0 &&
				((player->vec.y + (j * ss[SS_BACKGROUND]->tileHeight) + (player->height / 2)) / ss[SS_BACKGROUND]->tileHeight) < level->map->height
				) {

				if ((player->tileY + j) * level->width + player->tileX + i >= 0)
					if (layersRects[layer][(player->tileY + j) * level->width + player->tileX + i].w > 0)
						renderTexture(
							ss[SS_BACKGROUND],
							engine->renderer,
							&layersRects[layer][(player->tileY + j) * level->width + player->tileX + i],
							((grounds[layer][player->tileIndex].vec.x  + (i*ss[SS_BACKGROUND]->tileWidth)) * engine->scale) - cam->vec.x,
							((grounds[layer][player->tileIndex].vec.y  + (j*ss[SS_BACKGROUND]->tileHeight)) * engine->scale) - cam->vec.y,
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


	// NPCs
	for (int n = 0; n < npcCount; n++) {
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
			( (npcs[n]->vec.x - (npcs[n]->width / 2) ) * engine->scale) - cam->vec.x,
			( (npcs[n]->vec.y - (npcs[n]->height / 2) ) * engine->scale) - cam->vec.y,
			engine->scale,
			0,
			NULL,
			SDL_FLIP_NONE,
			engine->displayMode
		);
	}


	/** RENDER PLAYER */
	Animation * curAnim = &player->walkingAnimation[player->direction];
	
	renderTexture(
		ss[SS_PLAYER],
		engine->renderer,
		&curAnim->frames[curAnim->curFrame],
		( (player->vec.x - (player->width / 2) ) * engine->scale) - cam->vec.x,
		( (player->vec.y - (player->height / 2) ) * engine->scale) - cam->vec.y,
		engine->scale,
		0,
		NULL,
		SDL_FLIP_NONE,
		engine->displayMode
	);
	
	// ------------------- RENDER END -------------------
	
	SDL_RenderPresent(engine->renderer);
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
	


	// ASM test
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

	SpriteSheet* spriteSheetAssets[2];

	spriteSheetAssets[SS_PLAYER] = loadSpriteSheet("characters.png", engine->renderer, 16, 16);

	loadMusic(engine, "res/a_funny_moment.mod");

	Player* player = NULL;

	engine->camera = malloc(sizeof(Camera));
	engine->camera->vec = setVector(0, 0);


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


	// Animation* playerWalkingAnimation[4];
	player->walkingAnimation = malloc(sizeof(Animation) * 4);

	unsigned int framesPlayerLeft[]  = {16, 17, 18};
	unsigned int framesPlayerRight[] = {28, 29, 30};
	unsigned int framesPlayerUp[]    = {40, 41, 42};
	unsigned int framesPlayerDown[]  = {4,  5,  6 };

	player->walkingAnimation[WALK_UP]    = *prepareAnimation(spriteSheetAssets[SS_PLAYER], 6, player->width, player->height, 3, framesPlayerUp);
	player->walkingAnimation[WALK_RIGHT] = *prepareAnimation(spriteSheetAssets[SS_PLAYER], 6, player->width, player->height, 3, framesPlayerRight);
	player->walkingAnimation[WALK_DOWN]  = *prepareAnimation(spriteSheetAssets[SS_PLAYER], 6, player->width, player->height, 3, framesPlayerDown);
	player->walkingAnimation[WALK_LEFT]  = *prepareAnimation(spriteSheetAssets[SS_PLAYER], 6, player->width, player->height, 3, framesPlayerLeft);		


	const int npcCount = 30;
	NPC* npcs[npcCount];
		
	for (int i = 0; i < npcCount; i++) {
		npcs[i] = setNPC(
			10 + (i * 17),
			10 + (i * 17),
			spriteSheetAssets[SS_PLAYER]->tileWidth,
			spriteSheetAssets[SS_PLAYER]->tileHeight,
			DIR_RIGHT);
		
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
	

	engine->tilesOnScreenFromCenterX = (SCREEN_WIDTH /  (spriteSheetAssets[SS_BACKGROUND]->tileWidth * engine->scale) / 2) + 2;
	engine->tilesOnScreenFromCenterY = (SCREEN_HEIGHT /  (spriteSheetAssets[SS_BACKGROUND]->tileHeight * engine->scale) / 2) + 2;


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

			for (int i = 0; i < npcCount; i++)
				updateCollisionsNPC(npcs[i], engine->camera, engine->scale);
						
			// // ##########################


			// /** ------------------- COLLISSIONS -------------- */
			for (int i = 0; i < npcCount; i++)
				updateCollisionsNPC(npcs[i], engine->camera, engine->scale);

			
			// // ###### NPCs UPDATE #######
			for (int i = 0; i < npcCount; i++) {
			
				updateNPC(npcs[i], level);

				for (int n = i + 1; n < npcCount; n++) {
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

			for (int i = 0; i < npcCount; i++) {
				if (checkCollision(pTempCol, (npcs[i])->col) != 0)
					collisionsPlayerAndNPC = 1;
			}
			
			if (collisionsPlayerAndNPC == 0) {
				addVector(&player->vec, &player->moveVec);
			}


			// --------------------------------------------


			updateCollisionsPlayer(player, engine->camera, engine->scale);
			

			// ###### CAMERA UPDATE ######
			updateCamera(engine, player, level);
			// ###########################


			if (player->isMoving) nextFrame( &((player)->walkingAnimation[player->direction]) );

			player->tileX = getTileX(player, spriteSheetAssets[SS_BACKGROUND]->tileWidth);
			player->tileY = getTileY(player, spriteSheetAssets[SS_BACKGROUND]->tileHeight);
			player->tileIndex = (player->tileY * level->width) + player->tileX;

			// ---------------------- ENGINE UPDATE ----------------------
			engine->tilesOnScreenFromCenterX = (SCREEN_WIDTH /  (spriteSheetAssets[SS_BACKGROUND]->tileWidth * engine->scale) / 2) + 2;
			engine->tilesOnScreenFromCenterY = (SCREEN_HEIGHT /  (spriteSheetAssets[SS_BACKGROUND]->tileHeight * engine->scale) / 2) + 2;
			if (Mix_PlayingMusic() == 0) {
				Mix_PlayMusic(engine->music, -1);
			}		
			// -------------------------------------------


			// ############## UPDATE END ##############


			if (engine->fpsCap) {
				render(engine, player, level, engine->camera, spriteSheetAssets, layersRects, grounds, npcCount, npcs);
				engine->frames++;
			}
			engine->updates++;
			engine->delta--;
		}

		if (!engine->fpsCap) {
			render(engine, player, level, engine->camera, spriteSheetAssets, layersRects, grounds, npcCount, npcs);
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
