#include "level.h"

#include <stdio.h>

#include <libxml/parser.h>
#include <libxml/xmlIO.h>
#include <libxml/xinclude.h>
#include <libxml/tree.h>



void parseStory(xmlDocPtr doc, xmlNodePtr cur) {
	xmlChar* key;
	xmlChar* encoding;
	cur = cur->xmlChildrenNode;
	while (cur != NULL) {
	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"data"))) {
			encoding = xmlGetProp(cur,  (const xmlChar *) "encoding");
			printf("encoding: %s\n", encoding);
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
		    printf("Data: %s\n", key);
		    xmlFree(key);
 	    }
		cur = cur->next;
	}
    return;
}


void parsing(const char* fileName) {
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
			printf("Tileset: firstgrid=%s, source=%s\n", tilesetFirstGid, tilesetSource);
		}
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"layer"))) {
			layerId = xmlGetProp(cur,  (const xmlChar *) "id");
			layerName = xmlGetProp(cur,  (const xmlChar *) "name");
			layerWidth = xmlGetProp(cur,  (const xmlChar *) "width");
			layerHeight = xmlGetProp(cur,  (const xmlChar *) "height");
			printf("Layer: id=%s, name=%s, width=%s, height=%s\n", layerId, layerName, layerWidth, layerHeight);
			parseStory(doc, cur);
		}
	    cur = cur->next;
	}
}
