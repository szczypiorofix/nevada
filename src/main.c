#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #include <lua.h>
// #include <lauxlib.h>
// #include <lualib.h>

#include "main.h"
#include "registry.h"
#include "engine.h"
#include "objects.h"
#include "assetslist.h"
#include "textures.h"



// FORWARD DECLARATION

int compare(int, int);



// void renderText(Texture* t, SDL_Game* game, int x, int y, int w, int h) {
//     SDL_Rect renderQuad = {x, y, w, h};
//     SDL_RenderCopy(game->gRenderer, t->mTexture, NULL, &renderQuad);
// }

// Texture* loadFromRenderedText(const char* textureText, SDL_Game* game) {
//     assert(game != NULL && textureText != NULL);
//     Texture* t = malloc(sizeof(Texture));
//     if (t == NULL) return NULL;
//     TTF_Font *gFont = NULL;
//     gFont = TTF_OpenFont("res/camingo.ttf", 28);
//     if (gFont == NULL) {
//         printf("Unable to create texture from %s! SDL Error: %s\n", "res/camingo.ttf", SDL_GetError());
//     }
//     SDL_Color textColor = {0xFF, 0x65, 0x00};
//     SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText, textColor);
//     if (textSurface == NULL) {
//         printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
//     } else {
//         t->mTexture = SDL_CreateTextureFromSurface(game->gRenderer, textSurface);
//         if (t->mTexture == NULL) {
//             printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
//         } else {
//             t->width = textSurface->w;
//             t->height = textSurface->h;
//         }
//         SDL_FreeSurface(textSurface);
//     }
//     return t;
// }


// void luaTest() {
//     int status, result, i;
//     double sum;
//     lua_State *L;

//     /*
//      * All Lua contexts are held in this structure. We work with it almost
//      * all the time.
//      */
//     L = luaL_newstate();

//     luaL_openlibs(L); /* Load Lua libraries */

//     /* Load the file containing the script we are going to run */
//     status = luaL_loadfile(L, "res/script.lua");
//     if (status) {
//         /* If something went wrong, error message is at the top of */
//         /* the stack */
//         fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(L, -1));
//         exit(1);
//     }

//     /*
//      * Ok, now here we go: We pass data to the lua script on the stack.
//      * That is, we first have to prepare Lua's virtual stack the way we
//      * want the script to receive it, then ask Lua to run it.
//      */
//     lua_newtable(L);    /* We will pass a table */

//     /*
//      * To put values into the table, we first push the index, then the
//      * value, and then call lua_rawset() with the index of the table in the
//      * stack. Let's see why it's -3: In Lua, the value -1 always refers to
//      * the top of the stack. When you create the table with lua_newtable(),
//      * the table gets pushed into the top of the stack. When you push the
//      * index and then the cell value, the stack looks like:
//      *
//      * <- [stack bottom] -- table, index, value [top]
//      *
//      * So the -1 will refer to the cell value, thus -3 is used to refer to
//      * the table itself. Note that lua_rawset() pops the two last elements
//      * of the stack, so that after it has been called, the table is at the
//      * top of the stack.
//      */
//     for (i = 1; i <= 10; i++) {
//         lua_pushnumber(L, i);   /* Push the table index */
//         lua_pushnumber(L, i*2); /* Push the cell value */
//         lua_rawset(L, -3);      /* Stores the pair in the table */
//     }

//     /* By what name is the script going to reference our table? */
//     lua_setglobal(L, "foo");

//     /* Ask Lua to run our little script */
//     result = lua_pcall(L, 0, LUA_MULTRET, 0);
//     if (result) {
//         fprintf(stderr, "Failed to run script: %s\n", lua_tostring(L, -1));
//         exit(1);
//     }

//     /* Get the returned value at the top of the stack (index -1) */
//     sum = lua_tonumber(L, -1);

//     printf("Script returned: %.0f\n", sum);

//     lua_pop(L, 1);  /* Take the returned value out of the stack */
//     lua_close(L);
// }



