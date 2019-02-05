#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "main.h"
#include "engine.h"
#include "textures.h"
#include "objects.h"

#include "arraylist.h"



// FORWARD DECLARATION

void close(SDL_Game* g);

Animation* prepareAnimation(Texture* t, unsigned int speed, unsigned int sw, unsigned int sh, const unsigned int size, unsigned int* frames);
int nextFrame(Animation* an);



void renderText(Texture* t, SDL_Game* game, int x, int y, int w, int h) {
    SDL_Rect renderQuad = {x, y, w, h};
    SDL_RenderCopy(game->gRenderer, t->mTexture, NULL, &renderQuad);
}

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


// SDL_Game* initGame() {
//     SDL_Game* game = malloc(sizeof(SDL_Game));
//     if (game == NULL) return NULL;
//     game->success = 1;
//     if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
//         printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError());
//         game->success = 0;
//     } else {
//         game->gWindow = SDL_CreateWindow("Nevada", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);
//         if (game->gWindow == NULL) {
//             printf( "Window could not be created! SDL Error: %s\n", SDL_GetError());
//             game->success = 0;
//         } else {
//             game->gRenderer = SDL_CreateRenderer(game->gWindow, -1, rendererFlags);
//             if (game->gRenderer == NULL) {
//                 printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
//                 game->success = 0;
//             } else {
//                 SDL_SetRenderDrawColor(game->gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                
//                 int imgFlags = IMG_INIT_PNG;
//                 // INITIALIZE IMGs
//                 if (!(IMG_Init(imgFlags) & imgFlags)) {
//                     printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
//                     game->success = 0;
//                 }

//                 // INITIALIZE AUDIO
//                 if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
//                     printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
//                     game->success = 0;
//                 }

//                 // INITIALIZE TTF
//                 if(TTF_Init() == -1) {
//                     printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
//                     game->success = 0;
//                 }
//             }
//         }
//     }

//     game->gMusic = Mix_LoadMUS("res/ex-aws_cave.xm");
//     if (game->gMusic == NULL) {
//         printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
//         game->success = 0;
//     }

//     return game;
// }












