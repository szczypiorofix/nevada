#include <string.h>

#include <libxml/parser.h>
#include <libxml/xmlIO.h>
#include <libxml/xinclude.h>
#include <libxml/tree.h>

#include "level.h"
#include "defines.h"




// ------------------ FORWARD DECLARATION ------------------

Level* getLevel();
int xmlCharToInt(const xmlChar a[]);
int* parseData(xmlDocPtr doc, xmlNodePtr cur);
TiledMap* parseMap(const char* fileName);
void freeTiledMap(TiledMap* tiledMap);
TileSetSource* getTileSetSource(const char* tsxFileName);
TiledObject* getTiledObjects(xmlNodePtr cur, int tiledObjectsCounter);
int countTiledObjects(xmlNodePtr cur);


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

Level* getLevel() {
	// char tmp[50] = DIR_RES_IMAGES;
	// strcat(tmp, "map.tmx");
    TiledMap* tiledMap = parseMap("res/images/worldmap.tmx");
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



// ------------------ "PRIVATE" FUNCTIONS ------------------

TiledMap* parseMap(const char* fileName) {
	
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
	
	TileSet* tileSet = malloc(sizeof(TileSet) * tileSetCount);
	if (tileSet == NULL) {
		printf("Malloc (creating TileSet) error !!!\n");
		return NULL;
	}

	int lc = 0, tc = 0, ogc = 0;

	Layer* layers = malloc(sizeof(Layer) * layersCount);
	if (layers == NULL) {
		printf("Malloc (creating Layer) error !!!\n");
		return NULL;
	}

	ObjectGroup* objectGroups = malloc(sizeof(ObjectGroup) * objectGroupCount);
	if (layers == NULL) {
		printf("Malloc (creating ObjectGroup) error !!!\n");
		return NULL;
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
		return NULL;
	}
	while(ptr != NULL) {
		numArr[c] = stringToInt(ptr);
		ptr = strtok(NULL, delim);
		c++;
	}
	
	return numArr;
}


int countTiledObjects(xmlNodePtr cur) {
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

TiledObject* getTiledObjects(xmlNodePtr cur, int tiledObjectsCounter) {
	
	TiledObject* objects = malloc(sizeof(TiledObject) * tiledObjectsCounter);
	if (objects == NULL) {
		printf("Malloc (creating TiledObject) error !!!\n");
		return NULL;
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


			char tmp[50] = DIR_RES_IMAGES;
			strcat(tmp, objects[i].template);
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
					// tilesets
					objects[i].source = (char *)xmlGetProp(txCurNode, (const xmlChar *) "source");
					objects[i].firstGid = xmlCharToInt(xmlGetProp(txCurNode, (const xmlChar *) "firstgid"));
				}
				if (!(xmlStrcmp(txCurNode->name, (const xmlChar *)"object"))) {
					// objects
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


int* parseData(xmlDocPtr doc, xmlNodePtr cur) {
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

TileSetSource* getTileSetSource(const char* tsxFileName) {
	
	TileSetSource* tileSetSource = malloc(sizeof(TileSetSource));
	if (tileSetSource == NULL) {
		printf("Malloc (creating TileSetSource) error !!!\n");
		return NULL;
	}
	char tmp[50] = DIR_RES_IMAGES;
	strcat(tmp, tsxFileName);
	
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
