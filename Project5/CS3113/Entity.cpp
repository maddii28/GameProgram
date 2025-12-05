#include "Entity.h"
#include <cmath>

Entity::Entity()
: mPosition{0,0}, mMovement{0,0}, mVelocity{0,0}, mAcceleration{0,0},
  mScale{DEFAULT_SIZE, DEFAULT_SIZE}, mColliderDimensions{DEFAULT_SIZE, DEFAULT_SIZE},
  mTexture{}, mTextureType{SINGLE}, mSpriteSheetDimensions{},
  mDirection{RIGHT}, mAnimationAtlas{}, mAnimationIndices{},
  mFrameSpeed{DEFAULT_FRAME_SPEED}, mSpeed{DEFAULT_SPEED},
  mAngle{0}, mEntityType{NONE}, mMode{PHYSICS}
{ }

Entity::Entity(Vector2 position, Vector2 scale, const char *textureFilepath,
    EntityType entityType, EntityMode mode)
: mPosition{position}, mMovement{0,0}, mVelocity{0,0}, mAcceleration{0,0},
  mScale{scale}, mColliderDimensions{scale},
  mTexture{LoadTexture(textureFilepath)}, mTextureType{SINGLE},
  mDirection{RIGHT}, mAnimationAtlas{}, mAnimationIndices{},
  mFrameSpeed{DEFAULT_FRAME_SPEED}, mSpeed{DEFAULT_SPEED},
  mAngle{0}, mEntityType{entityType}, mMode{mode}
{ }

Entity::Entity(Vector2 position, Vector2 scale, const char *textureFilepath,
        TextureType textureType, Vector2 spriteSheetDimensions,
        std::map<Direction, std::vector<int>> animationAtlas,
        EntityType entityType, EntityMode mode)
: mPosition{position}, mMovement{0,0}, mVelocity{0,0}, mAcceleration{0,0},
  mScale{scale}, mColliderDimensions{scale},
  mTexture{LoadTexture(textureFilepath)}, mTextureType{textureType},
  mSpriteSheetDimensions{spriteSheetDimensions},
  mAnimationAtlas{animationAtlas},
  mDirection{RIGHT},
  mAnimationIndices{ animationAtlas.at(RIGHT) },
  mFrameSpeed{DEFAULT_FRAME_SPEED}, mSpeed{DEFAULT_SPEED},
  mAngle{0}, mEntityType{entityType}, mMode{mode}
{ }

Entity::~Entity() { UnloadTexture(mTexture); }

void Entity::animate(float deltaTime)
{
    if (mAnimationIndices.empty()) return;

    mAnimationTime += deltaTime;
    float frameDuration = 1.0f / mFrameSpeed;

    if (mAnimationTime >= frameDuration) {
        mAnimationTime = 0;
        mCurrentFrameIndex = (mCurrentFrameIndex + 1) % mAnimationIndices.size();
    }
}

void Entity::update(float dt, Entity *player, Map *map,
                    Entity *collidableEntities, int collisionCount)
{
    if (mEntityStatus == INACTIVE) return;

    // ----------------------------------------
    // ANIMATION-ONLY MODE (dance game)
    // ----------------------------------------
    if (mMode == ANIMATION_ONLY) {
    // Only animate if direction is NOT idle
        if (mDirection != IDLE)
            animate(dt);
        return;
    }


    if (mEntityType == NPC)
        AIActivate(player);

    resetColliderFlags();

    mVelocity.x = mMovement.x * mSpeed;

    mVelocity.x += mAcceleration.x * dt;
    mVelocity.y += mAcceleration.y * dt;

    if (mIsJumping) {
        mIsJumping = false;
        mVelocity.y -= mJumpingPower;
    }

    mPosition.y += mVelocity.y * dt;
    checkCollisionY(collidableEntities, collisionCount);
    checkCollisionY(map);

    mPosition.x += mVelocity.x * dt;
    checkCollisionX(collidableEntities, collisionCount);
    checkCollisionX(map);

    if (mTextureType == ATLAS && (mMovement.x != 0 || mMovement.y != 0))
        animate(dt);
}

void Entity::render()
{
    if (mEntityStatus == INACTIVE) return;

    Rectangle src;

    // SELECT CORRECT FRAME (single or atlas)
    if (mTextureType == SINGLE) {
        src = { 0, 0, (float)mTexture.width, (float)mTexture.height };
    } else {
        src = getUVRectangle(
            &mTexture,
            mAnimationIndices[mCurrentFrameIndex],
            mSpriteSheetDimensions.x,
            mSpriteSheetDimensions.y
        );
    }

    Rectangle dest = {
        mPosition.x,
        mPosition.y,
        mScale.x,
        mScale.y
    };

    Vector2 origin = {
        mScale.x / 2.0f,
        mScale.y / 2.0f
    };

    DrawTexturePro(mTexture, src, dest, origin, mAngle, WHITE);
}

void Entity::checkCollisionY(Entity *collidableEntities, int collisionCheckCount)
{
    // Unused in dance mode
}

void Entity::checkCollisionY(Map *map)
{
    // Unused in dance mode
}

void Entity::checkCollisionX(Entity *collidableEntities, int collisionCheckCount)
{
    // Unused in dance mode
}

void Entity::checkCollisionX(Map *map)
{
    // Unused in dance mode
}

void Entity::AIActivate(Entity *target)
{
    // Unused for a dance game (AI only switches frames randomly)
}

void Entity::AIWander()
{
    // Not needed
}

void Entity::AIFollow(Entity *target)
{
    // Not needed
}


// The collision functions remain unchanged (only used in PHYSICS mode)
