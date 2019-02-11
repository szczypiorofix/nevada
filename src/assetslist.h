#ifndef ASSETSLIST_H_
#define ASSETSLIST_H_
#pragma once

#include "textures.h"

typedef struct AssetsList {
  unsigned int size;
  unsigned int maxSize;
  unsigned int chunkSize;
  unsigned int sizeOfType;
  unsigned int flags;
  Texture** textures;
} AssetsList;



// AssetsList* createAssetsList(unsigned int initialSize, unsigned int chunkSize, unsigned int flags);
// int addTextureToList(AssetsList* list, Texture* texture);
// int clearAssetsList(AssetsList** list);
// Texture* getTexture(AssetsList* list, unsigned int index);


#endif
