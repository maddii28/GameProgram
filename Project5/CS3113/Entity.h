#ifndef ENTITY_H
#define ENTITY_H

#include "Map.h"

enum Direction    { IDLE, LEFT, UP, RIGHT, DOWN };
enum EntityStatus { ACTIVE, INACTIVE };
enum EntityType   { PLAYER, BLOCK, PLATFORM, NPC, NONE };
enum AIType       { WANDERER, FOLLOWER };
enum AIState      { WALKING, AI_IDLE, FOLLOWING };

enum EntityMode { PHYSICS, ANIMATION_ONLY };

class Entity
{
private:
    Vector2 mPosition;
    Vector2 mMovement;
    Vector2 mVelocity;
    Vector2 mAcceleration;

    Vector2 mScale;
    Vector2 mColliderDimensions;

    Texture2D mTexture;
    TextureType mTextureType;
    Vector2 mSpriteSheetDimensions;

    std::map<Direction, std::vector<int>> mAnimationAtlas;
    std::vector<int> mAnimationIndices;
    Direction mDirection;
    int mFrameSpeed;

    int mCurrentFrameIndex = 0;
    float mAnimationTime = 0.0f;

    bool mIsJumping = false;
    float mJumpingPower = 0.0f;

    int mSpeed;
    float mAngle;

    bool mIsCollidingTop    = false;
    bool mIsCollidingBottom = false;
    bool mIsCollidingRight  = false;
    bool mIsCollidingLeft   = false;

    EntityStatus mEntityStatus = ACTIVE;
    EntityType   mEntityType;

    AIType  mAIType;
    AIState mAIState;

    EntityMode mMode;

    void checkCollisionY(Entity *collidableEntities, int collisionCheckCount);
    void checkCollisionY(Map *map);
    void checkCollisionX(Entity *collidableEntities, int collisionCheckCount);
    void checkCollisionX(Map *map);

    void resetColliderFlags() {
        mIsCollidingTop = mIsCollidingBottom = false;
        mIsCollidingRight = mIsCollidingLeft = false;
    }

    void animate(float deltaTime);
    
    void AIActivate(Entity *target);
    void AIWander();
    void AIFollow(Entity *target);

public:
    static constexpr int   DEFAULT_SIZE          = 250;
    static constexpr int   DEFAULT_SPEED         = 350;
    static constexpr int   DEFAULT_FRAME_SPEED   = 14;
    static constexpr float Y_COLLISION_THRESHOLD = 0.5f;

    Entity();

    // NORMAL SINGLE TEXTURE
    Entity(Vector2 position, Vector2 scale, const char *textureFilepath,
           EntityType entityType, EntityMode mode = PHYSICS);

    // ATLAS (SPRITE SHEET)
    Entity(Vector2 position, Vector2 scale, const char *textureFilepath,
           TextureType textureType, Vector2 spriteSheetDimensions,
           std::map<Direction, std::vector<int>> animationAtlas,
           EntityType entityType, EntityMode mode = PHYSICS);

    ~Entity();

    void update(float deltaTime, Entity *player, Map *map,
                Entity *collidableEntities, int collisionCheckCount);

    void render();
    void displayCollider();

    void moveUp()    { mMovement.y = -1; mDirection = UP; }
    void moveDown()  { mMovement.y =  1; mDirection = DOWN; }
    void moveLeft()  { mMovement.x = -1; mDirection = LEFT; }
    void moveRight() { mMovement.x =  1; mDirection = RIGHT; }
    void resetMovement() { mMovement = {0,0}; }

    void activate()   { mEntityStatus = ACTIVE; }
    void deactivate() { mEntityStatus = INACTIVE; }
    
    void setSprite(Texture2D tex) {
        mTexture = tex;
    }
    void setPosition(Vector2 p) { mPosition = p; }

    void jump() { mIsJumping = true; }

    void setDirection(Direction d) {
        if (mDirection != d) {
        mDirection = d;
        mCurrentFrameIndex = 0;   // restart animation
        mAnimationTime = 0;
        mAnimationIndices = mAnimationAtlas.at(d);
    }
}

    bool isActive() const { return mEntityStatus == ACTIVE; }

    Vector2 getPosition() const           { return mPosition;              }
    Vector2 getScale() const              { return mScale;                 }
    EntityType getEntityType() const      { return mEntityType;            }
};

#endif
