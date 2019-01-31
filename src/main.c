#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <libxml/parser.h>
#include <libxml/xmlIO.h>
#include <libxml/xinclude.h>
#include <libxml/tree.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "main.h"

// RENDERER FLAGS
static Uint32 rendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
static Uint32 windowFlags = SDL_WINDOW_SHOWN;


// FORWARD DECLARATION
char* getCurrentTime(void);
void close(SDL_Game* g);
Player* resetPlayer(Texture* t);
void UpdateDeltaTime();
SDL_Game* initGame();
Level* getLevel(short n);
void updateCamera(Camera* c, Player player);
int getTileX(Player* p, Camera* c, Level* l, unsigned int tw);
int getTileY(Player* p, Camera* c, Level* l, unsigned int t);
Animation* prepareAnimation(Texture* t, unsigned int speed, unsigned int sw, unsigned int sh, const unsigned int size, unsigned int* frames);
int nextFrame(Animation* an);
int xmlCharToInt(const xmlChar a[]);
int* parseData(xmlDocPtr doc, xmlNodePtr cur);
TiledMap* parseMap(const char* fileName);
void freeTiledMap(TiledMap* tiledMap);
char* copyStringFromPointer(const char* s);
int fromBinary(const char *s);



unsigned long int startTick, endTick;
unsigned short int fpsCounter;
float delayTime = 0.0f;
float deltaTime = 0.0f;
unsigned short int fps = 0;



char* copyStringFromPointer(const char* s) {
	int sl = strlen(s) + 1;
	char *stringcopy = malloc(sl);
	if (stringcopy) strcpy(stringcopy, s);
	else fprintf(stderr, "malloc failure!");
	return stringcopy;
}



int fromBinary(const char *s) {
  return (int) strtol(s, NULL, 2);
}


/**
 * Release memory.
 * */
void freeTiledMap(TiledMap* tiledMap) {	
	// free Layers
	for (int i = 0; i < tiledMap->layersCount; i++) {
		free(tiledMap->layer[i].encoding);
		free(tiledMap->layer[i].name);
		free(tiledMap->layer[i].data);
		tiledMap->layer[i].height = 0;
		tiledMap->layer[i].width = 0;
		tiledMap->layer[i].id = 0;
	}

	// free TileSets
	for (int i = 0; i < tiledMap->tileSetCount; i++) {
		free(tiledMap->tileSet[i].source);
		free(tiledMap->tileSet[i].tileSetSource->name);
		free(tiledMap->tileSet[i].tileSetSource->imageSource);
		free(tiledMap->tileSet[i].tileSetSource);

		tiledMap->tileSet[i].firstGid = 0;
		tiledMap->tileSet[i].tileSetSource->columns = 0;
		tiledMap->tileSet[i].tileSetSource->tileCount = 0;
		tiledMap->tileSet[i].tileSetSource->tileHeight = 0;
		tiledMap->tileSet[i].tileSetSource->tileWidth = 0;
	}
} 

int xmlCharToInt(const xmlChar a[]) {
	int c = 0, sign = 0, offset = 0, n = 0;
	if (a[0] == '-') {
		sign = -1;
	}
	if (sign == -1) {
		offset = 1;
	} else {
		offset = 0;
	}
  	n = 0;
  	for (c = offset; a[c] != '\0'; c++) {
		  n = n * 10 + a[c] - '0';
	}
	if (sign == -1) {
		n = -n;
	}
	return n;
}

int stringToInt(const char a[]) {
	int c = 0, sign = 0, offset = 0, n = 0;
	if (a[0] == '-') {
		sign = -1;
	}
	if (sign == -1) {
		offset = 1;
	} else {
		offset = 0;
	}
  	n = 0;
  	for (c = offset; a[c] != '\0'; c++) {
		  n = n * 10 + a[c] - '0';
	}
	if (sign == -1) {
		n = -n;
	}
	return n;
}

