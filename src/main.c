#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>


#include "main.h"
#include "game.h"
#include "textures.h"
#include "level.h"





// FORWARD DECLARATION
char* getCurrentTime();
void close(SDL_Game* g);
Player* resetPlayer(Texture* t);
void UpdateDeltaTime();



unsigned long int startTick, endTick;
unsigned short int fpsCounter;
float delayTime = 0.0f;
float deltaTime = 0.0f;
unsigned short int fps = 0;


void UpdateDeltaTime() {
    startTick = SDL_GetPerformanceCounter();
    deltaTime = (float)(startTick - endTick) / SDL_GetPerformanceFrequency();
    endTick = startTick;
    fpsCounter++;
    delayTime += deltaTime;
    if (delayTime >= 1.0f) {
        delayTime -= 1.0f;
        fps = fpsCounter;
        fpsCounter = 0;
    }
}


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

void print_list(LinkedList* list) {
    ListItem* current = list->lastItem;
	int c = 1;
	while (current != NULL) {
        printf("Item no: %i, val = %d, previous pointer: %p\n", c, current->val, &current->next);
        current = current->next;
		c++;
    }
}

bool addToList(LinkedList* list, ListItem* item) {
	printf("Add to list ... value: %i, next pointer: %p \n", item->val, &item->next);	
	if (list->size == 0) {
		list->lastItem = NULL;
		list->lastItem = malloc(sizeof(LinkedList));
		if (list->lastItem == NULL) return FALSE;

		list->lastItem->val = item->val;
		list->lastItem->next = item->next;
	}

	list->size++;
	printf("List size: %i\n", list->size);
	return TRUE;
}


void luaTest() {
    int status, result, i;
    double sum;
    lua_State *L;

    /*
     * All Lua contexts are held in this structure. We work with it almost
     * all the time.
     */
    L = luaL_newstate();

    luaL_openlibs(L); /* Load Lua libraries */

    /* Load the file containing the script we are going to run */
    status = luaL_loadfile(L, "res/script.lua");
    if (status) {
        /* If something went wrong, error message is at the top of */
        /* the stack */
        fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(L, -1));
        exit(1);
    }

    /*
     * Ok, now here we go: We pass data to the lua script on the stack.
     * That is, we first have to prepare Lua's virtual stack the way we
     * want the script to receive it, then ask Lua to run it.
     */
    lua_newtable(L);    /* We will pass a table */

    /*
     * To put values into the table, we first push the index, then the
     * value, and then call lua_rawset() with the index of the table in the
     * stack. Let's see why it's -3: In Lua, the value -1 always refers to
     * the top of the stack. When you create the table with lua_newtable(),
     * the table gets pushed into the top of the stack. When you push the
     * index and then the cell value, the stack looks like:
     *
     * <- [stack bottom] -- table, index, value [top]
     *
     * So the -1 will refer to the cell value, thus -3 is used to refer to
     * the table itself. Note that lua_rawset() pops the two last elements
     * of the stack, so that after it has been called, the table is at the
     * top of the stack.
     */
    for (i = 1; i <= 10; i++) {
        lua_pushnumber(L, i);   /* Push the table index */
        lua_pushnumber(L, i*2); /* Push the cell value */
        lua_rawset(L, -3);      /* Stores the pair in the table */
    }

    /* By what name is the script going to reference our table? */
    lua_setglobal(L, "foo");

    /* Ask Lua to run our little script */
    result = lua_pcall(L, 0, LUA_MULTRET, 0);
    if (result) {
        fprintf(stderr, "Failed to run script: %s\n", lua_tostring(L, -1));
        exit(1);
    }

    /* Get the returned value at the top of the stack (index -1) */
    sum = lua_tonumber(L, -1);

    printf("Script returned: %.0f\n", sum);

    lua_pop(L, 1);  /* Take the returned value out of the stack */
    lua_close(L);
}


