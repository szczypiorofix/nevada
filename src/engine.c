#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/xmlIO.h>
#include <libxml/xinclude.h>
#include <libxml/tree.h>

#include "engine.h"

// --- CONSTANTS --
const short VSYNC_ON = 1;
const short VSYNC_OFF = 0;
const short TARGET_FPS = 60.0f;
const float OPTIMAL_TIME = 1000.0f / TARGET_FPS;

const short VIEW_LOCKED_ON_PLAYER = 1;
const short VIEW_LOCKED_ON_MOUSE = 2;


// --- STATIC ---
static int random(int min, int max);
static int initSDL(Engine* engine);
static int createWindow(Engine* engine);
static int createRenderer(Engine* engine);
static int initializePngImages(Engine* engine);
static int initializeAudioSystem(Engine* engine);
static int initializeTTFFonts(Engine* engine);
static int randomNPCActivity_move();
static int randomNPCActivity_wait();
static TiledMap* parseMap(const char* fileName);
static int* convertDataStringToArray(const xmlChar* s);
static int countTiledObjects(xmlNodePtr cur);
static TiledObject* getTiledObjects(xmlNodePtr cur, int tiledObjectsCounter);
static int* parseData(xmlDocPtr doc, xmlNodePtr cur);
static TileSetSource* getTileSetSource(const char* tsxFileName);


// --- PUBLIC ---
Engine* createEngine(void);
struct Engine* engineStart();
void engineStop(Engine** engine);
int loadMusic(char* musicFile);
void updateCamera();
int setFullScreen(WindowFullScreen flag);
SpriteSheet* loadSpriteSheet(char* fileName, SDL_Renderer* renderer, unsigned int spriteWidth, unsigned int spriteHeigth);
void freeTexture(SpriteSheet* t);
void renderTexture(SpriteSheet* t, SDL_Renderer* renderer, SDL_Rect* clip, int x, int y, float scale, double angle, SDL_Point* center, SDL_RendererFlip flip, int mode);
Animation* prepareAnimation(SpriteSheet* t, unsigned int speed, unsigned int sw, unsigned int sh, const unsigned short size, const unsigned int* frames);
int checkCollision(SDL_Rect r1, SDL_Rect r2);
Player* resetPlayer(char* name, float x, float y, short int width, short int height);
int getTileX(float x, unsigned int width, unsigned int tileWidth);
int getTileY(float y, unsigned int height, unsigned int tileHeight);
int updateNPC(NPC* npc, Level* level);
NPC* setNPC(int x, int y, int width, int height, Direction direction);
void updateCollisionsNPC(NPC* npc, const Camera* cam, const float scale);
Ground* setGround(float x, float y, short int width, short int height);
void updateCollisionsPlayer(Player* p, const Camera* cam, const float scale);
void drawNPCCollisions(NPC* npc, SDL_Renderer* renderer);
Level* getLevel(char* fileName);
int xmlCharToInt(const xmlChar a[]);
int* parseData(xmlDocPtr doc, xmlNodePtr cur);
int xmlCharToInt(const xmlChar a[]);
int stringToInt(const char a[]);
void freeTiledMap(TiledMap* tiledMap);
TileSetSource* getTileSetSource(const char* tsxFileName);
TiledObject* getTiledObjects(xmlNodePtr cur, int tiledObjectsCounter);
int countTiledObjects(xmlNodePtr cur);
TextFont* loadFromRenderedText(const char* textureText, SDL_Renderer* renderer);
void renderText(TextFont* t, SDL_Renderer* renderer, int x, int y, int w, int h);
void changeText(TextFont* t, SDL_Renderer* renderer, char* text);



// ------------------------ "PRIVATE" FUNCTIONS ------------------------

static int random(int min, int max) {
    int tmp;
    if (max >= min)
        max -= min;
    else{
        tmp = min - max;
        min = max;
        max = tmp;
    }
    return max ? (rand() % max + min) : min;
}

static int initSDL(Engine* engine) {
    engine->started = ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == 0 );
    if (!engine->started) printf( "SDL_Init() Error: %s\n", SDL_GetError() );
    atexit(SDL_Quit);
    return engine->started;
}