int* convertDataStringToArray(const xmlChar* s) {
	unsigned int numbers = 0;
	int c = 0;
	int cleanCharsNumber = 0;
	// Counting comas
	while(s[c] != '\0') {
		if (s[c] == ',') numbers++;
		// CHECK CR/LF
		if (s[c] != 13 && s[c] != 32 && s[c] != 10) cleanCharsNumber++;
		c++;
	}
	// How many numbers = comas + 1
	numbers++;
	// printf("Chars: %i, Numbers: %u, cleanCharsNumber: %u\n", c, numbers, cleanCharsNumber);
	c = 0;
	char cleanCharsArray[cleanCharsNumber];
	cleanCharsNumber = 0;
	while(s[c] != '\0') {
		// CHECK CR/LF
		if (s[c] != 13 && s[c] != 32 && s[c] != 10) {
			cleanCharsArray[cleanCharsNumber] = s[c];
			cleanCharsNumber++;
		}
		c++;
	}

	//printf("Clean chars string:\n%s\n", cleanCharsArray);
	c = 0;

	char delim[] = ",";
	char *ptr = strtok(cleanCharsArray, delim);
	int* numArr = malloc(sizeof(int) * numbers);
	if (numArr == NULL) {
		printf("Malloc 'numArr' error !!!\n");
		return NULL;
	}
	while(ptr != NULL) {
		//printf("Number: %s\n", ptr);
		numArr[c] = stringToInt(ptr);
		ptr = strtok(NULL, delim);
		c++;
	}

	return numArr;
}

int* parseData(xmlDocPtr doc, xmlNodePtr cur) {
	xmlChar* key;
	// xmlChar* encoding;
	cur = cur->xmlChildrenNode;
	int* arr = NULL;
	while (cur != NULL) {
	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"data"))) {
			// encoding = xmlGetProp(cur,  (const xmlChar *) "encoding");
			//printf("encoding: %s\n", encoding);
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			arr = convertDataStringToArray(key);
			//printf("Array: %i, %i, %i, %i\n", arr[0], arr[1], arr[2], arr[3]);
		    xmlFree(key);
 	    }
		cur = cur->next;
	}
    return arr;
}

TileSetSource* getTileSetSource(const char* tsxFileName) {
	
	TileSetSource* tileSetSource = malloc(sizeof(TileSetSource));
	if (tileSetSource == NULL) {
		printf("Malloc (creating TileSetSource) error !!!\n");
		return NULL;
	}
	char tmp[50] = DIR_RES_IMAGES;
	strcat(tmp, tsxFileName);
	//printf("Parsing tsx file: %s\n", tmp);
	
	xmlDocPtr tsxDoc;
	xmlNodePtr tsxCurNode;

	tsxDoc = xmlParseFile(tmp);
	if (tsxDoc == NULL) {
		fprintf(stderr, "Document not parsed successfully.\n");
        exit(0);
    }
	
	tsxCurNode = xmlDocGetRootElement(tsxDoc);
    if (tsxCurNode == NULL) {
		fprintf(stderr,"empty document\n");
		xmlFreeDoc(tsxDoc);
		exit(0);
	}
	
	if (xmlStrcmp(tsxCurNode->name, (const xmlChar *) "tileset")) {
		fprintf(stderr,"document of the wrong type, root node != map !!! \n");
		xmlFreeDoc(tsxDoc);
		exit(0);
	}
	tileSetSource->name = (char *)xmlGetProp(tsxCurNode, (const xmlChar *) "name");
	tileSetSource->tileWidth = xmlCharToInt(xmlGetProp(tsxCurNode, (const xmlChar *) "tilewidth"));
	tileSetSource->tileHeight = xmlCharToInt(xmlGetProp(tsxCurNode, (const xmlChar *) "tileheight"));
	tileSetSource->tileCount = xmlCharToInt(xmlGetProp(tsxCurNode, (const xmlChar *) "tilecount"));
	tileSetSource->columns = xmlCharToInt(xmlGetProp(tsxCurNode, (const xmlChar *) "columns"));

	//printf("TileSet props: name: %s, tileWidth: %i, tileHeight: %i, tileCount: %i, columns: %i\n", tileSetSource->name, tileSetSource->tileWidth, tileSetSource->tileHeight, tileSetSource->tileCount, tileSetSource->columns);

	tsxCurNode = tsxCurNode->xmlChildrenNode;
	while (tsxCurNode != NULL) {
		if ((!xmlStrcmp(tsxCurNode->name, (const xmlChar *) "image"))) {
			tileSetSource->imageSource= (char *) xmlGetProp(tsxCurNode, (const xmlChar *) "source");
			tileSetSource->width = xmlCharToInt(xmlGetProp(tsxCurNode, (const xmlChar *) "width"));
			tileSetSource->height = xmlCharToInt(xmlGetProp(tsxCurNode, (const xmlChar *) "height"));
		}
		tsxCurNode = tsxCurNode->next;
	}

	//printf("TileSetImage props: source: %s, width: %i, height: %i\n", tileSetSource->imageSource, tileSetSource->width, tileSetSource->height);
	
	return tileSetSource;
}

