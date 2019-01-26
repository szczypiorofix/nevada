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
		//printf("Malloc error !!!\n");
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


TiledMap* parseMap(const char* fileName) {
	printf("Parsing xml %s file.\n", fileName);
	
	xmlDocPtr doc;
	xmlNodePtr cur;

	doc = xmlParseFile("res/images/map.tmx");
    if (doc == NULL ) {
		fprintf(stderr,"Document not parsed successfully. \n");
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
	tiledMap->width = xmlCharToInt(xmlGetProp(cur, (const xmlChar *) "width"));
	tiledMap->height = xmlCharToInt(xmlGetProp(cur, (const xmlChar *) "height"));
	tiledMap->tileWidth = xmlCharToInt(xmlGetProp(cur, (const xmlChar *) "tilewidth"));
	tiledMap->tileHeight = xmlCharToInt(xmlGetProp(cur, (const xmlChar *) "tileheight"));

    cur = cur->xmlChildrenNode;

	xmlChar* layerId;
	xmlChar* layerName;
	xmlChar* layerWidth;
	xmlChar* layerHeight;

	xmlChar* tilesetFirstGid;
	xmlChar* tilesetSource;

    while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"tileset"))) {
			tilesetFirstGid = xmlGetProp(cur, (const xmlChar *) "firstgid");
			tilesetSource = xmlGetProp(cur, (const xmlChar *) "source");
			printf("Tileset: firstgid=%s, source=%s\n", tilesetFirstGid, tilesetSource);
		}
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"layer"))) {
			layerId = xmlGetProp(cur,  (const xmlChar *) "id");
			layerName = xmlGetProp(cur,  (const xmlChar *) "name");
			layerWidth = xmlGetProp(cur,  (const xmlChar *) "width");
			layerHeight = xmlGetProp(cur,  (const xmlChar *) "height");
			

			printf("Layer id: %i\n", xmlCharToInt(layerId));
			printf("Layer name: %s\n", layerName);
			printf("Layer width: %i\n", xmlCharToInt(layerWidth));
			printf("Layer height: %i\n", xmlCharToInt(layerHeight));
			parseData(doc, cur);
		}
	    cur = cur->next;
	}
	return tiledMap;
}