static int createWindow(Engine* engine) {
    short windowMode = 0;
    windowMode = engine->fullScreen == ENGINE_WINDOW ? SDL_WINDOW_SHOWN : SDL_WINDOW_FULLSCREEN;
    engine->window = SDL_CreateWindow("Nevada", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowMode);
    if (engine->window == NULL) {
        printf( "SDL_CreateWindow() Error: %s\n", SDL_GetError());
        engine->started = 0;
    }
    return (engine->window == NULL);
}


static int createRenderer(Engine* engine) {
    engine->renderer = SDL_CreateRenderer(engine->window, -1, SDL_RENDERER_ACCELERATED);// | SDL_RENDERER_PRESENTVSYNC);
    if (engine->renderer == NULL) {
        printf("SDL_CreateRenderer() Error: %s\n", SDL_GetError());
        engine->started = 0;
    } else {
        SDL_SetRenderDrawColor(engine->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    }
    return (engine->renderer == NULL);
}

static int initializePngImages(Engine* engine) {
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf( "IMG_Init() Error: %s\n", IMG_GetError());
        engine->started = 0;
    }
    return engine->started;
}

static int initializeAudioSystem(Engine* engine) {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer Mix_OpenAudio() Error: %s\n", Mix_GetError());
        engine->started = 0;
    }
    return engine->started;
}

static int initializeTTFFonts(Engine* engine) {
    if(TTF_Init() == -1) {
        printf( "SDL_ttf TTF_Init() Error: %s\n", TTF_GetError() );
        engine->started = 0;
    }
    return engine->started;
}

static int randomNPCActivity_move() {
    return random(60, 160);
}


static int randomNPCActivity_wait() {
    return random(20, 150);
}


static TiledMap* parseMap(const char* fileName) {
	
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
		fprintf(stderr,"Document of the wrong type, root node != map !!! \n");
		xmlFreeDoc(doc);
		exit(0);
	}

	TiledMap* tiledMap = malloc(sizeof (TiledMap));
	if (tiledMap == NULL) {
		printf("Malloc (creating TiledMap) error !!!\n");
		exit(1);
	}
	tiledMap->width = xmlCharToInt(xmlGetProp(cur, (const xmlChar *) "width"));
	tiledMap->height = xmlCharToInt(xmlGetProp(cur, (const xmlChar *) "height"));
	tiledMap->tileWidth = xmlCharToInt(xmlGetProp(cur, (const xmlChar *) "tilewidth"));
	tiledMap->tileHeight = xmlCharToInt(xmlGetProp(cur, (const xmlChar *) "tileheight"));

    cur = cur->xmlChildrenNode;

	int layersCount = 0;
	int tileSetCount = 0;
	int objectGroupCount = 0;

	xmlNodePtr firstForCounter = cur;
	
	// Counting layers and tilesets
	while(firstForCounter != NULL) {
		if ((!xmlStrcmp(firstForCounter->name, (const xmlChar *)"tileset"))) {
			tileSetCount++;
		}
		if ((!xmlStrcmp(firstForCounter->name, (const xmlChar *)"layer"))) {
			layersCount++;
		}
		if ((!xmlStrcmp(firstForCounter->name, (const xmlChar *)"objectgroup"))) {
			objectGroupCount++;
		}
		firstForCounter = firstForCounter->next;
	}
	
	int lc = 0, tc = 0, ogc = 0;

	TileSet* tileSet = malloc(sizeof(TileSet) * tileSetCount);
	if (tileSet == NULL) {
		printf("Malloc (creating TileSet) error !!!\n");
		exit(1);
	}

	Layer* layers = malloc(sizeof(Layer) * layersCount);
	if (layers == NULL) {
		printf("Malloc (creating Layer) error !!!\n");
		exit(1);
	}

	ObjectGroup* objectGroups = malloc(sizeof(ObjectGroup) * objectGroupCount);
	if (layers == NULL) {
		printf("Malloc (creating ObjectGroup) error !!!\n");
		exit(1);
	}


    while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"tileset"))) {
			tileSet[tc].firstGid = xmlCharToInt(xmlGetProp(cur, (const xmlChar *) "firstgid"));
			tileSet[tc].source = (char *)xmlGetProp(cur, (const xmlChar *) "source");
			TileSetSource* tss = getTileSetSource(tileSet[tc].source);
			tileSet[tc].tileSetSource = tss;
			tc++;
		}
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"layer"))) {
			layers[lc].id = xmlCharToInt(xmlGetProp(cur,  (const xmlChar *) "id"));
			layers[lc].name = (char *)xmlGetProp(cur,  (const xmlChar *) "name");
			layers[lc].width = xmlCharToInt(xmlGetProp(cur,  (const xmlChar *) "width"));
			layers[lc].height = xmlCharToInt(xmlGetProp(cur,  (const xmlChar *) "height"));
			layers[lc].data = parseData(doc, cur);
			lc++;
		}
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"objectgroup"))) {
			objectGroups[ogc].id = xmlCharToInt(xmlGetProp(cur, (const xmlChar *) "id"));
			objectGroups[ogc].name = (char *)xmlGetProp(cur, (const xmlChar *) "name");
			objectGroups[ogc].objectsCount = countTiledObjects(cur);
			objectGroups[ogc].objects = getTiledObjects(cur, objectGroups[ogc].objectsCount);
			ogc++;
		}
	    cur = cur->next;
	}
	tiledMap->tileSet = tileSet;
	tiledMap->layer = layers;
	tiledMap->ObjectGroup = objectGroups;
	tiledMap->layersCount = lc;
	tiledMap->tileSetCount = tc;
	tiledMap->objectGroupCount = ogc;

	xmlFreeDoc(doc);
	xmlCleanupMemory();
	xmlCleanupParser();
	
	return tiledMap;
} 