TiledMap* parseMap(const char* fileName) {
	//printf("Parsing tmx %s file.\n", fileName);
	
	xmlDocPtr doc;
	xmlNodePtr cur;

	doc = xmlParseFile(fileName);
    if (doc == NULL) {
		fprintf(stderr,"Document not parsed successfully.\n");
        exit(0);
    }

    cur = xmlDocGetRootElement(doc);
    
    if (cur == NULL) {
		fprintf(stderr,"empty document\n");
		xmlFreeDoc(doc);
		exit(0);
	}

    if (xmlStrcmp(cur->name, (const xmlChar *) "map")) {
		fprintf(stderr,"document of the wrong type, root node != map !!! \n");
		xmlFreeDoc(doc);
		exit(0);
	}

	TiledMap* tiledMap = malloc(sizeof (TiledMap));
	if (tiledMap == NULL) {
		printf("Malloc (creating TiledMap) error !!!\n");
		return NULL;
	}
	tiledMap->width = xmlCharToInt(xmlGetProp(cur, (const xmlChar *) "width"));
	tiledMap->height = xmlCharToInt(xmlGetProp(cur, (const xmlChar *) "height"));
	tiledMap->tileWidth = xmlCharToInt(xmlGetProp(cur, (const xmlChar *) "tilewidth"));
	tiledMap->tileHeight = xmlCharToInt(xmlGetProp(cur, (const xmlChar *) "tileheight"));

    cur = cur->xmlChildrenNode;

	int layersCount = 0;
	int tileSetCount = 0;
	
	xmlNodePtr firstForCounter = cur;
	
	// Counting layers and tilesets
	while(firstForCounter != NULL) {
		if ((!xmlStrcmp(firstForCounter->name, (const xmlChar *)"tileset"))) {
			tileSetCount++;
		}
		if ((!xmlStrcmp(firstForCounter->name, (const xmlChar *)"layer"))) {
			layersCount++;
		}
		firstForCounter = firstForCounter->next;
	}
	
	//printf("Layers count: %i, tileset count: %i\n", layersCount, tileSetCount);

	TileSet* tileSet = malloc(sizeof(TileSet) * tileSetCount);
	if (tileSet == NULL) {
		printf("Malloc (creating TileSet) error !!!\n");
		return NULL;
	}

	int lc = 0, tc = 0;

	Layer* layers = malloc(sizeof(Layer) * layersCount);
	if (layers == NULL) {
		printf("Malloc (creating Layer) error !!!\n");
		return NULL;
	}


    while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"tileset"))) {
			tileSet[tc].firstGid = xmlCharToInt(xmlGetProp(cur, (const xmlChar *) "firstgid"));
			tileSet[tc].source = (char *)xmlGetProp(cur, (const xmlChar *) "source");
			//printf("Tileset: firstgid=%i, source=%s\n", tileSet[tc].firstGid, tileSet[tc].source);
			//printf("Parsing tileSet file: %s.\n", tileSet[tc].source);
			TileSetSource* tss = getTileSetSource(tileSet[tc].source);
			tileSet[tc].tileSetSource = tss;
			tc++;
		}
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"layer"))) {
			layers[lc].id = xmlCharToInt(xmlGetProp(cur,  (const xmlChar *) "id"));
			layers[lc].name = (char *)xmlGetProp(cur,  (const xmlChar *) "name");
			layers[lc].width = xmlCharToInt(xmlGetProp(cur,  (const xmlChar *) "width"));
			layers[lc].height = xmlCharToInt(xmlGetProp(cur,  (const xmlChar *) "height"));
			// printf("Layer %i id: %i\n", lc, layers[lc].id);
			// printf("Layer %i name: %s\n", lc, layers[lc].name);
			// printf("Layer %i width: %i\n", lc, layers[lc].width);
			// printf("Layer %i height: %i\n", lc, layers[lc].height);
			layers[lc].data = parseData(doc, cur);
			lc++;
		}
	    cur = cur->next;
	}
	tiledMap->tileSet = tileSet;
	tiledMap->layer = layers;
	tiledMap->layersCount = lc;
	tiledMap->tileSetCount = tc;

	return tiledMap;
}


