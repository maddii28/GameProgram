#include "Entity.h"

Entity::Entity() : mPosition {0.0f, 0.0f}, mMovement {0.0f, 0.0f}, 
                   mVelocity {0.0f, 0.0f}, mAcceleration {0.0f, 0.0f},
                   mScale {DEFAULT_SIZE, DEFAULT_SIZE},
                   mColliderDimensions {DEFAULT_SIZE, DEFAULT_SIZE}, 
                   mTexture {NULL}, mTextureType {SINGLE}, mAngle {0.0f},
                   mSpriteSheetDimensions {}, mDirection {OPEN}, 
                   mAnimationAtlas {{}}, mAnimationIndices {}, mFrameSpeed {0},
                   mEntityType {PLAYER} { }

Entity::Entity(Vector2 position, Vector2 scale, const char *textureFilepath, 
    EntityType entityType) : mPosition {position}, mVelocity {0.0f, 0.0f}, 
    mAcceleration {0.0f, 0.0f}, mScale {scale}, mMovement {0.0f, 0.0f}, 
    mColliderDimensions {scale}, mTexture {LoadTexture(textureFilepath)}, 
    mTextureType {SINGLE}, mDirection {OPEN}, mAnimationAtlas {{}}, 
    mAnimationIndices {}, mFrameSpeed {0}, mSpeed {DEFAULT_SPEED}, 
    mAngle {0.0f}, mEntityType {entityType} { }

Entity::Entity(Vector2 position, Vector2 scale, const char *textureFilepath, 
        TextureType textureType, Vector2 spriteSheetDimensions, std::map<Direction, 
        std::vector<int>> animationAtlas, EntityType entityType) : 
        mPosition {position}, mVelocity {0.0f, 0.0f}, 
        mAcceleration {0.0f, 0.0f}, mMovement { 0.0f, 0.0f }, mScale {scale},
        mColliderDimensions {scale}, mTexture {LoadTexture(textureFilepath)}, 
        mTextureType {ATLAS}, mSpriteSheetDimensions {spriteSheetDimensions},
        mAnimationAtlas {animationAtlas}, mDirection {OPEN},
        mAnimationIndices {animationAtlas.at(OPEN)}, 
        mFrameSpeed {DEFAULT_FRAME_SPEED}, mAngle { 0.0f }, 
        mSpeed { DEFAULT_SPEED }, mEntityType {entityType} { }

Entity::~Entity() { UnloadTexture(mTexture); };

/**
 * Iterates through a list of collidable entities, checks for collisions with
 * the player entity, and resolves any vertical overlap by adjusting the 
 * player's position and velocity accordingly.
 * 
 * @param collidableEntities An array of pointers to `Entity` objects that 
 * represent the entities that the current `Entity` instance can potentially
 * collide with. The `collisionCheckCount` parameter specifies the number of
 * entities in the `collidableEntities` array that need to be checked for
 * collision.
 * @param collisionCheckCount The number of entities that the current entity
 * (`Entity`) should check for collisions with. This parameter specifies how
 * many entities are in the `collidableEntities` array that need to be checked
 * for collisions with the current entity.
 */
bool Entity::checkCollisionY(Entity *collidableEntity)
{     
        if (isColliding(collidableEntity))
        {
            // STEP 2: Calculate the distance between its centre and our centre
            //         and use that to calculate the amount of overlap between
            //         both bodies.
            float yDistance = fabs(mPosition.y - collidableEntity->mPosition.y);
            float yOverlap  = fabs(yDistance - (mColliderDimensions.y / 2.0f) - 
                              (collidableEntity->mColliderDimensions.y / 2.0f));
            
            // STEP 3: "Unclip" ourselves from the other entity, and zero our
            //         vertical velocity.
            if (mVelocity.y > 0) 
            {
                mPosition.y -= (3*yOverlap)/4;
                mVelocity.y  = 0;
                mIsCollidingBottom = true;
                return true;

            } else if (mVelocity.y < 0) 
            {
                mPosition.y += (3*yOverlap)/4;
                mVelocity.y  = 0;
                mIsCollidingTop = true;
            }
            return false;
        }
}


bool Entity::checkCollisionX(Entity *collidableEntity)
{
        if (isColliding(collidableEntity))
        {            
            // When standing on a platform, we're always slightly overlapping
            // it vertically due to gravity, which causes false horizontal
            // collision detections. So the solution I dound is only resolve X
            // collisions if there's significant Y overlap, preventing the 
            // platform we're standing on from acting like a wall.
            float yDistance = fabs(mPosition.y - collidableEntity->mPosition.y);
            float yOverlap  = fabs(yDistance - (mColliderDimensions.y / 2.0f) - (collidableEntity->mColliderDimensions.y / 2.0f));


            float xDistance = fabs(mPosition.x - collidableEntity->mPosition.x);
            float xOverlap  = fabs(xDistance - (mColliderDimensions.x / 2.0f) - (collidableEntity->mColliderDimensions.x / 2.0f));

            if (mVelocity.x > 0) {
                mPosition.x     -= (3*xOverlap)/4;
                mVelocity.x      = 0;

                // Collision!
                mIsCollidingRight = true;
            } else if (mVelocity.x < 0) {
                mPosition.x    += (3*xOverlap)/4;
                mVelocity.x     = 0;
 
                // Collision!
                mIsCollidingLeft = true;
            }
            if (xOverlap > 0){
                return true;
            }
            return false;
        }
}