static int* convertDataStringToArray(const xmlChar* s) {
	unsigned int numbers = 0;
	unsigned int c = 0;
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
	c = 0;
	char cleanCharsArray[cleanCharsNumber];
	cleanCharsNumber = 0;
	while(s[c] != '\0') {
		// CHECK CR/LF AND SPACES
		if (s[c] != 13 && s[c] != 32 && s[c] != 10) {
			cleanCharsArray[cleanCharsNumber] = s[c];
			cleanCharsNumber++;
		}
		c++;
	}

	c = 0;
	char delim[] = ",";
	char *ptr = strtok(cleanCharsArray, delim);
	int* numArr = malloc(sizeof(int) * numbers);
	if (numArr == NULL) {
		printf("Malloc 'numArr' error !!!\n");
		exit(1);
	}
	while(ptr != NULL) {
		numArr[c] = stringToInt(ptr);
		ptr = strtok(NULL, delim);
		c++;
	}
	
	return numArr;
}


static int countTiledObjects(xmlNodePtr cur) {
	cur = cur->xmlChildrenNode;
	int objectsCounter = 0;
	while (cur != NULL) {
	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"object"))) {
			objectsCounter++;
 	    }
		cur = cur->next;
	}
	return objectsCounter;
}


static TiledObject* getTiledObjects(xmlNodePtr cur, int tiledObjectsCounter) {
	
	TiledObject* objects = malloc(sizeof(TiledObject) * tiledObjectsCounter);
	if (objects == NULL) {
		printf("Malloc (creating TiledObject) error !!!\n");
		exit(1);
	}
	cur = cur->xmlChildrenNode;
	int i = 0;
	while (cur != NULL) {
	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"object"))) {
		    objects[i].id = xmlCharToInt(xmlGetProp(cur, (const xmlChar *) "id"));
			objects[i].name = (char *)xmlGetProp(cur, (const xmlChar *) "name");
			objects[i].type = (char *)xmlGetProp(cur, (const xmlChar *) "type");
			objects[i].template = (char *)xmlGetProp(cur, (const xmlChar *) "template");
			objects[i].x = xmlCharToInt(xmlGetProp(cur, (const xmlChar *) "x"));
			objects[i].y = xmlCharToInt(xmlGetProp(cur, (const xmlChar *) "y"));


            char tmp[50];
            sprintf(tmp, "%s%s", DIR_RES_IMAGES, objects[i].template);
			xmlDocPtr txDoc;
			xmlNodePtr txCurNode;
			txDoc = xmlParseFile(tmp);
			if (txDoc == NULL) {
				fprintf(stderr, "Document not parsed successfully.\n");
				exit(0);
			}
			txCurNode = xmlDocGetRootElement(txDoc);
			if (txCurNode == NULL) {
				fprintf(stderr,"empty document\n");
				xmlFreeDoc(txDoc);
				exit(0);
			}
			if (xmlStrcmp(txCurNode->name, (const xmlChar *) "template")) {
				fprintf(stderr,"document of the wrong type, root node != template !!! \n");
				xmlFreeDoc(txDoc);
				exit(0);
			}
			txCurNode = txCurNode->xmlChildrenNode;
			while (txCurNode != NULL) {
				if (!(xmlStrcmp(txCurNode->name, (const xmlChar *)"tileset"))) {
					objects[i].source = (char *)xmlGetProp(txCurNode, (const xmlChar *) "source");
					objects[i].firstGid = xmlCharToInt(xmlGetProp(txCurNode, (const xmlChar *) "firstgid"));
				}
				if (!(xmlStrcmp(txCurNode->name, (const xmlChar *)"object"))) {
					objects[i].gid = xmlCharToInt(xmlGetProp(txCurNode, (const xmlChar *) "gid"));
					objects[i].width = xmlCharToInt(xmlGetProp(txCurNode, (const xmlChar *) "width"));
					objects[i].height = xmlCharToInt(xmlGetProp(txCurNode, (const xmlChar *) "height"));
				}
				txCurNode = txCurNode->next;
			}
			xmlFreeDoc(txDoc);
			i++;
 	    }
		cur = cur->next;
	}
	return objects;
}