void freeTexture(Texture* t) {
    if (t->mTexture != NULL) {
        SDL_DestroyTexture(t->mTexture);
        t->mTexture = NULL;
        free(t);
    }
}


void renderTexture(Texture* t, SDL_Game* game, SDL_Rect* clip, int x, int y, unsigned int width, unsigned int height) {
    SDL_Rect renderQuad = {x, y, width, height};
    SDL_RenderCopy(game->gRenderer, t->mTexture, clip, &renderQuad);
}


Texture* loadSpriteSheet(char* fileName, SDL_Game* game, unsigned int spriteWidth, unsigned int spriteHeigth) {
    Texture* t = malloc(sizeof(Texture));
    SDL_Texture* newTexture = NULL;

    char str[50] = DIR_RES_IMAGES;
    const char *strFrom = fileName;
    strcat (str, strFrom);

    SDL_Surface* loadedSurface = IMG_Load(str);
    if (loadedSurface == NULL) {
        printf( "Unable to load image %s! SDL_image Error: %s\n", str, IMG_GetError());
        t = NULL;
    } else {
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
        newTexture = SDL_CreateTextureFromSurface(game->gRenderer, loadedSurface);
        if (newTexture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", str, SDL_GetError());
        } else {
            t->width = loadedSurface->w;
            t->height = loadedSurface->h;
        }
        SDL_FreeSurface(loadedSurface);
    }
    t->mTexture = newTexture;
    t->sWidth = spriteWidth;
    t->sHeight = spriteHeigth;
    return t;
}


SDL_Rect* getSpriteI(Texture* t, int index, unsigned int width, unsigned int height) {
    SDL_Rect* r = malloc(sizeof(SDL_Rect));
    int col = t->width / width;
    r->x = ((index - 1) % col) * width;
    r->y = ((index - 1) / col) * height;
    r->w = width;
    r->h = height;
    return r;
}

SDL_Rect* createRectsForSprites(Level* level, int layerCount, const unsigned int size, Texture* t) {
    SDL_Rect* l = malloc(sizeof(SDL_Rect) * size);
    for (int i = 0; i < level->size; i++)
        l[i] = *getSpriteI(t, level->content[layerCount].data[i], t->sWidth, t->sHeight);
    return l;
}

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


SDL_Game* initGame() {
    SDL_Game* game = malloc(sizeof(SDL_Game));
    if (game == NULL) return NULL;
    game->success = 1;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        game->success = 0;
    } else {
        game->gWindow = SDL_CreateWindow("Nevada", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);
        if (game->gWindow == NULL) {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError());
            game->success = 0;
        } else {
            game->gRenderer = SDL_CreateRenderer(game->gWindow, -1, rendererFlags);
            if (game->gRenderer == NULL) {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                game->success = 0;
            } else {
                SDL_SetRenderDrawColor(game->gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                
                int imgFlags = IMG_INIT_PNG;
                // INITIALIZE IMGs
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    game->success = 0;
                }

                // INITIALIZE AUDIO
                if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
                    printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
                    game->success = 0;
                }

                // INITIALIZE TTF
                if(TTF_Init() == -1) {
                    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                    game->success = 0;
                }
            }
        }
    }

    game->gMusic = Mix_LoadMUS("res/ex-aws_cave.xm");
    if (game->gMusic == NULL) {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        game->success = 0;
    }

    return game;
}


