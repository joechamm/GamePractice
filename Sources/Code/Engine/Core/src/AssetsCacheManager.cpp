#include "AssetsCacheManager.hpp"
#include "Core/ETLogger.hpp"

#include <cassert>

namespace {

const float DEFAULT_LIFETIME = 5.f;

} // namespace

AssetsCacheManager::AssetsCacheManager() :
    assetsLifetime(DEFAULT_LIFETIME) {
}

AssetsCacheManager::~AssetsCacheManager() {
}

bool AssetsCacheManager::init() {
    ETNode<ETAssetsCacheManager>::connect(getEntityId());
    ETNode<ETAssetsUpdateTask>::connect(getEntityId());
    return true;
}

void AssetsCacheManager::deinit() {
    ETNode<ETAssetsCacheManager>::disconnect();
    ETNode<ETAssetsUpdateTask>::disconnect();
}

void AssetsCacheManager::ET_updateAssets() {
    auto nowT = TimePoint::GetNowTime();
    auto dt = nowT.getSecondsElapsedFrom(lastTickT);
    lastTickT = nowT;

    auto it = assetsCacheMap.begin();
    while(it != assetsCacheMap.end()) {
        AssetCacheNode& node = it->second;
        node.lifetime -= dt;
        if(node.lifetime < 0.f) {
            it = assetsCacheMap.erase(it);
        } else {
            ++it;
        }
    }
}

Buffer AssetsCacheManager::ET_getAssetFromCache(const char* assetName) {
    auto it = assetsCacheMap.find(assetName);
    if(it == assetsCacheMap.end()) {
        return Buffer();
    }
    AssetCacheNode& node = it->second;
    node.lifetime = assetsLifetime;
    return node.buff;
}

void AssetsCacheManager::ET_putAssetToCache(const char* assetName, const Buffer& buff) {
    if(!assetName || !assetName[0]) {
        assert(false && "empty asset name");
        return;
    }
    if(!buff) {
        assert(false && "invalid asset buffer");
        return;
    }
    auto it = assetsCacheMap.find(assetName);
    if(it != assetsCacheMap.end()) {
        LogWarning("[AssetsCacheManager::ET_putAssetToCache] Double try to put asset into cache: '%s'", assetName);
        return;
    }
    if(assetsLifetime <= 0.f) {
        return;
    }

    assetsCacheMap[assetName] = {buff, assetsLifetime};
}

void AssetsCacheManager::ET_setCacheLifetime(float seconds) {
    if(seconds < 0.f) {
        assetsCacheMap.clear();
    }
    assetsLifetime = seconds;
}

float AssetsCacheManager::ET_getCacheLifetime() const {
    return assetsLifetime;
}

void AssetsCacheManager::ET_clear() {
    assetsCacheMap.clear();
}