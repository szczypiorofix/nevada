#include <libxml/parser.h>
#include <libxml/xmlIO.h>
#include <libxml/xinclude.h>
#include <libxml/tree.h>

#include "font.h"
#include "engine.h"


// ---------------------- CONSTATNS ------------------------

const float FONT_SPACE_WIDTH = 0.06f;


// ------------------ FORWARD DECLARATION ------------------
BitmapFont* parseBitmapFile(char* pngFileName, char* xmlFileName, SDL_Renderer* renderer);


BitmapFont* parseBitmapFile(char* pngFileName, char* xmlFileName, SDL_Renderer* renderer) {

	xmlDocPtr doc;
	xmlNodePtr cur;

	doc = xmlParseFile(xmlFileName);
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

    if (xmlStrcmp(cur->name, (const xmlChar *) "font")) {
		fprintf(stderr,"Document of the wrong type, root node != font !!! \n");
		xmlFreeDoc(doc);
		exit(0);
	}

	BitmapFont* bitmapFont = malloc(sizeof (BitmapFont));
	if (bitmapFont == NULL) {
		printf("Malloc (creating TiledMap) error !!!\n");
		return NULL;
	}
	bitmapFont->width = xmlCharToInt(xmlGetProp(cur, (const xmlChar *) "width"));
    bitmapFont->height = xmlCharToInt(xmlGetProp(cur, (const xmlChar *) "height"));
    bitmapFont->space = xmlCharToInt(xmlGetProp(cur, (const xmlChar *) "space"));

	cur = cur->xmlChildrenNode;

	bitmapFont->itemsCount = 0;
	xmlNodePtr itemsNode = cur;

	while(itemsNode != NULL) {
		if ((!xmlStrcmp(itemsNode->name, (const xmlChar *)"item"))) {
			bitmapFont->itemsCount++;
		}
		itemsNode = itemsNode->next;
	}

	BitmapFontChar bitmapFontChar[bitmapFont->itemsCount];
	int c = 0;
	itemsNode = cur;
	while(itemsNode != NULL) {
		if ((!xmlStrcmp(itemsNode->name, (const xmlChar *)"item"))) {
			
			short ascii = 0;
			short ucode = 0;
			unsigned char raw = 0;
			short bottom = 0;
			short top = 0;
			short x = 0;
			short y = 0;
			short width = 0;
			short height = 0;
			short trailing = 0;
			short leading = 0;

			ascii = xmlGetProp(itemsNode, (const xmlChar *) "ascii") != NULL ? xmlCharToInt( xmlGetProp(itemsNode, (const xmlChar *) "ascii") ) : -1;
			ucode = xmlGetProp(itemsNode, (const xmlChar *) "ucode") != NULL ? xmlCharToInt( xmlGetProp(itemsNode, (const xmlChar *) "ucode") ) : -1;
			raw = '.';
			bottom = xmlGetProp(itemsNode, (const xmlChar *) "bottom") != NULL ? xmlCharToInt( xmlGetProp(itemsNode, (const xmlChar *) "bottom") ) : -1;
			top = xmlGetProp(itemsNode, (const xmlChar *) "top") != NULL ? xmlCharToInt( xmlGetProp(itemsNode, (const xmlChar *) "top") ) : -1;
			x = xmlGetProp(itemsNode, (const xmlChar *) "x") != NULL ? xmlCharToInt( xmlGetProp(itemsNode, (const xmlChar *) "x") ) : -1;
			y = xmlGetProp(itemsNode, (const xmlChar *) "y") != NULL ? xmlCharToInt( xmlGetProp(itemsNode, (const xmlChar *) "y") ) : -1;
			width = xmlGetProp(itemsNode, (const xmlChar *) "width") != NULL ? xmlCharToInt( xmlGetProp(itemsNode, (const xmlChar *) "width") ) : -1;
			height = xmlGetProp(itemsNode, (const xmlChar *) "height") != NULL ? xmlCharToInt( xmlGetProp(itemsNode, (const xmlChar *) "height") ) : -1;
			trailing = xmlGetProp(itemsNode, (const xmlChar *) "trailing") != NULL ? xmlCharToInt( xmlGetProp(itemsNode, (const xmlChar *) "trailing") ) : -1;
			leading = xmlGetProp(itemsNode, (const xmlChar *) "leading") != NULL ? xmlCharToInt( xmlGetProp(itemsNode, (const xmlChar *) "leading") ) : -1;

			BitmapFontChar tempItem = {
				.ascii = ascii,
				.ucode = ucode,
				.raw = raw,
				.bottom = bottom,
				.top = top,
				.x = x,
				.y = y,
				.width = width,
				.height = height,
				.trailing = trailing,
				.leading = leading
			};
			bitmapFontChar[c] = tempItem;
			c++;
		}
		itemsNode = itemsNode->next;
	}

    printf("Items count: %i\n", bitmapFont->itemsCount);

	bitmapFont->bitmapFontChar = bitmapFontChar;

	// IMAGE:

	bitmapFont->bitmapFontImage = loadSpriteSheet(pngFileName, renderer, 16, 16);

	return bitmapFont;
}


