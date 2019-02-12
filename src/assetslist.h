#ifndef ASSETSLIST_H_
#define ASSETSLIST_H_
#pragma once

#include "textures.h"


#define ASSETSLIST_SHRINK_MANUAL (1 << 0)
#define ASSETSLIST_SHRINK_AFTER_DELETE (1 << 1)


typedef struct AssetsList {
  unsigned int size;
  unsigned int maxSize;
  unsigned int chunkSize;
  unsigned int sizeOfType;
  unsigned int flags;
  Texture** textures;
} AssetsList;



AssetsList* createAssetsList(unsigned int initialSize, unsigned int chunkSize, unsigned int sizeOfType, unsigned int flags);
int addTextureToAssets(AssetsList* list, Texture* item);
int clearAssetsList(AssetsList** list);
Texture* getTextureFromAssets(AssetsList* list, unsigned int index);


#endif