// char* getCurrentTime(void) {
// 	time_t t;
//     time(&t);
// 	return asctime(localtime(&t));
// }


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

	if (argc > 1) {
		for (int i = 1; i < argc; i++)
			printf("Paramers %i: %s\n", i, args[i]);
	}

	// void **ar = malloc(3 * sizeof(void *));

	// ar[0] = (char *) strdup("Some string");
	
	// ar[1] = malloc(sizeof(int));
	// *( (int *)(ar[1]) ) = 5;
	
	// ar[2] = malloc(sizeof(double));
	// *( (double *)(ar[2]) ) = 27.3;

	// printf( "String: %s\n", (char *)(ar[0]) );
	// printf( "Integer: %d\n", *((int *)(ar[1])) );
	// printf( "Double: %f\n", *((double *)(ar[2])) );

	// for (int i = 0; i < 3; i++)
	// 	free(ar[i]);

	// free(ar);
	// getchar();
	// exit(0);

	int arraySize = 4;
	int arrayChunkSize = 3;

	ArrayList_Int* arr = createArrayList(arraySize, arrayChunkSize, sizeof(int), ARRAYLIST_SHRINK_MANUAL);
	
	dumpArrayList(arr);

	addIntToArrayList(arr, 2);

	dumpArrayList(arr);

	addIntToArrayList(arr, 4);

	dumpArrayList(arr);

	addIntToArrayList(arr, 6);
	
	dumpArrayList(arr);

	addIntToArrayList(arr, -3);
	
	dumpArrayList(arr);

	addIntToArrayList(arr, 9);
	
	dumpArrayList(arr);


	printf("Pop ArrayList = %i\n", popArrayList(arr));
	dumpArrayList(arr);
	// shrinkArrayList(arr);
	// dumpArrayList(arr);


	addIntToArrayList(arr, 41);
	dumpArrayList(arr);

	printf("Pop ArrayList = %i\n", popArrayList(arr));
	dumpArrayList(arr);
	// shrinkArrayList(arr);
	// dumpArrayList(arr);

	addIntToArrayList(arr, 44);
	dumpArrayList(arr);

	addIntToArrayList(arr, 47);
	dumpArrayList(arr);

	addIntToArrayList(arr, 48);
	dumpArrayList(arr);

	addIntToArrayList(arr, 49);
	dumpArrayList(arr);

	printf("Pop ArrayList = %i\n", popArrayList(arr));
	dumpArrayList(arr);

	printf("Pop ArrayList = %i\n", popArrayList(arr));
	dumpArrayList(arr);

	printf("Pop ArrayList = %i\n", popArrayList(arr));
	dumpArrayList(arr);

	printf("Pop ArrayList = %i\n", popArrayList(arr));
	dumpArrayList(arr);

	// shrinkArrayList(arr);
	// dumpArrayList(arr);

	getchar();

	if (clearArrayList(&arr)) {
		printf("Array cleared successfully!\n");
	}

	getchar();
	exit(0);



	struct Engine* engine = engineStart();
	
	printf("Engine status: %s\n", engine->started == TRUE ? "working." : "start failure.");
	
	// Player* player = resetPlayer();

	// Camera cam;
	// cam.x = 0;
	// cam.y = 0;
	// cam.offsetX = 0;
	// cam.offsetY = 0;

	// Texture* playerSpriteSheet = loadSpriteSheet("animals1.png", NPC_SPRITESHEET, engine->renderer, 52, 72);

	// Texture* backgroundSpriteSheet = loadSpriteSheet("grassland.png", GRASSLAND1_SPRITESHEET, engine->renderer, 64, 64);

	engine->assets = createAssets();
	
	// if (addGraphicsToAssets(playerSpriteSheet, engine->assets)) {
	// 	printf("Graphics (player) added successfuly!\n");
	// }

	// if (addGraphicsToAssets(backgroundSpriteSheet, engine->assets)) {
	// 	printf("Graphics (background) added successfuly!\n");
	// }

	
	while(engine->quit == FALSE) {
		
		updateDeltaTime(engine);

		while(SDL_PollEvent(&engine->event) != 0) {
				if (engine->event.type == SDL_QUIT) {
					engine->quit = TRUE;
				} else {
					if (engine->event.type == SDL_KEYDOWN) {
						if (engine->event.key.keysym.sym == SDLK_ESCAPE) {
							engine->quit = TRUE;
						}
					}
				}
		}

		SDL_SetRenderDrawColor(engine->renderer, 0, 0, 0, 255);
		SDL_RenderClear(engine->renderer);
		
		// ------------------ RENDER START ------------------



		// ------------------- RENDER END -------------------
		engineDelay(engine);
		SDL_RenderPresent(engine->renderer);
	}

	engineStop(engine);
	exit(666);


	// printf("Parameters: %s\n", getFromResourceImagesDir("map.tmx"));

	// SDL_Game* game = initGame();
	// if (!game) {
	// 	printf("ERROR!\n");
	// 	exit(1);
	// } else {
	// 	// printf("Initialization SDL - OK!\nGame.success -> %i\n", game->success);		

	// 	// LEVEL STUFF...
	// 	Level* level = getLevel();
		
	// 	// printf("This level consists of %i layer(s).\n", level->layers);
	// 	// printf("This level consists of %i tilesets.\n", level->map->tileSetCount);

	// 	Texture* playerSpriteSheet = loadSpriteSheet("animals1.png", game, 52, 72);
	// 	// printf("Player spritesheet Width: %i, height: %i\n", playerSpriteSheet->width, playerSpriteSheet->height);

	// 	Texture* backgroundSpriteSheet = loadSpriteSheet("grassland.png", game, 64, 64);
	// 	// printf("Background spritesheet Width: %i, height: %i\n", backgroundSpriteSheet->width, backgroundSpriteSheet->height);

	// 	// Texture* font1 = loadFromRenderedText("THE TEMPLE OF THE LOST PUPPY", game);

	// 	// PLAYER
	// 	Player* player = resetPlayer();

	// 	Camera cam;
	// 	cam.x = 0;
	// 	cam.y = 0;
	// 	cam.offsetX = 0;
	// 	cam.offsetY = 0;
		
	// 	SDL_Rect* layersRects[level->layers];
	// 	for (int i = 0; i < level->layers; i++) {
	// 		layersRects[i] = createRectsForSprites(level, i, level->size, backgroundSpriteSheet);
	// 	}

	// 	short quit = 0;

	// 	// DOGS
	// 	// NPC dogs[] = {
	// 	// 	{650, 280, 0, 0, playerSpriteSheet->sWidth, playerSpriteSheet->sHeight},
	// 	// 	{400, 210, 0, 0, playerSpriteSheet->sWidth, playerSpriteSheet->sHeight},
	// 	// 	{500, 290, 0, 0, playerSpriteSheet->sWidth, playerSpriteSheet->sHeight},
	// 	// 	{400, 360, 0, 0, playerSpriteSheet->sWidth, playerSpriteSheet->sHeight},
	// 	// };

	// 	// unsigned int framesDog1[] = {73, 74, 75};
	// 	// unsigned int framesDog2[] = {19, 20, 21};
	// 	// unsigned int framesDog3[] = {22, 23, 24};
	// 	// unsigned int framesDog4[] = {34, 35, 36};

	// 	// Animation* dogsAnim[] = {
	// 	// 	prepareAnimation(playerSpriteSheet, 1, player->width, player->height, 3, framesDog1),
	// 	// 	prepareAnimation(playerSpriteSheet, 3, player->width, player->height, 3, framesDog2),
	// 	// 	prepareAnimation(playerSpriteSheet, 6, player->width, player->height, 3, framesDog3),
	// 	// 	prepareAnimation(playerSpriteSheet, 9, player->width, player->height, 3, framesDog4),
	// 	// };

	// 	// ANIMATIONS
	// 	Animation* walkingAnimation[4];

	// 	unsigned int framesPlayerLeft[]  = {16, 17, 18};
	// 	unsigned int framesPlayerRight[] = {28, 29, 30};
	// 	unsigned int framesPlayerUp[]    = {40, 41, 42};
	// 	unsigned int framesPlayerDown[]  = {4,  5,  6};

	// 	walkingAnimation[WALK_UP] = prepareAnimation(playerSpriteSheet, 6, player->width, player->height, 3, framesPlayerUp);
    //     walkingAnimation[WALK_RIGHT] = prepareAnimation(playerSpriteSheet, 6, player->width, player->height, 3, framesPlayerRight);
    //     walkingAnimation[WALK_DOWN] = prepareAnimation(playerSpriteSheet, 6, player->width, player->height, 3, framesPlayerDown);
    //     walkingAnimation[WALK_LEFT] = prepareAnimation(playerSpriteSheet, 6, player->width, player->height, 3, framesPlayerLeft);		

	// 	int walking = 0;
	// 	int currentWalk = WALK_LEFT;
	// 	walkingAnimation[currentWalk]->curFrame = 1;

	// 	SDL_Event e;
		
	// 	while(!quit) {
			
	// 		// updateDeltaTime();
	// 		/**
	// 		 * #################################################
	// 		 * ..................... INPUT .....................
	// 		 * #################################################
	// 		 * */
	// 		while(SDL_PollEvent(&e) != 0) {
	// 			if (e.type == SDL_QUIT) {
	// 				quit = 1;
	// 			} else {
	// 				if (e.type == SDL_KEYDOWN) {
	// 					if (e.key.keysym.sym == SDLK_ESCAPE) {
	// 						quit = 1;
	// 					}
	// 					if (e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_a) {
	// 						player->velX = -SPEED;
	// 					}
	// 					if (e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_d) {
	// 						player->velX = SPEED;
	// 					}
	// 					if (e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_w) {
	// 						player->velY = -SPEED;
	// 					}
	// 					if (e.key.keysym.sym == SDLK_DOWN || e.key.keysym.sym == SDLK_s) {
	// 						player->velY = SPEED;
	// 					}
	// 					if (e.key.keysym.sym == SDLK_SPACE) {
    //                         if (Mix_PlayingMusic() == 0) {
    //                             Mix_PlayMusic(game->gMusic, -1);
    //                         } else {
    //                             if (Mix_PausedMusic() == 1) {
    //                                 Mix_ResumeMusic();
    //                             } else {
    //                                 Mix_PauseMusic();
    //                             }
    //                         }
	// 					}
	// 				}
	// 				if (e.type == SDL_KEYUP) {
	// 					if (e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_d) {
	// 						if (player->velX > 0) {
	// 							player->velX = 0;
	// 						}
	// 					}
	// 					if (e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_a) {
	// 						if (player->velX < 0) {
	// 							player->velX = 0;
	// 						}
	// 					}
	// 					if (e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_w) {
	// 						if (player->velY < 0) {
	// 							player->velY = 0;
	// 						}
	// 					}
	// 					if (e.key.keysym.sym == SDLK_DOWN || e.key.keysym.sym == SDLK_s) {
	// 						if (player->velY > 0) {
	// 							player->velY = 0;
	// 						}
	// 					}
	// 				}
	// 			}
	// 		}

	// 		/**
	// 		 * #################################################
	// 		 * .................... UPDATE .....................
	// 		 * #################################################
	// 		 * */
		
	// 		// PLAYER'S VELOCITY
	// 		player->x += player->velX;
	// 		player->y += player->velY;
			
	// 		// WALKING
	// 		walking = 1;
	// 		if (player->velX == SPEED) {
	// 			currentWalk = WALK_RIGHT;
	// 		} else if (player->velX == -SPEED) {
	// 			currentWalk = WALK_LEFT;
	// 		} else if (player->velY == SPEED) {
	// 			currentWalk = WALK_DOWN;
	// 		} else if (player->velY == - SPEED) {
	// 			currentWalk = WALK_UP;
	// 		} else walking = 0;


	// 		updateCamera(&cam, *player);

	// 		if (Mix_PlayingMusic() == 0) {
	// 			Mix_PlayMusic(game->gMusic, -1);
	// 		}



	// 		/**
	// 		 * #################################################
	// 		 * .................... RENDER .....................
	// 		 * #################################################
	// 		 * */
	// 		SDL_SetRenderDrawColor(game->gRenderer, 0x1F, 0x1F, 0x1F, 0xFF);
	// 		SDL_RenderClear(game->gRenderer);


	// 		player->tileX = getTileX(player, backgroundSpriteSheet->sWidth);
	// 		player->tileY = getTileY(player, backgroundSpriteSheet->sHeight);
	// 		player->tileIndex = player->tileY * level->width + player->tileX;


	// 		for (int i = -7; i < 8; i++) {
	// 			for (int j = -5; j < 6; j++) {
	// 				if (
	// 					// Draw only the right tiles
	// 				  	((player->x + (i * 64) + (player->width / 2)) / 64) >= 0 &&
	// 					((player->x + (i * 64) + (player->width / 2)) / 64) < level->map->width &&
	// 					((player->y + (j * 64) + (player->height / 2)) / 64) >= 0 &&
	// 					((player->y + (j * 64) + (player->height / 2)) / 64) < level->map->height
	// 				  	) {

	// 					for (int t = 0; t < level->layers; t++) {
	// 						renderTexture(
	// 							backgroundSpriteSheet,
	// 							game,
	// 							&layersRects[t][(player->tileY + j) * level->width + player->tileX + i],
	// 							(( ((player->x + (i * 64) + (player->width / 2)) / 64) % backgroundSpriteSheet->sWidth) * 64) + cam.offsetX,
	// 							(( ((player->y + (j * 64) + (player->height / 2)) / 64) % backgroundSpriteSheet->sHeight) * 64) + cam.offsetY,
	// 							backgroundSpriteSheet->sWidth,
	// 							backgroundSpriteSheet->sHeight
	// 						);
	// 					}

	// 				}
	// 			}
	// 		}

	// 		// NPCs
	// 		// for (int i = 0; i < 4; i++)
	// 		// 	renderTexture(playerSpriteSheet, game, &dogsAnim[i]->frames[nextFrame(dogsAnim[i])], dogs[i].x + cam.offsetX, dogs[i].y + cam.offsetY, playerSpriteSheet->sWidth, playerSpriteSheet->sHeight);

	// 		// RENDER PLAYER
	// 		if (walking == 1) {
	// 			renderTexture(
	// 				playerSpriteSheet,
	// 				game,
	// 				&walkingAnimation[currentWalk]->frames[nextFrame(walkingAnimation[currentWalk])],
	// 				player->x + cam.offsetX,
	// 				player->y + cam.offsetY,
	// 				playerSpriteSheet->sWidth,
	// 				playerSpriteSheet->sHeight
	// 			);
	// 		} else {
	// 			renderTexture(
	// 				playerSpriteSheet,
	// 				game,
	// 				&walkingAnimation[currentWalk]->frames[walkingAnimation[currentWalk]->curFrame],
	// 				player->x + cam.offsetX,
	// 				player->y + cam.offsetY,
	// 				playerSpriteSheet->sWidth,
	// 				playerSpriteSheet->sHeight
	// 			);
	// 		}
			
	// 		// renderText(font1, game, 100, 50, 400, 50);

	// 		// FPS LIMIT
	// 		// float frameRate = 1000.0f / FPS_MAX;
    //     	// if (deltaTime < frameRate)
    //         // SDL_Delay ((int)(frameRate - deltaTime));

	// 		SDL_RenderPresent(game->gRenderer);
	// 	}


	// 	freeTexture(playerSpriteSheet);
	// 	playerSpriteSheet = NULL;

	// 	freeTexture(backgroundSpriteSheet);
	// 	backgroundSpriteSheet = NULL;

	// 	// freeTexture(font1);
	// 	// font1 = NULL;

	// 	freeTiledMap(level->map);
	// 	//free(level->content);
	// 	level->content = NULL;
	// 	free(level);
	// 	level = NULL;

	// 	close(game);
	// 	free(game);
	// 	game = NULL;
	// }

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
