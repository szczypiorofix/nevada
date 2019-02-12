#include <stdio.h>
#include <stdlib.h>
#include "assetslist.h"


AssetsList* createAssetsList(unsigned int initialSize, unsigned int chunkSize, unsigned int sizeOfType, unsigned int flags);
int addTextureToAssets(AssetsList* list, Texture* item);
int clearAssetsList(AssetsList** list);
Texture* getTextureFromAssets(AssetsList* list, unsigned int index);




Texture* getTextureFromAssets(AssetsList* list, unsigned int index) {
    if (list == NULL) {
        fprintf(stderr, "ERROR !!! AssetsList cannot be null !!!\n");
        return 0;
    }
    if (index > list->size - 1) {
        fprintf(stderr, "AssetsList ERROR !!! Index %i out of array size (%i)!\n", index, list->size - 1);
        return 0;
    }
    return list->textures[index];
}

int addTextureToAssets(AssetsList* list, Texture* item) {
    if (list == NULL) {
        fprintf(stderr, "AssetsList cannot be NULL !!!\n");
        return 0;
    }
    printf("Adding value item %s to AssetsList...\n", item->name);
    if (list->size < list->maxSize) {
        list->textures[list->size] = item;
        list->size++;
        return 1;
    }
    return 0;
}



int clearAssetsList(AssetsList** list) {
    if (list == NULL) {
        fprintf(stderr, "AssetsList cannot be NULL !!!\n");
        return 0;
    }
    printf("Clearing data list...\n");
    free((*list)->textures);
    (*list)->textures = NULL;

    printf("Free list ... \n");
    free(*list);
    *list = NULL;
    return 1;
}

AssetsList* createAssetsList(unsigned int initialSize, unsigned int chunkSize, unsigned int sizeOfType, unsigned int flags) {
    AssetsList* list = malloc(sizeof(AssetsList));
    if (list == NULL) {
        fprintf(stderr, "Cannot allocate memory for a new AssetsList !\n");
        return NULL;
    }

    list->size = 0;
    list->chunkSize = chunkSize;
    list->maxSize = initialSize;
    list->sizeOfType = sizeOfType;
    list->flags = flags;

    int dataSize = sizeof(void*) * initialSize;
    int pieceSize = sizeOfType;

    printf("Piece size: %i\n", pieceSize);
    printf("Data size: %i\n", dataSize);

    list->textures = malloc(list->maxSize * sizeof(Texture));

    return list;
}
