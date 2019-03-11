#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// #include <lua.h>
// #include <lauxlib.h>
// #include <lualib.h>

#include "main.h"
#include "registry.h"
#include "engine.h"
#include "objects.h"
#include "assetslist.h"
#include "textures.h"


int SPEED = 2;

// FORWARD DECLARATION

int compare(int, int);



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

	// printf("Engine status: %s\n", engine->started == TRUE ? "working." : "start failure.");


	SpriteSheet* playerSpriteSheet = loadSpriteSheet("characters.png", engine->renderer, 16, 16);


	loadMusic(engine, "res/a_funny_moment.mod");

	// SDL_Rect viewPort = { 10, 10, SCREEN_WIDTH - 20, SCREEN_HEIGHT - 20 };

	Player* player = NULL;

	Camera cam;
	cam.vec = setVector(0, 0);

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
	player = resetPlayer(
		"Player",
		96,
		32,
		16,
		16
	);
	SpriteSheet* backgroundSpriteSheet = loadSpriteSheet(
		*level->textureName,
		engine->renderer,
		level->map->tileWidth,
		level->map->tileWidth 
	);


	// SDL_Rect testRect = {0, 0, 64, 64};
	

	// addTextureToAssets(engine->assets, playerSpriteSheet);
	// addTextureToAssets(engine->assets, npc1SpriteSheet);
	// addTextureToAssets(engine->assets, backgroundSpriteSheet);

	
	// printf("Texture name %s, %i:%i\n", *level->textureName, level->map->tileWidth, level->map->tileWidth);

	TextFont* pxText = loadFromRenderedText("Player X", engine->renderer);
	TextFont* pyText = loadFromRenderedText("Player Y", engine->renderer);
	TextFont* cxText = loadFromRenderedText("Cam X", engine->renderer);
	TextFont* cyText = loadFromRenderedText("Cam Y", engine->renderer);


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


	const int npcCount = 15;
	NPC* npcs[npcCount];
	Animation* animations[npcCount][4];
	
	for (int i = 0; i < npcCount; i++) {
		npcs[i] = setNPC(
			10 + (i * 20),
			10 + (i * 16),
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
	
	int displayMode = 0;
	engine->tilesOnScreenFromCenterX = (SCREEN_WIDTH /  (backgroundSpriteSheet->tileWidth * engine->scale) / 2) + 2;
	engine->tilesOnScreenFromCenterY = (SCREEN_HEIGHT /  (backgroundSpriteSheet->tileHeight * engine->scale) / 2) + 2;

	/* ------------------------------ GAME LOOP ------------------------------ */
	while(engine->quit == FALSE) {
		
		
		// engine->frameStart = SDL_GetTicks();
		// SDL_Delay(1000 / 60);


		float px = player->vec.x;
		float py = player->vec.y;
		

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
							case SDLK_PAGEDOWN:
								if (SPEED < 10) SPEED++;
								break;
							case SDLK_PAGEUP:
								if (SPEED > 1) SPEED--;
								break;						
							case SDLK_RETURN:
								player->vec = setVector(0, 0);
								break;
							case SDLK_5:
								player->vec = setVector(16, 16);
								break;
							case SDLK_RSHIFT:
								displayMode++;
								if (displayMode > 2) displayMode = 0;
								break;
							case SDLK_ESCAPE:
								engine->quit = 1;
								break;
							case SDLK_LEFT:
							case SDLK_a:
								player->moveVec.x = -SPEED;								
								break;
							case SDLK_RIGHT:
							case SDLK_d:
								player->moveVec.x = SPEED;
								break;
							case SDLK_UP:
							case SDLK_w:
								player->moveVec.y = -SPEED;
								break;
							case SDLK_DOWN:
							case SDLK_s:
								player->moveVec.y = SPEED;
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


		// ------------------ UPDATE ------------------
		
		// ###### PLAYER UPDATE ######

		for (int i = 0; i < npcCount; i++)
			updateCollisionsNPC(npcs[i], &cam, engine->scale);
		

		updateCollisionsPlayer(player, &cam, engine->scale);

		// WALKING
		player->isMoving = 1;
		if (player->moveVec.x == SPEED) {
			player->direction = DIR_RIGHT;
		} else if (player->moveVec.x == -SPEED) {
			player->direction = DIR_LEFT;
		} else if (player->moveVec.y == SPEED) {
			player->direction = DIR_DOWN;
		} else if (player->moveVec.y == - SPEED) {
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

		
		// ##########################





		/** ------------------- COLLISSIONS -------------- */
		for (int i = 0; i < npcCount; i++)
			updateCollisionsNPC(npcs[i], &cam, engine->scale);
		

		// int npcCollision[npcCount];
		// for (int i = 0; i < npcCount; i++)
		// 	npcCollision[i] = 0;
		
		// ###### NPCs UPDATE #######
		for (int i = 0; i < npcCount; i++) {
		
			updateNPC(npcs[i], level);

			for (int n = i + 1; n < npcCount; n++) {
				SDL_Rect npc1TempRect = {
					( (npcs[i]->vec.x + npcs[i]->moveVec.x ) * engine->scale) - cam.vec.x,
					( (npcs[i]->vec.y + npcs[i]->moveVec.y ) * engine->scale) - cam.vec.y,
					npcs[i]->width * engine->scale,
					npcs[i]->height * engine->scale
				};
				SDL_Rect npc2TempRect = {
					( (npcs[n]->vec.x + npcs[n]->moveVec.x ) * engine->scale) - cam.vec.x,
					( (npcs[n]->vec.y + npcs[n]->moveVec.y ) * engine->scale) - cam.vec.y,
					npcs[n]->width * engine->scale,
					npcs[n]->height * engine->scale
				};
				if (checkCollision(npc1TempRect, npc2TempRect) != 0) {
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
		} else {
			// printf("Collissions!!!\n");	
		}

		// --------------------------------------------


		player->tileX = getTileX(player, backgroundSpriteSheet->tileWidth);
		player->tileY = getTileY(player, backgroundSpriteSheet->tileHeight);
		player->tileIndex = (player->tileY * level->width) + player->tileX;

		// ##########################

		

		// ###### CAMERA UPDATE ######
		updateCamera(&cam, player, level,  engine->scale);
		// ###########################


		// ---------------------- ENGINE UPDATE ----------------------
		engine->tilesOnScreenFromCenterX = (SCREEN_WIDTH /  (backgroundSpriteSheet->tileWidth * engine->scale) / 2) + 2;
		engine->tilesOnScreenFromCenterY = (SCREEN_HEIGHT /  (backgroundSpriteSheet->tileHeight * engine->scale) / 2) + 2;
		if (Mix_PlayingMusic() == 0) {
			Mix_PlayMusic(engine->music, -1);
		}		
		// --------------------------------------------



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
								displayMode
							);
					}
				}
			}
		}

	
		// for (unsigned int i = 0; i < level->size; i++) {
		// 	int x = (grounds[0][i].vec.x * engine->scale) - cam.vec.x;
		// 	int y = (grounds[0][i].vec.y * engine->scale) - cam.vec.y;
		// 	SDL_Rect renderQuad = {x, y, backgroundSpriteSheet->tileWidth * engine->scale, backgroundSpriteSheet->tileHeight * engine->scale};
		// 	SDL_RenderCopy(engine->renderer, backgroundSpriteSheet->mTexture, &layersRects[0][i], &renderQuad);
		// }


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
				displayMode
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
				displayMode
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
				displayMode
			);
		}
		


		/** ---------- TEXT -----------*/

		char str_px[50];
		char str_py[50];
		char str_cx[50];
		char str_cy[50];

		
		if (px != player->vec.x) {
			sprintf(str_px, "%s %.3f", "PX:", player->vec.x);
			changeText(pxText, engine->renderer, str_px);
			sprintf(str_cx, "%s %.3f", "CX:", cam.vec.x);
			changeText(cxText, engine->renderer, str_cx);			
		}
		if (py != player->vec.y) {
			sprintf(str_py, "%s %.3f", "PY:", player->vec.y);
			changeText(pyText, engine->renderer, str_py);
			sprintf(str_cy, "%s %.3f", "CY:", cam.vec.y);
			changeText(cyText, engine->renderer, str_cy);
		}


		renderText(pxText, engine->renderer, 10, 10, 120, 30);
		renderText(pyText, engine->renderer, 10, 30, 120, 30);
		renderText(cxText, engine->renderer, 10, 50, 120, 30);
		renderText(cyText, engine->renderer, 10, 70, 120, 30);



		SDL_SetRenderDrawColor(engine->renderer, 250, 220, 220, 250);
		// Collisiond draw - test
		for (int i = 0; i < npcCount; i++)
			drawNPCCollisions(npcs[i], engine->renderer);

		SDL_SetRenderDrawColor(engine->renderer, 250, 20, 20, 200);
		SDL_RenderDrawRect(engine->renderer, &player->col);

		// ------------------- RENDER END -------------------
		// engineDelay(engine);
		SDL_RenderPresent(engine->renderer);

		// engine->frameEnd = SDL_GetTicks();
		// engine->deltaTime = engine->frameEnd - engine->frameStart;
		
	}

	// ------------------ RELEASING ... ----------------------
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
	

	// registryRelease();
	engineStop(&engine);

	return 0;
}
