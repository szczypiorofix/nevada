#include <stdio.h>
#include <stdlib.h>
#include "assetslist.h"


// AssetsList* createAssetsList(unsigned int initialSize, unsigned int chunkSize, unsigned int flags);
// int addTextureToList(AssetsList* list, Texture* texture);
// int clearAssetsList(AssetsList** list);
// Texture* getTexture(AssetsList* list, unsigned int index);


// Texture* getTexture(AssetsList* list, unsigned int index) {
//     if (list == NULL) {
//         fprintf(stderr, "Cannot allocate memory for a new AssetsList !\n");
//         return NULL;
//     } else if (list->size == 0) {
//         fprintf(stderr, "Array is empty !\n");
//         return NULL;
//     }
//     return list->textures[index];
// }

// AssetsList* createAssetsList(unsigned int initialSize, unsigned int chunkSize, unsigned int flags) {
//     AssetsList* list = malloc(sizeof(AssetsList));
//     if (list == NULL) {
//         fprintf(stderr, "Cannot allocate memory for a new AssetsList !\n");
//         return NULL;
//     }

//     list->size = 0;
//     list->chunkSize = chunkSize;
//     list->maxSize = initialSize;
//     list->sizeOfType = sizeof(Texture);
//     list->flags = flags;

//     list->textures = malloc(sizeof(Texture*) * initialSize);

//     printf("Allocating data size: %i\n", list->sizeOfType);

//     for (unsigned int i = 0; i < list->maxSize; i++) {
//         list->textures[list->size] = malloc(sizeof(Texture*));
//         if (list->textures[i] == NULL) {
//             fprintf(stderr, "Cannot allocate memory for %i (int) elements in ArrayList !\n", initialSize);
//             return NULL;
//         }
//     }

//     return list;
// }



// int addTextureToList(AssetsList* list, Texture* texture) {
//     if (list == NULL) {
//         fprintf(stderr, "ArrayList cannot be NULL !!!\n");
//         return 0;
//     }
//     printf("Adding value item %s to ArrayList...\n", texture->name);
//     if (list->size < list->maxSize) {
//         list->textures[list->size] = texture;
//         list->size++;
//     }
//     return 1;
// }


// int clearAssetsList(AssetsList** list) {
//     if (*list == NULL) {
//         fprintf(stderr, "AssetsList cannot be NULL !!!\n");
//         return 0;
//     }
//     printf("Clearing list...\n");
//     for (unsigned int i = 0; i < (*list)->maxSize; i++) {
//         free( (*list)->textures[i] );
//         (*list)->textures[i] = NULL;
//     }
//     free((*list)->textures);
//     (*list)->textures = NULL;
//     printf("Free list ... \n");
//     free(*list);
//     *list = NULL;
//     return 1;
// }