Level* getLevel(short n) {

    TiledMap* tiledMap = parseMap("res/images/map.tmx");
    // printf("TiledMap width: %i x height:%i, tileWidth:%i, tileHeight:%i\n", tiledMap->width, tiledMap->height, tiledMap->tileWidth, tiledMap->tileHeight);

    int differentSizeOfLayers = 0;
    for (int i = 0; i < tiledMap->layersCount - 1; i++) {
        if (tiledMap->layer[i].width != tiledMap->layer[i+1].width ||
            tiledMap->layer[i].height != tiledMap->layer[i+1].height) {
            differentSizeOfLayers = 1;
        }
    }
    if (differentSizeOfLayers != 0) {
        printf("ERROR !!! Layers has different sizes !!!\n");
        exit(0);
    }
    // printf("TiledMap layers count: %i\n", tiledMap->layersCount);
    // for (int i = 0; i < tiledMap->layersCount; i++) {
    //     printf("Layer info - id: %i, tilesCount: %i\n", tiledMap->layer[i].id, tiledMap->layer[i].data[0]);
    // }

    Level* level = malloc(sizeof(Level));
    if (level == NULL) return NULL;

    level->width = tiledMap->layer[0].width;
    level->height = tiledMap->layer[0].height;
    level->size = level->width * level->height;

    level->content = malloc(sizeof *level->content * level->size);
    if (level->content == NULL) return NULL;

    level->content = tiledMap->layer;
    level->layers = tiledMap->layersCount;
    level->map = tiledMap;
    
    return level;
}


void updateCamera(Camera* c, Player player) {
    c->x = - player.x + (SCREEN_WIDTH / 2) - (player.width / 2);
    c->y = - player.y + (SCREEN_HEIGHT / 2) - (player.height / 2);
    c->offsetX = - player.x + (SCREEN_WIDTH / 2) - (player.width / 2);
    c->offsetY = - player.y + (SCREEN_HEIGHT / 2) - (player.height / 2);
}

int getTileX(Player* p, Camera* c, Level* l, unsigned int tw) {
    return ( (p->x + (p->width / 2)) / tw );
}

int getTileY(Player* p, Camera* c, Level* l, unsigned int th) {
    return ( (p->y + (p->height / 2)) / th );
}

Animation* prepareAnimation(Texture* t, unsigned int speed, unsigned int sw, unsigned int sh, const unsigned int size, unsigned int* frames) {
    Animation* anim = malloc(sizeof(Animation));
    if (anim == NULL) return NULL;

    anim->size = size;
    anim->speed = speed;
    anim->counter = 0;
    anim->curFrame = 0;

    anim->frames = malloc(sizeof(SDL_Rect) * size);
    if (anim->frames == NULL) return NULL;
    for (int i = 0; i < size; i++) {
        anim->frames[i] = *getSpriteI(t, frames[i], sw, sh);
    }
    return anim;
}

int nextFrame(Animation* an) {
    an->counter++;
    if (an->counter > an->speed) {
        an->counter = 0;
        an->curFrame++;
        if (an->curFrame >= an->size) {
            an->curFrame = 0;
        }
    }
    return an->curFrame;
}


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


