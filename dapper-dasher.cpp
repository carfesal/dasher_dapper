#include "raylib.h"

struct AnimationData
{
    Rectangle rect;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};


int main()
{
    const int width = 750;
    const int height = 750;

    InitWindow(width, height, "Dapper Dasher");
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
    characterData.pos.x = width/2 - characterData.rect.width/2;
    characterData.pos.y = height - characterData.rect.height;
    //other properties
    characterData.frame = 0;
    characterData.updateTime = 1.0/12.0;
    characterData.runningTime = 0.0;    
    

    //Hazards variables
    Texture2D hazard = LoadTexture("textures/12_nebula_spritesheet.png");

    //Anim Data for 1st hazard 
    AnimationData hazardData{
        {0.0, 0.0, hazard.width/8, hazard.height/8},
        {width + 300, height - hazard.height/8},
        0,
        1.0/12.0,
        0.0
    };

    AnimationData hazardData2{
        {0.0, 0.0, hazard.width/8, hazard.height/8}, //rect Rectangle
        {width, height - hazard.height/8}, // pos   Vector2
        0, // frame int
        1.0/16.0, // updateTime float
        0.0 //runningTime float
    };
    
    bool isInTheAir{};
    int velocity{0}; //pixels per frame
    int hazard_vel{-200}; // velocity of the hazard (pixels/second)    
    

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        float dT = GetFrameTime();

        if(characterData.pos.y >= height - characterData.rect.height){ // checking wheter rect is on the ground
            velocity = 0;
            isInTheAir = false;
        } else {
            velocity += gravity * dT;
            isInTheAir = true;
        }

        if(IsKeyPressed(KEY_SPACE) && !isInTheAir){
            velocity += jumpVel;
        }

        //update hazard position
        hazardData.pos.x += hazard_vel * dT;

        //update 2ND hazard position
        hazardData2.pos.x += hazard_vel * dT;
        //updating character position
        characterData.pos.y += velocity * dT;

        //Updating character animation frames
        characterData.runningTime += dT;
        if(!isInTheAir){
            if (characterData.runningTime >= characterData.updateTime)
            {
                characterData.runningTime = 0.0;
                //Update animation frame to move the rectangle to the next square of the sheet
                characterData.rect.x = characterData.frame * characterData.rect.width;
                characterData.frame++;

                if(characterData.frame > 5){
                    characterData.frame = 0; // to maintain the loop in 6 frames cause there are 6 sprites in the sheet
                }
            }
        }
        
        //update hazard animation frames        
        hazardData.runningTime += dT;
        if(hazardData.runningTime >= hazardData.updateTime){
            hazardData.runningTime = 0;
            hazardData.rect.x = hazardData.frame * hazardData.rect.width;
            hazardData.frame++;

            if(hazardData.frame > 7){
                hazardData.frame = 0;
            }
        }

        //update hazard animation frames        
        hazardData2.runningTime += dT;
        if(hazardData2.runningTime >= hazardData2.updateTime){
            hazardData2.runningTime = 0;
            hazardData2.rect.x = hazardData2.frame * hazardData2.rect.width;
            hazardData2.frame++;

            if(hazardData2.frame > 7){
                hazardData2.frame = 0;
            }
        }

        //Drawing hazard
        DrawTextureRec(hazard, hazardData.rect, hazardData.pos, WHITE);

        //Drawing 2nd hazard
        DrawTextureRec(hazard, hazardData2.rect, hazardData2.pos, BLACK);

        //Draw Character
        DrawTextureRec(character, characterData.rect, characterData.pos, WHITE);
        
        // CODE GOES HERE
        EndDrawing();
    }

    UnloadTexture(character); // TO UNLOAD THE TEXTURE2D object
    UnloadTexture(hazard);
    CloseWindow();
}