int main(int argc, char* args[]) {

	// LinkedList list = {0, NULL};

	luaTest();


	SDL_Game* game = initGame();
	if (!game) {
		printf("ERROR!\n");
		exit(1);
	} else {
		printf("Initialization SDL - OK!\nGame.success -> %i\n", game->success);		

		// LEVEL STUFF...
		Level* level = getLevel(0);
		
		printf("This level consists of %i layer(s).\n", level->layers);
		printf("This level consists of %i tilesets.\n", level->map->tileSetCount);

		Texture* playerSpriteSheet = loadSpriteSheet("animals1.png", game, 52, 72);
		printf("Player spritesheet Width: %i, height: %i\n", playerSpriteSheet->width, playerSpriteSheet->height);

		Texture* backgroundSpriteSheet = loadSpriteSheet("grassland.png", game, 64, 64);
		printf("Background spritesheet Width: %i, height: %i\n", backgroundSpriteSheet->width, backgroundSpriteSheet->height);

		Texture* font1 = loadFromRenderedText("THE TEMPLE OF THE LOST PUPPY", game);

		// PLAYER
		Player* player = resetPlayer(playerSpriteSheet);

		Camera cam;
		cam.x = 0;
		cam.y = 0;
		cam.offsetX = 0;
		cam.offsetY = 0;

		short walking = 0;
		short currentWalk = WALK_LEFT;
		
		SDL_Rect* layersRects[level->layers];
		for (int i = 0; i < level->layers; i++) {
			layersRects[i] = createRectsForSprites(level, i, level->size, backgroundSpriteSheet);
		}

		short quit = 0;

		// DOGS
		// NPC dogs[] = {
		// 	{650, 280, 0, 0, playerSpriteSheet->sWidth, playerSpriteSheet->sHeight},
		// 	{400, 210, 0, 0, playerSpriteSheet->sWidth, playerSpriteSheet->sHeight},
		// 	{500, 290, 0, 0, playerSpriteSheet->sWidth, playerSpriteSheet->sHeight},
		// 	{400, 360, 0, 0, playerSpriteSheet->sWidth, playerSpriteSheet->sHeight},
		// };

		// int framesDog1[] = {73, 74, 75};
		// int framesDog2[] = {19, 20, 21};
		// int framesDog3[] = {22, 23, 24};
		// int framesDog4[] = {34, 35, 36};

		// Animation* dogsAnim[] = {
		// 	prepareAnimation(playerSpriteSheet, 1, player->width, player->height, 3, framesDog1),
		// 	prepareAnimation(playerSpriteSheet, 3, player->width, player->height, 3, framesDog2),
		// 	prepareAnimation(playerSpriteSheet, 6, player->width, player->height, 3, framesDog3),
		// 	prepareAnimation(playerSpriteSheet, 9, player->width, player->height, 3, framesDog4),
		// };


		unsigned int framesPlayerLeft[] = {16, 17, 18};
		Animation* walkingLeftAnimation = prepareAnimation(playerSpriteSheet, 6, player->width, player->height, 3, framesPlayerLeft);

		unsigned int framesPlayerRight[] = {28, 29, 30};
		Animation* walkingRightAnimation = prepareAnimation(playerSpriteSheet, 6, player->width, player->height, 3, framesPlayerRight);

		unsigned int framesPlayerUp[] = {40, 41, 42};
		Animation* walkingUpAnimation = prepareAnimation(playerSpriteSheet, 6, player->width, player->height, 3, framesPlayerUp);

		unsigned int framesPlayerDown[] = {4, 5, 6};
		Animation* walkingDownAnimation = prepareAnimation(playerSpriteSheet, 6, player->width, player->height, 3, framesPlayerDown);


		Animation* currentWalkAnim = walkingLeftAnimation;
		// Hardcoded first frame of the player, without this line the dog is invisible at the beginning
		currentWalkAnim->curFrame = 1;


		SDL_Event e;
		
		while(!quit) {
			
			UpdateDeltaTime();

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

			currentWalk = NO_WALK;
			walking = 0;

			if (player->velX == SPEED) {
				currentWalk = WALK_RIGHT;
			} else if (player->velX == -SPEED) {
				currentWalk = WALK_LEFT;
			}

			if (player->velY == SPEED) {
				currentWalk = WALK_DOWN;
			} else if (player->velY == - SPEED) {
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
					if (
						// Draw only the right tiles
					  	((player->x + (i * 64) + (player->width / 2)) / 64) >= 0 &&
						((player->x + (i * 64) + (player->width / 2)) / 64) < level->map->width &&
						((player->y + (j * 64) + (player->height / 2)) / 64) >= 0 &&
						((player->y + (j * 64) + (player->height / 2)) / 64) < level->map->height
					  	) {

						for (int t = 0; t < level->layers; t++) {
							renderTexture(
								backgroundSpriteSheet,
								game,
								&layersRects[t][(player->tileY + j) * level->width + player->tileX + i],
								(( ((player->x + (i * 64) + (player->width / 2)) / 64) % backgroundSpriteSheet->sWidth) * 64) + cam.offsetX,
								(( ((player->y + (j * 64) + (player->height / 2)) / 64) % backgroundSpriteSheet->sHeight) * 64) + cam.offsetY,
								backgroundSpriteSheet->sWidth,
								backgroundSpriteSheet->sHeight
							);
						}

					}
				}
			}

			// NPCs
			// for (int i = 0; i < 4; i++)
			// 	renderTexture(playerSpriteSheet, game, &dogsAnim[i]->frames[nextFrame(dogsAnim[i])], dogs[i].x + cam.offsetX, dogs[i].y + cam.offsetY, playerSpriteSheet->sWidth, playerSpriteSheet->sHeight);


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

			// FPS LIMIT
			float frameRate = 1000.0f / FPS_MAX;
        	if (deltaTime < frameRate)
            SDL_Delay ((int)(frameRate - deltaTime));

			SDL_RenderPresent(game->gRenderer);
		}


		freeTexture(playerSpriteSheet);
		playerSpriteSheet = NULL;

		freeTexture(backgroundSpriteSheet);
		backgroundSpriteSheet = NULL;

		freeTexture(font1);
		font1 = NULL;

		freeTiledMap(level->map);
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