int main(int argc, char* args[]) {

	if (argc > 1) {
		for (int i = 1; i < argc; i++)
			printf("Paramers %i: %s\n", i, args[i]);
	}


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

	// registryInit();

	// String str1 = createString("Nowy 1");
	// printf("Data #1: %s\n", str1.str);

	// String str2 = createString("Nowy string 2");
	// printf("Data #2: %s\n", str2.str);

	printf("Engine status: %s\n", engine->started == TRUE ? "working." : "start failure.");


	Texture* playerSpriteSheet = loadSpriteSheet("animals1.png", NPC_SPRITESHEET, engine->renderer, 52, 72);
	// Texture* npc1SpriteSheet = loadSpriteSheet("birds1.png", NPC_SPRITESHEET, engine->renderer, 84, 72);
	// Texture* npc2SpriteSheet = loadSpriteSheet("animals3.png", NPC_SPRITESHEET, engine->renderer, 84, 72);


	loadMusic(engine, "res/a_funny_moment.mod");

	Player* player = NULL;

	Camera cam;
	cam.x = 0;
	cam.y = 0;
	cam.offsetX = 0;
	cam.offsetY = 0;

	// LEVEL STUFF...
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

	Texture* backgroundSpriteSheet = loadSpriteSheet(
		*level->textureName,
		GRASSLAND1_SPRITESHEET,
		engine->renderer,
		level->map->tileWidth,
		level->map->tileWidth
	);

	

	// addTextureToAssets(engine->assets, playerSpriteSheet);
	// addTextureToAssets(engine->assets, npc1SpriteSheet);
	// addTextureToAssets(engine->assets, backgroundSpriteSheet);

	
	// printf("Texture name %s, %i:%i\n", *level->textureName, level->map->tileWidth, level->map->tileWidth);


	SDL_Rect* layersRects[level->layers];
	for (int i = 0; i < level->layers; i++) {
		layersRects[i] = createRectsForSprites(level, i, level->size, backgroundSpriteSheet);
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

	// const int dogsCount = 3;
	// NPC* dogs[dogsCount];
	// // DOGS
	// dogs[0] = setNPC(
	// 	650,
	// 	280,
	// 	playerSpriteSheet->sWidth,
	// 	playerSpriteSheet->sHeight,
	// 	DIR_RIGHT);
	// unsigned int framesDog1Left[]  = {13, 14, 15};
	// unsigned int framesDog1Right[] = {25, 26, 27};
	// unsigned int framesDog1Up[]    = {37, 38, 39};
	// unsigned int framesDog1Down[]  = {1,  2,  3};

	// Animation* animations[dogsCount][4];
	
	// animations[0][WALK_UP]    = prepareAnimation(playerSpriteSheet, 6, dogs[0]->width, dogs[0]->height, 3, framesDog1Up);
	// animations[0][WALK_RIGHT] = prepareAnimation(playerSpriteSheet, 6, dogs[0]->width, dogs[0]->height, 3, framesDog1Right);
	// animations[0][WALK_DOWN]  = prepareAnimation(playerSpriteSheet, 6, dogs[0]->width, dogs[0]->height, 3, framesDog1Down);
	// animations[0][WALK_LEFT]  = prepareAnimation(playerSpriteSheet, 6, dogs[0]->width, dogs[0]->height, 3, framesDog1Left);


	// dogs[1] = setNPC(
	// 	730,
	// 	180,
	// 	npc2SpriteSheet->sWidth,
	// 	npc2SpriteSheet->sHeight,
	// 	DIR_RIGHT
	// );
	// unsigned int framesDog2Left[]  = {13, 14, 15};
	// unsigned int framesDog2Right[] = {25, 26, 27};
	// unsigned int framesDog2Up[]    = {37, 38, 39};
	// unsigned int framesDog2Down[]  = { 1,  2,  3};

	// animations[1][WALK_UP]    = prepareAnimation(npc2SpriteSheet, 5, dogs[1]->width, dogs[1]->height, 3, framesDog2Up);
	// animations[1][WALK_RIGHT] = prepareAnimation(npc2SpriteSheet, 5, dogs[1]->width, dogs[1]->height, 3, framesDog2Right);
	// animations[1][WALK_DOWN]  = prepareAnimation(npc2SpriteSheet, 5, dogs[1]->width, dogs[1]->height, 3, framesDog2Down);
	// animations[1][WALK_LEFT]  = prepareAnimation(npc2SpriteSheet, 5, dogs[1]->width, dogs[1]->height, 3, framesDog2Left);

	// dogs[2] = setNPC(
	// 	530,
	// 	320,
	// 	npc1SpriteSheet->sWidth,
	// 	npc1SpriteSheet->sHeight,
	// 	DIR_RIGHT
	// );
	// unsigned int framesDog3Left[]  = {16, 17, 18};
	// unsigned int framesDog3Right[] = { 4,  5,  6};
	// unsigned int framesDog3Up[]    = {28, 29, 30};
	// unsigned int framesDog3Down[]  = {40, 41, 42};
	
	// animations[2][WALK_UP]    = prepareAnimation(npc1SpriteSheet, 3, dogs[2]->width, dogs[2]->height, 3, framesDog3Up);
	// animations[2][WALK_RIGHT] = prepareAnimation(npc1SpriteSheet, 3, dogs[2]->width, dogs[2]->height, 3, framesDog3Right);
	// animations[2][WALK_DOWN]  = prepareAnimation(npc1SpriteSheet, 3, dogs[2]->width, dogs[2]->height, 3, framesDog3Down);
	// animations[2][WALK_LEFT]  = prepareAnimation(npc1SpriteSheet, 3, dogs[2]->width, dogs[2]->height, 3, framesDog3Left);

	player->direction = DIR_RIGHT;
	playerWalkingAnimation[player->direction]->curFrame = 1;
	

	// Uint8 tr = 255, tg = 255, tb = 255;
	// Uint8 alpha = 255;

	/* ------------------------------ GAME LOOP ------------------------------ */
	while(engine->quit == FALSE) {
		
		updateDeltaTime(engine);

		while(SDL_PollEvent(&engine->event) != 0) {
				if (engine->event.type == SDL_QUIT) {
					engine->quit = TRUE;
				} else {
					if (engine->event.type == SDL_KEYDOWN) {
						switch (engine->event.key.keysym.sym) {
							// case SDLK_9:
							// 	alpha += 32;
							// 	SDL_SetTextureAlphaMod(backgroundSpriteSheet->mTexture, alpha);
							// 	break;
							// case SDLK_0:
							// 	alpha -= 32;
							// 	SDL_SetTextureAlphaMod(backgroundSpriteSheet->mTexture, alpha);
							// 	break;
							// case SDLK_u:
							// 	tr -= 32;
							// 	SDL_SetTextureColorMod(backgroundSpriteSheet->mTexture, tr, tg, tb);
							// 	break;
							// case SDLK_i:
							// 	tr += 32;
							// 	SDL_SetTextureColorMod(backgroundSpriteSheet->mTexture, tr, tg, tb);
							// 	break;
							// case SDLK_j:
							// 	tg -= 32;
							// 	SDL_SetTextureColorMod(backgroundSpriteSheet->mTexture, tr, tg, tb);
							// 	break;
							// case SDLK_k:
							// 	tg += 32;
							// 	SDL_SetTextureColorMod(backgroundSpriteSheet->mTexture, tr, tg, tb);
							// 	break;
							// case SDLK_n:
							// 	tb -= 32;
							// 	SDL_SetTextureColorMod(backgroundSpriteSheet->mTexture, tr, tg, tb);
							// 	break;
							// case SDLK_m:
							// 	tb += 32;
							// 	SDL_SetTextureColorMod(backgroundSpriteSheet->mTexture, tr, tg, tb);
							// 	break;
							case SDLK_ESCAPE:
								engine->quit = 1;
								break;
							case SDLK_LEFT:
							case SDLK_a:
								player->velX = -SPEED;
								break;
							case SDLK_RIGHT:
							case SDLK_d:
								player->velX = SPEED;
								break;
							case SDLK_UP:
							case SDLK_w:
								player->velY = -SPEED;
								break;
							case SDLK_DOWN:
							case SDLK_s:
								player->velY = SPEED;
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
							case SDLK_ESCAPE:
								engine->quit = 1;
								break;
							case SDLK_LEFT:
							case SDLK_a:
								if (player->velX < 0) {
									player->velX = 0;
								}
								break;
							case SDLK_RIGHT:
							case SDLK_d:
								if (player->velX > 0) {
									player->velX = 0;
								}
								break;
							case SDLK_UP:
							case SDLK_w:
								if (player->velY < 0) {
									player->velY = 0;
								}
								break;
							case SDLK_DOWN:
							case SDLK_s:
								if (player->velY > 0) {
									player->velY = 0;
								}
								break;
						}

					}
				}
		}


		// ------------------ UPDATE ------------------
	
		// ###### PLAYER UPDATE ######

		
		// WALKING
		player->isMoving = 1;

		if (player->velX == SPEED) {
			// currentWalk = WALK_RIGHT;
			player->direction = DIR_RIGHT;
		} else if (player->velX == -SPEED) {
			// currentWalk = WALK_LEFT;
			player->direction = DIR_LEFT;
		} else if (player->velY == SPEED) {
			// currentWalk = WALK_DOWN;
			player->direction = DIR_DOWN;
		} else if (player->velY == - SPEED) {
			// currentWalk = WALK_UP;
			player->direction = DIR_UP;
		} else player->isMoving = 0; // walking = 0;
		// ##########################

		
		



		// PLAYER'S VELOCITY
		player->x += player->velX;
		player->y += player->velY;
		player->tileX = getTileX(player, backgroundSpriteSheet->sWidth);
		player->tileY = getTileY(player, backgroundSpriteSheet->sHeight);
		player->tileIndex = player->tileY * level->width + player->tileX;

		// ###### NPCs UPDATE #######
		
		// updateNPC(dogs[0]);
		// updateNPC(dogs[1]);
		// updateNPC(dogs[2]);


		// updateCollisionsNPC(dogs[0], &cam);
		// updateCollisionsNPC(dogs[1], &cam);
		// updateCollisionsNPC(dogs[2], &cam);

		updateCollisionsPlayer(player, &cam);


		// if ( checkCollision(player->col_right, dogs[0]->col_left) ) player->x -= SPEED;
		// if ( checkCollision(player->col_left, dogs[0]->col_right) ) player->x += SPEED;
		// if ( checkCollision(player->col_down, dogs[0]->col_up) ) player->y -= SPEED;
		// if ( checkCollision(player->col_up, dogs[0]->col_down) ) player->y += SPEED;
		


		// ##########################
		
		// ###### CAMERA UPDATE ######
		
		updateCamera(&cam, *player);
		
		// ###########################
		
		if (Mix_PlayingMusic() == 0) {
			Mix_PlayMusic(engine->music, -1);
		}

		
		// --------------------------------------------


		SDL_SetRenderDrawColor(engine->renderer, 0, 0, 0, 255);
		SDL_RenderClear(engine->renderer);
		
		// ------------------ RENDER START ------------------
		
		for (int i = -7; i < 8; i++) {
			for (int j = -5; j < 6; j++) {
				if (
					// Draw only the right tiles
					((player->x + (i * backgroundSpriteSheet->sWidth) + (player->width / 2)) / 64) >= 0 &&
					((player->x + (i * backgroundSpriteSheet->sWidth) + (player->width / 2)) / 64) < level->map->width &&
					((player->y + (j * backgroundSpriteSheet->sHeight) + (player->height / 2)) / 64) >= 0 &&
					((player->y + (j * backgroundSpriteSheet->sHeight) + (player->height / 2)) / 64) < level->map->height
					) {

					for (int l = 0; l < level->layers; l++) {

						if ((player->tileY + j) * level->width + player->tileX + i >= 0)
							
							if ( layersRects[l][(player->tileY + j) * level->width + player->tileX + i].w > 0 ) {
								renderTexture(
									backgroundSpriteSheet,
									engine->renderer,
									&layersRects[l][(player->tileY + j) * level->width + player->tileX + i],
									(( ((player->x + (i * 64) + (player->width / 2)) / 64) % backgroundSpriteSheet->sWidth) * 64) + cam.offsetX,
									(( ((player->y + (j * 64) + (player->height / 2)) / 64) % backgroundSpriteSheet->sHeight) * 64) + cam.offsetY
								);
							}	

					}
				}
			}
		}

		// RENDER PLAYER
		if (player->isMoving == 1) {
			renderTexture(
				playerSpriteSheet,
				engine->renderer,
				&playerWalkingAnimation[player->direction]->frames[nextFrame(playerWalkingAnimation[player->direction])],
				player->x + cam.offsetX,
				player->y + cam.offsetY
			);
		} else {
			renderTexture(
				playerSpriteSheet,
				engine->renderer,
				&playerWalkingAnimation[player->direction]->frames[playerWalkingAnimation[player->direction]->curFrame],
				player->x + cam.offsetX,
				player->y + cam.offsetY
			);
		}

		// NPCs
		// for (int dg = 0; dg < dogsCount; dg++) {
		// 	Animation* curAnim = animations[dg][dogs[dg]->direction];
		// 	SDL_Rect* clip;
		// 	if (dogs[dg]->takingAction == 1)
		// 		clip = &curAnim->frames[nextFrame(curAnim)];
		// 	else
		// 		clip = &curAnim->frames[curAnim->curFrame];
			
		// 	renderTexture(
		// 		curAnim->spriteSheet,
		// 		engine->renderer,
		// 		clip,
		// 		dogs[dg]->x + cam.offsetX,
		// 		dogs[dg]->y + cam.offsetY
		// 	);
		// }



		// Collisiond draw - test

		// SDL_RenderDrawRect(engine->renderer, &dog1->col_up);
		// SDL_RenderDrawRect(engine->renderer, &dog1->col_right);
		// SDL_RenderDrawRect(engine->renderer, &dog1->col_down);
		// SDL_RenderDrawRect(engine->renderer, &dog1->col_left);


		SDL_RenderDrawRect(engine->renderer, &player->col_up);
		SDL_RenderDrawRect(engine->renderer, &player->col_right);
		SDL_RenderDrawRect(engine->renderer, &player->col_down);
		SDL_RenderDrawRect(engine->renderer, &player->col_left);

		// ------------------- RENDER END -------------------
		engineDelay(engine);
		SDL_RenderPresent(engine->renderer);
	}

	/*
	free(level->map);

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

	free(&level->content);
	level->content = NULL;

	free(level);
	level = NULL;
	*/

	// registryRelease();
	engineStop(&engine);

	exit(0);

	return 0;
}