static int* parseData(xmlDocPtr doc, xmlNodePtr cur) {
	xmlChar* key;
	cur = cur->xmlChildrenNode;
	int* arr = NULL;
	while (cur != NULL) {
	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"data"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			arr = convertDataStringToArray(key);
		    xmlFree(key);
 	    }
		cur = cur->next;
	}
    return arr;
}


static TileSetSource* getTileSetSource(const char* tsxFileName) {
	TileSetSource* tileSetSource = malloc(sizeof(TileSetSource));
	if (tileSetSource == NULL) {
		printf("Malloc (creating TileSetSource) error !!!\n");
		exit(1);
	}
    char tmp[50];
    sprintf(tmp, "%s%s", DIR_RES_IMAGES, tsxFileName);
	
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
		fprintf(stderr,"document of the wrong type, root node != tileset !!! \n");
		xmlFreeDoc(tsxDoc);
		exit(0);
	}
	tileSetSource->name = (char *)xmlGetProp(tsxCurNode, (const xmlChar *) "name");
	tileSetSource->tileWidth = xmlCharToInt(xmlGetProp(tsxCurNode, (const xmlChar *) "tilewidth"));
	tileSetSource->tileHeight = xmlCharToInt(xmlGetProp(tsxCurNode, (const xmlChar *) "tileheight"));
	tileSetSource->tileCount = xmlCharToInt(xmlGetProp(tsxCurNode, (const xmlChar *) "tilecount"));
	tileSetSource->columns = xmlCharToInt(xmlGetProp(tsxCurNode, (const xmlChar *) "columns"));

	tsxCurNode = tsxCurNode->xmlChildrenNode;
	while (tsxCurNode != NULL) {
		if ((!xmlStrcmp(tsxCurNode->name, (const xmlChar *) "image"))) {
			tileSetSource->imageSource= (char *) xmlGetProp(tsxCurNode, (const xmlChar *) "source");
			tileSetSource->width = xmlCharToInt(xmlGetProp(tsxCurNode, (const xmlChar *) "width"));
			tileSetSource->height = xmlCharToInt(xmlGetProp(tsxCurNode, (const xmlChar *) "height"));
		}
		tsxCurNode = tsxCurNode->next;
	}
	xmlFreeDoc(tsxDoc);
	return tileSetSource;
}