/**
 * Checks if two entities are colliding based on their positions and collider 
 * dimensions.
 * 
 * @param other represents another Entity with which you want to check for 
 * collision. It is a pointer to the Entity class.
 * 
 * @return returns `true` if the two entities are colliding based on their
 * positions and collider dimensions, and `false` otherwise.
 */
bool Entity::isColliding(Entity *other) const 
{
    if (!other->isActive() || other == this) return false;

    float xDistance = fabs(mPosition.x - other->getPosition().x) - 
        ((mColliderDimensions.x + other->getColliderDimensions().x) / 2.0f);
    float yDistance = fabs(mPosition.y - other->getPosition().y) - 
        ((mColliderDimensions.y + other->getColliderDimensions().y) / 2.0f);

    if (xDistance < 0.0f && yDistance < 0.0f) return true;

    return false;
}

/**
 * Updates the current frame index of an entity's animation based on the 
 * elapsed time and frame speed.
 * 
 * @param deltaTime represents the time elapsed since the last frame update.
 */
void Entity::animate(float deltaTime)
{
    mAnimationIndices = mAnimationAtlas.at(mDirection);

    mAnimationTime += deltaTime;
    float framesPerSecond = 1.0f / mFrameSpeed;

    if (mAnimationTime >= framesPerSecond)
    {
        mAnimationTime = 0.0f;

        mCurrentFrameIndex++;
        mCurrentFrameIndex %= mAnimationIndices.size();
    }
}


void Entity::update(float deltaTime,Entity *Platform,
    Entity* Pad1,Entity* Pad2,Entity* Pad3, GameState &gamestate)
{
    if (mEntityStatus == INACTIVE) return;

    resetColliderFlags();

    if (mEntityType == PLAYER){

        const float Damping = 0.99982; 

        mVelocity.x += mAcceleration.x * deltaTime;
        mVelocity.y += mAcceleration.y * deltaTime;

        mPosition.x += mVelocity.x * deltaTime;
        mPosition.y += mVelocity.y * deltaTime;

        mVelocity.x *= Damping;
        mVelocity.y *= Damping;

        if (checkCollisionY(Pad1) || checkCollisionY(Pad2) || checkCollisionY(Pad3)){
            if (checkCollisionX(Pad1) || checkCollisionX(Pad2) || checkCollisionX(Pad3)){
                gamestate = MISSION_ACCOMPLISHED;
            }else{gamestate = MISSION_FAILED;}
        }else if (isColliding(Platform)){
            gamestate = MISSION_FAILED;
        }else if (mPosition.y > std::max({Pad1->mPosition.y, Pad2->mPosition.y, Pad3->mPosition.y})) {
            gamestate = MISSION_FAILED;
        }else if(mPosition.x<0 || mPosition.x>1000 || mPosition.y < 0 || mPosition.y > 600){
            gamestate = MISSION_FAILED;
        }
    }else if (mEntityType == PLATFORM){
        if (mVelocity.x == 0){
            mVelocity.x = 5.0f;
        }
        if (gamestate == PLAYING){

            mPosition.x += mVelocity.x * deltaTime;

            if (mPosition.x + mScale.x/2 > 1000) {
                mVelocity.x = -mVelocity.x; 
            } else if (mPosition.x - mScale.x/2 < 0) {
                mVelocity.x = -mVelocity.x; 
            }
        }
    }
}

void Entity::render()
{
    if(mEntityStatus == INACTIVE) return;

    Rectangle textureArea;

    switch (mTextureType)
    {
        case SINGLE:
            // Whole texture (UV coordinates)
            textureArea = {
                // top-left corner
                0.0f, 0.0f,

                // bottom-right corner (of texture)
                static_cast<float>(mTexture.width),
                static_cast<float>(mTexture.height)
            };
            break;
        case ATLAS:
            textureArea = getUVRectangle(
                &mTexture, 
                mAnimationIndices[mCurrentFrameIndex], 
                mSpriteSheetDimensions.x, 
                mSpriteSheetDimensions.y
            );
        
        default: break;
    }

    // Destination rectangle – centred on gPosition
    Rectangle destinationArea = {
        mPosition.x,
        mPosition.y,
        static_cast<float>(mScale.x),
        static_cast<float>(mScale.y)
    };

    // Origin inside the source texture (centre of the texture)
    Vector2 originOffset = {
        static_cast<float>(mScale.x) / 2.0f,
        static_cast<float>(mScale.y) / 2.0f
    };

    // Render the texture on screen
    DrawTexturePro(
        mTexture, 
        textureArea, destinationArea, originOffset,
        mAngle, WHITE
    );


}