char* getCurrentTime(void) {
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

	// luaTest();

	SDL_Game* game = initGame();
	if (!game) {
		printf("ERROR!\n");
		exit(1);
	} else {
		// printf("Initialization SDL - OK!\nGame.success -> %i\n", game->success);		

		// LEVEL STUFF...
		Level* level = getLevel(0);
		
		// printf("This level consists of %i layer(s).\n", level->layers);
		// printf("This level consists of %i tilesets.\n", level->map->tileSetCount);

		Texture* playerSpriteSheet = loadSpriteSheet("animals1.png", game, 52, 72);
		// printf("Player spritesheet Width: %i, height: %i\n", playerSpriteSheet->width, playerSpriteSheet->height);

		Texture* backgroundSpriteSheet = loadSpriteSheet("grassland.png", game, 64, 64);
		// printf("Background spritesheet Width: %i, height: %i\n", backgroundSpriteSheet->width, backgroundSpriteSheet->height);

		// Texture* font1 = loadFromRenderedText("THE TEMPLE OF THE LOST PUPPY", game);

		// PLAYER
		Player* player = resetPlayer(playerSpriteSheet);

		Camera cam;
		cam.x = 0;
		cam.y = 0;
		cam.offsetX = 0;
		cam.offsetY = 0;
		
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

		// unsigned int framesDog1[] = {73, 74, 75};
		// unsigned int framesDog2[] = {19, 20, 21};
		// unsigned int framesDog3[] = {22, 23, 24};
		// unsigned int framesDog4[] = {34, 35, 36};

		// Animation* dogsAnim[] = {
		// 	prepareAnimation(playerSpriteSheet, 1, player->width, player->height, 3, framesDog1),
		// 	prepareAnimation(playerSpriteSheet, 3, player->width, player->height, 3, framesDog2),
		// 	prepareAnimation(playerSpriteSheet, 6, player->width, player->height, 3, framesDog3),
		// 	prepareAnimation(playerSpriteSheet, 9, player->width, player->height, 3, framesDog4),
		// };

		// ANIMATIONS
		Animation* walkingAnimation[4];

		unsigned int framesPlayerLeft[]  = {16, 17, 18};
		unsigned int framesPlayerRight[] = {28, 29, 30};
		unsigned int framesPlayerUp[]    = {40, 41, 42};
		unsigned int framesPlayerDown[]  = {4,  5,  6};

		walkingAnimation[WALK_UP] = prepareAnimation(playerSpriteSheet, 6, player->width, player->height, 3, framesPlayerUp);
        walkingAnimation[WALK_RIGHT] = prepareAnimation(playerSpriteSheet, 6, player->width, player->height, 3, framesPlayerRight);
        walkingAnimation[WALK_DOWN] = prepareAnimation(playerSpriteSheet, 6, player->width, player->height, 3, framesPlayerDown);
        walkingAnimation[WALK_LEFT] = prepareAnimation(playerSpriteSheet, 6, player->width, player->height, 3, framesPlayerLeft);		

		int walking = 0;
		int currentWalk = WALK_LEFT;
		walkingAnimation[currentWalk]->curFrame = 1;

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
							if (player->velX > 0) {
								player->velX = 0;
							}
						}
						if (e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_a) {
							if (player->velX < 0) {
								player->velX = 0;
							}
						}
						if (e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_w) {
							if (player->velY < 0) {
								player->velY = 0;
							}
						}
						if (e.key.keysym.sym == SDLK_DOWN || e.key.keysym.sym == SDLK_s) {
							if (player->velY > 0) {
								player->velY = 0;
							}
						}
					}
				}
			}

			/**
			 * #################################################
			 * .................... UPDATE .....................
			 * #################################################
			 * */
		
			// PLAYER'S VELOCITY
			player->x += player->velX;
			player->y += player->velY;
			
			// WALKING
			walking = 1;
			if (player->velX == SPEED) {
				currentWalk = WALK_RIGHT;
			} else if (player->velX == -SPEED) {
				currentWalk = WALK_LEFT;
			} else if (player->velY == SPEED) {
				currentWalk = WALK_DOWN;
			} else if (player->velY == - SPEED) {
				currentWalk = WALK_UP;
			} else walking = 0;


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
					&walkingAnimation[currentWalk]->frames[nextFrame(walkingAnimation[currentWalk])],
					player->x + cam.offsetX,
					player->y + cam.offsetY,
					playerSpriteSheet->sWidth,
					playerSpriteSheet->sHeight
				);
			} else {
				renderTexture(
					playerSpriteSheet,
					game,
					&walkingAnimation[currentWalk]->frames[walkingAnimation[currentWalk]->curFrame],
					player->x + cam.offsetX,
					player->y + cam.offsetY,
					playerSpriteSheet->sWidth,
					playerSpriteSheet->sHeight
				);
			}
			
			// renderText(font1, game, 100, 50, 400, 50);

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

		// freeTexture(font1);
		// font1 = NULL;

		freeTiledMap(level->map);
		//free(level->content);
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
