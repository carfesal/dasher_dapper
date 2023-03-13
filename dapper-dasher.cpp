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
    int windowDimensions[2];
    windowDimensions[0] = 750;
    windowDimensions[1] = 750;    

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

    
    AnimationData hazards[3]{};

    for(int i = 0; i < 3;i++){
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

    bool isInTheAir{};
    int velocity{0}; //pixels per frame
    int hazard_vel{-200}; // velocity of the hazard (pixels/second)    
    

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        float dT = GetFrameTime();

        if(characterData.pos.y >=  windowDimensions[1] - characterData.rect.height){ // checking wheter character rect is on the ground
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
        hazards[0].pos.x += hazard_vel * dT;

        //update 2ND hazard position
        hazards[1].pos.x += hazard_vel * dT;
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
        hazards[0].runningTime += dT;
        if(hazards[0].runningTime >= hazards[0].updateTime){
            hazards[0].runningTime = 0;
            hazards[0].rect.x = hazards[0].frame * hazards[0].rect.width;
            hazards[0].frame++;

            if(hazards[0].frame > 7){
                hazards[0].frame = 0;
            }
        }

        //update hazard animation frames        
        hazards[1].runningTime += dT;
        if(hazards[1].runningTime >= hazards[1].updateTime){
            hazards[1].runningTime = 0;
            hazards[1].rect.x = hazards[1].frame * hazards[1].rect.width;
            hazards[1].frame++;

            if(hazards[1].frame > 7){
                hazards[1].frame = 0;
            }
        }

        for(int i = 0; i < sizeof(hazards); i++){
            //update hazard animation frames        
            hazards[i].runningTime += dT;
            if(hazards[i].runningTime >= hazards[i].updateTime){
                hazards[i].runningTime = 0;
                hazards[i].rect.x = hazards[i].frame * hazards[i].rect.width;
                hazards[i].frame++;

                if(hazards[i].frame > 7){
                    hazards[i].frame = 0;
                }
            }
        }

        //Drawing hazard
        DrawTextureRec(hazard, hazards[0].rect, hazards[0].pos, WHITE);

        //Drawing 2nd hazard
        DrawTextureRec(hazard, hazards[1].rect, hazards[1].pos, BLACK);

        //Draw Character
        DrawTextureRec(character, characterData.rect, characterData.pos, WHITE);
        
        // CODE GOES HERE
        EndDrawing();
    }

    UnloadTexture(character); // TO UNLOAD THE TEXTURE2D object
    UnloadTexture(hazard);
    CloseWindow();
}