// ------------------------ "PUBLIC" FUNCTIONS ------------------------

Engine* createEngine(void) {
    Engine* engine = malloc(sizeof(Engine));
    if (engine == NULL) {
        fprintf(stderr, "Engine is NULL !!! \n");
        exit(1);
    }
    
    engine->started = 0;
    engine->quit = 0;
    engine->window = NULL;
    engine->renderer = NULL;
    engine->music = NULL;
    engine->camera = NULL;
    engine->musicVolume = 20;
    engine->scale = 2.0f;
    engine->minScale = 2.0f;
    engine->maxScale = 5.0f;
    engine->tilesOnScreenFromCenterX = 0;
    engine->tilesOnScreenFromCenterY = 0;
    engine->coordinatesText[0] = ' ';
    
    engine->mouseRightButtonPressed = 0;

    engine->viewLockedOn = VIEW_LOCKED_ON_PLAYER;

    Vector2 tempScrollVector = {0.0f, 0.0f};
    engine->scrollVector = &tempScrollVector;
    engine->scrollVector->x = 0.0f;
    engine->scrollVector->y = 0.0f;
    

    // GAME LOOP
    engine->fpsCap = VSYNC_ON;
    engine->fullScreen = ENGINE_WINDOW;
    engine->lastTime = 0L;
	engine->timer = 0L;
    engine->delta = 0.0f;
	engine->updates = 0;
	engine->frames = 0;
	engine->now = 0L;
	engine->amountOfTicks = 60.0f;
	engine->fps_count = 0;
	engine->ticks_count = 0;
	engine->ns = 0;

    engine->displayMode = 0;

    return engine;
}


int setFullScreen(WindowFullScreen flag) {
    int s = SDL_SetWindowFullscreen(engine->window, flag);
    engine->fullScreen = flag;
    if (s == 0) return 1;
    else return 0;
}


Engine* engineStart(void) {
    Engine* engine = createEngine();
    if (engine == NULL) {
        fprintf(stderr, "Engine is NULL !!! \n");
        exit(1);
    }
    initSDL(engine);
    createWindow(engine);
    createRenderer(engine);
    initializePngImages(engine);
    initializeAudioSystem(engine);
    initializeTTFFonts(engine);

    engine->coordinates = loadFromRenderedText(engine->coordinatesText, engine->renderer);
    
    // engine->assets = createAssets();
    
    if (engine->started == 0) {
        fprintf(stderr, "Engine is NULL !!! \n");
        exit(1);
    }
   
    return engine;
}


void engineStop(Engine** engine) {

    free( (*engine)->camera );
    (*engine)->camera = NULL;

    (*engine)->started = 0;

    Mix_FreeMusic((*engine)->music);
    (*engine)->music = NULL;

    SDL_DestroyRenderer((*engine)->renderer);
    (*engine)->renderer = NULL;

    SDL_DestroyWindow((*engine)->window);
    (*engine)->window = NULL;
    	
    free(*engine);
    (*engine) = NULL;

	TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}


int loadMusic(char* musicFile) {
    engine->music = Mix_LoadMUS(musicFile);
    if (engine->music == NULL) {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        return 0;
    }
    Mix_VolumeMusic(engine->musicVolume);
    return 1;
}

void lockCameraOnObject(float* x, float* y) {
    engine->lockCameraOnObjectX = x;
    engine->lockCameraOnObjectY = y;
}

void updateCamera() {
    engine->camera->vec.x = ( *engine->lockCameraOnObjectX  * engine->scale) - (SCREEN_WIDTH / 2);
    engine->camera->vec.y = ( *engine->lockCameraOnObjectY * engine->scale) - (SCREEN_HEIGHT / 2);
}


void freeTexture(SpriteSheet* t) {
    if (t->mTexture != NULL) {
        SDL_DestroyTexture(t->mTexture);
        t->mTexture = NULL;
        free(t);
    }
}


