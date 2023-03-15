#include "raylib.h"

struct AnimationData
{
    Rectangle rect;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnTheGround(AnimationData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rect.height;
}

float movingArea(Texture2D texture, float xGround, int pixels, float deltaTime)
{
    xGround -= pixels * deltaTime;//to move 80 pixels per second the foreground

    if(xGround <= -texture.width * 2){ // 2 because we scale 2.0 the texture 
        xGround = 0.0;
    }

    return xGround;
}

AnimationData updateAnimationData(AnimationData data, float deltaTime, int maxFrame)
{
    data.runningTime += deltaTime;

    if(data.runningTime >= data.updateTime){
        data.runningTime = 0.0;
        //Update animation frame to move the rectangle to the next square of the sheet
        data.rect.x = data.frame * data.rect.width;
        data.frame++;
        if (data.frame > maxFrame)
        {
            data.frame = 0;// to maintain the loop in maxFrames frames cause there are n  sprites in the sheet
        }        
    }

    return data;
}



int main()
{
    int windowDimensions[2];
    windowDimensions[0] = 512;
    windowDimensions[1] = 340;    

    InitWindow(windowDimensions[0], windowDimensions[1], "Dapper Dasher");
    SetTargetFPS(60);    
    
    //acceleration pixels per second
    const int gravity{1000};

    // pixels per second
    const int jumpVel{-600};
    
    // SPRITE: 2D images
    //to load the sprite
    Texture2D character = LoadTexture("textures/scarfy.png");
    AnimationData characterData;
    //rect
    characterData.rect.width = character.width/6;
    characterData.rect.height = character.height;
    characterData.rect.x = 0;
    characterData.rect.y = 0;
    //pos
    characterData.pos.x = windowDimensions[0]/2 - characterData.rect.width/2;
    characterData.pos.y = windowDimensions[1] - characterData.rect.height;
    //other properties
    characterData.frame = 0;
    characterData.updateTime = 1.0/12.0;
    characterData.runningTime = 0.0;    
    

    //Hazards variables
    Texture2D hazard = LoadTexture("textures/12_nebula_spritesheet.png");
    

    const int numberOfHazards = 3;
    AnimationData hazards[numberOfHazards]{};

    for(int i = 0; i < numberOfHazards;i++){
        hazards[i].rect.x = 0.0;
        hazards[i].rect.y = 0.0;
        hazards[i].rect.width = hazard.width/8;
        hazards[i].rect.height = hazard.height/8;
        hazards[i].pos.y = windowDimensions[1] - hazard.height/8;
        hazards[i].pos.x = windowDimensions[0] + (i*300);
        hazards[i].frame = 0;
        hazards[i].runningTime = 0.0;
        hazards[i].updateTime = 1.0/16.0;
    }

    float finishLine{ hazards[numberOfHazards - 1].pos.x };
    bool isInTheAir{};
    int velocity{0}; //pixels per frame
    int hazard_vel{-200}; // velocity of the hazard (pixels/second)    
    
    Texture2D background = LoadTexture("textures/far-buildings.png");
    Texture2D foreground = LoadTexture("textures/foreground.png");
    Texture2D midground = LoadTexture("textures/back-buildings.png");

    float bgX{};
    float mgX{};
    float fgX{};
    bool collision{};

    while (!WindowShouldClose())
    {
        const float dT = GetFrameTime();

        BeginDrawing();
        ClearBackground(WHITE);

        /*bgX -= 20 * dT;//to move 20 pixels per second the background
        mgX -= 40 * dT;//to move 40 pixels per second the midround
        fgX -= 80 * dT;//to move 80 pixels per second the foreground

        if(bgX <= -background.width*2){
            bgX = 0.0;
        }*/

        bgX = movingArea(background, bgX, 20, dT);
        mgX = movingArea(midground, mgX, 40, dT);
        fgX = movingArea(foreground, fgX, 80, dT);

        //Draw background
        Vector2 bgPos1{bgX,0.0};    
        DrawTextureEx(background, bgPos1, 0.0, 2.0, WHITE);
        Vector2 bgPos2{bgX + background.width*2, 0.0};
        DrawTextureEx(background, bgPos2, 0.0, 2.0, WHITE);

        //Draw midground
        Vector2 mgPos1{mgX,0.0};    
        DrawTextureEx(midground, mgPos1, 0.0, 2.0, WHITE);
        Vector2 mgPos2{mgX + background.width*2, 0.0};
        DrawTextureEx(midground, mgPos2, 0.0, 2.0, WHITE);

        //Draw foreground
        Vector2 fgPos1{fgX, 0.0};    
        DrawTextureEx(foreground, fgPos1, 0.0, 2.0, WHITE);
        Vector2 fgPos2{fgX + foreground.width*2, 0.0};
        DrawTextureEx(foreground, fgPos2, 0.0, 2.0, WHITE);

        if(isOnTheGround(characterData, windowDimensions[1])){ // checking wheter character rect is on the ground
            velocity = 0;
            isInTheAir = false;
        } else {
            velocity += gravity * dT;
            isInTheAir = true;
        }

        if(IsKeyPressed(KEY_SPACE) && !isInTheAir){
            velocity += jumpVel;
        }


        for(int i = 0; i < numberOfHazards; i++){
            //update hazard position
            hazards[i].pos.x += hazard_vel * dT;
        }

        // moving the finishline every frame
        finishLine += hazard_vel * dT;

        //updating character position
        characterData.pos.y += velocity * dT;

        //updatin character animation frame
        if(!isInTheAir){            
            characterData = updateAnimationData(characterData, dT, 5);
        }

        //HAZARDS
        for(int i = 0; i < numberOfHazards; i++){
            hazards[i] = updateAnimationData(hazards[i], dT, 7);
        }
        

        for(AnimationData data : hazards)
        {
            float pad{50};

            Rectangle hazardRec{
                data.pos.x + pad,
                data.pos.y + pad,
                data.rect.width - 2*pad,
                data.rect.height - 2*pad
            };

            Rectangle characterRec{
                characterData.pos.x,
                characterData.pos.y,
                characterData.rect.width,
                characterData.rect.height
            };

            if (CheckCollisionRecs(hazardRec, characterRec))
            {
                collision = true;
            }
            
        }
        
        if (collision){ // if there is collision the texture must not draw
            DrawText("GAME OVER DOG!", windowDimensions[0]/4, windowDimensions[1]/2, 40, WHITE);                
        }else if(characterData.pos.x >= finishLine){
            DrawText("U WIN!", windowDimensions[0]/4, windowDimensions[1]/2, 40, WHITE);
        } else {
            for(int i = 0; i < numberOfHazards; i++){
                //Drawing hazard
                DrawTextureRec(hazard, hazards[i].rect, hazards[i].pos, WHITE);
            }
            //Draw Character
            DrawTextureRec(character, characterData.rect, characterData.pos, WHITE);
        }
        
        
        // CODE GOES HERE
        EndDrawing();
    }

    UnloadTexture(character); // TO UNLOAD THE TEXTURE2D object
    UnloadTexture(hazard);
    UnloadTexture(background);
    UnloadTexture(foreground);
    UnloadTexture(midground);

    CloseWindow();
}

