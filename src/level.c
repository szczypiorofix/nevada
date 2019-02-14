#include <string.h>

#include <libxml/parser.h>
#include <libxml/xmlIO.h>
#include <libxml/xinclude.h>
#include <libxml/tree.h>

#include "level.h"
#include "defines.h"




// ------------------ FORWARD DECLARATION ------------------

Level* getLevel(void);
int xmlCharToInt(const xmlChar a[]);
int* parseData(xmlDocPtr doc, xmlNodePtr cur);
TiledMap* parseMap(const char* fileName);
void freeTiledMap(TiledMap* tiledMap);
TileSetSource* getTileSetSource(const char* tsxFileName);


// ------------------ "PUBLIC" FUNCTIONS ------------------

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

Level* getLevel(void) {
	// char tmp[50] = DIR_RES_IMAGES;
	// strcat(tmp, "map.tmx");
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

	// FREE THAT CONTENT !!! Huge leak!
    // level->content = malloc(sizeof *level->content * level->size);
    // if (level->content == NULL) return NULL;

    level->content = tiledMap->layer;
    level->layers = tiledMap->layersCount;
    level->map = tiledMap;
    
    return level;
}



// ------------------ "PRIVATE" FUNCTIONS ------------------

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
	// printf("Chars: %i, Numbers: %u, cleanCharsNumber: %u\n", c, numbers, cleanCharsNumber);
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