void renderTexture(SpriteSheet* t, SDL_Renderer* renderer, SDL_Rect* clip, int x, int y, float scale, double angle, SDL_Point* center, SDL_RendererFlip flip, int mode) { 
    if (t == NULL || clip == NULL) return;
    SDL_Rect renderQuad = {x, y, t->tileWidth * scale, t->tileHeight * scale};
    if (clip != NULL) {
        renderQuad.w = clip->w * scale;
		renderQuad.h = clip->h * scale;
    }
    if (mode == 1) {
        SDL_RenderCopyEx(renderer, t->mTexture, clip, &renderQuad, angle, center, flip);
        SDL_RenderDrawRect(renderer, &renderQuad);
    } else if (mode == 2) {
        SDL_RenderDrawRect(renderer, &renderQuad);
    } else {
        SDL_RenderCopyEx(renderer, t->mTexture, clip, &renderQuad, angle, center, flip);
    }
}


SpriteSheet* loadSpriteSheet(char* fileName, SDL_Renderer* renderer, unsigned int tileWidth, unsigned int tileHeight) {
    SpriteSheet* t = malloc(sizeof(SpriteSheet));
    if (t == NULL) {
        fprintf(stderr, "Cannot load sprite sheet: %s !\n", fileName);
        exit(1);
    }
    SDL_Texture* newTexture = NULL;
    t->name = fileName;

    char str[50];
    sprintf(str, "%s%s", DIR_RES_IMAGES, fileName);

    printf("Loading file %s...\n", str);

    SDL_Surface* loadedSurface = IMG_Load(str);
    if (loadedSurface == NULL) {
        printf( "Unable to load image %s! SDL_image Error: %s\n", str, IMG_GetError());
        t = NULL;
    } else {
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", str, SDL_GetError());
        } else {
            t->width = loadedSurface->w;
            t->height = loadedSurface->h;
        }
        SDL_FreeSurface(loadedSurface);
    }
    t->mTexture   = newTexture;
    t->tileWidth  = tileWidth;
    t->tileHeight = tileHeight;
    return t;
}


SDL_Rect* createRectsForSprites(Level* level, unsigned short layerCount, SpriteSheet* t) {
    SDL_Rect* rects = malloc(sizeof(SDL_Rect) * level->size);
    unsigned int col = t->width / t->tileWidth;
    for (unsigned int i = 0; i < level->size; i++) {

        SDL_Rect r = {
            r.x = ((level->content[layerCount].data[i] - 1) % col) * t->tileWidth,
            r.y = ((level->content[layerCount].data[i] - 1) / col) * t->tileHeight,
            r.w = t->tileWidth,
            r.h = t->tileHeight
        };

        if ( (level->content[layerCount].data[i] - 1) == 0 ) {
            r.x = -1;
            r.y = -1;
            r.w = -1;
            r.h = -1;
        }
        rects[i] = r;
    }
    return rects;
}

