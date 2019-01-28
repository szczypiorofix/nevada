#include "level.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libxml/parser.h>
#include <libxml/xmlIO.h>
#include <libxml/xinclude.h>
#include <libxml/tree.h>


int xmlCharToInt(const xmlChar a[]);
int* parseData(xmlDocPtr doc, xmlNodePtr cur);
TiledMap* parseMap(const char* fileName);
void freeTiledMap(TiledMap* tiledMap);


void freeTiledMap(TiledMap* tiledMap) {
	// Free TileSetSourceImage, TileSetSource, TileLayer etc...
	
	// free layers
	for (int i = 0; i < tiledMap->layersCount; i++) {
		free(tiledMap->layer[i].encoding);
		free(tiledMap->layer[i].name);
		free(tiledMap->layer[i].data);
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
	printf("Chars: %i, Numbers: %u, cleanCharsNumber: %u\n", c, numbers, cleanCharsNumber);
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
	int* n = malloc(sizeof(int) * numbers);
	if (n == NULL) {
		printf("Malloc 'numbers' error !!!\n");
		return NULL;
	}
	while(ptr != NULL) {
		//printf("Number: %s\n", ptr);
		n[c] = stringToInt(ptr);
		ptr = strtok(NULL, delim);
		c++;
	}

	return n;
}

int* parseData(xmlDocPtr doc, xmlNodePtr cur) {
	xmlChar* key;
	xmlChar* encoding;
	cur = cur->xmlChildrenNode;
	int* arr = NULL;
	while (cur != NULL) {
	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"data"))) {
			encoding = xmlGetProp(cur,  (const xmlChar *) "encoding");
			printf("encoding: %s\n", encoding);
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			arr = convertDataStringToArray(key);
			printf("Array: %i, %i, %i, %i\n", arr[0], arr[1], arr[2], arr[3]);
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
	printf("Parsing tsx file: %s\n", tsxFileName);
	
	xmlDocPtr tsxDoc;
	xmlNodePtr tsxCurNode;

	tsxDoc = xmlParseFile("res/images/grassland.tsx");
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

	printf("TileSet props: name: %s, tileWidth: %i, tileHeight: %i, tileCount: %i, columns: %i\n", tileSetSource->name, tileSetSource->tileWidth, tileSetSource->tileHeight, tileSetSource->tileCount, tileSetSource->columns);
	
	TileSetSourceImage* tileSetSourceImage = malloc(sizeof(TileSetSourceImage));
	if (tileSetSourceImage == NULL) {
		printf("Malloc (creating TileSetSourceImage) error !!!\n");
		return NULL;
	}

	tsxCurNode = tsxCurNode->xmlChildrenNode;
	while (tsxCurNode != NULL) {
		if ((!xmlStrcmp(tsxCurNode->name, (const xmlChar *) "image"))) {
			tileSetSourceImage->source = (char *) xmlGetProp(tsxCurNode, (const xmlChar *) "source");
			tileSetSourceImage->width = xmlCharToInt(xmlGetProp(tsxCurNode, (const xmlChar *) "width"));
			tileSetSourceImage->height = xmlCharToInt(xmlGetProp(tsxCurNode, (const xmlChar *) "height"));
		}
		tsxCurNode = tsxCurNode->next;
	}

	printf("TileSetImage props: source: %s, width: %i, height: %i\n", tileSetSourceImage->source, tileSetSourceImage->width, tileSetSourceImage->height);

	tileSetSource->tileSetSourceImage = tileSetSourceImage;
	
	return tileSetSource;
}

TiledMap* parseMap(const char* fileName) {
	printf("Parsing tmx %s file.\n", fileName);
	
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


	TileSet* tileSet = malloc(sizeof(TileSet));
	if (tileSet == NULL) {
		printf("Malloc (creating TileSetSource) error !!!\n");
		return NULL;
	}

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
	
	printf("Layers count: %i, tileset count: %i\n", layersCount, tileSetCount);

	int lc = 0, tc = 0;

	Layer* layers = malloc(sizeof(Layer) * layersCount);
	if (layers == NULL) {
		printf("Malloc (creating Layer) error !!!\n");
		return NULL;
	}


    while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"tileset"))) {
			tileSet->firstGid = xmlCharToInt(xmlGetProp(cur, (const xmlChar *) "firstgid"));
			tileSet->source = (char *)xmlGetProp(cur, (const xmlChar *) "source");
			printf("Tileset: firstgid=%i, source=%s\n", tileSet->firstGid, tileSet->source);
			printf("Parsing tileSet file: %s.\n", tileSet->source);
			TileSetSource* tss = getTileSetSource(tileSet->source);
			tileSet->tileSetSource = tss;
			tc++;
		}
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"layer"))) {
			layers[lc].id = xmlCharToInt(xmlGetProp(cur,  (const xmlChar *) "id"));
			layers[lc].name = (char *)xmlGetProp(cur,  (const xmlChar *) "name");
			layers[lc].width = xmlCharToInt(xmlGetProp(cur,  (const xmlChar *) "width"));
			layers[lc].height = xmlCharToInt(xmlGetProp(cur,  (const xmlChar *) "height"));
			printf("Layer %i id: %i\n", lc, layers[lc].id);
			printf("Layer %i name: %s\n", lc, layers[lc].name);
			printf("Layer %i width: %i\n", lc, layers[lc].width);
			printf("Layer %i height: %i\n", lc, layers[lc].height);
			layers[lc].data = parseData(doc, cur);
			lc++;
		}
	    cur = cur->next;
	}
	tiledMap->layer = layers;
	tiledMap->layersCount = lc;

	return tiledMap;
}