Animation* prepareAnimation(SpriteSheet* t, unsigned int speed, unsigned int sw, unsigned int sh, const unsigned short size, const unsigned int* frames) {
    Animation* anim = malloc(sizeof(Animation));
    if (anim == NULL) {
        fprintf(stderr, "Animation is NULL !!!\n");
        exit(1);
    }
    anim->size = size;
    anim->speed = speed;
    anim->counter = 0;
    anim->curFrame = 0;
    anim->spriteSheet = t;

    anim->frames = malloc(sizeof(SDL_Rect) * size);
    if (anim->frames == NULL) {
        fprintf(stderr, "Animation frames are NULL !!!\n");
        exit(1);
    }
    for (unsigned int i = 0; i < size; i++) {
        int col = t->width / t->tileWidth;
        SDL_Rect r = {
            r.x = ((frames[i] - 1) % col) * sw,
            r.y = ((frames[i] - 1) / col) * sh,
            r.w = sw,
            r.h = sh
        };
        anim->frames[i] = r;
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


int checkCollision(SDL_Rect r1, SDL_Rect r2) {
    return (
        r1.x + r1.w > r2.x &&
        r1.y + r1.h > r2.y &&
        r1.x < r2.x + r2.w &&
        r1.y < r2.y + r2.h
    );
}


Player* resetPlayer(char* name, float x, float y, short int width, short int height) {
    Player* p = malloc(sizeof(Player));
	if (p == NULL) {
        fprintf(stderr, "Malloc error while creating Player!!!\n");
        return NULL;
    }
    p->name = name;
    p->width = width;
	p->height = height;
    p->direction = DIR_RIGHT;
    p->speed = 2.0f;
    p->tileIndex = 0;
    p->tileX = 0;
    p->tileY = 0;
    p->vec = setVector(x, y);
    p->moveVec = setVector(0, 0);
    p->isMoving = 0;
    SDL_Rect c = { p->vec.x, p->vec.y, p->width, p->height };
    p->col = c;
    return p;
}


int getTileX(float x, unsigned int width, unsigned int tileWidth) {
    return ( (x + (width / 2)) / tileWidth );
}


int getTileY(float y, unsigned int height, unsigned int tileHeight) {
    return ( (y + (height / 2)) / tileHeight );
}


void updateCollisionsNPC(NPC* npc, const Camera* cam, const float scale) {
    npc->col.x = ( (npc->vec.x - (npc->width / 2)) * scale) - cam->vec.x;
    npc->col.y = ( (npc->vec.y - (npc->height / 2)) * scale) - cam->vec.y;
    npc->col.w = npc->width * scale;
    npc->col.h = npc->height * scale;
}


void updateCollisionsPlayer(Player* p, const Camera* cam, const float scale) {
    p->col.x = ( (p->vec.x - (p->width / 2)) * scale) - cam->vec.x;
    p->col.y = ( (p->vec.y - (p->height / 2)) * scale) - cam->vec.y;
    p->col.w = p->width * scale;
    p->col.h = p->height * scale;
}


int updateNPC(NPC* npc, Level* level) {

    // COUNTING ACTIONS TIME
    if (npc->takingActionCounter < npc->maxTakingActionCounter)
        npc->takingActionCounter++;
    else {
        npc->takingActionCounter = 0;
        npc->maxTakingActionCounter = 0;
    }
    
    if (npc->takingActionCounter == 0) { // ANOTHER RANDOM ACTION
        if (npc->takingAction == 0) {
            switch (random(0, 8)) {
                // Basic directions
                case DIR_UP:
                    npc->moveVec.x = 0;
                    npc->moveVec.y = -NPC_SPEED;
                    npc->direction = DIR_UP;
                    npc->maxTakingActionCounter = randomNPCActivity_move();
                    break;
                case DIR_RIGHT:
                    npc->moveVec.x = NPC_SPEED;
                    npc->moveVec.y = 0;
                    npc->direction = DIR_RIGHT;
                    npc->maxTakingActionCounter = randomNPCActivity_move();
                    break;
                case DIR_DOWN:
                    npc->moveVec.x = 0;
                    npc->moveVec.y = NPC_SPEED;
                    npc->direction = DIR_DOWN;
                    npc->maxTakingActionCounter = randomNPCActivity_move();
                    break;
                case DIR_LEFT:
                    npc->moveVec.x = -NPC_SPEED;
                    npc->moveVec.y = 0;
                    npc->direction = DIR_LEFT;
                    npc->maxTakingActionCounter = randomNPCActivity_move();
                    break;
                // Additional directions
                case DIR_UP_RIGHT:
                    npc->moveVec.x = NPC_SPEED;
                    npc->moveVec.y = -NPC_SPEED;
                    npc->direction = DIR_RIGHT;
                    npc->maxTakingActionCounter = randomNPCActivity_move();
                    break;
                case DIR_DOWN_RIGHT:
                    npc->moveVec.x = NPC_SPEED;
                    npc->moveVec.y = NPC_SPEED;
                    npc->direction = DIR_RIGHT;
                    npc->maxTakingActionCounter = randomNPCActivity_move();
                    break;
                case DIR_DOWN_LEFT:
                    npc->moveVec.x = -NPC_SPEED;
                    npc->moveVec.y = NPC_SPEED;
                    npc->direction = DIR_LEFT;
                    npc->maxTakingActionCounter = randomNPCActivity_move();
                    break;
                case DIR_UP_LEFT:
                    npc->moveVec.x = -NPC_SPEED;
                    npc->moveVec.y = -NPC_SPEED;
                    npc->direction = DIR_LEFT;
                    npc->maxTakingActionCounter = randomNPCActivity_move();
                    break;
            }
            
            printf("NPC new direction: %i\n", npc->direction);
            printf("NPC anim:  %i\n", npc->walkingAnimation->curFrame);
            
            npc->takingAction = 1;
        } else {
            npc->takingAction = 0;
            npc->takingActionCounter = 0;
            npc->maxTakingActionCounter = randomNPCActivity_wait();
            npc->moveVec.x = 0;
            npc->moveVec.y = 0;
        }  
    }

    // addVector(&npc->vec, &npc->moveVec);
    if (npc->vec.x < npc->width / 2)
        npc->vec.x = npc->width / 2;
    if (npc->vec.y < npc->height / 2)
        npc->vec.y = npc->height / 2;
    if (npc->vec.x > (level->width * level->map->tileWidth) - npc->width)
        npc->vec.x = (level->width * level->map->tileWidth) - npc->width;
    if (npc->vec.y > (level->height * level->map->tileHeight) - npc->height)
        npc->vec.y = (level->height * level->map->tileHeight) - npc->height;
    return 1;
}


NPC* setNPC(int x, int y, int width, int height, Direction direction) {
    NPC* n = malloc(sizeof(NPC));
	if (n == NULL) {
        fprintf(stderr, "Malloc error while creating NPC!!!\n");
        return NULL;
    }
    
    n->name = "NPC...";

    n->vec = setVector(x, y);
    n->moveVec = setVector(0, 0);

    n->width = width;
	n->height = height;
    n->direction = direction;
    n->takingAction = 0;
    n->takingActionCounter = 0;
    n->maxTakingActionCounter = 0;
    SDL_Rect c = { n->vec.x, n->vec.y, n->width, n->height };
    n->col = c;
    return n;
}


Ground* setGround(float x, float y, short width, short height) {
    Ground* g = malloc(sizeof(Ground));
	if (g == NULL) {
        fprintf(stderr, "Malloc error while creating Ground!!!\n");
        return NULL;
    }
    g->vec = setVector(x, y);
    g->width = width;
    g->height = height;
    g->gid = 0;
    return g;
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


void drawNPCCollisions(NPC* npc, SDL_Renderer* renderer) {
    SDL_RenderDrawRect(renderer, &npc->col);
}


void freeTiledMap(TiledMap* tiledMap) {	
	for (int i = 0; i < tiledMap->layersCount; i++) {
		free(tiledMap->layer[i].encoding);
		free(tiledMap->layer[i].name);
		free(tiledMap->layer[i].data);
		tiledMap->layer[i].height = 0;
		tiledMap->layer[i].width = 0;
		tiledMap->layer[i].id = 0;
	}

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


Level* getLevel(char* fileName) {
    TiledMap* tiledMap = parseMap(fileName);
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

	// Loading texture file from map

	level->textureNameCount = tiledMap->tileSetCount;
	// printf("level->textureNameCount: %i\n", level->textureNameCount);
	
	for (int i = 0; i < tiledMap->tileSetCount; i++) {
		level->textureName[i] = tiledMap->tileSet[i].tileSetSource->imageSource;
	}

    level->content = tiledMap->layer;
    level->layers = tiledMap->layersCount;
    level->map = tiledMap;
	level->columns = level->size / level->height;
    return level;
}


void renderText(TextFont* t, SDL_Renderer* renderer, int x, int y, int w, int h) {
    SDL_Rect renderQuad = {x, y, w, h};
    SDL_RenderCopy(renderer, t->texture->mTexture, NULL, &renderQuad);
}


void changeText(TextFont* t, SDL_Renderer* renderer, char* text) {
    SDL_DestroyTexture(t->texture->mTexture);
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
